#include <fstream>
#include"BusinessManager/include/beBusinessManager.h"

extern ofstream fout;
using namespace std;

BE_BusinessManager * BE_BusinessManager::beBusinessManagerObj=NULL; //static variable initialization

/*
 * BE_BusinessManager constructor
 */
BE_BusinessManager :: BE_BusinessManager()
{
    fout<<"info:beBusinessManager.cpp:Creating BE_BusinessManager object"<<endl;
    beStockManager = new BE_StockManager();
    if(beStockManager)
        this->isStockDataRead = false;

    bePurchaseManager = new BE_PurchaseManager();
    if(bePurchaseManager)
        this->isPurchaseDataRead = false;

    beSalesManager = new BE_SalesManager();
    if(beSalesManager)
        this->isSalesDataRead = false;

    bePLManager = new BE_PLManager();
    if(bePLManager)
        this->isPLDataRead = false;

    beCashflowManager = new BE_CashflowManager();
    if(beCashflowManager)
        this->isCashFlowDataRead = false;

    beSumManager = new BE_SumManager();
    if(beSumManager)
        this->isSummaryDataRead = false;
}

/*
 * BE_BusinessManager destructor
 */
BE_BusinessManager :: ~BE_BusinessManager()
{
    fout<<"info:beBusinessManager.cpp:Deleting BE_BusinessManager object"<<endl;
    if(beStockManager)
    {
        delete beStockManager;
        beStockManager = '\0';
    }
    if(bePurchaseManager)
    {
        delete bePurchaseManager;
        bePurchaseManager = '\0';
    }
    if(beSalesManager)
    {
        delete beSalesManager;
        beSalesManager = '\0';
    }
    if(bePLManager)
    {
        delete bePLManager;
        bePLManager = '\0';
    }
    if(beCashflowManager)
    {
        delete beCashflowManager;
        beCashflowManager = '\0';
    }
    if(beSumManager)
    {
        delete beSumManager;
        beSumManager = '\0';
    }
}

/*
 * static fucntion to return the singleton BE_BusinessManager object
 */
BE_BusinessManager * BE_BusinessManager::getInstance()
{
    if(beBusinessManagerObj == NULL){
        fout << "info:beBusinessManager.cpp:Creating Business Manager" <<endl;
        beBusinessManagerObj = new BE_BusinessManager();
    }
    return beBusinessManagerObj;

}

/*
 * Returns the BE_StockManager object
 */
BE_StockManager * BE_BusinessManager::getStockManager()
{
    return beStockManager;
}

/*
 * Returns the BE_PurchaseManager object
 */
BE_PurchaseManager * BE_BusinessManager::getPurchaseManager()
{
    return bePurchaseManager;
}
/*
 * Returns the BE_SalesManager object
 */
BE_SalesManager * BE_BusinessManager::getSalesManager()
{
    return beSalesManager;
}

/*
 * Returns the BE_PLManager object
 */
BE_PLManager * BE_BusinessManager::getPLManager()
{
    return bePLManager;
}

/*
 * Returns the BE_CashflowManager object
 */
BE_CashflowManager * BE_BusinessManager::getCashflowManager()
{
    return beCashflowManager;
}
/*
 * Returns the BE_SumManager object
 */
BE_SumManager * BE_BusinessManager::getSumManager()
{
    return beSumManager;
}
