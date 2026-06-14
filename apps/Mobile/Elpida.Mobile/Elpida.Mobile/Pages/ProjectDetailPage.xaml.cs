using Elpida.Mobile.Models;

namespace Elpida.Mobile.Pages;

public partial class ProjectDetailPage : ContentPage
{
	public ProjectDetailPage(ProjectDetailPageModel model)
	{
		InitializeComponent();

		BindingContext = model;
	}
}