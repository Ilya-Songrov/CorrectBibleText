#include "DialogSettingsAnalysis.h"
#include "ui_DialogSettingsAnalysis.h"

DialogSettingsAnalysis::DialogSettingsAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingsAnalysis)
{
    ui->setupUi(this);
}

DialogSettingsAnalysis::~DialogSettingsAnalysis()
{
    delete ui;
}

QString DialogSettingsAnalysis::getAllBibleFilePath()
{
    return ui->lineEditAllBibleFilePath->text();
}

QString DialogSettingsAnalysis::getFileUrls()
{
    return ui->lineEditUrl->text();
}

QString DialogSettingsAnalysis::getWebTextCodec()
{
    return ui->lineEditTextCodec->text();
}

QString DialogSettingsAnalysis::getOutputFilePath()
{
    return ui->lineEditOutputFilePath->text();
}
