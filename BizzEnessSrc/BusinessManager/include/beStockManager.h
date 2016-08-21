#ifndef BESTOCKMANAGER_H
#define BESTOCKMANAGER_H

#include <map>
#include <string>
#include <list>
#include <common/include/csvParser.h>
#include <common/include/common.h>
using namespace std;

enum stock_table_flds{
    PROD_ID = 0,
    PROD_DATE = 1,
    PROD_BATCH = 2,
    PROD_NAME = 3,
    PROD_COMP = 4,
    PROD_MFG = 5,
    PROD_EXP = 6,
    PROD_NOB = 7,
    PROD_NOI = 8,
    PROD_NOTI = 9,
    PROD_CPB = 10,
    PROD_CPP = 11,
    PROD_PPB = 12,
    PROD_END
};

typedef struct {
    int product_id;
    string date;
    string batchNo;
    string productName;
    string company;
    string mfgDate;
    string expDate;
    unsigned int noOfBoxes;
    unsigned int noOfItems;
    unsigned int noOfTotItems;
    double cost_per_box;
    double cost_per_pcs;
    unsigned int pcs_per_box;
}stockData_t;

class BE_StockManager
{
public:
    BE_StockManager();
    ~BE_StockManager();
    errorType updateItem(unsigned int , int , string );
    errorType addNewItem(unsigned int );
    errorType saveItems();
    errorType deleteItem(int );
    errorType insertSavedItem(CSVRow &row);
    errorType readFileData(string **, int );
    map <int, stockData_t> & getStockTable();
    const char * getElement(stockData_t &stockData, int itemno);
    list<string> *getStockProductList();
    int totItems;
 private:
    map <int, stockData_t> stockTable;
};

#endif // BESTOCKMANAGER_H
