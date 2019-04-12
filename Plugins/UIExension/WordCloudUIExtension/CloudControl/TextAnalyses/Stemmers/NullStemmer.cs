using System.Globalization;
using Gma.CodeCloud.Controls.TextAnalyses.Stemmers;

namespace Gma.CodeCloud.Controls.TextAnalyses
{
    public class NullStemmer : IWordStemmer
    {
        public string GetStem(string word)
        {
            return word.ToLower(CultureInfo.InvariantCulture);
        }
    }
}
