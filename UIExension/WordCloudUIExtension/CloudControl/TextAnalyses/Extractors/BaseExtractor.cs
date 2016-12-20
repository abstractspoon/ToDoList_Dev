using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace Gma.CodeCloud.Controls.TextAnalyses.Extractors
{
    public abstract class BaseExtractor : IEnumerable<string>
    {
        protected BaseExtractor(IProgressIndicator progressIndicator)
        {
            ProgressIndicator = progressIndicator;
        }

        protected IProgressIndicator ProgressIndicator { get; set; }
        public abstract IEnumerable<string> GetWords();

        protected virtual IEnumerable<string> GetWords(string text)
        {
            StringBuilder word = new StringBuilder();
            foreach (char ch in text)
            {
                if (char.IsLetterOrDigit(ch))
                {
                    word.Append(ch);
                }
                else
                {
                    if (word.Length > 1)
                    {
                        yield return word.ToString();
                        OnWordProcessed(word);
                    }
                    word.Clear();
                }
                OnCharProcessed(ch);
            }
        }

        protected virtual void OnCharProcessed(char ch) { }
        protected virtual void OnWordProcessed(StringBuilder word) { }
        protected virtual void OnLineProcessed(string line) { }

        public IEnumerator<string> GetEnumerator()
        {
            return GetWords().GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}