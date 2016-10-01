#ifndef IFHANDLEGUI_H
#define IFHANDLEGUI_H
#include "common/include/common.h"
#include <QStringList>
#include <QTableWidget>
#include <string>

using namespace std;

errorType initializeGUI(int argc , char *argv[]);
void deinitializeGUI();

errorType populateSavedData();
QStringList *getProductList();
QStringList *getCompanyList();
QStringList * getFilteredList(int , int, int,QString );
QString * getTableItem(int, int, QString, int);
//Window specific functions
errorType populatePurWindowData(unsigned int *, int , string );
errorType importTableData(string);
errorType getPurchaseProds(QTableWidget *,int);

#endif // IFHANDLEGUI_H
