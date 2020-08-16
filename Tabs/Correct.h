#pragma once

#include <QObject>
#include <QStandardItemModel>

#include "TabBase.h"
#include "CorrectWorker.h"
#include "DialogMergeCorrectFile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Correct : public TabBase
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QString pathCorrectFile;
public:
    explicit Correct(Ui::MainWindow *ui, QObject *parent = nullptr);

protected:
    virtual void createConnects();

public slots:
    void on_pushButtonLoadCorrect_clicked();
    void on_pushButtonSaveCorrect_clicked();
    void onClickedViewCorrect();
    void on_actionReset_CorrectFile_triggered();
    void on_actionMerge_CorrectFile_triggered();

private:
    void updateListViewCorrect();
};

