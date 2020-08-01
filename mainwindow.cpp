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

void MainWindow::slotTabBarChanged(int index)
{
    if (index == 0 && !correct) {
        correct = new Correct(ui);
    }
    else if (index == 1 && !analysis) {
        analysis = new Analysis(ui);
    }
    else if (index == 2 && !content) {
        content = new Content(ui);
    }
}

void MainWindow::setSettings()
{
    resize(855, 455);
    ui->tabWidget->widget(0)->setStyleSheet("background-color: rgba(28, 54, 109, 100);");
    ui->tabWidget->widget(1)->setStyleSheet("background-color: rgba(28, 54, 45, 100);");
    ui->tabWidget->widget(2)->setStyleSheet("background-color: rgba(50, 85, 7, 150);");

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::slotTabBarChanged);
    const int currentTab { 0 };
    ui->tabWidget->setCurrentIndex(currentTab);
    slotTabBarChanged(currentTab);
#ifdef QT_DEBUG
//    ui->lineEditAnalysisUrl->setText("https://allbible.info/bible/sinodal/ge/1/");
//    ui->lineEditAnalysisUrl->setText("/media/songrov/1478E91378E8F500/IlyaFolder/Songrov_Ilya/Programming/"
//                    "QtProjects/CorrectBibleText/CorrectBibleText/Resource/AllbibleSynodalMap_little.txt");
#endif
}

