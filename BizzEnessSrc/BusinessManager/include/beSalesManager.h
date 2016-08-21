#ifndef BESALESMANAGER_H
#define BESALESMANAGER_H

#include <map>
#include <string>
#include <common/include/csvParser.h>
#include <common/include/common.h>
using namespace std;

enum sales_table_flds{
    SALES_ID = 0,
    SALES_DATE = 1,
    SALES_BILLNO = 2,
    SALES_PROD = 3,
    SALES_CUST = 4,
    SALES_BOX = 5,
    SALES_PIECE = 6,
    SALES_TOTCOST = 7,
    SALES_AMTPAID = 8,
    SALES_AMTDUE = 9,
    SALES_EXPS = 10,
    SALES_END
};

typedef struct {
    int salesId;
    string date;
    string billNo;
    string productName;
    string customer;
    unsigned int noOfBoxes;
    unsigned int noOfItems;
    double totalCost;
    double amtPaid;
    double amtDue;
    double expenses;
}salesData_t;

class BE_SalesManager
{
public:
    BE_SalesManager();
    ~BE_SalesManager();
    errorType updateItem(int , int , string );
    errorType addNewItem(int );
    errorType saveItems();
    errorType deleteItem(int );
    errorType insertSavedItem(CSVRow &row);
    errorType readFileData(int *);
    map <int, salesData_t> & getSalesTable();
    const char * getElement(salesData_t &salesData, int itemno);
    int totItems;
 private:
    map <int, salesData_t> salesTable;
};

#endif // BESALESMANAGER_H
