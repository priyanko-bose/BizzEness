#ifndef BECASHFLOWMANAGER_H
#define BECASHFLOWMANAGER_H

#include <map>
#include <string>
#include <common/include/csvParser.h>
#include <common/include/common.h>
using namespace std;

enum cashflow_table_flds{
    CASHFLOW_ID = 0,
    CASHFLOW_BILLNO = 1,
    CASHFLOW_BILLEDAMT = 2,
    CASHFLOW_PAIDAMT = 3,
    CASHFLOW_DUEAMT = 4,
    CASHFLOW_REFUNDAMT = 5,
    CASHFLOW_FORFEITEDAMT = 6,
    CASHFLOW_EXPNS = 7,
    CASHFLOW_CASHINBANK = 8,
    CASHFLOW_CHEQUEAMT = 9,
    CASHFLOW_CASHINHAND = 10,
    CASHFLOW_END
};

typedef struct {
    int cashflow_id;
    string billno;
    double billedAmt;
    double paidAmt;
    double dueAmt;
    double refundAmt;
    double forfeitedAmt;
    double expenses;
    double cashinbank;
    double chequeAmt;
    double cashinHand;
}cashflowData_t;

class BE_CashflowManager
{
public:
    BE_CashflowManager();
    ~BE_CashflowManager();
    errorType setItem(int , int , string );
    errorType updateItem(unsigned int , int , string, int );
    errorType addNewItem(int );
    errorType saveItems();
    errorType deleteItem(int );
    errorType insertSavedItem(CSVRow &row);
    errorType readFileData(int *);
    map <int, cashflowData_t> & getCashflowTable();
    const char * getElement(cashflowData_t &cashflowData, int itemno);
    int totItems;
 private:
    map <int, cashflowData_t> cashflowTable;
};

#endif // BECASHFLOWMANAGER_H
