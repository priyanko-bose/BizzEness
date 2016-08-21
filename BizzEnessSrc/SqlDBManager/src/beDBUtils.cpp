#include "SqlDBManager/include/beDBUtils.h"

using namespace std;

static const char * tableCreateQuery[4] = {"CREATE TABLE IF NOT EXISTS ","("," INT UNSIGNED NOT NULL PRIMARY KEY, ",")"};

static const string tableInsertQuery[4] = {"INSERT INTO ","(", ") VALUES (",")"};

static const string tableDeleteQuery[3] = {"DELETE FROM "," WHERE ","=:"};

static const string tableUpdateQuery[3] = {"UPDATE "," SET "," WHERE "};

static const char * tableEntriesQuery = "SELECT * FROM ";

static const char * tableDeleteAllEntries = "DELETE FROM ";

/* Returns  the table create Query */
const string getTableCreateQuery(tableType table, int start, int end){
    string query(tableCreateQuery[0]);
    query = query + table_name[table] + tableCreateQuery[1]
                    + table_fields[table][start] + tableCreateQuery[2];
    for(int iter = start + 1 ; iter < end ; iter ++){
        string tableField(table_fields[table][iter]);
        query = query + tableField + " VARCHAR("+ table_fields_desc[table][iter]+")";
        if(iter != (end - 1))
            query = query + ", ";
    }
    query = query + tableCreateQuery[3];
    return query.c_str();
}


/* Returns  the table row Insert Query */
const string getTableInsertQuery(tableType table, int start, int end){
    string query(tableInsertQuery[0]);
    query = query + table_name[table] + tableInsertQuery[1];
    for(int iter = start; iter < end ; iter ++){
        string tableField(table_fields[table][iter]);
        query = query + tableField ;
        if(iter != (end - 1))
            query = query + ",";
    }
    query = query + tableInsertQuery[2];
    for(int iter = start; iter < end ; iter ++){
        string tableField(table_fields[table][iter]);
        std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
        query = query + ":"+tableField;
        if(iter != (end - 1))
            query = query + ",";
    }
    query = query + tableInsertQuery[3];
    return query.c_str();
}

/* Returns  the table row update Query */
const string getTableUpdateQuery(tableType table, int start, int end){
    string query(tableUpdateQuery[0]);
    query = query + table_name[table] + tableUpdateQuery[1];
    for(int iter = start + 1; iter < end ; iter ++){
        string tableField(table_fields[table][iter]);
        query = query + tableField + "=?";
        if(iter != (end - 1))
            query = query + ",";
    }
    query = query + tableUpdateQuery[2];
    string tableField(table_fields[table][start]);
    query = query + tableField + "=?";
    return query.c_str();
}

/* Returns  the table row delete Query */
const string getTableDeleteQuery(tableType table){
    string query(tableDeleteQuery[0]);
    string tableField(table_fields[table][PUR_ID]);
    query = query + table_name[table] + tableDeleteQuery[1] + tableField + tableDeleteQuery[2];
    std::transform(tableField.begin(), tableField.end(), tableField.begin(), ::tolower);
    query = query + tableField;
    return query.c_str();
}

/* Returns  the table rows */
const string getTableEntriesQuery(tableType table){
    string query(tableEntriesQuery);
    query = query + table_name[table];
    return query;
}

/* Delete all the table rows */
const string getTableDeleteAllEntriesQuery(tableType table){
    string query(tableDeleteAllEntries);
    query = query + table_name[table];
    return query;
}
/*-----------------Common functions for this class and its child classes--------*/
QString getLastQuery(QSqlQuery &query)
{
    QString strQ = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    while (it.hasNext())
    {
        it.next();
        strQ.replace(it.key(),it.value().toString());
    }
    qDebug() << strQ;
    return strQ;
}
