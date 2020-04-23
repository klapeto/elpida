#ifndef ELPIDA_FILEINPUTVIEW_HPP
#define ELPIDA_FILEINPUTVIEW_HPP

#include <QWidget>
#include "Views/ConfigurationViews/ConfigurationViewBase.hpp"
#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>

namespace Elpida
{

	namespace Ui
	{
		class FileInputView;
	}

	class FileInputView : public ConfigurationViewBase
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
		ConfigurationValue<std::string>* _configurationValue;

	public slots:
		void onEditingFinished();
		void onBrowseClicked(bool checked);
	};

} // namespace Elpida
#endif // ELPIDA_FILEINPUTVIEW_HPP
