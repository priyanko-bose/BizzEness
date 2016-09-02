#ifndef BESUMMANAGER_H
#define BESUMMANAGER_H

#include <map>
#include <string>
#include <common/include/csvParser.h>
#include <common/include/common.h>
using namespace std;

enum sum_table_flds{
    SUM_ID = 0,
    SUM_BATCH = 1,
    SUM_NAME = 2,
    SUM_EXP = 3,
    SUM_NOB = 4,
    SUM_NOI = 5,
    SUM_PAMT = 6,
    SUM_SAMT = 7,
    SUM_PRAMT = 8,
    SUM_DUEAMT = 9,
    SUM_END
};

typedef struct {
    int sum_id;
    string batchNo;
    string productName;
    string expDate;
    unsigned int noOfBoxes;
    unsigned int noOfItems;
    double purchaseAmt;
    double salesAmt;
    double profitAmt;
    double dueAmt;
}sumData_t;

class BE_SumManager
{
public:
    BE_SumManager();
    ~BE_SumManager();
    errorType setItem(int , int , string );
    errorType updateItem(unsigned int , int , string );
    errorType addNewItem(int );
    errorType saveItems();
    errorType deleteItem(int );
    errorType insertSavedItem(CSVRow &row);
    errorType readFileData(int *);
    map <int, sumData_t> & getSumTable();
    const char * getElement(sumData_t &sumData, int itemno);
    int totItems;
 private:
    map <int, sumData_t> sumTable;
};

#endif // BESUMMANAGER_H
