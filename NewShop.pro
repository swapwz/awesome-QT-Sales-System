#-------------------------------------------------
#
# Project created by QtCreator 2013-09-27T18:54:19
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NewShop
TEMPLATE = app
RC_ICONS = icon/appicon.ico
SUBDIRS += model \
           tool


SOURCES += main.cpp\
        shopmainwindow.cpp \
    loginwindow.cpp \
    model/goodsinfomodel.cpp \
    model/goodsinfo.cpp \
    tool/databaseimpl.cpp \
    model/goodstype.cpp \
    model/goodstypemodel.cpp \
    model/userinfo.cpp \
    model/userinfomodel.cpp \ 
    model/purchaseinfo.cpp \
    model/gooddiscount.cpp \
    model/goodssell.cpp \
    model/unitinfo.cpp \
    model/goodsstock.cpp \
    shop.cpp \
    model/goodsstockmodel.cpp \
    model/unitinfomodel.cpp \
    dialogquerypasswd.cpp \
    readonlyitemdelegate.cpp \
    model/goodssellmodel.cpp \
    model/purchaseinfomodel.cpp \
    selltotalwindow.cpp \
    tool/editablequerymodel.cpp \
    deletedialog.cpp \
    model/goodscredit.cpp

HEADERS  += shopmainwindow.h\
    loginwindow.h \
    model/goodsinfomodel.h \
    model/goodsinfo.h \
    tool/databaseimpl.h \
    model/goodstype.h \
    model/goodstypemodel.h \
    model/userinfo.h \
    model/userinfomodel.h \ 
    model/purchaseinfo.h \
    model/gooddiscount.h \
    model/goodssell.h \
    model/unitinfo.h \
    model/goodsstock.h \
    shop.h \
    model/goodsstockmodel.h \
    model/unitinfomodel.h \
    dialogquerypasswd.h \
    readonlyitemdelegate.h \
    model/goodssellmodel.h \
    model/purchaseinfomodel.h \
    selltotalwindow.h \
    tool/editablequerymodel.h \
    deletedialog.h \
    model/goodscredit.h

FORMS    += shopmainwindow.ui \
    loginwindow.ui \
    dialogquerypasswd.ui \
    selltotalwindow.ui \
    deletedialog.ui

RESOURCES += \
    icon.qrc
