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
using System.IO;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Reflection;
using System.Collections;
using System.Net;
using System.Windows.Forms;
using System.ComponentModel;
using System.Threading;
using System.Collections.Generic;
using System.Reflection.Emit;

using Antlr.Runtime;
using Antlr.Runtime.Tree;

using unvell.ReoScript.Properties;
using unvell.ReoScript.Runtime;
using unvell.ReoScript.Parsers;
using unvell.ReoScript.Reflection;

namespace unvell.ReoScript
{
	#region Lexer & Parser
	sealed internal partial class ReoScriptLexer
	{
		public static readonly int HIDDEN = Hidden;

		public const int MAX_TOKENS = 200;
		public const int REPLACED_TREE = MAX_TOKENS - 1;
	}

	sealed internal partial class ReoScriptParser
	{
		//internal Preinterpreter Preinterpreter { get; set; }

		private CommonTree ConstLiteral(CommonTree t)
		{
			switch (t.Type)
			{
				case ReoScriptLexer.NUMBER_LITERATE:
					return new ConstValueNode(t, Convert.ToDouble(t.Text, System.Globalization.CultureInfo.InvariantCulture), ReoScriptLexer.NUMBER_LITERATE);

				case ReoScriptLexer.HEX_LITERATE:
					return new ConstValueNode(t, (double)Convert.ToInt32(t.Text.Substring(2), 16), ReoScriptLexer.NUMBER_LITERATE);

				case ReoScriptLexer.BINARY_LITERATE:
					return new ConstValueNode(t, (double)Convert.ToInt32(t.Text.Substring(2), 2), ReoScriptLexer.NUMBER_LITERATE);

				case ReoScriptLexer.STRING_LITERATE:
					string str = t.ToString();
					str = str.Substring(1, str.Length - 2);
					return new ConstValueNode(t, ScriptRunningMachine.ConvertEscapeLiterals(str), ReoScriptLexer.STRING_LITERATE);

				//case ReoScriptLexer.LIT_TRUE:
				//  return new ConstValueNode(t, true);

				//case ReoScriptLexer.LIT_FALSE:
				//  return new ConstValueNode(t, false);

				//case ReoScriptLexer.LIT_NULL:
				//case ReoScriptLexer.UNDEFINED:
				//  return new ConstValueNode(t, null);

				//case ReoScriptLexer.NAN:
				//  return new ConstValueNode(t, NaNValue.Value);
			}

			return t;
		}

		internal Action<ErrorObject> CompilingErrorHandler;

		internal List<ErrorObject> CompilingErrors = new List<ErrorObject>();

		public override void ReportError(RecognitionException re)
		{
			string msg = string.Format("syntax error at char {0} on line {1}", re.CharPositionInLine, re.Line);

			if (re is MissingTokenException)
			{
				MissingTokenException mte = (MissingTokenException)re;
				msg += string.Format(", missing {0}", ReoScriptParser.tokenNames[mte.MissingType]);
			}
			else if (re is MismatchedTokenException)
			{
				MismatchedTokenException mte = (MismatchedTokenException)re;
				msg += string.Format(", expect {0}", ReoScriptParser.tokenNames[mte.Expecting]);
			}
			else if (re is NoViableAltException)
			{
				NoViableAltException nvae = (NoViableAltException)re;
				msg += ", unexpected token " + nvae.Token.Text;
			}

			ErrorObject e = new ErrorObject();
			e.Message = msg;
			e.CharIndex = re.CharPositionInLine;
			e.Line = re.Line;

			CompilingErrors.Add(e);

			if (CompilingErrorHandler != null)
			{
				CompilingErrorHandler(e);
			}
		}

		private Stack<StaticFunctionScope> localStack = new Stack<StaticFunctionScope>();

		internal StaticFunctionScope CurrentStack { get; set; }

		private void PushLocalStack()
		{
			localStack.Push(CurrentStack = new StaticFunctionScope());
		}

		private StaticFunctionScope lastLocalScope;

		private void PopLocalStack()
		{
			if (localStack.Count > 1)
				lastLocalScope = localStack.Pop();

			CurrentStack = localStack.Peek();
		}

		private FunctionDefineNode DefineLocalFunction(string name, CommonTree paramsTree, CommonTree body,
			int modifierToken, int line, int charIndex)
		{
			FunctionInfo fi = new FunctionInfo
			{
				Name = name,
				IsInner = localStack.Count > 1,
				Args = GetArgumentList(paramsTree),
				IsAnonymous = false,
				ScopeModifier = GetScopeModifier(modifierToken),
				BodyTree = body,
				CharIndex = charIndex,
				Line = line,
				InnerScope = lastLocalScope,
				OuterScope = CurrentStack,
			};

			FunctionDefineNode fdn = new FunctionDefineNode
			{
				FunctionInfo = fi,
			};

			CurrentStack.Functions.Add(fi);

			return fdn;
		}

		private AnonymousFunctionDefineNode DefineAnonymousFunction(string arg1, CommonTree argsTree, CommonTree body,
			int modifierToken, int line, int charIndex)
		{
			FunctionInfo fi = new FunctionInfo
			{
				Name = "<anonymous>",
				Args = arg1 == null ? GetArgumentList(argsTree) : new string[] { arg1 },
				IsAnonymous = true,
				ScopeModifier = MemberScopeModifier.Public,
				BodyTree = body,
				CharIndex = charIndex,
				Line = line,
				InnerScope = lastLocalScope,
				OuterScope = CurrentStack,
			};

			AnonymousFunctionDefineNode afdn = new AnonymousFunctionDefineNode
			{
				FunctionInfo = fi,
			};

			if (body.Type != RETURN)
			{
				CurrentStack.Functions.Add(fi);
			}

			return afdn;
		}

		private VariableDefineNode DefineLocalVariable(string name, CommonTree val/*, int modifierToken*/,
			int line, int charIndex)
		{
			VariableInfo vi = new VariableInfo
			{
				Name = name,
				HasInitialValue = val != null,
				IsImplicitDeclaration = false,
				IsLocal = true,
				CharIndex = charIndex,
				Line = line,
				//TODO: ScopeModifier = GetScopeModifier(modifierToken),
				InitialValueTree = val,
			};

			VariableDefineNode vdn = new VariableDefineNode
			{
				VariableInfo = vi
			};

			CurrentStack.Variables.Add(vi);

			return vdn;
		}

		private static string[] GetArgumentList(CommonTree argsTree)
		{
			if (argsTree == null) return new string[0];

			if (argsTree.ChildCount == 0)
			{
				return new string[] { argsTree.Text };
			}
			else
			{
				string[] identifiers = new string[argsTree.ChildCount];

				for (int i = 0; i < identifiers.Length; i++)
				{
					identifiers[i] = argsTree.Children[i].ToString();
				}

				return identifiers;
			}
		}

		private static MemberScopeModifier GetScopeModifier(int tokenType)
		{
			switch (tokenType)
			{
				default:
				case ReoScriptLexer.PUBLIC:
					return MemberScopeModifier.Public;
				case ReoScriptLexer.INTERNAL:
					return MemberScopeModifier.Internal;
				case ReoScriptLexer.PROTECTED:
					return MemberScopeModifier.Protected;
				case ReoScriptLexer.PRIVATE:
					return MemberScopeModifier.Private;
			}
		}
	}

	public class StaticFunctionScope
	{
		internal readonly List<FunctionInfo> Functions;
		internal readonly List<VariableInfo> Variables;

		internal protected StaticFunctionScope()
		{
			Functions = new List<FunctionInfo>();
			Variables = new List<VariableInfo>();
		}
	}

	internal class FunctionDefineNode : CommonTree
	{
		public FunctionInfo FunctionInfo { get; set; }

		public FunctionDefineNode()
			: base(new CommonToken(ReoScriptLexer.FUNCTION_DEFINE))
		{
		}
	}

	internal class AnonymousFunctionDefineNode : CommonTree
	{
		public FunctionInfo FunctionInfo { get; set; }

		public AnonymousFunctionDefineNode()
			: base(new CommonToken(ReoScriptLexer.ANONYMOUS_FUNCTION))
		{
		}
	}

	namespace Reflection
	{
		/// <summary>
		/// ReoScript function information
		/// </summary>
		public class FunctionInfo
		{
			/// <summary>
			/// Function name
			/// </summary>
			public string Name { get; set; }

			/// <summary>
			/// Argument name list
			/// </summary>
			public string[] Args { get; set; }

			/// <summary>
			/// Specifies whether is anonymous function.
			/// </summary>
			public bool IsAnonymous { get; set; }

			/// <summary>
			/// Specifies whether is nested function inside another function.
			/// </summary>
			public bool IsInner { get; set; }

			/// <summary>
			/// Specifies the modifier of function visibility scope.
			/// </summary>
			public MemberScopeModifier ScopeModifier { get; set; }

			/// <summary>
			/// Specifies the char position on line where function is defined.
			/// </summary>
			public int CharIndex { get; set; }

			/// <summary>
			/// Specifies the line number where function is defined.
			/// </summary>
			public int Line { get; set; }

			internal CommonTree BodyTree { get; set; }
			internal StaticFunctionScope InnerScope { get; set; }
			internal StaticFunctionScope OuterScope { get; set; }

			/// <summary>
			/// Get string of function's body.
			/// </summary>
			/// <returns></returns>
			public string GetBodyText()
			{
				return BodyTree.ToString();
			}

			/// <summary>
			/// Get all inner functions defined in this function
			/// </summary>
			public List<FunctionInfo> DeclaredInnerFunctions
			{
				get
				{
					return InnerScope.Functions;
				}
			}

			/// <summary>
			/// Get local variables defined in this function
			/// </summary>
			public List<VariableInfo> DeclaredLocalVariables
			{
				get
				{
					return InnerScope.Variables;
				}
			}
		}

		/// <summary>
		/// ReoScript variable information
		/// </summary>
		public class VariableInfo
		{
			/// <summary>
			/// Variable name
			/// </summary>
			public string Name { get; set; }

			/// <summary>
			/// Specifies whether is local variable.
			/// </summary>
			public bool IsLocal { get; set; }

			/// <summary>
			/// Specifies whether the variable to be used without declaration.
			/// </summary>
			public bool IsImplicitDeclaration { get; set; }

			/// <summary>
			/// Specifies whether the variable has an initialize value.
			/// </summary>
			public bool HasInitialValue { get; set; }

			/// <summary>
			/// Specifies the modifier of variable visibility scope.
			/// </summary>
			public MemberScopeModifier ScopeModifier { get; set; }

			/// <summary>
			/// Specifies the char position on line where variable is defined.
			/// </summary>
			public int CharIndex { get; set; }

			/// <summary>
			/// Specifies the line number where variable is defined.
			/// </summary>
			public int Line { get; set; }

			internal CommonTree InitialValueTree { get; set; }

			/// <summary>
			/// Get string of initial value expression.
			/// </summary>
			/// <returns></returns>
			public string GetInitialValueExpression()
			{
				return InitialValueTree.ToString();
			}
		}
	}

	internal class VariableDefineNode : CommonTree
	{
		public VariableInfo VariableInfo { get; set; }

		public VariableDefineNode()
			: base(new CommonToken(ReoScriptLexer.LOCAL_DECLARE_ASSIGNMENT))
		{
		}
	}

	/// <summary>
	/// Member scope modifier. (RESERVED FEATURE)
	/// </summary>
	public enum MemberScopeModifier
	{
		/// <summary>
		/// Property or method visible to any scope
		/// </summary>
		Public,

		/// <summary>
		/// Property or method visible to other members defined in same file
		/// </summary>
		Internal,

		/// <summary>
		/// Property or method visible to other members belonging to same instance or prototype
		/// </summary>
		Protected,

		/// <summary>
		/// Property or method visible to other members belonging to same instance
		/// </summary>
		Private,
	}
	#endregion

	#region Syntax Tree
	internal interface ISyntaxTreeReturn { }

	#region NaNValue
	/// <summary>
	/// Runtime NaN value.
	/// </summary>
	public sealed class NaNValue : ISyntaxTreeReturn
	{
		public static readonly NaNValue Value = new NaNValue();
		private NaNValue() { }
		public override string ToString()
		{
			return "NaN";
		}
	}

	/// <summary>
	/// Runtime infinity value.
	/// </summary>
	public sealed class InfinityValue : ISyntaxTreeReturn
	{
		public static readonly InfinityValue Value = new InfinityValue();
		private InfinityValue() { }
		public override string ToString()
		{
			return "Infinity";
		}
	}

	/// <summary>
	/// Runtime -infinity value.
	/// </summary>
	public sealed class MinusInfinityValue : ISyntaxTreeReturn
	{
		public static readonly MinusInfinityValue Value = new MinusInfinityValue();
		private MinusInfinityValue() { }
		public override string ToString()
		{
			return "-Infinity";
		}
	}
	#endregion
	#region BreakNode
	class BreakNode : ISyntaxTreeReturn
	{
		public static readonly BreakNode Value = new BreakNode();
		private BreakNode() { }
	}
	#endregion
	#region ContinueNode
	class ContinueNode : ISyntaxTreeReturn
	{
		public static readonly ContinueNode Value = new ContinueNode();
		private ContinueNode() { }
	}
	#endregion
	#region ReturnNode
	class ReturnNode : ISyntaxTreeReturn
	{
		public object Value { get; set; }

		public ReturnNode(object value)
		{
			this.Value = value;
		}
	}
	#endregion

	#endregion

	#region Built-in Objects
	#region Object Value
	/// <summary>
	/// Object instance of ReoScript
	/// </summary>
	public class ObjectValue : ISyntaxTreeReturn, IEnumerable, IVariableContainer
	{
		/// <summary>
		/// Construct an object instance
		/// </summary>
		public ObjectValue()
		{
			Members = new Dictionary<string, object>();
		}

		private Dictionary<string, object> Members { get; set; }

		/// <summary>
		/// Get or set property
		/// </summary>
		/// <param name="identifier"></param>
		/// <returns></returns>
		public virtual object this[string identifier]
		{
			get
			{
				object v;
				return Members.TryGetValue(identifier, out v) ? v : null;
			}
			set
			{
				Members[identifier] = value;
			}
		}

		/// <summary>
		/// Check whether a property exists in object
		/// </summary>
		/// <param name="identifier"></param>
		/// <returns></returns>
		public bool HasOwnProperty(string identifier)
		{
			return Members.ContainsKey(identifier);
		}

		public object GetOwnProperty(string identifier)
		{
			return this[identifier];
		}

		public bool RemoveOwnProperty(string identifier)
		{
			if (Members.ContainsKey(identifier))
			{
				Members.Remove(identifier);
				return true;
			}
			else
			{
				return false;
			}
		}

		public virtual string Name { get { return Constructor == null ? "Object" : Constructor.FunName; } }

		public override string ToString()
		{
			return string.Format("[object {0}]", Name);
			//return DumpObject("Object");
		}

		public string DumpObject()
		{
			if (Members.Count > 0)
			{
				StringBuilder sb = new StringBuilder();
				if (this is ArrayObject || this is StringObject || this is NumberObject || this is BooleanObject)
				{
					sb.AppendLine(string.Format("[object {0}: {1}]", Name, this.ToString()));
				}
				else
				{
					sb.AppendLine(string.Format("[object {0}]", Name));
				}

				foreach (string name in Members.Keys)
				{
					if (name == ScriptRunningMachine.KEY___PROTO__) continue;

					object val = Members[name];

					sb.AppendLine(string.Format("  {0,-20}: {1}", name, (val == null ? "null" : Convert.ToString(val))));
				}

				return sb.ToString();
			}
			else
				return Name;
		}

		#region IEnumerable Members

		public virtual IEnumerator GetEnumerator()
		{
			string[] properties = Members.Keys.ToArray<string>();

			//FIXME: manage all of internal property names by SRM
			for (int i = 0; i < properties.Length; i++)
			{
				string key = properties[i];

				if (key != ScriptRunningMachine.KEY___PROTO__
						&& key != ScriptRunningMachine.KEY___ARGS__
						/*&& key != ScriptRunningMachine.KEY_CONSTRUCTOR*/)
				{
					yield return key;
				}
			}
		}

		#endregion

		internal AbstractFunctionObject Constructor { get; set; }

		/// <summary>
		/// Add properties given by IDictionary 
		/// </summary>
		/// <param name="properties"></param>
		public void AddProperties(IDictionary<string, object> properties)
		{
			foreach (string key in properties.Keys)
			{
				Members[key] = properties[key];
			}
		}

		public bool TryGetValue(string identifier, out object value)
		{
			return Members.TryGetValue(identifier, out value);
		}
	}

	class ObjectConstructorFunction : TypedNativeFunctionObject
	{
		private ObjectValue rootPrototype = new ObjectValue();

		public ObjectConstructorFunction()
			: base("Object")
		{
			// check whether property existed in owner object
			rootPrototype["hasOwnProperty"] = new NativeFunctionObject("hasOwnProperty", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return false;

				if (owner is string)
				{
					if (Convert.ToString(args[0]) == "length")
						return true;
				}

				ObjectValue ownerObject = owner as ObjectValue;

				if (ownerObject == null)
					return false;

				return ownerObject.HasOwnProperty(Convert.ToString(args[0]));
			});

			// remove only own property from owner object
			rootPrototype["removeOwnProperty"] = new NativeFunctionObject("removeOwnProperty", (ctx, owner, args) =>
			{
				ObjectValue ownerObject = owner as ObjectValue;

				if (ownerObject == null || args.Length < 1)
					return false;

				return ownerObject.RemoveOwnProperty(Convert.ToString(args[0]));
			});

			rootPrototype["toString"] = new NativeFunctionObject("toString", (ctx, owner, args) =>
			{
				return owner.ToString();
			});

			rootPrototype["valueOf"] = new NativeFunctionObject("valueOf", (ctx, owner, args) =>
			{
				return owner;
			});

			// root object in prototype chain
			this[ScriptRunningMachine.KEY___PROTO__] = rootPrototype;
		}

		public override object Invoke(ScriptContext context, object owner, object[] args)
		{
			ObjectValue obj = owner as ObjectValue;
			return obj == null ? context.CreateNewObject(this, false) : obj;
		}

		public override object CreateObject(ScriptContext context, object[] args)
		{
			return new ObjectValue();
		}

