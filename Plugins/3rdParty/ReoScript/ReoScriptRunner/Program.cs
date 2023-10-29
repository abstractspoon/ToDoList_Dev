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
using System.Text;
using System.IO;

using unvell.ReoScript.Extensions;
using unvell.ReoScript.Diagnostics;

namespace unvell.ReoScript
{
	class ConsoleRunnerProgram
	{
		static void Main(string[] args)
		{
			if (args.Length == 0)
			{
				Console.WriteLine(
@"ReoScript(TM) Running Machine
Copyright(c) 2012-2013 unvell, All Rights Reserved.

Usage: ReoScript.exe [filename|-workpath|-debug|-exec|-console]");
				return;
			}


			List<string> files = new List<string>();
			string workPath = null;
			bool debug = false;
			string initScript = null;

			bool consoleMode = false;
			bool compileMode = false;

			try
			{
				for (int i = 0; i < args.Length; i++)
				{
					string arg = args[i];

					if (arg.StartsWith("-"))
					{
						string param = arg.Substring(1);

						switch (param)
						{
							case "workpath":
								workPath = GetParameter(args, i);
								i++;
								break;

							case "debug":
								debug = true;
								break;

							case "exec":
								initScript = GetParameter(args, i);
								i++;
								break;

							case "com":
								compileMode = true;
								break;

							case "console":
								consoleMode = true;
								break;
						}
					}
					else
					{
						files.Add(arg);
					}
				}
			}
			catch (Exception ex)
			{
				OutLn(ex.Message);
				return;
			}

			List<FileInfo> sourceFiles = new List<FileInfo>();

			foreach (string file in files)
			{
				FileInfo fi = new FileInfo(string.IsNullOrEmpty(workPath)
					? file : Path.Combine(workPath, file));

				if (!fi.Exists)
				{
					Console.WriteLine("Resource not found: " + fi.FullName);
				}
				else
				{
					sourceFiles.Add(fi);

					if (string.IsNullOrEmpty(workPath))
					{
						workPath = fi.DirectoryName;
					}
				}
			}

			if (string.IsNullOrEmpty(workPath))
			{
				workPath = Environment.CurrentDirectory;
			}

			// for test!
			if (compileMode)
			{
			  using (StreamReader sr = new StreamReader(new FileStream(args[0], FileMode.Open, FileAccess.Read, FileShare.Read)))
			  {
			    Console.WriteLine(unvell.ReoScript.Compiler.ReoScriptCompiler.Run(sr.ReadToEnd()));
			  }
			  return;
			}

			// create SRM
			ScriptRunningMachine srm = new ScriptRunningMachine(CoreFeatures.FullFeatures);
			if (debug)
			{
				new ScriptDebugger(srm);
			}

			// set to full work mode
			srm.WorkMode |= MachineWorkMode.AllowImportTypeInScript
				| MachineWorkMode.AllowCLREventBind
				| MachineWorkMode.AllowDirectAccess;

			// change work path
			srm.WorkPath = workPath;

			// add built-in output listener
			srm.AddStdOutputListener(new BuiltinConsoleOutputListener());

			// not finished yet!
			//srm.SetGlobalVariable("File", new FileConstructorFunction());

			try
			{
				foreach (FileInfo file in sourceFiles)
				{
					// load script file
					srm.Run(file);
				}

				if (!string.IsNullOrEmpty(initScript))
				{
					srm.Run(initScript);
				}
			}
			catch (ReoScriptException ex)
			{
				string str = string.Empty;
				
				if (ex.ErrorObject == null)
				{
					str = ex.ToString();
				}
				else if (ex.ErrorObject is ErrorObject)
				{
					ErrorObject e = (ErrorObject)ex.ErrorObject;

					str += "Error: " + e.GetFullErrorInfo();
				}
				else
				{
					str += Convert.ToString(ex.ErrorObject);
				}

				Console.WriteLine(str);
			}

			if (consoleMode)
			{
				OutLn("\nReady.\n");

				bool isQuitRequired = false;

				while (!isQuitRequired)
				{
					Prompt();

					string line = In().Trim();
					if (line == null)
					{
						isQuitRequired = true;
						break;
					}
					else if (line.StartsWith("."))
					{
						srm.Load(line.Substring(1, line.Length - 1));
					}
					else if (line.StartsWith("/"))
					{
						string consoleCmd = line.Substring(1);

						switch (consoleCmd)
						{
							case "q":
							case "quit":
							case "exit":
								isQuitRequired = true;
								break;
							case "h":
							case "help":
								Help();
								break;
							default:
								break;
						}
					}
					else if (line.Equals("?"))
					{
						ObjectValue obj = srm.DefaultContext.ThisObject as ObjectValue;
						if (obj != null) OutLn(obj.DumpObject());
					}
					else if (line.StartsWith("?"))
					{
						string expression = line.Substring(1);
						try
						{
							object value = srm.CalcExpression(expression);
							if (value is ObjectValue)
							{
								OutLn(((ObjectValue)value).DumpObject());
							}
							else
							{
								OutLn(ScriptRunningMachine.ConvertToString(value));
							}
						}
						catch (Exception ex)
						{
							OutLn("error: " + ex.Message);
						}
					}
					else if (line.Length == 0)
					{
						continue;
					}
					else
					{
						try
						{
							srm.Run(line);
						}
						catch (ReoScriptException ex)
						{
							Console.WriteLine("error: " + ex.Message + "\n");
						}
					}
				}

				OutLn("Bye.");
			}
		}

		private static string GetParameter(string[] args, int i)
		{
			if (args.Length <= i + 1 || args[i + 1].StartsWith("-"))
			{
				throw new ArgumentException("Missing option parameters: " + args[i]);
			}
			else
				return args[i + 1];
		}

		private static void Help()
		{
			OutLn(@"
ReoScript Console Help

/<system command>       submit system command.
  quit | q              quit from console.
  help | h              show this topic.

?[experssion]           calculate an expression and output result.
                        if the expression is null, list all varaibles 
												in current global object.

<statement>;						run script statement.
");
		}

		private static void Prompt()
		{
			Out(">");
		}
		private static void Out(string msg)
		{
			Console.Write(msg);
		}
		private static void OutLn()
		{
			Console.WriteLine();
		}
		private static void OutLn(string msg)
		{
			Console.WriteLine(msg);
		}
		private static string In()
		{
			return Console.ReadLine();
		}
	}
}
