QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Adjutants/Data.cpp \
    Adjutants/FileWorker.cpp \
    Dialogs/DialogMergeCorrectFile.cpp \
    Dialogs/DialogSettingsAnalysis.cpp \
    Tabs/Analysis.cpp \
    Tabs/AnalysisWorker.cpp \
    Tabs/Content.cpp \
    Tabs/ContentWorker.cpp \
    Tabs/Correct.cpp \
    Tabs/CorrectWorker.cpp \
    Tabs/TabBase.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Adjutants/Data.h \
    Adjutants/FileWorker.h \
    Dialogs/DialogMergeCorrectFile.h \
    Dialogs/DialogSettingsAnalysis.h \
    Tabs/Analysis.h \
    Tabs/AnalysisWorker.h \
    Tabs/Content.h \
    Tabs/ContentWorker.h \
    Tabs/Correct.h \
    Tabs/CorrectWorker.h \
    Tabs/TabBase.h \
    mainwindow.h

FORMS += \
    Dialogs/DialogMergeCorrectFile.ui \
    Dialogs/DialogSettingsAnalysis.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += \
    Adjutants \
    Tabs \
    Dialogs \
