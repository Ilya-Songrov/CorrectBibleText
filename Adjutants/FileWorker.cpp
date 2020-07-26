#include "FileWorker.h"

FileWorker::FileWorker()
{

}

void FileWorker::readFileJson(QJsonDocument *doc, const QString &path)
{
    QJsonParseError error;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error ReadOnly file" << path << Qt::endl;
        return;
    }
    *doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();
}

QJsonDocument FileWorker::readFileJson(const QString &path)
{
    QJsonDocument doc;
    readFileJson(&doc, path);
    return doc;
}

void FileWorker::writeFileJson(const QJsonDocument &doc, const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)){
        qDebug() << "Error WriteOnly file" << path << Qt::endl;
        return;
    }
    file.write(doc.toJson());
    file.close();
}

void FileWorker::readFile(QByteArray *arr, const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error ReadOnly file" << path << Qt::endl;
        return;
    }
    *arr = file.readAll();
    file.close();
}

void FileWorker::writeFile(const QByteArray &arr, const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)){
        qDebug() << "Error WriteOnly file" << path << Qt::endl;
        return;
    }
    file.write(arr);
    file.close();
}

