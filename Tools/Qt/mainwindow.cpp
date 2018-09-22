#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Elpida/CpuInfo.hpp"
#include "Elpida/Task.hpp"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(QSize(1650, 1350));
    loadCpuInfo();
    loadTaskInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCpuInfo(void)
{
    auto cpuInfo = Elpida::CpuInfo::getCpuInfo();
    ui->lblVendorValue->setText(cpuInfo.getVendorString().c_str());
    ui->lblModelValue->setText(cpuInfo.getProcessorBrand().c_str());
    ui->lblFamilyValue->setText(QString::number(cpuInfo.getFamily()));
    ui->lblSteppingValue->setText(QString::number(cpuInfo.getStepping()));
    ui->lblTscFreqValue->setText(QString::number(cpuInfo.getTscFequency()/1000000000.0f,'g',3) + QString(" GHZ"));
    ui->lblLogicalCoresValue->setText(QString::number(cpuInfo.getLogicalProcessors()));
    ui->chkHyperthreading->setChecked(cpuInfo.isHyperThreading());
    ui->chkTurbo->setChecked(cpuInfo.isTurboBoost());
    ui->chkTurbo3->setChecked(cpuInfo.isTurboBoost3());

    auto instructions = cpuInfo.getInstructionSetSupport();
    ui->gbCpuFreatures->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->chkCmov->setChecked(instructions["CMOV"].isSupported());
    ui->chkMmx->setChecked(instructions["MMX"].isSupported());
    ui->chkMmxExt->setChecked(instructions["MmxExt"].isSupported());
    ui->chkSse->setChecked(instructions["SSE"].isSupported());
    ui->chkSse2->setChecked(instructions["SSE2"].isSupported());
    ui->chkSse3->setChecked(instructions["SSE3"].isSupported());
    ui->chkSsse3->setChecked(instructions["SSSE3"].isSupported());
    ui->chkSse41->setChecked(instructions["SSE41"].isSupported());
    ui->chkSse42->setChecked(instructions["SSE42"].isSupported());
    ui->chkSse4a->setChecked(instructions["SSE4A"].isSupported());
    ui->chkFma->setChecked(instructions["FMA"].isSupported());
    ui->chkFma4->setChecked(instructions["FMA4"].isSupported());
    ui->chkAes->setChecked(instructions["AES"].isSupported());
    ui->chkAvx->setChecked(instructions["AVX"].isSupported());
    ui->chkAvx2->setChecked(instructions["AVX2"].isSupported());
    ui->chkAvx512->setChecked(instructions["AVX512-F"].isSupported());
    ui->chkAvx512bw->setChecked(instructions["AVX512-BW"].isSupported());
    ui->chkAvx512dq->setChecked(instructions["AVX512-DQ"].isSupported());
    ui->chkAvx512Fma->setChecked(instructions["AVX512-IFMA"].isSupported());
    ui->chkAvx512pf->setChecked(instructions["AVX512-PF"].isSupported());
    ui->chkAvx512er->setChecked(instructions["AVX512-ER"].isSupported());
    ui->chkAvx512cd->setChecked(instructions["AVX512-CD"].isSupported());
    ui->chkAvx512vbmi->setChecked(instructions["AVX512-VBMI"].isSupported());
    ui->chkSha->setChecked(instructions["SHA"].isSupported());
    ui->chkXop->setChecked(instructions["XOP"].isSupported());
    ui->chk3DNow->setChecked(instructions["3DNow"].isSupported());
    ui->chk3DNowext->setChecked(instructions["3DNowExt"].isSupported());
    ui->chk3DNowPre->setChecked(instructions["3DNowPrefetch"].isSupported());
    ui->chkBmi1->setChecked(instructions["BMI1"].isSupported());
    ui->chkBmi2->setChecked(instructions["BMI2"].isSupported());
    ui->chkAbm->setChecked(instructions["ABM"].isSupported());
    ui->chkF16c->setChecked(instructions["F16C"].isSupported());
    ui->chkRdrand->setChecked(instructions["RDRAND"].isSupported());

    auto caches = cpuInfo.getCaches();

    auto cache = caches.at(0);	// L1 Inst
    ui->lblL1InstSizeValue->setText(QString::number(cache.size / 1000) + " KB");
    ui->lblL1InstAssocValue->setText(cache.associativity.c_str());
    ui->lblL1InstLptValue->setText(QString::number(cache.linesPerTag));
    ui->lblL1InstLsValue->setText(QString::number(cache.lineSize));

    cache = caches.at(1);	// L1 Data
    ui->lblL1DataSizeValue->setText(QString::number(cache.size / 1000) + " KB");
    ui->lblL1DataAssocValue->setText(cache.associativity.c_str());
    ui->lblL1DataLptValue->setText(QString::number(cache.linesPerTag));
    ui->lblL1DataLsValue->setText(QString::number(cache.lineSize));

    cache = caches.at(2);	// L2
    ui->lblL2SizeValue->setText(QString::number(cache.size / 1000) + " KB");
    ui->lblL2AssocValue->setText(cache.associativity.c_str());
    ui->lblL2LptValue->setText(QString::number(cache.linesPerTag));
    ui->lblL2LsValue->setText(QString::number(cache.lineSize));

    cache = caches.at(3);	// L3
    ui->lblL3SizeValue->setText(QString::number(cache.size / 1000) + " KB");
    ui->lblL3AssocValue->setText(cache.associativity.c_str());
    ui->lblL3LptValue->setText(QString::number(cache.linesPerTag));
    ui->lblL3LsValue->setText(QString::number(cache.lineSize));


}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(ui->centralWidget, "About: Elpida","Elpida is an open source x86 Cpu/Algorithm benchmarking tool. It is released under the General Public License v3 (GPL v3). More info at: https://github.com/klapeto/elpida");
}

void MainWindow::loadTaskInfo()
{
//    auto taskBatches = TaskRepository::getTaskBatches();

//    for (auto taskbatch :taskBatches){
//        QTreeWidgetItem* batchItem = new QTreeWidgetItem(ui->twTasks);
//        batchItem->setText(0,taskbatch.getName().c_str());
//        batchItem->setCheckState(1, Qt::CheckState::Checked);
//        auto tasks = taskbatch.getTasks();

//        for (auto task: tasks){
//            QTreeWidgetItem* taskItem = new QTreeWidgetItem(batchItem);
//            //batchItem->addChild(taskItem);
//            taskItem->setText(0, task->getName().c_str());
//            taskItem->setCheckState(1, Qt::CheckState::Checked);
//        }

//    }

}
