#ifndef BEPLMANAGER_H
#define BEPLMANAGER_H

#include <map>
#include <string>
#include <common/include/csvParser.h>
#include <common/include/common.h>

using namespace std;

enum pl_table_flds{
    PL_ID = 0,
    PL_NO = 1,
    PL_DATE = 2,
    PL_NAME = 3,
    PL_COMP = 4,
    PL_COST = 5,
    PL_COST_TAX = 6,
    PL_SALEAMT = 7,
    PL_SALEAMT_TAX = 8,
    PL_EXPS = 9,
    PL_PROFIT = 10,
    PL_LOSS = 11,
    PL_END
};

const char * const pl_table_col_name[12] = {
    "PL_ID",
    "PL_NO",
    "PL_DATE",
    "PL_NAME",
    "PL_COMP",
    "PL_COST",
    "PL_COST_TAX",
    "PL_SALEAMT",
    "PL_SALEAMT_TAX",
    "PL_EXPS",
    "PL_PROFIT",
    "PL_LOSS"
} ;

typedef struct {
    int pl_id;
    string plNo;
    string date;
    string productName;
    string company;
    double cost;
    double costTax;
    double saleAmt;
    double saleAmtTax;
    double expenses;
    double profit;
    double loss;
}plData_t;

class BE_PLManager
{
public:
    BE_PLManager();
    ~BE_PLManager();
    errorType updateItem(int , int , string );
    errorType addNewItem(int );
    errorType saveItems();
    errorType deleteItem(int );
    errorType insertSavedItem(CSVRow &row);
    errorType readFileData(int *);
    map <int, plData_t> & getPLTable();
    const char * getElement(plData_t &plData, int itemno);
    int totItems;
 private:
    map <int, plData_t> plTable;
};

#endif // BEPLMANAGER_H
