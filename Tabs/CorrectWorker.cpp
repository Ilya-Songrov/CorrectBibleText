#include "CorrectWorker.h"

CorrectWorker::CorrectWorker()
{

}

void CorrectWorker::readCorrectFile(QVector<QPair<QString, QString> > *vecPair, const QString &pathFile)
{
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, pathFile);
    QJsonArray arrMain = doc.array();
    bool isFirstItem = true;
    for (const QJsonValue &value: arrMain) {
        QJsonObject objVerse = value.toObject();
        if (objVerse.value("correct").toString().isEmpty()) {
            if (isFirstItem) {
                isFirstItem = false;
                continue;
            }
            vecPair->append(qMakePair(objVerse.value("link").toString(), objVerse.value("not_correct").toString()));
        }
    }
    // The structure of the Correct file is described in the ContentWorker constructor.
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

void CorrectWorker::resetCorrectFile(const QString &pathFile)
{
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, pathFile);
    QJsonArray arrMain = doc.array();
    for (QJsonValueRef valueRef: arrMain) {
        QJsonObject objVerse = valueRef.toObject();
        if (!objVerse.value("correct").toString().isEmpty()) {
            objVerse.insert("correct", "");
            valueRef = objVerse;
        }
    }
    FileWorker::writeFileJson(QJsonDocument(arrMain), pathFile);
}

void CorrectWorker::mergeCorrectFile(QString pathCorrectFile, QString pathAllBibleFile)
{
    QJsonDocument docCorrect;
    FileWorker::readFileJson(&docCorrect, pathCorrectFile);
    QJsonArray arrMainCorrect = docCorrect.array();
    const QJsonObject objInfo = arrMainCorrect.at(0).toObject();
    if (pathAllBibleFile.isEmpty()) {
        pathAllBibleFile = objInfo.value("file_all_bible").toString();
    }
    for (QJsonValueRef valueRef: arrMainCorrect) {
        QJsonObject objVerse = valueRef.toObject();
        if (!objVerse.value("correct").toString().isEmpty()) {
//            objVerse.value("not_correct")
        }
    }
    // The structure of the Correct file is described in the ContentWorker constructor.
    // The structure of the AllBible file is described in the the ContentWorker constructor.


    QJsonDocument docAllBible;
    FileWorker::readFileJson(&docAllBible, pathAllBibleFile);
    QJsonArray arrMainAllBible = docCorrect.array();
}
