QT       += core gui

QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


LIBS += -lws2_32

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
    DataFiles/checkingcaptcha/captcha.cpp \
    DataFiles/checkingcaptcha/dialog.cpp \
    Forms/buildercapcha.cpp \
    botgrameenv.cpp \
#    capchabuilder.cpp \
    chat.cpp \
    libraries_BotGram/Accont/Account.cpp \
    libraries_BotGram/Connection/clientHost.cpp \
    libraries_BotGram/capcha/capchacreator.cpp \
    libraries_BotGram/database/database_complex.cpp \
#    libraries_BotGram/database_complex.cpp \
    libraries_BotGram/database/user_database.cpp \
    main.cpp \
    botgram.cpp \
#    user_database.cpp

HEADERS += \
#    accontvalidity.h \
    DataFiles/checkingcaptcha/captcha.h \
    DataFiles/checkingcaptcha/dialog.h \
    Forms/buildercapcha.h \
    botgram.h \
    botgrameenv.h \
#    capchabuilder.h \
    chat.h \
    libraries_BotGram/Accont/Account.h \
    libraries_BotGram/Connection/clientHost.h \
    libraries_BotGram/Connection/packet.h \
    libraries_BotGram/Connection/serialize.h \
    libraries_BotGram/Handlers/RecvANDconnectionHandler.h \
    libraries_BotGram/capcha/api2captcha.hpp \
    libraries_BotGram/capcha/capchacreator.h \
    libraries_BotGram/database/Packages.h \
    libraries_BotGram/database/database_complex.h \
 \#    libraries_BotGram/database_complex.h
 \#    user_database.h
    libraries_BotGram/database/user_database.h \
    libraries_BotGram/server/Packages.h \
    libraries_BotGram/server/packages.h

FORMS += \
    DataFiles/checkingcaptcha/dialog.ui \
    Forms/buildercapcha.ui \
    botgram.ui \
    botgrameenv.ui \
 \#    capchabuilder.ui
    chat.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    DataFiles/files.qrc

DISTFILES += \
    DataBases/data.btg \
    DataBases/dataMe.xml \
    DataBases/sample.xml \
    DataFiles/checkingcaptcha/Captcha.sln \
    DataFiles/checkingcaptcha/Captcha.vcxproj \
    DataFiles/checkingcaptcha/Generate VC Project.bat \
    DataFiles/checkingcaptcha/LICENSE.txt \
    DataFiles/noise1.png \
    DataFiles/noise2.png \
    DataFiles/noise3.png \
    DataFiles/noise4.png \
    DataFiles/noise5.png


