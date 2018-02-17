using System;
using System.Collections.Generic;
using System.Linq;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;
using Gma.CodeCloud.Controls.TextAnalyses.Stemmers;

namespace Gma.CodeCloud.Controls.TextAnalyses.Processing
{
    public static class WordExtensions
    {
        public static IEnumerable<WordGroup> GroupByStem(this IEnumerable<IWord> words, IWordStemmer stemmer)
        {
            return
                words.GroupBy(
                    word => stemmer.GetStem(word.Text), 
                    (stem, sameStemWords) => new WordGroup(stem, sameStemWords));
            
        }

        public static IOrderedEnumerable<T> SortByOccurrences<T>(this IEnumerable<T> words) where T : IWord
        {
            return 
                words.OrderByDescending(
                    word => word.Occurrences);
        }

		public static IOrderedEnumerable<T> SortByText<T>(this IEnumerable<T> words) where T : IWord
		{
			return
				words.OrderBy(
					word => word.Text);
		}

		public static int MaxOccurrences<T>(this IEnumerable<T> words) where T : IWord
		{
			int max = 0;

			foreach (var word in words)
			{
				max = System.Math.Max(max, word.Occurrences);
			}

			return max;
		}

		public static int MinOccurrences<T>(this IEnumerable<T> words) where T : IWord
		{
			int min = System.Int32.MaxValue;

			foreach (var word in words)
			{
				min = System.Math.Min(min, word.Occurrences);
			}

			return min;
		}

        public static IEnumerable<int> UniqueOccurrences<T>(this IEnumerable<T> words) where T : IWord
        {
            var occurrences = new SortedSet<int>();

            foreach (var word in words)
                occurrences.Add(word.Occurrences);

            return occurrences;
        }

        public static IEnumerable<IWord> CountOccurences(this IEnumerable<string> terms)
        {
            return 
                terms.GroupBy(
                    term => term,
                    (term, equivalentTerms) => new Word(term, equivalentTerms.Count()), 
                    StringComparer.InvariantCultureIgnoreCase)
                    .Cast<IWord>();
        }

        public static IEnumerable<string> FilterTerms(this IEnumerable<string> terms, IBlacklist blacklist)
        {
            return
                terms.Where(
                    term => !blacklist.Countains(term));
        }

		public static IEnumerable<T> Filter<T>(this IEnumerable<T> words, int minOccurrences) where T : IWord
		{
			return
				words.Where(
					word => (word.Occurrences >= minOccurrences));
		}
    }
}