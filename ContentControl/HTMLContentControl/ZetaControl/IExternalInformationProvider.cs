namespace ZetaHtmlEditControl
{
    using System.Drawing;

    public sealed class TextModuleInfo
	{
		public string Name { get; set; }
		public string Html { get; set; }
	}

	public interface IExternalInformationProvider
	{
		void SavePerUserPerWorkstationValue(
			string name,
			string value );

		string RestorePerUserPerWorkstationValue(
			string name,
			string fallBackTo );

		/// <summary>
		/// Return NULL to disable text module support.
		/// </summary>
		TextModuleInfo[] GetTextModules();

        Font Font { get; }
	}
}