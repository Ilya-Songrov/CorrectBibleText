#include "Analysis.h"
#include "ui_mainwindow.h"

Analysis::Analysis(Ui::MainWindow *ui, QObject *parent) :
    TabBase(parent)
  , model(new QStandardItemModel)
  , ui(ui)
  , settings("IlyaOrganization", qApp->applicationName())
{
    setGeneralSettings();
    loadPreviousSettings();
}

Analysis::~Analysis()
{
    saveSettings();
    threadAnalyze.quit();
    threadAnalyze.wait();
}

void Analysis::setGeneralSettings()
{
    ui->progressBarAnalysis->setValue(0);
    ui->pushButtonStartAnalysis->setText("start");

    qRegisterMetaType<QList<QStandardItem*> >("QList<QStandardItem*>");
    AnalysisWorker *analysisWorker = new AnalysisWorker;
    analysisWorker->moveToThread(&threadAnalyze);
    connect(&threadAnalyze, &QThread::finished, analysisWorker, &QObject::deleteLater);
    connect(analysisWorker, &AnalysisWorker::analysisUpdated, this, &Analysis::refreshModel);
    connect(analysisWorker, &AnalysisWorker::analysisInfo, this, &Analysis::refreshInfo);
    connect(analysisWorker, &AnalysisWorker::analysisIsFinished, this, &Analysis::saveResult);
    connect(this, &Analysis::startWorker, analysisWorker, &AnalysisWorker::start);
    connect(this, &Analysis::abortWorker, analysisWorker, &AnalysisWorker::slotAbort);
    connect(ui->listViewAnalysis, &QListView::clicked, this, &Analysis::onClickedListView);
    connect(ui->pushButtonStartAnalysis, &QPushButton::clicked, this, &Analysis::startClicked);
    connect(ui->pushButtonSettingsAnalysis, &QPushButton::clicked, this, &Analysis::setSettingsAnalysis);
    threadAnalyze.start();
}

void Analysis::saveSettings()
{
    qDebug() << "saveSettings" << Qt::endl;
    settings.setValue("fileAllBible",       fileAllBible);
    settings.setValue("fileUrls",           fileUrls);
    settings.setValue("webTextCodec",       webTextCodec);
    settings.setValue("fileOutputCorrect",  fileOutputCorrect);
}

void Analysis::loadPreviousSettings()
{
    fileAllBible         = settings.value("fileAllBible"     ).toString();
    fileUrls             = settings.value("fileUrls"         ).toString();
    webTextCodec         = settings.value("webTextCodec"     ).toString();
    fileOutputCorrect    = settings.value("fileOutputCorrect").toString();
}

int Analysis::setSettingsAnalysis()
{
    DialogSettingsAnalysis dialogSettings;
    dialogSettings.setAllBibleFilePath(fileAllBible );
    dialogSettings.setFileUrls(fileUrls);
    dialogSettings.setWebTextCodec(webTextCodec);
    dialogSettings.setOutputFilePath(fileOutputCorrect);
    int dialogCode = dialogSettings.exec();
    if (dialogCode == QDialog::Rejected) {
        return dialogCode;
    }
    fileAllBible        = dialogSettings.getAllBibleFilePath();
    fileUrls            = dialogSettings.getFileUrls();
    webTextCodec        = dialogSettings.getWebTextCodec();
    fileOutputCorrect   = dialogSettings.getOutputFilePath();
    return dialogCode;
}

void Analysis::startClicked()
{
    if (fileAllBible.isEmpty() || fileUrls.isEmpty() || webTextCodec.isEmpty() || fileOutputCorrect.isEmpty()) {
        int dialogCode = setSettingsAnalysis();
        if (dialogCode == QDialog::Rejected) {
            return;
        }
    }
    rangeProgressBar = 0;
    ui->pushButtonStartAnalysis->setText(ui->pushButtonStartAnalysis->text() == "start" ? "stop" : "start");
    if (ui->pushButtonStartAnalysis->text() == "stop") { // because when you press the start button its name will change to stop
        ui->labelAnalysis->setText("Status: Please wait...");
        emit startWorker(fileUrls, fileAllBible, webTextCodec);
        return;
    }
    emit abortWorker();
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
    if (rangeProgressBar == 0) {
        rangeProgressBar = quantityUrls;
        ui->progressBarAnalysis->setRange(0, rangeProgressBar);
    }
    ui->progressBarAnalysis->setValue(rangeProgressBar - quantityUrls);
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
        { "file_all_bible", QFileInfo(fileAllBible).absoluteFilePath() },
        { "file_correct",   QFileInfo(fileOutputCorrect).absoluteFilePath() },
        { "file_urls",      QFileInfo(fileUrls).absoluteFilePath() },
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
    }
    FileWorker::writeFileJson(QJsonDocument(arrMain), fileOutputCorrect);
    ui->labelAnalysis->setText(QString("Incorrect verses: %1").arg(model->rowCount()));
    QMessageBox::information(ui->listViewAnalysis, "", QString("The analysis is over. The result is written to the %1 file.").
                                                                arg(QFileInfo(fileOutputCorrect).fileName()));
}



