#include <fstream>
#include "Interface/include/ifHandleDB.h"
#include "BusinessManager/include/beBusinessManager.h"
#include "BusinessManager/include/bePurchaseManager.h"
#include "BusinessManager/include/beStockManager.h"
#include "SqlDBManager/include/beDataBaseHandler.h"
#include <QDebug>

extern ofstream fout;

/*
 * This function is used to initialize SQL database manager abd database connection
 */
errorType  initializeDBManager()
{
    int errorCode = 0;
    BE_DatabaseHandler *beDbObj = 0;
    beDbObj = BE_DatabaseHandler::getDataBaseHandlerInstance();
    if(!beDbObj){
        errorCode = ERR_DBMANAGER_INIT;
        fout << "error("<<errorCode<<"):ifHandleDB.cpp:initializeBManager:Businessmanager couldn't be initialized:" <<endl;
        return (errorType)errorCode;
    }
    errorCode |= beDbObj->initDB();
    errorCode |= readRecords();
    return (errorType)errorCode;
}

errorType deinitializeDBManager()
{
    BE_DatabaseHandler *beDbObj = 0;
    beDbObj = BE_DatabaseHandler::getDataBaseHandlerInstance();
    if(beDbObj){
        beDbObj->deInitDB();
    }
    return ERR_NONE;
}

/* Read from database and populate into Businbess Manager */
errorType readRecords()
{
    BE_DatabaseHandler *beDbObj = BE_DatabaseHandler::getDataBaseHandlerInstance();
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();

    fout<<"info:ifHandleDB.cpp:initializeRecord:reading data from Purchase database"<<endl;
    int totItems = 0;
    matrix records;
    int errorCode = ERR_NONE;
    errorCode |= beDbObj->readDataBase(TABLE_PURCHASE, PUR_ID, PUR_END, &records, &totItems);
    for(int iter=0; iter < totItems; iter++)
        errorCode |= beMangObj->getPurchaseManager()->insertSavedItem(&records.at(iter));
    records.clear();
    if(totItems){
        beMangObj->isPurchaseDataRead = true;
        beMangObj->getPurchaseManager()->totItems = totItems;
    }

    /* fout<<"info:ifHandleDB.cpp:readRecords:reading data from STOCKdatabase"<<endl;
    int totItems = 0;
    string **records = 0;
    errorCode = ERR_NONE;
    errorCode |= beDbObj->stockTableHandler->readDataBase(matrix, &totItems);
    errorCode |= beMangObj->getStockManager()->readFileData(matrix, totItems);
    if(!errorCode)
        beMangObj->isStockDataRead = true;

    totItems = 0;
    matrix = 0;
    errorCode = ERR_NONE;
    errorCode = beMangObj->getSalesManager()->readFileData(&totItems);
    if(totItems)
        beMangObj->isSalesDataRead = true;

    totItems = 0;
    errorCode = beMangObj->getPLManager()->readFileData(&totItems);
    if(totItems)
        beMangObj->isPLDataRead = true;

    totItems = 0;
    errorCode = beMangObj->getCashflowManager()->readFileData(&totItems);
    if(totItems)
        beMangObj->isCashFlowDataRead = true;

    totItems = 0;
    errorCode = beMangObj->getSumManager()->readFileData(&totItems);
    if(totItems)
        beMangObj->isSummaryDataRead = true;*/


    if(errorCode)
        fout<<"error("<<errorCode<<"):ifHandleDB.cpp:initializeRecord:reading data from saved database failed!!"<<endl;
    else
        fout<<"info:ifHandleDB.cpp:initializeRecord:reading data from saved database complete!!"<<endl;

    return (errorType)errorCode;
}

/*Save data from Business Manager to DB Manager */
errorType saveRecordsInDB()
{
    int errorCode = 0 ;
    errorCode |= saveTableRecords(TABLE_PURCHASE);
    return (errorType)errorCode;
}

/*Save data from Business Manager to DB Manager by table name*/
errorType saveTableRecords(tableType type)
{
    int errorCode = 0 ;
    BE_DatabaseHandler *beDbObj = BE_DatabaseHandler::getDataBaseHandlerInstance();
    BE_BusinessManager *beMangObj = BE_BusinessManager::getInstance();

    int totItems = 0;
    matrix *records = 0;
    errorCode = ERR_NONE;

    if(type == TABLE_PURCHASE){
        fout<<"info:ifHandleDB.cpp:saveRecordsInDB:saving data into Purchase database"<<endl;
        errorCode |= beMangObj->getPurchaseManager()->readBMData(&records, &totItems);
        for(int iter=0; iter < totItems; iter++){
            fout << records->at(iter).at(0);
            errorCode |= beDbObj->editIntoDataBase(type, PUR_ID, PUR_END,
                                                   atol(records->at(iter).at(0).c_str()) , &records->at(iter));
        }
    }
    totItems = 0;
    delete records;
    records = '\0';

    if(errorCode)
        fout<<"error("<<errorCode<<"):ifHandleDB.cpp:saveRecordsInDB:saving data into saved database failed!!"<<endl;
    else
        fout<<"info:ifHandleDB.cpp:initializeRecord:saving data into saved database complete!!"<<endl;

    return (errorType)errorCode;
}

/* Add one record row in a table to database */
errorType saveRecordInDB(tableType tbl, unsigned int id, matrow *record)
{
    int errorCode = 0 ;
    BE_DatabaseHandler *beDbObj = BE_DatabaseHandler::getDataBaseHandlerInstance();
    switch(tbl){
        case TABLE_PURCHASE:
            errorCode = beDbObj->addIntoDataBase(TABLE_PURCHASE, PUR_ID, PUR_END, id, record);
            break;
        default:
            break;
    }
    delete record;
    record = '\0';
    return (errorType)errorCode;
}

/* Add one record row in a table to database */
errorType delFromDataBase(tableType tbl, unsigned int id)
{
    int errorCode = 0 ;
    BE_DatabaseHandler *beDbObj = BE_DatabaseHandler::getDataBaseHandlerInstance();
    switch(tbl){
        case TABLE_PURCHASE:
            errorCode = beDbObj->deleteFromDataBase(TABLE_PURCHASE, id);
            break;
        default:
            break;
    }
    return (errorType)errorCode;
}
