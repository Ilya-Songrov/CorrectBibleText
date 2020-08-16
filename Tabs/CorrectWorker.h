#pragma once

#include <QObject>

#include "FileWorker.h"

class CorrectWorker
{
public:
    CorrectWorker();

    static void readCorrectFile(QVector<QPair<QString, QString> > *vecPair, const QString &pathFile);
    static void setCorrectText(const QString &link, const QString &text, const QString &pathFile);
    static void resetCorrectFile(const QString &pathFile);
    static void mergeCorrectFile(QString pathCorrectFile, QString pathAllBibleFile);
    static QString getPathAllBible(const QString &pathCorrectFile);

private:
    static void parseLink(QString *abbrev, int *chapter, int *verse, const QString &link);
};

