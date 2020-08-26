#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QApplication>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QUrl>
#include <QTextCodec>

#include "Data.h"
#include "FileWorker.h"


class ContentWorker
{
    enum FamilyBooks { // Standard::Western
        Pentateuch = 5,
        HistoricalBooks = 17,
        Wisdom = 22,
        MajorProphets = 27,
        MinorProphets = 39,

        GospelsAndActs = 44,
        PaulineEpistles = 58,
        CatholicEpistles = 65,
        Apocalypse = 66,
    };
public:
    enum Standard {
        Western,
        EasternSynodal,
    };

    explicit ContentWorker();
    static void generateContentStandart(const ContentWorker::Standard requiredStandart);
    static void removeAllContent();
    static void getVectorVersesAllBible(QVector<QPair<QString, QString> > *vecPair, const QString &pathFileAllBible);
private:
    static QString getIndexBookStr(const int index);
    static QString getIndexBookStr(int index, const BibleEnums::Testament testament,
                                   const ContentWorker::Standard currentStandart, const ContentWorker::Standard requiredStandart);
    static int getIndexBook(int index, const BibleEnums::Testament testament,
                            const ContentWorker::Standard currentStandart, const ContentWorker::Standard requiredStandart);
    static void testGetIndexBook();

    static void generateContent_Photos();
    static QStringList getListFileInDirectory(const QString &dir);
    static void fillPhotos(QJsonObject *objBook, const QString &pathDir);

    static void generateContent_Folders(const bool onlyRemove = false);
    static void generateAllBibleJsonFromTxt_GETBIBLE();
    static void generateContent_Info_GETBIBLE(const ContentWorker::Standard requiredStandart);
    static void generateContent_JsonText_GETBIBLE(const ContentWorker::Standard requiredStandart);
    static void generateContent_JsonText(const QString &pathFrom, const QString &dirWhere,
                        const ContentWorker::Standard currentStandart, const ContentWorker::Standard requiredStandart);

    static QString getAbbrev(const int index, const ContentWorker::Standard requiredStandart, const QString &language = "ru");
    static QVector<QString> getVectorFamilyBooks(const FamilyBooks familyBooks, const QString &language = "ru");
    static QString getNameFamilyBooks(const FamilyBooks familyBooks, const QString &language = "ru");
};

