namespace Elpida.Mobile.Controls
{
	public partial class GroupBox 
	{
		public static readonly BindableProperty TitleProperty =
			BindableProperty.Create(nameof(Title), typeof(string), typeof(GroupBox), string.Empty);
		
		public static readonly BindableProperty IconProperty =
			BindableProperty.Create(nameof(Icon), typeof(string), typeof(GroupBox), string.Empty);
		
		public string Icon
		{
			get => (string)GetValue(IconProperty);
			set => SetValue(IconProperty, value);
		}
		
		public bool ShowIcon => !string.IsNullOrEmpty(Icon);
		
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