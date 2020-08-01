#pragma once

#include <QObject>

#include "FileWorker.h"

class CorrectWorker
{
public:
    CorrectWorker();

    static void readFileCorrect(QVector<QPair<QString, QString> > *vecPair, const QString &pathFile);
    static void setCorrectText(const QString &link, const QString &text, const QString &pathFile);
};

