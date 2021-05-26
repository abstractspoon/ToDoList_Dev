using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

using Microsoft.Win32;

using iTextSharp;
using iTextSharp.text;
using iTextSharp.text.html;
using iTextSharp.text.html.simpleparser;
using iTextSharp.text.pdf;

namespace iTextDemo
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();

			RegistryKey key = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\AbstractSpoon\iTextDemo");

			if (key != null)
			{
				textHtmlInput.Text = key.GetValue("HTMLInputFile", "").ToString();

				CreatePDF(textHtmlInput.Text);
			}
		}

		protected override void OnClosed(EventArgs e)
		{
			RegistryKey key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\AbstractSpoon\iTextDemo");

			if (key != null)
				key.SetValue("HTMLInputFile", textHtmlInput.Text);

			base.OnClosed(e);
		}

		private void OnBrowseInputFile(object sender, EventArgs e)
		{
			var dlg = new OpenFileDialog
			{
				Title = this.Text,

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,
				Filter = "HTML Files (*.htm, *.html)|*.htm;*.html",
				FilterIndex = 0,
				FileName = textHtmlInput.Text
			};

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				textHtmlInput.Text = dlg.FileName;

				CreatePDF(textHtmlInput.Text);
			}
		}

		private void CreatePDF(string htmlInputFile)
		{
			webBrowserPDF.Navigate("about:blank");

			if (string.IsNullOrEmpty(htmlInputFile))
				return;

			try
			{
				MemoryStream workStream = new MemoryStream();
				Document pdfDoc = new Document(PageSize.A4);
				PdfWriter pdfWriter = PdfWriter.GetInstance(pdfDoc, workStream);
				pdfWriter.CloseStream = false;
//				pdfWriter.PageEvent = new PDFBackgroundImage(m_WatermarkImagePath);

				pdfDoc.Open();

				string htmlInput = File.ReadAllText(htmlInputFile);

				//string FixBrokenMarkup(string broken)
				var h = new HtmlAgilityPack.HtmlDocument()
				{
					OptionAutoCloseOnEnd = true,
					OptionFixNestedTags = true,
					OptionWriteEmptyNodes = true
				};
				h.LoadHtml(htmlInput);

				// UPDATED to remove HtmlCommentNode
				var comments = h.DocumentNode.SelectNodes("//comment()");
				if (comments != null)
				{
					foreach (var node in comments)
						node.Remove();
				}

				htmlInput = h.DocumentNode.SelectNodes("child::*") != null
						//                            ^^^^^^^^^^
						// XPath above: string plain-text or contains markup/tags
						? h.DocumentNode.WriteTo()
						: string.Format("<span>{0}</span>", htmlInput);

// 				using (var sr = new StringReader(htmlInput))
// 				{
// 					//Parse the HTML
// 					//htmlWorker.Parse(sr);
// 					var parser = iTextSharp.tool.xml.XMLWorkerHelper.GetInstance();
// 					parser.ParseXHtml(pdfWriter, pdfDoc, sr);
// 				}

				Section section = new Chapter(0);
				AddContent(htmlInput, section);
				
				pdfDoc.Add(section);
				pdfDoc.Close();

				byte[] byteInfo = workStream.ToArray();
				workStream.Write(byteInfo, 0, byteInfo.Length);
				workStream.Position = 0;

				string pdfOutputFile = Path.ChangeExtension(htmlInputFile, "pdf");

				File.WriteAllBytes(pdfOutputFile, byteInfo);

				Uri pdfLink;

				if (Uri.TryCreate(pdfOutputFile, UriKind.Absolute, out pdfLink))
					webBrowserPDF.Navigate(pdfLink);
			}
			catch (Exception e)
			{
			}
		}

		private void AddContent(string html, Section section)
		{
			var elements = iTextSharp.tool.xml.XMLWorkerHelper.ParseToElementList(html, "");
			//var elements = HTMLWorker.ParseToList(new StringReader(html), new StyleSheet()/*m_ParseStyles*/);

			foreach (IElement element in elements)
			{
				var elementType = element.ToString();

				section.Add(element);
/*
//				if (element.IsContent())
				{
					var para = new Paragraph();

					foreach (var obj in element.Chunks)
					{
						var chunk = (obj as Chunk);
						var objType = obj.ToString();

						if (chunk != null)
						{
// 							var font = chunk.Font;
// 
// 							if (font == null)
// 								font = new Font(m_BaseFont, m_BaseFontSize);
// 							else
// 								font = new Font(m_BaseFont, font.Size, font.Style, font.Color);
// 
// 							chunk.Font = font;
							para.Add(chunk);
						}
						else
						{
							para.Add(obj);
						}
					}

					section.Add(para);
				}
// 				else
// 				{
// 					section.Add(element);
// 				}
*/
			}
		}

		private void btnParse_Click(object sender, EventArgs e)
		{
			CreatePDF(textHtmlInput.Text);
		}
	}
}
