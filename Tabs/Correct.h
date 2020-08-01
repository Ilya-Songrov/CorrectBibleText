#pragma once

#include <QObject>
#include <QStandardItemModel>

#include "TabBase.h"
#include "CorrectWorker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Correct : public TabBase
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QString pathFileCorrect;
public:
    explicit Correct(Ui::MainWindow *ui, QObject *parent = nullptr);

protected:
    virtual void createConnects();

public slots:
    void on_pushButtonLoadCorrect_clicked();
    void on_pushButtonSaveCorrect_clicked();
    void onClickedViewCorrect();

};

