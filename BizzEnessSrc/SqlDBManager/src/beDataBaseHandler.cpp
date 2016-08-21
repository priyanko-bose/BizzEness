#include <QVariant>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <fstream>

#include "SqlDBManager/include/beDataBaseHandler.h"
#include "SqlDBManager/include/bePurTableHandler.h"
#include "SqlDBManager/include/beDBUtils.h"


using namespace std;
extern ofstream fout;

BE_DatabaseHandler * BE_DatabaseHandler:: beDatabaseHandlerObj='\0';

/* Databse Handler Singleton class */
BE_DatabaseHandler * BE_DatabaseHandler::getDataBaseHandlerInstance()
{
    fout<<"BE_DatabaseHandler * BE_DatabaseHandler::getDataBaseHandlerInstance()"<<endl;
    if(beDatabaseHandlerObj=='\0'){
        beDatabaseHandlerObj=new BE_DatabaseHandler();
    }
    return beDatabaseHandlerObj;
}

/* Database handler constructor */
BE_DatabaseHandler::BE_DatabaseHandler()
{
    fout<<"In BE_DatabaseHandler::BE_DatabaseHandler()"<<endl;
}

/* Database Handler destructor */
BE_DatabaseHandler::~BE_DatabaseHandler()
{
    fout<<"BE_DatabaseHandler::~BE_DatabaseHandler"<<endl;
}

errorType BE_DatabaseHandler::initDB()
{
     beDB= new QSqlDatabase();
     *beDB=QSqlDatabase::addDatabase("QSQLITE","beDataBaseConnection");
     beDB->setDatabaseName("BIZNESSDATA.db");
     beDB->setHostName("HOMECOMPANY");
     beDB->setUserName("Priyanko");
     beDB->setPassword("nemo");
     if(beDB->isValid()){
        fout<<"BE_DatabaseHandler::BE_DatabaseHandler:Valid DataBase has been created"<<endl;
     }
     else {
        fout<<"BE_DatabaseHandler::BE_DatabaseHandler:DataBase Invalid"<<endl;
        return ERR_DB_INVALID;
     }
     bool ok=beDB->open();
     if(ok){
         QStringList s=beDB->tables();
         for(int tab = TABLE_SUMMARY; tab < TABLE_END; tab++){
             if(s.contains(table_name[tab])){
                 fout<<"BE_DatabaseHandler::BE_DatabaseHandler:Table already Exists"<<endl;
                 continue;
             }
             else{
                 QSqlQuery qr(*beDB);
                 if(tab == TABLE_PURCHASE){
                     string qry = getTableCreateQuery(TABLE_PURCHASE, PUR_ID, PUR_END);
                     bool val = qr.exec(qry.c_str());
                     if(!val)
                         fout<<"BE_PurTableHandler :: addIntoDataBase:Error:" << qr.lastError().text().toStdString()<<endl;
                     else
                         fout<<"BE_DatabaseHandler::BE_DatabaseHandler:Blank "<<table_name[tab]<<" Table Created"<<endl;
                 }
                 /*else if(tab == TABLE_STOCK){
                     qr.exec(getStockTableCreateQuery());
                     if(qr.record().isEmpty())
                         fout<<"BE_DatabaseHandler::BE_DatabaseHandler:Blank "<<table_name[tab]<<" Table Created"<<endl;
                 }*/
                 else
                     continue;
             }
         }
         beDB->close();
         fout<<"BE_DatabaseHandler::BE_DatabaseHandler:database Has been closed"<<endl;

     }
     else{
         fout<<"BE_DatabaseHandler::BE_DatabaseHandler:error in opening file"<<endl;
         return ERR_DB_OPEN;
     }

     purTableHandler = new BE_PurTableHandler();
     if(!purTableHandler)
         return ERR_PUR_DB_CREATE;
     //stockTableHandler = new BE_StockTableHandler();
     return ERR_NONE;
}

void BE_DatabaseHandler::deInitDB()
{
    if(this->purTableHandler){
        delete this->purTableHandler;
        this->purTableHandler = '\0';
    }
    //delete stockTableHandler;
}

/* Get database object */
QSqlDatabase * BE_DatabaseHandler::getDataBaseObj()
{
    fout<<"In QSqlDatabase & BE_DatabaseHandler::getDataBaseObj()"<<endl;
    return beDB;
}

/* ------------------ Vitual function definitions ------------------------*/
errorType BE_DatabaseHandler::readDataBase(matrix *, int *)
{
    fout<<"In BE_DatabaseHandler::readDataBase"<<endl;
    return ERR_NONE;
}

errorType BE_DatabaseHandler::addIntoDataBase(unsigned int ,matrow *)
{
    fout<<"In BE_DatabaseHandler::addIntoDataBase"<<endl;
    return ERR_NONE;
}

errorType BE_DatabaseHandler::deleteFromDataBase(unsigned int ){
    fout<<"In BE_DatabaseHandler::deleteFromDataBase"<<endl;
    return ERR_NONE;
}

errorType BE_DatabaseHandler::editIntoDataBase(unsigned int ,matrow *){

    fout<<"In BE_DatabaseHandler::editIntoDataBase"<<endl;
    return ERR_NONE;
}
errorType BE_DatabaseHandler::editFieldIntoDataBase(unsigned int , string *, string *)
{
    fout<<"In BE_DatabaseHandler::editFieldIntoDataBase"<<endl;
    return ERR_NONE;
}

errorType BE_DatabaseHandler::deleteAllTableEntries(){
    fout<<"In BE_DatabaseHandler::deleteAllTableEntries"<<endl;
    return ERR_NONE;
}
