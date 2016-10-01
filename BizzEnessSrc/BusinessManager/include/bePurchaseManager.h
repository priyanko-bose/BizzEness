#ifndef BEPURCHASEMANAGER_H
#define BEPURCHASEMANAGER_H

#include <map>
#include <string>
#include <common/include/csvParser.h>
#include <common/include/common.h>

using namespace std;

typedef struct {
    unsigned int purchase_id;
    string purno;
    string date;
    string billno;
    string batchNo;
    string productName;
    string supplier;
    unsigned int noOfBoxes;
    unsigned int noOfItems;
    double totalCost;
    double paidAmnt;
    double dueAmnt;
    string suppaddr;
    string contact;
    string contactno;
    string remarks;
    int pcsperbox;
    double costofbox;
    double costofpcs;
    double tax;
    double expns;
    double cashpaid;
    double chqpaid;
    double grandtotal;
}purchaseData_t;

class BE_PurchaseManager
{
public:
    BE_PurchaseManager();
    ~BE_PurchaseManager();
    errorType setItem(unsigned int , int , string );
    errorType updateItem(unsigned int , int , string, int );
    errorType addNewItem(unsigned int );
    errorType saveItems();
    errorType deleteItem(unsigned int );
    errorType insertSavedItem(matrow *);
    errorType readBMData(matrix **, int *);
    errorType getOneRecord(purchaseData_t &, matrow *);
    errorType getRecord(unsigned int ,matrow *);
    errorType readFileData(string , int *);
    errorType insertCSVData(CSVRow &);
    errorType deleteAllItems();
    map <unsigned int, purchaseData_t> & getPurchaseTable();
    const char * getElement(purchaseData_t &, int );
    errorType getItemText(int , string , int, string *);
    errorType getProductList(matrix *, unsigned int, int *);
    int totItems;
 private:
    map <unsigned int, purchaseData_t> purchaseTable;
};

#endif // BEPURCHASEMANAGER_H
