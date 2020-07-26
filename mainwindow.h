#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>

#include "Content.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonGenerateAllContent_clicked();
    void on_pushButtonRemoveAllContent_clicked();

    void on_pushButtonAnalyze_clicked();

private:
    void setSettings();
    Ui::MainWindow *ui;
};
