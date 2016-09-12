#ifndef IFHANDLEDB_H
#define IFHANDLEDB_H
#include "common/include/common.h"
#include <string>
using namespace std;

errorType initializeDBManager();
errorType deinitializeDBManager();
errorType readRecords();
errorType saveRecordsInDB();
errorType saveTableRecords(tableType );
errorType updateRecordInDB(tableType , unsigned int , matrow *);
errorType saveRecordInDB(tableType, unsigned int, matrow *);
errorType delFromDataBase(tableType , unsigned int );

#endif // IFHANDLEDB_H
