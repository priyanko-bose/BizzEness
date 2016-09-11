#include <QApplication>
#include <fstream>
#include "Interface/include/ifHandleDB.h"
#include "Interface/include/ifHandleGUI.h"
#include "Interface/include/ifHandleBM.h"
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
 * This returns the complete list of product names
 */
QStringList *getCompanyList()
{
    list<string>::iterator it;
    QStringList *compList = '\0';
    BE_BusinessManager *beBusinessManagerObj = 0;
    beBusinessManagerObj = BE_BusinessManager::getInstance();
    if(!beBusinessManagerObj)
        return compList;
    if(list<string> *mylist = beBusinessManagerObj->getStockManager()->getStockCompanyList())
    {
        compList = new QStringList();
        for (it=mylist->begin(); it!=mylist->end(); ++it){
            *compList << QString((*it).c_str()) ;
        }
        delete mylist;
        mylist='\0';
    }
    return compList;
}

QStringList * getFilteredList(int tableID, int filterBy, int filterOn, QString filterText)
{
    list<string>::iterator it;
    QStringList *listItems = '\0';
    BE_BusinessManager *beBusinessManagerObj = 0;
    list<string> *mylist = '\0';
    beBusinessManagerObj = BE_BusinessManager::getInstance();
    if(!beBusinessManagerObj)
        return listItems;
    if(tableID == TABLE_STOCK){
       mylist = beBusinessManagerObj->getStockManager()->getFilteredList(filterBy,filterOn, filterText.toStdString());
    }
    if(mylist)
    {
        listItems = new QStringList();
        for (it=mylist->begin(); it!=mylist->end(); ++it){
            *listItems << QString((*it).c_str()) ;
        }
        delete mylist;
        mylist='\0';
    }
    return listItems;
}

QString * getTableItem(int tableID, int searchFld, QString searchFldText, int requireFld)
{
    BE_BusinessManager *beBusinessManagerObj = 0;
    beBusinessManagerObj = BE_BusinessManager::getInstance();
    if(!beBusinessManagerObj)
        return '\0';
    QString * itemText = '\0';
    string rqrText;
    if(tableID == TABLE_PURCHASE){
       int err = 0;
       if(!(err = beBusinessManagerObj->getPurchaseManager()->getItemText(searchFld,searchFldText.toStdString(),requireFld, &rqrText)))
           itemText = new QString(QString::fromStdString(rqrText));
    }
    return itemText;
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

/* This function populates the GUI data to Business Manager */
errorType populatePurWindowData(unsigned int *id, int flds , string data)
{
    errorType errorCode = ERR_NONE;
    updateOP stockOP = ADD;
    switch(flds){
    case PURUI_NAME:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_PROD, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_NAME, data, stockOP);
        break;
    case PURUI_BATCH:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_BATCH, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_BATCH, data, stockOP);
        break;
    case PURUI_BOXNO:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_BOX, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_NOB, data, stockOP);
        break;
    case PURUI_PCSNO:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_ITEMS, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_NOI, data, stockOP);
        break;
    case PURUI_PCSPERBOX:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_PCSPERBOX, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_PPB, data, stockOP);
        break;
    case PURUI_COSTOFBOX:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_COSTOFBOX, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_CPB, data, stockOP);
        break;
    case PURUI_COSTOFPCS:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_COSTOFPCS, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_CPP, data, stockOP);
        break;
    case PURUI_TAX:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_TAX, data);
        break;
    case PURUI_EXPNS:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_EXPNS, data);
        break;
    case PURUI_TOTALCOST:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_TOTCOST, data);
        break;
    case PURUI_CASHPAID:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_CASHPAID, data);
        break;
    case PURUI_CHEQPAID:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_CHEQPAID, data);
        break;
    case PURUI_TOTALPAID:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_PAID, data);
        break;
    case PURUI_DUE:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_DUE, data);
        break;
    case PURUI_BILL:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_BILLNO, data);
        break;
    case PURUI_PURNO:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_NO, data);
        break;
    case PURUI_DATE:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_DATE, data);
        updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_DATE, data, stockOP);
        break;
    case PURUI_COMP:
       setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_SUPP, data);
       updateToBusinessManager(TABLE_STOCK, id[TABLE_STOCK], PROD_COMP, data, stockOP);
        break;
    case PURUI_ADDR:
       setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_SUPPADDR, data);
        break;
    case PURUI_CONTACT:
       setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_CONTACT, data);
       break;
    case PUTRUI_CONTACTNO:
       setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_CONTACTNO, data);
        break;
    case PURUI_REMARKS:
        setItemToBusinessManager(TABLE_PURCHASE, id[TABLE_PURCHASE], PUR_REMARKS, data);
        break;
    default:
        break;
    }
    return errorCode;
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
