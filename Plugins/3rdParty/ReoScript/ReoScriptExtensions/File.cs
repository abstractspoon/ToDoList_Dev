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

using System.IO;

namespace unvell.ReoScript.Extensions
{
	public class FileObject
	{
		public FileInfo FileInfo { get; set; }
	}

	public class DirectoryObject
	{
		public DirectoryInfo DirInfo { get; set; }
	}

	public class FileConstructorFunction : TypedNativeFunctionObject<DirectoryObject>
	{

	}

	public class DirectoryConstructorFunction : TypedNativeFunctionObject<DirectoryObject>
	{

	}

	[ModuleLoader]
	public class FileModuleLoader : IModuleLoader
	{
		public void LoadModule(ScriptRunningMachine srm)
		{
			srm.ImportType(typeof(FileConstructorFunction), "File");
			srm.ImportType(typeof(DirectoryConstructorFunction), "Directory");
		}

		public void UnloadModule(ScriptRunningMachine srm)
		{
		}
	}


}
