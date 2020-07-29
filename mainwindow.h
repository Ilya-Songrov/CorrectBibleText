#pragma once

#include <QMainWindow>
#include <QMessageBox>

#include "Content.h"
#include "Analysis.h"
#include "Translation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Translation *translation;
    Analysis *analysis;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotTabBarChanged(int index);
    void on_pushButtonGenerateAllContent_clicked();
    void on_pushButtonRemoveAllContent_clicked();

private:
    void setSettings();
    Ui::MainWindow *ui;
};
