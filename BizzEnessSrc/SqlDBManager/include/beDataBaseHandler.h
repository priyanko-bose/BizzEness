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

        virtual errorType readDataBase(matrix *, int *);
        virtual errorType addIntoDataBase(unsigned int ,matrow *);
        virtual errorType deleteFromDataBase(unsigned int );
        virtual errorType editIntoDataBase(unsigned int ,matrow *);
        virtual errorType editFieldIntoDataBase(unsigned int , string *, string *);
        virtual errorType deleteAllTableEntries();

        BE_DatabaseHandler *purTableHandler;
        //BE_DatabaseHandler *stockTableHandler;

    protected:
        BE_DatabaseHandler();

    private:
        static BE_DatabaseHandler *beDatabaseHandlerObj;
        QSqlDatabase *beDB;

};
#endif // BEDATABASEHANDLER_H
