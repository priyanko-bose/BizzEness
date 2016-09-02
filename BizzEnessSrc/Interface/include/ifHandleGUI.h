#ifndef IFHANDLEGUI_H
#define IFHANDLEGUI_H
#include "common/include/common.h"
#include <QStringList>
#include <string>

using namespace std;

errorType initializeGUI(int argc , char *argv[]);
void deinitializeGUI();

errorType populateSavedData();
QStringList *getProductList();

//Window specific functions
errorType populatePurWindowData(unsigned int *, int , string );
errorType importTableData(string);

#endif // IFHANDLEGUI_H
