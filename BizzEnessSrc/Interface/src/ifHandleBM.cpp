#include <fstream>
#include "Interface/include/ifHandleBM.h"
#include "common/include/common.h"
#include "BusinessManager/include/beBusinessManager.h"
#include "BusinessManager/include/bePurchaseManager.h"
#include "BusinessManager/include/beStockManager.h"

extern ofstream fout;

/*
 * This function is used to Initialize Backend Business Manager
 */
errorType initializeBManager(){
    errorType errorCode = ERR_NONE;
    BE_BusinessManager *beBusinessManagerObj = 0;
    beBusinessManagerObj = BE_BusinessManager::getInstance();
    if(!beBusinessManagerObj){
        errorCode =  ERR_BMANAGER_INIT;
        fout << "error("<<errorCode<<"):ifCommon.cpp:initializeBManager:Businessmanager couldn't be initialized:" <<endl;
    }
    fout << "info:ifCommon.cpp:initializeBManager:done"<<endl;
    return errorCode;
}

/*
 * This function is used to De-Initialize Backend Business Manager
 */
void deinitializeBManager()
{
    fout<<"ifCommon.cpp:Deinitializing BManager()"<<endl;
    BE_BusinessManager *beBusinessMgr = 0;
    beBusinessMgr = BE_BusinessManager::getInstance();
    if(beBusinessMgr){
        delete beBusinessMgr;
        beBusinessMgr='\0';
    }
    else{
        fout << "error:ifCommon.cpp:deinitializeBManager:Business Manager couldn't be fetched" <<endl;
    }
    fout << "info:ifCommon.cpp:deinitializeBManager:done"<<endl;
}

/*
 * This function is used to update the data stored in BUSINESSMANAGER Tables
 * It takes the table name to update, the table entry identified by id and column
 * and the data to be used for update
 */
errorType updateToBusinessManager(tableType tableId, unsigned  int id, int col, string data)
{
    errorType errorCode = ERR_NONE;
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    if(tableId == TABLE_STOCK)
        (beMangObj->getStockManager())->updateItem(id, col,data);
    else if(tableId == TABLE_PURCHASE)
        (beMangObj->getPurchaseManager())->updateItem(id, col,data);
    else if(tableId == TABLE_SALES)
        (beMangObj->getSalesManager())->updateItem(id, col,data);
    else if(tableId == TABLE_PL)
        (beMangObj->getPLManager())->updateItem(id, col,data);
    else if(tableId == TABLE_CASHFLOW)
        (beMangObj->getCashflowManager())->updateItem(id, col,data);
    else if(tableId == TABLE_SUMMARY)
        (beMangObj->getSumManager())->updateItem(id, col,data);
    else
    {
        fout <<"error("<<ERR_TABLENAME<<"):ifcommon.cpp:updateToBusinessManager: no table_name matched for update" << endl;
        return ERR_TABLENAME;
    }
    if(errorCode){
        fout <<"error("<<errorCode<<"):ifcommon.cpp:updateToBusinessManager:"
            <<string(table_name[tableId])<< "Table item update error:"<< endl;
    }
    return errorCode;
}

/*
 * This function is used to add a new the data initialized with with null values
 * into BUSINESSMANAGER Table. It takes the table name to add, and an id to identify
 * the table entry. This id is genreated by our App only and sent to the BUSINESSMANAGER
 */
errorType addItemToBusinessManager(tableType tableId, unsigned int id)
{
    errorType errorCode = ERR_NONE;
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    if(tableId == TABLE_STOCK)
        errorCode = (beMangObj->getStockManager())->addNewItem(id);
    else if(tableId == TABLE_PURCHASE)
        errorCode = (beMangObj->getPurchaseManager())->addNewItem(id);
    else if(tableId == TABLE_SALES)
        errorCode = (beMangObj->getSalesManager())->addNewItem(id);
    else if(tableId == TABLE_PL)
        errorCode = (beMangObj->getPLManager())->addNewItem(id);
    else if(tableId == TABLE_CASHFLOW)
        errorCode = (beMangObj->getCashflowManager())->addNewItem(id);
    else if(tableId == TABLE_SUMMARY)
        errorCode = (beMangObj->getSumManager())->addNewItem(id);
    else
    {
        fout <<"error("<<ERR_TABLENAME<<"):ifcommon.cpp:addItemToBusinessManager: no table_name matched for add" << endl;
        return ERR_TABLENAME;
    }
    if(errorCode){
        fout <<"error("<<errorCode<<"):ifcommon.cpp:addItemToBusinessManager:"
            <<string(table_name[tableId])<< "Table add row error:"<< endl;
    }
    else{
        fout <<"info:ifcommon.cpp:addItemToBusinessManager: "<<string(table_name[tableId])<< "Table row added"<< endl;
    }
    return errorCode;
}

/*
 * This function is used to save all the data with BUSINESSMANAGER tables
 * to data File in csv format which will be read next time when app starts
 */
