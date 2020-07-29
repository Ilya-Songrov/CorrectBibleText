#include "Analysis.h"
#include "ui_mainwindow.h"

Analysis::Analysis(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent)
  , model(new QStandardItemModel)
  , ui(ui)
{
    createConnects();
}

Analysis::~Analysis()
{
    threadAnalyze.quit();
    threadAnalyze.wait();
}

void Analysis::createConnects()
{
    qRegisterMetaType<QList<QStandardItem*> >("QList<QStandardItem*>");
    AnalysisWorker *analysisWorker = new AnalysisWorker;
    analysisWorker->moveToThread(&threadAnalyze);
    connect(&threadAnalyze, &QThread::finished, analysisWorker, &QObject::deleteLater);
    connect(analysisWorker, &AnalysisWorker::analysisUpdated, this, &Analysis::refreshModel);
    connect(analysisWorker, &AnalysisWorker::analysisInfo, this, &Analysis::refreshInfo);
    connect(analysisWorker, &AnalysisWorker::analysisIsFinished, this, &Analysis::saveResult);
    connect(this, &Analysis::startWorker, analysisWorker, &AnalysisWorker::start);
    connect(ui->listViewAnalysis, &QListView::clicked, this, &Analysis::onClickedListView);
    connect(ui->pushButtonStartAnalysis, &QPushButton::clicked, this, &Analysis::startClicked);
    threadAnalyze.start();
}

void Analysis::startClicked()
{
    emit startWorker(QUrl::fromUserInput(ui->lineEditAnalysisUrl->text()));
}

void Analysis::refreshModel(QList<QStandardItem *> listItems)
{
    model->deleteLater();
    model = new QStandardItemModel;
    model->appendColumn(listItems);
    ui->listViewAnalysis->setModel(model);
    qDebug() << "updating model of analysis" << Qt::endl;
}

void Analysis::refreshInfo(int quantityUrls, QString foundVerses)
{
    ui->labelAnalysis->setText(QString("Urls: %1; found verses: %2").arg(quantityUrls).arg(foundVerses));
}

void Analysis::onClickedListView()
{
    ui->textBrowserAnalysis->setText(ui->listViewAnalysis->currentIndex().data(Qt::UserRole+1).toString());
}

void Analysis::saveResult()
{
    QJsonObject objMain;
    for (int var = 0; var < model->columnCount(); ++var) {
        QStandardItem *item = model->item(0, var);
        objMain.insert(item->text(), item->data(Qt::UserRole+1).toString());
    }
    FileWorker::writeFileJson(QJsonDocument(objMain), "/media/songrov/1478E91378E8F500/IlyaFolder/"
            "Songrov_Ilya/Programming/QtProjects/AccurateTranslationBible/AccurateTranslationBible/Resource/ResultAnalysis.json");
}
