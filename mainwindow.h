#pragma once

#include <QMainWindow>
#include <QMessageBox>

#include "Correct.h"
#include "Analysis.h"
#include "Content.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Correct *correct;
    Analysis *analysis;
    Content *content;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotTabBarChanged(int index);

private:
    void setSettings();
    Ui::MainWindow *ui;
};
