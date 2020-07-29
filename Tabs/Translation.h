#pragma once

#include <QObject>
#include <QStandardItemModel>

#include "Content.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Translation : public QObject
{
    Q_OBJECT

    Ui::MainWindow *ui;
public:
    explicit Translation(Ui::MainWindow *ui, QObject *parent = nullptr);

public slots:
    void on_pushButtonLoad_clicked();
};

