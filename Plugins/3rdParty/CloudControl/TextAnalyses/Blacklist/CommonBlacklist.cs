using System;
using System.Collections.Generic;
using System.IO;

namespace Gma.CodeCloud.Controls.TextAnalyses.Blacklist
{
    public class CommonBlacklist : IBlacklist
    {
		private readonly HashSet<string> m_ExcludedWordsHashSet;

		public CommonBlacklist()
			: this(new string[] { })
		{
		}

		public CommonBlacklist(IEnumerable<string> excludedWords)
			: this(excludedWords, StringComparer.InvariantCultureIgnoreCase)
		{
		}


		public static IBlacklist CreateFromTextFile(string fileName)
		{
			if (!File.Exists(fileName))
				return new NullBlacklist();

			// else
			return CreateFromStreamReader(new FileInfo(fileName).OpenText());
		}

		public static IBlacklist CreateFromStreamReader(TextReader reader)
		{
			CommonBlacklist commonBlacklist = new CommonBlacklist();

			commonBlacklist.Append(reader);

			return commonBlacklist;
		}

		public int Append(TextReader reader)
		{
			if (reader == null)
				throw new ArgumentNullException("reader");

			int numWords = m_ExcludedWordsHashSet.Count;

			using (reader)
			{
				string line = reader.ReadLine();
				while (line != null)
				{
					line = line.Trim();

					// allow comment lines
					if ((line.Length > 0) && !line.StartsWith("//"))
						Add(line);

					line = reader.ReadLine();
				}
			}

			return (m_ExcludedWordsHashSet.Count - numWords);
		}

		public int Append(string fileName)
		{
			if (!File.Exists(fileName))
				return 0;

			return Append(new FileInfo(fileName).OpenText());
		}

		public CommonBlacklist(IEnumerable<string> excludedWords, StringComparer comparer)
		{
			m_ExcludedWordsHashSet = new HashSet<string>(excludedWords, comparer);
		}

        public bool Countains(string word)
        {
            return m_ExcludedWordsHashSet.Contains(word);
        }

        public void Add(string line)
        {
            m_ExcludedWordsHashSet.Add(line);
        }

        public int Count
        {
            get { return m_ExcludedWordsHashSet.Count; }
        }
    }
}