#ifndef IFOPERATION_H
#define IFOPERATION_H
#include <string>
#include "common/include/common.h"

using namespace std;

errorType initializeBManager();
void deinitializeBManager();
//Generic fucntions
errorType updateToBusinessManager(tableType , unsigned int , int , string );
errorType addItemToBusinessManager(tableType, unsigned int );
errorType saveRecordsInCSV(tableType);
errorType delItemFromBusinessManager(tableType, unsigned int);
errorType prepareRecord(tableType, unsigned int, matrow **);

#endif // IFOPERATION_H
