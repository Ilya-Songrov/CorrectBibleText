#pragma once

#include <QDialog>

namespace Ui {
class DialogSettingsAnalysis;
}

class DialogSettingsAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettingsAnalysis(QWidget *parent = nullptr);
    ~DialogSettingsAnalysis();

    QString getAllBibleFilePath();
    QString getFileUrls();
    QString getWebTextCodec();
    QString getOutputFilePath();
private:
    Ui::DialogSettingsAnalysis *ui;
};

