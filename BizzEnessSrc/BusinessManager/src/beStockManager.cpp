#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "BusinessManager/include/beStockManager.h"
#include "common/include/utils.h"

extern ofstream fout;

using namespace std;

/*
 * BE_StockManager Constructor
 */
BE_StockManager::BE_StockManager()
{
    fout<<"info:beStockManager.cpp:Creating BE_StockManager object"<<endl;
    totItems = 0;
}

/*
 * BE_StockManager Destructor
 */
BE_StockManager::~BE_StockManager()
{
    fout<<"info:beStockManager.cpp:Deleting BE_StockManager object"<<endl;
}

/*
 * This function returns the fields of a stock table entry
 * identified by field name
 */
const char * BE_StockManager::getElement(stockData_t &stockData, int itemno)
{
    switch(itemno)
    {
    case PROD_ID:
        return  intToString(stockData.product_id).c_str();
    case PROD_BATCH:
        return stockData.batchNo.c_str();
    case PROD_DATE:
        return stockData.date.c_str();
    case PROD_NAME:
        return stockData.productName.c_str();
    case PROD_COMP:
        return stockData.company.c_str();
    case PROD_MFG:
        return stockData.mfgDate.c_str();
    case PROD_EXP:
        return stockData.expDate.c_str();
    case PROD_NOB:
        return  intToString(stockData.noOfBoxes).c_str();
    case PROD_NOI:
        return  intToString(stockData.noOfItems).c_str();
    case PROD_NOTI:
        return intToString(stockData.noOfTotItems).c_str();
    case PROD_CPB:
        return  doubleToString(stockData.cost_per_box).c_str();
    case PROD_CPP:
        return  doubleToString(stockData.cost_per_pcs).c_str();
    case PROD_PPB:
        return  intToString(stockData.pcs_per_box).c_str();
    default:
        break;
    };
    return "";
}

/*
 * This function updates the fields of a stock table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_StockManager::updateItem(unsigned int key, int itemno, string val)
{
    //get the stock table entry
    if(this->stockTable.find(key) == this->stockTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):beStockManager.cpp:updateStockManger"<<endl;
        return ERR_WRONG_ID;
    }
    else{
        stockData_t & stockData = this->stockTable.at(key);

        switch(itemno)
        {
        case PROD_BATCH:
            stockData.batchNo = val.c_str();
            break;
        case PROD_DATE:
            stockData.date = val.c_str();
            break;
        case PROD_NAME:
            stockData.productName = val.c_str();
            break;
        case PROD_COMP:
            stockData.company = val.c_str();
            break;
        case PROD_MFG:
            stockData.mfgDate = val.c_str();
            break;
        case PROD_EXP:
            stockData.expDate = val.c_str();
            break;
        case PROD_NOB:
            stockData.noOfBoxes = atoi(val.c_str());;
            break;
        case PROD_NOI:
            stockData.noOfItems = atoi(val.c_str());
            break;
        case PROD_NOTI:
            stockData.noOfTotItems = atoi(val.c_str());
            break;
        case PROD_CPB:
            stockData.cost_per_box = atof(val.c_str());
            break;
        case PROD_CPP:
            stockData.cost_per_pcs = atof(val.c_str());
            break;
        case PROD_PPB:
            stockData.pcs_per_box = atoi(val.c_str());
            break;
        default:
            break;
        };
    }
    return ERR_NONE;
}

/*
 * This function adds a new stock table table entry
 * initialized with null values
 */
