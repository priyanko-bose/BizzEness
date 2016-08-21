#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

#include <fstream>
#include "common/include/common.h"
#include "SqlDBManager/include/bePurTableHandler.h"
#include "BusinessManager/include/bePurchaseManager.h"
#include "SqlDBManager/include/beDBUtils.h"

// in some header file
Q_DECLARE_METATYPE (std::string)

using namespace std;
extern ofstream fout;

/*string * getPurQueryData(QSqlQuery & query)
{
    string *rec = new string[PUR_END];
    QSqlRecord record = query.record();
    int colIndx = record.indexOf(get_get_pur_table_col_name(PUR_ID));
    rec[PUR_ID] = query.value(colIndx).toSting();
    for(int item = PUR_NO; item < PUR_END; item ++)
    {
        rec[item] = query.value(get_get_pur_table_col_name(item)).toSting().toStdString();
    }
    return rec;
}*/

BE_PurTableHandler :: BE_PurTableHandler()
{
    fout<<"BE_PurTableHandler::constructor"<<endl;
}

BE_PurTableHandler::~BE_PurTableHandler()
{
    fout<<"BE_PurTableHandler::destructor"<<endl;
}

errorType BE_PurTableHandler:: readDataBase(matrix *records, int *totItemes)
{
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_PurTableHandler::readDataBase:Database Read Error"<<endl;
        return ERR_DB_OPEN;
    }
    else{
        fout<<"BE_PurTableHandler::readDataBase: Purchase database Opened"<<endl;
        QSqlQuery query(*beDB);
        int rowEntry = 0;
        string qry = getTableEntriesQuery(TABLE_PURCHASE);
        if(!query.exec( qry.c_str()))
        {
            fout<<"BE_PurTableHandler::readDataBase:"<<query.lastError().text().toStdString()<<endl;
            qDebug()<<query.lastError()<<endl;
        }
        fout<<"BE_PurTableHandler :: readDataBase"<<getLastQuery(query).toStdString()<<endl;
        if(query.record().isEmpty()){
            fout<<"BE_PurTableHandler::readDataBase:No entries are there"<<endl;
            beDB->close();
            return ERR_NONE;
        }
        else{
            while(query.next()){
                QSqlRecord record = query.record();
                matrow oneEntry;
                for(int item = PUR_ID; item < PUR_END; item ++)
                {
                    int colIndx = record.indexOf(table_fields[TABLE_PURCHASE][item]);
                    oneEntry.push_back(query.value(colIndx).toString().toStdString());
                    //((*records)->at(rowEntry)).at(item) = query.value(colIndx).toString().toStdString();
                }
                (records)->push_back(oneEntry);
                rowEntry++;
            }
        }
        *totItemes = rowEntry;
        if(*totItemes <= 0)
            fout<<"BE_PurTableHandler:: readDataBase: No Data"<<endl;
        fout<<"BE_PurTableHandler::readDataBase:database Has been closed"<<endl;
        beDB->close();
    }
    return ERR_NONE;
}

/* Deletes Purchase table Data */
errorType BE_PurTableHandler :: deleteFromDataBase (unsigned int purId)
{
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
         string qry = getTableDeleteQuery(TABLE_PURCHASE);
         query.prepare(qry.c_str());
         string tableField(table_fields[TABLE_PURCHASE][0]);
         std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
         tableField = ":" + tableField;
         query.bindValue(tableField.c_str(), purId);
         if(!query.exec())
            err = ERR_DB_DEL;
         fout<<"BE_PurTableHandler :: deleteFromDataBase"<<getLastQuery(query).toStdString()<<endl;
         beDB->close();
    }
    return err;
}

