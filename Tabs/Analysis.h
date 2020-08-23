#pragma once

#include <QObject>
#include <QThread>
#include <QSettings>

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
    QSettings settings;
    QString fileAllBible;
    QString fileUrls;
    QString webTextCodec;
    QString fileOutputCorrect;
    int rangeProgressBar;
public:
    explicit Analysis(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Analysis();

protected:
    virtual void setGeneralSettings();

private:
    void saveSettings();
    void loadPreviousSettings();
private slots:
    int setSettingsAnalysis();
    void startClicked();
    void refreshModel(QList<QStandardItem *> listItems);
    void refreshInfo(int quantityUrls, QString foundVerses);
    void onClickedListView();
    void saveResult();

signals:
    void startWorker(const QString fileUrls, const QString fileAllBible, const QString webTextCodec);
    void abortWorker();

};

