#include <QApplication>
#include <fstream>
#include "Interface/include/ifHandleDB.h"
#include "Interface/include/ifHandleGUI.h"
#include "GUI/include/be_mainwindow.h"
#include "BusinessManager/include/beBusinessManager.h"
#include "BusinessManager/include/bePurchaseManager.h"
#include "BusinessManager/include/beStockManager.h"
#include "SqlDBManager/include/beDataBaseHandler.h"

extern int isAdmin;
namespace Ui {
class BE_MainWindow;
class tablesForm;
}

extern ofstream fout;

/*
 * This returns the complete list of product names
 */
QStringList *getProductList()
{
    list<string>::iterator it;
    QStringList *prodList = '\0';
    BE_BusinessManager *beBusinessManagerObj = 0;
    beBusinessManagerObj = BE_BusinessManager::getInstance();
    if(!beBusinessManagerObj)
        return prodList;
    if(list<string> *mylist = beBusinessManagerObj->getStockManager()->getStockProductList())
    {
        prodList = new QStringList();
        for (it=mylist->begin(); it!=mylist->end(); ++it){
            *prodList << QString((*it).c_str()) ;
        }
        delete mylist;
        mylist='\0';
    }
    return prodList;
}

/*
 * This function is used to Initialize GUI
 */
errorType initializeGUI(int argc , char *argv[])
{
    BE_MainWindow *w = 0;
    QApplication a(argc, argv);
    errorType errorCode = ERR_NONE;
    w = BE_MainWindow :: getBeMainWindow();    //create the single ton object of BE_MainWindow class
    if(!w){
        errorCode = ERR_UI_INIT;
        fout << "error("<<errorCode<<"):ifCommon.cpp:initializeGUI:mainWindow couldn't be initialized:" <<endl;

    }else{
        errorCode = populateSavedData();
        !errorCode? (fout << "info:ifCommon.cpp:initializeGUI:data populated"<<endl)
                  :(fout << "error("<<errorCode<<"):ifCommon.cpp:initializeGUI:data not populated"<<endl);
        w->initializeSignalsSlots();
        w->show();
        !a.exec()? errorCode = ERR_NONE : errorCode = ERR_QAPP ;
        if(errorCode)
            fout << "error("<<errorCode<<"):ifCommon.cpp:initializeGUI:couldn't start APP:"<<endl;
    }
    fout << "info:ifCommon.cpp:initializeGUI:done"<<endl;
    return errorCode;
}

/*
 * This function is used to De-Initialize GUI
 */
void deinitializeGUI()
{
    BE_MainWindow *w = BE_MainWindow :: getBeMainWindow();
    if(w){
        delete w;
        w='\0';
    }
    else{
        fout << "error:ifCommon.cpp:deinitializeGUI:mainWindow couldn't be fetched" <<endl;
    }
    fout << "info:ifCommon.cpp:deinitializeGUI:done"<<endl;
}

errorType importTableData(string path)
{
    int errorCode = 0;
    BE_DatabaseHandler *beDbObj = BE_DatabaseHandler::getDataBaseHandlerInstance();
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();

    //Restore Purchase data from all places --start
    errorCode |= beDbObj->purTableHandler->deleteAllTableEntries();
    errorCode |= beMangObj->getPurchaseManager()->deleteAllItems();
    int noOfItems = 0;
    errorCode |= beMangObj->getPurchaseManager()->readFileData(path, &noOfItems);
    beMangObj->isPurchaseDataRead = true;
    beMangObj->getPurchaseManager()->totItems = noOfItems;
    BE_MainWindow *w = BE_MainWindow :: getBeMainWindow();
    int rowCount = w->getPurchaseTable()->rowCount();
    for(int it = 0; it < rowCount; it++)
        w->getPurchaseTable()->removeRow(0);
    errorCode |= populateSavedData();

    int totItems = 0;
    matrix *records = 0;
    fout<<"info:ifHandleGUI.cpp:importTableData:saving data into Purchase database"<<endl;
    errorCode |= beMangObj->getPurchaseManager()->readBMData(&records, &totItems);
    for(int iter=0; iter < totItems; iter++){
        fout << records->at(iter).at(0);
        errorCode |= beDbObj->purTableHandler->addIntoDataBase(atol(records->at(iter).at(0).c_str()) , &records->at(iter));
    }
    totItems = 0;
    delete records;
    records = '\0';
    //Restore Purchase data from all places -- end

    return (errorType)errorCode;
}

/*
 * Interface function to populate data read from database into table widgets
 */
