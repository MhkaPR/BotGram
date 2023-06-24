QT       += core gui
QT += multimedia
QT += multimediawidgets
QT += network
QT += sql
DESTDIR = $$PWD
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
    Forms/chatpage.cpp \
    botgrameenv.cpp \
#    capchabuilder.cpp \
    chat.cpp \
    email_search.cpp \
    libraries_BotGram/Accont/Account.cpp \
    libraries_BotGram/Connection/clientHost.cpp \
   # libraries_BotGram/adduser_spacket.cpp \
    #libraries_BotGram/authentication.cpp \
    libraries_BotGram/adduser_spacket.cpp \
    libraries_BotGram/capcha/capchacreator.cpp \
    #libraries_BotGram/checkverifysafepacket.cpp \
    libraries_BotGram/checkverifysafepacket.cpp \
    libraries_BotGram/client.cpp \
   # libraries_BotGram/client_mssages.cpp \
    libraries_BotGram/connectverify.cpp \
    libraries_BotGram/database.cpp \
    libraries_BotGram/database/database_complex.cpp \
#    libraries_BotGram/database_complex.cpp \
    libraries_BotGram/database/user_database.cpp \
    libraries_BotGram/filemessage.cpp \
    libraries_BotGram/filemessagewidget.cpp \
    libraries_BotGram/loginpacket.cpp \
    libraries_BotGram/messagewidget.cpp \
    libraries_BotGram/package.cpp \
    libraries_BotGram/pvroom.cpp \
    libraries_BotGram/queriespacket.cpp \
    libraries_BotGram/room.cpp \
    libraries_BotGram/roompacket.cpp \
    libraries_BotGram/searchuser.cpp \
    libraries_BotGram/systemmessagepacket.cpp \
    libraries_BotGram/textmessage.cpp \
    #libraries_BotGram/tokenbuilder.cpp \
    libraries_BotGram/tokenpacket.cpp \
    libraries_BotGram/verify.cpp \
    main.cpp \
    botgram.cpp \
#    user_database.cpp

HEADERS += \
#    accontvalidity.h \
    DataFiles/checkingcaptcha/captcha.h \
    DataFiles/checkingcaptcha/dialog.h \
    Forms/buildercapcha.h \
    Forms/chatpage.h \
    botgram.h \
    botgrameenv.h \
#    capchabuilder.h \
    chat.h \
    email_search.h \
    libraries_BotGram/Accont/Account.h \
    libraries_BotGram/Connection/clientHost.h \
    libraries_BotGram/Connection/packet.h \
    libraries_BotGram/Connection/serialize.h \
    libraries_BotGram/Handlers/RecvANDconnectionHandler.h \
    #libraries_BotGram/Packages.h \
    #libraries_BotGram/adduser_spacket.h \
    #libraries_BotGram/authentication.h \
    libraries_BotGram/adduser_spacket.h \
    libraries_BotGram/capcha/api2captcha.hpp \
    libraries_BotGram/capcha/capchacreator.h \
    #libraries_BotGram/checkverifysafepacket.h \
    libraries_BotGram/checkverifysafepacket.h \
    libraries_BotGram/client.h \
   # libraries_BotGram/client_mssages.h \
    libraries_BotGram/connectverify.h \
    libraries_BotGram/database.h \
    #libraries_BotGram/database/Packages.h \
    libraries_BotGram/database/database_complex.h \
 #    libraries_BotGram/database_complex.h
 #    user_database.h
    libraries_BotGram/database/user_database.h \
    libraries_BotGram/filemessage.h \
    libraries_BotGram/filemessagewidget.h \
    moc_FileMessageWidget.cpp \
    libraries_BotGram/loginpacket.h \
    libraries_BotGram/messagewidget.h \
    moc_messagewidget.cpp \
    libraries_BotGram/package.h \
    libraries_BotGram/pvroom.h \
    libraries_BotGram/queriespacket.h \
    libraries_BotGram/room.h \
    libraries_BotGram/roompacket.h \
    #libraries_BotGram/server/Packages.h \
    #libraries_BotGram/server/packages.h \
    libraries_BotGram/searchuser.h \
    libraries_BotGram/systemmessagepacket.h \
    libraries_BotGram/textmessage.h \
    #libraries_BotGram/tokenbuilder.h \
    libraries_BotGram/tokenpacket.h \
    libraries_BotGram/verify.h

FORMS += \
    DataFiles/checkingcaptcha/dialog.ui \
    Forms/buildercapcha.ui \
    Forms/chatpage.ui \
    botgram.ui \
    botgrameenv.ui \
 \#    capchabuilder.ui
    chat.ui \
    email_search.ui

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


