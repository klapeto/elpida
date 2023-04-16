#ifndef ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
#define ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP

#include <QWidget>

namespace Elpida {
namespace Application {

namespace Ui {
class BenchmarkResultsView;
}

class BenchmarkResultsView : public QWidget
{
    Q_OBJECT

public:
    explicit BenchmarkResultsView(QWidget *parent = nullptr);
    ~BenchmarkResultsView();

private:
    Ui::BenchmarkResultsView *ui;
};


} // namespace Application
} // namespace Elpida
#endif // ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
