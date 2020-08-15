#pragma once

#include <QObject>
#include <QThread>

#include "TabBase.h"
#include "Dialogs/DialogSettingsAnalysis.h"
#include "AnalysisWorker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Analysis : public TabBase
{
    Q_OBJECT

    QThread threadAnalyze;
    QStandardItemModel *model;
    Ui::MainWindow *ui;
    QString fileAllBible;
    QString fileUrls;
    QString webTextCodec;
    QString fileOutputCorrect;
public:
    explicit Analysis(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Analysis();

protected:
    virtual void createConnects();

private slots:
    void setSettingsAnalysis();
    void startClicked();
    void refreshModel(QList<QStandardItem *> listItems);
    void refreshInfo(int quantityUrls, QString foundVerses);
    void onClickedListView();
    void saveResult();

signals:
    void startWorker(const QUrl url, const QString fileAllBible, const QString webTextCodec);
};

