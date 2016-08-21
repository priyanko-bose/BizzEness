#ifndef BEPURTABLEHANDLER_H
#define BEPURTABLEHANDLER_H
#include <string>
#include "common/include/common.h"
#include "SqlDBManager/include/beDataBaseHandler.h"


using namespace std;

class BE_PurTableHandler :public BE_DatabaseHandler
{
public:
    BE_PurTableHandler();
    ~BE_PurTableHandler();

    errorType readDataBase(matrix *, int *);
    errorType addIntoDataBase(unsigned int ,matrow *);
    errorType deleteFromDataBase(unsigned int );
    errorType editIntoDataBase(unsigned int ,matrow *);
    errorType editFieldIntoDataBase(unsigned int , string *, string *);
    errorType deleteAllTableEntries();
};



#endif // BEPURTABLEHANDLER_H
