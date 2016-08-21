#ifndef BUSINESSMANAGER_H
#define BUSINESSMANAGER_H

#include "BusinessManager/include/beStockManager.h"
#include "BusinessManager/include/bePurchaseManager.h"
#include "BusinessManager/include/beSalesManager.h"
#include "BusinessManager/include/bePLManager.h"
#include "BusinessManager/include/beSummaryManager.h"
#include "BusinessManager/include/beCashFlowManager.h"
using namespace std;

class BE_BusinessManager{

public:
    ~BE_BusinessManager();
    static BE_BusinessManager *getInstance();
    BE_StockManager * getStockManager();
    BE_PurchaseManager * getPurchaseManager();
    BE_SalesManager * getSalesManager();
    BE_PLManager * getPLManager();
    BE_CashflowManager * getCashflowManager();
    BE_SumManager * getSumManager();

    bool isStockDataRead;
    bool isPurchaseDataRead;
    bool isSalesDataRead;
    bool isCashFlowDataRead;
    bool isPLDataRead;
    bool isSummaryDataRead;

private:
    BE_BusinessManager();
    static BE_BusinessManager *beBusinessManagerObj;
    BE_StockManager *beStockManager;
    BE_PurchaseManager *bePurchaseManager;
    BE_SalesManager *beSalesManager;
    BE_PLManager * bePLManager;
    BE_CashflowManager* beCashflowManager;
    BE_SumManager *beSumManager;
};

#endif
