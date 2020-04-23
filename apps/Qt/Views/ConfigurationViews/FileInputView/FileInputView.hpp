#ifndef ELPIDA_FILEINPUTVIEW_HPP
#define ELPIDA_FILEINPUTVIEW_HPP

#include <QWidget>
#include "Views/ConfigurationViews/ConfigurationViewBase.hpp"

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

		explicit FileInputView();
		~FileInputView() override;
	private:
		Ui::FileInputView* _ui;
		ConfigurationValueBase* _configurationValue;
	};

} // namespace Elpida
#endif // ELPIDA_FILEINPUTVIEW_HPP
