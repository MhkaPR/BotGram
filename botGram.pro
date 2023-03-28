QT       += core gui

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
#    accontvalidity.cpp \
    libraries_BotGram/Accont/Account.cpp \
    libraries_BotGram/database/database_complex.cpp \
#    libraries_BotGram/database_complex.cpp \
    libraries_BotGram/database/user_database.cpp \
    main.cpp \
    botgram.cpp \
#    user_database.cpp

HEADERS += \
#    accontvalidity.h \
    botgram.h \
    libraries_BotGram/Accont/Account.h \
    libraries_BotGram/database/database_complex.h \
 \#    libraries_BotGram/database_complex.h
 \#    user_database.h
    libraries_BotGram/database/user_database.h

FORMS += \
    botgram.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    DataFiles/files.qrc

DISTFILES += \
    DataBases/BotGramData.xml \
    DataBases/sample.xml
