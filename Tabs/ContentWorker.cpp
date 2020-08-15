#include "ContentWorker.h"


QNetworkAccessManager ContentWorker::mngr(nullptr);


ContentWorker::ContentWorker()
{
    // Correct file
    //    [
    //        {
    //            "file_all_bible": "",
    //            "file_correct": "",
    //            "file_urls": "",
    //            "web_text_codec": ""
    //        },
    //        {
    //            "correct": "",
    //            "link": "Быт 1:3",
    //            "not_correct": "И сказал Бог: да будет свет. И сталсвет."
    //        },
    //        {
    //        ...
    //        },
    //    ]

    // AllBible file
    //    [
    //        {
    //        "abbrev" : "abbrev"
    //        "name" : "book"
    //        "chapters":
    //            [
    //                ["Verse 1", "Verse 2", "Verse 3", "..."],
    //                ["Verse 1", "Verse 2", "Verse 3", "..."],
    //                ["Verse 1", "Verse 2", "Verse 3", "..."]
    //            ]
    //        },
    //        {
    //        ...
    //        },
    //    ]
}

void ContentWorker::generateContentStandart(const ContentWorker::Standard requiredStandart, const ProviderContent providerContent)
{
    testGetIndexBook();
    generateContent_Folders(providerContent);
    if (providerContent == GITHUB_BIBLE_XML_AND_JSON_THIAGO_BODRUK) {
        generateContent_Info_BODRUK(requiredStandart);
        generateContent_JsonText_BODRUK(requiredStandart);
    }
    if(providerContent == BIBLEONLINE_RU){
        //        getOnlineBookList();
        //        generateContent_Info_ONLINE(standart);
        //        getOnlineBible();
        //        generateContent_JsonText_ONLINE(standart);
    }
    if(providerContent == GITHUB_BIBLE_USFM2JSON_RUSLAN_KAZAKOV){
        generateValidJson_KAZAKOV();
        generateContent_Info_KAZAKOV(requiredStandart);
        generateContent_JsonText_KAZAKOV(requiredStandart);
    }
    if(providerContent == GETBIBLE_NET){
        generateJsonFromTxt_GETBIBLE();
        generateContent_Info_GETBIBLE(requiredStandart);
        generateContent_JsonText_GETBIBLE(requiredStandart);
    }
    generateContent_TwoArraysBooks(providerContent);
}

void ContentWorker::removeAllContent()
{
    generateContent_Folders(GETBIBLE_NET, true);
    generateContent_Folders(GITHUB_BIBLE_XML_AND_JSON_THIAGO_BODRUK, true);
    generateContent_Folders(GITHUB_BIBLE_USFM2JSON_RUSLAN_KAZAKOV, true);
    generateContent_Folders(BIBLEONLINE_RU, true);
    QFile::remove(Path::fileContent_New_Testament_Info_BODRUK);
    QFile::remove(Path::fileContent_New_Testament_Info_KAZAKOV);
    QFile::remove(Path::fileContent_New_Testament_Info_ONLINE);
    QFile::remove(Path::fileContent_New_Testament_Info_GETBIBLE);
    QFile::remove(Path::fileContent_Old_Testament_Info_BODRUK);
    QFile::remove(Path::fileContent_Old_Testament_Info_KAZAKOV);
    QFile::remove(Path::fileContent_Old_Testament_Info_ONLINE);
    QFile::remove(Path::fileContent_Old_Testament_Info_GETBIBLE);
    QFile::remove(Path::fileContent_New_Testament_ArrayBooks_BODRUK);
    QFile::remove(Path::fileContent_New_Testament_ArrayBooks_KAZAKOV);
    QFile::remove(Path::fileContent_New_Testament_ArrayBooks_ONLINE);
    QFile::remove(Path::fileContent_New_Testament_ArrayBooks_GETBIBLE);
    QFile::remove(Path::fileContent_Old_Testament_ArrayBooks_BODRUK);
    QFile::remove(Path::fileContent_Old_Testament_ArrayBooks_KAZAKOV);
    QFile::remove(Path::fileContent_Old_Testament_ArrayBooks_ONLINE);
    QFile::remove(Path::fileContent_Old_Testament_ArrayBooks_GETBIBLE);
    QFile::remove(Path::tempJson);
}

void ContentWorker::getVectorVersesAllBible(QVector<QPair<QString, QString> > *vecPair, const QString &pathFileAllBible)
{
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, pathFileAllBible);
    QJsonArray arrMainBooks = doc.array();
    Q_ASSERT(arrMainBooks.size() == BibleEnums::Old_Testament + BibleEnums::New_Testament);

    for (int var = 0; var < arrMainBooks.size(); ++var) {
        QJsonObject objBook = arrMainBooks.at(var).toObject();
        QJsonArray arrChapters = objBook.value("chapters").toArray();
        const QString abbrev { objBook.value("abbrev").toString() };
        for (int varChapter = 0; varChapter < arrChapters.size(); ++varChapter) {
            QJsonArray arrVerses = arrChapters.at(varChapter).toArray();
            for (int varVerse = 0; varVerse < arrVerses.size(); ++varVerse) {
                vecPair->append(qMakePair(QString("%1 %2:%3").arg(abbrev).arg(varChapter + 1).arg(varVerse + 1),
                                          arrVerses.at(varVerse).toString()));
            }
        }
    }
    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

void ContentWorker::generateContent_Photos()
{
    const int Old_New_Testament { 2 };
    for (int var = 0; var < Old_New_Testament; ++var) {
        const QString dirContent = var == 0 ? Path::dirContent_Old_Testament_Photos : Path::dirContent_New_Testament_Photos;
        const QString fileContent = var == 0 ? Path::fileContent_Old_Testament_Photos : Path::fileContent_New_Testament_Photos;

        QJsonObject objMain;
        QStringList listDir = getListFileInDirectory(QFileInfo(dirContent).absolutePath());
        for (int var = 0; var < listDir.size(); ++var) {
            const QString bookIndex = getIndexBookStr(var);
            QJsonObject objBook;
            fillPhotos(&objBook, dirContent + bookIndex);
            objMain.insert(bookIndex, objBook);
        }
        FileWorker::writeFileJson(QJsonDocument(objMain), fileContent);
    }


    //    {
    //        "book_36": {
    //            "IMG_20190604_124816_HDR.jpg": "0",
    //            "IMG_20190604_124821_HDR.jpg": "0"
    //        },
    //        "book_37": {
    //            "IMG_20190604_124821_HDR.jpg": "0",
    //            "IMG_20190604_124827_HDR.jpg": "0"
    //        },
    //        "book_38": {
    //            "IMG_20190604_124827_HDR.jpg": "0",
    //            "IMG_20190604_124835_HDR.jpg": "0",
    //            "IMG_20190604_124841_HDR.jpg": "0",
    //            "IMG_20190604_124848_HDR.jpg": "0",
    //            "IMG_20190604_124854_HDR.jpg": "0",
    //            "IMG_20190604_124901_HDR.jpg": "0"
    //        },
    //    }
}

