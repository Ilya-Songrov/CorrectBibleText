#pragma once

#include <QObject>
#include <QMessageBox>
#include <QFileDialog>

class TabBase : public QObject
{
    Q_OBJECT
public:
    explicit TabBase(QObject *parent = nullptr);

protected:
    virtual void setGeneralSettings() = 0;
};

