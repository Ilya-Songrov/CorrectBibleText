#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , correct(nullptr)
    , analysis(nullptr)
    , content(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotTabWidgetChanged(int index)
{
    if (index == 0 && !correct) {
        correct = new Correct(ui, this);
    }
    else if (index == 1 && !analysis) {
        analysis = new Analysis(ui, this);
    }
    else if (index == 2 && !content) {
        content = new Content(ui, this);
    }
}

void MainWindow::setSettings()
{
    resize(855, 455);
    ui->tabWidget->widget(0)->setStyleSheet("background-color: rgba(28, 54, 109, 100);");
    ui->tabWidget->widget(1)->setStyleSheet("background-color: rgba(28, 54, 45, 100);");
    ui->tabWidget->widget(2)->setStyleSheet("background-color: rgba(50, 85, 7, 150);");

    connect(ui->actionExitApp, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::slotTabWidgetChanged);
    const int currentTab { 0 };
    ui->tabWidget->setCurrentIndex(currentTab);
    slotTabWidgetChanged(currentTab);
}

