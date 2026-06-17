namespace Elpida.Mobile.Models
{
	public class OsInfoModel
	{
		public string Category { get; }
		public string Name { get; }
		public string Version { get; }

		public OsInfoModel(string category, string name, string version)
		{
			Category = category;
			Name = name;
			Version = version;
		}
	}
}