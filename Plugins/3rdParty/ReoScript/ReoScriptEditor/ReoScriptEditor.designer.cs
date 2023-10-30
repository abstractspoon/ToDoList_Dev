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

namespace unvell.ReoScript.Editor
{
	partial class ReoScriptEditor
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ReoScriptEditor));
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.cutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.pasteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.scriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.runToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.stopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
			this.checkSyntaxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableDirectAccessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableImportNamespacesAndClassesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableAutoImportDependencyTypeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.enableEventBindingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.splitter1 = new System.Windows.Forms.Splitter();
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.toolStrip1 = new IIControls.ToolStripEx();
			this.newToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.openToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.saveToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.cutToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.copyToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.pasteToolStripDropDownButton = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.undoToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.redoToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			this.checkSyntaxStripButton = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
			this.runToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.stopToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.editor = new unvell.ReoScript.Editor.ReoScriptEditorControl();
			this.console = new unvell.ReoScript.Editor.FctbConsoleControl();
			this.menuStrip1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.scriptToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(747, 24);
			this.menuStrip1.TabIndex = 2;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "&File";
			// 
			// newToolStripMenuItem
			// 
			this.newToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.NewDocumentHS;
			this.newToolStripMenuItem.Name = "newToolStripMenuItem";
			this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
			this.newToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
			this.newToolStripMenuItem.Text = "&New";
			// 
			// openToolStripMenuItem
			// 
			this.openToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.openHS;
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
			this.openToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
			this.openToolStripMenuItem.Text = "&Open...";
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.saveHS1;
			this.saveToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
			this.saveToolStripMenuItem.Text = "&Save";
			// 
			// saveAsToolStripMenuItem
			// 
			this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
			this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
			this.saveAsToolStripMenuItem.Text = "Save &As...";
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(152, 6);
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
			this.exitToolStripMenuItem.Text = "Exit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
			// 
			// editToolStripMenuItem
			// 
			this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem,
            this.toolStripMenuItem2,
            this.cutToolStripMenuItem,
            this.copyToolStripMenuItem,
            this.pasteToolStripMenuItem});
			this.editToolStripMenuItem.Name = "editToolStripMenuItem";
			this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
			this.editToolStripMenuItem.Text = "&Edit";
			// 
			// undoToolStripMenuItem
			// 
			this.undoToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.Edit_UndoHS;
			this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
			this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
			this.undoToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
			this.undoToolStripMenuItem.Text = "&Undo";
			// 
			// redoToolStripMenuItem
			// 
			this.redoToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.Edit_RedoHS;
			this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
			this.redoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
			this.redoToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
			this.redoToolStripMenuItem.Text = "&Redo";
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(141, 6);
			// 
			// cutToolStripMenuItem
			// 
			this.cutToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.CutHS1;
			this.cutToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.cutToolStripMenuItem.Name = "cutToolStripMenuItem";
			this.cutToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
			this.cutToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
			this.cutToolStripMenuItem.Text = "C&ut";
			// 
			// copyToolStripMenuItem
			// 
			this.copyToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.CopyHS1;
			this.copyToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
			this.copyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
			this.copyToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
			this.copyToolStripMenuItem.Text = "&Copy";
			// 
			// pasteToolStripMenuItem
			// 
			this.pasteToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.PasteHS1;
			this.pasteToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.pasteToolStripMenuItem.Name = "pasteToolStripMenuItem";
			this.pasteToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
			this.pasteToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
			this.pasteToolStripMenuItem.Text = "&Paste";
			// 
			// scriptToolStripMenuItem
			// 
			this.scriptToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.runToolStripMenuItem,
            this.stopToolStripMenuItem,
            this.toolStripMenuItem4,
            this.checkSyntaxToolStripMenuItem});
			this.scriptToolStripMenuItem.Name = "scriptToolStripMenuItem";
			this.scriptToolStripMenuItem.Size = new System.Drawing.Size(49, 20);
			this.scriptToolStripMenuItem.Text = "&Script";
			// 
			// runToolStripMenuItem
			// 
			this.runToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.PlayHS;
			this.runToolStripMenuItem.Name = "runToolStripMenuItem";
			this.runToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
			this.runToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
			this.runToolStripMenuItem.Text = "&Run";
			// 
			// stopToolStripMenuItem
			// 
			this.stopToolStripMenuItem.Enabled = false;
			this.stopToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.StopHS;
			this.stopToolStripMenuItem.Name = "stopToolStripMenuItem";
			this.stopToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
			this.stopToolStripMenuItem.Text = "&Stop";
			// 
			// toolStripMenuItem4
			// 
			this.toolStripMenuItem4.Name = "toolStripMenuItem4";
			this.toolStripMenuItem4.Size = new System.Drawing.Size(161, 6);
			// 
			// checkSyntaxToolStripMenuItem
			// 
			this.checkSyntaxToolStripMenuItem.Image = global::unvell.ReoScript.Editor.Properties.Resources.CheckGrammarHS;
			this.checkSyntaxToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.checkSyntaxToolStripMenuItem.Name = "checkSyntaxToolStripMenuItem";
			this.checkSyntaxToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F4;
			this.checkSyntaxToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
			this.checkSyntaxToolStripMenuItem.Text = "Check Syntax";
			// 
			// toolsToolStripMenuItem
			// 
			this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.enableDirectAccessToolStripMenuItem,
            this.enableImportNamespacesAndClassesToolStripMenuItem,
            this.enableAutoImportDependencyTypeToolStripMenuItem,
            this.enableEventBindingToolStripMenuItem});
			this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
			this.toolsToolStripMenuItem.Size = new System.Drawing.Size(46, 20);
			this.toolsToolStripMenuItem.Text = "&Tools";
			// 
			// enableDirectAccessToolStripMenuItem
			// 
			this.enableDirectAccessToolStripMenuItem.CheckOnClick = true;
			this.enableDirectAccessToolStripMenuItem.Name = "enableDirectAccessToolStripMenuItem";
			this.enableDirectAccessToolStripMenuItem.Size = new System.Drawing.Size(282, 22);
			this.enableDirectAccessToolStripMenuItem.Text = "Enable DirectAccess";
			// 
			// enableImportNamespacesAndClassesToolStripMenuItem
			// 
			this.enableImportNamespacesAndClassesToolStripMenuItem.CheckOnClick = true;
			this.enableImportNamespacesAndClassesToolStripMenuItem.Name = "enableImportNamespacesAndClassesToolStripMenuItem";
			this.enableImportNamespacesAndClassesToolStripMenuItem.Size = new System.Drawing.Size(282, 22);
			this.enableImportNamespacesAndClassesToolStripMenuItem.Text = "Enable Import Namespaces and Classes";
			// 
			// enableAutoImportDependencyTypeToolStripMenuItem
			// 
			this.enableAutoImportDependencyTypeToolStripMenuItem.CheckOnClick = true;
			this.enableAutoImportDependencyTypeToolStripMenuItem.Name = "enableAutoImportDependencyTypeToolStripMenuItem";
			this.enableAutoImportDependencyTypeToolStripMenuItem.Size = new System.Drawing.Size(282, 22);
			this.enableAutoImportDependencyTypeToolStripMenuItem.Text = "Enable Auto Import Dependency Type";
			// 
			// enableEventBindingToolStripMenuItem
			// 
			this.enableEventBindingToolStripMenuItem.CheckOnClick = true;
			this.enableEventBindingToolStripMenuItem.Name = "enableEventBindingToolStripMenuItem";
			this.enableEventBindingToolStripMenuItem.Size = new System.Drawing.Size(282, 22);
			this.enableEventBindingToolStripMenuItem.Text = "Enable Event Binding";
			// 
			// helpToolStripMenuItem
			// 
			this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
			this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
			this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
			this.helpToolStripMenuItem.Text = "&Help";
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
			this.aboutToolStripMenuItem.Text = "&About...";
			this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
			// 
			// splitter1
			// 
			this.splitter1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.splitter1.Location = new System.Drawing.Point(0, 250);
			this.splitter1.Name = "splitter1";
			this.splitter1.Size = new System.Drawing.Size(747, 3);
			this.splitter1.TabIndex = 5;
			this.splitter1.TabStop = false;
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
			// 
			// toolStrip1
			// 
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripButton,
            this.openToolStripButton,
            this.saveToolStripButton,
            this.toolStripSeparator1,
            this.cutToolStripButton,
            this.copyToolStripButton,
            this.pasteToolStripDropDownButton,
            this.toolStripSeparator2,
            this.undoToolStripButton,
            this.redoToolStripButton,
            this.toolStripSeparator3,
            this.checkSyntaxStripButton,
            this.toolStripSeparator4,
            this.runToolStripButton,
            this.stopToolStripButton});
			this.toolStrip1.Location = new System.Drawing.Point(0, 24);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
			this.toolStrip1.ShowItemToolTips = false;
			this.toolStrip1.Size = new System.Drawing.Size(747, 31);
			this.toolStrip1.TabIndex = 8;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// newToolStripButton
			// 
			this.newToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.newToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.NewDocumentHS;
			this.newToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.newToolStripButton.Name = "newToolStripButton";
			this.newToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.newToolStripButton.Text = "New Script";
			// 
			// openToolStripButton
			// 
			this.openToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.openToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.openHS;
			this.openToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.openToolStripButton.Name = "openToolStripButton";
			this.openToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.openToolStripButton.Text = "Open Script";
			// 
			// saveToolStripButton
			// 
			this.saveToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.saveToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.saveHS1;
			this.saveToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.saveToolStripButton.Name = "saveToolStripButton";
			this.saveToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.saveToolStripButton.Text = "Save Script";
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 31);
			// 
			// cutToolStripButton
			// 
			this.cutToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.cutToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.CutHS1;
			this.cutToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.cutToolStripButton.Name = "cutToolStripButton";
			this.cutToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.cutToolStripButton.Text = "Cut";
			// 
			// copyToolStripButton
			// 
			this.copyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.copyToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.CopyHS1;
			this.copyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.copyToolStripButton.Name = "copyToolStripButton";
			this.copyToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.copyToolStripButton.Text = "Copy";
			// 
			// pasteToolStripDropDownButton
			// 
			this.pasteToolStripDropDownButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.pasteToolStripDropDownButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.PasteHS1;
			this.pasteToolStripDropDownButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.pasteToolStripDropDownButton.Name = "pasteToolStripDropDownButton";
			this.pasteToolStripDropDownButton.Size = new System.Drawing.Size(28, 28);
			this.pasteToolStripDropDownButton.Text = "Paste";
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 31);
			// 
			// undoToolStripButton
			// 
			this.undoToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.undoToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.Edit_UndoHS;
			this.undoToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.undoToolStripButton.Name = "undoToolStripButton";
			this.undoToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.undoToolStripButton.Text = "Undo";
			// 
			// redoToolStripButton
			// 
			this.redoToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.redoToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.Edit_RedoHS;
			this.redoToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.redoToolStripButton.Name = "redoToolStripButton";
			this.redoToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.redoToolStripButton.Text = "Redo";
			// 
			// toolStripSeparator3
			// 
			this.toolStripSeparator3.Name = "toolStripSeparator3";
			this.toolStripSeparator3.Size = new System.Drawing.Size(6, 31);
			// 
			// checkSyntaxStripButton
			// 
			this.checkSyntaxStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.checkSyntaxStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.CheckGrammarHS;
			this.checkSyntaxStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.checkSyntaxStripButton.Name = "checkSyntaxStripButton";
			this.checkSyntaxStripButton.Size = new System.Drawing.Size(28, 28);
			this.checkSyntaxStripButton.Text = "Check syntax errors by compiling script (F4)";
			// 
			// toolStripSeparator4
			// 
			this.toolStripSeparator4.Name = "toolStripSeparator4";
			this.toolStripSeparator4.Size = new System.Drawing.Size(6, 31);
			// 
			// runToolStripButton
			// 
			this.runToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.runToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.PlayHS;
			this.runToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.runToolStripButton.Name = "runToolStripButton";
			this.runToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.runToolStripButton.Text = "Run (F5)";
			// 
			// stopToolStripButton
			// 
			this.stopToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.stopToolStripButton.Enabled = false;
			this.stopToolStripButton.Image = global::unvell.ReoScript.Editor.Properties.Resources.StopHS;
			this.stopToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.stopToolStripButton.Name = "stopToolStripButton";
			this.stopToolStripButton.Size = new System.Drawing.Size(28, 28);
			this.stopToolStripButton.Text = "Stop";
			// 
			// editor
			// 
			this.editor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.editor.Location = new System.Drawing.Point(0, 55);
			this.editor.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
			this.editor.Name = "editor";
			this.editor.Padding = new System.Windows.Forms.Padding(3, 0, 3, 3);
			this.editor.Size = new System.Drawing.Size(747, 195);
			this.editor.TabIndex = 6;
			// 
			// console
			// 
			this.console.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.console.Location = new System.Drawing.Point(0, 253);
			this.console.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
			this.console.Name = "console";
			this.console.Padding = new System.Windows.Forms.Padding(3, 0, 3, 3);
			this.console.Size = new System.Drawing.Size(747, 153);
			this.console.TabIndex = 9;
			// 
			// ReoScriptEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(747, 406);
			this.Controls.Add(this.editor);
			this.Controls.Add(this.splitter1);
			this.Controls.Add(this.toolStrip1);
			this.Controls.Add(this.menuStrip1);
			this.Controls.Add(this.console);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "ReoScriptEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "ScriptEditor";
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem scriptToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem runToolStripMenuItem;
		private System.Windows.Forms.Splitter splitter1;
		private unvell.ReoScript.Editor.ReoScriptEditorControl editor;
		private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripButton newToolStripButton;
		private System.Windows.Forms.ToolStripButton openToolStripButton;
		private System.Windows.Forms.ToolStripButton saveToolStripButton;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton cutToolStripButton;
		private System.Windows.Forms.ToolStripButton copyToolStripButton;
		private System.Windows.Forms.ToolStripButton pasteToolStripDropDownButton;
		private System.Windows.Forms.ToolStripButton undoToolStripButton;
		private System.Windows.Forms.ToolStripButton redoToolStripButton;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStripButton runToolStripButton;
		private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem cutToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem pasteToolStripMenuItem;
		private FctbConsoleControl console;
		private System.Windows.Forms.ToolStripButton stopToolStripButton;
		private System.Windows.Forms.ToolStripMenuItem stopToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem enableDirectAccessToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem enableImportNamespacesAndClassesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem enableAutoImportDependencyTypeToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem enableEventBindingToolStripMenuItem;
		private System.Windows.Forms.ToolStripButton checkSyntaxStripButton;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
		private System.Windows.Forms.ToolStripMenuItem checkSyntaxToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
		private IIControls.ToolStripEx toolStrip1;
	}
}