#include "ContentWorker.h"


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
    //        "abbrev" : "Быт"
    //        "book_index" : 0
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

void ContentWorker::generateContentStandart(const ContentWorker::Standard requiredStandart)
{
    testGetIndexBook();
    generateContent_Folders();
    generateAllBibleJsonFromTxt_GETBIBLE();
    generateContent_Info_GETBIBLE(requiredStandart);
    generateContent_JsonText_GETBIBLE(requiredStandart);
}

void ContentWorker::removeAllContent()
{
    generateContent_Folders(true);
    QFile::remove(Path::fileAllBibleJsonText_GETBIBLE);
    QFile::remove(Path::fileContent_Info_GETBIBLE);
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
    QJsonObject objMain;
    QStringList listDir = getListFileInDirectory(QFileInfo(Path::dirContent_Photos).absolutePath());
    for (int var = 0; var < listDir.size(); ++var) {
        const QString bookIndex = getIndexBookStr(var);
        QJsonObject objBook;
        fillPhotos(&objBook, Path::dirContent_Photos + bookIndex);
        objMain.insert(bookIndex, objBook);
    }
    FileWorker::writeFileJson(QJsonDocument(objMain), Path::fileContent_Photos);


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

void ContentWorker::generateContent_Folders(const bool onlyRemove)
{
    QDir(Path::dirContent_JsonText_GETBIBLE).removeRecursively();
    if (onlyRemove) {
        return;
    }
//    QThread::msleep(2000);
    QDir().mkdir(Path::dirContent_Photos);
    QDir().mkdir(Path::dirContent_JsonText_GETBIBLE);
}

void ContentWorker::generateAllBibleJsonFromTxt_GETBIBLE()
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
        qDebug() << "Text" << objBook << Qt::endl;
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

void ContentWorker::generateContent_Info_GETBIBLE(const ContentWorker::Standard requiredStandart)
{
    QJsonObject objInfo;
    for (int var = 0; var < BibleEnums::Old_Testament + BibleEnums::New_Testament; ++var) {
        FamilyBooks currentFamilyBooks { Pentateuch };
        QString bookIndex;
        int nextFamily { 0 };
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
        }
        QJsonObject objBook;
        objBook.insert("abbrev", getAbbrev(var, Standard::Western, "ru"));
        objBook.insert("name_en", getVectorFamilyBooks(currentFamilyBooks, "en").at(var - nextFamily));
        objBook.insert("name_ru", getVectorFamilyBooks(currentFamilyBooks, "ru").at(var - nextFamily));
        objBook.insert("family_books_en", getNameFamilyBooks(Apocalypse, "en"));
        objBook.insert("family_books_ru", getNameFamilyBooks(Apocalypse, "ru"));

        objInfo.insert(bookIndex, objBook);
        Q_ASSERT(!objBook.isEmpty());
    }
    FileWorker::writeFileJson(QJsonDocument(objInfo), Path::fileContent_Info_GETBIBLE);

    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

void ContentWorker::generateContent_JsonText_GETBIBLE(const ContentWorker::Standard requiredStandart)
{
    generateContent_JsonText(Path::fileAllBibleJsonText_GETBIBLE, Path::dirContent_JsonText_GETBIBLE, Western, requiredStandart);
}

void ContentWorker::generateContent_JsonText(const QString &pathFrom, const QString &dirWhere,
                        const ContentWorker::Standard currentStandart, const ContentWorker::Standard requiredStandart)
{
//    Path::fileAllBibleJsonText_GETBIBLE, Path::dirContent_JsonText_GETBIBLE
    QJsonDocument doc;
    FileWorker::readFileJson(&doc, pathFrom);
    QJsonArray arrMain = doc.array();
    qDebug() << "arrMain.size()" << arrMain.size() << Qt::endl;
    Q_ASSERT(arrMain.size() == BibleEnums::Old_Testament + BibleEnums::New_Testament);

    for (int var = 0; var < arrMain.size(); ++var) {
        QJsonObject objBook = arrMain.at(var).toObject();
        QJsonArray arrChapters = objBook.value("chapters").toArray();
        QString bookIndex;
        if (var < BibleEnums::Old_Testament) {
            bookIndex = getIndexBookStr(var);
        }
        else{
            bookIndex = getIndexBookStr(var, BibleEnums::New_Testament, currentStandart, requiredStandart);
        }
        FileWorker::writeFileJson(QJsonDocument(arrChapters), dirWhere + bookIndex + ".json");
    }

    // The structure of the AllBible file is described in the the ContentWorker constructor.
}

QString ContentWorker::getAbbrev(const int index, const ContentWorker::Standard requiredStandart, const QString &language)
{
    Q_UNUSED(language);
    QVector<QString> vecAbbrev {
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
        // NewTestament
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
        return vecAbbrev.at(index);
    }
    if (requiredStandart == Western) {
        return vecAbbrev.at(getIndexBook(index, BibleEnums::New_Testament, Western, Western));
    }
    return vecAbbrev.at(getIndexBook(index, BibleEnums::New_Testament, EasternSynodal, Western));
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