void ContentWorker::generateNewNameFolder_Photos()
{
    const int Old_New_Testament { 2 };
    for (int var = 0; var < Old_New_Testament; ++var) {
        const QString dirContent = var == 0 ? Path::dirContent_Old_Testament_Photos : Path::dirContent_New_Testament_Photos;
        const QString fileContent = var == 0 ? Path::fileContent_Old_Testament_Photos : Path::fileContent_New_Testament_Photos;

        QStringList listDir = getListFileInDirectory(QFileInfo(dirContent).absolutePath());
        for (int var = 0; var < listDir.size(); ++var) {
            const QString bookIndex = getIndexBookStr(var);
            QFile::rename(dirContent + getIndexBookStr(var), dirContent + bookIndex);
        }
    }
}

void ContentWorker::generateContent_Folders(const ContentWorker::ProviderContent typeContent, const bool onlyRemove)
{
    QString dirContent_Old { Path::dirContent_Old_Testament_JsonText_BODRUK };
    QString dirContent_New { Path::dirContent_New_Testament_JsonText_BODRUK };
    if (typeContent == GITHUB_BIBLE_USFM2JSON_RUSLAN_KAZAKOV) {
        dirContent_Old = Path::dirContent_Old_Testament_JsonText_KAZAKOV;
        dirContent_New = Path::dirContent_New_Testament_JsonText_KAZAKOV;
    }
    else if (typeContent == GETBIBLE_NET) {
        dirContent_Old = Path::dirContent_Old_Testament_JsonText_GETBIBLE;
        dirContent_New = Path::dirContent_New_Testament_JsonText_GETBIBLE;
    }
    QDir(dirContent_Old).removeRecursively();
    QDir(dirContent_New).removeRecursively();
    if (onlyRemove) {
        return;
    }
    QThread::msleep(2000);
    QDir().mkdir(dirContent_Old);
    QDir().mkdir(dirContent_New);
    QDir().mkdir(Path::dirContent_Old_Testament_Photos);
    QDir().mkdir(Path::dirContent_New_Testament_Photos);
}