errorType BE_StockManager::addNewItem(unsigned int key)
{
    stockData_t stockData;

    stockData.batchNo = "";
    stockData.company = "";
    stockData.date = "";
    stockData.productName = "";
    stockData.expDate = "";
    stockData.mfgDate = "";

    stockData.noOfItems = 0;
    stockData.noOfBoxes = 0;
    stockData.noOfTotItems = 0;
    stockData.cost_per_box = 0.0;
    stockData.cost_per_pcs = 0.0;
    stockData.pcs_per_box = 0;

    stockData.product_id = key;
    //insert into the tock table map
    this->stockTable.insert(std::pair<unsigned int,stockData_t>(key,stockData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function saves stock table table entries
 * into respective data base file
 */
errorType BE_StockManager::saveItems()
{
    ofstream myfile;
    string filename = (string(table_name[TABLE_STOCK]) + ".csv" );
    map <int, stockData_t> & mymap = this->stockTable;
    myfile.open (filename.c_str());
    if(myfile.is_open())
    {
        myfile << "ProductId,Date,BatchNo,ProductName,Company,"
           <<"MfgDate,ExpDate,NoOfBoxes,NoOfItems,"
           <<"NoOfTotItems,CostPerBox,CostPerPcs,PcsPerBox"<<endl;
        for (map<int,stockData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            myfile << it->first<<","<< it->second.date<<","<< it->second.batchNo <<","
               <<it->second.productName<<","<<it->second.company<<","<<it->second.mfgDate<<","
               <<it->second.expDate<<","<<it->second.noOfBoxes<<","<<it->second.noOfItems<<","
               <<it->second.noOfTotItems<<","<<it->second.cost_per_box<<","
               <<it->second.cost_per_pcs<<","<<it->second.pcs_per_box<<endl;
        }
        fout<<"info:BE_StockManager::saveStockManager:saved data:"<<endl;
    }
    else{
        fout<<"error("<<ERR_DB_OPEN<<"):BE_StockManager::saveStockManager:Couln't open file to save data:"<<endl;
        return ERR_DB_OPEN;
    }
    myfile.close();
    return ERR_NONE;
}

/*
 * This function returns the stock table
 */
map <int, stockData_t> & BE_StockManager::getStockTable()
{
    return stockTable;
}

/*
 * This function adds a new stock table table entry
 * initialized with null values
 */
errorType BE_StockManager::deleteItem(int id)
{
    map <int, stockData_t> & mymap = this->stockTable;
    map<int,stockData_t>::iterator it=mymap.find(id);
    if(it != mymap.end())
        mymap.erase (it);
    else{
        fout << "error("<<ERR_WRONG_ID<<"):beStockManager.cpp:deleteItem"<<endl;
        return ERR_WRONG_ID;
    }
    totItems--;
    return ERR_NONE;
}

/*
 * This function adds a new stock table table entry
 * initialized with values read from the file
 */
errorType BE_StockManager::insertSavedItem(CSVRow &row)
{
    stockData_t stockData;
    stockData.product_id = atoi(row[PROD_ID].c_str());
    stockData.batchNo = row[PROD_BATCH];
    stockData.date = row[PROD_DATE];
    stockData.company = row[PROD_COMP];
    stockData.productName = row[PROD_NAME];
    stockData.expDate = row[PROD_EXP];
    stockData.mfgDate = row[PROD_MFG];

    stockData.noOfItems = atoi(row[PROD_NOI].c_str());
    stockData.noOfBoxes = atoi(row[PROD_NOB].c_str());
    stockData.noOfTotItems = atoi(row[PROD_NOTI].c_str());
    stockData.cost_per_box = atof(row[PROD_CPB].c_str());
    stockData.cost_per_pcs = atof(row[PROD_CPP].c_str());
    stockData.pcs_per_box = atoi(row[PROD_PPB].c_str());
    //insert into the stock table
    this->stockTable.insert(std::pair< int,stockData_t>(stockData.product_id,stockData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function is used to read the respective database file
 * for a table and insert those items in BusinessManager Tables
 */
errorType BE_StockManager::readFileData(string **matrix, int totitems)
{
    string filename(string(table_name[TABLE_STOCK])+".csv");
    std::ifstream  file(filename.c_str());
    if(!file.good()){
        fout << "info:beStockManager.cpp:readFileData:"<<string(table_name[TABLE_STOCK])<<"not exists"<<endl;
        return ERR_NONE;
    }
    else{
        file.close();
        file.open(filename.c_str());
        if(file.is_open())
        {
            CSVRow  row;
            file >> row; //read the column header row
            while(file >> row)
            {
                cout << "4th Element(" << row[3] << ")\n";
                this->insertSavedItem(row);
                totitems = totitems + 1;
            }
            fout << "info:beStockManager.cpp:readFileData:from table"
                 <<string(table_name[TABLE_STOCK])<<" "<<totitems<<"entries read"<<endl;
            file.close();
        }
        else{
            fout << "error:beStockManager.cpp:readFileData:"<<string(table_name[TABLE_STOCK])
                 <<"could not be opened"<<endl;
            return ERR_STOCK_FILE_OPEN;
        }
    }
    return ERR_NONE;
}

/*
 * This returns the complete list of product names
 */
list<string> *BE_StockManager::getStockProductList()
{
    list<string> *prodList =  '\0';
    map <int, stockData_t> & mymap = this->stockTable;
    if(!mymap.empty())
    {
        prodList = new list<string>();
        for (map<int,stockData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            string item = it->second.productName + "(" +it->second.batchNo +")";
            prodList->push_back(item);
        }
    }
    return prodList;
}
