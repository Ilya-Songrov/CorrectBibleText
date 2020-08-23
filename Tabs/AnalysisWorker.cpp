#include "AnalysisWorker.h"

AnalysisWorker::AnalysisWorker(QObject *parent) : QObject(parent)
{
    connect(this, &AnalysisWorker::requestNextUrl, this, &AnalysisWorker::requestAllUrls);
}

void AnalysisWorker::start(const QString fileUrls, const QString fileAllBible, const QString webTextCodec)
{
    abort = false;
    this->webTextCodec = webTextCodec;
    readFileAllBible(fileAllBible);

    QFile file(fileUrls);
    if (!file.open(QFile::ReadOnly)) {
        const QString message = "Error, file is not open " + file.fileName();
        qWarning() << message << Qt::endl;
        emit analysisIsFinished(message);
        return;
    }
    while (!file.atEnd()) {
        QByteArray lineArr = file.readLine();
        if (lineArr.isEmpty()){
            continue;
        }
        int lenRemove { 0 };
        for (int var = 0; var < lineArr.size(); ++var) {
            if (lineArr.at(var) != '-') {
                break;
            }
            ++lenRemove;
        }
        lineArr.remove(0, lenRemove);
        lineArr.remove(lineArr.size() - 1, lineArr.back() == '\n' ? 1 : 0);
        vecUrls.append(lineArr);
    }
    file.close();
    requestAllUrls();
}

void AnalysisWorker::slotAbort()
{
    this->abort = true;
}

void AnalysisWorker::readFileAllBible(const QString &fileAllBible)
{
    ContentWorker::getVectorVersesAllBible(&vecPairVerses, fileAllBible);
    Q_ASSERT(vecPairVerses.size() > 0);
}

void AnalysisWorker::parseReply()
{
    QList<QStandardItem *> listItems;
    QString foundVerses;
    for (QPair<QString, QString> &pair: vecPairVerses) {
        if (pair.first.isEmpty() || strReply.contains(pair.second)) {
            if (!pair.first.isEmpty()) {
                foundVerses.append(pair.first + ",");
                pair.first.clear();
            }
            continue;
        }
        QStandardItem *item = new QStandardItem(pair.first);
        item->setData(pair.second);
        listItems.append(item);
    }
    qDebug() << "listItems" << listItems.size() << Qt::endl;
    emit analysisUpdated(listItems);
    emit analysisInfo(vecUrls.size(), foundVerses);
}

void AnalysisWorker::requestAllUrls()
{
    if (vecUrls.isEmpty() || abort) {
        emit analysisIsFinished();
        return;
    }
    QUrl url(vecUrls.last());
    vecUrls.pop_back();
    qDebug() << Qt::endl << vecUrls.size() << "request:" << url << Qt::endl;
    static QNetworkAccessManager mngrAnalyze;
    static bool notConnected { true };
    if (notConnected) {
        connect(&mngrAnalyze, &QNetworkAccessManager::sslErrors, this, &AnalysisWorker::slotSSLErrors);
        connect(&mngrAnalyze, &QNetworkAccessManager::finished, this, &AnalysisWorker::slotReplyAnalyze);
        notConnected = false;
    }
    QNetworkRequest request(url);
    mngrAnalyze.get(request);
}

void AnalysisWorker::slotSSLErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qDebug() << "QSslError: " << errors << Qt::endl;
    reply->ignoreSslErrors(errors);
    emit requestNextUrl();
}

void AnalysisWorker::slotReplyAnalyze(QNetworkReply *reply)
{
    QByteArray byteArr = reply->readAll();
    QTextCodec* textCodec = QTextCodec::codecForName(webTextCodec.toLocal8Bit());
    strReply = textCodec->toUnicode(byteArr);

    QTextDocument doc;
    doc.setHtml(strReply);

    strReply = doc.toPlainText().simplified();
    qDebug() << "strReply" << strReply << Qt::endl;

    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError){
        qWarning() << "reply->error()" << reply->error() << Qt::endl;
        emit requestNextUrl();
        return;
    }
    parseReply();
    emit requestNextUrl();
}
