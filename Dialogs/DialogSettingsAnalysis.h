#pragma once

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class DialogSettingsAnalysis;
}

class DialogSettingsAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettingsAnalysis(QWidget *parent = nullptr);
    ~DialogSettingsAnalysis();

    void setAllBibleFilePath(const QString &str);
    void setFileUrls(const QString &str);
    void setWebTextCodec(const QString &str);
    void setOutputFilePath(const QString &str);

    QString getAllBibleFilePath();
    QString getFileUrls();
    QString getWebTextCodec();
    QString getOutputFilePath();
private:
    Ui::DialogSettingsAnalysis *ui;
};

