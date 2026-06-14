using CommunityToolkit.Mvvm.Input;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.PageModels;

public interface IProjectTaskPageModel
{
	IAsyncRelayCommand<ProjectTask> NavigateToTaskCommand { get; }
	bool IsBusy { get; }
}