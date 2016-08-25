#include <QVariant>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <fstream>

#include "SqlDBManager/include/beDataBaseHandler.h"
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
         for(int tab = TABLE_NONE + 1; tab < TABLE_END; tab++){
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

     return ERR_NONE;
}

void BE_DatabaseHandler::deInitDB()
{

}

/* Get database object */
QSqlDatabase * BE_DatabaseHandler::getDataBaseObj()
{
    fout<<"In BE_DatabaseHandler::getDataBaseObj()"<<endl;
    return beDB;
}

/* ------------------ Vitual function definitions ------------------------*/
errorType BE_DatabaseHandler::readDataBase(tableType tbl, int startIndex, int endIndex,
                                           matrix *records, int *totItemes)
{
    fout<<"In BE_DatabaseHandler::readDataBase"<<endl;
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_DatabaseHandler::readDataBase:Database Read Error"<<endl;
        return ERR_DB_OPEN;
    }
    else{
        fout<<"BE_DatabaseHandler::readDataBase: "<< table_name[tbl] <<" database Opened"<<endl;
        QSqlQuery query(*beDB);
        int rowEntry = 0;
        string qry = getTableEntriesQuery(tbl);
        if(!query.exec( qry.c_str()))
        {
            fout<<"BE_DatabaseHandler::readDataBase:"<<query.lastError().text().toStdString()<<endl;
            qDebug()<<query.lastError()<<endl;
        }
        fout<<"BE_DatabaseHandler :: readDataBase"<<getLastQuery(query).toStdString()<<endl;
        if(query.record().isEmpty()){
            fout<<"BE_DatabaseHandler::readDataBase:No entries are there"<<endl;
            beDB->close();
            return ERR_NONE;
        }
        else{
            while(query.next()){
                QSqlRecord record = query.record();
                matrow oneEntry;
                for(int item = startIndex; item < endIndex; item ++)
                {
                    int colIndx = record.indexOf(table_fields[tbl][item]);
                    oneEntry.push_back(query.value(colIndx).toString().toStdString());
                }
                (records)->push_back(oneEntry);
                rowEntry++;
            }
        }
        *totItemes = rowEntry;
        if(*totItemes <= 0)
            fout<<"BE_DatabaseHandler:: readDataBase:" <<  table_name[tbl] << " No Data"<<endl;
        fout<<"BE_DatabaseHandler::readDataBase:database Has been closed"<<endl;
        beDB->close();
    }
    return ERR_NONE;
}

errorType BE_DatabaseHandler::addIntoDataBase(tableType tbl, int startIndex, int endIndex,
                                              unsigned int purId, matrow *str)
{
    fout<<"In BE_DatabaseHandler::addIntoDataBase"<<endl;
    qRegisterMetaType<std::string>("std::string");
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    errorType err = ERR_NONE;
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_DatabaseHandler::addIntoDataBase:Cann't be added into the database"<<endl;
        return ERR_DB_OPEN;
    }
    else{

        QSqlQuery query(*beDB);
        string qry = getTableInsertQuery(tbl, startIndex, endIndex);
        query.prepare(qry.c_str());
        string tableField(table_fields[tbl][startIndex]);
        std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
        tableField = ":" + tableField;
        query.bindValue(tableField.c_str(), purId);
        for(int iter = startIndex + 1; iter < endIndex; iter++)
        {
            string tableField(table_fields[tbl][iter]);
            std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
            tableField = ":" + tableField;
            query.bindValue(tableField.c_str(), QString((str->at(iter)).c_str()));
        }
        if(!query.exec()){
            err = ERR_DB_ADD;
            fout<<"BE_DatabaseHandler :: addIntoDataBase:Error:" << query.lastError().text().toStdString()<<endl;
            qDebug() << "drivers: "<< query.lastError();
        }
        fout<<"BE_DatabaseHandler :: addIntoDataBase"<<getLastQuery(query).toStdString()<<endl;
        beDB->close();
    }
    return err;
}

