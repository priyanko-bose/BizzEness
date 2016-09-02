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
        return  uintToString(stockData.product_id).c_str();
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
 * This function sets the fields of a stock table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_StockManager::setItem(unsigned int key, int itemno, string val)
{
    //get the stock table entry
    if(this->stockTable.find(key) == this->stockTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):beStockManager.cpp:setItem"<<endl;
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
            stockData.noOfBoxes = atoi(val.c_str());
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
            stockData.noOfBoxes = stockData.noOfBoxes + atoi(val.c_str());
            break;
        case PROD_NOI:
            stockData.noOfItems = stockData.noOfItems + atoi(val.c_str());
            stockData.noOfTotItems = stockData.noOfTotItems + atoi(val.c_str());
            break;
        case PROD_NOTI:
            stockData.noOfTotItems = stockData.noOfTotItems + atoi(val.c_str());
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
    if(this->stockTable.find(key) == this->stockTable.end()){
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
    }
    else{
        fout << "Key Already Exists:beStockManager.cpp:addNewItem"<<endl;
        return ERR_DUP_ID;
    }
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
    map <unsigned int, stockData_t> & mymap = this->stockTable;
    myfile.open (filename.c_str());
    if(myfile.is_open())
    {
        for(int iter =  PROD_ID; iter < PROD_END; iter++)
            myfile << table_fields[TABLE_STOCK][iter] << ",";
        myfile<<endl;
        for (map<unsigned int,stockData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
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
map <unsigned int, stockData_t> & BE_StockManager::getStockTable()
{
    return stockTable;
}

/*
 * This function adds a new stock table table entry
 * initialized with null values
 */
errorType BE_StockManager::deleteItem(unsigned int id)
{
    map <unsigned int, stockData_t> & mymap = this->stockTable;
    map<unsigned int,stockData_t>::iterator it=mymap.find(id);
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
errorType BE_StockManager::insertSavedItem(matrow *row)
{
    stockData_t stockData;
    stockData.product_id = atol(row->at(PROD_ID).c_str());
    stockData.batchNo = row->at(PROD_BATCH);
    stockData.date = row->at(PROD_DATE);
    stockData.company = row->at(PROD_COMP);
    stockData.productName = row->at(PROD_NAME);
    stockData.expDate = row->at(PROD_EXP);
    stockData.mfgDate = row->at(PROD_MFG);

    stockData.noOfItems = atoi(row->at(PROD_NOI).c_str());
    stockData.noOfBoxes = atoi(row->at(PROD_NOB).c_str());
    stockData.noOfTotItems = atoi(row->at(PROD_NOTI).c_str());
    stockData.cost_per_box = atof(row->at(PROD_CPB).c_str());
    stockData.cost_per_pcs = atof(row->at(PROD_CPP).c_str());
    stockData.pcs_per_box = atoi(row->at(PROD_PPB).c_str());
    //insert into the stock table
    this->stockTable.insert(std::pair< int,stockData_t>(stockData.product_id,stockData));
    row->clear();
    return ERR_NONE;
}

/*
 * This function adds a new stock table table entry
 * initialized with values read from the file
 */
errorType BE_StockManager::insertCSVData(CSVRow &row)
{
    stockData_t stockData;
    stockData.product_id = atoi(row[PROD_ID].c_str());
    stockData.date = row[PROD_DATE];
    stockData.batchNo = row[PROD_BATCH];
    stockData.productName = row[PROD_NAME];
    stockData.company = row[PROD_COMP];
    stockData.mfgDate = row[PROD_MFG];
    stockData.expDate = row[PROD_EXP];
    stockData.noOfBoxes = atoi(row[PROD_NOB].c_str());
    stockData.noOfItems = atoi(row[PROD_NOI].c_str());
    stockData.noOfTotItems = atoi(row[PROD_NOTI].c_str());

    stockData.pcs_per_box = atoi(row[PROD_PPB].c_str());
    stockData.cost_per_box = atof(row[PROD_CPB].c_str());
    stockData.cost_per_pcs = atof(row[PROD_CPP].c_str());

    //insert into the stock table
    this->stockTable.insert(std::pair<unsigned int,stockData_t>(stockData.product_id,stockData));
    return ERR_NONE;
}

/* Get the record string with stock data*/
errorType BE_StockManager:: getOneRecord(stockData_t &stockData, matrow *row)
{
    row->insert(row->begin() + PROD_ID, uintToString(stockData.product_id).c_str());
    row->insert(row->begin() + PROD_DATE , stockData.date.c_str());
    row->insert(row->begin() + PROD_BATCH , stockData.batchNo.c_str());
    row->insert(row->begin() + PROD_NAME , stockData.productName.c_str());
    row->insert(row->begin() + PROD_COMP , stockData.company.c_str());
    row->insert(row->begin() + PROD_MFG , stockData.mfgDate.c_str());
    row->insert(row->begin() + PROD_EXP , stockData.expDate.c_str());
    row->insert(row->begin() + PROD_NOB , uintToString(stockData.noOfBoxes).c_str());
    row->insert(row->begin() + PROD_NOI , uintToString(stockData.noOfItems).c_str());
    row->insert(row->begin() + PROD_NOTI , uintToString(stockData.noOfTotItems).c_str());

    row->insert(row->begin() + PROD_CPB , doubleToString(stockData.cost_per_box).c_str());
    row->insert(row->begin() + PROD_CPP , doubleToString(stockData.cost_per_pcs).c_str());
    row->insert(row->begin() + PROD_PPB , intToString(stockData.pcs_per_box).c_str());

    return ERR_NONE;
}

errorType BE_StockManager :: getRecord(unsigned int id,matrow *record)
{
    if(this->stockTable.find(id) == this->stockTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):beStockManager.cpp:setItem"<<endl;
        return ERR_WRONG_ID;
    }
    map <unsigned int, stockData_t> & mymap = this->stockTable;
    getOneRecord(mymap.at(id),record);
    return ERR_NONE;
}

/*
 * This function is used to read the respective BusinessManager Tables
 * for a table and insert those items in a row of strings
 */
errorType BE_StockManager::readBMData(matrix **records, int *totitems)
{
    int err = 0;
    map <unsigned int, stockData_t> & mymap = this->stockTable;
    map<unsigned int,stockData_t>::iterator it;
    *totitems = mymap.size();
    *records = new matrix(*totitems, std::vector<string>());
    int row=0;
    for(it=mymap.begin(); it!=mymap.end(); ++it){
        getOneRecord(it->second, &((*records)->at(row++)));
    }
    fout << "info:BE_StockManager.cpp:readBMData:from stock manager map"
                 <<string(table_name[TABLE_STOCK])<<" "<<*totitems<<"entries read"<<endl;
    return errorType(err);
}
/*
 * This function is used to read the respective database file
 * for a table and insert those items in BusinessManager Tables
 */
errorType BE_StockManager::readFileData(string path, int *totitems)
{
    string filename(string(table_name[TABLE_STOCK])+".csv");
    filename = path + filename;
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
                cout << "Item ID (" << row[0] << ")\n";
                this->insertCSVData(row);
                *totitems = *totitems + 1;
            }
            fout << "info:beStockManager.cpp:readFileData:from table"
                 <<string(table_name[TABLE_STOCK])<<" "<<*totitems<<"entries read"<<endl;
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

/* Delete all Stock Manager Table Data */
errorType BE_StockManager:: deleteAllItems()
{
    stockTable.clear();
    return ERR_NONE;
}
/*
 * This returns the complete list of product names
 */
list<string> *BE_StockManager::getStockProductList()
{
    list<string> *prodList =  '\0';
    map <unsigned int, stockData_t> & mymap = this->stockTable;
    if(!mymap.empty())
    {
        prodList = new list<string>();
        for (map<unsigned int,stockData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            string item = it->second.productName;
            if(!item.empty()){
                item = item + "(" +it->second.batchNo +")";
                prodList->push_back(item);
            }
        }
    }
    return prodList;
}
