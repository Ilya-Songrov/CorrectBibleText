#include "Content.h"
#include "ui_mainwindow.h"

Content::Content(Ui::MainWindow *ui, QObject *parent) :
    TabBase(parent)
  , ui(ui)
{
    setGeneralSettings();
}

void Content::setGeneralSettings()
{
    connect(ui->pushButtonGenerateAllContent, &QPushButton::clicked, this, &Content::on_pushButtonGenerateAllContent_clicked);
    connect(ui->pushButtonRemoveAllContent, &QPushButton::clicked, this, &Content::on_pushButtonRemoveAllContent_clicked);
}

void Content::on_pushButtonGenerateAllContent_clicked()
{
    const QMessageBox::StandardButton button = QMessageBox::question(ui->pushButtonGenerateAllContent, "",
                                                                     "Are you sure you want to generate all content?");
    if (button == QMessageBox::StandardButton::No) {
        return;
    }
    ContentWorker::generateContentStandart(ContentWorker::Standard::EasternSynodal);
    QMessageBox::information(ui->pushButtonGenerateAllContent, "", "Content has generated");
}

void Content::on_pushButtonRemoveAllContent_clicked()
{
    const QMessageBox::StandardButton button = QMessageBox::question(ui->pushButtonGenerateAllContent, "",
                                                                     "Are you sure you want to remove all content?");
    if (button == QMessageBox::StandardButton::No) {
        return;
    }
    ContentWorker::removeAllContent();
    QMessageBox::information(ui->pushButtonRemoveAllContent, "", "Content has removed");
}