errorType BE_DatabaseHandler::deleteFromDataBase(tableType tbl, unsigned int purId){
    fout<<"In BE_DatabaseHandler::deleteFromDataBase"<<endl;
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    errorType err = ERR_NONE;
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_DatabaseHandler::deleteFromDataBase:Cann't be added into the database"<<endl;
         return ERR_DB_OPEN;
    }
    else{
         QSqlQuery query(*beDB);
         fout<<"BE_DatabaseHandler::deleteFromDataBase:Deleting query"<<endl;
         string qry = getTableDeleteQuery(tbl);
         query.prepare(qry.c_str());
         string tableField(table_fields[tbl][0]);
         std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
         tableField = ":" + tableField;
         query.bindValue(tableField.c_str(), purId);
         if(!query.exec())
            err = ERR_DB_DEL;
         fout<<"BE_DatabaseHandler :: deleteFromDataBase"<<getLastQuery(query).toStdString()<<endl;
         beDB->close();
    }
    return err;
}

errorType BE_DatabaseHandler::editIntoDataBase(tableType tbl,  int startIndex,int endIndex,
                                               unsigned int purId, matrow *input)
{

    fout<<"In BE_DatabaseHandler::editIntoDataBase"<<endl;
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    errorType err = ERR_NONE;
    fout<<"BE_PurTableHandler::editIntoDataBase:editing Purchase Table"<<endl;
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_DatabaseHandler::editIntoDataBase:Cann't be added into the database"<<endl;
        return ERR_DB_OPEN;
    }
    else{

        QSqlQuery query(*beDB);
        string qry = getTableUpdateQuery(tbl, startIndex, endIndex);
        query.prepare(qry.c_str());

        for(int iter = startIndex + 1; iter < endIndex; iter++){
            query.addBindValue(QString((input->at(iter)).c_str()));
        }
        query.addBindValue(purId);
        if(!query.exec())
            err = ERR_DB_EDIT;
        qDebug() << "drivers: "<< query.lastError();
        fout<<"BE_PurTableHandler :: editIntoDataBase"<<getLastQuery(query).toStdString()<<endl;
        beDB->close();
    }
    return err;
}
errorType BE_DatabaseHandler::editFieldIntoDataBase(tableType tbl, unsigned int purId,
                                                    string *field, string *input)
{
    fout<<"In BE_DatabaseHandler::editFieldIntoDataBase"<<endl;
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    errorType err = ERR_NONE;
    fout<<"BE_DatabaseHandler::editFieldIntoDataBase:editing Purchase Table"<<endl;
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_DatabaseHandler::editFieldIntoDataBase:Cann't be added into the database"<<endl;
        return ERR_DB_OPEN;
    }
    else{

        QSqlQuery query(*beDB);
        QString qStm = QString();
        qStm = "UPDATE " + QString(table_name[tbl]) +
                " SET "+ QString(field->c_str()) +
                "=? WHERE " + QString(table_fields[tbl][0]) +"=?";
        query.prepare(qStm);
        query.addBindValue(QString(input->c_str()));
        query.addBindValue(purId);
        if(!query.exec())
            err = ERR_DB_EDIT;
        fout<<"BE_DatabaseHandler :: editFieldIntoDataBase"<<getLastQuery(query).toStdString()<<endl;
        beDB->close();
    }
    return (errorType)err;
}

errorType BE_DatabaseHandler::deleteAllTableEntries(tableType tbl){
    fout<<"In BE_DatabaseHandler::deleteAllTableEntries"<<endl;
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    errorType err = ERR_NONE;
    fout<<"BE_DatabaseHandler::deleteAllTableEntries:deleting Purchase Table"<<endl;
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_DatabaseHandler::deleteAllTableEntries:Cann't delete from the database"<<endl;
        return ERR_DB_OPEN;
    }
    else{

        QSqlQuery query(*beDB);
        string qry = getTableDeleteAllEntriesQuery(tbl);
        if(!query.exec( qry.c_str()))
        {
            fout<<"BE_DatabaseHandler::deleteAllTableEntries:"<<query.lastError().text().toStdString()<<endl;
            qDebug()<<query.lastError()<<endl;
            err = ERR_DB_DEL_ALL;
        }

        fout<<"BE_DatabaseHandler::deleteAllTableEntries"<<getLastQuery(query).toStdString()<<endl;
        beDB->close();
    }
    return (errorType)err;
}
