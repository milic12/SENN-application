QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    parametersscreen.cpp

HEADERS += \
    mainwindow.h \
    parametersscreen.h \
    structs.h

FORMS += \
    mainwindow.ui \
    parametersscreen.ui

QLIBXLSXWRITER_PARENTPATH = ./Qlibxlsxwriter/libxlsxwriter
include(./Qlibxlsxwriter/Qlibxlsxwriter/Qlibxlsxwriter.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = logo_icon.ico

DISTFILES += \
    ../../../Diplomski/slike/logo_icon.ico

RESOURCES += \
    resource.qrc
