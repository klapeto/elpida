namespace Elpida.Mobile.Controls
{
	public partial class GroupBox 
	{
		public static readonly BindableProperty TitleProperty =
			BindableProperty.Create(nameof(Title), typeof(string), typeof(GroupBox), string.Empty);
		
		public GroupBox()
		{
			InitializeComponent();
		}

		public string Title
		{
			get => (string)GetValue(TitleProperty);
			set => SetValue(TitleProperty, value);
		}
	}
}