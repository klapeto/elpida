#ifndef ELPIDA_FILEINPUTVIEW_HPP
#define ELPIDA_FILEINPUTVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>

namespace Elpida
{

	namespace Ui
	{
		class FileInputView;
	}

	class FileInputView : public ConfigurationValueViewBase
	{
	Q_OBJECT

	public:
		void setConfiguration(ConfigurationValueBase* configurationValue) override;
		ConfigurationValueBase* getConfiguration() override;
		void saveSetting() override;

		explicit FileInputView();
		~FileInputView() override;
	private:
		Ui::FileInputView* _ui;
		ConfigurationValue<ConfigurationType::FilePath>* _configurationValue;

	public slots:
		void onEditingFinished();
		void onBrowseClicked(bool checked);
	};

} // namespace Elpida
#endif // ELPIDA_FILEINPUTVIEW_HPP
