#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

class FileWorker
{

public:
    explicit FileWorker();

    static void readFileJson(QJsonDocument *doc, const QString &path);
    static QJsonDocument readFileJson(const QString &path);
    static void writeFileJson(const QJsonDocument &doc, const QString &path);

    static void readFile(QByteArray *arr, const QString &path);
    static void writeFile(const QByteArray &arr, const QString &path);
};
