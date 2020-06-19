using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms.VisualStyles;

using System.IO;
using System.Reflection;
using System.Xml;
using System.Globalization;
using System.Security.Cryptography;
using System.Security.Cryptography.Xml;

namespace Abstractspoon.Tdl.PluginHelpers
{
	class RhinoLicensing
	{
		static String PUBLIC_KEY  = "<RSAKeyValue><Modulus>9twJpwt/Ofe58BOdK5Cb8XKGP5bvgxGh3IYkvCqvdzOCH3pi9BvOX+/fsRo/7HFbNmPr3Txu+hBl1JVH9ACXDxm20oKqgl6TzIk33iV6SrbuiZASi1OPAiTmsWBGKTIwrG9KiQ8JGmBotV/v2gRflqKELwiMUOO9W2DlgJ6szq0=</Modulus><Exponent>AQAB</Exponent></RSAKeyValue>";
		static String ALL_MODULES = "00000000-0000-0000-0000-000000000000";
		static char USERID_DELIM  = ':';

		public enum LicenseType
		{
			Free,
			Trial,
			Paid,
			Supporter,
			Contributor,
		};

		public static LicenseType GetLicense(String typeId)
		{
			String licType;

			// Paid/free licenses take priority
			if (HasLicense(typeId, out licType))
			{
				switch (licType)
				{
					case "Free": return LicenseType.Free;
					case "Paid": return LicenseType.Paid;
				}
			}
			else if (HasLicense(ALL_MODULES, out licType))
			{
				switch (licType)
				{
					case "Supported": return LicenseType.Supporter;
					case "Contributor": return LicenseType.Contributor;
				}
			}

			// all else
			return LicenseType.Trial;
		}

		private static bool HasLicense(String typeId, out String licType)
		{
			licType = String.Empty;

			String appFolder = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
			String licenseFolder = Path.Combine(appFolder, "Resources\\Licenses");

			if (Directory.Exists(licenseFolder))
			{
				var searchPattern = ("*" + typeId + ".xml"); // total flexibility
				var licenseFiles = Directory.EnumerateFiles(licenseFolder, searchPattern, SearchOption.AllDirectories);
				var attributes = new Dictionary<String, String>();

				foreach (String licenseFile in licenseFiles)
				{
					if (!CheckLicense(PUBLIC_KEY, licenseFile, attributes))
						continue;

					// Validate attributes
					String licId, expiryDate, ownerId, pluginName, pluginId;

					if (!attributes.TryGetValue("id", out licId) ||
						!attributes.TryGetValue("expiration", out expiryDate) ||
						!attributes.TryGetValue("type", out licType) ||
						!attributes.TryGetValue("owner_id", out ownerId) ||
						!attributes.TryGetValue("plugin_name", out pluginName) ||
						!attributes.TryGetValue("plugin_id", out pluginId))
					{
						continue;
					}

					if (!pluginId.Equals(typeId))
						continue;

					String userName, domainName, computerName;

					if (!DecodeUserID(ownerId, out userName, out domainName, out computerName))
						continue;

					if (!VerifyUserIDs(userName, domainName, computerName))
						continue;

					return true;
				}
			}


			return false;
		}

		public static bool CheckLicense(String publicKey, String licensePath, /*out*/ Dictionary<String, String> attributes)
		{
			if (String.IsNullOrEmpty(publicKey) || String.IsNullOrEmpty(licensePath) || !System.IO.File.Exists(licensePath))
				return false;

			var licenseText = File.ReadAllText(licensePath);

			var doc = new XmlDocument();
			doc.LoadXml(licenseText);

			var rsa = new RSACryptoServiceProvider();
			rsa.FromXmlString(publicKey);

			var nsMgr = new XmlNamespaceManager(doc.NameTable);
			nsMgr.AddNamespace("sig", "http://www.w3.org/2000/09/xmldsig#");

			var signedXml = new SignedXml(doc);
			var sig = (XmlElement)doc.SelectSingleNode("//sig:Signature", nsMgr);

			if (sig == null)
			{
				//Log.WarnFormat("Could not find this signature node on license:\r\n{0}", License);
				return false;
			}

			signedXml.LoadXml(sig);

			if (!signedXml.CheckSignature(rsa))
				return false;

			// Extract attributes
			var license = doc.SelectSingleNode("/license");
			int att = license.Attributes.Count;

			while (att-- > 0)
			{
				var attrib = license.Attributes[att];

				attributes[attrib.Name] = attrib.Value;
			}

			return true;
		}

