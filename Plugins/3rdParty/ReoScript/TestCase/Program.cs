/*****************************************************************************
 * 
 * ReoScript - .NET Script Language Engine
 * 
 * https://github.com/unvell/ReoScript
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * This software released under MIT license.
 * Copyright (c) 2012-2019 Jingwood, unvell.com, all rights reserved.
 * 
 ****************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Xml.Serialization;
using System.Diagnostics;
using System.Reflection;
using unvell.ReoScript.Diagnostics;

namespace unvell.ReoScript.TestCase
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static int Main(string[] args)
		{

			List<string> ids = new List<string>();
			List<string> enabledTags = new List<string>();

			foreach (string a in args)
			{
				if (a.StartsWith("-et"))
				{
					enabledTags.Add(a.Substring(3, a.Length - 3).ToLower());
				}
				else
				{
					ids.Add(args[0]);
				}
			}

			TestCaseRunner runner = new TestCaseRunner();

			bool hasErrors = runner.RunLanguageTests(ids, enabledTags);
			hasErrors |= runner.RunCLRTests();

			//using (ReoScriptEditor editor = new ReoScriptEditor())
			//{
			//  using (StreamReader sr = new StreamReader(new FileStream("scripts/winform.rs", FileMode.Open)))
			//  {
			//    editor.Srm.WorkMode |= MachineWorkMode.AllowDirectAccess 
			//      | MachineWorkMode.AllowImportTypeInScript | MachineWorkMode.AllowCLREventBind;

			//    editor.Script = sr.ReadToEnd();
			//  }

			//  Application.Run(editor);
			//}

			Console.WriteLine("Total result: \n");
			Console.WriteLine("    {0,3} test cases, {1,3} successed, {2,3} failed, {3,3} skipped",
				runner.TotalCases, runner.TotalSuccesses, runner.TotalFailures,
				(runner.TotalCases - runner.TotalSuccesses - runner.TotalFailures));
			Console.WriteLine("  {0,5} objects created.\n", runner.TotalObjectCreates);

			return hasErrors ? 1 : 0;
		}
	}

	class TestCaseFailureException : Exception
	{
		public TestCaseFailureException(string msg) :
			base(msg) { }
	}

	class TestCaseRunner
	{
		public TestCaseRunner()
		{
		}

		public int TotalCases { get; set; }
		public int TotalSuccesses { get; set; }
		public int TotalFailures { get; set; }
		public int TotalObjectCreates { get; set; }

		private static readonly XmlSerializer xmlSuiteSerializer = new XmlSerializer(typeof(XmlTestSuite));

		public bool RunLanguageTests(List<string> filter, List<string> enabledTags)
		{
			Console.WriteLine("Run Core tests...\n");

			bool hasErrors = false;

			ScriptRunningMachine srm = new ScriptRunningMachine();
			ScriptDebugger debugMonitor = new ScriptDebugger(srm);

			int testCases = 0, success = 0, failed = 0;
			int createdObjs = 0;

			foreach (string filename in Directory.GetFiles("tests"))
			{
				if (filter.Count > 0 && !filter.Any(f => filename.Contains(f))) continue;

				XmlTestSuite suite = xmlSuiteSerializer.Deserialize(File.OpenRead(filename)) as XmlTestSuite;

				if (suite != null)
				{
					testCases += suite.TestCases.Count;
				}

				if (!string.IsNullOrEmpty(suite.Tag))
				{
					string[] tags = suite.Tag.ToLower().Split(' ');

					if (!enabledTags.Any(t => tags.Contains(t)))
						continue;
				}

				Stopwatch sw = Stopwatch.StartNew();

				suite.TestCases.ForEach(t =>
				{
					string caseId = string.Format("{0,3}-{1,3}", suite.Id, t.Id);

					var testCode = t.Script;
					if (string.IsNullOrEmpty(testCode))
					{
						testCode = t.TestCode;
					}

					if (t.Disabled || string.IsNullOrEmpty(testCode)
						|| (filter.Count > 0 && !filter.Any(id => caseId.Contains(id))))
						return;

					srm.Reset();

					Console.Write("[{0,6} {1,-10}] {2,-30} : ", caseId, suite.Name, t.Name);

					try
					{
						sw.Reset();
						sw.Start();

						srm.Run(testCode);

						sw.Stop();

						success++;

						Console.WriteLine("{0,5} ms. {1,4} objs.", sw.ElapsedMilliseconds,
							debugMonitor.TotalObjectCreated - createdObjs);

						createdObjs = debugMonitor.TotalObjectCreated;
					}
					catch (Exception ex)
					{
						failed++;
						Console.WriteLine(string.IsNullOrEmpty(ex.Message) ? "failed"
							: ex.Message);

						hasErrors = true;
					}
					finally
					{
						sw.Stop();
					}
				});

				//Console.WriteLine();
			}

			Console.WriteLine("\n    {0,3} test cases, {1,3} successed, {2,3} failed, {3,3} skipped",
				testCases, success, failed, (testCases - success - failed));
			Console.WriteLine("  {0,5} objects created.\n", debugMonitor.TotalObjectCreated);

			TotalCases += testCases;
			TotalFailures += failed;
			TotalSuccesses += success;
			TotalObjectCreates += debugMonitor.TotalObjectCreated;

			return hasErrors;
		}

		public bool RunCLRTests()
		{
			Console.WriteLine("Run CLR tests...\n");

			ScriptRunningMachine srm = new ScriptRunningMachine();
			srm.WorkMode |= MachineWorkMode.AllowDirectAccess;

			ScriptDebugger debugMonitor = new ScriptDebugger(srm);

			int testCases = 0, success = 0, failed = 0;
			int createdObjs = 0;

			Stopwatch sw = Stopwatch.StartNew();

			bool hasErrors = false;

			foreach (Type type in this.GetType().Assembly.GetTypes())
			{
				TestSuiteAttribute[] attrs = type.GetCustomAttributes(typeof(TestSuiteAttribute), true) as
					TestSuiteAttribute[];

				if (attrs.Length > 0)
				{
					var testName = attrs[0].Name;
					if (string.IsNullOrEmpty(testName)) testName = type.Name;

					object testSuite = System.Activator.CreateInstance(type);

					foreach (MethodInfo method in type.GetMethods())
					{
						TestCaseAttribute[] caseAttrs = method.GetCustomAttributes(typeof(TestCaseAttribute), false)
							as TestCaseAttribute[];

						if (caseAttrs.Length > 0 && !caseAttrs[0].Disabled)
						{
							testCases++;

							var caseName = caseAttrs[0].Desc;
							if (string.IsNullOrEmpty(caseName)) caseName = method.Name;

							Console.Write("[{0,-18}] {1,-30} : ", testName, caseName);

							if (testSuite is ReoScriptTestSuite)
							{
								srm.WorkMode = caseAttrs[0].WorkMode;
								srm.Reset();
								((ReoScriptTestSuite)testSuite).SRM = srm;
							}

							try
							{
								sw.Reset();
								sw.Start();

								method.Invoke(testSuite, null);

								sw.Stop();
								success++;

								Console.WriteLine("{0,5} ms. {1,4} objs.", sw.ElapsedMilliseconds,
									debugMonitor.TotalObjectCreated - createdObjs);

								createdObjs = debugMonitor.TotalObjectCreated;
							}
							catch (Exception ex)
							{
								failed++;
								Console.WriteLine(string.IsNullOrEmpty(ex.InnerException.Message) ? "failed" : ex.InnerException.Message);

								hasErrors = true;
							}
							finally
							{
								sw.Stop();
							}
						}
					}
				}
			}

			Console.WriteLine("\n    {0,3} test cases, {1,3} successed, {2,3} failed, {3,3} skipped",
				testCases, success, failed, (testCases - success - failed));
			Console.WriteLine("  {0,5} objects created.\n", debugMonitor.TotalObjectCreated);

			TotalCases += testCases;
			TotalFailures += failed;
			TotalSuccesses += success;
			TotalObjectCreates += debugMonitor.TotalObjectCreated;

			return hasErrors;
		}
	}

	[XmlRoot("test-suite")]
	public class XmlTestSuite
	{
		[XmlAttribute("id")]
		public string Id { get; set; }

		[XmlAttribute("name")]
		public string Name { get; set; }

		[XmlAttribute("tag")]
		public string Tag { get; set; }

		[XmlElement("test-case")]
		public List<XmlTestCase> TestCases { get; set; }
	}

	public class XmlTestCase
	{
		[XmlAttribute("id")]
		public string Id { get; set; }

		[XmlAttribute("name")]
		public string Name { get; set; }

		[XmlElement("script")]
		public string Script { get; set; }

		[XmlText]
		public string TestCode { get; set; }

		[XmlAttribute("disabled")]
		public bool Disabled { get; set; }
	}

	public class TestSuiteAttribute : Attribute
	{
		public string Name { get; set; }

		public TestSuiteAttribute() { }
		public TestSuiteAttribute(string name) : base() { this.Name = name; }
	}

	public class TestCaseAttribute : Attribute
	{
		public string Desc { get; set; }

		public bool Disabled { get; set; }

		public MachineWorkMode WorkMode { get; set; }

		public TestCaseAttribute()
		{
			WorkMode = MachineWorkMode.Default | MachineWorkMode.AllowDirectAccess
				| MachineWorkMode.AllowCLREventBind | MachineWorkMode.AllowImportTypeInScript | MachineWorkMode.AutoImportRelationType
				| MachineWorkMode.AutoUppercaseWhenCLRCalling | MachineWorkMode.IgnoreCLRExceptions;
		}

		public TestCaseAttribute(string desc) : base() { this.Desc = desc; }
	}
}
