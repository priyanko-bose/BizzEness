#ifndef BEDATABASEHANDLER_H
#define BEDATABASEHANDLER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include "common/include/common.h"
#include <string>
using namespace std;

class BE_DatabaseHandler
{
    public:
        virtual ~BE_DatabaseHandler();
        static BE_DatabaseHandler * getDataBaseHandlerInstance();
        QSqlQuery & execQuery(const QString , QSqlDatabase &);
        QSqlDatabase * getDataBaseObj();
        errorType initDB();
        void deInitDB();

        errorType readDataBase(tableType, int , int , matrix *, int *);
        errorType addIntoDataBase(tableType tbl, int , int ,unsigned int ,matrow *);
        errorType deleteFromDataBase(tableType, unsigned int );
        errorType editIntoDataBase(tableType, int , int ,unsigned int ,matrow *);
        errorType editFieldIntoDataBase(tableType, unsigned int , string *, string *);
        errorType deleteAllTableEntries(tableType);

    protected:
        BE_DatabaseHandler();

    private:
        static BE_DatabaseHandler *beDatabaseHandlerObj;
        QSqlDatabase *beDB;

};
#endif // BEDATABASEHANDLER_H