void ContentWorker::generateContent_Info(const QString &fileOld, const QString &fileNew, const ContentWorker::Standard requiredStandart)
{
    QJsonObject objOldTestament;
    QJsonObject objNewTestament;
    for (int var = 0; var < BibleEnums::Old_Testament + BibleEnums::New_Testament; ++var) {
        FamilyBooks currentFamilyBooks { Pentateuch };
        QString bookIndex;
        int nextFamily { 0 };
        QJsonObject *refObjTestament = &objOldTestament;
        if (var < BibleEnums::Old_Testament) {
            if (var < FamilyBooks::Pentateuch) {
                currentFamilyBooks = Pentateuch;
            }
            else if (var < FamilyBooks::HistoricalBooks) {
                currentFamilyBooks = HistoricalBooks;
                nextFamily = FamilyBooks::Pentateuch;
            }
            else if (var < FamilyBooks::Wisdom) {
                currentFamilyBooks = Wisdom;
                nextFamily = FamilyBooks::HistoricalBooks;
            }
            else if (var < FamilyBooks::MajorProphets) {
                currentFamilyBooks = MajorProphets;
                nextFamily = FamilyBooks::Wisdom;
            }
            else if (var < FamilyBooks::MinorProphets) {
                currentFamilyBooks = MinorProphets;
                nextFamily = FamilyBooks::MajorProphets;
            }
            bookIndex = getIndexBookStr(var);
        }
        else{
            if (var < FamilyBooks::GospelsAndActs) {
                currentFamilyBooks = GospelsAndActs;
                nextFamily = FamilyBooks::MinorProphets;
            }
            else if (var < FamilyBooks::PaulineEpistles) {
                currentFamilyBooks = PaulineEpistles;
                nextFamily = FamilyBooks::GospelsAndActs;
            }
            else if (var < FamilyBooks::CatholicEpistles) {
                currentFamilyBooks = CatholicEpistles;
                nextFamily = FamilyBooks::PaulineEpistles;
            }
            else if (var < FamilyBooks::Apocalypse) {
                currentFamilyBooks = Apocalypse;
                nextFamily = FamilyBooks::CatholicEpistles;
            }
            bookIndex = getIndexBookStr(var, BibleEnums::New_Testament, Standard::Western, requiredStandart);
            refObjTestament = &objNewTestament;
        }
        QJsonObject objBook;
        objBook.insert("abbrev", getAbbrev(var, Standard::Western, "ru"));
        objBook.insert("name_en", getVectorFamilyBooks(currentFamilyBooks, "en").at(var - nextFamily));
        objBook.insert("name_ru", getVectorFamilyBooks(currentFamilyBooks, "ru").at(var - nextFamily));
        objBook.insert("family_books_en", getNameFamilyBooks(Apocalypse, "en"));
        objBook.insert("family_books_ru", getNameFamilyBooks(Apocalypse, "ru"));

        refObjTestament->insert(bookIndex, objBook);
        Q_ASSERT(!objBook.isEmpty());
    }
    FileWorker::writeFileJson(QJsonDocument(objOldTestament), fileOld);
    FileWorker::writeFileJson(QJsonDocument(objNewTestament), fileNew);

    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

void ContentWorker::generateContent_Info_BODRUK(const ContentWorker::Standard requiredStandart)
{
    QJsonDocument doc;
    // BODRUK has the Standard::Western standart.
    FileWorker::readFileJson(&doc, Path::providerBibleJsonText_BODRUK);
    QJsonArray arrMain = doc.array();

    QJsonObject objOldTestament;
    QJsonObject objNewTestament;
    for (int var = 0; var < arrMain.size(); ++var) {
        QJsonObject objBook = arrMain.at(var).toObject();
        objBook.remove("chapters");
        objBook.insert("name_en", objBook.value("name").toString());
        objBook.remove("name");

        QVector<QString> vecFamilyBooks;
        QString family_books_en;
        QString family_books_ru;
        QString bookIndex;
        int nextFamily { 0 };
        QJsonObject *refObjTestament = &objOldTestament;
        if (var < BibleEnums::Old_Testament) {
            if (var < FamilyBooks::Pentateuch) {
                vecFamilyBooks = getVectorFamilyBooks(Pentateuch);
                family_books_en = getNameFamilyBooks(Pentateuch, "en");
                family_books_ru = getNameFamilyBooks(Pentateuch, "ru");
            }
            else if (var < FamilyBooks::HistoricalBooks) {
                vecFamilyBooks = getVectorFamilyBooks(HistoricalBooks);
                family_books_en = getNameFamilyBooks(HistoricalBooks, "en");
                family_books_ru = getNameFamilyBooks(HistoricalBooks, "ru");
                nextFamily = FamilyBooks::Pentateuch;
            }
            else if (var < FamilyBooks::Wisdom) {
                vecFamilyBooks = getVectorFamilyBooks(Wisdom);
                family_books_en = getNameFamilyBooks(Wisdom, "en");
                family_books_ru = getNameFamilyBooks(Wisdom, "ru");
                nextFamily = FamilyBooks::HistoricalBooks;
            }
            else if (var < FamilyBooks::MajorProphets) {
                vecFamilyBooks = getVectorFamilyBooks(MajorProphets);
                family_books_en = getNameFamilyBooks(MajorProphets, "en");
                family_books_ru = getNameFamilyBooks(MajorProphets, "ru");
                nextFamily = FamilyBooks::Wisdom;
            }
            else if (var < FamilyBooks::MinorProphets) {
                vecFamilyBooks = getVectorFamilyBooks(MinorProphets);
                family_books_en = getNameFamilyBooks(MinorProphets, "en");
                family_books_ru = getNameFamilyBooks(MinorProphets, "ru");
                nextFamily = FamilyBooks::MajorProphets;
            }
            bookIndex = getIndexBookStr(var);
        }
        else{
            if (var < FamilyBooks::GospelsAndActs) {
                vecFamilyBooks = getVectorFamilyBooks(GospelsAndActs);
                family_books_en = getNameFamilyBooks(GospelsAndActs, "en");
                family_books_ru = getNameFamilyBooks(GospelsAndActs, "ru");
                nextFamily = FamilyBooks::MinorProphets;
            }
            else if (var < FamilyBooks::PaulineEpistles) {
                vecFamilyBooks = getVectorFamilyBooks(PaulineEpistles);
                family_books_en = getNameFamilyBooks(PaulineEpistles, "en");
                family_books_ru = getNameFamilyBooks(PaulineEpistles, "ru");
                nextFamily = FamilyBooks::GospelsAndActs;
            }
            else if (var < FamilyBooks::CatholicEpistles) {
                vecFamilyBooks = getVectorFamilyBooks(CatholicEpistles);
                family_books_en = getNameFamilyBooks(CatholicEpistles, "en");
                family_books_ru = getNameFamilyBooks(CatholicEpistles, "ru");
                nextFamily = FamilyBooks::PaulineEpistles;
            }
            else if (var < FamilyBooks::Apocalypse) {
                vecFamilyBooks = getVectorFamilyBooks(Apocalypse);
                family_books_en = getNameFamilyBooks(Apocalypse, "en");
                family_books_ru = getNameFamilyBooks(Apocalypse, "ru");
                nextFamily = FamilyBooks::CatholicEpistles;
            }
            bookIndex = getIndexBookStr(var, BibleEnums::New_Testament, Standard::Western, requiredStandart);
            refObjTestament = &objNewTestament;
        }
        objBook.insert("name_ru", vecFamilyBooks.at(var - nextFamily));
        objBook.insert("family_books_en", family_books_en);
        objBook.insert("family_books_ru", family_books_ru);

        refObjTestament->insert(bookIndex, objBook);
        Q_ASSERT(!objBook.isEmpty());
    }
    FileWorker::writeFileJson(QJsonDocument(objOldTestament), Path::fileContent_Old_Testament_Info_BODRUK);
    FileWorker::writeFileJson(QJsonDocument(objNewTestament), Path::fileContent_New_Testament_Info_BODRUK);

    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

void ContentWorker::generateContent_Info_ONLINE(const ContentWorker::Standard requiredStandart)
{
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, Path::tempJson);
    QJsonArray arrMain = doc.array();

    QJsonObject objOldTestament;
    QJsonObject objNewTestament;
    QString family_books_en;
    QString family_books_ru;
    for (int var = 0; var < arrMain.size(); ++var) {
        QJsonObject obj = arrMain.at(var).toObject();
        if (obj.keys().contains("h4")) {
            family_books_en = getNameFamilyBooks(FamilyBooks::Wisdom, "en");
            family_books_ru = obj.value("h4").toString();
        }
        if (!obj.keys().contains("li")) {
            continue;
        }
        QJsonObject objBook = obj.value("li").toObject();
        objBook.insert("name_en", objBook.value("Name").toString());
        objBook.remove("Name");
        objBook.insert("name_ru", objBook.value("locale").toString());
        objBook.remove("locale");
        int index = objBook.value("id").toString().toInt() - 1;
        objBook.remove("id");

        QString bookIndex;
        QJsonObject *refObjTestament = &objOldTestament;
        if (index < BibleEnums::Old_Testament) {
            if (index < FamilyBooks::Pentateuch) {
                family_books_en = getNameFamilyBooks(Pentateuch, "en");
            }
            else if (index < FamilyBooks::HistoricalBooks) {
                family_books_en = getNameFamilyBooks(HistoricalBooks, "en");
            }
            else if (index < FamilyBooks::Wisdom) {
                family_books_en = getNameFamilyBooks(Wisdom, "en");
            }
            else if (index < FamilyBooks::MajorProphets) {
                family_books_en = getNameFamilyBooks(MajorProphets, "en");
            }
            else if (index < FamilyBooks::MinorProphets) {
                family_books_en = getNameFamilyBooks(MinorProphets, "en");
            }
            bookIndex = getIndexBookStr(index);
        }
        else{
            if (index < FamilyBooks::GospelsAndActs) {
                family_books_en = getNameFamilyBooks(GospelsAndActs, "en");
            }
            else if (index < FamilyBooks::PaulineEpistles) {
                family_books_en = getNameFamilyBooks(PaulineEpistles, "en");
            }
            else if (index < FamilyBooks::CatholicEpistles) {
                family_books_en = getNameFamilyBooks(CatholicEpistles, "en");
            }
            else if (index < FamilyBooks::Apocalypse) {
                family_books_en = getNameFamilyBooks(Apocalypse, "en");
            }
            bookIndex = getIndexBookStr(index, BibleEnums::New_Testament, Standard::Western, requiredStandart);
            refObjTestament = &objNewTestament;
        }
        objBook.insert("family_books_en", family_books_en);
        objBook.insert("family_books_ru", family_books_ru);

        refObjTestament->insert(bookIndex, objBook);
        Q_ASSERT(!objBook.isEmpty());
    }
    FileWorker::writeFileJson(QJsonDocument(objOldTestament), Path::fileContent_Old_Testament_Info_ONLINE);
    FileWorker::writeFileJson(QJsonDocument(objNewTestament), Path::fileContent_New_Testament_Info_ONLINE);

    //    [
    //        {
    //            "h2": "Русский синодальный перевод"
    //        },
    //        {
    //            "h3": "Книги Ветхого завета"
    //        },
    //        {
    //            "h4": "Пятикнижие Моисея"
    //        },
    //        {
    //            "li": {
    //                "Name": "Genesis",
    //                "id": "1",
    //                "locale": "Книга Бытие"
    //            }
    //        },
    //        {
    //            "li": {
    //                "Name": "Exodus",
    //                "id": "2",
    //                "locale": "Книга Исход"
    //            }
    //        },
    //        {
    //            "li": {
    //                "Name": "Leviticus",
    //                "id": "3",
    //                "locale": "Книга Левит"
    //            }
    //        },
    //        {
    //            "li": {
    //                "Name": "Numbers",
    //                "id": "4",
    //                "locale": "Книга Числа"
    //            }
    //        },
    //        {
    //            "li": {
    //                "Name": "Deuteronomy",
    //                "id": "5",
    //                "locale": "Книга Второзаконие"
    //            }
    //        },
    //        {
    //            "h4": "Книги исторические"
    //        },
    //        {
    //            "li": {
    //                "Name": "Joshua",
    //                "id": "6",
    //                "locale": "Книга Иисуса Навина"
    //            }
    //        },
    //        ...
    //    ]
}

void ContentWorker::generateContent_Info_KAZAKOV(const ContentWorker::Standard requiredStandart)
{
    QJsonDocument doc;
    // KAZAKOV has the Standard::Western standart.
    FileWorker::readFileJson(&doc, Path::tempJson);
    QJsonArray arrMain = doc.array();

    Q_ASSERT(!arrMain.isEmpty());

    QJsonObject objOldTestament;
    QJsonObject objNewTestament;
    int index = -1;
    for (int var = 0; var < arrMain.size(); ++var) {
        QJsonObject objBook = arrMain.at(var).toObject();
        if (objBook.value("chapter").toInt() != 1 || objBook.value("verse").toInt() != 1) {
            continue;
        }
        ++index;
        objBook.remove("chapter");
        objBook.remove("verse");
        objBook.remove("text");
        objBook.insert("name_en", objBook.value("book_name").toString());
        objBook.remove("book_name");

        QVector<QString> vecFamilyBooks;
        QString family_books_en;
        QString family_books_ru;
        QString bookIndex;
        int nextFamily { 0 };
        QJsonObject *refObjTestament = &objOldTestament;
        if (index < BibleEnums::Old_Testament) {
            if (index < FamilyBooks::Pentateuch) {
                vecFamilyBooks = getVectorFamilyBooks(Pentateuch);
                family_books_en = getNameFamilyBooks(Pentateuch, "en");
                family_books_ru = getNameFamilyBooks(Pentateuch, "ru");
            }
            else if (index < FamilyBooks::HistoricalBooks) {
                vecFamilyBooks = getVectorFamilyBooks(HistoricalBooks);
                family_books_en = getNameFamilyBooks(HistoricalBooks, "en");
                family_books_ru = getNameFamilyBooks(HistoricalBooks, "ru");
                nextFamily = FamilyBooks::Pentateuch;
            }
            else if (index < FamilyBooks::Wisdom) {
                vecFamilyBooks = getVectorFamilyBooks(Wisdom);
                family_books_en = getNameFamilyBooks(Wisdom, "en");
                family_books_ru = getNameFamilyBooks(Wisdom, "ru");
                nextFamily = FamilyBooks::HistoricalBooks;
            }
            else if (index < FamilyBooks::MajorProphets) {
                vecFamilyBooks = getVectorFamilyBooks(MajorProphets);
                family_books_en = getNameFamilyBooks(MajorProphets, "en");
                family_books_ru = getNameFamilyBooks(MajorProphets, "ru");
                nextFamily = FamilyBooks::Wisdom;
            }
            else if (index < FamilyBooks::MinorProphets) {
                vecFamilyBooks = getVectorFamilyBooks(MinorProphets);
                family_books_en = getNameFamilyBooks(MinorProphets, "en");
                family_books_ru = getNameFamilyBooks(MinorProphets, "ru");
                nextFamily = FamilyBooks::MajorProphets;
            }
            bookIndex = getIndexBookStr(index);
        }
        else{
            if (index < FamilyBooks::GospelsAndActs) {
                vecFamilyBooks = getVectorFamilyBooks(GospelsAndActs);
                family_books_en = getNameFamilyBooks(GospelsAndActs, "en");
                family_books_ru = getNameFamilyBooks(GospelsAndActs, "ru");
                nextFamily = FamilyBooks::MinorProphets;
            }
            else if (index < FamilyBooks::PaulineEpistles) {
                vecFamilyBooks = getVectorFamilyBooks(PaulineEpistles);
                family_books_en = getNameFamilyBooks(PaulineEpistles, "en");
                family_books_ru = getNameFamilyBooks(PaulineEpistles, "ru");
                nextFamily = FamilyBooks::GospelsAndActs;
            }
            else if (index < FamilyBooks::CatholicEpistles) {
                vecFamilyBooks = getVectorFamilyBooks(CatholicEpistles);
                family_books_en = getNameFamilyBooks(CatholicEpistles, "en");
                family_books_ru = getNameFamilyBooks(CatholicEpistles, "ru");
                nextFamily = FamilyBooks::PaulineEpistles;
            }
            else if (index < FamilyBooks::Apocalypse) {
                vecFamilyBooks = getVectorFamilyBooks(Apocalypse);
                family_books_en = getNameFamilyBooks(Apocalypse, "en");
                family_books_ru = getNameFamilyBooks(Apocalypse, "ru");
                nextFamily = FamilyBooks::CatholicEpistles;
            }
            bookIndex = getIndexBookStr(index, BibleEnums::New_Testament, Standard::Western, requiredStandart);
            refObjTestament = &objNewTestament;
        }
        objBook.insert("name_ru", vecFamilyBooks.at(index - nextFamily));
        objBook.insert("family_books_en", family_books_en);
        objBook.insert("family_books_ru", family_books_ru);

        refObjTestament->insert(bookIndex, objBook);
        Q_ASSERT(!objBook.isEmpty());
    }
    FileWorker::writeFileJson(QJsonDocument(objOldTestament), Path::fileContent_Old_Testament_Info_KAZAKOV);
    FileWorker::writeFileJson(QJsonDocument(objNewTestament), Path::fileContent_New_Testament_Info_KAZAKOV);

    //    [
    //        {
    //        "chapter" : 1,
    //        "verse" : 1,
    //        "text" : "В начале сотворил Бог небо и землю.",
    //        "translation_id" : "RST",
    //        "book_id" : "Gen",
    //        "book_name" : "Genesis",
    //        },
    //        {
    //        ...
    //        },
    //    ]
}

void ContentWorker::generateContent_Info_GETBIBLE(const ContentWorker::Standard requiredStandart)
{
    generateContent_Info(Path::fileContent_Old_Testament_Info_GETBIBLE,
                         Path::fileContent_New_Testament_Info_GETBIBLE, requiredStandart);
}

void ContentWorker::generateContent_JsonText(const QString &pathFrom, const QString &dirOld, const QString &dirNew,
                                       const ContentWorker::Standard currentStandart, const ContentWorker::Standard requiredStandart)
{
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, pathFrom);
    QJsonArray arrMain = doc.array();
    qDebug() << "arrMain.size()" << arrMain.size() << Qt::endl;
    Q_ASSERT(arrMain.size() == BibleEnums::Old_Testament + BibleEnums::New_Testament);

    for (int var = 0; var < arrMain.size(); ++var) {
        QJsonObject objBook = arrMain.at(var).toObject();
        QJsonArray arrChapters = objBook.value("chapters").toArray();
        if (var < BibleEnums::Old_Testament) {
            const QString bookIndex = getIndexBookStr(var);
            FileWorker::writeFileJson(QJsonDocument(arrChapters), dirOld + bookIndex + ".json");
        }
        else{
            const QString bookIndex = getIndexBookStr(var, BibleEnums::New_Testament, currentStandart, requiredStandart);
            FileWorker::writeFileJson(QJsonDocument(arrChapters), dirNew + bookIndex + ".json");
        }
    }

    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

void ContentWorker::generateContent_JsonText_BODRUK(const Standard requiredStandart)
{
    QJsonDocument doc;
    // BODRUK has the Standard::Western standart.
    FileWorker::readFileJson(&doc, Path::providerBibleJsonText_BODRUK);
    QJsonArray arrMain = doc.array();

    QString dirContent { Path::dirContent_Old_Testament_JsonText_BODRUK };
    for (int var = 0; var < arrMain.size(); ++var) {
        QJsonObject objBook = arrMain.at(var).toObject();
        QJsonArray arrChapters = objBook.value("chapters").toArray();
        if (var < BibleEnums::Old_Testament) {
            const QString bookIndex = getIndexBookStr(var);
            FileWorker::writeFileJson(QJsonDocument(arrChapters), dirContent + bookIndex + ".json");
        }
        else{
            dirContent = Path::dirContent_New_Testament_JsonText_BODRUK;
            const QString bookIndex = getIndexBookStr(var, BibleEnums::New_Testament, Standard::Western, requiredStandart);
            FileWorker::writeFileJson(QJsonDocument(arrChapters), dirContent + bookIndex + ".json");
        }
    }

    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

void ContentWorker::generateContent_JsonText_ONLINE(const ContentWorker::Standard requiredStandart)
{
    Q_UNUSED(requiredStandart);
}

void ContentWorker::generateContent_JsonText_KAZAKOV(const ContentWorker::Standard requiredStandart)
{
    QJsonDocument doc;
    // KAZAKOV has the Standard::Western standart.
    FileWorker::readFileJson(&doc, Path::tempJson);
    QJsonArray arrMain = doc.array();
    Q_ASSERT(!arrMain.isEmpty());

    QString dirContent { Path::dirContent_Old_Testament_JsonText_KAZAKOV };
    int index { 0 };
    QJsonArray arrChapters;
    QJsonArray arrVerses;
    for (int var = 0; var < arrMain.size(); ++var) {
        QJsonObject objBookVerse = arrMain.at(var).toObject();
        QJsonObject nextObjBookVerse = var + 1 < arrMain.size() ? arrMain.at(var + 1).toObject() : QJsonObject();
        const int currentChapter = objBookVerse.value("chapter").toInt();
        const QString currentBook = objBookVerse.value("book_name").toString();
        const int nextChapter = nextObjBookVerse.value("chapter").toInt();
        const QString nextBook = nextObjBookVerse.value("book_name").toString();
        arrVerses.append(objBookVerse.value("text"));
        if (currentChapter != nextChapter || currentBook != nextBook) {
            arrChapters.append(arrVerses);
            arrVerses = QJsonArray();
        }

        if (currentBook == nextBook && var + 1 < arrMain.size()) {
            continue;
        }
        if (var == arrMain.size() - 1) {
            arrChapters.append(arrVerses);
        }

        Q_ASSERT(!arrChapters.isEmpty());
        if (index < BibleEnums::Old_Testament) {
            const QString bookIndex = getIndexBookStr(index);
            FileWorker::writeFileJson(QJsonDocument(arrChapters), dirContent + bookIndex + ".json");
        }
        else{
            dirContent = Path::dirContent_New_Testament_JsonText_KAZAKOV;
            const QString bookIndex = getIndexBookStr(index, BibleEnums::New_Testament, Standard::EasternSynodal, requiredStandart);
            FileWorker::writeFileJson(QJsonDocument(arrChapters), dirContent + bookIndex + ".json");
        }
        arrChapters = QJsonArray();
        ++index;
    }
    //    [
    //        {
    //        "chapter" : 1,
    //        "verse" : 1,
    //        "text" : "В начале сотворил Бог небо и землю.",
    //        "translation_id" : "RST",
    //        "book_id" : "Gen",
    //        "book_name" : "Genesis",
    //        },
    //        {
    //        ...
    //        },
    //    ]
}

void ContentWorker::generateContent_JsonText_GETBIBLE(const ContentWorker::Standard requiredStandart)
{
    generateContent_JsonText(Path::fileAllBibleJsonText_GETBIBLE, Path::dirContent_Old_Testament_JsonText_GETBIBLE,
                             Path::dirContent_New_Testament_JsonText_GETBIBLE, Western, requiredStandart);
}

void ContentWorker::generateContent_TwoArraysBooks(const ProviderContent typeContent)
{
    const int Old_New_Testament { 2 };
    for (int var = 0; var < Old_New_Testament; ++var) {
        QString fileContentJsonText = var == 0 ? Path::fileContent_Old_Testament_Info_BODRUK : Path::fileContent_New_Testament_Info_BODRUK;
        QString fileContentArrayBooks = var == 0 ? Path::fileContent_Old_Testament_ArrayBooks_BODRUK : Path::fileContent_New_Testament_ArrayBooks_BODRUK;
        if (typeContent == ProviderContent::BIBLEONLINE_RU) {
            fileContentJsonText = var == 0 ? Path::fileContent_Old_Testament_Info_ONLINE : Path::fileContent_New_Testament_Info_ONLINE;
            fileContentArrayBooks = var == 0 ? Path::fileContent_Old_Testament_ArrayBooks_ONLINE : Path::fileContent_New_Testament_ArrayBooks_ONLINE;
        }
        else if (typeContent == ProviderContent::GITHUB_BIBLE_USFM2JSON_RUSLAN_KAZAKOV) {
            fileContentJsonText = var == 0 ? Path::fileContent_Old_Testament_Info_KAZAKOV : Path::fileContent_New_Testament_Info_KAZAKOV;
            fileContentArrayBooks = var == 0 ? Path::fileContent_Old_Testament_ArrayBooks_KAZAKOV : Path::fileContent_New_Testament_ArrayBooks_KAZAKOV;
        }
        else if (typeContent == ProviderContent::GETBIBLE_NET) {
            fileContentJsonText = var == 0 ? Path::fileContent_Old_Testament_Info_GETBIBLE : Path::fileContent_New_Testament_Info_GETBIBLE;
            fileContentArrayBooks = var == 0 ? Path::fileContent_Old_Testament_ArrayBooks_GETBIBLE : Path::fileContent_New_Testament_ArrayBooks_GETBIBLE;
        }

        QJsonDocument doc;
        QJsonArray arrBooks;
        FileWorker::readFileJson(&doc, fileContentJsonText);
        QJsonObject objMain = doc.object();
        QStringList listKeys { objMain.keys() };
        for (const QString &bookKey : listKeys) {
            QJsonObject objBook = objMain.value(bookKey).toObject();
            arrBooks.append(objBook.value("abbrev"));
        }
        FileWorker::writeFileJson(QJsonDocument(arrBooks), fileContentArrayBooks);
        Q_ASSERT(arrBooks.size() == (var == 0 ? BibleEnums::Old_Testament : BibleEnums::New_Testament));
    }
}

QString ContentWorker::getAbbrev(const int index, const ContentWorker::Standard requiredStandart, const QString &language)
{
    Q_UNUSED(language);
    QVector<QString> vecAbbrevOld {
        "Быт",
        "Исх",
        "Лев",
        "Числ",
        "Втор",
        "Ис Нав",
        "Суд",
        "Руфь",
        "1 Цар",
        "2 Цар",
        "3 Цар",
        "4 Цар",
        "1 Пар",
        "2 Пар",
        "Ездр",
        "Неем",
        "Есф",
        "Иов",
        "Пс",
        "Прит",
        "Екл",
        "Псн Псн",
        "Исаия",
        "Иер",
        "Пл Иер",
        "Иез",
        "Дан",
        "Осия",
        "Иоил",
        "Амос",
        "Авд",
        "Иона",
        "Мих",
        "Наум",
        "Авв",
        "Соф",
        "Агг",
        "Зах",
        "Мал",
    };
    QVector<QString> vecAbbrevNew {
        "Матф",
        "Марк",
        "Лук",
        "Иоан",
        "Деян",
        "Рим",
        "1 Кор",
        "2 Кор",
        "Гал",
        "Еф",
        "Фил",
        "Кол",
        "1 Фес",
        "2 Фес",
        "1 Тим",
        "2 Тим",
        "Тит",
        "Фил",
        "Евр",
        "Иак",
        "1 Пет",
        "2 Пет",
        "1 Иоан",
        "2 Иоан",
        "3 Иоан",
        "Иуд",
        "Откр",
    };
    if (index < BibleEnums::Old_Testament) {
        return vecAbbrevOld.at(index);
    }
    if (requiredStandart == Western) {
        return vecAbbrevNew.at(getIndexBook(index, BibleEnums::New_Testament, Western, Western));
    }
    return vecAbbrevNew.at(getIndexBook(index, BibleEnums::New_Testament, EasternSynodal, Western));
}

QStringList ContentWorker::getListFileInDirectory(const QString &dir)
{
    QStringList list;
    const QDir directory(dir);

    for(const QFileInfo &info: directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
        //        if(!info.isDir()){
        list.append(info.absoluteFilePath());
        //        }
    }
    list.sort();
    return list;
}

void ContentWorker::fillPhotos(QJsonObject *objBook, const QString &pathDir)
{
    QStringList list = getListFileInDirectory(pathDir);
    for (const QString &str: list) {
        objBook->insert(QFileInfo(str).fileName(), "0");
    }
}

QString ContentWorker::getIndexBookStr(const int index)
{
    return "book_" + QString::number(index).rightJustified(2, '0');
}

QString ContentWorker::getIndexBookStr(int index, const BibleEnums::Testament testament,
                                 const ContentWorker::Standard currentStandart, const ContentWorker::Standard requiredStandart)
{
    int indexBook = getIndexBook(index, testament, currentStandart, requiredStandart);
    return getIndexBookStr(indexBook);
}

int ContentWorker::getIndexBook(int index, const BibleEnums::Testament testament,
                          const ContentWorker::Standard currentStandart, const ContentWorker::Standard requiredStandart)
{
    // сюда попадает index (currentStandart), возвращается index (requiredStandart)
    // currentStandart == EasternSynodal (index == 5 == Иакова) --> requiredStandart == Western (index == 19 == Иакова)
    if (testament == BibleEnums::New_Testament && currentStandart != requiredStandart) {
        const int quantityCatholicEpistles { 7 };
        const int quantityPaulineEpistles { 14 };
        // Western --> EasternSynodal : Western <-- EasternSynodal
        const int firstFamily = requiredStandart == Standard::EasternSynodal ? quantityCatholicEpistles : quantityPaulineEpistles;
        const int lastFamily = requiredStandart == Standard::EasternSynodal ? quantityPaulineEpistles : quantityCatholicEpistles;
        if (index >= FamilyBooks::GospelsAndActs && index < FamilyBooks::GospelsAndActs + lastFamily) {
            index += firstFamily;
        }
        else if(index >= FamilyBooks::GospelsAndActs + lastFamily && index < FamilyBooks::GospelsAndActs + lastFamily + firstFamily) {
            index -= lastFamily;
        }
    }
    if (testament == BibleEnums::New_Testament) {
        index -= BibleEnums::Old_Testament;
    }
    return index;
}

void ContentWorker::testGetIndexBook()
{
    for (int var = 44; var < 58; ++var) Q_ASSERT(getIndexBook(var, BibleEnums::New_Testament, Western, EasternSynodal) == var + 7 - BibleEnums::Old_Testament);
    for (int var = 58; var < 65; ++var) Q_ASSERT(getIndexBook(var, BibleEnums::New_Testament, Western, EasternSynodal) == var - 14 - BibleEnums::Old_Testament);
    for (int var = 44; var < 51; ++var) Q_ASSERT(getIndexBook(var, BibleEnums::New_Testament, EasternSynodal, Western) == var + 14 - BibleEnums::Old_Testament);
    for (int var = 51; var < 65; ++var) Q_ASSERT(getIndexBook(var, BibleEnums::New_Testament, EasternSynodal, Western) == var - 7  - BibleEnums::Old_Testament);
}

QVector<QString> ContentWorker::getVectorFamilyBooks(const ContentWorker::FamilyBooks familyBooks, const QString &language)
{
    QVector<QString> vecFamilyBooksRu;
    QVector<QString> vecFamilyBooksEn;
    if (familyBooks == FamilyBooks::Pentateuch) {
        vecFamilyBooksRu = { "Бытие", "Исход", "Левит", "Числа", "Второзаконие" };
        vecFamilyBooksEn = { "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy" };
    }
    else if (familyBooks == FamilyBooks::HistoricalBooks) {
        vecFamilyBooksRu = { "Иисус Навин", "Судьи", "Руфь", "1 Царств", "2 Царств", "3 Царств", "4 Царств",
                             "1 Паралипомен", "2 Паралипомен", "Ездра", "Неемия", "Есфирь" };
        vecFamilyBooksEn = { "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "3 Kings", "4 Kings",
                             "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah", "Esther" };
    }
    else if (familyBooks == FamilyBooks::Wisdom) {
        vecFamilyBooksRu = { "Иов", "Псалтирь", "Притчи", "Екклеcиаст", "Песни песней" };
        vecFamilyBooksEn = { "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Solomon" };
    }
    else if (familyBooks == FamilyBooks::MajorProphets) {
        vecFamilyBooksRu = { "Исаия", "Иеремия", "Плач Иеремии", "Иезекииль", "Даниил" };
        vecFamilyBooksEn = { "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel" };
    }
    else if (familyBooks == FamilyBooks::MinorProphets) {
        vecFamilyBooksRu = { "Осия", "Иоиль", "Амос", "Авдий", "Иона", "Михей", "Наум", "Аввакум", "Софония", "Аггей",
                             "Захария", "Малахия" };
        vecFamilyBooksEn = { "Hosea", "Joel", "Amos", "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk", "Zephaniah", "Haggai",
                             "Zechariah", "Malachi" };
    }
    else if (familyBooks == FamilyBooks::GospelsAndActs) {
        vecFamilyBooksRu = { "Матфея", "Марка", "Луки", "Иоанна", "Деяния" };
        vecFamilyBooksEn = { "Matthew", "Mark", "Luke", "John", "Acts" };
    }
    else if (familyBooks == FamilyBooks::PaulineEpistles) {
        vecFamilyBooksRu = { "Римлянам", "1 Коринфянам", "2 Коринфянам", "Галатам", "Ефесянам", "Филиппийцам", "Колоссянам",
                             "1 Фессалоникийцам", "2 Фессалоникийцам", "1 Тимофею", "2 Тимофею", "Титу", "Филимону", "Евреям"};
        vecFamilyBooksEn = { "Romans", "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians", "Philippians", "Colossians",
                             "1 Thessalonians", "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon", "Hebrews"};
    }
    else if (familyBooks == FamilyBooks::CatholicEpistles) {
        vecFamilyBooksRu = { "Иакова", "1 Петра", "2 Петра", "1 Иоанна", "2 Иоанна", "3 Иоанна", "Иуда" };
        vecFamilyBooksEn = { "James", "1 Peter", "2 Peter", "1 Иоанна", "2 Иоанна", "3 Иоанна", "Jude" };
    }
    else if (familyBooks == FamilyBooks::Apocalypse) {
        vecFamilyBooksRu = { "Откровение" };
        vecFamilyBooksEn = { "Revelation" };
    }
    if (language.toLower() == "en") {
        return vecFamilyBooksEn;
    }
    return  vecFamilyBooksRu;
}

QString ContentWorker::getNameFamilyBooks(const ContentWorker::FamilyBooks familyBooks, const QString &language)
{
    QString family_books_en;
    QString family_books_ru;
    if (familyBooks == FamilyBooks::Pentateuch) {
        family_books_en = "Pentateuch";
        family_books_ru = "Пятикнижие";
    }
    else if (familyBooks == FamilyBooks::HistoricalBooks) {
        family_books_en = "Historical Books";
        family_books_ru = "Исторические";
    }
    else if (familyBooks == FamilyBooks::Wisdom) {
        family_books_en = "Wisdom";
        family_books_ru = "Учительные";
    }
    else if (familyBooks == FamilyBooks::MajorProphets) {
        family_books_en = "Major Prophets";
        family_books_ru = "Большие пророки";
    }
    else if (familyBooks == FamilyBooks::MinorProphets) {
        family_books_en = "Minor Prophets";
        family_books_ru = "Малые пророки";
    }
    else if (familyBooks == FamilyBooks::GospelsAndActs) {
        family_books_en = "Gospels and Acts";
        family_books_ru = "Евангелия и Деяния";
    }
    else if (familyBooks == FamilyBooks::PaulineEpistles) {
        family_books_en = "Pauline Epistles";
        family_books_ru = "Послания Павла";
    }
    else if (familyBooks == FamilyBooks::CatholicEpistles) {
        family_books_en = "Catholic Epistles";
        family_books_ru = "Соборные послания";
    }
    else if (familyBooks == FamilyBooks::Apocalypse) {
        family_books_en = "Apocalypse";
        family_books_ru = "Пророческие";
    }

    if (language.toLower() == "en") {
        return family_books_en;
    }
    return family_books_ru;
}

void ContentWorker::getOnlineBookList()
{
    const QString url { "https://api.bibleonline.ru/booklist" };
    const QJsonDocument docParam({
                                     { "callback", "bible" },
                                     { "trans", "rus" }
                                     //                                     { "trans", "lut" }
                                 });
    sendGetRequest(url, docParam.toJson(), std::bind(&ContentWorker::slotGetReply, std::placeholders::_1));
}

void ContentWorker::getOnlineBible()
{
    const QString url { "https://api.bibleonline.ru/bible" };
    const QJsonDocument docParam({
                                     { "callback", "bible" },
                                     { "trans", "rus" },
                                     { "max", "2" },
                                     { "q", "Genesis 3.3"}
                                 });
    sendGetRequest(url, docParam.toJson(), std::bind(&ContentWorker::slotGetReply, std::placeholders::_1));
}

void ContentWorker::generateValidJson_KAZAKOV()
{
    QByteArray arr;
    FileWorker::readFile(&arr, Path::providerBibleJsonText_KAZAKOV);
    arr.replace("\"}\n", "\"},\n");
    arr.remove(arr.size() - 2, 2); // remove last ",\n"
    arr.insert(0, '[');
    arr.append(']');
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    if (!doc.isEmpty()) {
        qDebug() << "doc is valid" << Qt::endl;
    }
    FileWorker::writeFile(arr, Path::tempJson);
}

void ContentWorker::generateJsonFromTxt_GETBIBLE()
{
    // GETBIBLE has the Standard::Western standart.
    QFile file(Path::providerBibleTxtText_GETBIBLE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error ReadOnly file" << file.fileName() << Qt::endl;
        return;
    }

    QJsonArray arrMain;
    QJsonObject objBook;
    QJsonArray arrChapters;
    QJsonArray arrVerses;
    int currentChapter = 1;
    int indexBook { 0 };
    QTextStream in(&file);
    // (O --> old) 01O||1||1||В начале сотворил Бог небо и землю.
    // (N --> new) 40N||1||1||Родословие Иисуса Христа, Сына Давидова, Сына Авраамова.
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList list = line.split("||");
        Q_ASSERT(list.size() == 4);
        Q_ASSERT(list.at(0).size() == 3);
        const int lineBook = list.at(0).leftRef(2).toInt();
        const int lineChapter = list.at(1).toInt();
        if (currentChapter != lineChapter || indexBook + 1 != lineBook) {
            arrChapters.append(arrVerses);
            arrVerses = QJsonArray();
            currentChapter = lineChapter;
        }
        arrVerses.append(list.back());

        if (indexBook + 1 != lineBook) {
            objBook.insert("abbrev", getAbbrev(indexBook, Western));
            objBook.insert("book_index", indexBook);
            objBook.insert("chapters", arrChapters);
            arrChapters = QJsonArray();
            arrMain.append(objBook);
            ++indexBook;
        }
    }
    arrChapters.append(arrVerses);
    objBook.insert("abbrev", getAbbrev(indexBook, Western));
    objBook.insert("book_index", indexBook);
    objBook.insert("chapters", arrChapters);
    arrMain.append(objBook);
    qDebug() << "indexBook finish" << indexBook << Qt::endl;
    file.close();

    FileWorker::writeFileJson(QJsonDocument(arrMain), Path::fileAllBibleJsonText_GETBIBLE);

    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

void ContentWorker::sendGetRequest(const QString &urlStr, const QByteArray &paramJson, std::function<void (QNetworkReply *)> funcSlotReply)
{
    const QString header { "application/json" };
    qDebug() << "send param Utf8" << Qt::endl << qPrintable(QJsonDocument::fromJson(paramJson).toJson());
    QUrl url(urlStr);
    QUrlQuery query(paramJson);
    url.setQuery(query.query());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, header);
    mngr.get(request);
    QObject::connect(&mngr, &QNetworkAccessManager::sslErrors, &ContentWorker::slotSSLErrors);
    QObject::connect(&mngr, &QNetworkAccessManager::finished, funcSlotReply);
}

void ContentWorker::slotSSLErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qDebug() << "QSslError: " << errors << Qt::endl;
    reply->ignoreSslErrors(errors);
    QObject::disconnect(&mngr, &QNetworkAccessManager::sslErrors, nullptr, nullptr);
}

void ContentWorker::slotGetReply(QNetworkReply *reply)
{
    QByteArray arrReply { reply->readAll() };
    reply->deleteLater();
    QObject::disconnect(&mngr, &QNetworkAccessManager::finished, nullptr, nullptr);
    if(reply->error() != QNetworkReply::NoError){
        qWarning() << "reply->error()" << reply->error() << Qt::endl;
        return;
    }
    arrReply.remove(0, 1);
    arrReply.remove(arrReply.size() - 2, 2);
    QJsonDocument doc = QJsonDocument::fromJson(arrReply);
    qDebug() << "Response from server: " << Qt::endl << qPrintable(doc.toJson(QJsonDocument::Indented));
    if (doc.isEmpty()) {
        qDebug() << "arrReply" << arrReply << Qt::endl;
    }
    FileWorker::writeFileJson(doc, Path::tempJson);
}