errorType saveRecordsInCSV(tableType tableId)
{
    errorType errorCode = ERR_NONE;
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    if(tableId == TABLE_STOCK)
        errorCode = (beMangObj->getStockManager())->saveItems();
    else if(tableId == TABLE_PURCHASE)
        errorCode = (beMangObj->getPurchaseManager())->saveItems();
    else if(tableId == TABLE_SALES)
        errorCode = (beMangObj->getSalesManager())->saveItems();
    else if(tableId == TABLE_PL)
        errorCode = (beMangObj->getPLManager())->saveItems();
    else if(tableId == TABLE_CASHFLOW)
        errorCode = (beMangObj->getCashflowManager())->saveItems();
    else if(tableId == TABLE_SUMMARY)
        errorCode = (beMangObj->getSumManager())->saveItems();
    else {
        fout <<"error("<<ERR_TABLENAME<<"):ifcommon.cpp:saveToBusinessManager:no table_name matched for save"<< endl;
        return ERR_TABLENAME;
    }
    if(errorCode){
        fout <<"error("<<errorCode<<"):ifcommon.cpp:saveToBusinessManager:"
            <<string(table_name[tableId])<< "Table save error:"<< endl;
    }
    else{
        fout <<"info:ifcommon.cpp:saveToBusinessManager: "<<string(table_name[tableId])<< "Table saved"<< endl;
    }
    return errorCode;
}

/*
 * This function is used to delete an entry from BUSINESSMANAGER Table
 * the entry is identified by the table name an the unique id assigned
 * to the table entry
 */
errorType delItemFromBusinessManager(tableType tableId, unsigned int id)
{
    errorType errorCode = ERR_NONE;
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    if(tableId == TABLE_STOCK)
        errorCode = (beMangObj->getStockManager())->deleteItem(id);
    else if(tableId == TABLE_PURCHASE)
        errorCode = (beMangObj->getPurchaseManager())->deleteItem(id);
    else if(tableId == TABLE_SALES)
        errorCode = (beMangObj->getSalesManager())->deleteItem(id);
    else if(tableId == TABLE_PL)
        errorCode = (beMangObj->getPLManager())->deleteItem(id);
    else if(tableId == TABLE_CASHFLOW)
        errorCode = (beMangObj->getCashflowManager())->deleteItem(id);
    else if(tableId == TABLE_SUMMARY)
        errorCode = (beMangObj->getSumManager())->deleteItem(id);
    else
    {
        fout <<"error("<<ERR_TABLENAME<<"):ifcommon.cpp:delItemFromBusinessManager: no table_name matched for delete" << endl;
        return ERR_TABLENAME;
    }
    if(errorCode){
        fout <<"error("<<errorCode<<"):ifcommon.cpp:delItemFromBusinessManager:"
            <<string(table_name[tableId])<< "Table row delete error:" <<endl;

    }
    else{
        fout <<"info:ifcommon.cpp:delItemFromBusinessManager:"<<string(table_name[tableId])
             << "Table row deleted"<< endl;
    }
    return errorCode;
}

/* This function populates the GUI data to Business Manager */
errorType populatePurWindowData(unsigned int id, int flds , string data)
{
    errorType errorCode = ERR_NONE;
    switch(flds){
    case PURUI_NAME:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_PROD, data);
        break;
    case PURUI_BATCH:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_BATCH, data);
        break;
    case PURUI_BOXNO:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_BOX, data);
        break;
    case PURUI_PCSNO:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_ITEMS, data);
        break;
    case PURUI_PCSPERBOX:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_PCSPERBOX, data);
        break;
    case PURUI_COSTOFBOX:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_COSTOFBOX, data);
        break;
    case PURUI_COSTOFPCS:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_COSTOFPCS, data);
        break;
    case PURUI_TAX:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_TAX, data);
        break;
    case PURUI_EXPNS:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_EXPNS, data);
        break;
    case PURUI_TOTALCOST:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_TOTCOST, data);
        break;
    case PURUI_CASHPAID:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_CASHPAID, data);
        break;
    case PURUI_CHEQPAID:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_CHEQPAID, data);
        break;
    case PURUI_TOTALPAID:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_PAID, data);
        break;
    case PURUI_DUE:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_DUE, data);
        break;
    case PURUI_BILL:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_BILLNO, data);
        break;
    case PURUI_PURNO:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_NO, data);
        break;
    case PURUI_DATE:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_DATE, data);
        break;
    case PURUI_COMP:
       updateToBusinessManager(TABLE_PURCHASE, id, PUR_SUPP, data);
        break;
    case PURUI_ADDR:
       updateToBusinessManager(TABLE_PURCHASE, id, PUR_SUPPADDR, data);
        break;
    case PURUI_CONTACT:
       updateToBusinessManager(TABLE_PURCHASE, id, PUR_CONTACT, data);
       break;
    case PUTRUI_CONTACTNO:
       updateToBusinessManager(TABLE_PURCHASE, id, PUR_CONTACTNO, data);
        break;
    case PURUI_REMARKS:
        updateToBusinessManager(TABLE_PURCHASE, id, PUR_REMARKS, data);
        break;
    default:
        break;
    }
    return errorCode;
}

errorType prepareRecord(tableType tbl, unsigned int id, matrow **record)
{
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    int err = 0;
    switch(tbl)
    {
        case TABLE_PURCHASE:
            *record = new matrow();
            err = beMangObj->getPurchaseManager()->getRecord(id,*record);
            break;
        case TABLE_STOCK:
            *record = new matrow();
            err = beMangObj->getStockManager()->getRecord(id,*record);
            break;
        default:
            break;
    }
    return (errorType)err;
}
