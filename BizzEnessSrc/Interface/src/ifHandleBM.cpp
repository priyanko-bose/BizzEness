#include <fstream>
#include "Interface/include/ifHandleBM.h"
#include "common/include/common.h"
#include "BusinessManager/include/beBusinessManager.h"
#include "BusinessManager/include/bePurchaseManager.h"
#include "BusinessManager/include/beStockManager.h"
#include "Interface/include/ifHandleDB.h"

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
errorType updateToBusinessManager(tableType tableId, unsigned  int id, int col, string data, int updateOP)
{
    errorType errorCode = ERR_NONE;
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    if(tableId == TABLE_STOCK)
        (beMangObj->getStockManager())->updateItem(id, col,data, updateOP);
    else if(tableId == TABLE_PURCHASE)
        (beMangObj->getPurchaseManager())->updateItem(id, col,data, updateOP);
    else if(tableId == TABLE_SALES)
        (beMangObj->getSalesManager())->updateItem(id, col,data, updateOP);
    else if(tableId == TABLE_PL)
        (beMangObj->getPLManager())->updateItem(id, col,data, updateOP);
    else if(tableId == TABLE_CASHFLOW)
        (beMangObj->getCashflowManager())->updateItem(id, col,data, updateOP);
    else if(tableId == TABLE_SUMMARY)
        (beMangObj->getSumManager())->updateItem(id, col,data, updateOP);
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
 * This function is used to set the data stored in BUSINESSMANAGER Tables
 * It takes the table name to set, the table entry identified by id and column
 * and the data to be used for set
 */
errorType setItemToBusinessManager(tableType tableId, unsigned  int id, int col, string data)
{
    errorType errorCode = ERR_NONE;
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    if(tableId == TABLE_STOCK)
        (beMangObj->getStockManager())->setItem(id, col,data);
    else if(tableId == TABLE_PURCHASE)
        (beMangObj->getPurchaseManager())->setItem(id, col,data);
    else if(tableId == TABLE_SALES)
        (beMangObj->getSalesManager())->setItem(id, col,data);
    else if(tableId == TABLE_PL)
        (beMangObj->getPLManager())->setItem(id, col,data);
    else if(tableId == TABLE_CASHFLOW)
        (beMangObj->getCashflowManager())->setItem(id, col,data);
    else if(tableId == TABLE_SUMMARY)
        (beMangObj->getSumManager())->setItem(id, col,data);
    else
    {
        fout <<"error("<<ERR_TABLENAME<<"):ifcommon.cpp:setItemToBusinessManager: no table_name matched for update" << endl;
        return ERR_TABLENAME;
    }
    if(errorCode){
        fout <<"error("<<errorCode<<"):ifcommon.cpp:setItemToBusinessManager:"
            <<string(table_name[tableId])<< "Table item set error:"<< endl;
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

errorType updateItemKeyToBusinessManager(tableType tableId, unsigned int oldKey, unsigned int newKey)
{
    int errorCode = 0;
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();
    matrow record;
    if(tableId == TABLE_STOCK){
        BE_StockManager *stockMgr = beMangObj->getStockManager();
        errorCode = stockMgr->getRecord(oldKey, &record);
        if(!errorCode){
            errorCode |= stockMgr->addNewItem(newKey);
            for(int iter =  PROD_ID; iter < PROD_END; iter++)
                errorCode |= stockMgr->setItem(newKey, iter, record[iter]);
            errorCode |= stockMgr->deleteItem(oldKey);
            errorCode |= saveRecordInDB(TABLE_STOCK, newKey, &record);
        }
    }
    else if(tableId == TABLE_PURCHASE){
        BE_PurchaseManager *purMgr = beMangObj->getPurchaseManager();
        errorCode = purMgr->getRecord(oldKey, &record);
        if(!errorCode){
            errorCode |= purMgr->addNewItem(newKey);
            for(int iter =  PUR_ID; iter < PUR_ID; iter++)
                errorCode |= purMgr->setItem(newKey, iter, record[iter]);
            errorCode |= purMgr->deleteItem(oldKey);
            errorCode |= saveRecordInDB(TABLE_PURCHASE, newKey, &record);
        }
    }
//    else if(tableId == TABLE_SALES)
//        errorCode = (beMangObj->getSalesManager())->deleteItem(id);
//    else if(tableId == TABLE_PL)
//        errorCode = (beMangObj->getPLManager())->deleteItem(id);
//    else if(tableId == TABLE_CASHFLOW)
//        errorCode = (beMangObj->getCashflowManager())->deleteItem(id);
//    else if(tableId == TABLE_SUMMARY)
//        errorCode = (beMangObj->getSumManager())->deleteItem(id);
    else
    {
        fout <<"error("<<ERR_TABLENAME<<"):ifcommon.cpp:updateItemKeyToBusinessManager: no table_name matched for update" << endl;
        return ERR_TABLENAME;
    }
    return (errorType)errorCode;
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
