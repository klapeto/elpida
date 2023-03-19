#ifndef ELPIDA_OSINFOVIEW_HPP
#define ELPIDA_OSINFOVIEW_HPP

#include <QWidget>

namespace Ui
{
	class OsInfoView;
}

namespace Elpida
{
	class OsInfo;
}

class OsInfoView : public QWidget
{
Q_OBJECT

public:
	explicit OsInfoView(const Elpida::OsInfo& osInfo, QWidget* parent = nullptr);

	~OsInfoView() override;

private:
	Ui::OsInfoView* _ui;
};

#endif // ELPIDA_OSINFOVIEW_HPP
