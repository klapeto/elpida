#include "BenchmarkResultsView.hpp"
#include "ui_BenchmarkResultsView.h"

namespace Elpida {
namespace Application {

BenchmarkResultsView::BenchmarkResultsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BenchmarkResultsView)
{
    ui->setupUi(this);
}

BenchmarkResultsView::~BenchmarkResultsView()
{
    delete ui;
}

} // namespace Application
} // namespace Elpida
