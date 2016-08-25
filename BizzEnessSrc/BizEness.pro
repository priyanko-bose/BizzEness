#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T14:36:57
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BizEness
TEMPLATE = app


SOURCES +=\        
    GUI/src/be_mainwindow.cpp \
    BusinessManager/src/beStockManager.cpp \
    BusinessManager/src/beBusinessManager.cpp \
    main/src/main.cpp \
    BusinessManager/src/bePurchaseManager.cpp \
    Interface/src/ifHandleDB.cpp \
    BusinessManager/src/beSalesManager.cpp \
    BusinessManager/src/beSummaryManager.cpp \
    BusinessManager/src/beCashFlowManager.cpp \
    BusinessManager/src/bePLManager.cpp \
    GUI/src/be_purwindow.cpp \
    SqlDBManager/src/beDataBaseHandler.cpp \
    Interface/src/ifHandleBM.cpp \
    Interface/src/ifHandleGUI.cpp \
    SqlDBManager/src/beDBUtils.cpp \
    common/src/csvParser.cpp \
    common/src/utils.cpp \
    common/src/common.cpp \
    GUI/src/be_guiutils.cpp

HEADERS  +=\
    GUI/include/be_mainwindow.h \
    BusinessManager/include/beStockManager.h \
    BusinessManager/include/beBusinessManager.h \
    BusinessManager/include/bePurchaseManager.h \
    Interface/include/ifHandleDB.h \
    BusinessManager/include/beSalesManager.h \
    BusinessManager/include/beCashFlowManager.h \
    BusinessManager/include/bePLManager.h \
    BusinessManager/include/beSummaryManager.h \
    GUI/include/be_purwindow.h \
    SqlDBManager/include/beDataBaseHandler.h \
    Interface/include/ifHandleGUI.h \
    Interface/include/ifHandleBM.h \
    SqlDBManager/include/beDBUtils.h \
    common/include/common.h \
    common/include/csvParser.h \
    common/include/utils.h \
    GUI/include/be_guiutils.h

FORMS    += \
    GUI/forms/be_mainwindow.ui \
GUI/forms/be_purchase.ui
