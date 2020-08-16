#include "DialogMergeCorrectFile.h"
#include "ui_DialogMergeCorrectFile.h"

DialogMergeCorrectFile::DialogMergeCorrectFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMergeCorrectFile)
{
    ui->setupUi(this);
}

DialogMergeCorrectFile::~DialogMergeCorrectFile()
{
    delete ui;
}

void DialogMergeCorrectFile::setCorrectFilePath(const QString &file)
{
    ui->lineEdit_CorrectFile->setText(file);
}

void DialogMergeCorrectFile::setAllBibleFile(const QString &file)
{
    ui->lineEdit_AllBibleFile->setText(file);
}

QString DialogMergeCorrectFile::getCorrectFilePath()
{
    return ui->lineEdit_CorrectFile->text();
}

QString DialogMergeCorrectFile::getAllBibleFile()
{
    return ui->lineEdit_AllBibleFile->text();
}