		private static bool TestUserIds()
		{
			var userId = EncodeUserID();

			String userName, domainName, computerName;

			if (!DecodeUserID(userId, out userName, out domainName, out computerName))
				return false;

			if (!VerifyUserIDs(userName, domainName, computerName))
				return false;

			return true;
		}

		private static void GetUserIDs(out String userName, out String domainName, out String computerName)
		{
			userName = Environment.UserName;
			domainName = Environment.UserDomainName;
			computerName = Environment.MachineName;
		}

		public static String EncodeUserID()
		{
			String userName, domainName, computerName;
			GetUserIDs(out userName, out domainName, out computerName);

			String userId = (userName + USERID_DELIM + computerName);

			if (!String.IsNullOrEmpty(domainName))
				userId = (userId + USERID_DELIM + domainName);

			var bytes = Encoding.UTF8.GetBytes(userId);
			Array.Reverse(bytes);

			return Convert.ToBase64String(bytes);
		}

		private static bool DecodeUserID(String userId, out String userName, out String domainName, out String computerName)
		{
			var bytes = Convert.FromBase64String(userId);
			Array.Reverse(bytes);

			var userIds = Encoding.UTF8.GetString(bytes).Split(USERID_DELIM);

			switch (userIds.Count())
			{
				case 2:
					userName = userIds[0];
					computerName = userIds[1];
					domainName = String.Empty;
					break;

				case 3:
					userName = userIds[0];
					computerName = userIds[1];
					domainName = userIds[2];
					break;

				default:
					userName = String.Empty;
					computerName = String.Empty;
					domainName = String.Empty;
					return false;
			}

			return true;
		}

		private static bool VerifyUserIDs(String userName, String domainName, String computerName)
		{
			String user, domain, computer;
			GetUserIDs(out user, out domain, out computer);

			return (user.Equals(userName) && domain.Equals(domainName) && computer.Equals(computerName));
		}

		// Returns the visible height of the banner
		public static int CreateBanner(String typeId, String uiName, Control parent, Translator trans, int dollarPrice)
		{
			if (parent == null)
				return 0;

			// Add once per parent
			var banner = GetBanner(parent);

			if (banner == null)
			{
				banner = new Banner(typeId, uiName, trans, dollarPrice);

				banner.Location = new Point(0, 0);
				banner.Size = new Size(parent.ClientSize.Width, banner.Height);

				parent.Controls.Add(banner);
			}

			return banner.Size.Height;
		}

		public static void SetUITheme(Control bannerParent, UITheme theme)
		{
			var banner = GetBanner(bannerParent);

			if (banner != null)
				banner.SetUITheme(theme);
		}

		private static Banner GetBanner(Control bannerParent)
		{
			if (bannerParent != null)
			{
				foreach (var ctrl in bannerParent.Controls)
				{
					if (ctrl is Banner)
						return (ctrl as Banner);
				}
			}

			return null;
		}

        public static int GetBannerHeight(Control bannerParent)
        {
            if (bannerParent != null)
            {
                foreach (Control ctrl in bannerParent.Controls)
                {
                    if (ctrl is Banner)
                        return ctrl.Height;
                }
            }

            return 0;
        }

        private class LinkLabelEx : LinkLabel
		{
			private const int WM_SETCURSOR = 0x0020;

			[DllImport("user32.dll", CharSet = CharSet.Auto)]
			private static extern IntPtr SetCursor(IntPtr hCursor);

