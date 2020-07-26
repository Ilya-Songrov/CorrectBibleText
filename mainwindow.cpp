#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonGenerateAllContent_clicked()
{
    Content::generateContentStandart(Content::Standard::EasternSynodal, Content::ProviderContent::GETBIBLE_NET);
    QMessageBox::information(this, "", "Content has generated");
}

void MainWindow::on_pushButtonRemoveAllContent_clicked()
{
    Content::removeAllContent();
    QMessageBox::information(this, "", "Content has removed");
}

void MainWindow::setSettings()
{
    resize(855, 455);
    ui->tabWidget->widget(0)->setStyleSheet("background-color: rgba(28, 54, 109, 100);");
    ui->tabWidget->widget(1)->setStyleSheet("background-color: rgba(50, 85, 7, 150);");
#ifdef QT_DEBUG
    ui->tabWidget->setCurrentIndex(0);
#endif
}

void MainWindow::on_pushButtonAnalyze_clicked()
{
    QVector<QPair<QString, QString> > vecPair;
    Content::getVectorVersesAllBible(&vecPair, Content::ProviderContent::GETBIBLE_NET);
    QStandardItemModel *model = new QStandardItemModel(ui->listViewTranslation);
    model->setItem();
    ui->listViewTranslation->setModel(model);
    Q_ASSERT(vecPair.size() > 0);
    int step = 0;
    for (const QPair<QString, QString> pair: vecPair) {
//        QAction *action = new QAction(pair.first, ui->listViewTranslation);
//        action->setData(pair.second);
//        ui->listViewTranslation->addAction(action);
        if (++step > 200) {
            break;
        }
    }
    qDebug() << "finish" << Qt::endl;
}






