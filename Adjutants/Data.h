#ifndef DATA_H
#define DATA_H

#include <QObject>


namespace BibleEnums
{

Q_NAMESPACE         // required for meta object creation
enum Testament{
    Old_Testament = 39,
    New_Testament = 27,
};
Q_ENUM_NS(Testament)  // register the enum in meta object data
}

namespace Path {
const QString prefix = "../CorrectBibleText";

const QString providerBibleTxtText_GETBIBLE  = prefix + "/TextBible_OfflineSources/Getbible_Net/Russian__Synodal_Translation_(1876)__synodal__LTR(getbible.net).txt";

const QString fileAllBibleJsonText_GETBIBLE     = prefix + "/Resource/Content/AllBible_JsonText_GETBIBLE.json";
const QString fileContent_Info_GETBIBLE         = prefix + "/Resource/Content/Content_Info_GETBIBLE.json";
const QString dirContent_JsonText_GETBIBLE      = prefix + "/Resource/Content/JsonText_GETBIBLE/";

const QString fileContent_Photos                = prefix + "/Resource/Content/Content_Photos.json";
const QString dirContent_Photos                 = prefix + "/Resource/Content/Photos/";
}



#endif // DATA_H
