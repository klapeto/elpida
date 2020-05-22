#ifndef ELPIDA_NUMBERINPUTVIEW_HPP
#define ELPIDA_NUMBERINPUTVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include <Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp>

namespace Elpida
{

	namespace Ui
	{
		class NumberInputView;
	}

	class NumberInputView : public ConfigurationValueViewBase
	{
	Q_OBJECT

	public:

		void setConfiguration(ConfigurationValueBase* configurationValue) override;
		ConfigurationValueBase* getConfiguration() override;
		void saveSetting() override;

		explicit NumberInputView();
		~NumberInputView() override;
	private:
		Ui::NumberInputView* _ui;
		ConfigurationValueBase* _configurationValue;
		double _currentRawValue;
		bool _updating;
		double getScaledValue() const;
		void reInitializePrefix();

	private slots:
		void onStandardChanged(int state);
		void onScaleChanged(int state);
		void onValueChanged(double newValue);

		void setValue(double value);
		double calculateScaledValue(int index, const double* scaleValues, size_t size) const;
	};


} // namespace Elpida
#endif // ELPIDA_NUMBERINPUTVIEW_HPP
