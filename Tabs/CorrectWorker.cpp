#include "CorrectWorker.h"

CorrectWorker::CorrectWorker()
{

}

void CorrectWorker::readFileCorrect(QVector<QPair<QString, QString> > *vecPair, const QString &pathFile)
{
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, pathFile);
    QJsonArray arrMain = doc.array();
    for (const QJsonValue &value: arrMain) {
        QJsonObject objVerse = value.toObject();
        if (objVerse.value("correct").toString().isEmpty()) {
            vecPair->append(qMakePair(objVerse.value("link").toString(), objVerse.value("not_correct").toString()));
        }
    }
}

void CorrectWorker::setCorrectText(const QString &link, const QString &text, const QString &pathFile)
{
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, pathFile);
    QJsonArray arrMain = doc.array();
    for (QJsonValueRef valueRef: arrMain) {
        QJsonObject objVerse = valueRef.toObject();
        if (objVerse.value("link").toString() == link) {
            objVerse.insert("correct", text);
            valueRef = objVerse;
            break;
        }
    }
    FileWorker::writeFileJson(QJsonDocument(arrMain), pathFile);
}