			protected override void WndProc(ref Message m)
			{
				if (m.Msg == WM_SETCURSOR)
				{
					// Set the cursor to use the system hand cursor
					SetCursor(UIExtension.HandCursor().Handle);

					// Indicate that the message has been handled
					m.Result = IntPtr.Zero;
					return;
				}

				base.WndProc(ref m);
			}
		}

		private class Banner : Label
		{
			static String PAYPAL_URL = @"https://www.paypal.com/cgi-bin/webscr?cmd=_xclick&business=abstractspoon2%40optusnet%2ecom%2eau&item_name={0}({1})&amount={2}";

			// ---------------------------------------------

			private String m_TypeId, m_UiName;
			private RhinoLicensing.LicenseType m_LicenseType;
			private Translator m_Trans;
			private LinkLabelEx m_buyBtn;
            private ToolTip m_buyBtnTooltip;
			private int m_DollarPrice = 0;
			private UITheme m_Theme;
			private Color m_ThemedBorderColor; // we draw it
			
			// ---------------------------------------------

			public Banner(String typeId, String uiName, Translator trans, int dollarPrice)
			{
				m_TypeId = typeId;
				m_UiName = trans.Translate(uiName);
				m_Trans = trans;
				m_DollarPrice = dollarPrice;
				m_Theme = new UITheme();

				if (m_DollarPrice < 0)
					m_LicenseType = LicenseType.Free;
				else
					m_LicenseType = RhinoLicensing.GetLicense(typeId);

				InitializeComponent();
			}

			public void SetUITheme(UITheme theme)
			{
				m_Theme = theme;

				RefreshColors();
				FixupBorder();
			}

			private void InitializeComponent()
			{
				Text = LicenseString;
				Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
				TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

				if ((m_LicenseType != RhinoLicensing.LicenseType.Free) &&
					(m_LicenseType != RhinoLicensing.LicenseType.Paid) &&
					(m_DollarPrice > 0))
				{
					m_buyBtn = new LinkLabelEx();
					m_buyBtn.Text = String.Format("{0} (USD{1})", m_Trans.Translate("Buy"), m_DollarPrice);
					m_buyBtn.Anchor = AnchorStyles.Top | AnchorStyles.Right;
					m_buyBtn.Height = Height;
					m_buyBtn.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
					m_buyBtn.LinkClicked += new LinkLabelLinkClickedEventHandler(OnBuyLicense);

					this.Controls.Add(m_buyBtn);

                    m_buyBtnTooltip = new ToolTip();
                    m_buyBtnTooltip.SetToolTip(m_buyBtn, "https://www.paypal.com");
				}

				RefreshColors();
				FixupBorder();
			}

			private void OnBuyLicense(object sender, LinkLabelLinkClickedEventArgs e)
			{
				System.Diagnostics.Process.Start(FormatPaypalUrl());
			}

			protected override void OnPaint(PaintEventArgs e)
			{
				// Always draw ourselves to handle disability
				TextRenderer.DrawText(e.Graphics, Text, Font, ClientRectangle, ForeColor, TextFormatFlags.Left | TextFormatFlags.VerticalCenter);
			}

			protected override void OnPaintBackground(PaintEventArgs e)
			{
				switch (m_LicenseType)
				{
					case RhinoLicensing.LicenseType.Free:
					case RhinoLicensing.LicenseType.Supporter:
					case RhinoLicensing.LicenseType.Contributor:
						{
							var drawRect = Bounds;

							if (m_ThemedBorderColor != Color.Empty)
							{
								using (var brush = new SolidBrush(m_ThemedBorderColor))
									e.Graphics.FillRectangle(brush, drawRect);

								drawRect.Inflate(-1, -1);
							}

							UITheme.DrawHorizontalBar(e.Graphics,
														drawRect,
														m_Theme.GetAppDrawingColor(UITheme.AppColor.StatusBarLight),
														m_Theme.GetAppDrawingColor(UITheme.AppColor.StatusBarDark),
														m_Theme.GetRenderStyle());
						}
						break;

					default:
					case RhinoLicensing.LicenseType.Paid:
					case RhinoLicensing.LicenseType.Trial:
						base.OnPaintBackground(e);
						break;
				}
			}