		public override object CreatePrototype(ScriptContext context)
		{
			return rootPrototype;
		}
	}
	#endregion
	#region NumberObject
	public class NumberObject : ObjectValue
	{
		public double Number { get; set; }
		public NumberObject() : this(0) { }
		public NumberObject(double num)
		{
			this.Number = num;
		}
		public override string ToString()
		{
			return Number.ToString();
		}
	}
	#endregion
	#region DateTimeValue
	public class DateObject : ObjectValue
	{
		public static readonly long StartTimeTicks = (new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc)).Ticks;

		public DateTime DateTime;

		public DateObject(DateTime value)
		{
			this.DateTime = value;

			this["getFullYear"] = new NativeFunctionObject("getFullYear", (ctx, owner, args) => { return DateTime.Year; });
			this["getMonth"] = new NativeFunctionObject("getMonth", (ctx, owner, args) => { return DateTime.Month; });
			this["getDate"] = new NativeFunctionObject("getDate", (ctx, owner, args) => { return DateTime.Day; });
			this["getDay"] = new NativeFunctionObject("getDay", (ctx, owner, args) => { return (int)DateTime.DayOfWeek; });
			this["getHours"] = new NativeFunctionObject("getHours", (ctx, owner, args) => { return DateTime.Hour; });
			this["getMinutes"] = new NativeFunctionObject("getMinutes", (ctx, owner, args) => { return DateTime.Minute; });
			this["getSeconds"] = new NativeFunctionObject("getSeconds", (ctx, owner, args) => { return DateTime.Second; });
			this["getMilliseconds"] = new NativeFunctionObject("getMilliseconds", (ctx, owner, args) => { return DateTime.Millisecond; });
		}

		public DateObject() :
			this(DateTime.Now)
		{
		}

		public double Ticks
		{
			get
			{
				return (DateTime.ToUniversalTime().Ticks - StartTimeTicks) / 10000d;
			}
		}

		public override string ToString()
		{
			return DateTime.ToLongDateString();
		}
	}
	#endregion
	#region StringValue
	public class StringObject : ObjectValue
	{
		public string String { get; set; }

		public StringObject()
			: this(string.Empty)
		{
			// this()
		}
		public StringObject(string text)
		{
			String = text;
			this["length"] = new ExternalProperty(() => String.Length, v => { });
		}
		public override bool Equals(object obj)
		{
			return (String == null && obj == null) ? false : String.Equals(Convert.ToString(obj));
		}
		public static bool operator ==(StringObject str1, object str2)
		{
			if (!(str2 is StringObject)) return false;

			return str1.Equals((StringObject)str2);
		}
		public static bool operator !=(StringObject str1, object str2)
		{
			if (!(str2 is StringObject)) return false;

			return !str1.Equals((StringObject)str2);
		}
		public override int GetHashCode()
		{
			return String.GetHashCode();
		}
		public override string ToString()
		{
			return String;
		}

		#region IEnumerable Members
		public override IEnumerator GetEnumerator()
		{
			for (int i = 0; i < String.Length; i++)
			{
				yield return String[i];
			}
		}

		#endregion
	}

	class StringConstructorFunction : TypedNativeFunctionObject
	{
		public StringConstructorFunction()
			: base(typeof(StringObject), "String")
		{
		}

		public override object Invoke(ScriptContext context, object owner, object[] args)
		{
			return args == null || args.Length <= 0 ? string.Empty : Convert.ToString(args[0]);
		}

		public override object CreateObject(ScriptContext context, object[] args)
		{
			return new StringObject((args == null || args.Length <= 0 ? string.Empty : ScriptRunningMachine.ConvertToString(args[0])));
		}

		public override object CreatePrototype(ScriptContext context)
		{
			ScriptRunningMachine srm = context.Srm;
			ObjectValue obj = context.CreateNewObject(srm.BuiltinConstructors.ObjectFunction) as ObjectValue;

			if (obj != null)
			{
				obj["trim"] = new NativeFunctionObject("trim", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;
					return ((string)owner).Trim();
				});

				obj["indexOf"] = new NativeFunctionObject("indexOf", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;
					return args.Length == 0 ? -1 : Convert.ToString(owner).IndexOf(Convert.ToString(args[0]));
				});

				obj["lastIndexOf"] = new NativeFunctionObject("lastIndexOf", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;
					return args.Length == 0 ? -1 : Convert.ToString(owner).LastIndexOf(Convert.ToString(args[0]));
				});

				obj["charAt"] = new NativeFunctionObject("charAt", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;

					string res = string.Empty;

					if (args.Length > 0)
					{
						int index = ScriptRunningMachine.GetIntParam(args, 0, -1);
						string str = Convert.ToString(owner);

						if (index >= 0 && index < str.Length)
							res = Convert.ToString(str[index]);
					}

					return res;
				});

				obj["charCodeAt"] = new NativeFunctionObject("charCodeAt", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;

					if (args.Length > 0)
					{
						int index = ScriptRunningMachine.GetIntParam(args, 0, -1);
						string str = Convert.ToString(owner);

						if (index >= 0 && index < str.Length)
							return (int)str[index];
					}

					return NaNValue.Value;
				});

				obj["startsWith"] = new NativeFunctionObject("startsWith", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;
					return args.Length == 0 ? false : Convert.ToString(owner).StartsWith(Convert.ToString(args[0]));
				});

				obj["endsWith"] = new NativeFunctionObject("endWith", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;
					return args.Length == 0 ? false : Convert.ToString(owner).EndsWith(Convert.ToString(args[0]));
				});

				obj["repeat"] = new NativeFunctionObject("repeat", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;

					int count = ScriptRunningMachine.GetIntParam(args, 0, 0);

					string result = string.Empty;

					if (count > 0)
					{
						string str = ((string)owner);
						StringBuilder sb = new StringBuilder();
						for (int i = 0; i < count; i++) sb.Append(str);
						result = sb.ToString();
					}

					return result;
				});

				//obj["join"] = new NativeFunctionObject("join", (ctx, owner, args) =>
				//{
				//  if (!(owner is string || owner is StringObject)) return null;
				//  //TODO
				//  return string.Empty;
				//});

				obj["split"] = new NativeFunctionObject("split", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;

					string str = Convert.ToString(owner);

					ArrayObject arr = ctx.CreateNewArray();
					if (args.Length == 0)
					{
						arr.List.Add(str);
					}
					else
					{
						string separator = args[0] == null ? string.Empty : Convert.ToString(args[0]);
						if (!string.IsNullOrEmpty(separator))
						{
							if (args.Length == 1)
							{
								arr.List.AddRange(str.Split(new string[] { separator },
									StringSplitOptions.RemoveEmptyEntries));
							}
							else
							{
								int limits = ScriptRunningMachine.GetIntParam(args, 1, 0);

								string[] splitted = str.Split(new string[] { separator }, limits + 1,
									StringSplitOptions.RemoveEmptyEntries);

								arr.List.Capacity = Math.Min(limits, splitted.Length);

								for (int i = 0; i < arr.List.Capacity; i++)
								{
									arr.List.Add(splitted[i]);
								}
							}
						}
					}

					return arr;
				});

				obj["substr"] = new NativeFunctionObject("substr", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;

					string str = Convert.ToString(owner);
					string newstr = string.Empty;

					if (args.Length < 1)
						return newstr;
					else
					{
						int from = ScriptRunningMachine.GetIntParam(args, 0, 0);
						if (from < 0 || from > str.Length - 1)
						{
							return newstr;
						}

						int len = ScriptRunningMachine.GetIntParam(args, 1, str.Length - from);

						newstr = str.Substring(from, len);
					}

					return newstr;
				});

				obj["toLowerCase"] = new NativeFunctionObject("toLowerCase", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;
					return Convert.ToString(owner).ToLower();
				});

				obj["toUpperCase"] = new NativeFunctionObject("toUpperCase", (ctx, owner, args) =>
				{
					if (!(owner is string || owner is StringObject)) return null;
					return Convert.ToString(owner).ToUpper();
				});

				obj["valueOf"] = new NativeFunctionObject("valueOf", (ctx, owner, args) =>
				{
					if (owner is string)
						return owner;
					else if (owner is StringObject)
						return ((StringObject)owner).String;
					else
						return null;
				});
			}

			return obj;
		}
	}
	#endregion
	#region BooleanObject
	class BooleanObject : ObjectValue
	{
		public bool Boolean { get; set; }
		public BooleanObject() : this(false) { }
		public BooleanObject(bool boolean)
		{
			this.Boolean = boolean;
		}
	}
	#endregion
	#region ErrorObject

	/// <summary>
	/// Error object describes an error happening in script execution.
	/// </summary>
	public class ErrorObject : ObjectValue
	{
		/// <summary>
		/// Execution context when error happened
		/// </summary>
		public ScriptContext Context { get; set; }

		/// <summary>
		/// Message of error content
		/// </summary>
		public string Message { get; set; }

		internal List<CallScopeObject> CallStack { get; set; }

		/// <summary>
		/// Character index of line where error happening
		/// </summary>
		public int CharIndex { get; set; }

		/// <summary>
		/// Line number where error happening
		/// </summary>
		public int Line { get; set; }

		/// <summary>
		/// Construct an error without message content
		/// </summary>
		public ErrorObject() : this(string.Empty) { }

		/// <summary>
		/// Construct an error with a message content
		/// </summary>
		/// <param name="msg"></param>
		public ErrorObject(string msg)
		{
			this.Message = msg;

			this["message"] = new ExternalProperty(() => Message);
			this["charIndex"] = new ExternalProperty(() => CharIndex);
			this["line"] = new ExternalProperty(() => Line);
			this["stack"] = new ExternalProperty(() => CallStack);
		}

		/// <summary>
		/// Get entire information of error contains call-stack.
		/// </summary>
		/// <returns></returns>
		public string GetFullErrorInfo()
		{
			if (CallStack == null || CallStack.Count == 0)
			{
				return Message;
			}
			else
			{
				StringBuilder sb = new StringBuilder();
				sb.AppendLine(Message);
				sb.Append(DumpCallStack());
				return sb.ToString();
			}
		}

		/// <summary>
		/// Get call-stack inforamtion about error
		/// </summary>
		/// <returns></returns>
		public string DumpCallStack()
		{
			if (CallStack == null) return string.Empty;

			StringBuilder sb = new StringBuilder();

			foreach (CallScopeObject scope in CallStack)
			{
				sb.AppendLine("    " + scope.ToString());
			}

			return sb.ToString();
		}

		/// <summary>
		/// Customized error object can be thrown from script
		/// </summary>
		public object CustomeErrorObject { get; set; }
	}

	internal class CallScopeObject : ObjectValue
	{
		internal CallScope CallScope { get; set; }

		internal CallScopeObject() { }

		internal CallScopeObject(CallScope callScope)
		{
			this.CallScope = callScope;
		}

		public override string ToString()
		{
			return CallScope.ToString();
		}
	}

	internal class ErrorConstructorFunction : TypedNativeFunctionObject
	{
		public ErrorConstructorFunction() : base(typeof(ErrorObject), "Error") { }

		public override object CreateObject(ScriptContext context, object[] args)
		{
			return (args == null || args.Length <= 0 ? new ErrorObject() : new ErrorObject(ScriptRunningMachine.ConvertToString(args[0])));
		}

		public override object CreatePrototype(ScriptContext context)
		{
			object obj = base.CreatePrototype(context);
			ObjectValue proto = obj as ObjectValue;
			if (proto != null)
			{
				proto["dumpStack"] = new NativeFunctionObject("dumpStack", (ctx, owner, args) =>
				{
					if (!(owner is ErrorObject)) return string.Empty;
					else return ((ErrorObject)owner).GetFullErrorInfo();
				});
			}
			return obj;
		}
	}
	#endregion

	#region World Value
	internal class WorldObject : ObjectValue
	{
		#region Built-in functions
		private static readonly NativeFunctionObject __stdin__ = new NativeFunctionObject("__stdin__", (ctx, owner, args) =>
		{
			return ctx.Srm.StandardInputProvider.Read();
		});

		private static readonly NativeFunctionObject __stdinln__ = new NativeFunctionObject("__stdinln__", (ctx, owner, args) =>
		{
			return ctx.Srm.StandardInputProvider.ReadLine();
		});

		private static readonly NativeFunctionObject __stdout__ = new NativeFunctionObject("__stdout__", (ctx, owner, args) =>
		{
			if (args == null || args.Length == 0)
			{
				ctx.Srm.StandardIOWrite(0);
			}
			else
			{
				//ctx.Srm.StandardIOWrite(args[0] == null ? ScriptRunningMachine.KEY_UNDEFINED : Convert.ToString(args[0]));
				ctx.Srm.StandardIOWrite(args[0]);
			}

			if (args.Length > 1)
			{
				StringBuilder sb = new StringBuilder();
				for (int i = 1; i < args.Length; i++)
				{
					sb.Append(' ');
					sb.Append(args[0] == null ? ScriptRunningMachine.KEY_UNDEFINED : Convert.ToString(args[i]));
				}

				ctx.Srm.StandardIOWrite(sb.ToString());
			}

			return null;
		});

		private static readonly NativeFunctionObject __stdoutln__ = new NativeFunctionObject("__stdoutln__", (ctx, owner, args) =>
		{
			if (args == null || args.Length == 0 || (args.Length == 1 && args[0] == null))
			{
				ctx.Srm.StandardIOWriteLine(string.Empty);
			}
			else
			{
				ctx.Srm.StandardIOWriteLine(args[0] == null ? ScriptRunningMachine.KEY_UNDEFINED : Convert.ToString(args[0]));
			}

			if (args.Length > 1)
			{
				StringBuilder sb = new StringBuilder();
				for (int i = 1; i < args.Length; i++)
				{
					sb.Append(' ');
					sb.Append(args[0] == null ? ScriptRunningMachine.KEY_UNDEFINED : Convert.ToString(args[i]));
				}

				ctx.Srm.StandardIOWriteLine(sb.ToString());
			}

			return null;
		});

		private static readonly NativeFunctionObject __parseInt__ = new NativeFunctionObject("parseInt", (ctx, owner, args) =>
		{
			if (args.Length == 0) return 0;
			else if (args.Length == 1)
			{
				if (args[0] is double) return Convert.ToInt32(args[0]);

				int i = 0;
				int.TryParse(Convert.ToString(args[0]), out i);
				return i;
			}
			else
			{
				try
				{
					return Convert.ToInt32(Convert.ToString(args[0]), Convert.ToInt32(args[1]));
				}
				catch
				{
					return 0;
				}
			}
		});

		private static readonly NativeFunctionObject __isNumber__ = new NativeFunctionObject("isNumber",
			(ctx, owner, args) =>
			{
				if (args.Length == 1)
				{
					double val;
					return ScriptRunningMachine.TryGetNumberValue(args[0], out val);
				}
				else
					return true;
			});

		private static readonly NativeFunctionObject __isNaN__ = new NativeFunctionObject("isNaN",
			(ctx, owner, args) =>
			{
				if (args.Length == 1)
				{
					double val;
					return !ScriptRunningMachine.TryGetNumberValue(args[0], out val);
				}
				else
					return true;
			});
		#endregion

		public WorldObject()
		{
			//this.Name = "Script";

			// built-in native functions
			this[__stdin__.FunName] = __stdin__;
			this[__stdinln__.FunName] = __stdinln__;
			this[__stdout__.FunName] = __stdout__;
			this[__stdoutln__.FunName] = __stdoutln__;
			this[__parseInt__.FunName] = __parseInt__;
			this[__isNumber__.FunName] = __isNumber__;
			this[__isNaN__.FunName] = __isNaN__;
		}
	}
	#endregion

	#region Math
	class MathObject : ObjectValue
	{
		private static readonly Random rand = new Random();

		public MathObject()
		{
			#region random
			this["random"] = new NativeFunctionObject("random", (ctx, owner, args) =>
			{
				return rand.NextDouble();
			});
			#endregion // random
			#region round
			this["round"] = new NativeFunctionObject("round", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else if (args.Length < 2)
					return (Math.Round(ScriptRunningMachine.GetNumberValue(args[0])));
				else
					return (Math.Round(ScriptRunningMachine.GetNumberValue(args[0]),
						ScriptRunningMachine.GetIntValue(args[1])));
			});
			#endregion // round
			#region floor
			this["floor"] = new NativeFunctionObject("floor", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return (Math.Floor(ScriptRunningMachine.GetNumberValue(args[0])));
			});
			#endregion // floor

			#region sin
			this["sin"] = new NativeFunctionObject("sin", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Sin(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // sin
			#region cos
			this["cos"] = new NativeFunctionObject("cos", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Cos(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // ocs
			#region tan
			this["tan"] = new NativeFunctionObject("tan", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Tan(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // tan
			#region atan
			this["atan"] = new NativeFunctionObject("atan", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Atan(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // atan
			#region atan2
			this["atan2"] = new NativeFunctionObject("atan2", (ctx, owner, args) =>
			{
				if (args.Length < 2)
					return NaNValue.Value;
				else
					return Math.Atan2(ScriptRunningMachine.GetNumberValue(args[0], 0),
						ScriptRunningMachine.GetNumberValue(args[0], 1));
			});
			#endregion // atan2

			#region abs
			this["abs"] = new NativeFunctionObject("abs", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Abs(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // abs
			#region exp
			this["exp"] = new NativeFunctionObject("exp", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Exp(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // exp
			#region log
			this["log"] = new NativeFunctionObject("log", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Log(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // log
			#region cosh
			this["cosh"] = new NativeFunctionObject("cosh", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Cosh(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // log
			#region pow
			this["pow"] = new NativeFunctionObject("pow", (ctx, owner, args) =>
			{
				if (args.Length < 2)
					return NaNValue.Value;
				else
					return Math.Pow(ScriptRunningMachine.GetNumberValue(args[0], 0),
						ScriptRunningMachine.GetNumberValue(args[1], 0));
			});
			#endregion //pow
			#region sqrt
			this["sqrt"] = new NativeFunctionObject("sqrt", (ctx, owner, args) =>
			{
				if (args.Length < 1)
					return NaNValue.Value;
				else
					return Math.Sqrt(ScriptRunningMachine.GetNumberValue(args[0], 0));
			});
			#endregion // sqrt		
		}
	}
	#endregion

	#region Array
	public class ArrayObject : ObjectValue, IList
	{
		private ArrayList list = new ArrayList(5);

		public ArrayList List
		{
			get { return list; }
			set { list = value; }
		}

		public ArrayObject()
		{
			this["length"] = new ExternalProperty(
				() => this.Length,
				v => this.Length = ScriptRunningMachine.GetIntValue(v, List.Count));
		}

		public int Length
		{
			get
			{
				return list.Count;
			}
			set
			{
				int len = value;

				if (len < list.Count)
				{
					list.RemoveRange(len, list.Count - len);
				}
				else
				{
					object[] empty = new object[len - list.Count];
					list.AddRange(empty);
					//ArrayList newArr = new ArrayList(len+5);
					//newArr.AddRange(list);
					//list = newArr;
				}
			}
		}

		public bool IsReadOnly => false;

		public bool IsFixedSize => false;

		public int Count => this.list.Count;

		public object SyncRoot => this.list.SyncRoot;

		public bool IsSynchronized => this.list.IsSynchronized;

		public object this[int index]
		{
			get
			{
				return index >= 0 && index < list.Count ? list[index] : null;
			}
			set
			{
				if (index >= list.Count)
				{
					this.Length = index + 1;
				}

				this.list[index] = value;
			}
		}

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder(128);
			sb.Append('[');
			for (int i = 0; i < list.Count; i++)
			{
				if (i > 0) sb.Append(", ");
				object val = list[i];
				sb.Append(val == null ? ScriptRunningMachine.KEY_UNDEFINED : Convert.ToString(val));
			}
			sb.Append(']');
			return sb.ToString();
		}

		#region IEnumerable Members

		public override IEnumerator GetEnumerator()
		{
			for (int i = 0; i < list.Count; i++)
			{
				yield return list[i];
			}
		}

		public int Add(object value)
		{
			return this.list.Add(value);
		}

		public bool Contains(object value)
		{
			return this.list.Contains(value);
		}

		public void Clear()
		{
			this.list.Clear();
		}

		public int IndexOf(object value)
		{
			return this.list.IndexOf(value);
		}

		public void Insert(int index, object value)
		{
			this.list.Insert(index, value);
		}

		public void Remove(object value)
		{
			this.list.Remove(value);
		}

		public void RemoveAt(int index)
		{
			this.list.RemoveAt(index);
		}

		public void CopyTo(Array array, int index)
		{
			this.list.CopyTo(array, index);
		}
		#endregion IEnumerable Members

	}
	class ArrayConstructorFunction : TypedNativeFunctionObject
	{
		public ArrayConstructorFunction() :
			base(typeof(ArrayObject), "Array")
		{ }

		public override object Invoke(ScriptContext context, object owner, object[] args)
		{
			return base.Invoke(context, owner, args);
		}

		public override object CreateObject(ScriptContext context, object[] args)
		{
			ArrayObject arr = new ArrayObject();
			if (args != null)
			{
				arr.List.AddRange(args);
			}
			return arr;
		}

		public override object CreatePrototype(ScriptContext context)
		{
			ScriptRunningMachine srm = context.Srm;
			object obj = srm.CreateNewObject(context, srm.BuiltinConstructors.ObjectFunction);

			if (obj is ObjectValue)
			{
				ObjectValue objValue = (ObjectValue)obj;

				objValue["push"] = new NativeFunctionObject("push", (ctx, owner, args) =>
				{
					if (!(owner is ArrayObject)) return null;

					foreach (object v in args)
					{
						((ArrayObject)owner).List.Add(v);
					}
					return null;
				});

				objValue["slice"] = new NativeFunctionObject("slice", (ctx, owner, args) =>
				{
					if (args.Length < 1 || !(owner is ArrayObject)) return false;

					ArrayObject arr = (ArrayObject)owner;
					ArrayObject newArray = ctx.CreateNewArray();

					int index = ScriptRunningMachine.GetIntParam(args, 0, 0);
					if (index < 0 || index >= arr.Length)
					{
						return newArray;
					}

					int howmany = ScriptRunningMachine.GetIntParam(args, 1, arr.Length - index);

					newArray.List.AddRange(arr.List.GetRange(index, howmany));

					return newArray;
				});

				objValue["splice"] = new NativeFunctionObject("splice", (ctx, owner, args) =>
				{
					if (args.Length < 2 || !(owner is ArrayObject)) return false;

					ArrayObject arr = (ArrayObject)owner;

					int index = ScriptRunningMachine.GetIntParam(args, 0, 0);
					if (index < 0) index = 0;
					if (index >= arr.Length) return false;

					int howmany = ScriptRunningMachine.GetIntParam(args, 1, arr.Length - index);

					arr.List.RemoveRange(index, howmany);

					for (int i = 2; i < args.Length; i++)
						arr.List.Insert(index++, args[i]);

					return true;
				});

				objValue["remove"] = new NativeFunctionObject("remove", (ctx, owner, args) =>
				{
					if (args.Length <= 0 || !(owner is ArrayObject)) return null;

					ArrayObject arr = (ArrayObject)owner;
					arr.List.Remove(args[0]);

					return null;
				});

				objValue["indexOf"] = new NativeFunctionObject("indexOf", (ctx, owner, args) =>
				{
					if (!(owner is ArrayObject)) return NaNValue.Value;

					if (args == null || args.Length <= 0) return -1;

					return ((ArrayObject)owner).List.IndexOf(args[0]);
				});

				objValue["sort"] = new NativeFunctionObject("sort", (ctx, owner, args) =>
				{
					if (!(owner is ArrayObject)) return null;

					((ArrayObject)owner).List.Sort();
					return null;
				});

				objValue["join"] = new NativeFunctionObject("join", (ctx, owner, args) =>
				{
					if (!(owner is ArrayObject)) return null;

					string separator = args == null || args.Length == 0 ? "," : Convert.ToString(args[0]);

					StringBuilder sb = new StringBuilder();
					foreach (object element in ((ArrayObject)owner).List)
					{
						if (sb.Length > 0) sb.Append(separator);
						sb.Append(Convert.ToString(element));
					}

					return sb.ToString();
				});

				objValue["concat"] = new NativeFunctionObject("concat", (ctx, owner, args) =>
				{
					if (!(owner is ArrayObject)) return null;

					if (args.Length <= 0) return owner;

					var newArr = ctx.CreateNewArray();
					newArr.List.AddRange(((ArrayObject)owner).List);

					foreach (var arg in args)
					{
						if (arg is ArrayObject)
						{
							newArr.List.AddRange((arg as ArrayObject).List);
						}
						else
						{
							newArr.List.Add(arg);
						}
					}

					return newArr;
				});
			}

			return obj;
		}
	}
	#endregion Array
	#endregion Built-in Objects

	#region Extension Objects
	/// <summary>
	/// Dynamic access properties of an object
	/// </summary>
	public class DynamicPropertyObject : ObjectValue
	{
		public Action<string, object> propertySetter { get; set; }
		public Func<string> propertyGetter { get; set; }

		public DynamicPropertyObject(Action<string, object> setter, Func<string> getter)
		{
			this.propertySetter = setter;
			this.propertyGetter = getter;
		}

		public override object this[string name]
		{
			get
			{
				return propertyGetter != null ? propertyGetter() : base[name];
			}
			set
			{
				if (propertySetter != null)
				{
					propertySetter(name, value);
				}
				else
				{
					base[name] = value;
				}
			}
		}
	}

	/// <summary>
	/// ExternalProperty class provides an interface to extend a property to an 
	/// object which declared and used in ReoScript context. ExternalProperty 
	/// has a getter and setter delegate method that will be invoked automatically
	/// when the property value is accessed in script at runtime.
	/// </summary>
	public class ExternalProperty : ISyntaxTreeReturn
	{
		/// <summary>
		/// Getter method will be invoked when value is required in script
		/// </summary>
		public Func<object> Getter { get; set; }

		/// <summary>
		/// Setter method will be invoked when property is set to a value 
		/// in script.
		/// </summary>
		public Action<object> Setter { get; set; }

		/// <summary>
		/// Create extension property for object with getter and setter method
		/// </summary>
		/// <param name="getter">delegate invoked when value be getted</param>
		/// <param name="setter">delegate invoked when value be setted</param>
		public ExternalProperty(Func<object> getter, Action<object> setter)
		{
			this.Getter = getter;
			this.Setter = setter;
		}

		/// <summary>
		/// Create readonly extension property with only getter method
		/// </summary>
		/// <param name="getter">method invoked when value be getted</param>
		public ExternalProperty(Func<object> getter)
			: this(getter, null) { }

		/// <summary>
		/// Invoke getter to retrieve a value from CLR runtime
		/// </summary>
		/// <returns></returns>
		public object GetNativeValue()
		{
			return Getter == null ? null : Getter();
		}

		/// <summary>
		/// Invoke setter to set property value that provided from CLR
		/// </summary>
		/// <param name="value"></param>
		public void SetNativeValue(object value)
		{
			if (Setter != null) Setter(value);
		}

		/// <summary>
		/// Convert value to string
		/// </summary>
		/// <returns></returns>
		public override string ToString()
		{
			return Getter == null ? "null" : Convert.ToString(Getter());
		}
	}

	/// <summary>
	/// IDirectAccess is a empty interface that allows CLR instance can be 
	/// accessed directly from ReoScript.
	/// </summary>
	//public interface IDirectAccess
	//{
	//}

	/// <summary>
	/// DirectAccessAttribute allows CLR instace can be accessed directly 
	/// from ReoScript.
	/// </summary>
	//public class DirectAccessAttribute : Attribute
	//{
	//}

	#endregion // Extension Values

	#region Function Objects
	//public interface IFunctionObject
	//{
	//  object Invoke(ScriptRunningMachine srm, object owner, object[] args);
	//}
	//public interface IConstructorFunctionObject : IFunctionObject
	//{
	//  object CreateObject(ScriptRunningMachine srm);
	//  ObjectValue Prototype { get; set; }
	//}
	/// <summary>
	/// Abstract of ReoScript executable function
	/// </summary>
	public abstract class AbstractFunctionObject : ObjectValue
	{
		public abstract string FunName { get; set; }
		//public ObjectValue Prototype { get; set; }

		internal AbstractFunctionObject() { }

		public virtual object CreateObject(ScriptContext context, object[] args)
		{
			return new ObjectValue();
		}

		public virtual object CreatePrototype(ScriptContext context)
		{
			return context.CreateNewObject(context.Srm.BuiltinConstructors.ObjectFunction) as ObjectValue;
		}

		internal CallScope OuterCallScope { get; set; }
	}

	/// <summary>
	/// Executable function defined in script
	/// </summary>
	public class FunctionObject : AbstractFunctionObject
	{
		/// <summary>
		/// Name of function
		/// </summary>
		public override string FunName { get; set; }

		/// <summary>
		/// Argument name list
		/// </summary>
		public string[] Args { get; set; }

		/// <summary>
		/// Syntax Tree of function body
		/// </summary>
		internal CommonTree Body { get; set; }

		public override string ToString()
		{
			return string.IsNullOrEmpty(FunName) ? "function() {...}"
				: "function " + FunName + "() {...}";
		}

		/// <summary>
		/// Function meta information
		/// </summary>
		public FunctionInfo FunctionInfo { get; set; }
	}

	/// <summary>
	/// Executable function defined in .NET
	/// </summary>
	public class NativeFunctionObject : AbstractFunctionObject
	{
		/// <summary>
		/// Name of function
		/// </summary>
		public override string FunName { get; set; }

		public override string ToString()
		{
			return "function " + FunName + "() { [native code] }";
		}

		/// <summary>
		/// Construct executable function with name
		/// </summary>
		/// <param name="name">name of function</param>
		public NativeFunctionObject(string name)
		{
			//this.Name = name;
			this.FunName = name;
		}

		/// <summary>
		/// Construct executable function with name and body
		/// </summary>
		/// <param name="name">name of function</param>
		/// <param name="body">body of function</param>
		public NativeFunctionObject(string name, Func<ScriptContext, object, object[], object> body)
		{
			this.FunName = name;
			this.Body = body;
		}

		/// <summary>
		/// Invoke function
		/// </summary>
		/// <param name="context">Context of script execution</param>
		/// <param name="owner">this object to call function</param>
		/// <param name="args">argument list</param>
		/// <returns></returns>
		public virtual object Invoke(ScriptContext context, object owner, object[] args)
		{
			return (Body == null ? null : Body(context, owner, args));
		}

		/// <summary>
		/// Body of function
		/// </summary>
		public Func<ScriptContext, object, object[], object> Body { get; set; }
	}

	/// <summary>
	/// Executable typed constructor function. Typed function can create an instance of object
	/// that is specified by user.
	/// </summary>
	public class TypedNativeFunctionObject : NativeFunctionObject
	{
		public Type Type { get; set; }

		public Action<ObjectValue> PrototypeBuilder { get; set; }

		public TypedNativeFunctionObject(string name)
			: this(null, name, null, null)
		{
			// this()
		}

		public TypedNativeFunctionObject(Type type, string name)
			: this(type, name, null, null)
		{
			// this()
		}

		public TypedNativeFunctionObject(Type type, string name,
			Func<ScriptContext, object, object[], object> body)
			: this(type, name, body, null)
		{
			// this()
		}

		public TypedNativeFunctionObject(Type type, string name,
			Func<ScriptContext, object, object[], object> body,
			Action<ObjectValue> prototypeBuilder)
			: base(name, body)
		{
			this.Type = type;
			this.PrototypeBuilder = prototypeBuilder;
		}

		public override object CreateObject(ScriptContext context, object[] args)
		{
			//try
			//{
				object[] cargs = null;

				if (args != null && args.Length > 0)
				{
					ConstructorInfo ci = this.Type.GetConstructors().FirstOrDefault(c => c.GetParameters().Length == args.Length);

					if (ci == null)
					{
						throw new ReoScriptRuntimeException("Cannot to create .NET instance with incorrect parameters.");
					}

					cargs = new object[args.Length];

					ParameterInfo[] pis = ci.GetParameters();
					for (int i = 0; i < args.Length; i++)
					{
						cargs[i] = context.Srm.ConvertToCLRType(context, args[i], pis[i].ParameterType);
					}
				}

				return System.Activator.CreateInstance(this.Type, BindingFlags.Default, null, cargs, null);
			//}
			//catch (Exception ex)
			//{
			//  throw new ReoScriptRuntimeException(context, "Error to create .Net instance: " + this.Type.ToString(), null, ex);
			//}
		}

		public override object CreatePrototype(ScriptContext context)
		{
			ObjectValue obj = context.CreateNewObject(context.Srm.BuiltinConstructors.ObjectFunction) as ObjectValue;

			if (obj == null) return obj;

			PrototypeBuilder?.Invoke(obj);

			return obj;
		}
	}

	/// <summary>
	/// Generic typed constructor function.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	public class TypedNativeFunctionObject<T> : TypedNativeFunctionObject
	{
		public TypedNativeFunctionObject()
			: this(typeof(T).Name)
		{
			// this();
		}
		public TypedNativeFunctionObject(string name)
			: this(name, null)
		{
			// this()
		}
		public TypedNativeFunctionObject(string name, Func<ScriptContext, object, object[], object> body)
			: this(name, body, null)
		{
			// this()
		}
		public TypedNativeFunctionObject(string name, Func<ScriptContext, object, object[], object> body,
			Action<ObjectValue> prototypeBuilder)
			: base(typeof(T), name, body, prototypeBuilder)
		{
			// base()
		}
	}
	#endregion

	#region Extension Attributes
	[AttributeUsage(AttributeTargets.Field |
		 AttributeTargets.Property | AttributeTargets.Method)]
	public class RSPropertyAttribute : Attribute
	{
	}
	#endregion

	#region Exceptions
	/// <summary>
	/// Base exception of all exceptions
	/// </summary>
	public class ReoScriptException : Exception
	{
		public ErrorObject ErrorObject { get; set; }
		public ReoScriptException(string msg) : base(msg) { this.ErrorObject = new ErrorObject() { Message = msg }; }
		public ReoScriptException(ErrorObject error) : this(error == null ? string.Empty : error.Message) { this.ErrorObject = error; }
		public ReoScriptException(ErrorObject error, Exception inner) : base(inner == null ? string.Empty : inner.Message) { this.ErrorObject = error; }
		public ReoScriptException(string msg, Exception inner) : base(msg, inner) { }

		public string GetFullErrorInfo()
		{
			ErrorObject e = ErrorObject as ErrorObject;
			if (e == null)
			{
				return this.Message;
			}
			else
			{
				return e.GetFullErrorInfo();
			}
		}
	}
	/// <summary>
	/// Runtime error exception
	/// </summary>
	public class ReoScriptRuntimeException : ReoScriptException
	{
		public ReoScriptRuntimeException(string msg) : base(msg) { }
		public ReoScriptRuntimeException(ErrorObject error) : base(error) { }
		public ReoScriptRuntimeException(ErrorObject error, Exception inner) : base(error, inner) { }
	}
	/// <summary>
	/// Compile-time error exception
	/// </summary>
	public class ReoScriptCompilingException : ReoScriptException
	{
		public ReoScriptCompilingException(ErrorObject error) : base(error) { }
	}
	//public class ReoScriptSyntaxErrorException : ReoScriptCompilingException
	//{
	//  public ReoScriptSyntaxErrorException(ErrorObject error) : base(error) { }
	//}
	public enum SyntaxErrorType
	{
		ExpectToken,
		UnexpectedToken,
		MissingToken,
		MistakeToken,
	}

	/// <summary>
	/// Assertion failure exception. This exception thrown by debug.assert built-in function.
	/// </summary>
	public class ReoScriptAssertionException : ReoScriptRuntimeException
	{
		public ReoScriptAssertionException(string msg) : base(msg) { }
	}

	/// <summary>
	/// Call-stack overflow exception. This exception will be thrown if the call-stack reached the max limitation.
	/// </summary>
	public class CallStackOverflowException : ReoScriptRuntimeException
	{
		public CallStackOverflowException(string msg) : base(msg) { }
	}

	/// <summary>
	/// This exception will be thrown if script attempts to call an undefined function.
	/// </summary>
	public class FunctionNotDefinedException : ReoScriptRuntimeException
	{
		public FunctionNotDefinedException(ErrorObject error) : base(error) { }
	}

	//public class ClassNotFoundException : ReoScriptRuntimeException
	//{
	//  public ClassNotFoundException(ScriptContext context, string msg) : base(context, msg) { }
	//  public ClassNotFoundException(ScriptContext context, string msg, Exception inner) : base(context, msg, null, inner) { }
	//}

	#endregion

	#region Accessor
	// FIXME: Accessor mechanism should be removed in order to improve the execution speed
	//[Obsolete("Accessor mechanism should be removed in order to improve the execution speed.")]
	interface IAccess : ISyntaxTreeReturn
	{
		void Set(object value);
		object Get();
	}

	abstract class AccessValue : ISyntaxTreeReturn, IAccess
	{
		protected ScriptContext Context { get; set; }
		protected ScriptRunningMachine Srm { get; set; }

		public AccessValue(ScriptRunningMachine srm, ScriptContext ctx)
		{
			this.Srm = srm;
			this.Context = ctx;
		}

		#region IAccess Members

		public abstract void Set(object value);
		public abstract object Get();

		#endregion
	}

	#region Variable Access
	class VariableAccess : AccessValue
	{
		public string Identifier { get; set; }
		public IVariableContainer Scope { get; set; }
		public object Value { get; set; }

		public VariableAccess(ScriptRunningMachine srm, ScriptContext ctx, string identifier)
			: base(srm, ctx)
		{
			this.Identifier = identifier;

			CallScope cs = ctx.CurrentCallScope;

			if (cs != null)
			{
				if (cs.Variables.ContainsKey(identifier))
				{
					Scope = cs;
				}
				else
				{
					CallScope outerScope = cs.CurrentFunction.OuterCallScope;
					while (outerScope != null)
					{
						if (outerScope.Variables.ContainsKey(identifier))
						{
							Scope = outerScope;
							break;
						}

						outerScope = outerScope.CurrentFunction.OuterCallScope;
					}
				}
			}

			if (Scope == null)
			{
				Scope = ctx.GlobalObject;
			}

#if DEBUG
			Debug.Assert(Scope != null);
#endif

			object o = null;
			Scope.TryGetValue(Identifier, out o);
			Value = o;

		}

		//private CallScope scope;
		//public CallScope Scope { get { return scope; } }
		//public ObjectValue GlobalObject { get; set; }
		#region Access Members
		public override void Set(object value)
		{
			if (Value is ExternalProperty)
			{
				((ExternalProperty)Value).Setter(value);
			}
			else
			{
				Scope[Identifier] = value;
			}
		}
		public override object Get()
		{
			return Value;
			//object o = null;
			//Scope.TryGetValue(Identifier, out o);
			//return o;

			//if (Identifier == ScriptRunningMachine.GLOBAL_VARIABLE_NAME)
			//{
			//  return Context.GlobalObject;
			//}
			//else
			//  return Scope == null ? (GlobalObject == null ? null : GlobalObject[Identifier]) : Scope[Identifier];
		}
		#endregion Access Members
	}
	#endregion Variable Access

	#region Array Access
	class ArrayAccess : AccessValue
	{
		public IList Array { get; set; }
		public int Index { get; set; }

		public ArrayAccess(ScriptRunningMachine srm, ScriptContext ctx, IList array, int index)
			: base(srm, ctx)
		{
			this.Array = array;
			this.Index = index;
		}

		#region Access Members
		public override void Set(object value)
		{
			Array[Index] = value;
		}

		public override object Get()
		{
			return Index >= Array.Count ? null : Array[Index];
		}
		#endregion
	}
	#endregion Array Access

	#region Property Access
	class PropertyAccess : AccessValue
	{
		private object obj;
		public object Object
		{
			get { return obj; }
			set { obj = value; }
		}
		private string identifier;
		public string Identifier
		{
			get { return identifier; }
			set { identifier = value; }
		}
		public PropertyAccess(ScriptRunningMachine srm, ScriptContext ctx, object obj, string identifer)
			: base(srm, ctx)
		{
			this.obj = obj;
			this.identifier = identifer;
		}
		#region Access Members
		public override void Set(object value)
		{
			PropertyAccessHelper.SetProperty(Context, obj, identifier, value);
		}
		public override object Get()
		{
			return PropertyAccessHelper.GetProperty(Context, obj, identifier);
		}
		#endregion
	}

	static class PropertyAccessHelper
	{
		internal static void SetProperty(ScriptContext context, object target, string identifier, object value)
		{
			ScriptRunningMachine srm = context.Srm;

			if (target is ObjectValue)
			{
				ObjectValue objValue = (ObjectValue)target;

				object val = objValue[identifier];

				if (val is ExternalProperty)
				{
					((ExternalProperty)val).SetNativeValue(value);
				}
				else
				{
					objValue[identifier] = value;
				}
			}
			else if (target is IDictionary<string, object>)
			{
				IDictionary<string, object> dict = (IDictionary<string, object>)target;
				dict[identifier] = value;
			}
			else if (srm.AllowDirectAccess && !(target is ISyntaxTreeReturn))
			{
				string memberName = ScriptRunningMachine.GetNativeIdentifier(identifier);

				// if value is anonymous function, try to attach CLR event
				if (value is FunctionObject)
				{
					if (srm.AllowCLREvent)
					{
						EventInfo ei = target.GetType().GetEvent(memberName);
						if (ei != null)
						{
							srm.AttachEvent(context, target, ei, value as FunctionObject);

							if (target is ObjectValue)
							{
								((ObjectValue)target)[identifier] = value;
							}
						}
					}
				}
				else
				{
					PropertyInfo pi = target.GetType().GetProperty(memberName, BindingFlags.Instance | BindingFlags.Public);

					if (pi != null)
					{
						try
						{
							if (target is Control && ((Control)target).InvokeRequired)
							{
								((Control)target).Invoke((MethodInvoker)(() =>
								{
									pi.SetValue(target, srm.ConvertToCLRType(context, value, pi.PropertyType), null);
								}));
							}
							else
							{
								pi.SetValue(target, srm.ConvertToCLRType(context, value, pi.PropertyType), null);
							}
						}
						catch (Exception ex)
						{
							if (srm.IgnoreCLRExceptions)
							{
								// call error, do nothing
							}
							else
								throw ex;
						}
					}
					else
					{
						FieldInfo fi = target.GetType().GetField(memberName, BindingFlags.Instance | BindingFlags.Public);

						if (fi != null)
						{
							try
							{
								fi.SetValue(target, value);
							}
							catch (Exception ex)
							{
								if (srm.IgnoreCLRExceptions)
								{
									// call error, do nothing
								}
								else
									throw ex;
							}
						}
						else
						{
							// remove event if property value is set to null
							if (value == null && srm.AllowCLREvent)
							{
								EventInfo ei = target.GetType().GetEvent(memberName, BindingFlags.Instance | BindingFlags.Public);

								if (ei != null)
								{
									srm.DetachEvent(target, ei);
								}
							}

							if (target is ObjectValue)
							{
								((ObjectValue)target)[identifier] = value;
							}
							else
							{
								// can not found property or field, ignore this access
							}
						}
					}
				}
			}
			else
			{
				// unknown type, ignore it
			}
		}

		internal static object GetProperty(ScriptContext ctx, object target, string identifier)
		{
			ScriptRunningMachine srm = ctx.Srm;

			if (target is string)
			{
				// FIXME: not very good to get property 'length' in hard coding
				if (identifier == "length")
					return (target as string).Length;
				else
					return PropertyAccessHelper.GetProperty(ctx, srm.BuiltinConstructors.StringFunction[
						ScriptRunningMachine.KEY_PROTOTYPE], identifier);
			}
			else if (ScriptRunningMachine.IsPrimitiveNumber(target))
			{
				return PropertyAccessHelper.GetProperty(ctx, srm.BuiltinConstructors.NumberFunction[
					ScriptRunningMachine.KEY_PROTOTYPE], identifier);
			}
			else if (target is bool)
			{
				return PropertyAccessHelper.GetProperty(ctx, srm.BuiltinConstructors.BooleanFunction[
					ScriptRunningMachine.KEY_PROTOTYPE], identifier);
			}
			else if (target is IList && !(target is ArrayObject))
			{
				// FIXME: not very good to get property 'length' in hard coding
				if (identifier == "length")
				{
					return (target as IList).Count;
				}
				else
				{
					return PropertyAccessHelper.GetProperty(ctx, srm.BuiltinConstructors.ArrayFunction[
						ScriptRunningMachine.KEY_PROTOTYPE], identifier);
				}
			}
			else if (target is ObjectValue)
			{
				var objValue = (target as ObjectValue);
				object val = objValue[identifier];

				if (val is ExternalProperty)
				{
					return (val as ExternalProperty).GetNativeValue();
				}
				else
				{
					// if value is not found, get property from its prototype
					if (val == null && objValue.HasOwnProperty(ScriptRunningMachine.KEY___PROTO__))
					{
						val = PropertyAccessHelper.GetProperty(ctx,
							objValue.GetOwnProperty(ScriptRunningMachine.KEY___PROTO__), identifier);
					}

					return val;
				}
			}
			else if (target is IDictionary<string, object>)
			{
				IDictionary<string, object> dict = (IDictionary<string, object>)target;
				object o = null;
				dict.TryGetValue(identifier, out o);
				return o;
			}
			else if (srm.AllowDirectAccess && !(target is ISyntaxTreeReturn))
			{
				string memberName = ((srm.WorkMode & MachineWorkMode.AutoUppercaseWhenCLRCalling)
					== MachineWorkMode.AutoUppercaseWhenCLRCalling)
					? ScriptRunningMachine.GetNativeIdentifier(identifier) : identifier;

				PropertyInfo pi = target.GetType().GetProperty(memberName, BindingFlags.Public | BindingFlags.Instance);

				if (pi != null)
				{
					try
					{
						object returnObj = pi.GetValue(target, null);

						if (srm.AutoImportRelationType)
						{
							srm.ImportType(returnObj.GetType());
						}

						return returnObj;
					}
					catch (Exception ex)
					{
						if (srm.IgnoreCLRExceptions)
						{
							// call error, return undefined
							return null;
						}
						else
							throw ex;
					}
				}
				else
				{
					FieldInfo fi = target.GetType().GetField(memberName, BindingFlags.Public | BindingFlags.Instance);

					if (fi != null)
					{
						try
						{
							return fi.GetValue(target);
						}
						catch (Exception ex)
						{
							if (srm.IgnoreCLRExceptions)
							{
								// call error, return undefined
								return null;
							}
							else
								throw ex;
						}
					}
					else
					{
						EventInfo ei = target.GetType().GetEvent(memberName, BindingFlags.Public | BindingFlags.Instance);
						if (ei != null)
						{
							object attachedEventFun = srm.GetAttachedEvent(target, ei);

							// synchronize registed event and property of object
							if (target is ObjectValue)
							{
								((ObjectValue)target)[identifier] = attachedEventFun;
							}

							return attachedEventFun;
						}
						else if (target is ObjectValue)
						{
							return ((ObjectValue)target)[identifier];
						}
					}
				}
			}

			return null;
		}
	}
	#endregion Property Access

	#region String Access
	class StringAccess : AccessValue
	{
		public object StringVariable { get; set; }
		public int Index { get; set; }

		public StringAccess(ScriptRunningMachine srm, ScriptContext ctx, object array, int index)
				: base(srm, ctx)
		{
			this.StringVariable = array;
			this.Index = index;
		}

		#region Access Members
		public override void Set(object value)
		{
			// modifying string is not supported
		}

		public override object Get()
		{
			if (StringVariable is string)
			{
				return Context.CreateNewObject(Srm.BuiltinConstructors.StringFunction, true, new object[] { (StringVariable as string)[Index].ToString() });
			}
			else if (StringVariable is StringObject)
			{
				return Context.CreateNewObject(Srm.BuiltinConstructors.StringFunction, true, new object[] { (StringVariable as StringObject).String[Index].ToString() });
			}
			else if (StringVariable is StringBuilder)
			{
				return Context.CreateNewObject(Srm.BuiltinConstructors.StringFunction, true, new object[] { (StringVariable as StringBuilder)[Index].ToString() });
			}
			else
			{
				return null;
			}
		}
		#endregion Access Members
	}
	#endregion String Access

	#endregion Accessor

	#region Parsers
	namespace Parsers
	{
		#region Parser Interface
		interface INodeParser
		{
			object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx);
		}
		#endregion

		#region Import Statement
		class ImportNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				if (t.Children[0].Type == ReoScriptLexer.STRING_LITERATE)
				{
					string codeFile = t.Children[0].ToString();
					codeFile = codeFile.Substring(1, codeFile.Length - 2);

					string path = Path.GetFullPath(Path.Combine(
						string.IsNullOrEmpty(ctx.SourceFilePath) ? srm.WorkPath
						: Path.GetDirectoryName(ctx.SourceFilePath), codeFile));

					srm.ImportCodeFile(path);
				}
				else if (srm.AllowImportTypeInScript)
				{
					StringBuilder sb = new StringBuilder();
					for (int i = 0; i < t.ChildCount; i++)
					{
						string ns = t.Children[i].ToString();

						if (i == t.ChildCount - 1)
						{
							if (ns == "*")
							{
								srm.ImportNamespace(sb.ToString());
								return null;
							}
							else
							{
								string name = sb.ToString();
								if (name.EndsWith(".")) name = name.Substring(0, name.Length - 1);

								Type type = srm.GetTypeFromAssembly(name, ns);
								if (type != null)
								{
									srm.ImportType(type);
								}
								return null;
							}
						}

						sb.Append(ns);
					}
				}

				return null;
			}

			#endregion
		}
		#endregion
		#region Local Variable Declaration
		class DeclarationNodeParser : INodeParser
		{
			private AssignmentNodeParser assignmentParser = new AssignmentNodeParser();

			#region INodeParser Members
			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext context)
			{
				//for (int i = 0; i < t.ChildCount; i++)
				//{
				VariableInfo vi = ((VariableDefineNode)t).VariableInfo;

				string identifier = vi.Name;
				object value = null;

				if (vi.HasInitialValue)
				{
					value = ScriptRunningMachine.ParseNode(vi.InitialValueTree, context);
					if (value is IAccess) value = ((IAccess)value).Get();
				}

				// declare variable in current call stack
				context[identifier] = value;
				//}
				//for (int i = 1; i < t.ChildCount; i++)
				//{
				//  var identifier = t.Children[0].ToString();
				//  var value = srm.ParseNode((CommonTree)t.Children[i], context);

				//  if (value is IAccess) value = ((IAccess)value).Get();

				//  // declare variable in current call stack
				//  if (srm.IsInGlobalScope(context))
				//  {
				//    srm[identifier] = value;
				//  }
				//  else
				//  {
				//    context.GetCurrentCallScope()[identifier] = value;
				//  }

				//  lastValue = value;
				//}

				return value;
			}
			#endregion
		}
		#endregion
		#region Assignment =
		class AssignmentNodeParser : INodeParser
		{
			private static readonly ExprPlusNodeParser exprPlusNodeParser = new ExprPlusNodeParser();

			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				if (t.ChildCount == 1)
				{
					return null;
				}

				if (t.Children[0].Type == ReoScriptLexer.IDENTIFIER)
				{
					string identifier = t.Children[0].Text;

					IVariableContainer container = null;

					CallScope cs = ctx.CurrentCallScope;

					if (cs != null)
					{
						if (cs.Variables.ContainsKey(identifier))
						{
							container = cs;
						}
						else
						{
							CallScope outerScope = cs.CurrentFunction.OuterCallScope;
							while (outerScope != null)
							{
								if (outerScope.Variables.ContainsKey(identifier))
								{
									container = outerScope;
									break;
								}

								outerScope = outerScope.CurrentFunction.OuterCallScope;
							}
						}
					}

					if (container == null)
					{
						container = ctx.GlobalObject;
					}

					object orginal = null;
					container.TryGetValue(identifier, out orginal);

					object target = ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx);

					if (orginal is ExternalProperty)
					{
						((ExternalProperty)orginal).Setter(target);
					}
					else
					{
						container[identifier] = target;
					}

					return target;
				}
				else
				{
					IAccess access = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx) as IAccess;
					CommonTree expr = t.ChildCount > 1 ? (CommonTree)t.Children[1] : null;

					object value = null;
					if (expr != null)
					{
						value = ScriptRunningMachine.ParseNode(expr, ctx);
					}

					if (value is IAccess) value = ((IAccess)value).Get();

					if (access != null)
					{
						access.Set(value);
					}
					else if (!srm.IsInGlobalScope(ctx))
					{
						ctx[t.Children[0].Text] = value;
					}

					return value;
				}
			}
			#endregion
		}
		#endregion
		#region Expresion Operator
		abstract class ExpressionOperatorNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object left = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx);
				if (left is IAccess) left = ((IAccess)left).Get();

				object right = ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx);
				if (right is IAccess) right = ((IAccess)right).Get();

				return Calc(left, right, srm, ctx);
			}

			public abstract object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context);

			#endregion
		}
		abstract class MathExpressionOperatorParser : ExpressionOperatorNodeParser
		{
			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				if (left == NaNValue.Value || right == NaNValue.Value)
					return NaNValue.Value;

				if (!ScriptRunningMachine.IsPrimitiveNumber(left)) left = 0;
				if (!ScriptRunningMachine.IsPrimitiveNumber(right)) right = 0;

				return MathCalc(ScriptRunningMachine.GetNumberValue(left),
					ScriptRunningMachine.GetNumberValue(right));
			}

			public abstract object MathCalc(double left, double right);
		}
		#region Plus +
		class ExprPlusNodeParser : ExpressionOperatorNodeParser
		{
			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				if (left == null && right == null) return null;

				if (left == null) return right;
				else if (right == null) return left;

				if (left == NaNValue.Value && right == NaNValue.Value) return NaNValue.Value;

				if (ScriptRunningMachine.IsPrimitiveNumber(left))
				{
					if (ScriptRunningMachine.IsPrimitiveNumber(right))
					{
						return ScriptRunningMachine.GetNumberValue(left) + ScriptRunningMachine.GetNumberValue(right);
					}
					else if (right == null)
					{
						return left;
					}
					else if (right == NaNValue.Value)
					{
						return NaNValue.Value;
					}
					else if (right is NumberObject)
					{
						return ScriptRunningMachine.GetNumberValue(left) + ((NumberObject)right).Number;
					}
				}
				else if (ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					if (ScriptRunningMachine.IsPrimitiveNumber(left))
					{
						return ScriptRunningMachine.GetNumberValue(left) + ScriptRunningMachine.GetNumberValue(right);
					}
					else if (left == null)
					{
						return right;
					}
					else if (left == NaNValue.Value)
					{
						return NaNValue.Value;
					}
					else if (left is NumberObject)
					{
						return ((NumberObject)left).Number + ScriptRunningMachine.GetNumberValue(right);
					}
				}

				if (left.GetType() == typeof(ObjectValue) && right.GetType() == typeof(ObjectValue))
				{
					ObjectValue obj = srm.CreateNewObject(context);
					srm.CombineObject(context, obj, ((ObjectValue)left));
					srm.CombineObject(context, obj, ((ObjectValue)right));
					return obj;
				}
				else
				{
					return Convert.ToString(left) + Convert.ToString(right);
				}
			}
		}
		#endregion
		#region Minus -
		class ExprMinusNodeParser : ExpressionOperatorNodeParser
		{
			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				if (left == null && right == null)
					return null;

				if (left == null)
					return right;
				else if (right == null)
					return left;

				if (left == NaNValue.Value && right == NaNValue.Value) return NaNValue.Value;

				if (ScriptRunningMachine.IsPrimitiveNumber(left))
				{
					if (right == null)
					{
						return left;
					}
					else if (right == NaNValue.Value)
					{
						return NaNValue.Value;
					}
					else if (ScriptRunningMachine.IsPrimitiveNumber(right))
					{
						return ScriptRunningMachine.GetNumberValue(left) - ScriptRunningMachine.GetNumberValue(right);
					}
					else if (right is NumberObject)
					{
						return ScriptRunningMachine.GetNumberValue(left) - ((NumberObject)right).Number;
					}
					else if (right is DateObject)
					{
						return ((NumberObject)left).Number - ((DateObject)right).Ticks;
					}
				}
				else if (ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					if (left == null)
					{
						return right;
					}
					else if (left == NaNValue.Value)
					{
						return NaNValue.Value;
					}
					else if (ScriptRunningMachine.IsPrimitiveNumber(left))
					{
						return ScriptRunningMachine.GetNumberValue(left) - ScriptRunningMachine.GetNumberValue(right);
					}
					else if (left is NumberObject)
					{
						return ((NumberObject)left).Number - ScriptRunningMachine.GetNumberValue(right);
					}
					else if (left is DateObject)
					{
						return ((DateObject)left).Ticks - ScriptRunningMachine.GetNumberValue(right);
					}
				}
				else if (left is DateObject && right is DateObject)
				{
					return (((DateObject)left).DateTime - ((DateObject)right).DateTime).TotalMilliseconds;
				}

				return double.NaN;
			}
		}
		#endregion
		#region Mul *
		class ExprMultiNodeParser : MathExpressionOperatorParser
		{
			public override object MathCalc(double left, double right)
			{
				double val = left * right;
				return (double.IsNaN(val)) ? NaNValue.Value : (object)val;
			}
		}
		#endregion
		#region Div /
		class ExprDivNodeParser : MathExpressionOperatorParser
		{
			public override object MathCalc(double left, double right)
			{
				double val = left / right;
				return (double.IsNaN(val)) ? NaNValue.Value : (object)val;
			}
		}
		#endregion
		#region Mod %
		class ExprModNodeParser : MathExpressionOperatorParser
		{
			#region INodeParser Members

			public override object MathCalc(double left, double right)
			{
				return (left % right);
			}

			#endregion
		}
		#endregion
		#region And &
		class ExprAndNodeParser : ExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				if ((left is int || left is double || left is long || left is float)
					&& (right is int || right is double || right is long || right is float))
				{
					return (Convert.ToInt64(left) & Convert.ToInt64(right));
				}
				else
				{
					return NaNValue.Value;
				}
			}

			#endregion
		}
		#endregion
		#region Or |
		class ExprOrNodeParser : ExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				if ((left is int || left is long || left is float || left is double)
					&& (right is int || right is long || right is float || right is double))
				{
					return (Convert.ToInt64(left) | Convert.ToInt64(right));
				}
				else
				{
					return NaNValue.Value;
				}
			}

			#endregion
		}
		#endregion
		#region Xor ^
		class ExprXorNodeParser : ExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				if (left is long && right is long)
				{
					return ((long)left ^ (long)right);
				}
				else if (left is ObjectValue && right is ObjectValue)
				{
					ObjectValue leftObj = (ObjectValue)left;
					ObjectValue rightObj = (ObjectValue)right;

					ObjectValue resultObject = srm.CreateNewObject(context);

					foreach (string key in leftObj)
					{
						if (rightObj.HasOwnProperty(key))
						{
							resultObject[key] = leftObj[key];
						}
					}

					return resultObject;
				}
				else
				{
					return NaNValue.Value;
				}
			}

			#endregion
		}
		#endregion
		#region Left Shift <<
		class ExprLeftShiftNodeParser : ExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				try
				{
					int l = Convert.ToInt32(left);
					int r = Convert.ToInt32(right);

					return l << r;
				}
				catch
				{
					return 0;
				}
			}

			#endregion
		}
		#endregion
		#region Right Shift >>
		class ExprRightShiftNodeParser : ExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				try
				{
					int l = Convert.ToInt32(left);
					int r = Convert.ToInt32(right);

					return l >> r;
				}
				catch
				{
					return 0;
				}
			}

			#endregion
		}
		#endregion
		#endregion
		#region Unary for Primary Expression
		class ExprUnaryNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				string oprt = t.Children[0].ToString();
				object value = ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx);

				// get value
				while (value is IAccess) value = ((IAccess)value).Get();

				if (value == null) return null;

				switch (oprt)
				{
					default:
					case "+":
						return value;

					case "-":
						if (value is long)
							return -((long)value);
						else if (value is double)
							return -((double)value);
						else
							return null;

					case "!":
						if (!(value is bool))
						{
							return null;
						}
						return !((bool)value);

					case "~":
						return (~(Convert.ToInt64(value)));
				}
			}

			#endregion
		}
		#endregion
		#region Post Increment i++
		class ExprPostIncrementNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				if (t.Children[0].Type == ReoScriptLexer.IDENTIFIER)
				{
					string identifier = t.Children[0].Text;

					IVariableContainer container = null;

					CallScope cs = ctx.CurrentCallScope;

					if (cs != null)
					{
						if (cs.Variables.ContainsKey(identifier))
						{
							container = cs;
						}
						else
						{
							CallScope outerScope = cs.CurrentFunction.OuterCallScope;
							while (outerScope != null)
							{
								if (outerScope.Variables.ContainsKey(identifier))
								{
									container = outerScope;
									break;
								}

								outerScope = outerScope.CurrentFunction.OuterCallScope;
							}
						}
					}

					if (container == null)
					{
						container = ctx.GlobalObject;
					}

					object orginal = null;
					container.TryGetValue(identifier, out orginal);

					if (!ScriptRunningMachine.IsPrimitiveNumber(orginal))
					{
						throw ctx.CreateRuntimeError(t, "only number can be used as increment or decrement statement.");
					}

					double target = ScriptRunningMachine.GetNumberValue(orginal) + (t.Children[1].Type == ReoScriptLexer.INCREMENT ? 1 : -1);

					if (orginal is ExternalProperty)
					{
						((ExternalProperty)orginal).Setter(target);
					}
					else
					{
						container[identifier] = target;
					}

					return orginal;
				}
				else
				{

					IAccess access = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx) as IAccess;

					if (access == null)
					{
						throw ctx.CreateRuntimeError(t, "only property, indexer, and variable can be used as increment or decrement statement.");
					}

					object oldValue = access.Get();
					if (oldValue == null)
					{
						oldValue = 0;
					}

					if (!ScriptRunningMachine.IsPrimitiveNumber(oldValue))
					{
						throw ctx.CreateRuntimeError(t, "only number can be used as increment or decrement statement.");
					}

					double value = ScriptRunningMachine.GetNumberValue(oldValue);
					double returnValue = value;
					access.Set((value + (t.Children[1].Type == ReoScriptLexer.INCREMENT ? 1 : -1)));
					return returnValue;
				}
			}

			#endregion
		}
		#endregion
		#region Pre Increment ++i
		class ExprPreIncrementNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				if (t.Children[0].Type == ReoScriptLexer.IDENTIFIER)
				{
					string identifier = t.Children[0].Text;

					IVariableContainer container = null;

					CallScope cs = ctx.CurrentCallScope;

					if (cs != null)
					{
						if (cs.Variables.ContainsKey(identifier))
						{
							container = cs;
						}
						else
						{
							CallScope outerScope = cs.CurrentFunction.OuterCallScope;
							while (outerScope != null)
							{
								if (outerScope.Variables.ContainsKey(identifier))
								{
									container = outerScope;
									break;
								}

								outerScope = outerScope.CurrentFunction.OuterCallScope;
							}
						}
					}

					if (container == null)
					{
						container = ctx.GlobalObject;
					}

					object orginal = null;
					container.TryGetValue(identifier, out orginal);

					if (!ScriptRunningMachine.IsPrimitiveNumber(orginal))
					{
						throw ctx.CreateRuntimeError(t, "only number can be used as increment or decrement statement.");
					}

					double target = ScriptRunningMachine.GetNumberValue(orginal) + (t.Children[1].Type == ReoScriptLexer.INCREMENT ? 1 : -1);

					if (orginal is ExternalProperty)
					{
						((ExternalProperty)orginal).Setter(target);
					}
					else
					{
						container[identifier] = target;
					}

					return target;
				}
				else
				{
					CommonTree target = (CommonTree)t.Children[0];
					IAccess access = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx) as IAccess;
					if (access == null)
					{
						throw ctx.CreateRuntimeError(t, "only property, indexer, and variable can be used as increment or decrement statement.");
					}
					object oldValue = access.Get();
					if (oldValue == null)
					{
						oldValue = 0;
					}

					if (!ScriptRunningMachine.IsPrimitiveNumber(oldValue))
					{
						throw ctx.CreateRuntimeError(t, "only number can be used as increment or decrement statement.");
					}

					double value = ScriptRunningMachine.GetNumberValue(oldValue);

					object v = (value + (t.Children[1].Type == ReoScriptLexer.INCREMENT ? 1 : -1));
					access.Set(v);
					return v;
				}
			}

			#endregion
		}
		#endregion
		#region Condition ? :
		class ExprConditionNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object value = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx);
				if (!(value is bool))
				{
					throw ctx.CreateRuntimeError(t, "only boolean expression can be used for conditional expression.");
				}
				bool condition = (bool)value;
				return condition ? ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx)
					: ScriptRunningMachine.ParseNode((CommonTree)t.Children[2], ctx);
			}

			#endregion
		}
		#endregion
		#region Relation Expression Operator
		abstract class RelationExpressionOperatorNodeParser : ExpressionOperatorNodeParser
		{
			#region INodeParser Members
			public override object Calc(object left, object right, ScriptRunningMachine srm, ScriptContext context)
			{
				return Compare(left, right, srm);
			}

			public abstract bool Compare(object left, object right, ScriptRunningMachine srm);
			#endregion
		}

		#region Equals ==
		class ExprEqualsNodeParser : RelationExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				if (left == null && right == null) return true;
				if (left == null || right == null) return false;
				if (left == NaNValue.Value && right == NaNValue.Value) return true;
				if (left == NaNValue.Value || right == NaNValue.Value) return false;

				if (left is string || left is StringObject)
				{
					return Convert.ToString(left).Equals(Convert.ToString(right));
				}
				else if (right is string || right is StringObject)
				{
					return Convert.ToString(right).Equals(Convert.ToString(left));
				}
				else
				{
					if (ScriptRunningMachine.IsPrimitiveNumber(left) && ScriptRunningMachine.IsPrimitiveNumber(right))
					{
						if (left is float || right is float)
						{
							return Convert.ToSingle(left) == Convert.ToSingle(right);
						}
						else
						{
							return ScriptRunningMachine.GetNumberValue(left) == ScriptRunningMachine.GetNumberValue(right);
						}
					}

					else if (left is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(right))
					{
						return ((NumberObject)left).Number == ScriptRunningMachine.GetNumberValue(right);
					}
					else if (right is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(left))
					{
						return ScriptRunningMachine.GetNumberValue(left) == ((NumberObject)right).Number;
					}
					else if (left is NumberObject && right is NumberObject)
					{
						return ((NumberObject)left).Number == ((NumberObject)right).Number;
					}

					else if (left is BooleanObject && right is bool)
					{
						return ((BooleanObject)left).Boolean == (bool)right;
					}
					else if (right is BooleanObject && left is bool)
					{
						return ((BooleanObject)right).Boolean == (bool)left;
					}
					else if (left is BooleanObject && right is BooleanObject)
					{
						return ((BooleanObject)left).Boolean == ((BooleanObject)right).Boolean;
					}
					else
					{
						return left.Equals(right);
					}
				}
			}

			#endregion
		}
		#endregion
		#region Not Equals !=
		class ExprNotEqualsNodeParser : RelationExpressionOperatorNodeParser
		{
			private static readonly ExprEqualsNodeParser equalsParser = new ExprEqualsNodeParser();
			#region INodeParser Members

			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				return !equalsParser.Compare(left, right, srm);
			}

			#endregion
		}
		#endregion
		#region Strict Equals ===
		class ExprStrictEqualsNodeParser : RelationExpressionOperatorNodeParser
		{
			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				if (left == null && right == null) return true;
				if (left == null || right == null) return false;

				if (ScriptRunningMachine.IsPrimitiveNumber(left) && ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					if (left is float || right is float)
					{
						return Convert.ToSingle(left) == Convert.ToSingle(right);
					}
					else
					{
						return ScriptRunningMachine.GetNumberValue(left) == ScriptRunningMachine.GetNumberValue(right);
					}
				}
				else if ((left is string && right is string) || (left is bool && right is bool))
				{
					return left.Equals(right);
				}
				else
				{
					return left == right;
				}
			}
		}
		#endregion
		#region Strict Not Equals !==
		class ExprStrictNotEqualsNodeParser : RelationExpressionOperatorNodeParser
		{
			private ExprStrictEqualsNodeParser equalsParser = new ExprStrictEqualsNodeParser();

			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				return !equalsParser.Compare(left, right, srm);
				//return left != right;
			}
		}
		#endregion

		#region Greater Than >
		class ExprGreaterThanNodeParser : RelationExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				if (left is float || right is float)
				{
					return Convert.ToSingle(left) > Convert.ToSingle(right);
				}
				else if (ScriptRunningMachine.IsPrimitiveNumber(left) && ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					return ScriptRunningMachine.GetNumberValue(left) > ScriptRunningMachine.GetNumberValue(right);
				}
				else if (left is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					return ((NumberObject)left).Number > ScriptRunningMachine.GetNumberValue(right);
				}
				else if (right is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(left))
				{
					return ScriptRunningMachine.GetNumberValue(left) > ((NumberObject)right).Number;
				}
				else if (ScriptRunningMachine.IsPrimitiveString(left) && ScriptRunningMachine.IsPrimitiveString(right))
				{
					return string.Compare(ScriptRunningMachine.ConvertToString(left), ScriptRunningMachine.ConvertToString(right)) > 0;
				}
				else
				{
					return false;
				}
			}

			#endregion
		}
		#endregion
		#region Greater Or Equals >=
		class ExprGreaterOrEqualsNodeParser : RelationExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				if (left is float || right is float)
				{
					return Convert.ToSingle(left) >= Convert.ToSingle(right);
				}
				else if (ScriptRunningMachine.IsPrimitiveNumber(left) && ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					return ScriptRunningMachine.GetNumberValue(left) >= ScriptRunningMachine.GetNumberValue(right);
				}
				else if (left is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					return ((NumberObject)left).Number >= ScriptRunningMachine.GetNumberValue(right);
				}
				else if (right is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(left))
				{
					return ScriptRunningMachine.GetNumberValue(left) >= ((NumberObject)right).Number;
				}
				else if (ScriptRunningMachine.IsPrimitiveString(left) && ScriptRunningMachine.IsPrimitiveString(right))
				{
					return string.Compare(ScriptRunningMachine.ConvertToString(left), ScriptRunningMachine.ConvertToString(right)) >= 0;
				}
				else
				{
					return false;
				}
			}

			#endregion
		}
		#endregion
		#region Less Than <
		class ExprLessThanNodeParser : RelationExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				if (ScriptRunningMachine.IsPrimitiveNumber(left))
				{
					if (ScriptRunningMachine.IsPrimitiveNumber(right))
					{
						if (left is float || right is float)
						{
							return Convert.ToSingle(left) < Convert.ToSingle(right);
						}
						else
						{
							return ScriptRunningMachine.GetNumberValue(left) < ScriptRunningMachine.GetNumberValue(right);
						}
					}
					else if (right is NumberObject)
					{
						return ScriptRunningMachine.GetNumberValue(left) < ((NumberObject)right).Number;
					}
					else if (right is string)
					{
						double d = 0;
						if (double.TryParse(Convert.ToString(right), out d))
							return ScriptRunningMachine.GetNumberValue(left) < d;
						else
							return false;
					}
				}
				else if (ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					if (ScriptRunningMachine.IsPrimitiveNumber(left))
					{
						if (left is float || right is float)
						{
							return Convert.ToSingle(left) < Convert.ToSingle(right);
						}
						else
						{
							return ScriptRunningMachine.GetNumberValue(left) < ScriptRunningMachine.GetNumberValue(right);
						}
					}
					else if (left is NumberObject)
					{
						return ((NumberObject)left).Number < ScriptRunningMachine.GetNumberValue(right);
					}
					else if (ScriptRunningMachine.IsPrimitiveString(left))
					{
						double d;
						if (double.TryParse(ScriptRunningMachine.ConvertToString(left), out d))
							return d < ScriptRunningMachine.GetNumberValue(right);
						else
							return false;
					}
				}
				else if (ScriptRunningMachine.IsPrimitiveString(left) && ScriptRunningMachine.IsPrimitiveString(right))
				{
					return string.Compare(ScriptRunningMachine.ConvertToString(left), ScriptRunningMachine.ConvertToString(right)) < 0;
				}

				return false;
			}

			#endregion
		}
		#endregion
		#region Less Or Equals <=
		class ExprLessOrEqualsNodeParser : RelationExpressionOperatorNodeParser
		{
			#region INodeParser Members

			public override bool Compare(object left, object right, ScriptRunningMachine srm)
			{
				if (ScriptRunningMachine.IsPrimitiveNumber(left) && ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					if (left is float || right is float)
						return Convert.ToSingle(left) <= Convert.ToSingle(right);
					else
						return ScriptRunningMachine.GetNumberValue(left) <= ScriptRunningMachine.GetNumberValue(right);
				}
				else if (left is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(right))
				{
					return ((NumberObject)left).Number <= ScriptRunningMachine.GetNumberValue(right);
				}
				else if (right is NumberObject && ScriptRunningMachine.IsPrimitiveNumber(left))
				{
					return ScriptRunningMachine.GetNumberValue(left) <= ((NumberObject)right).Number;
				}
				else if (ScriptRunningMachine.IsPrimitiveString(left) && ScriptRunningMachine.IsPrimitiveString(right))
				{
					return string.Compare(ScriptRunningMachine.ConvertToString(left), ScriptRunningMachine.ConvertToString(right)) <= 0;
				}
				else
				{
					return false;
				}
			}

			#endregion
		}
		#endregion
		#endregion
		#region Boolean &&
		class BooleanAndNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object left = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx);
				if (left is IAccess) left = ((IAccess)left).Get();

				if (left == null || !(left is bool))
					return false;

				bool leftBool = (bool)left;
				if (!leftBool) return false;

				object right = ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx);
				if (right is IAccess) right = ((IAccess)right).Get();

				if (right == null || !(right is bool))
					return false;

				bool rightBool = (bool)right;
				return rightBool;
			}

			#endregion
		}
		#endregion
		#region Boolean ||
		class BooleanOrNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object left = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx);
				if (left is IAccess) left = ((IAccess)left).Get();

				if (left == null || !(left is bool))
					return false;

				bool leftBool = (bool)left;
				if (leftBool) return true;

				object right = ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx);
				if (right is IAccess) right = ((IAccess)right).Get();

				if (right == null || !(right is bool))
					return false;

				bool rightBool = (bool)right;
				return rightBool;
			}

			#endregion
		}
		#endregion
		#region If Else
		class IfStatementNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object value = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx);
				if (value is IAccess) value = ((IAccess)value).Get();

				if (!(value is bool))
				{
					return false;
					//throw new AWDLRuntimeException("only boolean expression can be used as test condition.");
				}
				bool condition = (bool)value;
				if (condition)
				{
					return ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx);
				}
				else if (t.ChildCount == 3)
				{
					return ScriptRunningMachine.ParseNode((CommonTree)t.Children[2], ctx);
				}
				else
					return null;
			}

			#endregion
		}
		#endregion
		#region Switch Case
		class SwitchCaseStatementNodeParser : INodeParser
		{
			private ExprEqualsNodeParser equalsParser = new ExprEqualsNodeParser();

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				if (t.ChildCount == 0) return null;

				object source = ScriptRunningMachine.ParseNode((CommonTree)(t.Children[0]), ctx);
				while (source is IAccess) source = ((IAccess)source).Get();

				if (source == null) return null;

				int defaultCaseLine = 0;
				bool doParse = false;

				int i = 1;

			doDefault:
				while (i < t.ChildCount)
				{
					CommonTree caseTree = (CommonTree)t.Children[i];

					if (caseTree.Type == ReoScriptLexer.BREAK)
					{
						if (doParse) return null;
					}
					else if (caseTree.Type == ReoScriptLexer.RETURN)
					{
						if (doParse) return ScriptRunningMachine.ParseNode(caseTree, ctx);
					}
					else if (caseTree.Type == ReoScriptLexer.SWITCH_CASE_ELSE)
					{
						defaultCaseLine = i;
					}
					else if (caseTree.Type == ReoScriptLexer.SWITCH_CASE)
					{
						if (caseTree.ChildCount > 0)
						{
							object target = ScriptRunningMachine.ParseNode((CommonTree)caseTree.Children[0], ctx);
							if (target is IAccess) target = ((IAccess)target).Get();

							if ((bool)equalsParser.Calc(source, target, srm, ctx))
							{
								doParse = true;
							}
						}
					}
					else if (doParse)
					{
						ScriptRunningMachine.ParseNode(caseTree, ctx);
					}

					i++;
				}

				if (defaultCaseLine > 0)
				{
					i = defaultCaseLine;
					doParse = true;
					goto doDefault;
				}

				return null;
			}
		}
		#endregion
		#region For
		class ForStatementNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				CommonTree forInit = (CommonTree)t.Children[0];
				ScriptRunningMachine.ParseChildNodes(forInit, ctx);

				CommonTree conditionTree = ((CommonTree)t.Children[1]);
				CommonTree condition = null;

				if (conditionTree.ChildCount > 0)
				{
					condition = (CommonTree)(conditionTree.Children[0]);
				}

				CommonTree iteratorTree = ((CommonTree)t.Children[2]);
				CommonTree body = (CommonTree)(((CommonTree)t.Children[3]).Children[0]);

				bool hasCondition = condition != null && condition.ChildCount > 0;
				bool hasBody = body.ChildCount > 0;

				while (true)
				{
					if (hasCondition)
					{
						//object conditionValue = i++ < 500000;
						//object conditionValue = i++ < (double)ScriptRunningMachine.ParseNode((CommonTree)(condition.Children[1]), ctx);

						//object conditionValue = (double)ctx[condition.Children[0].Text] < 500000;
						object conditionValue = ScriptRunningMachine.ParseNode(condition, ctx);

						bool rs = false;

						if (conditionValue is bool)
						{
							rs = (bool)conditionValue;
						}
						else if (conditionValue is BooleanObject)
						{
							rs = ((BooleanObject)conditionValue).Boolean;
						}
						else
						{
							throw ctx.CreateRuntimeError(t, "only boolean expression can be used as test condition.");
						}

						if (!rs) return null;
					}

					if (hasBody)
					{
						object result = ScriptRunningMachine.ParseNode(body, ctx);

						if (result is BreakNode)
						{
							return null;
						}
						else if (result is ReturnNode)
						{
							return result;
						}
					}

					ScriptRunningMachine.ParseChildNodes(iteratorTree, ctx);
				}
			}

			#endregion
		}
		public class JITForStatementNodeParser : INodeParser
		{
			#region INodeParser Members
			private static long count = 0;

			//private struct ForSession
			//{
			//  public CommonTree Condition;
			//  public CommonTree Body;
			//  public CommonTree Iterators;
			//  public ScriptContext ctx;
			//}

			//public static object ParseCondition(ForSession ses)
			//{
			//  return ScriptRunningMachine.ParseNode(ses.Condition, ses.ctx);
			//}

			private static readonly MethodInfo _ParseNode = typeof(ScriptRunningMachine).GetMethod("ParseNode");
			private static readonly MethodInfo _ParseChildNodes = typeof(ScriptRunningMachine).GetMethod("ParseChildNodes");

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				CommonTree forInit = (CommonTree)t.Children[0];
				ScriptRunningMachine.ParseChildNodes(forInit, ctx);

				CommonTree condition = ((CommonTree)t.Children[1]);

				bool test = ScriptRunningMachine.GetBoolValue(ScriptRunningMachine.ParseNode(condition, ctx));
				if (!test) return null;

				DynamicMethod dm = new DynamicMethod("__for$" + count++, typeof(object),
					new Type[] { typeof(ScriptContext), typeof(CommonTree), typeof(CommonTree), typeof(CommonTree) });
				ILGenerator il = dm.GetILGenerator();

				CommonTree body = (CommonTree)(((CommonTree)t.Children[3]).Children[0]);
				CommonTree iterators = ((CommonTree)t.Children[2]);

				System.Reflection.Emit.Label start = il.DefineLabel();
				System.Reflection.Emit.Label end = il.DefineLabel();
				//LocalBuilder local = il.DeclareLocal(typeof(object[]));

				//ForSession ses=  new ForSession(){
				//  Condition = condition,
				//  Body= body,
				//  Iterators=iterators,};

				//object[] forses = new object[]{ ctx, condition, body, iterators};

				il.MarkLabel(start);

				// condition
				il.Emit(OpCodes.Ldarg_1);
				il.Emit(OpCodes.Ldarg_0);
				il.Emit(OpCodes.Call, _ParseNode);

				il.Emit(OpCodes.Unbox_Any, typeof(bool));
				il.Emit(OpCodes.Brfalse_S, end);

				// body
				il.Emit(OpCodes.Ldarg_2);
				il.Emit(OpCodes.Ldarg_0);
				il.Emit(OpCodes.Call, _ParseNode);
				il.Emit(OpCodes.Pop);

				// iterators
				il.Emit(OpCodes.Ldarg_3);
				il.Emit(OpCodes.Ldarg_0);
				il.Emit(OpCodes.Call, _ParseChildNodes);
				il.Emit(OpCodes.Pop);

				//// loop
				il.Emit(OpCodes.Br_S, start);

				il.MarkLabel(end);
				il.Emit(OpCodes.Ldnull);
				il.Emit(OpCodes.Ret);

				dm.Invoke(null, new object[] { ctx, condition, body, iterators });

				return null;
			}

			#endregion
		}

		#endregion
		#region Foreach
		class ForEachStatementNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				string varName = t.Children[0].ToString();

				CallScope scope = null;

				if (t.ChildCount > 3 && t.Children[3].Type == ReoScriptLexer.TYPE
					&& !srm.IsInGlobalScope(ctx))
				{
					scope = ctx.CurrentCallScope;
				}

				CommonTree body = (CommonTree)t.Children[2];

				// retrieve target object
				object target = ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], ctx);
				if (target is IAccess) target = ((IAccess)target).Get();

				if (target is IDictionary<string, object>)
				{
					IDictionary<string, object> dict = (IDictionary<string, object>)target;

					foreach (string key in dict.Keys)
					{
						if (scope == null)
						{
							srm[varName] = key;
						}
						else
						{
							scope[varName] = key;
						}

						// call iterator and terminal loop if break
						object result = ScriptRunningMachine.ParseNode(body, ctx);
						if (result is BreakNode)
						{
							return null;
						}
						else if (result is ReturnNode)
						{
							return result;
						}
					}
				}
				else if (target is IEnumerable)
				{
					IEnumerator iterator = ((IEnumerable)target).GetEnumerator();

					while (iterator.MoveNext())
					{
						// prepare key
						object value = iterator.Current;

						if (scope == null)
						{
							ctx.Srm[varName] = value;
						}
						else
						{
							scope[varName] = value;
						}

						// call iterator and terminal loop if break
						object result = ScriptRunningMachine.ParseNode(body, ctx);
						if (result is BreakNode)
						{
							return null;
						}
						else if (result is ReturnNode)
						{
							return result;
						}
					}
				}

				return null;
			}

			#endregion

		}

		#endregion
		#region Return
		class ReturnNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object v = null;

				if (t.ChildCount > 0)
				{
					v = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx);
					if (v is IAccess) v = ((IAccess)v).Get();
				}

				// TODO: make ReturnNode single instance
				return new ReturnNode(v);
			}

			#endregion
		}
		#endregion
		#region Try Catch
		class TryCatchNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				CommonTree tryBody = t.Children[0] as CommonTree;

				string errorObjIdentifier = null;

				CommonTree catchNode = t.Children[1] as CommonTree;
				CommonTree catchBody = catchNode.Children[0] as CommonTree;

				if (catchNode.ChildCount > 1)
				{
					errorObjIdentifier = catchNode.Children[1].Text;
				}

				CommonTree finallyNode = t.Children[2] as CommonTree;
				CommonTree finallyBody = finallyNode.ChildCount > 0 ? finallyNode.Children[0] as CommonTree : null;

				object ret = null;

				try
				{
					ret = ScriptRunningMachine.ParseNode(tryBody, ctx);
				}
				catch (ReoScriptException ex)
				{
					if (catchBody != null)
					{
						if (!string.IsNullOrEmpty(errorObjIdentifier))
						{
							ctx[errorObjIdentifier] = ex.ErrorObject.CustomeErrorObject == null ? ex.ErrorObject : ex.ErrorObject.CustomeErrorObject;

							try
							{
								ret = ScriptRunningMachine.ParseNode(catchBody, ctx);
							}
							finally
							{
								ctx.RemoveVariable(errorObjIdentifier);
							}
						}
						else
						{
							ret = ScriptRunningMachine.ParseNode(catchBody, ctx);
						}
					}
				}
				finally
				{
					if (finallyBody != null) ret = ScriptRunningMachine.ParseNode(finallyBody, ctx);
				}

				return ret;
			}

			#endregion
		}
		#endregion
		#region Throw
		class ThrowNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object obj = ScriptRunningMachine.ParseNode(t.Children[0] as CommonTree, ctx);
				if (obj is IAccess) obj = ((IAccess)obj).Get();

				ErrorObject err = ctx.CreateErrorObject(t, Convert.ToString(obj));
				err.CustomeErrorObject = obj;
				throw new ReoScriptRuntimeException(err);
			}

			#endregion
		}
		#endregion
		#region Function Define
		class FunctionDefineNodeParser : INodeParser
		{
			private AssignmentNodeParser assignParser = new AssignmentNodeParser();

			#region INodeParser Members
			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext context)
			{
				FunctionObject fun = CreateAndInitFunction(context, ((FunctionDefineNode)t).FunctionInfo);
				srm[fun.FunName] = fun;
				return fun;
			}
			#endregion

			internal static FunctionObject CreateAndInitFunction(ScriptContext context, FunctionInfo fi)
			{
				ScriptRunningMachine srm = context.Srm;

				FunctionObject fun = srm.CreateNewObject(context,
					srm.BuiltinConstructors.FunctionFunction) as FunctionObject;

				if (fun == null) return null;

				ObjectValue prototype = srm.CreateNewObject(context, srm.BuiltinConstructors.ObjectFunction) as ObjectValue;
				prototype[ScriptRunningMachine.KEY___PROTO__] = fun[ScriptRunningMachine.KEY___PROTO__];

				fun[ScriptRunningMachine.KEY_PROTOTYPE] = prototype;

				if (!fi.IsAnonymous) fun.FunName = fi.Name;
				fun.Args = fi.Args;
				fun.Body = fi.BodyTree;
				fun.FunctionInfo = fi;
				//fun.OuterStack = context.GetCurrentCallScope();

				//AbstractFunctionObject inFun = context.GetCurrentCallScope().CurrentFunction;
				//if(inFun is FunctionObject){
				//  FunctionObject calledInFunction = (FunctionObject)inFun;
				//  fun.DeclaredFilePath = calledInFunction.DeclaredFilePath;
				return fun;
			}
		}
		#endregion
		#region Anonymous Function
		class AnonymousFunctionNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				FunctionObject fun = FunctionDefineNodeParser.CreateAndInitFunction(ctx,
					((AnonymousFunctionDefineNode)t).FunctionInfo);
				fun.OuterCallScope = ctx.CurrentCallScope;
				return fun;
			}

			#endregion
		}

		#endregion
		#region Function Call
		class FunctionCallNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext context)
			{
				object funObj = null;
				object ownerObj = null;

				// local function call
				if (t.Children[0].Type == ReoScriptLexer.IDENTIFIER)
				{
					string funName = t.Children[0].ToString();
					funObj = context[funName];

					if (funObj == null && context.CurrentCallScope != null)
					{
						CallScope outerScope = context.CurrentCallScope.CurrentFunction.OuterCallScope;
						while (outerScope != null)
						{
							if (outerScope.Variables.TryGetValue(funName, out funObj))
							{
								break;
							}

							outerScope = outerScope.CurrentFunction.OuterCallScope;
						}
					}
				}
				else
				{
					// object method call
					funObj = ScriptRunningMachine.ParseNode(t.Children[0] as CommonTree, context);

					if (funObj is PropertyAccess)
					{
						ownerObj = ((PropertyAccess)funObj).Object;
						string methodName = ((PropertyAccess)funObj).Identifier;

						if (!ScriptRunningMachine.IsPrimitiveTypes(ownerObj) && !(ownerObj is ISyntaxTreeReturn))
						{
							if (!srm.AllowDirectAccess)
							{
								// owner object is not ReoScript object and DirectAccess is disabled.
								// there is nothing can do so just return undefined.
								return null;
							}
							else
							{
								if (srm.AllowDirectAccess && !(ownerObj is ISyntaxTreeReturn))
								{
									object[] args = srm.GetParameterList(
											(t.ChildCount <= 1 ? null : t.Children[1] as CommonTree), context);

									methodName = ((srm.WorkMode & MachineWorkMode.AutoUppercaseWhenCLRCalling)
										== MachineWorkMode.AutoUppercaseWhenCLRCalling)
										? ScriptRunningMachine.GetNativeIdentifier(methodName) : methodName;

									MethodInfo mi = ScriptRunningMachine.FindCLRMethodAmbiguous(ownerObj, methodName, args);

									if (mi != null)
									{
										ParameterInfo[] paramTypeList = mi.GetParameters();

										try
										{
											object[] convertedArgs = new object[args.Length];
											for (int i = 0; i < convertedArgs.Length; i++)
											{
												convertedArgs[i] = srm.ConvertToCLRType(context, args[i], paramTypeList[i].ParameterType);
											}
											return mi.Invoke(ownerObj, convertedArgs);
										}
										catch (Exception ex)
										{
											if (srm.IgnoreCLRExceptions)
											{
												// call error, return undefined
												return null;
											}
											else
												throw ex;
										}
									}
								}
							}
						}

						funObj = ((IAccess)funObj).Get();

						if (funObj == null)
						{
							throw context.CreateRuntimeError(t, string.Format("{0} has no method '{1}'", ownerObj, methodName));
						}
					}
					else
					{
						if (funObj is IAccess) funObj = ((IAccess)funObj).Get();
					}
				}

				if (funObj == null)
				{
					throw context.CreateRuntimeError(t, "Function is not defined: " + t.Children[0].ToString());
				}

				AbstractFunctionObject fun = funObj as AbstractFunctionObject;

				if (fun == null)
				{
					throw context.CreateRuntimeError(t, "Object is not a function: " + Convert.ToString(funObj));
				}

				if (ownerObj == null) ownerObj = context.ThisObject;

				CommonTree argTree = t.ChildCount < 2 ? null : t.Children[1] as CommonTree;

				try
				{
					return srm.InvokeFunction(context, ownerObj, fun, srm.GetParameterList(argTree, context), t.CharPositionInLine, t.Line);
				}
				catch (Exception ex)
				{
					ReoScriptException rex = ex as ReoScriptException;

					if (rex != null && rex.ErrorObject == null)
					{
						rex.ErrorObject = context.CreateErrorObject(t, ex.Message);
					}
					else if (rex == null)
					{
						ex = new ReoScriptRuntimeException(context.CreateErrorObject(t, ex.Message), ex);
					}

					throw ex;
				}
			}

			#endregion
		}
		#endregion
		#region Create
		class CreateObjectNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext context)
			{
				// combiled construct
				if (t.ChildCount > 0 && t.Children[0].Type == ReoScriptLexer.COMBINE_OBJECT)
				{
					CommonTree combileTree = ((CommonTree)t.Children[0]);
					ObjectValue combileObj = ScriptRunningMachine.ParseNode(combileTree.Children[1] as CommonTree, context) as ObjectValue;

					object createdObj = Parse(combileTree.Children[0] as CommonTree, srm, context);
					srm.CombineObject(context, createdObj, combileObj);
					return createdObj;
				}

				CommonTree tempTree = t.Children[0] as CommonTree;
				CommonTree constructTree = t;

				// need a depth variable to remember the depth of construct calling
				int committedDepth = 0, depth = 0;

				// find construct calling
				while (tempTree != null && tempTree.Children != null)
				{
					if (tempTree.Type == ReoScriptLexer.FUNCTION_CALL)
					{
						constructTree = tempTree;
						committedDepth += depth;
					}

					tempTree = tempTree.Children[0] as CommonTree;
					depth++;
				}

				if (constructTree == null) throw context.CreateRuntimeError(t, "unexpected end to new operator.");

				// get constructor if it is need to retrieve from other Accessors
				object constructorValue = ScriptRunningMachine.ParseNode((CommonTree)constructTree.Children[0], context);

				// get identifier of constructor
				string constructorName = constructTree.Children[0].Type == ReoScriptLexer.IDENTIFIER
					? constructTree.Children[0].Text : ScriptRunningMachine.KEY_UNDEFINED;

				if (constructorValue is IAccess) constructorValue = ((IAccess)constructorValue).Get();

				if (constructorValue == null)
				{
					constructorValue = srm.GetClass(constructorName);
				}

				if (constructorValue == null && srm.AllowDirectAccess)
				{
					Type type = srm.GetImportedTypeFromNamespaces(constructorName);
					if (type != null)
					{
						constructorValue = new TypedNativeFunctionObject(type, type.Name);
					}
				}

				if (constructorValue == null)
				{
					throw context.CreateRuntimeError(t, "Constructor not found: " + constructorName);
				}

				if (!(constructorValue is AbstractFunctionObject))
				{
					throw context.CreateRuntimeError(t, "Constructor is not of function: " + constructorName);
				}
				else
				{
					// call constructor
					AbstractFunctionObject funObj = (AbstractFunctionObject)constructorValue;

					CommonTree argTree = (constructTree == null || constructTree.ChildCount < 2) ? null : constructTree.Children[1] as CommonTree;
					object[] args = srm.GetParameterList(argTree, context);

					object obj = srm.CreateNewObject(context, funObj, constructArguments: args);

					// committedDepth > 0 means there is some primaryExpressions are remaining.
					// replace current construction tree and call srm to execute the remaining.
					if (committedDepth > 0)
					{
						CommonTreeAdaptor ad = new CommonTreeAdaptor();
						CommonTree newTree = ad.DupTree(t) as CommonTree;

						int d = 0;
						CommonTree ct = newTree.Children[0] as CommonTree;
						while (d++ < committedDepth - 1)
						{
							ct = ct.Children[0] as CommonTree;
						}

						// replace construction tree with created object
						ct.ReplaceChildren(0, 0, new ReplacedCommonTree(obj));

						// drop the construction tree topmost node [CREATE]
						newTree = newTree.Children[0] as CommonTree;

						// execute remained nodes than construction tree
						return ScriptRunningMachine.ParseNode(newTree, context);
					}
					else
						return obj;
				}
			}

			#endregion
		}
		#endregion
		#region ArrayAccess
		class IndexAccessNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext context)
			{
				object value = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], context);
				if (value is IAccess) value = ((IAccess)value).Get();

				if (value == null)
				{
					throw context.CreateRuntimeError(t, "Attempt to access an array or object that is null or undefined.");
				}

				object indexValue = ScriptRunningMachine.ParseNode((CommonTree)t.Children[1], context);
				if (indexValue is IAccess) indexValue = ((IAccess)indexValue).Get();

				if (value is IList)
				{
					// index access for array
					return new ArrayAccess(srm, context, (value as IList), ScriptRunningMachine.GetIntValue(indexValue));
				}
				else if (value is ObjectValue)
				{
					// index access for object
					return new PropertyAccess(srm, context, value, ScriptRunningMachine.ConvertToString(indexValue));
				}
				else if (value is string || value is StringObject || value is StringBuilder)
				{
					// index access for string
					return new StringAccess(srm, context, value, ScriptRunningMachine.GetIntValue(indexValue));
				}
				else if (indexValue is string)
				{
					// index access for object
					return new PropertyAccess(srm, context, value, (indexValue as string));
				}
				else
				{
					return null;
				}
			}

			#endregion
		}
		#endregion
		#region PropertyAccess
		class PropertyAccessNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object value = null;

				value = ScriptRunningMachine.ParseNode((CommonTree)t.Children[0], ctx);
				if (value is IAccess) value = ((IAccess)value).Get();

				if (value == null) throw ctx.CreateRuntimeError(t,
					"Attempt to access property of null or undefined object" +
					((t.Children[0].Type == ReoScriptLexer.IDENTIFIER)
					? (": " + t.Children[0].ToString()) : "."));

				string identifier = t.Children[1].Text;

				if (ScriptRunningMachine.IsPrimitiveNumber(value) || value is string || value is bool || value is IList)
				{
					// no need check
				}
				else if (!(value is ObjectValue))
				{
					if (value is ISyntaxTreeReturn)
					{
						throw ctx.CreateRuntimeError(t,
							string.Format("Attempt to access an object '{0}' that is not of Object type.", value.ToString()));
					}
					else if (!srm.AllowDirectAccess)
					{
						throw ctx.CreateRuntimeError(t, string.Format(
							"Attempt to access an object '{0}' that is not of Object type. To access .NET object, set the WorkMode to allow DirectAccess.",
							value.ToString()));
					}
				}

				return new PropertyAccess(srm, ctx, value, identifier);
			}

			#endregion
		}
		#endregion
		#region Delete Property
		class DeletePropertyNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				t = t.Children[0] as CommonTree;
				if (t == null) return false;

				if (t.Type == ReoScriptLexer.IDENTIFIER)
				{
					string identifier = t.Text;
					if (ctx.GlobalObject.HasOwnProperty(identifier))
					{
						ctx.GlobalObject.RemoveOwnProperty(identifier);
						return true;
					}
				}
				else if (t.Type == ReoScriptLexer.PROPERTY_ACCESS)
				{
					if (t.Children[1].Type != ReoScriptLexer.IDENTIFIER)
					{
						throw ctx.CreateRuntimeError(t, "delete keyword requires an identifier to delete property from object.");
					}

					object owner = ScriptRunningMachine.ParseNode(t.Children[0] as CommonTree, ctx);
					if (owner is IAccess) owner = ((IAccess)owner).Get();

					if (owner == null)
					{
						string msg = "Attmpt to delete property from null or undefined object";

						if (t.Children[0].Type == ReoScriptLexer.IDENTIFIER)
						{
							msg += ": " + t.Text;
						}
						else
						{
							msg += ".";
						}

						throw ctx.CreateRuntimeError(t, msg);
					}

					ObjectValue ownerObject = (ObjectValue)owner;

					if (ownerObject != null)
					{
						ownerObject.RemoveOwnProperty(t.Children[1].Text);
						return true;
					}
				}

				return false;
			}

			#endregion
		}

		#endregion
		#region Typeof
		class TypeofNodeParser : INodeParser
		{
			public static string Typeof(ScriptRunningMachine srm, object obj)
			{
				if (obj == null)
				{
					return "null";
				}
				else if (obj is bool)
				{
					return srm.BuiltinConstructors.BooleanFunction.FunName.ToLower();
				}
				else if (ScriptRunningMachine.IsPrimitiveNumber(obj)
					|| obj == NaNValue.Value || obj == InfinityValue.Value || obj == MinusInfinityValue.Value)
				{
					return srm.BuiltinConstructors.NumberFunction.FunName.ToLower();
				}
				else if (obj is string)
				{
					return srm.BuiltinConstructors.StringFunction.FunName.ToLower();
				}
				else if (obj is AbstractFunctionObject)
				{
					return srm.BuiltinConstructors.FunctionFunction.FunName.ToLower();
				}
				else if (obj is ObjectValue)
				{
					return srm.BuiltinConstructors.ObjectFunction.FunName.ToLower();
				}
				else if ((srm.WorkMode & MachineWorkMode.AllowDirectAccess) == MachineWorkMode.AllowDirectAccess)
				{
					return "native object";
				}
				else
				{
					return "unknown";
				}
			}

			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object obj = ScriptRunningMachine.ParseNode(t.Children[0] as CommonTree, ctx);
				if (obj is IAccess) obj = ((IAccess)obj).Get();
				return Typeof(srm, obj);
			}

			#endregion
		}
		#endregion
		#region InstanceOf
		class InstanceOfNodeParser : INodeParser
		{
			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				object obj = ScriptRunningMachine.ParseNode(t.Children[0] as CommonTree, ctx);
				if (obj is IAccess) obj = ((IAccess)obj).Get();

				object constructor = ScriptRunningMachine.ParseNode(t.Children[1] as CommonTree, ctx);
				if (constructor is IAccess) constructor = ((IAccess)constructor).Get();

				if (!(constructor is AbstractFunctionObject)) return false;

				if (obj is string)
				{
					return constructor == srm.BuiltinConstructors.StringFunction;
				}
				else if (ScriptRunningMachine.IsPrimitiveNumber(obj))
				{
					return constructor == srm.BuiltinConstructors.NumberFunction;
				}
				else if (obj is bool)
				{
					return constructor == srm.BuiltinConstructors.BooleanFunction;
				}
				else if (srm.AllowDirectAccess && !(obj is ISyntaxTreeReturn)
					&& constructor is TypedNativeFunctionObject)
				{
					return obj.GetType().IsAssignableFrom(((TypedNativeFunctionObject)constructor).Type);
				}

				ObjectValue objValue = ((ObjectValue)obj);

				while (objValue != null)
				{
					bool instanceof = objValue.Constructor == constructor;
					if (instanceof) return true;

					objValue = objValue[ScriptRunningMachine.KEY___PROTO__] as ObjectValue;
				}

				return false;
			}
		}

		#endregion
		#region Tag
		class TagNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				string tagName = (((CommonTree)t.Children[0]).Children[0].ToString());

				tagName = ((srm.WorkMode & MachineWorkMode.AutoUppercaseWhenCLRCalling)
					== MachineWorkMode.AutoUppercaseWhenCLRCalling)
					? ScriptRunningMachine.GetNativeIdentifier(tagName) : tagName;

				AbstractFunctionObject classConstructor = srm.GetClass(tagName) as AbstractFunctionObject;
				if (classConstructor == null) throw ctx.CreateRuntimeError(t, "Class not found: " + tagName);

				ObjectValue obj = srm.CreateNewObject(ctx, classConstructor) as ObjectValue;

				if (obj != null)
				{
					if (classConstructor is TemplateConstructorObject)
					{
						// start of constructing tag from template

						TemplateConstructorObject templateConstructor = classConstructor as TemplateConstructorObject;

						Dictionary<string, object> mergedPropertes = new Dictionary<string, object>();

						// instance property setter
						CommonTree instAttrTree = t.Children[1] as CommonTree;

						CallScope scope = null;

						if (instAttrTree.ChildCount > 0)
						{
							scope = new CallScope(obj, classConstructor)
							{
								CharIndex = t.CharPositionInLine,
								Line = t.Line,
							};

							for (int i = 0; i < instAttrTree.ChildCount; i++)
							{
								CommonTree attr = instAttrTree.Children[i] as CommonTree;

								object val = ScriptRunningMachine.ParseNode(attr.Children[1] as CommonTree, ctx);
								if (val is IAccess) val = ((IAccess)val).Get();

								string propertyName = attr.Children[0].ToString();
								mergedPropertes[propertyName] = val;

								if (templateConstructor.Args.Contains(propertyName))
								{
									scope[propertyName] = val;
								}
							}
						}

						// template default setter
						CommonTree templateTagAttrTree = templateConstructor.TemplateTag.Children[1] as CommonTree;
						for (int i = 0; i < templateTagAttrTree.ChildCount; i++)
						{
							CommonTree attr = templateTagAttrTree.Children[i] as CommonTree;

							object val = ScriptRunningMachine.ParseNode(attr.Children[1] as CommonTree, ctx);
							if (val is IAccess) val = ((IAccess)val).Get();

							string propertyName = attr.Children[0].ToString();

							// properties that not need to set if value be setted from user-side
							if (!mergedPropertes.ContainsKey(propertyName))
							{
								mergedPropertes[propertyName] = val;
							}
						}

						// copy values into object
						foreach (string propertyName in mergedPropertes.Keys)
						{
							PropertyAccessHelper.SetProperty(ctx, obj, propertyName, mergedPropertes[propertyName]);
						}

						if (scope == null && templateConstructor.TemplateTag.ChildCount > 2)
						{
							scope = new CallScope(obj, classConstructor)
							{
								CharIndex = t.CharPositionInLine,
								Line = t.Line,
							};
						}

						if (scope != null)
						{
							ctx.PushCallStack(scope, true);

							try
							{
								for (int i = 2; i < templateConstructor.TemplateTag.ChildCount; i++)
								{
									CommonTree tagStmt = templateConstructor.TemplateTag.Children[i] as CommonTree;

									if (tagStmt.Type == ReoScriptLexer.TAG)
									{
										srm.InvokeFunctionIfExisted(obj, "appendChild", ScriptRunningMachine.ParseNode(tagStmt, ctx));
									}
									else
									{
										ScriptRunningMachine.ParseNode(tagStmt, ctx);
									}
								}
							}
							finally
							{
								ctx.PopCallStack();
							}
						}

						// end of constructing tag from template
					}
					else
					{
						// start of constructing tag from class
						CommonTree attrTree = t.Children[1] as CommonTree;
						for (int i = 0; i < attrTree.ChildCount; i++)
						{
							CommonTree attr = attrTree.Children[i] as CommonTree;

							object val = ScriptRunningMachine.ParseNode(attr.Children[1] as CommonTree, ctx);
							if (val is IAccess) val = ((IAccess)val).Get();

							PropertyAccessHelper.SetProperty(ctx, obj, attr.Children[0].ToString(), val);
						}
						// end of constructing tag from class
					}

					if (t.ChildCount > 2)
					{
						CallScope scope = new CallScope(obj, classConstructor)
						{
							CharIndex = t.CharPositionInLine,
							Line = t.Line,
						};
						ctx.PushCallStack(scope, true);

						try
						{
							for (int i = 2; i < t.ChildCount; i++)
							{
								CommonTree tagStmt = t.Children[i] as CommonTree;

								if (tagStmt.Type == ReoScriptLexer.TAG)
								{
									srm.InvokeFunctionIfExisted(obj, "appendChild", ScriptRunningMachine.ParseNode(tagStmt, ctx));
								}
								else
								{
									ScriptRunningMachine.ParseNode(tagStmt, ctx);
								}
							}
						}
						finally
						{
							ctx.PopCallStack();
						}
					}
				}

				return obj;
			}

			static void RunInTag(CommonTree tagTree, ScriptContext context)
			{

			}
			#endregion
		}

		class TemplateDefineNodeParser : INodeParser
		{
			#region INodeParser Members

			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				string typeName = t.Children[0].Text;

				CommonTree paramsTree = t.Children[1] as CommonTree;
				CommonTree rootTag = t.Children[2] as CommonTree;

				string tagName = (rootTag.Children[0] as CommonTree).Children[0].Text;

				TypedNativeFunctionObject typedConstructor = srm.GetClass(typeName) as TypedNativeFunctionObject;

				if (typedConstructor == null)
					throw ctx.CreateRuntimeError(t, "Class not found: " + typeName);

				TemplateConstructorObject templateDefine = new TemplateConstructorObject(tagName)
				{
					TemplateTag = rootTag,
					TypedConstructor = typedConstructor,
				};

				CommonTree rootAttrTree = rootTag.Children[1] as CommonTree;

				templateDefine[ScriptRunningMachine.KEY_PROTOTYPE] = templateDefine.CreatePrototype(
					new ScriptContext(srm, ScriptRunningMachine.entryFunction));

				string[] identifiers = new string[paramsTree.ChildCount];

				for (int i = 0; i < identifiers.Length; i++)
				{
					identifiers[i] = paramsTree.Children[i].ToString();
				}

				templateDefine.Args = identifiers;

				srm.RegisterClass(templateDefine, tagName);
				return null;
			}
			#endregion
		}
		class TemplateConstructorObject : TypedNativeFunctionObject
		{
			public TemplateConstructorObject(string name) : base(name) { }

			public CommonTree TemplateTag { get; set; }

			public string[] Args { get; set; }

			public TypedNativeFunctionObject TypedConstructor { get; set; }

			public override object CreateObject(ScriptContext context, object[] args)
			{
				return TypedConstructor.CreateObject(context, args);
			}

			public override object CreatePrototype(ScriptContext context)
			{
				return TypedConstructor.CreatePrototype(context);
			}

			public override object Invoke(ScriptContext context, object owner, object[] args)
			{
				return TypedConstructor.Invoke(context, owner, args);
			}
		}
		#endregion
		#region RangeGenerator
