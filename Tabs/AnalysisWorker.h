#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStandardItemModel>
#include <QTextDocument>
#include <QTextCodec>

#include "ContentWorker.h"

class AnalysisWorker : public QObject
{
    Q_OBJECT

    QVector<QPair<QString, QString> > vecPairVerses;
    QVector<QString> vecUrls;
    QString strReply;
    QString webTextCodec;
public:
    explicit AnalysisWorker(QObject *parent = nullptr);

public slots:
    void start(const QUrl url, const QString fileAllBible, const QString webTextCodec);

private:
    void readFileAllBible(const QString &fileAllBible);
    void parseReply();
private slots:
    void requestAllUrls();
    void slotSSLErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slotReplyAnalyze(QNetworkReply* reply);

signals:
    void analysisIsFinished(QString message = "");
    void analysisUpdated(QList<QStandardItem *> listItems);
    void analysisInfo(int quantityUrls, QString foundVerses);
};

