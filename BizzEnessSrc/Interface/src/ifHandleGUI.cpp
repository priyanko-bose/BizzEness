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
    BE_MainWindow *w = BE_MainWindow :: getBeMainWindow();

    //Restore All Table data from all places --start
    for(int tbl = TABLE_NONE + 1; tbl < TABLE_END; tbl++  ){
        int noOfItems = 0;
        int totItems = 0;
        int rowCount = 0;
        matrix *records = 0;
        switch(tbl){
            case TABLE_PURCHASE:
                fout<<"info:ifHandleGUI.cpp:importTableData:Purchase database"<<endl;
                errorCode |= beDbObj->deleteAllTableEntries(TABLE_PURCHASE);
                errorCode |= beMangObj->getPurchaseManager()->deleteAllItems();
                errorCode |= beMangObj->getPurchaseManager()->readFileData(path, &noOfItems);
                beMangObj->isPurchaseDataRead = true;
                beMangObj->getPurchaseManager()->totItems = noOfItems;
                errorCode |= beMangObj->getPurchaseManager()->readBMData(&records, &totItems);

                for(int iter=0; iter < totItems; iter++){
                    fout << records->at(iter).at(0);
                        errorCode |= beDbObj->addIntoDataBase((tableType)tbl, PUR_ID, PUR_END,
                                atol(records->at(iter).at(0).c_str()) , &records->at(iter));
                }
                rowCount = w->getPurchaseTable()->rowCount();
                for(int it = 0; it < rowCount; it++)
                    w->getPurchaseTable()->removeRow(0);
                break;
            case TABLE_STOCK:
                fout<<"info:ifHandleGUI.cpp:importTableData:Stock database"<<endl;
                errorCode |= beDbObj->deleteAllTableEntries(TABLE_STOCK);
                errorCode |= beMangObj->getStockManager()->deleteAllItems();
                errorCode |= beMangObj->getStockManager()->readFileData(path, &noOfItems);
                beMangObj->isStockDataRead = true;
                beMangObj->getStockManager()->totItems = noOfItems;
                errorCode |= beMangObj->getStockManager()->readBMData(&records, &totItems);

                for(int iter=0; iter < totItems; iter++){
                    fout << records->at(iter).at(0);
                        errorCode |= beDbObj->addIntoDataBase((tableType)tbl, PROD_ID, PROD_END,
                                atol(records->at(iter).at(0).c_str()) , &records->at(iter));
                }
                rowCount = w->getStockTable()->rowCount();
                for(int it = 0; it < rowCount; it++)
                    w->getStockTable()->removeRow(0);
                break;
            default:
                break;
        }
        totItems = 0;
        if(records)
            delete records;
        records = '\0';
    }
    //Restore Table data from all places -- end
    errorCode |= populateSavedData();
    return (errorType)errorCode;
}

/*
 * Interface function to populate data read from database into table widgets
 */
errorType populateSavedData()
{
    BE_BusinessManager *beBussinessMgr = 0;
    beBussinessMgr = BE_BusinessManager::getInstance();
    if(!beBussinessMgr)
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

    if(beBussinessMgr->isStockDataRead)
    {
        QTableWidget *stockTable = w->getStockTable();
        BE_StockManager *stockMgr = beBussinessMgr->getStockManager();
        map <unsigned int, stockData_t> & table = stockMgr->getStockTable();
        int rowCount = stockMgr->totItems;
        int row = 0;
        for(map<unsigned int,stockData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            stockTable->insertRow(row);
            for(int col=PROD_ID; col < PROD_END; col++)
            {
                if(!stockTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data, stockMgr->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    if(!isAdmin || col == PROD_ID)
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    stockTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:stock data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no stock data populated" << endl;

    if(beBussinessMgr->isPurchaseDataRead)
    {
        QTableWidget *purchaseTable = w->getPurchaseTable();
        BE_PurchaseManager *purMgr = beBussinessMgr->getPurchaseManager();
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

    if(beBussinessMgr->isSalesDataRead)
    {
        QTableWidget *salesTable = w->getSalesTable();
        BE_SalesManager *saleMgr = beBussinessMgr->getSalesManager();
        map <int, salesData_t> & table = saleMgr->getSalesTable();
        int rowCount = saleMgr->totItems;
        int row = 0;
        for(map<int,salesData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            salesTable->insertRow(row);
            for(int col=SALES_ID; col < SALES_END; col++)
            {
                if(!salesTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,saleMgr->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    if(!isAdmin || col == SALES_ID)
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    salesTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:sales data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no sales data populated" << endl;

    if(beBussinessMgr->isPLDataRead)
    {
        QTableWidget *plTable = w->getPLTable();
        BE_PLManager *plMgr = beBussinessMgr->getPLManager();
        map <int, plData_t> & table = plMgr->getPLTable();
        int rowCount = plMgr->totItems;
        int row = 0;
        for(map<int,plData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            plTable->insertRow(row);
            for(int col=PL_ID; col < PL_END; col++)
            {
                if(!plTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,plMgr->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    if(!isAdmin || col == PL_ID)
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    plTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:Profit&Loss data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no Profit&Loss data populated" << endl;
    if(beBussinessMgr->isCashFlowDataRead)
    {
        QTableWidget *cashFlowTable = w->getCashFlowTable();
        BE_CashflowManager *cflwMgr = beBussinessMgr->getCashflowManager();
        map <int, cashflowData_t> & table = cflwMgr->getCashflowTable();
        int rowCount = cflwMgr->totItems;
        int row = 0;
        for(map<int,cashflowData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            cashFlowTable->insertRow(row);
            for(int col=CASHFLOW_ID; col < CASHFLOW_END; col++)
            {
                if(!cashFlowTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,cflwMgr->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    if(!isAdmin || col == CASHFLOW_ID)
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    cashFlowTable->setItem(row,col,item);
                }
            }
        }
        fout <<"info:ifHandleDB.cpp:populateSavedData:Cash Flow data populated "<<rowCount <<"items"<<endl;
    }
    else
        fout <<"info:ifHandleDB.cpp:populateSavedData:no Cash Flow data populated" << endl;
    if(beBussinessMgr->isSummaryDataRead)
    {
        QTableWidget *sumTable = w->getSummaryTable();
        BE_SumManager *sumMgr = beBussinessMgr->getSumManager();
        map <int, sumData_t> & table = sumMgr->getSumTable();
        int rowCount = sumMgr->totItems;
        int row = 0;
        for(map<int,sumData_t>::iterator it=table.begin(); it!=table.end() && row < rowCount; ++it, row++)
        {
            sumTable->insertRow(row);
            for(int col=SUM_ID; col < SUM_END; col++)
            {
                if(!sumTable->item(row,col))
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    char data[256] = {'\0',};
                    strcpy(data,sumMgr->getElement(it->second, col));
                    item->setText(data);
                    item->setToolTip(data);
                    if(!isAdmin || col == SUM_ID)
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
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