#if EXTERNAL_GETTER_SETTER
		class RangeLiteralParser : INodeParser
		{
			public object Parse(CommonTree t, ScriptRunningMachine srm, ScriptContext ctx)
			{
				string fromText = ((CommonTree)t.Children[0]).Text;
				string toText = ((CommonTree)t.Children[0]).Text;

				//object from = ScriptRunningMachine.ParseNode(t.Children[0] as CommonTree, ctx);
				//object to = ScriptRunningMachine.ParseNode(t.Children[1] as CommonTree, ctx);

				//if (from is IAccess) from = ((IAccess)from).Get();
				//if (to is IAccess) to = ((IAccess)to).Get();

				if (ctx.ExternalRangeGenerator != null)
				{
					ctx.ExternalRangeGenerator(fromText, toText);
				}

				return null;
			}
		}
#endif
		#endregion
	}
	#endregion

	#region Parser Adapter
	#region Define Interface
	interface IParserAdapter
	{
		INodeParser MatchParser(CommonTree t);
	}
	#endregion

	#region AWDLDefaultParserAdapter
	class AWDLLogicSyntaxParserAdapter : IParserAdapter
	{
		internal static readonly INodeParser[] definedParser = new INodeParser[ReoScriptLexer.MAX_TOKENS];

		static AWDLLogicSyntaxParserAdapter()
		{
			#region Generic Parsers
			definedParser[ReoScriptLexer.IMPORT] = new ImportNodeParser();
			definedParser[ReoScriptLexer.LOCAL_DECLARE_ASSIGNMENT] = new DeclarationNodeParser();
			definedParser[ReoScriptLexer.ASSIGNMENT] = new AssignmentNodeParser();
			definedParser[ReoScriptLexer.IF_STATEMENT] = new IfStatementNodeParser();
			definedParser[ReoScriptLexer.FOR_STATEMENT] = new ForStatementNodeParser();
			//definedParser[ReoScriptLexer.FOR_STATEMENT] = new JITForStatementNodeParser();
			definedParser[ReoScriptLexer.FOREACH_STATEMENT] = new ForEachStatementNodeParser();
			definedParser[ReoScriptLexer.SWITCH] = new SwitchCaseStatementNodeParser();
			definedParser[ReoScriptLexer.FUNCTION_CALL] = new FunctionCallNodeParser();
			//definedParser[ReoScriptLexer.FUNCTION_DEFINE] = new FunctionDefineNodeParser();
			definedParser[ReoScriptLexer.ANONYMOUS_FUNCTION] = new AnonymousFunctionNodeParser();
			//definedParser[ReoScriptLexer.BREAK] = new BreakNodeParser();
			//definedParser[ReoScriptLexer.CONTINUE] = new ContinueNodeParser();
			definedParser[ReoScriptLexer.RETURN] = new ReturnNodeParser();
			definedParser[ReoScriptLexer.CREATE] = new CreateObjectNodeParser();
			definedParser[ReoScriptLexer.TRY_CATCH] = new TryCatchNodeParser();
			definedParser[ReoScriptLexer.TRY_CATCH_TRHOW] = new ThrowNodeParser();
			definedParser[ReoScriptLexer.ARRAY_ACCESS] = new IndexAccessNodeParser();
			definedParser[ReoScriptLexer.PROPERTY_ACCESS] = new PropertyAccessNodeParser();
			definedParser[ReoScriptLexer.DELETE_PROP] = new DeletePropertyNodeParser();
			definedParser[ReoScriptLexer.TYPEOF] = new TypeofNodeParser();
			definedParser[ReoScriptLexer.INSTANCEOF] = new InstanceOfNodeParser();

			#endregion

			#region Operators
			definedParser[ReoScriptLexer.PLUS] = new ExprPlusNodeParser();
			definedParser[ReoScriptLexer.MINUS] = new ExprMinusNodeParser();
			definedParser[ReoScriptLexer.MUL] = new ExprMultiNodeParser();
			definedParser[ReoScriptLexer.DIV] = new ExprDivNodeParser();
			definedParser[ReoScriptLexer.MOD] = new ExprModNodeParser();
			definedParser[ReoScriptLexer.AND] = new ExprAndNodeParser();
			definedParser[ReoScriptLexer.OR] = new ExprOrNodeParser();
			definedParser[ReoScriptLexer.XOR] = new ExprXorNodeParser();
			definedParser[ReoScriptLexer.LSHIFT] = new ExprLeftShiftNodeParser();
			definedParser[ReoScriptLexer.RSHIFT] = new ExprRightShiftNodeParser();
			#endregion

			#region Unary Operators
			definedParser[ReoScriptLexer.PRE_UNARY] = new ExprUnaryNodeParser();
			definedParser[ReoScriptLexer.PRE_UNARY_STEP] = new ExprPreIncrementNodeParser();
			definedParser[ReoScriptLexer.POST_UNARY_STEP] = new ExprPostIncrementNodeParser();
			#endregion

			#region Relation Operators
			definedParser[ReoScriptLexer.CONDITION] = new ExprConditionNodeParser();
			definedParser[ReoScriptLexer.EQUALS] = new ExprEqualsNodeParser();
			definedParser[ReoScriptLexer.NOT_EQUALS] = new ExprNotEqualsNodeParser();
			definedParser[ReoScriptLexer.STRICT_EQUALS] = new ExprStrictEqualsNodeParser();
			definedParser[ReoScriptLexer.STRICT_NOT_EQUALS] = new ExprStrictNotEqualsNodeParser();
			definedParser[ReoScriptLexer.GREAT_THAN] = new ExprGreaterThanNodeParser();
			definedParser[ReoScriptLexer.GREAT_EQUALS] = new ExprGreaterOrEqualsNodeParser();
			definedParser[ReoScriptLexer.LESS_THAN] = new ExprLessThanNodeParser();
			definedParser[ReoScriptLexer.LESS_EQUALS] = new ExprLessOrEqualsNodeParser();
			#endregion

			#region Boolean Operations
			definedParser[ReoScriptLexer.LOGICAL_AND] = new BooleanAndNodeParser();
			definedParser[ReoScriptLexer.LOGICAL_OR] = new BooleanOrNodeParser();
			#endregion

			#region External Statements
			definedParser[ReoScriptLexer.TAG] = new TagNodeParser();
			definedParser[ReoScriptLexer.TEMPLATE_DEFINE] = new TemplateDefineNodeParser();
			//definedParser[ReoScriptLexer.RANGE_EXP] = new RangeLiteralParser();
			#endregion
		}

		static object Parse(CommonTree t, ScriptContext ctx)
		{
			INodeParser parser = definedParser[t.Type];
			return (parser != null) ? parser.Parse(t, ctx.Srm, ctx) : null;
		}


		#region IParserAdapter Members

		public virtual INodeParser MatchParser(CommonTree t)
		{
			return definedParser[t.Type];
		}

		#endregion
	}
	#endregion

	#endregion

	#region ScriptContext
	/// <summary>
	/// A script context used in multi-thread executing
	/// </summary>
	public sealed class ScriptContext
	{
		internal static readonly int MAX_STACK = 50;

		#region Context Constructor

		/// <summary>
		/// Path of file loaded by Load method of SRM
		/// </summary>
		internal ScriptContext(ScriptRunningMachine srm, AbstractFunctionObject function) :
			this(srm, function, null)
		{ }

		internal ScriptContext(ScriptRunningMachine srm, AbstractFunctionObject function, string filePath)
		{
#if DEBUG
			Debug.Assert(srm != null);
			Debug.Assert(srm.GlobalObject != null);
			Debug.Assert(function != null);
#endif

			this.SourceFilePath = filePath;
			this.GlobalObject = srm.GlobalObject;
			this.Srm = srm;

#if EXTERNAL_GETTER_SETTER
			PropertyGetter=new Dictionary<Func<string,bool>,Func<string,object>>();
			PropertySetter=new Dictionary<Func<string,bool>,Func<string,object>>();
#endif

			FunctionStack = new Stack<CallScope>();
			callStack.Push(new CallScope(this.GlobalObject, ScriptRunningMachine.entryFunction));

		}

		public string SourceFilePath { get; set; }

		/// <summary>
		/// Current context this object
		/// </summary>
		public object ThisObject
		{
			get
			{
				return CurrentCallScope == null ? GlobalObject : CurrentCallScope.ThisObject;
			}
			set
			{
				if (CurrentCallScope != null) CurrentCallScope.ThisObject = value;
			}
		}

		#endregion

		#region Variable & Property

		/// <summary>
		/// ScriptRunningMachine instance
		/// </summary>
		public ScriptRunningMachine Srm { get; set; }

		/// <summary>
		/// Global object (Root object in script context)
		/// </summary>
		internal ObjectValue GlobalObject { get; set; }

		/// <summary>
		/// Not supported
		/// </summary>
		internal ObjectValue WithObject { get; set; }

		/// <summary>
		/// Get or set variable in current call-stack.
		/// </summary>
		/// <param name="identifier">name of variable</param>
		/// <returns>value of variable</returns>
		public object this[string identifier]
		{
			get
			{
#if EXTERNAL_GETTER_SETTER
				foreach (var getter in PropertyGetter)
				{
					if (getter.Key(identifier))
					{
						return getter.Value(identifier);
					}
				}
#endif

				IVariableContainer container = null;

				CallScope cs = CurrentCallScope;

				if (cs != null)
				{
					if (cs.Variables.ContainsKey(identifier))
					{
						container = cs;
					}
					else
					{
						CallScope outerScope = cs.CurrentFunction.OuterCallScope;
						while (outerScope != null)
						{
							if (outerScope.Variables.ContainsKey(identifier))
							{
								container = outerScope;
								break;
							}

							outerScope = outerScope.CurrentFunction.OuterCallScope;
						}
					}
				}

				if (container == null)
				{
					container = GlobalObject;
				}

				object o = null;
				container.TryGetValue(identifier, out o);

				//if (CurrentCallScope == null)
				//  return GlobalObject[identifier];

				//object obj = CurrentCallScope[identifier];

				//if (obj != null)
				//  return obj;
				//else
				//  return GlobalObject[identifier];
				return o;
			}
			set
			{
#if EXTERNAL_GETTER_SETTER
				foreach (var setter in PropertySetter)
				{
					if (setter.Key(identifier)) setter.Value(identifier);
				}
#endif

				if (CurrentCallScope != null)
					CurrentCallScope[identifier] = value;
				else
					GlobalObject[identifier] = value;
			}
		}

		/// <summary>
		/// Set variable by specified name into current call-stack. 
		/// If does not exist, set variable into global object.
		/// </summary>
		/// <param name="identifier">name of variable</param>
		/// <param name="value">value of variable</param>
		/// <returns>value to be set</returns>
		public object SetVariable(string identifier, object value)
		{
			if (CurrentCallScope != null)
				CurrentCallScope[identifier] = value;
			else
				GlobalObject[identifier] = value;

			return value;
		}

		/// <summary>
		/// Get variable by specified name from current call-stack.
		/// If does not exist, get variable from global object.
		/// </summary>
		/// <param name="identifier">name of variable</param>
		/// <returns>value of specified variable</returns>
		public object GetVariable(string identifier)
		{
			if (CurrentCallScope == null)
				return GlobalObject[identifier];

			object obj = CurrentCallScope[identifier];

			if (obj != null)
				return obj;
			else
				return GlobalObject[identifier];
		}

		/// <summary>
		/// Remove specified variable from current call-stack.
		/// If variable cannot be found in current call-stack, remove variable from global object.
		/// </summary>
		/// <param name="errorObjIdentifier">name of variable</param>
		public void RemoveVariable(string identifier)
		{
			if (CurrentCallScope == null)
				GlobalObject.RemoveOwnProperty(identifier);
			else
				CurrentCallScope.Variables.Remove(identifier);
		}

		/// <summary>
		/// Evaluate a property getting operation.
		/// </summary>
		/// <param name="obj">get specified property from this object</param>
		/// <param name="identifier">name of property</param>
		/// <returns>value of property</returns>
		public object EvaluatePropertyGet(ObjectValue obj, string identifier)
		{
			return PropertyAccessHelper.GetProperty(this, obj, identifier);
		}

		/// <summary>
		/// Evaluate a property setting operation.
		/// </summary>
		/// <param name="obj">set specified property into this object</param>
		/// <param name="identifier">name of property</param>
		/// <param name="value">value of property</param>
		public void EvaluatePropertySet(ObjectValue obj, string identifier, object value)
		{
			PropertyAccessHelper.SetProperty(this, obj, identifier, value);
		}

#if EXTERNAL_GETTER_SETTER
		public Dictionary<Func<string, bool>, Func<string, object>> PropertyGetter { get; set; }

		public Dictionary<Func<string, bool>, Func<string, object>> PropertySetter { get; set; }

		public Func<string, string, object> ExternalRangeGenerator { get; set; }
#endif

		#endregion

		#region CallStack
		private readonly Stack<CallScope> callStack = new Stack<CallScope>();

		/// <summary>
		/// Call-stack for function call.
		/// </summary>
		internal Stack<CallScope> CallStack { get { return callStack; } }

		internal Stack<CallScope> FunctionStack { get; set; }

		/// <summary>
		/// Current call-stack of function call.
		/// </summary>
		internal CallScope CurrentCallScope { get; set; }

		/// <summary>
		/// Push a call-scope into call-stack.
		/// </summary>
		/// <param name="scope">call-scope will be pushed into call-stack</param>
		internal void PushCallStack(CallScope scope, bool innerCall)
		{
			if (callStack.Count >= MAX_STACK)
			{
				throw new CallStackOverflowException("Call stack overflow.");
			}

#if DEBUG
			Debug.Assert(scope != null);
			Debug.Assert(scope.CurrentFunction != null);

			// allow null 'this' reference
			//Debug.Assert(scope.ThisObject != null);
#endif

			callStack.Push(scope);

			CurrentCallScope = scope;
		}

		internal void PopCallStack()
		{
			if (callStack.Count > 0) callStack.Pop();
			CurrentCallScope = callStack.Count > 1 ? callStack.Peek() : null;
		}
		#endregion

		#region Create Object
		/// <summary>
		/// Create an object instance.
		/// </summary>
		/// <returns></returns>
		public ObjectValue CreateNewObject()
		{
			return Srm.CreateNewObject(this);
		}

		/// <summary>
		/// Create an object instance and add initial properties.
		/// </summary>
		/// <param name="properties">properties will be added into created instance</param>
		/// <returns>created object instance</returns>
		public ObjectValue CreateNewObject(Dictionary<string, object> properties)
		{
			return CreateNewObject((obj) => obj.AddProperties(properties));
		}

		/// <summary>
		/// Create an object instance and perform customized initialization.
		/// </summary>
		/// <param name="initializer">given initializer will be invoked after object is created</param>
		/// <returns>created object instance</returns>
		public ObjectValue CreateNewObject(Action<ObjectValue> initializer)
		{
			ObjectValue obj = CreateNewObject();
			initializer?.Invoke(obj);
			return obj;
		}

		/// <summary>
		/// Create an object using specified function constructor
		/// </summary>
		/// <param name="funObject">function constructor</param>
		/// <param name="invokeConstructor">specifies whether allow to call constructor</param>
		/// <param name="args">arguments for calling constructor</param>
		/// <returns>created object</returns>
		public object CreateNewObject(AbstractFunctionObject funObject, bool invokeConstructor = true, object[] args = null)
		{
			return Srm.CreateNewObject(this, funObject, invokeConstructor, args);
		}

		/// <summary>
		/// Create an empty array object
		/// </summary>
		/// <returns></returns>
		public ArrayObject CreateNewArray()
		{
			return CreateNewArray(null);
		}

		/// <summary>
		/// Create an array object with initial elements
		/// </summary>
		/// <param name="initElements"></param>
		/// <returns></returns>
		public ArrayObject CreateNewArray(params object[] initElements)
		{
			ArrayObject arr = Srm.CreateNewObject(this, Srm.BuiltinConstructors.ArrayFunction) as ArrayObject;
			if (arr != null && initElements != null)
			{
				arr.List.AddRange(initElements);
			}
			return arr;
		}

		internal ErrorObject CreateErrorObject(CommonTree t, string msg)
		{
			ErrorObject err = CreateNewObject(Srm.BuiltinConstructors.ErrorFunction) as ErrorObject;
			err.Line = t.Line;
			err.CharIndex = t.CharPositionInLine;
			err.Message = msg;
			err.CallStack = new List<CallScopeObject>();

			CallScope sc = callStack.Peek();
			sc.Line = t.Line;
			sc.CharIndex = t.CharPositionInLine;

			foreach (CallScope scope in callStack)
			{
				err.CallStack.Add(new CallScopeObject(scope));
			}

			//err.CallStack.Add(new CallScopeObject(new CallScope(GlobalObject, ScriptRunningMachine.entryFunction)
			//{
			//  CharIndex = t.CharPositionInLine,
			//  Line = t.Line
			//}));

			return err;
		}

		internal ReoScriptRuntimeException CreateRuntimeError(CommonTree t, string msg)
		{
			return new ReoScriptRuntimeException(CreateErrorObject(t, msg));
		}

		#endregion

	}

	interface IVariableContainer
	{
		object this[string identifier] { get; set; }
		bool TryGetValue(string identifier, out object value);
	}

	internal class CallScope : IVariableContainer
	{
		public object ThisObject { get; set; }
		public AbstractFunctionObject CurrentFunction { get; set; }

		public CallScope(object thisObject, AbstractFunctionObject funObject)
		{
			this.ThisObject = thisObject;
			this.CurrentFunction = funObject;
		}

		private Dictionary<string, object> variables = new Dictionary<string, object>();

		public Dictionary<string, object> Variables
		{
			get { return variables; }
		}

		public object this[string identifier]
		{
			get
			{
				object o;
				return (variables.TryGetValue(identifier, out o)) ? o : null;
			}
			set
			{
				variables[identifier] = value;
			}
		}

		public bool TryGetValue(string identifier, out object value)
		{
			return variables.TryGetValue(identifier, out value);
		}

		public bool IsInnerCall { get; set; }

		public int CharIndex { get; set; }
		public int Line { get; set; }

		public override string ToString()
		{
			return string.Format("at {0} ({1}:{2})", GetFunctionName(CurrentFunction), Line, CharIndex);
		}

		private static string GetFunctionName(AbstractFunctionObject fun)
		{
			if (fun is FunctionObject)
			{
				FunctionObject funObj = ((FunctionObject)fun);

				if (funObj.FunctionInfo != null && funObj.FunctionInfo.IsAnonymous)
				{
					return "<anonymous>";
				}
			}

			return fun.FunName;
		}

	}


	#endregion

	#region ScriptRunningMachine
	/// <summary>
	/// A virtual machine to execute ReoScript language. 
	/// </summary>
	public sealed class ScriptRunningMachine
	{
		#region Const & Keywords

		/// <summary>
		/// Keyword of undefined script object
		/// </summary>
		internal static readonly string KEY_UNDEFINED = "undefined";

		/// <summary>
		/// keyword of prototype of constructor function
		/// </summary>
		internal static readonly string KEY_PROTOTYPE = "prototype";

		internal static readonly string KEY___PROTO__ = "__proto__";

		internal static readonly string KEY_CONSTRUCTOR = "constructor";

		internal static readonly string KEY___ARGS__ = "__args__";

		/// <summary>
		/// Global variable name
		/// </summary>
		internal static readonly string GLOBAL_VARIABLE_NAME = "script";

		#endregion

		#region Constructor

		/// <summary>
		/// Specifies what features can be supported by SRM.
		/// After modify this value call Reset method to apply the changes.
		/// </summary>
		public CoreFeatures CoreFeatures { get; set; }

		/// <summary>
		/// Construct SRM with Standard feature support.
		/// </summary>
		public ScriptRunningMachine()
			: this(CoreFeatures.StandardFeatures)
		{
			//this();
		}

		/// <summary>
		/// Construct SRM with specified feature support.
		/// </summary>
		/// <param name="coreFeatures">Specifies what features can be supported by SRM.</param>
		public ScriptRunningMachine(CoreFeatures coreFeatures)
		{
			this.WorkPath = Environment.CurrentDirectory;
			this.CoreFeatures = coreFeatures;

			Reset();
		}

		~ScriptRunningMachine()
		{
			DetachAllEvents();

			try
			{
				if (asyncCallThread != null) asyncCallThread.Abort();
			}
			catch { }
		}

		#endregion

		#region Context
		internal BuiltinConstructors BuiltinConstructors = new BuiltinConstructors();

		//internal ScriptContext CurrentContext { get; set; }

		//internal object RetrieveVariable(string identifier)
		//{
		//  object obj = CurrentContext[identifier];
		//  return obj == null ? this[identifier] : obj;
		//}

		/// <summary>
		/// Reset current context to clear all variables and restart running machine.
		/// </summary>
		public void Reset()
		{
			// force stop current executing
			ForceStop();

			// wait for current executing exiting
			while (IsRunning)
				Thread.Sleep(100);

			// detach all attched CLR events
			DetachAllEvents();

			// reset imported namespace and types
			ImportedNamespace.Clear();
			ImportedTypes.Clear();

			// reset machine status
			isForceStop = false;

			// renew global object
			GlobalObject = new WorldObject();

			// initialize built-in objects
			BuiltinConstructors = new BuiltinConstructors();
			BuiltinConstructors.ApplyToScriptRunningMachine(this);

			// load core library
			LoadCoreLibraries();

			// initialize default context
			defaultContext = new ScriptContext(this, entryFunction);

			if (Resetted != null) Resetted(this, null);
		}

		internal void LoadCoreLibraries()
		{
			MemoryStream ms;

			using (ms = new MemoryStream(Resources.lib_core))
			{
				Load(ms);
			}

			using (ms = new MemoryStream(Resources.lib_array))
			{
				Load(ms);
			}

			if ((this.CoreFeatures & CoreFeatures.ArrayExtension) == CoreFeatures.ArrayExtension)
			{
				using (ms = new MemoryStream(Resources.lib_array_ext))
				{
					Load(ms);
				}
			}
		}

		private ScriptContext defaultContext;

		/// <summary>
		/// Default script context
		/// </summary>
		public ScriptContext DefaultContext { get { return defaultContext; } }

		/// <summary>
		/// Create new script context
		/// </summary>
		/// <returns>created script context</returns>
		public ScriptContext CreateContext()
		{
			return new ScriptContext(this, entryFunction);
		}
		#endregion

		#region Global Variable
		public ObjectValue GlobalObject { get; set; }

		/// <summary>
		/// Set value as a property to the global object. Value name specified by
		/// identifier. After this, the value can be used in script like a normal 
		/// variable.
		/// </summary>
		/// <param name="identifier">name to variable</param>
		/// <param name="obj">value of variable</param>
		public void SetGlobalVariable(string identifier, object obj)
		{
			// if object is function, prepare its prototype 
			if (obj is AbstractFunctionObject)
			{
				AbstractFunctionObject functionObj = (AbstractFunctionObject)obj;

				if (functionObj[KEY_PROTOTYPE] == null)
				{
					functionObj[KEY_PROTOTYPE] = functionObj.CreatePrototype(
						new ScriptContext(this, entryFunction));
				}
			}

			GlobalObject[identifier] = obj;
		}

		/// <summary>
		/// Get a global variable from global object by specified name.
		/// </summary>
		/// <param name="identifier">variable name</param>
		/// <returns>value of global variable</returns>
		public object GetGlobalVariable(string identifier)
		{
			return GlobalObject[identifier];
		}

		/// <summary>
		/// Get a global variable of specified type from global object. If the type convertion is failed, 
		/// a null value will be returned.
		/// </summary>
		/// <typeparam name="T">type to be converted</typeparam>
		/// <param name="identifier">variable name in globla object</param>
		/// <returns>object retrieved from global object</returns>
		public T GetGlobalVariable<T>(string identifier)
		{
			return GetGlobalVariable<T>(identifier, null);
		}

		/// <summary>
		/// Get a global variable of specified type from global object. If the type convertion is failed, 
		/// the defaultValue will be returned.
		/// </summary>
		/// <typeparam name="T">type to be converted</typeparam>
		/// <param name="identifier">variable name in globla object</param>
		/// <param name="defaultValue">if convertion is failed, this value will be returned</param>
		/// <returns>object retrieved from global object</returns>
		public T GetGlobalVariable<T>(string identifier, object defaultValue)
		{
			try
			{
				return (T)Convert.ChangeType(GetGlobalVariable(identifier), typeof(T));
			}
			catch
			{
				return (T)defaultValue;
			}
		}

		/// <summary>
		/// Delete a specified global variable.
		/// </summary>
		/// <param name="identifier">variable name</param>
		/// <returns>true if specified variable does exist and deleting is successed</returns>
		public bool RemoveGlobalVariable(string identifier)
		{
			return GlobalObject.RemoveOwnProperty(identifier);
		}

		/// <summary>
		/// Set or get global variables
		/// </summary>
		/// <param name="identifier">identifier to be used as variable name</param>
		/// <returns>object in global object</returns>
		public object this[string identifier]
		{
			get
			{
				return GetGlobalVariable(identifier);
			}
			set
			{
				SetGlobalVariable(identifier, value);
			}
		}

		/// <summary>
		/// Dummy function object for most outside code scope.
		/// </summary>
		internal static readonly FunctionObject entryFunction = new FunctionObject()
		{
			FunName = "__entry__",
		};

		internal bool IsInGlobalScope(ScriptContext context)
		{
			return context.CurrentCallScope == null;
		}

		#endregion

		#region Object Management

		/// <summary>
		/// Create a new object instance 
		/// </summary>
		/// <returns>object is created</returns>
		public ObjectValue CreateNewObject()
		{
			return CreateNewObject(new ScriptContext(this, entryFunction), BuiltinConstructors.ObjectFunction) as ObjectValue;
		}

		//public ObjectValue CreateNewObject(string name)
		//{
		//  ObjectValue obj = CreateNewObject();
		//  //obj.Name = name;
		//  return obj;
		//}

		public ObjectValue CreateNewObject(Action<ObjectValue> initializer)
		{
			ObjectValue obj = CreateNewObject();
			initializer(obj);
			return obj;
		}

		public ObjectValue CreateNewObject(Dictionary<string, object> properties)
		{
			return CreateNewObject(obj => obj.AddProperties(properties));
		}

		internal ObjectValue CreateNewObject(ScriptContext context)
		{
			return CreateNewObject(context, BuiltinConstructors.ObjectFunction) as ObjectValue;
		}

		internal object CreateNewObject(ScriptContext context, AbstractFunctionObject constructor, bool invokeConstructor = true, object[] constructArguments = null)
		{
			object obj = null;

			if (constructor is NativeFunctionObject)
			{
				obj = ((NativeFunctionObject)constructor).CreateObject(context, constructArguments);
			}

			if (obj == null) obj = new ObjectValue();

			if (obj is ObjectValue)
			{
				ObjectValue objValue = obj as ObjectValue;

				//objValue.Name = constructor.FunName == null ? 
				//  constructor.Name : constructor.FunName;

				// point to constructor
				//objValue[ScriptRunningMachine.KEY_CONSTRUCTOR] = constructor;
				objValue.Constructor = constructor;

				// get prototype from constructor
				object prototype = constructor[KEY_PROTOTYPE];

				// create prototype if not existed
				objValue[KEY___PROTO__] = prototype;
			}

			if (invokeConstructor)
			{
				InvokeFunction(context, obj, constructor, constructArguments);
			}

			if (obj != null && NewObjectCreated != null)
			{
				NewObjectCreated(this, new ReoScriptObjectEventArgs(obj, constructor));
			}

			return obj;
		}

		//internal StringValue CreateNewString(string str)
		//{
		//  StringValue strObj = new StringValue(ConvertEscapeLiterals(str)) { Name = "String" };

		//  strObj[ScriptRunningMachine.KEY___PROTO__]
		//    = BuiltinConstructors.StringFunction[ScriptRunningMachine.KEY_PROTOTYPE];

		//  //strObj[ScriptRunningMachine.KEY_CONSTRUCTOR]
		//  //  = BuiltinConstructors.StringFunction;

		//  strObj.Constructor = BuiltinConstructors.StringFunction;

		//  if (NewObjectCreated != null)
		//  {
		//    NewObjectCreated(this, new ReoScriptObjectEventArgs(strObj, BuiltinConstructors.StringFunction));
		//  }

		//  return strObj;
		//}
		#endregion

		#region CLR Type Import
		private List<ScriptRunningMachine.EventHandlerInfo> registeredEventHandlers = new List<ScriptRunningMachine.EventHandlerInfo>();

		internal List<ScriptRunningMachine.EventHandlerInfo> RegisteredEventHandlers
		{
			get { return registeredEventHandlers; }
			set { registeredEventHandlers = value; }
		}

		private List<Type> importedTypes = new List<Type>();

		internal List<Type> ImportedTypes
		{
			get { return importedTypes; }
			set { importedTypes = value; }
		}

		private List<string> importedNamespace = new List<string>();

		internal List<string> ImportedNamespace
		{
			get { return importedNamespace; }
			set { importedNamespace = value; }
		}

		/// <summary>
		/// Import a .NET type into script context. This method will create a constructor function
		/// by type's name and save it as property into global object. Note that if there 
		/// is an object named type's name the object will be overwritten.
		/// </summary>
		/// <param name="type">type to be added into script context</param>
		public void ImportType(Type type)
		{
			ImportType(type, type.Name);
		}

		/// <summary>
		/// Import a .NET type into script context. This method will create a constrcutor function
		/// by given alias name and save it as property into global object. Note that if there
		/// is an object named type's name the object will be overwritten.
		/// </summary>
		/// <param name="type">type to be added into script context</param>
		/// <param name="name">alias name to create constructor function in global object</param>
		public void ImportType(Type type, string name)
		{
			if (ImportedTypes.Contains(type))
			{
				ImportedTypes.Remove(type);
			}

			ImportedTypes.Add(type);

			SetGlobalVariable(name, new TypedNativeFunctionObject(type, name));
		}

		/// <summary>
		/// Import a namespace into script context
		/// </summary>
		/// <param name="name">namespace to be registered into script context</param>
		public void ImportNamespace(string name)
		{
			if (name.EndsWith("*")) name = name.Substring(0, name.Length - 1);
			if (name.EndsWith(".")) name = name.Substring(0, name.Length - 1);

			if (!ImportedNamespace.Contains(name))
			{
				ImportedNamespace.Add(name);
			}
		}

		internal Type GetImportedTypeFromNamespaces(string typeName)
		{
			Type type = null;

			foreach (string ns in ImportedNamespace)
			{
				type = GetTypeFromAssembly(ns, typeName);
				if (type != null) return type;
			}

			return type;
		}

		internal Type GetTypeFromAssembly(string ns, string typeName)
		{
			Type type = null;

			// search assembly which's name starting with specified namespace
			Assembly ass = AppDomain.CurrentDomain.GetAssemblies().FirstOrDefault(q => q.FullName.StartsWith(ns));

			if (ass != null)
			{
				type = ass.GetType(ns + "." + typeName);
				if (type != null)
				{
					ImportType(type);
					return type;
				}
			}

			return type;
		}

		internal void CombineObject(ScriptContext context, object target, ObjectValue source)
		{
			foreach (string key in source)
			{
				// FIXME: get member by PropertyAccessHelper.GetProperty?
				//        enumerated in only own members
				PropertyAccessHelper.SetProperty(context, target, key, source[key]);
			}
		}

		#endregion

		#region CLR Event

		internal void AttachEvent(ScriptContext context, object obj, EventInfo ei, FunctionObject functionValue)
		{
			// remove last attached event to sample object
			DetachEvent(obj, ei);

			EventHandlerInfo ehi = new EventHandlerInfo(this, context, obj, ei, null, functionValue);
			Action<object> doEvent = (e) => InvokeFunction(context, obj, functionValue, new object[] { e });

			Delegate d = null;
			if (ei.EventHandlerType == typeof(EventHandler))
			{
				d = new EventHandler((s, e) => doEvent(e));
			}
			else if (ei.EventHandlerType == typeof(MouseEventHandler))
			{
				d = new MouseEventHandler((s, e) => doEvent(e));
			}
			else if (ei.EventHandlerType == typeof(KeyEventHandler))
			{
				d = new KeyEventHandler((s, e) => doEvent(e));
			}
			else if (ei.EventHandlerType == typeof(PaintEventHandler))
			{
				d = new PaintEventHandler((s, e) => doEvent(e));
			}

			ehi.ActionMethod = d;
			ei.AddEventHandler(obj, d);

			RegisteredEventHandlers.Add(ehi);
			return;

			// Get an EventInfo representing the Click event, and get the 
			// type of delegate that handles the event. 
			//
			EventInfo evClick = ei;
			Type tDelegate = evClick.EventHandlerType;

			// If you already have a method with the correct signature, 
			// you can simply get a MethodInfo for it.  
			//
			//MethodInfo miHandler =
			//    typeof(Example).GetMethod("LuckyHandler",
			//        BindingFlags.NonPublic | BindingFlags.Instance);

			// Create an instance of the delegate. Using the overloads 
			// of CreateDelegate that take MethodInfo is recommended. 
			//
			//Delegate d = Delegate.CreateDelegate(tDelegate, this, miHandler);

			// Get the "add" accessor of the event and invoke it late-
			// bound, passing in the delegate instance. This is equivalent 
			// to using the += operator in C#, or AddHandler in Visual 
			// Basic. The instance on which the "add" accessor is invoked
			// is the form; the arguments must be passed as an array. 
			//
			MethodInfo addHandler = evClick.GetAddMethod();
			//Object[] addHandlerArgs = { d };
			//addHandler.Invoke(exFormAsObj, addandlerArgs);
			//---------------------------------------------

			// Event handler methods can also be generated at run time, 
			// using lightweight dynamic methods and Reflection.Emit.  
			// To construct an event handler, you need the return type 
			// and parameter types of the delegate. These can be obtained 
			// by examining the delegate's Invoke method.  
			// 
			// It is not necessary to name dynamic methods, so the empty  
			// string can be used. The last argument associates the  
			// dynamic method with the current type, giving the delegate 
			// access to all the public and private members of Example, 
			// as if it were an instance method. 
			//
			Type returnType = GetDelegateReturnType(tDelegate);
			if (returnType != typeof(void))
				throw new ApplicationException("Delegate has a return type.");

			DynamicMethod handler =
					new DynamicMethod(string.Empty,
														null,
														GetDelegateParameterTypes(tDelegate),
														typeof(ScriptRunningMachine));

			// Generate a method body. This method loads a string, calls  
			// the Show method overload that takes a string, pops the  
			// return value off the stack (because the handler has no 
			// return type), and returns. 
			//
			ILGenerator ilgen = handler.GetILGenerator();

			//Type[] showParameters = { typeof(object) };
			//MethodInfo simpleShow = typeof(MessageBox).GetMethod("Show", showParameters);

			MethodInfo mi = GetType().GetMethod("DoEventFunction", BindingFlags.NonPublic | BindingFlags.Static);

			//Type[] showParameters = { typeof(String) };
			//MethodInfo simpleShow =
			//    typeof(MessageBox).GetMethod("Show", showParameters);

			ilgen.Emit(OpCodes.Ldarg_1);
			//ilgen.Emit(OpCodes.Ldobj);
			//ilgen.Emit(OpCodes.Ldobj, 
			//ilgen.Emit(OpCodes.Ldstr, "This event handler was constructed at run time.");
			//ilgen.Emit(OpCodes.Call, evtHandler.Method);
			ilgen.Emit(OpCodes.Call, doEvent.Method);
			//ilgen.Emit(OpCodes.Pop);
			ilgen.Emit(OpCodes.Ret);

			// Complete the dynamic method by calling its CreateDelegate 
			// method. Use the "add" accessor to add the delegate to
			// the invocation list for the event. 
			//
			Delegate dEmitted = handler.CreateDelegate(tDelegate);
			addHandler.Invoke(obj, new Object[] { dEmitted });

			//ehi.ActionMethod = Delegate.CreateDelegate(ei.EventHandlerType, ehi, "DoEvent");
			//ei.AddEventHandler(obj, ehi.ActionMethod);

			RegisteredEventHandlers.Add(ehi);
		}

		private static void DoEventFunction(object e)
		{
			MessageBox.Show("ok" + e.ToString());
		}

		private Type[] GetDelegateParameterTypes(Type d)
		{
			if (d.BaseType != typeof(MulticastDelegate))
				throw new ApplicationException("Not a delegate.");

			MethodInfo invoke = d.GetMethod("Invoke");
			if (invoke == null)
				throw new ApplicationException("Not a delegate.");

			ParameterInfo[] parameters = invoke.GetParameters();
			Type[] typeParameters = new Type[parameters.Length];
			for (int i = 0; i < parameters.Length; i++)
			{
				typeParameters[i] = parameters[i].ParameterType;
			}
			return typeParameters;
		}

		private Type GetDelegateReturnType(Type d)
		{
			if (d.BaseType != typeof(MulticastDelegate))
				throw new ApplicationException("Not a delegate.");

			MethodInfo invoke = d.GetMethod("Invoke");
			if (invoke == null)
				throw new ApplicationException("Not a delegate.");

			return invoke.ReturnType;
		}

		internal void DetachEvent(object obj, EventInfo ei)
		{
			var ehi = RegisteredEventHandlers.FirstOrDefault(reh =>
				reh.EventInfo == ei && reh.Instance == obj);

			if (ehi != null)
			{
				ehi.EventInfo.RemoveEventHandler(obj, ehi.ActionMethod);

				RegisteredEventHandlers.Remove(ehi);
			}
		}

		private void DetachAllEvents()
		{
			foreach (EventHandlerInfo handlerInfo in RegisteredEventHandlers)
			{
				handlerInfo.EventInfo.RemoveEventHandler(handlerInfo.Instance, handlerInfo.ActionMethod);
			}

			RegisteredEventHandlers.Clear();
		}

		internal FunctionObject GetAttachedEvent(object obj, EventInfo ei)
		{
			var ehi = RegisteredEventHandlers.FirstOrDefault(reh =>
				reh.EventInfo == ei && reh.Instance == obj);

			return ehi == null ? null : ehi.FunctionValue;
		}

		internal class EventHandlerInfo
		{
			public object Instance { get; set; }
			public EventInfo EventInfo { get; set; }
			public Delegate ActionMethod { get; set; }
			public FunctionObject FunctionValue { get; set; }
			public ScriptRunningMachine Srm { get; set; }
			public ScriptContext Context { get; set; }

			internal EventHandlerInfo(ScriptRunningMachine srm, ScriptContext context, object instance,
				EventInfo eventInfo, Delegate delegateMethod, FunctionObject functionValue)
			{
				this.Srm = srm;
				this.Context = context;
				this.Instance = instance;
				this.EventInfo = eventInfo;
				this.ActionMethod = delegateMethod;
				this.FunctionValue = functionValue;
			}

			public void DoEvent(object sender, object arg)
			{
				Srm.InvokeFunction(Context, Instance, FunctionValue, new object[] { arg });
			}
		}

		#endregion

		#region Work Mode
		private MachineWorkMode workMode = MachineWorkMode.Default;

		/// <summary>
		/// Get or set the working mode of script running machine
		/// </summary>
		public MachineWorkMode WorkMode
		{
			get { return workMode; }
			set
			{
				if (workMode != value)
				{
					workMode = value;
					if (WorkModeChanged != null)
					{
						WorkModeChanged(this, null);
					}
				}
			}
		}

		/// <summary>
		/// Event fired when work mode has been changed.
		/// </summary>
		public event EventHandler WorkModeChanged;

		/// <summary>
		/// Get or set whether allowed to access .NET object, type, namespace, etc. directly. (default is false)
		/// </summary>
		public bool AllowDirectAccess
		{
			get { return (workMode & MachineWorkMode.AllowDirectAccess) == MachineWorkMode.AllowDirectAccess; }
			set
			{
				if (value)
					workMode |= MachineWorkMode.AllowDirectAccess;
				else
					workMode &= ~(MachineWorkMode.AllowDirectAccess);
			}
		}

		/// <summary>
		/// Get or set whether allowed to ignore all exceptions that is happened from CLR calling. (default is true)
		/// </summary>
		public bool IgnoreCLRExceptions
		{
			get { return (workMode & MachineWorkMode.IgnoreCLRExceptions) == MachineWorkMode.IgnoreCLRExceptions; }
			set
			{
				if (value)
					workMode |= MachineWorkMode.IgnoreCLRExceptions;
				else
					workMode &= ~(MachineWorkMode.IgnoreCLRExceptions);
			}
		}

		/// <summary>
		/// Get or set whether allowed to auto-import the associated types that is used in other imported types. (default is true)
		/// </summary>
		public bool AutoImportRelationType
		{
			get { return (workMode & MachineWorkMode.AutoImportRelationType) == MachineWorkMode.AutoImportRelationType; }
			set
			{
				if (value)
					workMode |= MachineWorkMode.AutoImportRelationType;
				else
					workMode &= ~(MachineWorkMode.AutoImportRelationType);
			}
		}

		/// <summary>
		/// Get or set whether allowed to import .NET namespaces or classes using 'import' keyword from script. (default is fasle)
		/// </summary>
		public bool AllowImportTypeInScript
		{
			get { return (workMode & MachineWorkMode.AllowImportTypeInScript) == MachineWorkMode.AllowImportTypeInScript; }
			set
			{
				if (value)
					workMode |= MachineWorkMode.AllowImportTypeInScript;
				else
					workMode &= ~(MachineWorkMode.AllowImportTypeInScript);
			}
		}

		/// <summary>
		/// Get or set whether allowed to bind a CLR event. This option works only when AllowDirectAccess is enabled. (default is false)
		/// </summary>
		public bool AllowCLREvent
		{
			get { return (workMode & MachineWorkMode.AllowCLREventBind) == MachineWorkMode.AllowCLREventBind; }
			set
			{
				if (value)
					workMode |= MachineWorkMode.AllowCLREventBind;
				else
					workMode &= ~(MachineWorkMode.AllowCLREventBind);
			}
		}

		#endregion

		#region Invoke Function
		internal object InvokeAbstractFunction(object ownerObject, AbstractFunctionObject funObject, object[] args)
		{
			ScriptContext context = new ScriptContext(this, entryFunction);
			return InvokeFunction(context, ownerObject, funObject, args);
		}

		internal object InvokeFunction(ScriptContext context, object ownerObject, AbstractFunctionObject funObject, object[] args)
		{
			return InvokeFunction(context, ownerObject, funObject, args, 0, 0);
		}

		internal object InvokeFunction(ScriptContext context, object ownerObject, AbstractFunctionObject funObject, object[] args,
			int charIndex, int line)
		{
			if (funObject is NativeFunctionObject)
			{
				NativeFunctionObject nativeFun = funObject as NativeFunctionObject;
				return nativeFun.Invoke(context, ownerObject, args);
			}
			else if (funObject is FunctionObject)
			{
				FunctionObject fun = funObject as FunctionObject;

				CallScope lastScope = context.CallStack.Peek();
				if (lastScope != null)
				{
					lastScope.Line = line;
					lastScope.CharIndex = charIndex;
				}

				CallScope newScope = new CallScope(ownerObject, fun);

				// create inner functions from static local scope
				if (fun.FunctionInfo.InnerScope != null)
				{
					foreach (FunctionInfo fi in fun.FunctionInfo.InnerScope.Functions)
					{
						if (!fi.IsAnonymous)
						{
							Debug.Assert(!newScope.Variables.ContainsKey(fi.Name));
							newScope[fi.Name] = FunctionDefineNodeParser.CreateAndInitFunction(context, fi);
						}
					}
				}

				// prepare arguments
				if (args != null && fun.Args != null)
				{
					for (int i = 0; i < fun.Args.Length && i < args.Length; i++)
					{
						string identifier = fun.Args[i];
						newScope[identifier] = args[i];
					}
				}

				if (!fun.FunctionInfo.IsAnonymous // anonymous function's scope set by create-time
					&& fun.FunctionInfo.IsInner     // only inner function need to set outer scope

					// check whether the function will be called is inner function of current function
					&& fun.FunctionInfo.OuterScope.Functions.Any(f => f == fun.FunctionInfo))
				{
					fun.OuterCallScope = context.CurrentCallScope;
				}

				newScope[KEY___ARGS__] = args;

				context.PushCallStack(newScope, fun.FunctionInfo.IsInner || fun.FunctionInfo.IsAnonymous);

				ReturnNode returnValue = null;

				try
				{
					returnValue = ParseNode(fun.Body, context) as ReturnNode;
				}
				finally
				{
					context.PopCallStack();

					// clear cached outer stack from current function
					fun.OuterCallScope = null;
				}

				return returnValue != null ? returnValue.Value : null;
			}
			else
				throw new ReoScriptRuntimeException(string.Format("{0} is not a function", Convert.ToString(funObject)));
		}

		/// <summary>
		/// Call a function in script specified by function name. 
		/// The function object must can be retrieved from current context or global object.
		/// 
		/// This method equals the evaluation of expression: if (typeof fun == 'function') fun();
		/// </summary>
		/// <param name="funName">function name</param>
		/// <param name="p">parameters if existed</param>
		/// <returns>return from function call</returns>
		public object InvokeFunctionIfExisted(string funName, params object[] p)
		{
			return InvokeFunctionIfExisted(GlobalObject, funName, p);
		}

		/// <summary>
		/// Call a function in script specified by function name.
		/// The function object must can be retrieved from speicifed owner object.
		/// 
		/// This method equals the evaluation of expression: if (obj != null && (typeof obj.fun == 'function')) obj.fun();
		/// </summary>
		/// <param name="owner">owner object (this object after be called)</param>
		/// <param name="funName">function object to execute</param>
		/// <param name="p">parameters if existed</param>
		/// <returns>return from function call</returns>
		public object InvokeFunctionIfExisted(object owner, string funName, params object[] p)
		{
			var ctx = new ScriptContext(this, entryFunction);

			AbstractFunctionObject fun = PropertyAccessHelper.GetProperty(
				ctx, owner, funName) as AbstractFunctionObject;

			return fun != null ? InvokeFunction(ctx, owner, fun, p) : null;
		}
		#endregion

		#region Async Calling
		private bool isForceStop = false;

		/// <summary>
		/// Indicate whether the current execution is forced to stop. To force stop execution call ForceStop method.
		/// </summary>
		public bool IsForceStop { get { return isForceStop; } }

		/// <summary>
		/// Indicate whether script is running. To force stop current execution call ForceStop method.
		/// </summary>
		public bool IsRunning
		{
			get
			{
				return timeoutList.Count > 0;
				//return asyncCallThread != null;
				//return timeoutList.Count > 0;
			}
		}

		/// <summary>
		/// Force interrupt current execution.
		/// </summary>
		public void ForceStop()
		{
			isForceStop = true;

			//asyncCallerList.Clear();

			//if (asyncCallThread != null)
			//{
			//  asyncCallThread.Abort();
			//  asyncCallThread = null;
			//}

			lock (timeoutList)
			{
				for (int i = 0; i < timeoutList.Count; i++)
				{
					AsyncBackgroundWorker bw = timeoutList[i];

					try
					{
						if (bw != null)
						{
							timeoutList.Remove(bw);

							bw.CancelAsync();
							bw.Dispose();
							bw = null;
						}
					}
					catch { }
				}
			}
		}

		private List<AsyncBackgroundWorker> timeoutList = new List<AsyncBackgroundWorker>();

		private long asyncCallingCount = 0;

		internal long AsyncCall(object code, int ms, bool forever, object[] args)
		{
			if (IsForceStop) return 0;

			//AddAsyncCall(ms, () =>
			//{
			//  Thread.Sleep(ms);

			//  if (code is FunctionObject)
			//  {
			//    this.ParseNode(((FunctionObject)code).Body);
			//  }
			//  else if (code is CommonTree)
			//  {
			//    this.ParseNode(((CommonTree)code));
			//  }
			//  else
			//  {
			//    //FIXME: should merge Run and CalcExpression
			//    string codeStr = Convert.ToString(code).TrimEnd();
			//    if (codeStr.EndsWith(";") || codeStr.EndsWith("}"))
			//    {
			//      this.Run(Convert.ToString(codeStr));
			//    }
			//    else
			//    {
			//      this.CalcExpression(Convert.ToString(codeStr));
			//    }
			//  }

			//  return true;
			//});

			AsyncBackgroundWorker bw = new AsyncBackgroundWorker()
			{
				WorkerSupportsCancellation = true,
				ScriptContext = new ScriptContext(this, entryFunction),
			};

			bw.Id = ++asyncCallingCount;

			bw.DoWork += (s, e) =>
			{
				try
				{
					do
					{
						DateTime dt = DateTime.Now.AddMilliseconds(ms);

						while (DateTime.Now < dt)
						{
							if (isForceStop || bw.CancellationPending) break;
							Thread.Sleep(10);
						}

						if (isForceStop || bw.CancellationPending)
						{
							break;
						}
						else
						{
							if (code is FunctionObject)
							{
								this.InvokeFunction(bw.ScriptContext, GlobalObject, code as FunctionObject, args);
							}
							else if (code is CommonTree)
							{
								ScriptRunningMachine.ParseNode(((CommonTree)code), bw.ScriptContext);
							}
							else
							{
								this.CalcExpression(Convert.ToString(code));
							}
						}

						if (!forever)
						{
							lock (timeoutList)
							{
								timeoutList.Remove(bw);
							}
						}
					} while (forever);
				}
				catch (Exception ex)
				{
					throw ex;
				}
			};

			lock (timeoutList)
			{
				timeoutList.Add(bw);
			}
			bw.RunWorkerAsync();

			return bw.Id;
		}

		class AsyncBackgroundWorker : BackgroundWorker
		{
			public long Id { get; set; }

			public ScriptContext ScriptContext { get; set; }
		}

		internal bool CancelAsyncCall(long id)
		{
			lock (timeoutList)
			{
				for (int i = 0; i < timeoutList.Count; i++)
				{
					if (timeoutList[i].Id == id)
					{
						timeoutList[i].CancelAsync();
						timeoutList.RemoveAt(i);
						return true;
					}
				}
			}

			return false;
		}

		private Thread asyncCallThread;

		private List<AsyncCaller> asyncCallerList = new List<AsyncCaller>();

		private int minAsyncCallInterval = 0;

		private void AddAsyncCall(int interval, Func<bool> caller)
		{
			if (minAsyncCallInterval == 0
				|| minAsyncCallInterval > interval)
			{
				minAsyncCallInterval = interval;
			}

			if (minAsyncCallInterval > 10) minAsyncCallInterval = 10;

			asyncCallerList.Add(new AsyncCaller(interval, caller));

			if (asyncCallThread == null)
			{
				asyncCallThread = new Thread(AsyncCallLoop);
				asyncCallThread.Start();
			}
		}

		private void AsyncCallLoop()
		{
			DateTime dt = DateTime.Now;

			while (asyncCallerList.Count > 0)
			{
				// should wait more than 0 ms
				Debug.Assert(minAsyncCallInterval > 0 && minAsyncCallInterval <= 100);

				Thread.Sleep(minAsyncCallInterval);

				for (int i = 0; i < asyncCallerList.Count; i++)
				{
					AsyncCaller caller = asyncCallerList[i];

					double check = (DateTime.Now - caller.LastCalled).TotalMilliseconds;
					Debug.WriteLine("check = " + check);

					if (check > caller.Interval)
					{
						try
						{
							caller.LastCalled = DateTime.Now;
							caller.DoCall();
						}
						catch
						{
							// error caused in this caller, planned to remove it 
							caller.Finished = true;
						}

						Debug.WriteLine((DateTime.Now - dt).Milliseconds);
						dt = DateTime.Now;
					}

					if (caller.Finished)
					{
						asyncCallerList.Remove(caller);
					}
				}
			}

			asyncCallThread = null;
		}

		internal class AsyncCaller
		{
			public AsyncCaller(int interval, Func<bool> caller)
			{
				this.interval = interval;
				this.lastCalled = DateTime.Now;
				this.action = caller;
				this.finished = false;
			}

			private DateTime lastCalled;

			public DateTime LastCalled
			{
				get { return lastCalled; }
				set { lastCalled = value; }
			}

			private int interval;

			public int Interval
			{
				get { return interval; }
				set { interval = value; }
			}

			private Func<bool> action;

			/// <summary>
			/// Do async calling.
			/// If return true this calling will be removed from timer loop.
			/// </summary>
			/// <returns>return true if calling is finished.</returns>
			public void DoCall()
			{
				if (action != null)
				{
					if (action()) finished = true;
				}
			}

			private bool finished;

			public bool Finished
			{
				get { return finished; }
				set { finished = value; }
			}
		}
		#endregion

		#region Standard I/O Interface

		private IStandardInputProvider standardInputProvider = new BuiltinConsoleInputProvider();

		public IStandardInputProvider StandardInputProvider
		{
			get { return standardInputProvider; }
			set { standardInputProvider = value; }
		}

		private List<IStandardOutputListener> outputListeners = new List<IStandardOutputListener>();

		/// <summary>
		/// List of Standard Output Listeners
		/// </summary>
		public List<IStandardOutputListener> OutputListeners
		{
			get { return outputListeners; }
			set { outputListeners = value; }
		}

		/// <summary>
		/// Add a lisenter to get standard output of console.
		/// </summary>
		/// <param name="lisenter">a lisenter to get standard output of console</param>
		public void AddStdOutputListener(IStandardOutputListener lisenter)
		{
			if (outputListeners == null) outputListeners = new List<IStandardOutputListener>();
			outputListeners.Add(lisenter);
		}

		/// <summary>
		/// Check whether specified listener has been added.
		/// </summary>
		/// <param name="listener">a lisenter to get standard output of console</param>
		/// <returns>true if specified listener has already added.</returns>
		public bool HasStdOutputListener(IStandardOutputListener listener)
		{
			return outputListeners == null ? false : outputListeners.Contains(listener);
		}

		/// <summary>
		/// Remove listener from list of lisenters.
		/// </summary>
		/// <param name="lisenter">a lisenter to get standard output of console</param>
		public void RemoveStdOutputListener(IStandardOutputListener lisenter)
		{
			if (outputListeners == null) return;
			outputListeners.Remove(lisenter);
		}

		internal void StandardIOWrite(byte[] buf, int index, int count)
		{
			if (outputListeners != null)
			{
				outputListeners.ForEach(ol => ol.Write(buf, index, count));
				Application.DoEvents();
			}
		}

		internal void StandardIOWrite(object obj)
		{
			if (outputListeners != null)
			{
				outputListeners.ForEach(ol => ol.Write(obj));
				Application.DoEvents();
			}
		}

		internal void StandardIOWriteLine(string line)
		{
			if (outputListeners != null)
			{
				outputListeners.ForEach(ol => ol.WriteLine(line));
				Application.DoEvents();
			}
		}

		#endregion

		#region Load & Run
		/// <summary>
		/// Get or set WorkPath. WorkPath is used when importing an external script file or resource.
		/// </summary>
		public string WorkPath { get; set; }

		/// <summary>
		/// Load script file from specified stream. 
		/// </summary>
		/// <remarks>setTimeout and setInterval function will be disabled when loading script from external stream, uri or file.</remarks>
		/// <param name="s">stream to load script</param>
		public void Load(Stream s)
		{
			Load(new ANTLRInputStream(s), null);
		}

		/// <summary>
		/// Load script library from a specified uri. Uri can be remote resource on Internet.
		/// </summary>
		/// <param name="uri">uri to load script</param>
		public void Load(Uri uri)
		{
			using (WebClient c = new WebClient())
			{
				using (Stream stream = c.OpenRead(uri))
				{
					Load(stream);
				}
			}
		}

		/// <summary>
		/// Load script from specified file with full path.
		/// </summary>
		/// <param name="path">file to load script</param>
		public void Load(string path)
		{
			FileInfo fi = new FileInfo(path);
			if (!fi.Exists)
			{
				throw new ReoScriptException("File not found: " + fi.FullName);
			}

			Load(new ANTLRFileStream(path), path);
		}

		private void Load(ICharStream stream, string filepath)
		{
			Run(stream, filepath, true);
		}

		/// <summary>
		/// Run script from specified file and get the last value of execution.
		/// </summary>
		/// <param name="script">file to be executed</param>
		/// <returns>last result returned from script</returns>
		public object Run(FileInfo filePath)
		{
			return Run(filePath, true);
		}

		/// <summary>
		/// Run script from specified file and get the last value of execution.
		/// </summary>
		/// <param name="filePath">file to be executed</param>
		/// <param name="ignoreSyntaxErrors">indicates whether allowed to ignore syntax check. (default is true)</param>
		/// <returns>last result returned from script</returns>
		public object Run(FileInfo filePath, bool ignoreSyntaxErrors)
		{
			return Run(new ANTLRFileStream(filePath.FullName), filePath.FullName, ignoreSyntaxErrors);
		}

		/// <summary>
		/// Run specified script in text and get the last value of execution.
		/// </summary>
		/// <param name="script">script to be executed</param>
		/// <returns>result of last exected statement</returns>
		public object Run(string script)
		{
			if (!script.EndsWith(";")) script += ";";
			return Run(script, true);
		}

		/// <summary>
		/// Run specified script in text and get the last value from script.
		/// </summary>
		/// <param name="script">script to be executed</param>
		/// <param name="context">current script context</param>
		/// <returns>result of last evaluated statement</returns>
		public object Run(string script, ScriptContext context)
		{
			return Run(script, true, context);
		}

		/// <summary>
		/// Run specified script in text and get the last value from script.
		/// </summary>
		/// <param name="script">script to be executed</param>
		/// <param name="ignoreSyntaxErrors">indicates whether allow to ignore syntax error. (default is true)</param>
		/// <returns>result of last evaluated statement</returns>
		public object Run(string script, bool ignoreSyntaxErrors)
		{
			return Run(script, null, true);
		}

		/// <summary>
		/// Run specified script in text and get the last value from script.
		/// </summary>
		/// <param name="script">script to be executed</param>
		/// <param name="ignoreSyntaxErrors">indicates whether allow to ignore syntax error. (default is true)</param>
		/// <param name="context">current script context</param>
		/// <returns>result of last evaluated statement</returns>
		public object Run(string script, bool ignoreSyntaxErrors, ScriptContext context)
		{
			return Run(script, null, true, context);
		}

		internal object Run(string script, string filePath, bool ignoreSyntaxErrors)
		{
			return Run(new ANTLRStringStream(script), filePath, ignoreSyntaxErrors);
		}

		internal object Run(string script, string filePath, bool ignoreSyntaxErrors, ScriptContext context)
		{
			return Run(new ANTLRStringStream(script), filePath, ignoreSyntaxErrors, context);
		}

		internal object Run(ICharStream stream, string filepath, bool ignoreSyntaxErrors)
		{
			return Run(stream, filepath, ignoreSyntaxErrors ? (Action<ErrorObject>)(e => { }) : null);
		}

		internal object Run(ICharStream stream, string filepath, bool ignoreSyntaxErrors, ScriptContext context)
		{
			return Run(stream, filepath, ignoreSyntaxErrors ? (Action<ErrorObject>)(e => { }) : null, context);
		}

		internal object Run(ICharStream stream, string filePath, Action<ErrorObject> compilingErrorHandler)
		{
			ScriptContext context = new ScriptContext(this, entryFunction, filePath);
			return Run(stream, filePath, compilingErrorHandler, context);
		}

		internal object Run(ICharStream stream, string filePath, Action<ErrorObject> compilingErrorHandler, ScriptContext context)
		{
			return RunCompiledScript(Compile(stream, compilingErrorHandler), context);
		}

		/// <summary>
		/// Run compiled script
		/// </summary>
		/// <param name="cs">compiled script to run</param>
		/// <returns>last return value from script</returns>
		//public object Run(CompiledScript cs)
		//{
		//  return RunCompiledScript(cs);
		//}

		/// <summary>
		/// Run compiled script. 
		/// </summary>
		/// <see cref="CompiledScript"/>
		/// <param name="script">a compiled script will be executed</param>
		/// <returns>return value from script</returns>
		public object RunCompiledScript(CompiledScript script)
		{
			return RunCompiledScript(script, new ScriptContext(this, entryFunction));
		}

		/// <summary>
		/// Run compiled script.
		/// </summary>
		/// <see cref="CompiledScript"/>
		/// <param name="script">a compiled script will be executed</param>
		/// <param name="context">current executing context</param>
		/// <returns>return value from script</returns>
		public object RunCompiledScript(CompiledScript script, ScriptContext context)
		{
			// clear ForceStop flag
			isForceStop = false;

			if (script.RootNode == null) return null;

			// define global functions 
			if (script.RootScope != null)
			{
				foreach (FunctionInfo fi in script.RootScope.Functions.Where(fi => !fi.IsAnonymous && !fi.IsInner))
				{
					GlobalObject[fi.Name] = FunctionDefineNodeParser.CreateAndInitFunction(context, fi);
				}
			}

			// run syntax tree and return value
			return UnboxAnything(ParseNode(script.RootNode, context));
		}

		/// <summary>
		/// Unbox anything from script return
		/// </summary>
		/// <param name="o">object returned from script</param>
		/// <returns>object unboxed</returns>
		public static object UnboxAnything(object o)
		{
			// retrieve value from accessors
			if (o is IAccess) o = UnboxAnything(((IAccess)o).Get());

			// retrieve value from ReturnNode
			if (o is ReturnNode) o = UnboxAnything(((ReturnNode)o).Value);

			// retrieve value from const values
			if (o is ConstValueNode) o = UnboxAnything(((ConstValueNode)o).ConstValue);

			// retrieve value from wrapped objects
			if (o is ArrayObject) o = UnboxAnything(((ArrayObject)o).List);
			if (o is StringObject) o = UnboxAnything(((StringObject)o).String);
			if (o is NumberObject) o = UnboxAnything(((NumberObject)o).Number);
			if (o is BooleanObject) o = UnboxAnything(((BooleanObject)o).Boolean);
			if (o is DateObject) o = UnboxAnything(((DateObject)o).DateTime);

			return o;
		}

		/// <summary>
		/// Evaluate specified expression. Only an expression can be properly 
		/// calculated by this method, control statements like if, for, switch, etc.
		/// can not be executed. To execute the statements use Run method.
		/// </summary>
		/// <param name="expression">expression to be calculated</param>
		/// <returns>value of expression</returns>
		public object CalcExpression(string expression)
		{
			return CalcExpression(expression, true);
		}

		/// <summary>
		/// Evaluate specified expression. Only an expression can be properly 
		/// calculated by this method, control statements like if, for, switch, etc.
		/// can not be executed. To execute the statements use Run method.
		/// </summary>
		/// <param name="expression">expression to be calculated</param>
		/// <param name="ignoreErrors">whether allowed to ignore syntax error</param>
		/// <returns>value of expression</returns>
		public object CalcExpression(string expression, bool ignoreErrors)
		{
			return CalcExpression(expression, new ScriptContext(this, entryFunction), ignoreErrors);
		}

		public object CalcExpression(string expression, ScriptContext context)
		{
			return CalcExpression(expression, context, false);
		}

		public object CalcExpression(string expression, ScriptContext context, bool ignoreErrors)
		{
			ReoScriptLexer exprLex = new ReoScriptLexer(new ANTLRStringStream(expression));
			CommonTokenStream exprTokens = new CommonTokenStream(exprLex);
			ReoScriptParser exprParser = new ReoScriptParser(exprTokens);
			if (!ignoreErrors)
			{
				exprParser.CompilingErrorHandler = e => { throw new ReoScriptCompilingException(e); };
			}

			CommonTree t = exprParser.expression().Tree as CommonTree;
			isForceStop = false;
			object v = ParseNode(t, context);
			while (v is IAccess) v = ((IAccess)v).Get();

			return v;
		}

		private FunctionDefineNodeParser globalFunctionDefineNodeParser = new FunctionDefineNodeParser();

		/// <summary>
		/// Pre-interpret specified script. Pre-interpreting parses the script and tires to construct a 
		/// syntax-tree in memory. Syntax errors will be detected and thrown as ReoScriptCompilingException.
		/// </summary>
		/// <param name="script">script to be compiled</param>
		/// <returns>compiled script instance in memory</returns>
		public CompiledScript Compile(string script)
		{
			return Compile(script, false);
		}

		/// <summary>
		/// Pre-interpret script from specified file. 
		/// Syntax errors will be detected and thrown as ReoScriptCompilingException.
		/// </summary>
		/// <param name="file">file to be compiled</param>
		/// <returns>compiled script instance in memory</returns>
		public CompiledScript Compile(FileInfo file)
		{
			return Compile(file, false);
		}

		/// <summary>
		/// Pre-interpret specified script in text. 
		/// Syntax errors will be detected and stored in the error list of CompiledScript.
		/// </summary>
		/// <param name="script">script to be compiled</param>
		/// <returns>compiled script instance in memory</returns>
		public CompiledScript Compile(string script, bool compilingWithoutException)
		{
			return Compile(script, compilingWithoutException ? (Action<ErrorObject>)((e) => { }) : null);
		}

		/// <summary>
		/// Pre-interpret script from specified file. 
		/// Syntax errors will be detected and stored in the error list of CompiledScript.
		/// </summary>
		/// <param name="script">specified script to be compiled</param>
		/// <returns>compiled script instance in memory</returns>
		public CompiledScript Compile(FileInfo file, bool compilingWithoutException)
		{
			return Compile(file, compilingWithoutException ? (Action<ErrorObject>)((e) => { }) : null);
		}

		/// <summary>
		/// Pre-interpret script from specified file. 
		/// Syntax errors will be detected and passed into CompilingErrorHandler instantly. 
		/// </summary>
		/// <param name="file">file specified to compile</param>
		/// <param name="compilingErrorHandler">error handler to receive a syntax error instantly</param>
		/// <returns>compiled script instance in memory</returns>
		public CompiledScript Compile(FileInfo file, Action<ErrorObject> compilingErrorHandler)
		{
			return Compile(new ANTLRFileStream(file.FullName), compilingErrorHandler);
		}

		/// <summary>
		/// Pre-interpret specified script in text. 
		/// Syntax errors will be detected and passed into CompilingErrorHandler instantly. 
		/// </summary>
		/// <param name="script">script specified to compile</param>
		/// <param name="compilingErrorHandler">error handler to receive syntax error</param>
		/// <returns>compiled script instance in memory</returns>
		public CompiledScript Compile(string script, Action<ErrorObject> compilingErrorHandler)
		{
			return Compile(new ANTLRStringStream(script), compilingErrorHandler);
		}

		internal CompiledScript Compile(ICharStream stream, Action<ErrorObject> compilingErrorHandler)
		{
			ReoScriptLexer lex = new ReoScriptLexer(stream);
			CommonTokenStream tokens = new CommonTokenStream(lex);
			ReoScriptParser parser = new ReoScriptParser(tokens);

			if (compilingErrorHandler == null)
			{
				compilingErrorHandler = e => { throw new ReoScriptCompilingException(e); };
			}

			parser.CompilingErrorHandler = compilingErrorHandler;

			// parse script and build AST
			CommonTree t = parser.script().Tree;

			return new CompiledScript
			{
				RootNode = t,
				CompilingErrors = parser.CompilingErrors,
				RootScope = parser.CurrentStack,
			};
		}

		internal static void IterateAST(CommonTree parent, Action<CommonTree> iterate)
		{
			if (parent != null && parent.ChildCount > 0)
			{
				foreach (CommonTree t in parent.Children)
				{
					iterate(t);
				}
			}
		}

		#endregion

		#region Node Parsing
		private IParserAdapter parserAdapter = new AWDLLogicSyntaxParserAdapter();

		internal IParserAdapter ParserAdapter
		{
			get { return parserAdapter; }
		}

		private static readonly AWDLLogicSyntaxParserAdapter parseAdapter = new AWDLLogicSyntaxParserAdapter();

		internal IParserAdapter SelectParserAdapter(IParserAdapter adapter)
		{
			IParserAdapter oldAdapter = this.parserAdapter;
			this.parserAdapter = adapter;
			return oldAdapter;
		}

		public static object ParseNode(CommonTree t, ScriptContext ctx)
		{
			if (t == null || ctx.Srm.isForceStop)
			{
				return null;
			}

			INodeParser parser = null;
			if ((parser = AWDLLogicSyntaxParserAdapter.definedParser[t.Type]) != null)
			{
				return parser.Parse(t, ctx.Srm, ctx);
			}
			else
			{
				switch (t.Type)
				{
					case ReoScriptLexer.IDENTIFIER:
						{
							if (t.Text == ScriptRunningMachine.GLOBAL_VARIABLE_NAME)
								return ctx.GlobalObject;
							else
							{
								return ctx[t.Text];
							}
							//return new VariableAccess(ctx.Srm, ctx, t.Text);
						}

					case ReoScriptLexer.THIS:
						return ctx.ThisObject;

					case ReoScriptLexer.CONST_VALUE:
						{
							CommonTree child = (CommonTree)t.Children[0];

							switch (child.Type)
							{
								case ReoScriptLexer.CONST_VALUE:
									return ((ConstValueNode)child).ConstValue;

								case ReoScriptLexer.LIT_TRUE:
									return true;

								case ReoScriptLexer.LIT_FALSE:
									return false;

								case ReoScriptLexer.LIT_NULL:
								case ReoScriptLexer.UNDEFINED:
									return null;
							}
						}
						break;

					//case ReoScriptLexer.NUMBER_LITERATE:
					//  return Convert.ToDouble(t.Text);

					//case ReoScriptLexer.HEX_LITERATE:
					//  return (double)Convert.ToInt32(t.Text.Substring(2), 16);

					//case ReoScriptLexer.BINARY_LITERATE:
					//  return (double)Convert.ToInt32(t.Text.Substring(2), 2);

					//case ReoScriptLexer.STRING_LITERATE:
					//  string str = t.ToString();
					//  str = str.Substring(1, str.Length - 2);
					//  return ConvertEscapeLiterals(str);


					case ReoScriptLexer.OBJECT_LITERAL:
						if (t.ChildCount % 2 != 0)
							throw ctx.CreateRuntimeError(t, "object literal should be key/value paired.");

						ObjectValue val = ctx.CreateNewObject();

						for (int i = 0; i < t.ChildCount; i += 2)
						{
							object value = ParseNode((CommonTree)t.Children[i + 1], ctx);
							if (value is IAccess) value = ((IAccess)value).Get();

							string identifier = t.Children[i].ToString();
							if (t.Children[i].Type == ReoScriptLexer.STRING_LITERATE)
								identifier = identifier.Substring(1, identifier.Length - 2);

							val[identifier] = value;
						}

						return val;

					case ReoScriptLexer.ARRAY_LITERAL:
						ArrayObject arr = ctx.CreateNewObject(ctx.Srm.BuiltinConstructors.ArrayFunction) as ArrayObject;

						if (arr == null) return arr;

						for (int i = 0; i < t.ChildCount; i++)
						{
							object value = ParseNode((CommonTree)t.Children[i], ctx);
							if (value is IAccess) value = ((IAccess)value).Get();
							arr.List.Add(value);
						}
						return arr;

					case ReoScriptLexer.REPLACED_TREE:
						return ((ReplacedCommonTree)t).Object;

					case ReoScriptLexer.NAN:
						return NaNValue.Value;

					case ReoScriptLexer.BREAK:
						return BreakNode.Value;

					case ReoScriptLexer.CONTINUE:
						return ContinueNode.Value;
				}

				return ParseChildNodes(t, ctx);
			}
		}
		public static object ParseChildNodes(CommonTree t, ScriptContext ctx)
		{
			object childValue = null;
			if (t.ChildCount > 0)
			{
				//foreach (CommonTree child in t.Children)
				for (int i = 0; i < t.ChildCount; i++)
				{
					childValue = ParseNode((CommonTree)t.Children[i], ctx);

					if (childValue is BreakNode || childValue is ContinueNode || childValue is ReturnNode)
						return childValue;
				}
			}
			return childValue;
		}
		#endregion

		#region Utility
		internal static string ConvertEscapeLiterals(string instr)
		{
			return instr.Replace("\\n", new string(new char[] { '\n' }))
				.Replace("\\t", new string(new char[] { '\t' }))
				.Replace("\\\\", new string(new char[] { '\\' }));
		}

		internal object[] GetParameterList(CommonTree paramsTree, ScriptContext ctx)
		{
			int argCount = paramsTree == null ? 0 : paramsTree.ChildCount;
			object[] args = new object[argCount];

			if (argCount > 0)
			{
				for (int i = 0; i < argCount; i++)
				{
					object val = ParseNode((CommonTree)paramsTree.Children[i], ctx);
					if (val is IAccess) val = ((IAccess)val).Get();

					args[i] = val;
				}
			}

			return args;
		}

		#region Get primitive data
		/// <summary>
		/// Convert object into integer value.
		/// </summary>
		/// <param name="obj">object to be converted</param>
		/// <returns>converted integer value</returns>
		public static int GetIntValue(object obj)
		{
			return GetIntValue(obj, 0);
		}

		/// <summary>
		/// Convert object into integer value from argument array.
		/// </summary>
		/// <param name="args">argument array to get integer value</param>
		/// <param name="index">zero-based index to get integer value from argument array</param>
		/// <returns>converted integer value</returns>
		public static int GetIntParam(object[] args, int index)
		{
			return GetIntParam(args, index, 0);
		}

		/// <summary>
		/// Convert object into integer value from argument array.
		/// </summary>
		/// <param name="args">argument array to get integer value</param>
		/// <param name="index">zero-based index to get integer value from argument array</param>
		/// <param name="def">default value will be return if converting is failed</param>
		/// <returns>converted integer value</returns>
		public static int GetIntParam(object[] args, int index, int def)
		{
			if (args.Length <= index)
				return def;
			else
				return GetIntValue(args[index], def);
		}

		/// <summary>
		/// Convert object into integer value. If converting is failed, a specified default value will be returned.
		/// </summary>
		/// <param name="obj">object to be converted</param>
		/// <param name="def">default value will be return if converting is failed</param>
		/// <returns>converted integer value</returns>
		public static int GetIntValue(object obj, int def)
		{
			if (obj is int || obj is long)
			{
				return (int)obj;
			}
			else if (obj is double || ScriptRunningMachine.IsPrimitiveNumber(obj))
			{
				return (int)(double)obj;
			}
			else if (obj is string || obj is StringObject)
			{
				double v = def;
				double.TryParse(Convert.ToString(obj), out v);
				return (int)v;
			}
			else if (obj is NumberObject)
			{
				return (int)((NumberObject)obj).Number;
			}
			else
				return def;
		}

		/// <summary>
		/// Convert object into long integer value.
		/// </summary>
		/// <param name="obj">object to be converted</param>
		/// <returns>converted long integer value</returns>
		public static long GetLongValue(object obj)
		{
			return GetLongValue(obj, 0);
		}

		/// <summary>
		/// Convert object into long integer value from argument array.
		/// </summary>
		/// <param name="args">argument array to get long integer value</param>
		/// <param name="index">zero-based index to get long integer value from argument array</param>
		/// <param name="def">default value will be return if converting is failed</param>
		/// <returns>converted long integer value</returns>
		public static long GetLongParam(object[] args, int index, long def)
		{
			if (args.Length <= index)
				return def;
			else
				return GetLongValue(args[index], def);
		}

		/// <summary>
		/// Convert object into long integer value. If converting is failed, 
		/// a specified default value will be returned.
		/// </summary>
		/// <param name="obj">object to be converted</param>
		/// <param name="def">default value will be return if converting is failed</param>
		/// <returns>converted long integer value</returns>
		public static long GetLongValue(object obj, long def)
		{
			if (obj is int || obj is long)
			{
				return (long)obj;
			}
			else if (obj is double || ScriptRunningMachine.IsPrimitiveNumber(obj))
			{
				return (long)(double)obj;
			}
			else if (obj is string || obj is StringObject)
			{
				double v = def;
				double.TryParse(Convert.ToString(obj), out v);
				return (long)v;
			}
			else if (obj is NumberObject)
			{
				return (long)((NumberObject)obj).Number;
			}
			else
				return def;
		}

		public static float GetFloatValue(object obj)
		{
			return GetFloatValue(obj, 0);
		}

		/// <summary>
		/// Convert object into float value. If converting is failed, a specified default value will be returned.
		/// </summary>
		/// <param name="obj">object to be converted</param>
		/// <param name="def">default value will be return if converting is failed</param>
		/// <returns>converted float value</returns>
		public static float GetFloatValue(object obj, float def)
		{
			if (obj is double)
			{
				return (float)(double)obj;
			}
			else if (obj is float)
			{
				return (float)obj;
			}
			else if (ScriptRunningMachine.IsPrimitiveNumber(obj))
			{
				return (float)(double)obj;
			}
			else if (obj is string || obj is StringObject)
			{
				double v = def;
				double.TryParse(Convert.ToString(obj), out v);
				return (float)v;
			}
			else if (obj is NumberObject)
			{
				return (float)((NumberObject)obj).Number;
			}
			else
				return def;
		}

		/// <summary>
		/// Convert object into float value from argument array.
		/// </summary>
		/// <param name="args">argument array to get float value</param>
		/// <param name="index">zero-based index to get float value from argument array</param>
		/// <param name="def">default value will be return if converting is failed</param>
		/// <returns>converted float value</returns>
		public static float GetFloatParam(object[] args, int index, float def)
		{
			if (args.Length <= index)
				return def;
			else
				return GetFloatValue(args[index], def);
		}

		public static double GetNumberValue(object obj)
		{
			return GetNumberValue(obj, 0);
		}

		/// <summary>
		/// Convert object into double value. If converting is failed, a specified default value will be returned.
		/// </summary>
		/// <param name="obj">object to be converted</param>
		/// <param name="def">default value will be return if converting is failed</param>
		/// <returns>converted double value</returns>
		public static double GetNumberValue(object obj, double def)
		{
			if (obj is double)
			{
				return (double)obj;
			}
			else if (ScriptRunningMachine.IsPrimitiveNumber(obj))
			{
				return Convert.ToDouble(obj);
			}

			else if (obj is string || obj is StringObject)
			{
				double v = def;
				double.TryParse(Convert.ToString(obj), out v);
				return v;
			}
			else if (obj is NumberObject)
			{
				return ((NumberObject)obj).Number;
			}
			else
				return def;
		}

		public static bool TryGetNumberValue(object obj, out double value)
		{
			if (obj is double)
			{
				value = (double)obj;
				return true;
			}
			else if (IsPrimitiveNumber(obj))
			{
				value = Convert.ToDouble(obj);
				return true;
			}
			else if (IsPrimitiveString(obj))
			{
				return double.TryParse(ConvertToString(obj), out value);
			}
			else if (obj is NumberObject)
			{
				value = (obj as NumberObject).Number;
				return true;
			}

			value = 0;
			return false;
		}

		/// <summary>
		/// Convert object into boolean value. If converting is failed, false will be returned.
		/// </summary>
		/// <param name="obj">object to be converted</param>
		/// <returns>converted boolean value</returns>
		public static bool GetBoolValue(object obj)
		{
			return ((obj is bool) && ((bool)obj)) || ((obj is BooleanObject) && (((BooleanObject)obj).Boolean));
		}

		/// <summary>
		/// Convert an object into string.
		/// </summary>
		/// <param name="v">object will be converted</param>
		/// <returns>converted string</returns>
		public static string ConvertToString(object v)
		{
			if (v == null)
			{
				return "null";
			}
			else if (v is bool)
			{
				return ((bool)v) ? "true" : "false";
			}
			else
			{
				return Convert.ToString(v);
			}
		}
		#endregion

		internal static string GetNativeIdentifier(string identifier)
		{
			return string.IsNullOrEmpty(identifier) ? string.Empty :
				(identifier.Substring(0, 1).ToUpper() + identifier.Substring(1));
		}

		internal object ConvertToCLRType(ScriptContext context, object value, Type type)
		{
			if (type == typeof(string))
			{
				return Convert.ToString(value);
			}
			else if (type == typeof(int))
			{
				if (value is double)
				{
					return (int)((double)value);
				}
				else
				{
					return Convert.ToInt32(value);
				}
			}
			else if (type == typeof(long))
			{
				if (value is double)
				{
					return (long)((double)value);
				}
				else
					return Convert.ToInt64(value);
			}
			else if (type == typeof(float))
			{
				if (value is double)
				{
					return (float)(double)value;
				}
				else
					return Convert.ToSingle(value);
			}
			else if (value is ObjectValue)
			{
				if (type == typeof(ObjectValue))
				{
					return value;
				}
				else if (AllowDirectAccess)
				{
					object obj;

					if (type.IsArray && value is ArrayObject)
					{
						ArrayObject arrSource = (ArrayObject)value;
						int count = arrSource.List.Count;

						object[] arrTo = Array.CreateInstance(type.GetElementType(), count) as object[];

						for (int i = 0; i < count; i++)
						{
							arrTo[i] = ConvertToCLRType(context, arrSource.List[i], type.GetElementType());
						}

						obj = arrTo;
					}
					else
					{
						try
						{
							if (type.IsEnum && value is string || value is NumberObject)
							{
								obj = Enum.Parse(type, Convert.ToString(value));
							}
							else
							{
								obj = System.Activator.CreateInstance(type);
							}
						}
						catch (Exception ex)
						{
							throw new ReoScriptException("cannot convert to .NET object from value: " + value, ex);
						}

						CombineObject(context, obj, (ObjectValue)value);
					}
					return obj;
				}
				else
					return value;
			}
			else
			{
				return value;
			}
		}

		/// <summary>
		/// Check whether a specified object is number.
		/// </summary>
		/// <param name="target">object will be checked.</param>
		/// <returns>true if specified object is of number type.</returns>
		public static bool IsPrimitiveNumber(object target)
		{
			return target is double || target is int || target is float || target is char || target is byte
				|| target is short || target is long;
		}

		/// <summary>
		/// Check whether a specified object is string.
		/// </summary>
		/// <param name="target">object will be checked.</param>
		/// <returns>true if specified object is instance of <class>StringObject</class>, <class>string</class> or <class>StringBuilder</class>.</returns>
		public static bool IsPrimitiveString(object target)
		{
			return target is string || target is StringObject || target is StringBuilder;
		}

		internal static bool IsPrimitiveTypes(object obj)
		{
			return obj == null || obj is bool || obj is string || IsPrimitiveNumber(obj);
		}

		internal static MethodInfo FindCLRMethodAmbiguous(object obj, string methodName, object[] args)
		{
			var q = obj.GetType().GetMethods(BindingFlags.Instance | BindingFlags.Public).Where(_q => _q.Name == methodName);

			//MethodInfo method = null;

			foreach (MethodInfo mi in q)
			{
				ParameterInfo[] pi = mi.GetParameters();

				if (pi.Length == args.Length)
				{
					return mi;
				}
				//else
				//{
				//  for (int i = 0; i < args.Length && i < pi.Length; i++)
				//  {
				//    if (pi[i].ParameterType == typeof(string))
				//    {
				//      if(
				//    }
				//  }

				//  // compare every parameters
				//}

			}

			return q == null || q.Count() == 0 ? null : q.First();
		}

		#region JSON Converation
		/// <summary>
		/// Convert an object into JSON string
		/// </summary>
		/// <param name="obj">object will be converted</param>
		/// <param name="handler">handler to process every properties. This parameter can be null.</param>
		/// <returns>converted JSON string</returns>
		public static string ConvertToJSONString(object obj, Func<string, object, object> handler)
		{
			return ConvertToJSONString(obj, handler, true);
		}

		internal static string ConvertToJSONString(object obj, Func<string, object, object> handler, bool allowDotNetObjects)
		{
			StringBuilder sb = new StringBuilder("{");

			if (obj is ISyntaxTreeReturn)
			{
				if (obj is ObjectValue)
				{
					ObjectValue objVal = (ObjectValue)obj;
					foreach (string key in objVal)
					{
						if (sb.Length > 1) sb.Append(',');
						ConvertPropertyToJSONString(sb, key, objVal[key], handler);
					}
				}
			}
			else if (obj is IDictionary<string, object>)
			{
				IDictionary<string, object> objVal = (IDictionary<string, object>)obj;
				foreach (string key in objVal.Keys)
				{
					if (sb.Length > 1) sb.Append(',');
					ConvertPropertyToJSONString(sb, key, objVal[key], handler);
				}
			}
			else if (obj is KeyValuePair<string, object>)
			{
				KeyValuePair<string, object> objVal = (KeyValuePair<string, object>)obj;
				ConvertPropertyToJSONString(sb, objVal.Key, objVal.Value, handler);
			}
			else if (allowDotNetObjects)
			{
				PropertyInfo[] pis = obj.GetType().GetProperties(BindingFlags.Instance | BindingFlags.Public);
				foreach (PropertyInfo pi in pis)
				{
					if (sb.Length > 1) sb.Append(',');
					ConvertPropertyToJSONString(sb, pi.Name, pi.GetValue(obj, null), handler);
				}
			}

			sb.Append('}');
			return sb.ToString();
		}

		private static void ConvertPropertyToJSONString(StringBuilder sb, string key, object value, Func<string, object, object> handler)
		{
			sb.Append(key);
			sb.Append(':');

			if (handler != null) value = handler(key, value);

			if (value == null)
			{
				sb.Append("null");
			}
			else if (value is string || value is StringObject)
			{
				string str = Convert.ToString(value);
				str = str.IndexOf('\"') < 0 ? ("\"" + str + "\"") : ("'" + str + "'");
				sb.Append(str);
			}
			else
			{
				sb.Append(Convert.ToString(value));
			}
		}
		#endregion

		#endregion

		#region Events
		internal event EventHandler<ReoScriptObjectEventArgs> NewObjectCreated;
		//internal event EventHandler<ReoScriptObjectEventArgs> PropertyDeleted;

		/// <summary>
		/// Event fired when script running machine is resetted.
		/// </summary>
		public event EventHandler Resetted;
		#endregion

		#region Namespace & Class & CodeFile
		private readonly Dictionary<string, AbstractFunctionObject> classDefines
			= new Dictionary<string, AbstractFunctionObject>();

		/// <summary>
		/// Check whether specified class has been registered into script context.
		/// </summary>
		/// <param name="name">class name to be checked</param>
		/// <returns>true if specified class has been registered</returns>
		public bool HasClassRegistered(string name)
		{
			return classDefines.ContainsKey(name);
		}

		internal AbstractFunctionObject GetClass(string name)
		{
			if (classDefines.ContainsKey(name))
				return classDefines[name];
			if (this.GlobalObject[name] is AbstractFunctionObject)
				return this[name] as AbstractFunctionObject;
			else
				return null;
		}

		/// <summary>
		/// Register a class into script running machine.
		/// </summary>
		/// <param name="constructor">constructor of class to be registered</param>
		public void RegisterClass(AbstractFunctionObject constructor)
		{
			RegisterClass(constructor, constructor.FunName);
		}

		/// <summary>
		/// Register a class into script running machine.
		/// </summary>
		/// <param name="constructor">constructor of class to be registered</param>
		/// <param name="name"></param>
		public void RegisterClass(AbstractFunctionObject constructor, string name)
		{
			if (string.IsNullOrEmpty(name))
				name = constructor.FunName;

			if (string.IsNullOrEmpty(name))
			{
				throw new ReoScriptException("Class should has a name to register: " + constructor.ToString());
			}

			if (classDefines.ContainsKey(name))
			{
				throw new ReoScriptException(string.Format("Class named '{0}' has been defined.", name));
			}

			// if object is function, prepare its prototype 
			if (constructor[KEY_PROTOTYPE] == null)
			{
				constructor[KEY_PROTOTYPE] = constructor.CreatePrototype(new ScriptContext(this, entryFunction));
			}

			constructor.FunName = name;
			classDefines[name] = constructor;
		}

		private static readonly List<string> importedCodeFiles = new List<string>();

		internal void ImportCodeFile(string fullPath)
		{
			if (!importedCodeFiles.Contains(fullPath))
			{
				importedCodeFiles.Add(fullPath);

				Load(fullPath);
			}
		}

		#endregion

		//public void Test()
		//{
		//	string script = @"c = 10; function xx(a,b,c){ return {aa:'bb',result:a+b+c}} arr=[1,2,3]; xx(10,11,12) ";
		//	ReoScriptLexer l = new ReoScriptLexer(new ANTLRStringStream(script));
		//	CommonTokenStream t = new CommonTokenStream(l);
		//	ReoScriptParser p = new ReoScriptParser(t);

		//	CommonTree tree = p.script().Tree;


		//}
	}

	#region CompiledScript

	/// <summary>
	/// Compiled script instance in memory. Script in text will be pre-interpreted and converted into a syntax-tree. 
	/// </summary>
	public class CompiledScript
	{
		internal CommonTree RootNode { get; set; }

		/// <summary>
		/// Errors happened at compiling-time
		/// </summary>
		public List<ErrorObject> CompilingErrors { get; set; }

		internal StaticFunctionScope RootScope { get; set; }

		internal CompiledScript()
		{
		}

		/// <summary>
		/// Get all global functions defined in global object.
		/// </summary>
		public List<FunctionInfo> DeclaredFunctions
		{
			get
			{
				return RootScope.Functions;
			}
		}

		/// <summary>
		/// Get all local variables defined in global object.
		/// </summary>
		public List<VariableInfo> DeclaredVariables
		{
			get
			{
				return RootScope.Variables;
			}
		}

		internal static IEnumerable IterateAST(CommonTree node)
		{
			if (node != null && node.ChildCount > 0)
			{
				foreach (CommonTree t in node.Children)
				{
					if (t.ChildCount > 0)
						yield return IterateAST(node);
					else
						yield return t;
				}
			}
		}

		//public void IterateProperties(Action<string, string> iterate)
		//{

		//}
	}

	#endregion

	#region built-in Object Constructors
	internal class BuiltinConstructors
	{
		internal ObjectConstructorFunction ObjectFunction;
		internal StringConstructorFunction StringFunction;
		internal ArrayConstructorFunction ArrayFunction;
		internal TypedNativeFunctionObject FunctionFunction;
		internal TypedNativeFunctionObject NumberFunction;
		internal TypedNativeFunctionObject DateFunction;
		internal ErrorConstructorFunction ErrorFunction;
		internal NativeFunctionObject BooleanFunction;

		public BuiltinConstructors()
		{
			ObjectFunction = new ObjectConstructorFunction();
			StringFunction = new StringConstructorFunction();

			#region Function
			FunctionFunction = new TypedNativeFunctionObject
				(typeof(FunctionObject), "Function", (ctx, owner, args) =>
				{
					FunctionObject fun = owner as FunctionObject;
					//TOOD: create function from string
					if (fun == null) fun = ctx.CreateNewObject(FunctionFunction, false) as FunctionObject;
					return fun;
				}, (proto) =>
				{
					proto["call"] = new NativeFunctionObject("call", (ctx, owner, args) =>
					{
						AbstractFunctionObject func = owner as AbstractFunctionObject;
						if (func != null)
						{
							object[] callArgs = null;

							if (args.Length > 1)
							{
								callArgs = new object[args.Length - 1];
								Array.Copy(args, 1, callArgs, 0, args.Length - 1);
							}

							return ctx.Srm.InvokeFunction(ctx, args.Length > 0 ? args[0] : null, func, callArgs);
						}
						return null;
					});

					proto["apply"] = new NativeFunctionObject("apply", (ctx, owner, args) =>
					{
						AbstractFunctionObject func = owner as AbstractFunctionObject;
						if (func != null)
						{
							List<object> callArgs = new List<object>();

							if (args.Length > 1 && args[1] is IEnumerable)
							{
								foreach (var arg in (args[1] as IEnumerable))
								{
									callArgs.Add(arg);
								}
							}

							return ctx.Srm.InvokeFunction(ctx, args.Length > 0 ? args[0] : null, func,  callArgs.ToArray());
						}
						return null;
					});
				});
			#endregion

			#region Number
			NumberFunction = new TypedNativeFunctionObject
				(typeof(NumberObject), "Number", (ctx, owner, args) =>
				{
					if (args == null || args.Length <= 0)
					{
						return 0;
					}

					double num = 0;
					if (double.TryParse(Convert.ToString(args[0]), out num))
					{
						return num;
					}
					else
					{
						return NaNValue.Value;
					}
				}, (proto) =>
				{
					proto["toString"] = new NativeFunctionObject("toString", (ctx, owner, args) =>
					{
						int radix = 10;

						if (args != null && args.Length > 0)
						{
							radix = ScriptRunningMachine.GetIntParam(args, 0);
						}

						double num = 0;

						if (ScriptRunningMachine.IsPrimitiveNumber(owner))
						{
							num = ScriptRunningMachine.GetNumberValue(owner);
						}
						else if (owner is NumberObject)
						{
							num = ((NumberObject)owner).Number;
						}

						try
						{
							return Convert.ToString((int)num, radix);
						}
						catch
						{
							throw new ReoScriptRuntimeException("Number.toString radix argument must between 2 and 36.");
						}
					});
				});
			#endregion

			#region Date
			DateFunction = new TypedNativeFunctionObject
				(typeof(DateObject), "Date", (ctx, owner, args) =>
				{
					DateObject dateObj = owner as DateObject;
					if (dateObj == null) dateObj = ctx.CreateNewObject(DateFunction, false) as DateObject;
					return dateObj;
				}, (obj) =>
				{
					if (obj is ObjectValue)
					{
						ObjectValue proto = obj as ObjectValue;
						proto["subtract"] = new NativeFunctionObject("subtract", (ctx, owner, args) =>
						{
							if (args.Length < 1 || !(args[0] is DateObject) || !(owner is DateObject)) return NaNValue.Value;

							DateObject ownerProto = (DateObject)owner;
							return ownerProto.DateTime.Subtract(((DateObject)args[0]).DateTime).TotalMilliseconds;
						});
					}
				});
			#endregion

			#region Array
			ArrayFunction = new ArrayConstructorFunction();
			#endregion

			#region Error
			ErrorFunction = new ErrorConstructorFunction();
			#endregion

			#region Boolean
			BooleanFunction = new TypedNativeFunctionObject<BooleanObject>("Boolean", (ctx, owner, args) =>
			{
				if (args.Length == 0)
				{
					return false;
				}
				else
				{
					object obj = args[0];

					return ((obj is bool) && ((bool)obj));
				}
			}, (protoObj) =>
			{
				if (protoObj is ObjectValue)
				{
					ObjectValue proto = (ObjectValue)protoObj;

					proto["valueOf"] = new NativeFunctionObject("valueOf", (ctx, owner, args) =>
					{
						if (owner is BooleanObject)
						{
							return ((BooleanObject)owner).Boolean;
						}
						else
						{
							return false;
						}
					});
				}
			});
			#endregion
		}

		#region Internal Global Functions
		private static readonly NativeFunctionObject __setTimeout__ = new NativeFunctionObject("setTimeout", (ctx, owner, args) =>
		{
			if (args.Length < 2) return 0;
			int interval = ScriptRunningMachine.GetIntParam(args, 1, 1000);
			object[] callArgs = null;
			if (args != null && args.Length > 2)
			{
				callArgs = new object[args.Length - 2];
				Array.Copy(args, 2, callArgs, 0, args.Length - 2);
			}
			return ctx.Srm.AsyncCall(args[0], interval, false, callArgs);
		});

		private static readonly NativeFunctionObject __setInterval__ = new NativeFunctionObject("setInterval", (ctx, owner, args) =>
		{
			if (args.Length < 2) return 0;
			int interval = ScriptRunningMachine.GetIntParam(args, 1, 1000);
			object[] callArgs = null;
			if (args != null && args.Length > 2)
			{
				callArgs = new object[args.Length - 2];
				Array.Copy(args, 2, callArgs, 0, args.Length - 2);
			}
			return ctx.Srm.AsyncCall(args[0], interval, true, callArgs);
		});

		private static readonly NativeFunctionObject __clearTimeout__ = new NativeFunctionObject("clearTimeout", (ctx, owner, args) =>
		{
			if (args.Length < 1) return 0;
			long id = ScriptRunningMachine.GetLongParam(args, 0, 0);
			return ctx.Srm.CancelAsyncCall(id);
		});

		private static readonly NativeFunctionObject __alert__ = new NativeFunctionObject("alert", (ctx, owner, args) =>
		{
			if (args.Length > 0)
			{
				MessageBox.Show(Convert.ToString(args[0]), args.Length > 1 ? Convert.ToString(args[1]) : string.Empty);
			}
			return null;
		});

		private static readonly NativeFunctionObject __confirm__ = new NativeFunctionObject("confirm", (ctx, owner, args) =>
		{
			if (args.Length > 0)
			{
				return MessageBox.Show(
					Convert.ToString(args[0]),
					args.Length > 1 ? Convert.ToString(args[1]) : string.Empty,
					MessageBoxButtons.YesNo,
					MessageBoxIcon.Question) == DialogResult.Yes;
			}
			return null;
		});

		private static readonly NativeFunctionObject __eval__ = new NativeFunctionObject("eval", (ctx, owner, args) =>
		{
			if (args.Length == 0) return false;
			return ctx.Srm.CalcExpression(Convert.ToString(args[0]), ctx);
		});
		#endregion

		public void ApplyToScriptRunningMachine(ScriptRunningMachine srm)
		{
			if (srm != null && srm.GlobalObject != null)
			{
				// built-in object constructors
				srm.SetGlobalVariable(ObjectFunction.FunName, ObjectFunction);
				srm.SetGlobalVariable(FunctionFunction.FunName, FunctionFunction);
				srm.SetGlobalVariable(ErrorFunction.FunName, ErrorFunction);
				srm.SetGlobalVariable(StringFunction.FunName, StringFunction);
				srm.SetGlobalVariable(NumberFunction.FunName, NumberFunction);
				srm.SetGlobalVariable(DateFunction.FunName, DateFunction);
				srm.SetGlobalVariable(ArrayFunction.FunName, ArrayFunction);
				srm.SetGlobalVariable(BooleanFunction.FunName, BooleanFunction);

				// built-in objects
				srm.SetGlobalVariable("Math", new MathObject());

				if ((srm.CoreFeatures & CoreFeatures.Console) == CoreFeatures.Console)
					srm.GlobalObject["console"] = srm.CreateNewObject();

				if ((srm.CoreFeatures & CoreFeatures.Eval) == CoreFeatures.Eval)
					srm.GlobalObject[__eval__.FunName] = __eval__;

				if ((srm.CoreFeatures & CoreFeatures.AsyncCalling) == CoreFeatures.AsyncCalling)
				{
					srm.GlobalObject[__setTimeout__.FunName] = __setTimeout__;
					srm.GlobalObject[__clearTimeout__.FunName] = __clearTimeout__;
				}

				if ((srm.CoreFeatures & CoreFeatures.AsyncCalling) == CoreFeatures.AsyncCalling)
				{
					srm.GlobalObject[__setInterval__.FunName] = __setInterval__;
					srm.GlobalObject["clearInterval"] = __clearTimeout__;
				}

				if ((srm.CoreFeatures & CoreFeatures.Alert) == CoreFeatures.Alert)
				{
					srm.GlobalObject[__alert__.FunName] = __alert__;
					srm.GlobalObject[__confirm__.FunName] = __confirm__;
				}

				#region JSON
				if ((srm.CoreFeatures & CoreFeatures.JSON) == CoreFeatures.JSON)
				{
					ObjectValue json = new ObjectValue()
					{
						//Name = "JSON",
					};

					json["parse"] = new NativeFunctionObject("parse", (ctx, owner, args) =>
					{
						if (args.Length == 0) return null;

						string lit = Convert.ToString(args[0]);
						if (string.IsNullOrEmpty(lit)) return null;

						ReoScriptLexer lex = new ReoScriptLexer(new ANTLRStringStream(lit.Trim()));
						CommonTokenStream tokens = new CommonTokenStream(lex);
						ReoScriptParser parser = new ReoScriptParser(tokens);

						// read script and build ASTree
						ObjectValue obj = ctx.CreateNewObject();

						if (args.Length == 1 || args[1] == null || (!(args[1] is AbstractFunctionObject)))
						{
							parser.jsonParse(ctx, (key, value) => { obj[key] = value; });
						}
						else
						{
							AbstractFunctionObject func = ((args[1]) as AbstractFunctionObject);
							parser.jsonParse(ctx, (key, value) =>
							{
								obj[key] = srm.InvokeAbstractFunction(ctx.ThisObject, func, new object[] { key, value });
							});
						}

						return obj;
					});

					json["stringify"] = new NativeFunctionObject("stringify", (ctx, owner, args) =>
					{
						if (args.Length == 0 || args[0] == null)
						{
							return string.Empty; // FIXME: StringObject ?
						}

						AbstractFunctionObject func = null;
						if (args.Length >= 2 && args[1] is AbstractFunctionObject)
						{
							func = ((args[1]) as AbstractFunctionObject);
						}

						return ScriptRunningMachine.ConvertToJSONString(args[0], (key, value) =>
						{
							return func == null ? value : srm.InvokeAbstractFunction(ctx.ThisObject, func, new object[] { key, value });
						}, srm.AllowDirectAccess);
					});

					srm.SetGlobalVariable("JSON", json);
				}
				#endregion
			}
		}
	}
	#endregion

	#region WorkMode & CoreLibrary
	/// <summary>
	/// Defines and represents the working mode of script running machine
	/// </summary>
	public enum MachineWorkMode
	{
		/// <summary>
		/// Default working mode
		/// </summary>
		Default = 0 | MachineWorkMode.IgnoreCLRExceptions | MachineWorkMode.AutoImportRelationType |
			MachineWorkMode.AutoUppercaseWhenCLRCalling,

		/// <summary>
		/// Allows to access .NET object, type, namespace, etc. directly.
		/// </summary>
		AllowDirectAccess = 0x1,

		/// <summary>
		/// Allows to auto-bind CLR event. This option works with AllowDirectAccess.
		/// </summary>
		AllowCLREventBind = 0x2,

		/// <summary>
		/// Allows import .NET namespaces and classes in script using 'import' keyword.
		/// </summary>
		AllowImportTypeInScript = 0x4,

		/// <summary>
		/// Ignore all exception in CLR invoking.
		/// </summary>
		IgnoreCLRExceptions = 0x8,

		/// <summary>
		/// Allows ReoScript to auto-import the relation types that may used in other imported type.
		/// </summary>
		AutoImportRelationType = 0x10,

		/// <summary>
		/// Allows to auto-convert the name into uppercase to find member or property during CLR calling.
		/// </summary>
		AutoUppercaseWhenCLRCalling = 0x20,
	}

	/// <summary>
	/// Specifies what features can be supported by ScriptRunningMachine.
	/// </summary>
	public enum CoreFeatures
	{
		/// <summary>
		/// A set of full features will be supported, both StandardFeatures and ExtendedFeatures.
		/// </summary>
		FullFeatures = StandardFeatures | ExtendedFeatures,

		/// <summary>
		/// A set of standard features will be supported. (Compatible with ECMAScript)
		/// Contains the alert, eval, setTimeout, setInterval and console object.
		/// </summary>
		StandardFeatures = Alert | Console | Eval | AsyncCalling | JSON,

		/// <summary>
		/// A set of featuers supported by default. (equals StandardFeatures)
		/// </summary>
		Default = StandardFeatures,

		/// <summary>
		/// Only minimum features will be supported
		/// </summary>
		None = 0x0,

		/// <summary>
		/// alert function support
		/// </summary>
		Alert = 0x1,

		/// <summary>
		/// eval function support
		/// </summary>
		Eval = 0x2,

		/// <summary>
		/// setTimeout and setInterval function support
		/// </summary>
		AsyncCalling = 0x4,

		/// <summary>
		/// console object support
		/// </summary>
		Console = 0x8,

		/// <summary>
		/// JSON object with 'parse' and 'stringify' method will be provided for script
		/// </summary>
		JSON = 0x10,

		/// <summary>
		/// Extended Feature supported by ReoScript (Non-compatible with ECMAScript)
		/// </summary>
		ExtendedFeatures = ArrayExtension,

		/// <summary>
		/// Array extension feature support
		/// </summary>
		ArrayExtension = 0x10000,
	}

	#endregion

	#region ReplacedCommonTree
	/// <summary>
	/// ReplacedCommonTree is used to replace a node of syntax tree in runtime.
	/// </summary>
	class ReplacedCommonTree : CommonTree
	{
		public object Object { get; set; }

		public ReplacedCommonTree(object obj)
			: base(new CommonToken(ReoScriptLexer.REPLACED_TREE))
		{
			this.Object = obj;
		}
	}
	class ConstValueNode : CommonTree
	{
		public object ConstValue { get; set; }

		public ConstValueNode(CommonTree t, object constValue, int tokenType)
			: base(t)
		{
			this.ConstValue = constValue;
			this.Token = new CommonToken(ReoScriptLexer.CONST_VALUE);
			this.TokenType = tokenType;
		}

		public int TokenType { get; set; }

		//public override int Type
		//{
		//  get { return ReoScriptLexer.CONST_VALUE; }
		//  set { base.Type = value; }
		//}

		//public override bool IsNil
		//{
		//  get { return false; }
		//}
	}
	#endregion

	#endregion

	#region Standard I/O Interface

	/// <summary>
	/// Interface to provide standard input data for script. 
	/// The method of implementation of this interface will be invoked when data is requested 
	/// to input from script by __stdout__ and __stdoutln__ built-in functions.
	/// </summary>
	public interface IStandardInputProvider
	{
		/// <summary>
		/// Read a byte from provider.
		/// </summary>
		/// <returns>byte be read</returns>
		byte Read();

		/// <summary>
		/// Read a whole line string from provider.
		/// </summary>
		/// <returns>string line</returns>
		string ReadLine();
	}

	/// <summary>
	/// An interface to listen what data has been outputed by __stdout__ and __stdoutln__
	/// built-in functions in script.
	/// </summary>
	public interface IStandardOutputListener
	{
		/// <summary>
		/// Write a byte array to listener.
		/// </summary>
		/// <param name="buf">buffer where the byte array is saved</param>
		/// <param name="index">index in buffer read from</param>
		/// <param name="count">count in buffer to read</param>
		void Write(byte[] buf, int index, int count);

		/// <summary>
		/// Write a line to listener.
		/// </summary>
		/// <param name="line">line to be output</param>
		void WriteLine(string line);

		/// <summary>
		/// write a object to listener.
		/// </summary>
		/// <param name="obj">object to be output</param>
		void Write(object obj);
	}

	/// <summary>
	/// Default built-in console input provider for Standard I/O Interface.
	/// </summary>
	public class BuiltinConsoleInputProvider : IStandardInputProvider
	{
		/// <summary>
		/// Read a byte from console.
		/// </summary>
		/// <returns></returns>
		public byte Read()
		{
			return (byte)Console.ReadKey().KeyChar;
		}

		/// <summary>
		/// Read a string line from console.
		/// </summary>
		/// <returns></returns>
		public string ReadLine()
		{
			return Console.ReadLine();
		}
	}

	/// <summary>
	/// Default built-in 
	/// </summary>
	public class BuiltinConsoleOutputListener : IStandardOutputListener
	{
		/// <summary>
		/// Print byte array in console.
		/// </summary>
		/// <param name="buf">buffer where the byte array saved</param>
		/// <param name="index">byte index from array</param>
		/// <param name="count">byte count in array</param>
		public void Write(byte[] buf, int index, int count)
		{
			Console.Write(Encoding.ASCII.GetString(buf, index, count));
		}

		/// <summary>
		/// Output string line passed from script.
		/// </summary>
		/// <param name="line">string line</param>
		public void WriteLine(string line)
		{
			Console.WriteLine(line);
		}

		/// <summary>
		/// Output serialized string of specified object from script.
		/// </summary>
		/// <param name="obj">the object to output</param>
		public void Write(object obj)
		{
			Console.Write(Convert.ToString(obj));
		}
	}
	#endregion

	#region Debug Monitor

	internal class ReoScriptObjectEventArgs : EventArgs
	{
		public object Object { get; set; }

		public AbstractFunctionObject Constructor { get; set; }

		public ReoScriptObjectEventArgs(object obj, AbstractFunctionObject constructor)
		{
			this.Object = obj;
			this.Constructor = constructor;
		}
	}

	namespace Diagnostics
	{
		/// <summary>
		/// Provides ability to debug script
		/// </summary>
		public sealed class ScriptDebugger
		{
			public static readonly string DEBUG_OBJECT_NAME = "debug";

			private ObjectValue debugObject;

			/// <summary>
			/// Debug object named 'debug' will be added into script context
			/// </summary>
			public ObjectValue DebugObject { get { return debugObject; } }

			/// <summary>
			/// Current SRM instance to be monitored
			/// </summary>
			public ScriptRunningMachine Srm { get; set; }

			/// <summary>
			/// Current context used to script executing
			/// </summary>
			public ScriptContext Context { get; set; }

			private int totalObjectCreated = 0;

			/// <summary>
			/// Total counter of object created in script
			/// </summary>
			public int TotalObjectCreated
			{
				get { return totalObjectCreated; }
				set { totalObjectCreated = value; }
			}

			ExprEqualsNodeParser comparer = new ExprEqualsNodeParser();

			/// <summary>
			/// Construct debugger to monitor specified script running machine 
			/// </summary>
			/// <param name="srm">instance of script running machine</param>
			public ScriptDebugger(ScriptRunningMachine srm)
			{
				this.Srm = srm;
				this.Context = new ScriptContext(srm, ScriptRunningMachine.entryFunction);

				srm.NewObjectCreated += new EventHandler<ReoScriptObjectEventArgs>(srm_NewObjectCreated);
				srm.Resetted += (s, e) => RestoreDebugger();

				debugObject = srm.CreateNewObject(Context) as ObjectValue;

				if (DebugObject != null)
				{
					DebugObject["assert"] = new NativeFunctionObject("assert", (ctx, owner, args) =>
					{
						if (args.Length == 0)
						{
							throw new ReoScriptAssertionException("assertion failed.");
						}
						else if (args.Length == 1)
						{
							if (!ScriptRunningMachine.GetBoolValue(args[0]))
							{
								throw new ReoScriptAssertionException("assertion failed.");
							}
						}
						else if (args.Length == 2)
						{
							if (!comparer.Compare(args[0], args[1], srm))
							{
								throw new ReoScriptAssertionException(string.Format("expect '{0}' but '{1}'",
									ScriptRunningMachine.ConvertToString(args[1]),
									ScriptRunningMachine.ConvertToString(args[0])));
							}
						}
						else if (args.Length == 3)
						{
							if (!comparer.Compare(args[0], args[1], srm))
							{
								throw new ReoScriptAssertionException(ScriptRunningMachine.ConvertToString(args[2]));
							}
						}
						return null;
					});

					DebugObject["total_created_objects"] = new ExternalProperty(() => totalObjectCreated, null);
				}

				RestoreDebugger();
			}

			/// <summary>
			/// Restore a debugger after monitoring SRM resetting
			/// </summary>
			public void RestoreDebugger()
			{
				if (DebugObject != null)
				{
					Srm[DEBUG_OBJECT_NAME] = DebugObject;
				}

				// debug script library
				using (MemoryStream ms = new MemoryStream(Resources.lib_debug))
				{
					Srm.Load(ms);
				}
			}

			void srm_NewObjectCreated(object sender, ReoScriptObjectEventArgs e)
			{
				totalObjectCreated++;
			}

			private List<Breakpoint> breakpoints = new List<Breakpoint>();

			/// <summary>
			/// Add a breakpoint at specified position. (NOT AVAILABLE YET!)
			/// </summary>
			/// <param name="breakpoint"></param>
			public void AddBreakpoint(Breakpoint breakpoint)
			{
				this.breakpoints.Add(breakpoint);
			}
		}

		/// <summary>
		/// Breakpoint to add at specified position.
		/// </summary>
		public class Breakpoint
		{
			public string FilePath { get; set; }
			public int Line { get; set; }
			public int Index { get; set; }
			public int Length { get; set; }
			public string TestCode { get; set; }
		}

	}

	#endregion

	#region Compiler
	#region NOT AVAILABLE YET !
	namespace Compiler
	{

		/// <summary>
		/// Compile ReoScript into .NET assembly. (NOT AVAILABLE YET!)
		/// </summary>
		public sealed class ReoScriptCompiler
		{
			public delegate object EntryFunction(ScriptContext ctx);

			public static object Run(string script)
			{
				EntryFunction fun = Compile(script);

				ScriptRunningMachine srm = new ScriptRunningMachine();
				ScriptContext ctx = new ScriptContext(srm, ScriptRunningMachine.entryFunction);

				Console.Write("Run... ");

				Stopwatch sw = Stopwatch.StartNew();

				object rs = fun(ctx);

				sw.Stop();
				Console.WriteLine("Done. {0} ms.", sw.ElapsedMilliseconds);

				return ScriptRunningMachine.UnboxAnything(rs);
			}

			public static EntryFunction Compile(string text)
			{
				return Compile(text, "rs");
			}

			public static EntryFunction Compile(string text, string filename)
			{
				ReoScriptLexer lex = new ReoScriptLexer(new ANTLRStringStream(text));
				CommonTokenStream tokens = new CommonTokenStream(lex);
				ReoScriptParser parser = new ReoScriptParser(tokens);

				Console.Write("Parsing... ");
				Stopwatch sw = Stopwatch.StartNew();
				CommonTree t = parser.script().Tree;
				sw.Stop();
				Console.WriteLine("Done. {0} ms.", sw.ElapsedMilliseconds);

				Console.Write("Compiling... ");
				sw.Reset();
				sw.Start();

				AssemblyName name = new AssemblyName(filename);
				AssemblyBuilder ab = AppDomain.CurrentDomain.DefineDynamicAssembly(name, AssemblyBuilderAccess.Save);
				ModuleBuilder mb = ab.DefineDynamicModule(name.Name, name.Name + ".dll");
				TypeBuilder tb = mb.DefineType("T1");

				ILGenerator il;

				CompilerContext cc = new CompilerContext(tb);

				// functions
				StaticFunctionScope slc = parser.CurrentStack;
				CompileFunction(cc, slc);

				MethodBuilder globalFun = tb.DefineMethod("__entry__", MethodAttributes.Static | MethodAttributes.Public,
					typeof(object), new Type[] { typeof(ScriptContext) });

				il = globalFun.GetILGenerator();
				DumpTree(cc, il, t);

				il.Emit(OpCodes.Ret);

				Type typ = tb.CreateType();

				EntryFunction fun = (EntryFunction)Delegate.CreateDelegate(typeof(EntryFunction), typ.GetMethod("__entry__"));

				ab.Save(name.Name + ".dll");

				sw.Stop();
				Console.WriteLine("Done. {0} ms.", sw.ElapsedMilliseconds);

				return fun;
			}

			private static void CompileFunction(CompilerContext cc, StaticFunctionScope sfs)
			{
				foreach (FunctionInfo fi in sfs.Functions)
				{
					ILGenerator il = cc.EnterFunction(fi);
					DumpTree(cc, il, fi.BodyTree);
					CompiledFunction cm = cc.ExitFunction();

					if (fi.InnerScope != null)
					{
						CompileFunction(cc, fi.InnerScope);
					}
				}
			}

			internal static void DumpTree(CompilerContext cc, ILGenerator il, CommonTree t)
			{
				switch (t.Type)
				{
					case ReoScriptLexer.DECLARATION:
						for (int i = 1; i < t.ChildCount; i++)
							DumpTree(cc, il, ((CommonTree)t.Children[i]));
						break;

					case ReoScriptLexer.LOCAL_DECLARE_ASSIGNMENT:
						VariableInfo vi = ((VariableDefineNode)t).VariableInfo;

						if (vi.HasInitialValue)
						{
							// get
							il.Emit(OpCodes.Ldarg_0); // ctx
							il.Emit(OpCodes.Ldstr, vi.Name); // id
							DumpTree(cc, il, vi.InitialValueTree); // value

							// set
							il.Emit(OpCodes.Callvirt, _Context_SetVariable); // set
						}
						break;

					case ReoScriptLexer.ASSIGNMENT:
						{
							switch (t.Children[0].Type)
							{
								case ReoScriptLexer.IDENTIFIER:
									{
										// get
										il.Emit(OpCodes.Ldarg_0); // ctx
										il.Emit(OpCodes.Ldstr, t.Children[0].Text); // id
										DumpTree(cc, il, (CommonTree)t.Children[1]); // value

										// set
										il.Emit(OpCodes.Callvirt, _Context_SetVariable); // set
									}
									break;

								case ReoScriptLexer.PROPERTY_ACCESS:
									{
										il.Emit(OpCodes.Ldarg_0);
										DumpTree(cc, il, (CommonTree)((CommonTree)t.Children[0]).Children[0]);
										il.Emit(OpCodes.Ldstr, ((CommonTree)((CommonTree)t.Children[0]).Children[1]).Text);
										DumpTree(cc, il, (CommonTree)t.Children[1]);
										il.Emit(OpCodes.Call, _SetProperty);
									}
									break;

								case ReoScriptLexer.ARRAY_ACCESS:
									break;

								default:
									throw CreateCompilingException(t, "only variable, index and property can be used in left-hand.");
							}
						}
						break;

					#region For
					case ReoScriptLexer.FOR_STATEMENT:
						{
							CommonTree forInit = (CommonTree)t.Children[0];

							for (int i = 0; i < forInit.ChildCount; i++)
							{
								DumpTree(cc, il, (CommonTree)forInit.Children[i]);
								il.Emit(OpCodes.Pop);
							}

							System.Reflection.Emit.Label start = il.DefineLabel();
							System.Reflection.Emit.Label end = il.DefineLabel();

							il.MarkLabel(start);

							// condition
							CommonTree condition = ((CommonTree)t.Children[1]);
							DumpTree(cc, il, (CommonTree)(condition.Children[0]));
							il.Emit(OpCodes.Unbox_Any, typeof(bool));
							il.Emit(OpCodes.Brfalse_S, end);

							CommonTree body = (CommonTree)(((CommonTree)t.Children[3]).Children[0]);

							// body
							for (int i = 0; i < body.ChildCount; i++)
							{
								DumpTree(cc, il, (CommonTree)body.Children[i]);
								il.Emit(OpCodes.Pop);
							}

							// iterators
							CommonTree iterators = ((CommonTree)t.Children[2]);
							for (int i = 0; i < iterators.ChildCount; i++)
							{
								DumpTree(cc, il, (CommonTree)iterators.Children[i]);
								il.Emit(OpCodes.Pop);
							}

							// loop
							il.Emit(OpCodes.Br_S, start);
							il.MarkLabel(end);

							il.Emit(OpCodes.Ldnull);
						}
						break;
					#endregion

					#region Primary Expressions
					#region Const
					case ReoScriptLexer.CONST_VALUE:
						{
							switch (t.Children[0].Type)
							{
								case ReoScriptLexer.CONST_VALUE:
									{
										ConstValueNode constNode = ((ConstValueNode)t.Children[0]);
										switch (constNode.TokenType)
										{
											case ReoScriptLexer.NUMBER_LITERATE:
												il.Emit(OpCodes.Ldc_R8, (double)constNode.ConstValue);
												il.Emit(OpCodes.Box, typeof(double));
												break;

											case ReoScriptLexer.STRING_LITERATE:
												il.Emit(OpCodes.Ldstr, (string)constNode.ConstValue);
												break;
										}
									}
									break;

								case ReoScriptLexer.LIT_TRUE:
									il.Emit(OpCodes.Ldc_I4_S, 1);
									il.Emit(OpCodes.Box, typeof(bool));
									break;

								case ReoScriptLexer.LIT_FALSE:
									il.Emit(OpCodes.Ldc_I4_S, 0);
									il.Emit(OpCodes.Box, typeof(bool));
									break;

								case ReoScriptLexer.LIT_NULL:
									il.Emit(OpCodes.Ldnull);
									break;



								case ReoScriptLexer.ARRAY_LITERAL:
								case ReoScriptLexer.OBJECT_LITERAL:
									break;
							}
						}
						break;

					case ReoScriptLexer.THIS:
						il.Emit(OpCodes.Ldarg_0);
						il.Emit(OpCodes.Call, _GetThisObject);
						break;
					#endregion

					case ReoScriptLexer.IDENTIFIER:
						GetVariable(il, t);
						break;

					case ReoScriptLexer.OBJECT_LITERAL:
						if (t.ChildCount % 2 != 0)
						{
							throw CreateCompilingException(t, "object literal must be a key/value paired list.");
						}

						il.Emit(OpCodes.Ldarg_0);
						il.Emit(OpCodes.Callvirt, _CreateNewObject);

						for (int i = 0; i < t.ChildCount; i += 2)
						{
							il.Emit(OpCodes.Dup); // obj
							il.Emit(OpCodes.Ldstr, t.Children[i].Text); // key
							DumpTree(cc, il, (CommonTree)t.Children[i + 1]); // value
							il.Emit(OpCodes.Callvirt, _ObjectValue_SetItem);
						}
						break;

					case ReoScriptLexer.ARRAY_LITERAL:
						il.Emit(OpCodes.Ldarg_0);
						il.Emit(OpCodes.Callvirt, _CreateNewArray);
						break;

					case ReoScriptLexer.ANONYMOUS_FUNCTION:
						FunctionInfo fi = ((AnonymousFunctionDefineNode)t).FunctionInfo;
						MethodBuilder mb = DumpFunctionTree(cc, fi).MethodBuilder;
						il.Emit(OpCodes.Ldftn, mb);
						il.Emit(OpCodes.Box, typeof(IntPtr));
						break;

					#endregion

					case ReoScriptLexer.PROPERTY_ACCESS:
						il.Emit(OpCodes.Ldarg_0); // ctx
						DumpTree(cc, il, (CommonTree)t.Children[0]);
						il.Emit(OpCodes.Ldstr, t.Children[1].Text);
						il.Emit(OpCodes.Call, _GetProperty);
						break;

					case ReoScriptLexer.PLUS:
						GenerateTreeCall(cc, il, t, _Plus);
						break;

					case ReoScriptLexer.LESS_THAN:
						GenerateTreeCall(cc, il, t, _LessThan);
						il.Emit(OpCodes.Box, typeof(bool));
						break;

					case ReoScriptLexer.LESS_EQUALS:
						GenerateTreeCall(cc, il, t, _LessEquals);
						il.Emit(OpCodes.Box, typeof(bool));
						break;

					case ReoScriptLexer.POST_UNARY_STEP:
						// prepare to set
						il.Emit(OpCodes.Ldarg_0);
						il.Emit(OpCodes.Ldstr, t.Children[0].Text);

						// get
						il.Emit(OpCodes.Ldarg_0);
						il.Emit(OpCodes.Ldstr, t.Children[0].Text);
						il.Emit(OpCodes.Callvirt, _Context_GetVariable);

						// add
						UnboxDouble(il);
						il.Emit(OpCodes.Ldc_I4, 1);
						il.Emit(OpCodes.Add);
						il.Emit(OpCodes.Box, typeof(double));

						// set
						il.Emit(OpCodes.Callvirt, _Context_SetVariable);
						break;

					case ReoScriptLexer.FUNCTION_CALL:
						// local call
						if (t.Children[0].Type == ReoScriptLexer.IDENTIFIER)
						{
							bool hasArgs = t.ChildCount > 1 && t.Children[1].ChildCount > 0;
							LocalBuilder args = null;

							if (hasArgs)
							{
								args = il.DeclareLocal(typeof(object[]));

								// get fun ref
								MakeCallArguments(cc, il, (CommonTree)t.Children[1]);
								il.Emit(OpCodes.Stloc, args);
							}

							System.Reflection.Emit.Label expCall = il.DefineLabel();
							System.Reflection.Emit.Label end = il.DefineLabel();

							il.Emit(OpCodes.Ldarg_0); // ctx

							if (hasArgs)
								il.Emit(OpCodes.Ldloc, args);
							else
								il.Emit(OpCodes.Ldnull); // args

							GetVariable(il, (CommonTree)t.Children[0]);

							// check 
							il.Emit(OpCodes.Dup);
							il.Emit(OpCodes.Isinst, typeof(IntPtr));
							il.Emit(OpCodes.Brfalse_S, expCall);

							// true:
							il.Emit(OpCodes.Unbox_Any, typeof(IntPtr));
							// call						
							il.EmitCalli(OpCodes.Calli, CallingConventions.Standard,
								typeof(object), new Type[] { typeof(ScriptContext), typeof(object[]) }, null);
							il.Emit(OpCodes.Br_S, end);

							// false
							il.MarkLabel(expCall);

							il.Emit(OpCodes.Pop); // fd
							il.Emit(OpCodes.Pop); // args
							il.Emit(OpCodes.Pop); // ctx
							il.Emit(OpCodes.Ldnull);

							//il.Emit(OpCodes.Ldarg_0); // ctx

							//// this
							//il.Emit(OpCodes.Ldarg_0);
							//il.Emit(OpCodes.Call, _GetThisObject);

							//GetVariable(il, (CommonTree)t.Children[0]);

							//il.Emit(OpCodes.Call, _CallFunction);
							il.MarkLabel(end);
						}
						else if (t.Children[0].Type == ReoScriptLexer.PROPERTY_ACCESS)
						{
							il.Emit(OpCodes.Ldnull);
						}
						else
						{
							throw CreateCompilingException(t, "invalid function call expression.");
						}
						break;

					case ReoScriptLexer.RETURN:
						DumpTree(cc, il, (CommonTree)t.Children[0]);
						il.Emit(OpCodes.Ret);
						break;

					default:
						if (t.ChildCount > 0)
						{
							for (int i = 0; i < t.ChildCount; i++)
							{
								if (t.Children[i].ChildCount > 0)
								{
									DumpTree(cc, il, (CommonTree)t.Children[i]);
									if (i < t.ChildCount - 1) il.Emit(OpCodes.Pop); // pop if not last
								}
							}
						}
						else
						{
							il.Emit(OpCodes.Ldnull);
						}

						break;
				}
			}

			private static void UnboxDouble(ILGenerator il)
			{
				System.Reflection.Emit.Label cvt = il.DefineLabel();
				System.Reflection.Emit.Label ret = il.DefineLabel();

				il.Emit(OpCodes.Dup);

				il.Emit(OpCodes.Isinst, typeof(double));
				il.Emit(OpCodes.Brfalse_S, cvt);

				//true:
				il.Emit(OpCodes.Unbox_Any, typeof(double));
				il.Emit(OpCodes.Br_S, ret);

				//false:
				il.MarkLabel(cvt);

				il.Emit(OpCodes.Call, _GetDoubleValue);

				//ret:
				il.MarkLabel(ret);
			}

			private static void GenerateTreeCall(CompilerContext cc, ILGenerator il, CommonTree t, MethodInfo mi)
			{
				il.Emit(OpCodes.Ldarg_0);
				DumpTree(cc, il, (CommonTree)t.Children[0]); // left
				DumpTree(cc, il, (CommonTree)t.Children[1]); // right
				il.Emit(OpCodes.Call, mi);
			}

			private static void GetVariable(ILGenerator il, CommonTree t)
			{
				il.Emit(OpCodes.Ldarg_0);
				il.Emit(OpCodes.Ldstr, t.Text);
				il.Emit(OpCodes.Callvirt, _Context_GetVariable);
			}

			private static void SetVariable(CompilerContext cc, ILGenerator il, CommonTree t1, CommonTree t2)
			{
				il.Emit(OpCodes.Ldarg_0);
				il.Emit(OpCodes.Ldstr, t1.Text);
				DumpTree(cc, il, t2);
				il.Emit(OpCodes.Callvirt, _Context_SetVariable);
			}

			private static CompiledFunction DumpFunctionTree(CompilerContext cc, FunctionInfo fi)
			{
				ILGenerator il = cc.EnterFunction(fi);
				DumpTree(cc, il, fi.BodyTree);
				return cc.ExitFunction();
			}

			private static void MakeCallArguments(CompilerContext cc, ILGenerator il, CommonTree argTree)
			{
				il.Emit(OpCodes.Ldc_I4, argTree.ChildCount);
				il.Emit(OpCodes.Newarr, typeof(object));

				for (int i = 0; i < argTree.ChildCount; i++)
				{
					il.Emit(OpCodes.Dup);
					il.Emit(OpCodes.Ldc_I4, i);
					DumpTree(cc, il, (CommonTree)argTree.Children[i]);
					il.Emit(OpCodes.Stelem_Ref);
				}
			}

			private static ReoScriptCompilingException CreateCompilingException(CommonTree t, string msg)
			{
				return new ReoScriptCompilingException(new ErrorObject(msg)
				{
					CharIndex = t.CharPositionInLine,
					Line = t.Line,
				});
			}

			#region Runtime Methods
			//private static MethodInfo _SetVariable = typeof(ReoScriptCompiler).GetMethod("SetVariable");
			//public static object SetVariable(ScriptContext ctx, string identifier, object value)
			//{
			//  ctx[identifier] = value;
			//  return value;
			//}

			//private static MethodInfo _GetVariable = typeof(ReoScriptCompiler).GetMethod("GetVariable");
			//public static object GetVariable(ScriptContext ctx, string identifier)
			//{
			//  return ctx[identifier];
			//}

			private static MethodInfo _GetProperty = typeof(ReoScriptCompiler).GetMethod("GetProperty");

			private static MethodInfo _SetProperty = typeof(ReoScriptCompiler).GetMethod("SetProperty");


			//private static MethodInfo _GetThisObject = typeof(RSRuntimeInterface).GetMethod("GetThisObject");
			private static MethodInfo _GetThisObject = typeof(ScriptContext).GetMethod("get_ThisObject");

			private static MethodInfo _CreateNewObject = typeof(ScriptContext).GetMethod("CreateNewObject", Type.EmptyTypes);
			private static MethodInfo _CreateNewArray = typeof(ScriptContext).GetMethod("CreateNewArray", Type.EmptyTypes);
			private static MethodInfo _ObjectValue_SetItem = typeof(ObjectValue).GetMethod("set_Item", new Type[] { typeof(string), typeof(object) });

			private static MethodInfo _Context_SetVariable = typeof(ScriptContext).GetMethod("SetVariable", new Type[] { typeof(string), typeof(object) });
			private static MethodInfo _Context_GetVariable = typeof(ScriptContext).GetMethod("GetVariable", new Type[] { typeof(string) });

			//private static MethodInfo _CreateFunction = typeof(FunctionDefineNodeParser).GetMethod("CreateAndInitFunction");

			private static MethodInfo _CallFunction = typeof(RSRuntimeInterface).GetMethod("CallFunction");

			private static readonly MethodInfo _Plus = typeof(RSRuntimeInterface).GetMethod("Plus");
			private static readonly MethodInfo _LessThan = typeof(RSRuntimeInterface).GetMethod("LessThan");
			private static readonly MethodInfo _LessEquals = typeof(RSRuntimeInterface).GetMethod("LessEquals");

			private static readonly MethodInfo _GetDoubleValue = typeof(ScriptRunningMachine).GetMethod("GetDoubleValue", new Type[] { typeof(object) });
			//private static readonly MethodInfo _IsNumber = typeof(ScriptRunningMachine).GetMethod("IsNumber");
			//private static readonly MethodInfo _DoubleTryParse = typeof(System.Double).GetMethod("TryParse");
			//private static readonly MethodInfo _ConvertToDouble = typeof(Convert).GetMethod("ToDouble", new Type[] { typeof(object) });

			#endregion
		}

		internal class CompilerContext
		{
			internal Dictionary<FunctionInfo, CompiledFunction> CompiledMethods { get; set; }
			internal Stack<FunctionInfo> FunctionScopeStack { get; set; }

			public CompilerContext(TypeBuilder typeBuilder)
			{
				TypeBuilder = typeBuilder;
				CompiledMethods = new Dictionary<FunctionInfo, CompiledFunction>();
				FunctionScopeStack = new Stack<FunctionInfo>();
			}

			public TypeBuilder TypeBuilder { get; set; }
			private MethodBuilder CurrentMethod { get; set; }

			public ILGenerator EnterFunction(FunctionInfo fi)
			{
				CompiledFunction cm = new CompiledFunction { FunctionInfo = fi };

				cm.FunId = CompiledMethods.Count;
				string funId = cm.FunId.ToString("000000");

				string prefix = DumpFunctionStackPath();

				if (fi.IsAnonymous)
				{
					CurrentMethod = TypeBuilder.DefineMethod(prefix + string.Format("__af{0}", funId),
						MethodAttributes.Private | MethodAttributes.Static, typeof(object),
						new Type[] { typeof(ScriptContext), typeof(object[]) });

					//cm.FieldBuilder = TypeBuilder.DefineField(string.Format("__cf{0}", funId), typeof(Func<ScriptContext, object[]>),
					//  FieldAttributes.Static | FieldAttributes.Private);
				}
				else
				{
					CurrentMethod = TypeBuilder.DefineMethod(prefix + fi.Name,
						MethodAttributes.Private | MethodAttributes.Static, typeof(object),
						new Type[] { typeof(ScriptContext), typeof(object[]) });
				}

				cm.ArgNameField = TypeBuilder.DefineField(
					fi.IsAnonymous ? ("__ma" + cm.FunId.ToString("000000"))
					: (prefix + fi.Name),
					typeof(string[]), FieldAttributes.Static | FieldAttributes.Private | FieldAttributes.InitOnly);

				cm.MethodBuilder = CurrentMethod;

				FunctionScopeStack.Push(fi);
				CompiledMethods[fi] = cm;

				return CurrentMethod.GetILGenerator();
			}

			private string DumpFunctionStackPath()
			{
				StringBuilder sb = new StringBuilder();
				foreach (FunctionInfo fi in FunctionScopeStack)
				{
					sb.Append(fi.Name);
					sb.Append("$");
				}
				return sb.ToString();
			}

			public CompiledFunction ExitFunction()
			{
				FunctionInfo fi = FunctionScopeStack.Pop();
				CurrentMethod.GetILGenerator().Emit(OpCodes.Ret);

				CompiledFunction cm = new CompiledFunction { FunctionInfo = fi, MethodBuilder = CurrentMethod };
				CompiledMethods[fi] = cm;
				return cm;
			}
		}

		internal class CompiledFunction
		{
			internal FunctionInfo FunctionInfo { get; set; }
			internal int FunId { get; set; }
			internal MethodBuilder MethodBuilder { get; set; }
			internal FieldBuilder CachedFunctionFieldBuilder { get; set; }
			internal FieldBuilder ArgNameField { get; set; }
		}
	}

	namespace Runtime
	{
		public static class RSRuntimeInterface
		{
			public static object GetProperty(ScriptContext ctx, object owner, string identifier)
			{
				return PropertyAccessHelper.GetProperty(ctx, owner, identifier);
			}
			public static object SetProperty(ScriptContext ctx, object owner, string identifier, object value)
			{
				PropertyAccessHelper.SetProperty(ctx, owner, identifier, value);
				return value;
			}
			public static object GetThisObject(ScriptContext ctx)
			{
				return ctx.ThisObject;
			}
			public static object Plus(ScriptContext ctx, object left, object right)
			{
				return ((ExprPlusNodeParser)AWDLLogicSyntaxParserAdapter.definedParser[ReoScriptLexer.PLUS]).Calc(left, right, ctx.Srm, ctx);
			}
			public static bool LessThan(ScriptContext ctx, object left, object right)
			{
				return ((ExprLessThanNodeParser)AWDLLogicSyntaxParserAdapter.definedParser[ReoScriptLexer.LESS_THAN]).Compare(left, right, ctx.Srm);
			}
			public static bool LessEquals(ScriptContext ctx, object left, object right)
			{
				return ((ExprLessOrEqualsNodeParser)AWDLLogicSyntaxParserAdapter.definedParser[ReoScriptLexer.LESS_EQUALS]).Compare(left, right, ctx.Srm);
			}
			public static object CallFunction(ScriptContext ctx, object owner, object funObj, object[] args)
			{
				AbstractFunctionObject fun = funObj as AbstractFunctionObject;

				if (fun == null)
				{
					// FIXME: throw an error info object
					throw new ReoScriptRuntimeException("Object is not of function.");
				}

				if (owner == null) owner = ctx.ThisObject;

				return ctx.Srm.InvokeAbstractFunction(owner, fun, args);
			}
		}

		public class RuntimeContext
		{

		}
	}

	#endregion
	#endregion
}
