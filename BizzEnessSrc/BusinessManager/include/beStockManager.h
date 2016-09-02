#ifndef BESTOCKMANAGER_H
#define BESTOCKMANAGER_H

#include <map>
#include <string>
#include <list>
#include <common/include/csvParser.h>
#include <common/include/common.h>
using namespace std;

typedef struct {
    unsigned int product_id;
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
    int pcs_per_box;
}stockData_t;

class BE_StockManager
{
public:
    BE_StockManager();
    ~BE_StockManager();
    errorType updateItem(unsigned int , int , string );
    errorType setItem(unsigned int , int , string );
    errorType addNewItem(unsigned int );
    errorType saveItems();
    errorType deleteItem(unsigned int );
    errorType insertSavedItem(matrow *);
    errorType readBMData(matrix **, int *);
    errorType getOneRecord(stockData_t &, matrow *);
    errorType getRecord(unsigned int ,matrow *);
    errorType readFileData(string , int *);
    errorType insertCSVData(CSVRow &);
    errorType deleteAllItems();
    map <unsigned int, stockData_t> & getStockTable();
    const char * getElement(stockData_t &, int );
    list<string> *getStockProductList();
    int totItems;
 private:
    map <unsigned int, stockData_t> stockTable;
};

#endif // BESTOCKMANAGER_H
