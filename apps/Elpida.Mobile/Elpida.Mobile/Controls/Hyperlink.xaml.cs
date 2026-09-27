namespace Elpida.Mobile.Controls
{
	public partial class Hyperlink : Label
	{
		public static readonly BindableProperty UrlProperty =
			BindableProperty.Create(nameof(Url), typeof(string), typeof(Hyperlink), null);

		public string Url
		{
			get => (string)GetValue(UrlProperty);
			set => SetValue(UrlProperty, value);
		}

		public Hyperlink()
		{
			TextDecorations = TextDecorations.Underline;
			TextColor = Colors.Blue;
			GestureRecognizers.Add(new TapGestureRecognizer
			{
				Command = new Command(async () => await Launcher.OpenAsync(Url))
			});
		}
	}
}