#pragma once

#include <QObject>
#include <QThread>

#include "AnalysisWorker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Analysis : public QObject
{
    Q_OBJECT

    QThread threadAnalyze;
    QStandardItemModel *model;
    Ui::MainWindow *ui;
public:
    explicit Analysis(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Analysis();

public slots:
    void createConnects();

private slots:
    void startClicked();
    void refreshModel(QList<QStandardItem *> listItems);
    void refreshInfo(int quantityUrls, QString foundVerses);
    void onClickedListView();
    void saveResult();

signals:
    void startWorker(QUrl url);
};

