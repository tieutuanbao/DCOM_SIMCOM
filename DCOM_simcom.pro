QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += --std=c++11 c11

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
    main.cpp \
    libs/SerialThread/SerialThread.cpp \
    components/SIM7600/simcom_7600_api.c \
    components/SIM7600/simcom_7600.c \
    components/SIM7600/simcom_7600_command.c \
    libs/millis.c \
    libs/delay.c \
    view/mainwindow.cpp \
    view/messenger/MessRecvContentform.cpp \
    view/messenger/MessSentContentForm.cpp \
    view/messenger/MessShort.cpp \
    view/messenger/MessengerForm.cpp

HEADERS += \
    SimCardManager.h \
    components/SIM7600/simcom_7600.h \
    components/SIM7600/simcom_7600_api.h \
    components/SIM7600/simcom_7600_command.h \
    libs/SerialThread/SerialThread.h \
    libs/delay.h \
    libs/millis.h \
    view/mainwindow.h \
    view/messenger/MessRecvContentform.h \
    view/messenger/MessSentContentForm.h \
    view/messenger/MessShort.h \
    view/messenger/MessengerForm.h

INCLUDEPATH += \
    view \
    view/messenger \
    libs \
    libs/SerialThread \
    components/SIM7600

FORMS += \
    view/mainwindow.ui \
    view/messenger/MessRecvContentform.ui \
    view/messenger/MessSentContentForm.ui \
    view/messenger/MessShort.ui \
    view/messenger/MessengerForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    icons.qrc
