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
    // The structure of the Correct file is described in the ContentWorker constructor.
    // The structure of the AllBible file is described in the the ContentWorker constructor.
    QJsonDocument docCorrect;
    FileWorker::readFileJson(&docCorrect, pathCorrectFile);
    QJsonArray arrMainCorrect = docCorrect.array();

    QJsonDocument docAllBible;
    FileWorker::readFileJson(&docAllBible, pathAllBibleFile);
    if (docAllBible.isEmpty()) {
        qDebug() << QString("Error, %1 file is empty").arg(QFileInfo(pathAllBibleFile).fileName()) << Qt::endl;
        return;
    }
    QJsonArray arrMainAllBible = docAllBible.array();
    Q_ASSERT(arrMainAllBible.size() > 0);

    for (const QJsonValue &value: arrMainCorrect) {
        QJsonObject objVerse = value.toObject();
        QString strCorrect = objVerse.value("correct").toString();
        if (!strCorrect.isEmpty()) {
            QString abbrev;
            int chapter { 0 };
            int verse   { 0 };
            parseLink(&abbrev, &chapter, &verse, objVerse.value("link").toString());
            auto itBook = std::find_if(arrMainAllBible.begin(), arrMainAllBible.end(), [abbrev](const QJsonValue &book){ return book["abbrev"].toString() == abbrev; });
            if (itBook != std::end(arrMainAllBible)) {
                QJsonObject objBook = itBook->toObject();
                QJsonArray arrChapters = objBook.value("chapters").toArray();
                QJsonArray arrCorrectChapter = arrChapters[chapter].toArray();
                arrCorrectChapter[verse] = QJsonValue(strCorrect);
                arrChapters[chapter] = arrCorrectChapter;
                objBook["chapters"] = arrChapters;
                *itBook = objBook;
            }
        }
    }
    FileWorker::writeFileJson(QJsonDocument(arrMainAllBible), pathAllBibleFile);
}

QString CorrectWorker::getPathAllBible(const QString &pathCorrectFile)
{
    QJsonDocument docCorrect;
    FileWorker::readFileJson(&docCorrect, pathCorrectFile);
    QJsonArray arrMainCorrect = docCorrect.array();
    const QJsonObject objInfo = arrMainCorrect.at(0).toObject();
    return objInfo.value("file_all_bible").toString();
}

void CorrectWorker::parseLink(QString *abbrev, int *chapter, int *verse, const QString &link)
{
    QStringList list = link.split(' ');
    if (list.size() > 1) {
        *abbrev = list.at(0);
        QStringList listChapterVerse = list.back().split(':');
        if (listChapterVerse.size() > 1) {
            *chapter = listChapterVerse.at(0).toUInt() - 1;
            *verse = listChapterVerse.at(1).toUInt() - 1;
        }
    }
}