errorType BE_PurTableHandler :: addIntoDataBase(unsigned int purId, matrow *str)
{
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
        string qry = getTableInsertQuery(TABLE_PURCHASE, PUR_ID, PUR_END);
        query.prepare(qry.c_str());
        string tableField(table_fields[TABLE_PURCHASE][PUR_ID]);
        std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
        tableField = ":" + tableField;
        query.bindValue(tableField.c_str(), purId);
        for(int iter = PUR_ID + 1; iter < PUR_END; iter++)
        {
            string tableField(table_fields[TABLE_PURCHASE][iter]);
            std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
            tableField = ":" + tableField;
            query.bindValue(tableField.c_str(), QString((str->at(iter)).c_str()));
        }
        /*query.bindValue(":purno", QString((str->at(PUR_NO)).c_str()));
        query.bindValue(":date",QString((str->at(PUR_DATE)).c_str()));
        query.bindValue(":billno",QString((str->at(PUR_BILLNO)).c_str()));
        query.bindValue(":batchno",QString((str->at(PUR_BATCH)).c_str()));
        query.bindValue(":prodname",QString((str->at(PUR_PROD)).c_str()));
        query.bindValue(":supplier",QString((str->at(PUR_SUPP)).c_str()));
        query.bindValue(":box",QString((str->at(PUR_BOX)).c_str()));
        query.bindValue(":pieces",QString((str->at(PUR_ITEMS)).c_str()));
        query.bindValue(":totalcost",QString((str->at(PUR_TOTCOST)).c_str()));
        query.bindValue(":paid",QString((str->at(PUR_PAID)).c_str()));
        query.bindValue(":due",QString((str->at(PUR_DUE)).c_str()));
        query.bindValue(":suppaddr",QString((str->at(PUR_SUPPADDR)).c_str()));
        query.bindValue(":contact",QString((str->at(PUR_CONTACT)).c_str()));
        query.bindValue(":contactno",QString((str->at(PUR_CONTACTNO)).c_str()));
        query.bindValue(":remarks",QString((str->at(PUR_REMARKS)).c_str()));
        query.bindValue(":pcsperbox",QString((str->at(PUR_PCSPERBOX)).c_str()));
        query.bindValue(":costperbox",QString((str->at(PUR_COSTOFBOX)).c_str()));
        query.bindValue(":costperpcs",QString((str->at(PUR_COSTOFPCS)).c_str()));
        query.bindValue(":tax",QString((str->at(PUR_TAX)).c_str()));
        query.bindValue(":expns",QString((str->at(PUR_EXPNS)).c_str()));
        query.bindValue(":cashpaid",QString((str->at(PUR_CASHPAID)).c_str()));
        query.bindValue(":chqpaid",QString((str->at(PUR_CHEQPAID)).c_str()));*/

        if(!query.exec()){
            err = ERR_DB_ADD;
            fout<<"BE_PurTableHandler :: addIntoDataBase:Error:" << query.lastError().text().toStdString()<<endl;
            qDebug() << "drivers: "<< query.lastError();
        }
        fout<<"BE_PurTableHandler :: addIntoDataBase"<<getLastQuery(query).toStdString()<<endl;
        beDB->close();
    }
    return err;
}

errorType BE_PurTableHandler :: editIntoDataBase(unsigned int purId, matrow *input)
{
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
        string qry = getTableUpdateQuery(TABLE_PURCHASE, PUR_ID, PUR_END);
        query.prepare(qry.c_str());

        for(int iter = PUR_NO; iter< PUR_END; iter++){
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

errorType BE_PurTableHandler :: deleteAllTableEntries()
{
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    errorType err = ERR_NONE;
    fout<<"BE_PurTableHandler::deleteAllTableEntries:deleting Purchase Table"<<endl;
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_PurTableHandler::deleteAllTableEntries:Cann't delete from the database"<<endl;
        return ERR_DB_OPEN;
    }
    else{

        QSqlQuery query(*beDB);
        string qry = getTableDeleteAllEntriesQuery(TABLE_PURCHASE);
        if(!query.exec( qry.c_str()))
        {
            fout<<"BE_PurTableHandler::deleteAllTableEntries:"<<query.lastError().text().toStdString()<<endl;
            qDebug()<<query.lastError()<<endl;
            err = ERR_DB_DEL_ALL;
        }

        fout<<"BE_PurTableHandler::deleteAllTableEntries"<<getLastQuery(query).toStdString()<<endl;
        beDB->close();
    }
    return (errorType)err;
}

errorType BE_PurTableHandler :: editFieldIntoDataBase(unsigned int purId, string *field, string *input)
{
    QSqlDatabase *beDB = BE_DatabaseHandler::getDataBaseHandlerInstance()->getDataBaseObj();
    errorType err = ERR_NONE;
    fout<<"BE_PurTableHandler::editFieldIntoDataBase:editing Purchase Table"<<endl;
    bool ok=beDB->open("Priyanko","nemo");
    if(!ok){
        fout<<"BE_DatabaseHandler::editFieldIntoDataBase:Cann't be added into the database"<<endl;
        return ERR_DB_OPEN;
    }
    else{

        QSqlQuery query(*beDB);
        QString qStm = QString();
        qStm = "UPDATE Purchase SET "+ QString(field->c_str()) + "=? WHERE PurchaseId=?";
        query.prepare(qStm);
        query.addBindValue(QString(input->c_str()));
        query.addBindValue(purId);
        if(!query.exec())
            err = ERR_DB_EDIT;
        fout<<"BE_PurTableHandler :: editFieldIntoDataBase"<<getLastQuery(query).toStdString()<<endl;
        beDB->close();
    }
    return (errorType)err;
}
