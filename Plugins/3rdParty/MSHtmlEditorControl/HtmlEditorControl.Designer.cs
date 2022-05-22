namespace MSDN.Html.Editor
{
    partial class HtmlEditorControl
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HtmlEditorControl));
			this.browserPanel = new System.Windows.Forms.Panel();
			this.editorWebBrowser = new WebBrowserEx.WebBrowserEx();
			this.toolstripEditor = new IIControls.ToolStripEx();
			this.toolstripEnableEditing = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator17 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripTextCut = new System.Windows.Forms.ToolStripButton();
			this.toolstripTextCopy = new System.Windows.Forms.ToolStripButton();
			this.toolstripTextPaste = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripEditUndo = new System.Windows.Forms.ToolStripButton();
			this.toolstripEditRedo = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripFontComboBox = new System.Windows.Forms.ToolStripComboBox();
			this.toolStripSeparator26 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripFormatBold = new System.Windows.Forms.ToolStripButton();
			this.toolstripFormatUnderline = new System.Windows.Forms.ToolStripButton();
			this.toolstripFormatItalic = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator21 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripFontDialog = new System.Windows.Forms.ToolStripButton();
			this.toolstripFontNormal = new System.Windows.Forms.ToolStripButton();
			this.toolstripTextColor = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator19 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripFontIncrease = new System.Windows.Forms.ToolStripButton();
			this.toolstripFontDecrease = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripJustifyLeft = new System.Windows.Forms.ToolStripButton();
			this.toolstripJustifyCenter = new System.Windows.Forms.ToolStripButton();
			this.toolstripJustifyRight = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripListOrdered = new System.Windows.Forms.ToolStripButton();
			this.toolstripListUnordered = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripFontOutdent = new System.Windows.Forms.ToolStripButton();
			this.toolstripFontIndent = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator20 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripInsertLine = new System.Windows.Forms.ToolStripButton();
			this.toolstripInsertTable = new System.Windows.Forms.ToolStripButton();
			this.toolstripInsertImage = new System.Windows.Forms.ToolStripButton();
			this.toolstripInsertLink = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripFindReplace = new System.Windows.Forms.ToolStripButton();
			this.toolstripDocumentNew = new System.Windows.Forms.ToolStripButton();
			this.toolstripDocumentOpen = new System.Windows.Forms.ToolStripButton();
			this.toolstripDocumentEditHTML = new System.Windows.Forms.ToolStripButton();
			this.toolstripDocumentSave = new System.Windows.Forms.ToolStripButton();
			this.toolstripDocumentPrint = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
			this.toolstripDocumentHelp = new System.Windows.Forms.ToolStripButton();
			this.contextEditor = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.contextTable = new System.Windows.Forms.ToolStripMenuItem();
			this.contextTableModify = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator22 = new System.Windows.Forms.ToolStripSeparator();
			this.contextTableInsertRow = new System.Windows.Forms.ToolStripMenuItem();
			this.contextTableDeleteRow = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator23 = new System.Windows.Forms.ToolStripSeparator();
			this.contextTableRowModify = new System.Windows.Forms.ToolStripMenuItem();
			this.contextTableCellModify = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator24 = new System.Windows.Forms.ToolStripSeparator();
			this.contextEdit = new System.Windows.Forms.ToolStripMenuItem();
			this.contextEditUndo = new System.Windows.Forms.ToolStripMenuItem();
			this.contextEditRedo = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator14 = new System.Windows.Forms.ToolStripSeparator();
			this.contextEditCut = new System.Windows.Forms.ToolStripMenuItem();
			this.contextEditCopy = new System.Windows.Forms.ToolStripMenuItem();
			this.contextEditPaste = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator15 = new System.Windows.Forms.ToolStripSeparator();
			this.contextEditFindReplace = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator16 = new System.Windows.Forms.ToolStripSeparator();
			this.contextEditSelectAll = new System.Windows.Forms.ToolStripMenuItem();
			this.contextEditDelete = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator25 = new System.Windows.Forms.ToolStripSeparator();
			this.contextDocumentEditHTML = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocument = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocumentNew = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocumentOpen = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocumentSave = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
			this.contextDocumentPrint = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
			this.contextDocumentToolbar = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocumentScrollbar = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocumentWordwrap = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocumentOverwrite = new System.Windows.Forms.ToolStripMenuItem();
			this.contextDocumentHelp = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
			this.contextFormat = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatFontDialog = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatColorDialog = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator11 = new System.Windows.Forms.ToolStripSeparator();
			this.contextFormatNormal = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator18 = new System.Windows.Forms.ToolStripSeparator();
			this.contextFormatBold = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatItalic = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatUnderline = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatSuperscript = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatSubscript = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatStrikeout = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator12 = new System.Windows.Forms.ToolStripSeparator();
			this.contextFormatIncrease = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatDecrease = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatIndent = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatOutdent = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator13 = new System.Windows.Forms.ToolStripSeparator();
			this.contextFormatListOrdered = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatListUnordered = new System.Windows.Forms.ToolStripMenuItem();
			this.contextJustify = new System.Windows.Forms.ToolStripMenuItem();
			this.contextJustifyLeft = new System.Windows.Forms.ToolStripMenuItem();
			this.contextJustifyCenter = new System.Windows.Forms.ToolStripMenuItem();
			this.contextJustifyRight = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormatting = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormattingNormal = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormattingHeading1 = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormattingHeading2 = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormattingHeading3 = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormattingHeading4 = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormattingHeading5 = new System.Windows.Forms.ToolStripMenuItem();
			this.contextFormattingFormatted = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
			this.contextInsert = new System.Windows.Forms.ToolStripMenuItem();
			this.contextInsertLine = new System.Windows.Forms.ToolStripMenuItem();
			this.contextInsertLink = new System.Windows.Forms.ToolStripMenuItem();
			this.contextInsertImage = new System.Windows.Forms.ToolStripMenuItem();
			this.contextInsertText = new System.Windows.Forms.ToolStripMenuItem();
			this.contextInsertHtml = new System.Windows.Forms.ToolStripMenuItem();
			this.contextInsertTable = new System.Windows.Forms.ToolStripMenuItem();
			this.browserPanel.SuspendLayout();
			this.toolstripEditor.SuspendLayout();
			this.contextEditor.SuspendLayout();
			this.SuspendLayout();
			// 
			// browserPanel
			// 
			this.browserPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.browserPanel.Controls.Add(this.editorWebBrowser);
			resources.ApplyResources(this.browserPanel, "browserPanel");
			this.browserPanel.Name = "browserPanel";
			this.browserPanel.Resize += new System.EventHandler(this.browserPanelResize);
			// 
			// editorWebBrowser
			// 
			this.editorWebBrowser.IsWebBrowserContextMenuEnabled = false;
			resources.ApplyResources(this.editorWebBrowser, "editorWebBrowser");
			this.editorWebBrowser.Name = "editorWebBrowser";
			this.editorWebBrowser.ScriptErrorsSuppressed = true;
			this.editorWebBrowser.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.BrowserDocumentComplete);
			this.editorWebBrowser.Navigating += new System.Windows.Forms.WebBrowserNavigatingEventHandler(this.BrowserBeforeNavigate);
			// 
			// toolstripEditor
			// 
			this.toolstripEditor.CanOverflow = false;
			resources.ApplyResources(this.toolstripEditor, "toolstripEditor");
			this.toolstripEditor.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolstripEditor.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolstripEnableEditing,
            this.toolStripSeparator17,
            this.toolstripTextCut,
            this.toolstripTextCopy,
            this.toolstripTextPaste,
            this.toolStripSeparator1,
            this.toolstripEditUndo,
            this.toolstripEditRedo,
            this.toolStripSeparator2,
            this.toolstripFontComboBox,
            this.toolStripSeparator26,
            this.toolstripFormatBold,
            this.toolstripFormatUnderline,
            this.toolstripFormatItalic,
            this.toolStripSeparator21,
            this.toolstripFontDialog,
            this.toolstripFontNormal,
            this.toolstripTextColor,
            this.toolStripSeparator19,
            this.toolstripFontIncrease,
            this.toolstripFontDecrease,
            this.toolStripSeparator3,
            this.toolstripJustifyLeft,
            this.toolstripJustifyCenter,
            this.toolstripJustifyRight,
            this.toolStripSeparator4,
            this.toolstripListOrdered,
            this.toolstripListUnordered,
            this.toolStripSeparator5,
            this.toolstripFontOutdent,
            this.toolstripFontIndent,
            this.toolStripSeparator20,
            this.toolstripInsertLine,
            this.toolstripInsertTable,
            this.toolstripInsertImage,
            this.toolstripInsertLink,
            this.toolStripSeparator6,
            this.toolstripFindReplace,
            this.toolstripDocumentNew,
            this.toolstripDocumentOpen,
            this.toolstripDocumentEditHTML,
            this.toolstripDocumentSave,
            this.toolstripDocumentPrint,
            this.toolStripSeparator,
            this.toolstripDocumentHelp});
			this.toolstripEditor.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
			this.toolstripEditor.Name = "toolstripEditor";
			this.toolstripEditor.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolstripEditor.ShowItemToolTips = false;
			// 
			// toolstripEnableEditing
			// 
			this.toolstripEnableEditing.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripEnableEditing.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarEdit;
			resources.ApplyResources(this.toolstripEnableEditing, "toolstripEnableEditing");
			this.toolstripEnableEditing.Name = "toolstripEnableEditing";
			this.toolstripEnableEditing.Click += new System.EventHandler(this.toolstripEnableEditClick);
			// 
			// toolStripSeparator17
			// 
			this.toolStripSeparator17.Name = "toolStripSeparator17";
			resources.ApplyResources(this.toolStripSeparator17, "toolStripSeparator17");
			// 
			// toolstripTextCut
			// 
			this.toolstripTextCut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripTextCut.Image = global::MSDN.Html.Editor.Properties.Resources.ToolBarCut;
			resources.ApplyResources(this.toolstripTextCut, "toolstripTextCut");
			this.toolstripTextCut.Name = "toolstripTextCut";
			this.toolstripTextCut.Tag = "TextCut";
			this.toolstripTextCut.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripTextCopy
			// 
			this.toolstripTextCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripTextCopy.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarCopy;
			resources.ApplyResources(this.toolstripTextCopy, "toolstripTextCopy");
			this.toolstripTextCopy.Name = "toolstripTextCopy";
			this.toolstripTextCopy.Tag = "TextCopy";
			this.toolstripTextCopy.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripTextPaste
			// 
			this.toolstripTextPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripTextPaste.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarPaste;
			resources.ApplyResources(this.toolstripTextPaste, "toolstripTextPaste");
			this.toolstripTextPaste.Name = "toolstripTextPaste";
			this.toolstripTextPaste.Tag = "TextPaste";
			this.toolstripTextPaste.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			resources.ApplyResources(this.toolStripSeparator1, "toolStripSeparator1");
			// 
			// toolstripEditUndo
			// 
			this.toolstripEditUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripEditUndo.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarUndo;
			resources.ApplyResources(this.toolstripEditUndo, "toolstripEditUndo");
			this.toolstripEditUndo.Name = "toolstripEditUndo";
			this.toolstripEditUndo.Tag = "EditUndo";
			this.toolstripEditUndo.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripEditRedo
			// 
			this.toolstripEditRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripEditRedo.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarRedo;
			resources.ApplyResources(this.toolstripEditRedo, "toolstripEditRedo");
			this.toolstripEditRedo.Name = "toolstripEditRedo";
			this.toolstripEditRedo.Tag = "EditRedo";
			this.toolstripEditRedo.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			resources.ApplyResources(this.toolStripSeparator2, "toolStripSeparator2");
			// 
			// toolstripFontComboBox
			// 
			this.toolstripFontComboBox.DropDownHeight = 400;
			this.toolstripFontComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.toolstripFontComboBox.DropDownWidth = 200;
			resources.ApplyResources(this.toolstripFontComboBox, "toolstripFontComboBox");
			this.toolstripFontComboBox.Name = "toolstripFontComboBox";
			this.toolstripFontComboBox.Tag = "FontCombo";
			this.toolstripFontComboBox.SelectedIndexChanged += new System.EventHandler(this.toolstripFontSelectionChanged);
			// 
			// toolStripSeparator26
			// 
			this.toolStripSeparator26.Name = "toolStripSeparator26";
			resources.ApplyResources(this.toolStripSeparator26, "toolStripSeparator26");
			// 
			// toolstripFormatBold
			// 
			this.toolstripFormatBold.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFormatBold.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarBold;
			resources.ApplyResources(this.toolstripFormatBold, "toolstripFormatBold");
			this.toolstripFormatBold.Name = "toolstripFormatBold";
			this.toolstripFormatBold.Tag = "FormatBold";
			this.toolstripFormatBold.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripFormatUnderline
			// 
			this.toolstripFormatUnderline.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFormatUnderline.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarUnderline;
			resources.ApplyResources(this.toolstripFormatUnderline, "toolstripFormatUnderline");
			this.toolstripFormatUnderline.Name = "toolstripFormatUnderline";
			this.toolstripFormatUnderline.Tag = "FormatUnderline";
			this.toolstripFormatUnderline.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripFormatItalic
			// 
			this.toolstripFormatItalic.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFormatItalic.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarItalic;
			resources.ApplyResources(this.toolstripFormatItalic, "toolstripFormatItalic");
			this.toolstripFormatItalic.Name = "toolstripFormatItalic";
			this.toolstripFormatItalic.Tag = "FormatItalic";
			this.toolstripFormatItalic.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator21
			// 
			this.toolStripSeparator21.Name = "toolStripSeparator21";
			resources.ApplyResources(this.toolStripSeparator21, "toolStripSeparator21");
			// 
			// toolstripFontDialog
			// 
			this.toolstripFontDialog.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFontDialog.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontStyle;
			resources.ApplyResources(this.toolstripFontDialog, "toolstripFontDialog");
			this.toolstripFontDialog.Name = "toolstripFontDialog";
			this.toolstripFontDialog.Tag = "FontDialog";
			this.toolstripFontDialog.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripFontNormal
			// 
			this.toolstripFontNormal.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFontNormal.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontNormal;
			resources.ApplyResources(this.toolstripFontNormal, "toolstripFontNormal");
			this.toolstripFontNormal.Name = "toolstripFontNormal";
			this.toolstripFontNormal.Tag = "FontNormal";
			this.toolstripFontNormal.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripTextColor
			// 
			this.toolstripTextColor.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripTextColor.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTextColor;
			resources.ApplyResources(this.toolstripTextColor, "toolstripTextColor");
			this.toolstripTextColor.Name = "toolstripTextColor";
			this.toolstripTextColor.Tag = "TextColor";
			this.toolstripTextColor.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator19
			// 
			this.toolStripSeparator19.Name = "toolStripSeparator19";
			resources.ApplyResources(this.toolStripSeparator19, "toolStripSeparator19");
			// 
			// toolstripFontIncrease
			// 
			this.toolstripFontIncrease.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFontIncrease.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontUp;
			resources.ApplyResources(this.toolstripFontIncrease, "toolstripFontIncrease");
			this.toolstripFontIncrease.Name = "toolstripFontIncrease";
			this.toolstripFontIncrease.Tag = "FontIncrease";
			this.toolstripFontIncrease.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripFontDecrease
			// 
			this.toolstripFontDecrease.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFontDecrease.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontDown;
			resources.ApplyResources(this.toolstripFontDecrease, "toolstripFontDecrease");
			this.toolstripFontDecrease.Name = "toolstripFontDecrease";
			this.toolstripFontDecrease.Tag = "FontDecrease";
			this.toolstripFontDecrease.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator3
			// 
			this.toolStripSeparator3.Name = "toolStripSeparator3";
			resources.ApplyResources(this.toolStripSeparator3, "toolStripSeparator3");
			// 
			// toolstripJustifyLeft
			// 
			this.toolstripJustifyLeft.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripJustifyLeft.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarJustifyLeft;
			resources.ApplyResources(this.toolstripJustifyLeft, "toolstripJustifyLeft");
			this.toolstripJustifyLeft.Name = "toolstripJustifyLeft";
			this.toolstripJustifyLeft.Tag = "JustifyLeft";
			this.toolstripJustifyLeft.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripJustifyCenter
			// 
			this.toolstripJustifyCenter.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripJustifyCenter.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarJustifyCenter;
			resources.ApplyResources(this.toolstripJustifyCenter, "toolstripJustifyCenter");
			this.toolstripJustifyCenter.Name = "toolstripJustifyCenter";
			this.toolstripJustifyCenter.Tag = "JustifyCenter";
			this.toolstripJustifyCenter.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripJustifyRight
			// 
			this.toolstripJustifyRight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripJustifyRight.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarJustifyRight;
			resources.ApplyResources(this.toolstripJustifyRight, "toolstripJustifyRight");
			this.toolstripJustifyRight.Name = "toolstripJustifyRight";
			this.toolstripJustifyRight.Tag = "JustifyRight";
			this.toolstripJustifyRight.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator4
			// 
			this.toolStripSeparator4.Name = "toolStripSeparator4";
			resources.ApplyResources(this.toolStripSeparator4, "toolStripSeparator4");
			// 
			// toolstripListOrdered
			// 
			this.toolstripListOrdered.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripListOrdered.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarListOrdered;
			resources.ApplyResources(this.toolstripListOrdered, "toolstripListOrdered");
			this.toolstripListOrdered.Name = "toolstripListOrdered";
			this.toolstripListOrdered.Tag = "ListOrdered";
			this.toolstripListOrdered.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripListUnordered
			// 
			this.toolstripListUnordered.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripListUnordered.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarListUnordered;
			resources.ApplyResources(this.toolstripListUnordered, "toolstripListUnordered");
			this.toolstripListUnordered.Name = "toolstripListUnordered";
			this.toolstripListUnordered.Tag = "ListUnordered";
			this.toolstripListUnordered.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator5
			// 
			this.toolStripSeparator5.Name = "toolStripSeparator5";
			resources.ApplyResources(this.toolStripSeparator5, "toolStripSeparator5");
			// 
			// toolstripFontOutdent
			// 
			this.toolstripFontOutdent.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFontOutdent.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTabRemove;
			resources.ApplyResources(this.toolstripFontOutdent, "toolstripFontOutdent");
			this.toolstripFontOutdent.Name = "toolstripFontOutdent";
			this.toolstripFontOutdent.Tag = "FontOutdent";
			this.toolstripFontOutdent.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripFontIndent
			// 
			this.toolstripFontIndent.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFontIndent.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTabInsert;
			resources.ApplyResources(this.toolstripFontIndent, "toolstripFontIndent");
			this.toolstripFontIndent.Name = "toolstripFontIndent";
			this.toolstripFontIndent.Tag = "FontIndent";
			this.toolstripFontIndent.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator20
			// 
			this.toolStripSeparator20.Name = "toolStripSeparator20";
			resources.ApplyResources(this.toolStripSeparator20, "toolStripSeparator20");
			// 
			// toolstripInsertLine
			// 
			this.toolstripInsertLine.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripInsertLine.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarLine;
			resources.ApplyResources(this.toolstripInsertLine, "toolstripInsertLine");
			this.toolstripInsertLine.Name = "toolstripInsertLine";
			this.toolstripInsertLine.Tag = "InsertLine";
			this.toolstripInsertLine.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripInsertTable
			// 
			this.toolstripInsertTable.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripInsertTable.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTable;
			resources.ApplyResources(this.toolstripInsertTable, "toolstripInsertTable");
			this.toolstripInsertTable.Name = "toolstripInsertTable";
			this.toolstripInsertTable.Tag = "InsertTable";
			this.toolstripInsertTable.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripInsertImage
			// 
			this.toolstripInsertImage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripInsertImage.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarImage;
			resources.ApplyResources(this.toolstripInsertImage, "toolstripInsertImage");
			this.toolstripInsertImage.Name = "toolstripInsertImage";
			this.toolstripInsertImage.Tag = "InsertImage";
			this.toolstripInsertImage.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripInsertLink
			// 
			this.toolstripInsertLink.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripInsertLink.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarWebLink;
			resources.ApplyResources(this.toolstripInsertLink, "toolstripInsertLink");
			this.toolstripInsertLink.Name = "toolstripInsertLink";
			this.toolstripInsertLink.Tag = "InsertLink";
			this.toolstripInsertLink.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolStripSeparator6
			// 
			this.toolStripSeparator6.Name = "toolStripSeparator6";
			resources.ApplyResources(this.toolStripSeparator6, "toolStripSeparator6");
			// 
			// toolstripFindReplace
			// 
			this.toolstripFindReplace.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripFindReplace.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFind;
			resources.ApplyResources(this.toolstripFindReplace, "toolstripFindReplace");
			this.toolstripFindReplace.Name = "toolstripFindReplace";
			this.toolstripFindReplace.Tag = "FindReplace";
			this.toolstripFindReplace.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripDocumentNew
			// 
			this.toolstripDocumentNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			resources.ApplyResources(this.toolstripDocumentNew, "toolstripDocumentNew");
			this.toolstripDocumentNew.Name = "toolstripDocumentNew";
			// 
			// toolstripDocumentOpen
			// 
			this.toolstripDocumentOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripDocumentOpen.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFileOpen;
			resources.ApplyResources(this.toolstripDocumentOpen, "toolstripDocumentOpen");
			this.toolstripDocumentOpen.Name = "toolstripDocumentOpen";
			// 
			// toolstripDocumentEditHTML
			// 
			this.toolstripDocumentEditHTML.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripDocumentEditHTML.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarEditHtml;
			resources.ApplyResources(this.toolstripDocumentEditHTML, "toolstripDocumentEditHTML");
			this.toolstripDocumentEditHTML.Name = "toolstripDocumentEditHTML";
			this.toolstripDocumentEditHTML.Tag = "EditHTML";
			this.toolstripDocumentEditHTML.Click += new System.EventHandler(this.toolstripEditorClick);
			// 
			// toolstripDocumentSave
			// 
			this.toolstripDocumentSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolstripDocumentSave.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFileSave;
			resources.ApplyResources(this.toolstripDocumentSave, "toolstripDocumentSave");
			this.toolstripDocumentSave.Name = "toolstripDocumentSave";
			// 
			// toolstripDocumentPrint
			// 
			this.toolstripDocumentPrint.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			resources.ApplyResources(this.toolstripDocumentPrint, "toolstripDocumentPrint");
			this.toolstripDocumentPrint.Name = "toolstripDocumentPrint";
			// 
			// toolStripSeparator
			// 
			this.toolStripSeparator.Name = "toolStripSeparator";
			resources.ApplyResources(this.toolStripSeparator, "toolStripSeparator");
			// 
			// toolstripDocumentHelp
			// 
			this.toolstripDocumentHelp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			resources.ApplyResources(this.toolstripDocumentHelp, "toolstripDocumentHelp");
			this.toolstripDocumentHelp.Name = "toolstripDocumentHelp";
			// 
			// contextEditor
			// 
			this.contextEditor.AllowDrop = true;
			this.contextEditor.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextTable,
            this.toolStripSeparator24,
            this.contextEdit,
            this.contextDocument,
            this.toolStripSeparator7,
            this.contextFormat,
            this.contextJustify,
            this.contextFormatting,
            this.toolStripSeparator8,
            this.contextInsert});
			this.contextEditor.Name = "contextEditor";
			this.contextEditor.ShowImageMargin = false;
			resources.ApplyResources(this.contextEditor, "contextEditor");
			// 
			// contextTable
			// 
			this.contextTable.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextTableModify,
            this.toolStripSeparator22,
            this.contextTableInsertRow,
            this.contextTableDeleteRow,
            this.toolStripSeparator23,
            this.contextTableRowModify,
            this.contextTableCellModify});
			resources.ApplyResources(this.contextTable, "contextTable");
			this.contextTable.Name = "contextTable";
			// 
			// contextTableModify
			// 
			this.contextTableModify.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTable;
			resources.ApplyResources(this.contextTableModify, "contextTableModify");
			this.contextTableModify.Name = "contextTableModify";
			this.contextTableModify.Tag = "TableModify";
			this.contextTableModify.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator22
			// 
			this.toolStripSeparator22.Name = "toolStripSeparator22";
			resources.ApplyResources(this.toolStripSeparator22, "toolStripSeparator22");
			// 
			// contextTableInsertRow
			// 
			this.contextTableInsertRow.Name = "contextTableInsertRow";
			resources.ApplyResources(this.contextTableInsertRow, "contextTableInsertRow");
			this.contextTableInsertRow.Tag = "TableInsertRow";
			this.contextTableInsertRow.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextTableDeleteRow
			// 
			this.contextTableDeleteRow.Name = "contextTableDeleteRow";
			resources.ApplyResources(this.contextTableDeleteRow, "contextTableDeleteRow");
			this.contextTableDeleteRow.Tag = "TableDeleteRow";
			this.contextTableDeleteRow.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator23
			// 
			this.toolStripSeparator23.Name = "toolStripSeparator23";
			resources.ApplyResources(this.toolStripSeparator23, "toolStripSeparator23");
			// 
			// contextTableRowModify
			// 
			this.contextTableRowModify.Name = "contextTableRowModify";
			resources.ApplyResources(this.contextTableRowModify, "contextTableRowModify");
			this.contextTableRowModify.Tag = "TableRowModify";
			this.contextTableRowModify.Click += new System.EventHandler(this.TableRowModifyPrompt);
			// 
			// contextTableCellModify
			// 
			this.contextTableCellModify.Name = "contextTableCellModify";
			resources.ApplyResources(this.contextTableCellModify, "contextTableCellModify");
			this.contextTableCellModify.Tag = "TableCellModify";
			this.contextTableCellModify.Click += new System.EventHandler(this.TableCellModifyPrompt);
			// 
			// toolStripSeparator24
			// 
			this.toolStripSeparator24.Name = "toolStripSeparator24";
			resources.ApplyResources(this.toolStripSeparator24, "toolStripSeparator24");
			// 
			// contextEdit
			// 
			this.contextEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextEditUndo,
            this.contextEditRedo,
            this.toolStripSeparator14,
            this.contextEditCut,
            this.contextEditCopy,
            this.contextEditPaste,
            this.toolStripSeparator15,
            this.contextEditFindReplace,
            this.toolStripSeparator16,
            this.contextEditSelectAll,
            this.contextEditDelete,
            this.toolStripSeparator25,
            this.contextDocumentEditHTML});
			this.contextEdit.Name = "contextEdit";
			resources.ApplyResources(this.contextEdit, "contextEdit");
			// 
			// contextEditUndo
			// 
			this.contextEditUndo.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarUndo;
			resources.ApplyResources(this.contextEditUndo, "contextEditUndo");
			this.contextEditUndo.Name = "contextEditUndo";
			this.contextEditUndo.Tag = "EditUndo";
			this.contextEditUndo.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextEditRedo
			// 
			this.contextEditRedo.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarRedo;
			resources.ApplyResources(this.contextEditRedo, "contextEditRedo");
			this.contextEditRedo.Name = "contextEditRedo";
			this.contextEditRedo.Tag = "EditRedo";
			this.contextEditRedo.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator14
			// 
			this.toolStripSeparator14.Name = "toolStripSeparator14";
			resources.ApplyResources(this.toolStripSeparator14, "toolStripSeparator14");
			// 
			// contextEditCut
			// 
			this.contextEditCut.Image = global::MSDN.Html.Editor.Properties.Resources.ToolBarCut;
			resources.ApplyResources(this.contextEditCut, "contextEditCut");
			this.contextEditCut.Name = "contextEditCut";
			this.contextEditCut.Tag = "TextCut";
			this.contextEditCut.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextEditCopy
			// 
			this.contextEditCopy.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarCopy;
			resources.ApplyResources(this.contextEditCopy, "contextEditCopy");
			this.contextEditCopy.Name = "contextEditCopy";
			this.contextEditCopy.Tag = "TextCopy";
			this.contextEditCopy.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextEditPaste
			// 
			this.contextEditPaste.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarPaste;
			resources.ApplyResources(this.contextEditPaste, "contextEditPaste");
			this.contextEditPaste.Name = "contextEditPaste";
			this.contextEditPaste.Tag = "TextPaste";
			this.contextEditPaste.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator15
			// 
			this.toolStripSeparator15.Name = "toolStripSeparator15";
			resources.ApplyResources(this.toolStripSeparator15, "toolStripSeparator15");
			// 
			// contextEditFindReplace
			// 
			this.contextEditFindReplace.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFind;
			resources.ApplyResources(this.contextEditFindReplace, "contextEditFindReplace");
			this.contextEditFindReplace.Name = "contextEditFindReplace";
			this.contextEditFindReplace.Tag = "FindReplace";
			this.contextEditFindReplace.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator16
			// 
			this.toolStripSeparator16.Name = "toolStripSeparator16";
			resources.ApplyResources(this.toolStripSeparator16, "toolStripSeparator16");
			// 
			// contextEditSelectAll
			// 
			this.contextEditSelectAll.Name = "contextEditSelectAll";
			resources.ApplyResources(this.contextEditSelectAll, "contextEditSelectAll");
			this.contextEditSelectAll.Tag = "SelectAll";
			this.contextEditSelectAll.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextEditDelete
			// 
			this.contextEditDelete.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarDelete;
			resources.ApplyResources(this.contextEditDelete, "contextEditDelete");
			this.contextEditDelete.Name = "contextEditDelete";
			this.contextEditDelete.Tag = "TextDelete";
			this.contextEditDelete.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator25
			// 
			this.toolStripSeparator25.Name = "toolStripSeparator25";
			resources.ApplyResources(this.toolStripSeparator25, "toolStripSeparator25");
			// 
			// contextDocumentEditHTML
			// 
			this.contextDocumentEditHTML.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarEditHtml;
			resources.ApplyResources(this.contextDocumentEditHTML, "contextDocumentEditHTML");
			this.contextDocumentEditHTML.Name = "contextDocumentEditHTML";
			this.contextDocumentEditHTML.Tag = "EditHTML";
			this.contextDocumentEditHTML.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextDocument
			// 
			this.contextDocument.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextDocumentNew,
            this.contextDocumentOpen,
            this.contextDocumentSave,
            this.toolStripSeparator9,
            this.contextDocumentPrint,
            this.toolStripSeparator10,
            this.contextDocumentToolbar,
            this.contextDocumentScrollbar,
            this.contextDocumentWordwrap,
            this.contextDocumentOverwrite,
            this.contextDocumentHelp});
			this.contextDocument.Name = "contextDocument";
			resources.ApplyResources(this.contextDocument, "contextDocument");
			// 
			// contextDocumentNew
			// 
			this.contextDocumentNew.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFileNew;
			resources.ApplyResources(this.contextDocumentNew, "contextDocumentNew");
			this.contextDocumentNew.Name = "contextDocumentNew";
			// 
			// contextDocumentOpen
			// 
			this.contextDocumentOpen.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFileOpen;
			resources.ApplyResources(this.contextDocumentOpen, "contextDocumentOpen");
			this.contextDocumentOpen.Name = "contextDocumentOpen";
			this.contextDocumentOpen.Tag = "OpenFile";
			this.contextDocumentOpen.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextDocumentSave
			// 
			this.contextDocumentSave.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFileSave;
			resources.ApplyResources(this.contextDocumentSave, "contextDocumentSave");
			this.contextDocumentSave.Name = "contextDocumentSave";
			this.contextDocumentSave.Tag = "SaveFile";
			this.contextDocumentSave.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator9
			// 
			this.toolStripSeparator9.Name = "toolStripSeparator9";
			resources.ApplyResources(this.toolStripSeparator9, "toolStripSeparator9");
			// 
			// contextDocumentPrint
			// 
			this.contextDocumentPrint.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarPrint;
			resources.ApplyResources(this.contextDocumentPrint, "contextDocumentPrint");
			this.contextDocumentPrint.Name = "contextDocumentPrint";
			this.contextDocumentPrint.Tag = "DocumentPrint";
			this.contextDocumentPrint.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator10
			// 
			this.toolStripSeparator10.Name = "toolStripSeparator10";
			resources.ApplyResources(this.toolStripSeparator10, "toolStripSeparator10");
			// 
			// contextDocumentToolbar
			// 
			this.contextDocumentToolbar.Name = "contextDocumentToolbar";
			resources.ApplyResources(this.contextDocumentToolbar, "contextDocumentToolbar");
			this.contextDocumentToolbar.Tag = "ToggleToolbar";
			this.contextDocumentToolbar.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextDocumentScrollbar
			// 
			this.contextDocumentScrollbar.Name = "contextDocumentScrollbar";
			resources.ApplyResources(this.contextDocumentScrollbar, "contextDocumentScrollbar");
			this.contextDocumentScrollbar.Tag = "ToggleScrollbar";
			this.contextDocumentScrollbar.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextDocumentWordwrap
			// 
			this.contextDocumentWordwrap.Name = "contextDocumentWordwrap";
			resources.ApplyResources(this.contextDocumentWordwrap, "contextDocumentWordwrap");
			this.contextDocumentWordwrap.Tag = "ToggleWordwrap";
			this.contextDocumentWordwrap.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextDocumentOverwrite
			// 
			this.contextDocumentOverwrite.Name = "contextDocumentOverwrite";
			resources.ApplyResources(this.contextDocumentOverwrite, "contextDocumentOverwrite");
			this.contextDocumentOverwrite.Tag = "ToggleOverwrite";
			this.contextDocumentOverwrite.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextDocumentHelp
			// 
			this.contextDocumentHelp.Name = "contextDocumentHelp";
			resources.ApplyResources(this.contextDocumentHelp, "contextDocumentHelp");
			// 
			// toolStripSeparator7
			// 
			this.toolStripSeparator7.Name = "toolStripSeparator7";
			resources.ApplyResources(this.toolStripSeparator7, "toolStripSeparator7");
			// 
			// contextFormat
			// 
			this.contextFormat.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextFormatFontDialog,
            this.contextFormatColorDialog,
            this.toolStripSeparator11,
            this.contextFormatNormal,
            this.toolStripSeparator18,
            this.contextFormatBold,
            this.contextFormatItalic,
            this.contextFormatUnderline,
            this.contextFormatSuperscript,
            this.contextFormatSubscript,
            this.contextFormatStrikeout,
            this.toolStripSeparator12,
            this.contextFormatIncrease,
            this.contextFormatDecrease,
            this.contextFormatIndent,
            this.contextFormatOutdent,
            this.toolStripSeparator13,
            this.contextFormatListOrdered,
            this.contextFormatListUnordered});
			this.contextFormat.Name = "contextFormat";
			resources.ApplyResources(this.contextFormat, "contextFormat");
			// 
			// contextFormatFontDialog
			// 
			this.contextFormatFontDialog.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontStyle;
			resources.ApplyResources(this.contextFormatFontDialog, "contextFormatFontDialog");
			this.contextFormatFontDialog.Name = "contextFormatFontDialog";
			this.contextFormatFontDialog.Tag = "FontDialog";
			this.contextFormatFontDialog.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatColorDialog
			// 
			this.contextFormatColorDialog.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTextColor;
			resources.ApplyResources(this.contextFormatColorDialog, "contextFormatColorDialog");
			this.contextFormatColorDialog.Name = "contextFormatColorDialog";
			this.contextFormatColorDialog.Tag = "ColorDialog";
			this.contextFormatColorDialog.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator11
			// 
			this.toolStripSeparator11.Name = "toolStripSeparator11";
			resources.ApplyResources(this.toolStripSeparator11, "toolStripSeparator11");
			// 
			// contextFormatNormal
			// 
			this.contextFormatNormal.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontNormal;
			resources.ApplyResources(this.contextFormatNormal, "contextFormatNormal");
			this.contextFormatNormal.Name = "contextFormatNormal";
			this.contextFormatNormal.Tag = "FontNormal";
			this.contextFormatNormal.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator18
			// 
			this.toolStripSeparator18.Name = "toolStripSeparator18";
			resources.ApplyResources(this.toolStripSeparator18, "toolStripSeparator18");
			// 
			// contextFormatBold
			// 
			this.contextFormatBold.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarBold;
			resources.ApplyResources(this.contextFormatBold, "contextFormatBold");
			this.contextFormatBold.Name = "contextFormatBold";
			this.contextFormatBold.Tag = "FormatBold";
			this.contextFormatBold.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatItalic
			// 
			this.contextFormatItalic.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarItalic;
			resources.ApplyResources(this.contextFormatItalic, "contextFormatItalic");
			this.contextFormatItalic.Name = "contextFormatItalic";
			this.contextFormatItalic.Tag = "FormatItalic";
			this.contextFormatItalic.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatUnderline
			// 
			this.contextFormatUnderline.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarUnderline;
			resources.ApplyResources(this.contextFormatUnderline, "contextFormatUnderline");
			this.contextFormatUnderline.Name = "contextFormatUnderline";
			this.contextFormatUnderline.Tag = "FormatUnderline";
			this.contextFormatUnderline.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatSuperscript
			// 
			this.contextFormatSuperscript.Name = "contextFormatSuperscript";
			resources.ApplyResources(this.contextFormatSuperscript, "contextFormatSuperscript");
			this.contextFormatSuperscript.Tag = "FormatSuperscript";
			this.contextFormatSuperscript.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatSubscript
			// 
			this.contextFormatSubscript.Name = "contextFormatSubscript";
			resources.ApplyResources(this.contextFormatSubscript, "contextFormatSubscript");
			this.contextFormatSubscript.Tag = "FormatSubscript";
			this.contextFormatSubscript.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatStrikeout
			// 
			this.contextFormatStrikeout.Name = "contextFormatStrikeout";
			resources.ApplyResources(this.contextFormatStrikeout, "contextFormatStrikeout");
			this.contextFormatStrikeout.Tag = "FormatStrikeout";
			this.contextFormatStrikeout.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator12
			// 
			this.toolStripSeparator12.Name = "toolStripSeparator12";
			resources.ApplyResources(this.toolStripSeparator12, "toolStripSeparator12");
			// 
			// contextFormatIncrease
			// 
			this.contextFormatIncrease.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontUp;
			resources.ApplyResources(this.contextFormatIncrease, "contextFormatIncrease");
			this.contextFormatIncrease.Name = "contextFormatIncrease";
			this.contextFormatIncrease.Tag = "FontIncrease";
			this.contextFormatIncrease.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatDecrease
			// 
			this.contextFormatDecrease.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarFontDown;
			resources.ApplyResources(this.contextFormatDecrease, "contextFormatDecrease");
			this.contextFormatDecrease.Name = "contextFormatDecrease";
			this.contextFormatDecrease.Tag = "FontDecrease";
			this.contextFormatDecrease.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatIndent
			// 
			this.contextFormatIndent.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTabInsert;
			resources.ApplyResources(this.contextFormatIndent, "contextFormatIndent");
			this.contextFormatIndent.Name = "contextFormatIndent";
			this.contextFormatIndent.Tag = "FontIndent";
			this.contextFormatIndent.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatOutdent
			// 
			this.contextFormatOutdent.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTabRemove;
			resources.ApplyResources(this.contextFormatOutdent, "contextFormatOutdent");
			this.contextFormatOutdent.Name = "contextFormatOutdent";
			this.contextFormatOutdent.Tag = "FontOutdent";
			this.contextFormatOutdent.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator13
			// 
			this.toolStripSeparator13.Name = "toolStripSeparator13";
			resources.ApplyResources(this.toolStripSeparator13, "toolStripSeparator13");
			// 
			// contextFormatListOrdered
			// 
			this.contextFormatListOrdered.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarListOrdered;
			resources.ApplyResources(this.contextFormatListOrdered, "contextFormatListOrdered");
			this.contextFormatListOrdered.Name = "contextFormatListOrdered";
			this.contextFormatListOrdered.Tag = "ListOrdered";
			this.contextFormatListOrdered.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatListUnordered
			// 
			this.contextFormatListUnordered.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarListUnordered;
			resources.ApplyResources(this.contextFormatListUnordered, "contextFormatListUnordered");
			this.contextFormatListUnordered.Name = "contextFormatListUnordered";
			this.contextFormatListUnordered.Tag = "ListUnordered";
			this.contextFormatListUnordered.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextJustify
			// 
			this.contextJustify.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextJustifyLeft,
            this.contextJustifyCenter,
            this.contextJustifyRight});
			this.contextJustify.Name = "contextJustify";
			resources.ApplyResources(this.contextJustify, "contextJustify");
			// 
			// contextJustifyLeft
			// 
			this.contextJustifyLeft.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarJustifyLeft;
			resources.ApplyResources(this.contextJustifyLeft, "contextJustifyLeft");
			this.contextJustifyLeft.Name = "contextJustifyLeft";
			this.contextJustifyLeft.Tag = "JustifyLeft";
			this.contextJustifyLeft.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextJustifyCenter
			// 
			this.contextJustifyCenter.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarJustifyCenter;
			resources.ApplyResources(this.contextJustifyCenter, "contextJustifyCenter");
			this.contextJustifyCenter.Name = "contextJustifyCenter";
			this.contextJustifyCenter.Tag = "JustifyCenter";
			this.contextJustifyCenter.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextJustifyRight
			// 
			this.contextJustifyRight.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarJustifyRight;
			resources.ApplyResources(this.contextJustifyRight, "contextJustifyRight");
			this.contextJustifyRight.Name = "contextJustifyRight";
			this.contextJustifyRight.Tag = "JustifyRight";
			this.contextJustifyRight.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormatting
			// 
			this.contextFormatting.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextFormattingNormal,
            this.contextFormattingHeading1,
            this.contextFormattingHeading2,
            this.contextFormattingHeading3,
            this.contextFormattingHeading4,
            this.contextFormattingHeading5,
            this.contextFormattingFormatted});
			this.contextFormatting.Name = "contextFormatting";
			resources.ApplyResources(this.contextFormatting, "contextFormatting");
			// 
			// contextFormattingNormal
			// 
			resources.ApplyResources(this.contextFormattingNormal, "contextFormattingNormal");
			this.contextFormattingNormal.Name = "contextFormattingNormal";
			this.contextFormattingNormal.Tag = "FormattedNormal";
			this.contextFormattingNormal.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormattingHeading1
			// 
			this.contextFormattingHeading1.Name = "contextFormattingHeading1";
			resources.ApplyResources(this.contextFormattingHeading1, "contextFormattingHeading1");
			this.contextFormattingHeading1.Tag = "FormattedHeading1";
			this.contextFormattingHeading1.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormattingHeading2
			// 
			this.contextFormattingHeading2.Name = "contextFormattingHeading2";
			resources.ApplyResources(this.contextFormattingHeading2, "contextFormattingHeading2");
			this.contextFormattingHeading2.Tag = "FormattedHeading2";
			this.contextFormattingHeading2.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormattingHeading3
			// 
			this.contextFormattingHeading3.Name = "contextFormattingHeading3";
			resources.ApplyResources(this.contextFormattingHeading3, "contextFormattingHeading3");
			this.contextFormattingHeading3.Tag = "FormattedHeading3";
			this.contextFormattingHeading3.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormattingHeading4
			// 
			this.contextFormattingHeading4.Name = "contextFormattingHeading4";
			resources.ApplyResources(this.contextFormattingHeading4, "contextFormattingHeading4");
			this.contextFormattingHeading4.Tag = "FormattedHeading4";
			this.contextFormattingHeading4.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormattingHeading5
			// 
			this.contextFormattingHeading5.Name = "contextFormattingHeading5";
			resources.ApplyResources(this.contextFormattingHeading5, "contextFormattingHeading5");
			this.contextFormattingHeading5.Tag = "FormattedHeading5";
			this.contextFormattingHeading5.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextFormattingFormatted
			// 
			this.contextFormattingFormatted.Name = "contextFormattingFormatted";
			resources.ApplyResources(this.contextFormattingFormatted, "contextFormattingFormatted");
			this.contextFormattingFormatted.Tag = "FormattedPre";
			this.contextFormattingFormatted.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// toolStripSeparator8
			// 
			this.toolStripSeparator8.Name = "toolStripSeparator8";
			resources.ApplyResources(this.toolStripSeparator8, "toolStripSeparator8");
			// 
			// contextInsert
			// 
			this.contextInsert.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextInsertLine,
            this.contextInsertLink,
            this.contextInsertImage,
            this.contextInsertText,
            this.contextInsertHtml,
            this.contextInsertTable});
			this.contextInsert.Name = "contextInsert";
			resources.ApplyResources(this.contextInsert, "contextInsert");
			// 
			// contextInsertLine
			// 
			this.contextInsertLine.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarLine;
			resources.ApplyResources(this.contextInsertLine, "contextInsertLine");
			this.contextInsertLine.Name = "contextInsertLine";
			this.contextInsertLine.Tag = "InsertLine";
			this.contextInsertLine.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextInsertLink
			// 
			this.contextInsertLink.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarWebLink;
			resources.ApplyResources(this.contextInsertLink, "contextInsertLink");
			this.contextInsertLink.Name = "contextInsertLink";
			this.contextInsertLink.Tag = "InsertLink";
			this.contextInsertLink.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextInsertImage
			// 
			this.contextInsertImage.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarImage;
			resources.ApplyResources(this.contextInsertImage, "contextInsertImage");
			this.contextInsertImage.Name = "contextInsertImage";
			this.contextInsertImage.Tag = "InsertImage";
			this.contextInsertImage.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextInsertText
			// 
			this.contextInsertText.Name = "contextInsertText";
			resources.ApplyResources(this.contextInsertText, "contextInsertText");
			this.contextInsertText.Tag = "InsertText";
			this.contextInsertText.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextInsertHtml
			// 
			this.contextInsertHtml.Name = "contextInsertHtml";
			resources.ApplyResources(this.contextInsertHtml, "contextInsertHtml");
			this.contextInsertHtml.Tag = "InsertHtml";
			this.contextInsertHtml.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// contextInsertTable
			// 
			this.contextInsertTable.Image = global::MSDN.Html.Editor.Properties.Resources.ToolbarTable;
			resources.ApplyResources(this.contextInsertTable, "contextInsertTable");
			this.contextInsertTable.Name = "contextInsertTable";
			this.contextInsertTable.Tag = "InsertTable";
			this.contextInsertTable.Click += new System.EventHandler(this.contextEditorClick);
			// 
			// HtmlEditorControl
			// 
			this.Controls.Add(this.toolstripEditor);
			this.Controls.Add(this.browserPanel);
			this.Name = "HtmlEditorControl";
			resources.ApplyResources(this, "$this");
			this.browserPanel.ResumeLayout(false);
			this.toolstripEditor.ResumeLayout(false);
			this.toolstripEditor.PerformLayout();
			this.contextEditor.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

        }
        #endregion

        private System.Windows.Forms.Panel browserPanel;
        private IIControls.ToolStripEx toolstripEditor;
        private System.Windows.Forms.ToolStripButton toolstripTextCut;
        private System.Windows.Forms.ToolStripButton toolstripTextCopy;
        private System.Windows.Forms.ToolStripButton toolstripTextPaste;
        private System.Windows.Forms.ToolStripButton toolstripEditUndo;
        private System.Windows.Forms.ToolStripButton toolstripEditRedo;
        private System.Windows.Forms.ToolStripButton toolstripFormatBold;
        private System.Windows.Forms.ToolStripButton toolstripFormatUnderline;
        private System.Windows.Forms.ToolStripButton toolstripFormatItalic;
        private System.Windows.Forms.ToolStripButton toolstripFontDialog;
        private System.Windows.Forms.ToolStripButton toolstripFontNormal;
        private System.Windows.Forms.ToolStripButton toolstripTextColor;
        private System.Windows.Forms.ToolStripButton toolstripFontIncrease;
        private System.Windows.Forms.ToolStripButton toolstripFontDecrease;
        private System.Windows.Forms.ToolStripButton toolstripJustifyLeft;
        private System.Windows.Forms.ToolStripButton toolstripJustifyCenter;
        private System.Windows.Forms.ToolStripButton toolstripJustifyRight;
        private System.Windows.Forms.ToolStripButton toolstripFontIndent;
        private System.Windows.Forms.ToolStripButton toolstripFontOutdent;
        private System.Windows.Forms.ToolStripButton toolstripListOrdered;
        private System.Windows.Forms.ToolStripButton toolstripListUnordered;
        private System.Windows.Forms.ToolStripButton toolstripInsertLine;
        private System.Windows.Forms.ToolStripButton toolstripInsertTable;
        private System.Windows.Forms.ToolStripButton toolstripInsertImage;
        private System.Windows.Forms.ToolStripButton toolstripInsertLink;
        private System.Windows.Forms.ToolStripButton toolstripFindReplace;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ContextMenuStrip contextEditor;
        private System.Windows.Forms.ToolStripMenuItem contextTable;
        private System.Windows.Forms.ToolStripMenuItem contextEdit;
        private System.Windows.Forms.ToolStripMenuItem contextDocument;
        private System.Windows.Forms.ToolStripMenuItem contextFormat;
        private System.Windows.Forms.ToolStripMenuItem contextJustify;
        private System.Windows.Forms.ToolStripMenuItem contextFormatting;
        private System.Windows.Forms.ToolStripMenuItem contextInsert;
        private System.Windows.Forms.ToolStripMenuItem contextTableModify;
        private System.Windows.Forms.ToolStripMenuItem contextTableInsertRow;
        private System.Windows.Forms.ToolStripMenuItem contextTableDeleteRow;
        private System.Windows.Forms.ToolStripMenuItem contextEditUndo;
        private System.Windows.Forms.ToolStripMenuItem contextEditRedo;
        private System.Windows.Forms.ToolStripMenuItem contextEditCut;
        private System.Windows.Forms.ToolStripMenuItem contextEditCopy;
        private System.Windows.Forms.ToolStripMenuItem contextEditPaste;
        private System.Windows.Forms.ToolStripMenuItem contextEditFindReplace;
        private System.Windows.Forms.ToolStripMenuItem contextEditSelectAll;
        private System.Windows.Forms.ToolStripMenuItem contextEditDelete;
        private System.Windows.Forms.ToolStripMenuItem contextDocumentOpen;
        private System.Windows.Forms.ToolStripMenuItem contextDocumentSave;
        private System.Windows.Forms.ToolStripMenuItem contextDocumentPrint;
        private System.Windows.Forms.ToolStripMenuItem contextDocumentToolbar;
        private System.Windows.Forms.ToolStripMenuItem contextDocumentScrollbar;
        private System.Windows.Forms.ToolStripMenuItem contextDocumentWordwrap;
        private System.Windows.Forms.ToolStripMenuItem contextDocumentOverwrite;
        private System.Windows.Forms.ToolStripMenuItem contextFormatFontDialog;
        private System.Windows.Forms.ToolStripMenuItem contextFormatColorDialog;
        private System.Windows.Forms.ToolStripMenuItem contextFormatNormal;
        private System.Windows.Forms.ToolStripMenuItem contextFormatBold;
        private System.Windows.Forms.ToolStripMenuItem contextFormatItalic;
        private System.Windows.Forms.ToolStripMenuItem contextFormatUnderline;
        private System.Windows.Forms.ToolStripMenuItem contextFormatSuperscript;
        private System.Windows.Forms.ToolStripMenuItem contextFormatSubscript;
        private System.Windows.Forms.ToolStripMenuItem contextFormatStrikeout;
        private System.Windows.Forms.ToolStripMenuItem contextFormatIncrease;
        private System.Windows.Forms.ToolStripMenuItem contextFormatDecrease;
        private System.Windows.Forms.ToolStripMenuItem contextFormatIndent;
        private System.Windows.Forms.ToolStripMenuItem contextFormatOutdent;
        private System.Windows.Forms.ToolStripMenuItem contextFormatListOrdered;
        private System.Windows.Forms.ToolStripMenuItem contextFormatListUnordered;
        private System.Windows.Forms.ToolStripMenuItem contextJustifyLeft;
        private System.Windows.Forms.ToolStripMenuItem contextJustifyCenter;
        private System.Windows.Forms.ToolStripMenuItem contextJustifyRight;
        private System.Windows.Forms.ToolStripMenuItem contextInsertLine;
        private System.Windows.Forms.ToolStripMenuItem contextInsertLink;
        private System.Windows.Forms.ToolStripMenuItem contextInsertImage;
        private System.Windows.Forms.ToolStripMenuItem contextInsertText;
        private System.Windows.Forms.ToolStripMenuItem contextInsertHtml;
        private System.Windows.Forms.ToolStripMenuItem contextFormattingNormal;
        private System.Windows.Forms.ToolStripMenuItem contextFormattingHeading1;
        private System.Windows.Forms.ToolStripMenuItem contextFormattingHeading2;
        private System.Windows.Forms.ToolStripMenuItem contextFormattingHeading3;
        private System.Windows.Forms.ToolStripMenuItem contextFormattingHeading4;
        private System.Windows.Forms.ToolStripMenuItem contextFormattingHeading5;
        private System.Windows.Forms.ToolStripMenuItem contextFormattingFormatted;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator11;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator12;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator13;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator14;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator15;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator16;
        private System.Windows.Forms.ToolStripMenuItem contextInsertTable;
        private WebBrowserEx.WebBrowserEx editorWebBrowser;
		private System.Windows.Forms.ToolStripButton toolstripEnableEditing;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator17;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator18;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator21;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator19;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator20;
		private System.Windows.Forms.ToolStripButton toolstripDocumentNew;
		private System.Windows.Forms.ToolStripButton toolstripDocumentOpen;
		private System.Windows.Forms.ToolStripButton toolstripDocumentSave;
		private System.Windows.Forms.ToolStripButton toolstripDocumentPrint;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
		private System.Windows.Forms.ToolStripButton toolstripDocumentHelp;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator22;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator23;
		private System.Windows.Forms.ToolStripMenuItem contextTableRowModify;
		private System.Windows.Forms.ToolStripMenuItem contextTableCellModify;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator24;
		private System.Windows.Forms.ToolStripMenuItem contextDocumentNew;
		private System.Windows.Forms.ToolStripMenuItem contextDocumentHelp;
		private System.Windows.Forms.ToolStripButton toolstripDocumentEditHTML;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator25;
		private System.Windows.Forms.ToolStripMenuItem contextDocumentEditHTML;
		private System.Windows.Forms.ToolStripComboBox toolstripFontComboBox;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator26;
	}
}