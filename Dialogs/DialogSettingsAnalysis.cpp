#include "DialogSettingsAnalysis.h"
#include "ui_DialogSettingsAnalysis.h"

DialogSettingsAnalysis::DialogSettingsAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingsAnalysis)
{
    ui->setupUi(this);
    connect(ui->toolButtonAllBibleFile, &QToolButton::clicked, this, [this](){ ui->lineEditAllBibleFilePath->setText(QFileDialog::getOpenFileName(this)); });
    connect(ui->toolButtonUrl, &QToolButton::clicked, this, [this](){ ui->lineEditUrlFilePath->setText(QFileDialog::getOpenFileName(this)); });
    connect(ui->toolButtonOutputFile, &QToolButton::clicked, this, [this](){ ui->lineEditOutputFilePath->setText(QFileDialog::getOpenFileName(this)); });
}

DialogSettingsAnalysis::~DialogSettingsAnalysis()
{
    delete ui;
}

void DialogSettingsAnalysis::setAllBibleFilePath(const QString &str)
{
    ui->lineEditAllBibleFilePath->setText(str);
}

void DialogSettingsAnalysis::setFileUrls(const QString &str)
{
    ui->lineEditUrlFilePath->setText(str);
}

void DialogSettingsAnalysis::setWebTextCodec(const QString &str)
{
    ui->lineEditTextCodec->setText(str);
}

void DialogSettingsAnalysis::setOutputFilePath(const QString &str)
{
    ui->lineEditOutputFilePath->setText(str);
}

QString DialogSettingsAnalysis::getAllBibleFilePath()
{
    return QFileInfo(ui->lineEditAllBibleFilePath->text()).absoluteFilePath();
}

QString DialogSettingsAnalysis::getFileUrls()
{
    return QFileInfo(ui->lineEditUrlFilePath->text()).absoluteFilePath();
}

QString DialogSettingsAnalysis::getWebTextCodec()
{
    return ui->lineEditTextCodec->text();
}

QString DialogSettingsAnalysis::getOutputFilePath()
{
    return QFileInfo(ui->lineEditOutputFilePath->text()).absoluteFilePath();
}
