#include "Analysis.h"
#include "ui_mainwindow.h"

Analysis::Analysis(Ui::MainWindow *ui, QObject *parent) :
    TabBase(parent)
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
    connect(ui->pushButtonSettingsAnalysis, &QPushButton::clicked, this, &Analysis::setSettingsAnalysis);
    threadAnalyze.start();
}

void Analysis::setSettingsAnalysis()
{
    DialogSettingsAnalysis dialogSettings;
    if (dialogSettings.exec() == QDialog::Rejected) {
        return;
    }
    fileAllBible = dialogSettings.getAllBibleFilePath();
    fileUrls = dialogSettings.getFileUrls();
    webTextCodec = dialogSettings.getWebTextCodec();
    fileOutputCorrect = dialogSettings.getOutputFilePath();
}

void Analysis::startClicked()
{
//    if (ui->lineEditAnalysisUrl->text().isEmpty()) {
//        fileAllBible = QFileDialog::getOpenFileName();
//        if (fileAllBible.isEmpty()) {
//            QMessageBox::information(ui->listViewAnalysis, "", "Please enter url or file with urls");
//            return;
//        }
//    }
    const QUrl url = QUrl::fromUserInput(fileAllBible);
    fileAllBible = url.isLocalFile() ? url.toString() : ""; // if input is url, we will not be able to use auto-merge
    emit startWorker(url);
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
    ui->labelAnalysis->setText(QString("Remaining urls: %1\nFound verses: %2").arg(quantityUrls).arg(foundVerses));
}

void Analysis::onClickedListView()
{
    ui->textBrowserAnalysis->setText(ui->listViewAnalysis->currentIndex().data(Qt::UserRole+1).toString());
}

void Analysis::saveResult()
{
    QJsonArray arrMain;
    QJsonObject objInfo
    {
        { "file_all_bible", fileAllBible },
        { "file_correct",   fileOutputCorrect },
        { "file_urls",      fileUrls },
        { "web_text_codec", webTextCodec }
    };
    arrMain.append(objInfo);
    for (int var = 0; var < model->rowCount(); ++var) {
        QStandardItem *item = model->item(var, 0);
        QJsonObject objVerse;
        objVerse.insert("link", item->text());
        objVerse.insert("correct", "");
        objVerse.insert("not_correct", item->data(Qt::UserRole+1).toString());
        arrMain.append(objVerse);
#ifdef QT_DEBUG
        if (var > 50) {
            break;
        }
#endif
    }
#ifdef QT_DEBUG
    fileOutputCorrect = "/media/songrov/1478E91378E8F500/IlyaFolder/"
                        "Songrov_Ilya/Programming/QtProjects/CorrectBibleText/CorrectBibleText/Resource/ResultAnalysis.json";
#endif
    FileWorker::writeFileJson(QJsonDocument(arrMain), fileOutputCorrect);
}