			public new int Height
            {
                get
                {
                    if (m_LicenseType == RhinoLicensing.LicenseType.Paid)
                        return 0;

                    return PluginHelpers.DPIScaling.Scale(22); // Allows for border on Classic theme
                }
            }

			private String LicenseString
			{
				get
				{
					String license = String.Empty;

					switch (m_LicenseType)
					{
						case RhinoLicensing.LicenseType.Free:
							license = m_Trans.Translate("Free License");
							break;

						case RhinoLicensing.LicenseType.Trial:
							if (m_DollarPrice > 0)
								license = m_Trans.Translate("Trial License");
							else
								license = m_Trans.Translate("Under Development");
							break;

						case RhinoLicensing.LicenseType.Supporter:
							license = m_Trans.Translate("Supporter License");
							break;

						case RhinoLicensing.LicenseType.Contributor:
							license = m_Trans.Translate("Contributor License");
							break;

						case RhinoLicensing.LicenseType.Paid:
						default:
							return String.Empty;
					}

					if (string.IsNullOrEmpty(m_UiName))
						return license;

					// else
					return String.Format("{0} ({1})", license, m_UiName);
				}
			}

			private void RefreshColors()
			{
				BackColor = SystemColors.ButtonFace;
				ForeColor = SystemColors.WindowText;

                Color PressedColor = ForeColor;

				switch (m_LicenseType)
				{
					case RhinoLicensing.LicenseType.Free:
					case RhinoLicensing.LicenseType.Supporter:
					case RhinoLicensing.LicenseType.Contributor:
						PressedColor = Color.DimGray;
						ForeColor = m_Theme.GetAppDrawingColor(UITheme.AppColor.StatusBarText);
						break;

					case RhinoLicensing.LicenseType.Paid:
						break;

					case RhinoLicensing.LicenseType.Trial:
					default:
						BackColor = Color.DarkRed;
                        PressedColor = Color.LightPink;
						ForeColor = ColorUtil.DrawingColor.GetBestTextColor(BackColor);
						break;
				}


				if (m_buyBtn != null)
				{
					m_buyBtn.BackColor = BackColor;
					m_buyBtn.LinkColor = ForeColor;
                    m_buyBtn.ActiveLinkColor = PressedColor;
                    m_buyBtn.VisitedLinkColor = PressedColor;
				}
			}

			private String FormatPaypalUrl()
			{
				return String.Format(PAYPAL_URL, m_TypeId, RhinoLicensing.EncodeUserID(), m_DollarPrice);
			}

            // --------------------------------------------------------

            private const int WM_THEMECHANGED = 0x031A;

			[DllImport("user32.dll", CharSet = CharSet.Auto)]
			private static extern IntPtr SetCursor(IntPtr hCursor);

			protected override void WndProc(ref Message m)
			{
                if (m.Msg == WM_THEMECHANGED)
                    FixupBorder();

				base.WndProc(ref m);
			}

            private void FixupBorder()
            {
				if (!VisualStyleRenderer.IsSupported)
				{
					BorderStyle = BorderStyle.Fixed3D;
					return;
				}

				// Disable built-in border
				BorderStyle = BorderStyle.None;

				// Add border if (nearly) gray or the colour 
				// is too close to the parent background colour
				Color borderColor = Color.Empty;

				Color parentColor = m_Theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
				Color bannerColor = m_Theme.GetAppDrawingColor(UITheme.AppColor.StatusBarLight);

				bool wantThemedBorder = ((bannerColor.GetSaturation() < 0.2f) ||
										(ColorUtil.DrawingColor.CalculateColorDifference(bannerColor, parentColor) < 0.2f));

				if (wantThemedBorder)
				{
					if (ColorUtil.DrawingColor.GetLuminance(parentColor) > 0.5f)
						borderColor = m_Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);
					else
						borderColor = m_Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesLight);
				}

				if (borderColor != m_ThemedBorderColor)
				{ 
					m_ThemedBorderColor = borderColor;
					Invalidate();
				}
            }
		}
	}

}
