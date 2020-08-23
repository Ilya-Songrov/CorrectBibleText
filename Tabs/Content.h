#pragma once

#include <QObject>

#include "TabBase.h"
#include "ContentWorker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Content : public TabBase
{
    Q_OBJECT

    Ui::MainWindow *ui;
public:
    Content(Ui::MainWindow *ui, QObject *parent = nullptr);

protected:
    virtual void setGeneralSettings();

private slots:
    void on_pushButtonGenerateAllContent_clicked();
    void on_pushButtonRemoveAllContent_clicked();
};

