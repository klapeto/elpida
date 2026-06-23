using System.Text;

namespace Elpida.Mobile.Converters
{
	public class Utilities
	{
		public static string GetValueScaleString(double value,
			double[] denominators,
			string[] prefixes,
			int decimals)
		{
			var sb = new StringBuilder();
			
			var i = denominators.Length - 1;

			while (i > 0)
			{
				if (value >= denominators[i])
				{
					break;
				}

				i--;
			}

			sb.Append((value / denominators[i]).ToString($"F{decimals}"));
			sb.Append(' ');
			sb.Append(prefixes[i]);
			
			return sb.ToString();
		}
	}
}