errorType populateSavedData()
{
    BE_BusinessManager *beBusinessMgr = 0;
    beBusinessMgr = BE_BusinessManager::getInstance();
    if(!beBusinessMgr)
    {
        fout << "error("<< ERR_BMANAGER_INIT<<"):ifHandleDB.cpp:populateSavedData:Couldn't get backend object" <<endl;
        return ERR_BMANAGER_INIT;
    }
    BE_MainWindow *w = 0;
    w = BE_MainWindow :: getBeMainWindow();
    if(!w )
    {
        fout << "error("<< ERR_UI_INIT<<"):ifHandleDB.cpp:populateSavedData:Coudn't get GUI object" << endl;
        return ERR_UI_INIT;
    }

    if(beBusinessMgr->isStockDataRead)
    {
        QTableWidget *stockTable = w->getStockTable();
        map <int, stockData_t> & table = beBusinessMgr->getStockManager()->getStockTable();
        int rowCount = beBusinessMgr->getStockManager()->totItems;
        int row = 0;
        for(map<int,stockData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            if(it->first > w->startProdId )
                w->startProdId = it->first;
            stockTable->insertRow(row);
            for(int col=PROD_ID; col < PROD_END; col++)
            {
                if(!stockTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,beBusinessMgr->getStockManager()->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    stockTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:stock data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no stock data populated" << endl;

    if(beBusinessMgr->isPurchaseDataRead)
    {
        QTableWidget *purchaseTable = w->getPurchaseTable();
        BE_PurchaseManager *purMgr = beBusinessMgr->getPurchaseManager();
        map <unsigned int, purchaseData_t> & table = purMgr->getPurchaseTable();
        int rowCount = purMgr->totItems;
        int row = 0;
        for(map<unsigned int,purchaseData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            purchaseTable->insertRow(row);
            for(int col=PUR_ID; col <= PUR_DUE; col++)
            {
                if(!purchaseTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,purMgr->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    if(!isAdmin || col == PUR_ID)
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    purchaseTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:purchase data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no purchase data populated" << endl;

    if(beBusinessMgr->isSalesDataRead)
    {
        QTableWidget *salesTable = w->getSalesTable();
        map <int, salesData_t> & table = beBusinessMgr->getSalesManager()->getSalesTable();
        int rowCount = beBusinessMgr->getSalesManager()->totItems;
        int row = 0;
        for(map<int,salesData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            if(it->first > w->startSalesId )
                w->startSalesId = it->first;
            salesTable->insertRow(row);
            for(int col=SALES_ID; col < SALES_END; col++)
            {
                if(!salesTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,beBusinessMgr->getSalesManager()->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    salesTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:sales data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no sales data populated" << endl;

    if(beBusinessMgr->isPLDataRead)
    {
        QTableWidget *plTable = w->getPLTable();
        map <int, plData_t> & table = beBusinessMgr->getPLManager()->getPLTable();
        int rowCount = beBusinessMgr->getPLManager()->totItems;
        int row = 0;
        for(map<int,plData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            if(it->first > w->startPLId )
                w->startPLId = it->first;
            plTable->insertRow(row);
            for(int col=PL_ID; col < PL_END; col++)
            {
                if(!plTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,beBusinessMgr->getPLManager()->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    plTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:Profit&Loss data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no Profit&Loss data populated" << endl;
    if(beBusinessMgr->isCashFlowDataRead)
    {
        QTableWidget *cashFlowTable = w->getCashFlowTable();
        map <int, cashflowData_t> & table = beBusinessMgr->getCashflowManager()->getCashflowTable();
        int rowCount = beBusinessMgr->getCashflowManager()->totItems;
        int row = 0;
        for(map<int,cashflowData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            if(it->first > w->startCashFlowId )
                w->startCashFlowId = it->first;
            cashFlowTable->insertRow(row);
            for(int col=CASHFLOW_ID; col < CASHFLOW_END; col++)
            {
                if(!cashFlowTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,beBusinessMgr->getCashflowManager()->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    cashFlowTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:Cash Flow data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no Cash Flow data populated" << endl;
    if(beBusinessMgr->isSummaryDataRead)
    {
        QTableWidget *sumTable = w->getSummaryTable();
        map <int, sumData_t> & table = beBusinessMgr->getSumManager()->getSumTable();
        int rowCount = beBusinessMgr->getSumManager()->totItems;
        int row = 0;
        for(map<int,sumData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            if(it->first > w->startSumId )
                w->startSumId = it->first;
            sumTable->insertRow(row);
            for(int col=SUM_ID; col < SUM_END; col++)
            {
                if(!sumTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,beBusinessMgr->getSumManager()->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    sumTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:Summary data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no Summary data populated" << endl;
    return ERR_NONE;
}
