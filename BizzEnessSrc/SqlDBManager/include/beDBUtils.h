#ifndef BEDBUTILS_H
#define BEDBUTILS_H
#include <QDebug>
#include <QSqlQuery>
#include <QString>

#include "common/include/common.h"

QString getLastQuery(QSqlQuery &query);

const string getTableCreateQuery(tableType, int, int);
const string getTableInsertQuery(tableType, int, int);
const string getTableDeleteQuery(tableType);
const string getTableUpdateQuery(tableType, int, int);
const string getTableEntriesQuery(tableType);
const string getTableDeleteAllEntriesQuery(tableType);

#endif // BEDBUTILS_H
