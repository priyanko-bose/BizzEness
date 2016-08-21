#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "BusinessManager/include/beSummaryManager.h"
#include "common/include/utils.h"

extern ofstream fout;

using namespace std;

/*
 * BE_SumManager Constructor
 */
BE_SumManager::BE_SumManager()
{
    fout<<"info:beSumManager.cpp:Creating BE_SumManager object"<<endl;
    totItems = 0;
}

/*
 * BE_SumManager Destructor
 */
BE_SumManager::~BE_SumManager()
{
    fout<<"info:beSumManager.cpp:Deleting BE_SumManager object"<<endl;
}

/*
 * This function returns the fields of a sum table entry
 * identified by field name
 */
const char * BE_SumManager::getElement(sumData_t &sumData, int itemno)
{
    switch(itemno)
    {
    case SUM_ID:
        return  intToString(sumData.sum_id).c_str();
    case SUM_BATCH:
        return sumData.batchNo.c_str();
    case SUM_NAME:
        return sumData.productName.c_str();
    case SUM_EXP:
        return sumData.expDate.c_str();
    case SUM_NOB:
        return  intToString(sumData.noOfBoxes).c_str();
    case SUM_NOI:
        return  intToString(sumData.noOfItems).c_str();
    case SUM_PAMT:
        return doubleToString(sumData.purchaseAmt).c_str();
    case SUM_SAMT:
        return  doubleToString(sumData.salesAmt).c_str();
    case SUM_PRAMT:
        return  doubleToString(sumData.profitAmt).c_str();
    case SUM_DUEAMT:
        return  doubleToString(sumData.dueAmt).c_str();
    default:
        break;
    };
    return "";
}

/*
 * This function updates the fields of a sum table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_SumManager::updateItem(int key, int itemno, string val)
{
    //get the sum table entry
    if(this->sumTable.find(key) == this->sumTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):beSumManager.cpp:updateSumManger"<<endl;
        return ERR_WRONG_ID;
    }
    else{
        sumData_t & sumData = this->sumTable.at(key);

        switch(itemno)
        {
        case SUM_BATCH:
            sumData.batchNo = val.c_str();
            break;
        case SUM_NAME:
            sumData.productName = val.c_str();
            break;
        case SUM_EXP:
            sumData.expDate = val.c_str();
            break;
        case SUM_NOB:
            sumData.noOfBoxes = atoi(val.c_str());;
            break;
        case SUM_NOI:
            sumData.noOfItems = atoi(val.c_str());
            break;
        case SUM_PAMT:
            sumData.purchaseAmt = atof(val.c_str());
            break;
        case SUM_SAMT:
            sumData.salesAmt = atof(val.c_str());
            break;
        case SUM_PRAMT:
            sumData.profitAmt = atof(val.c_str());
            break;
        case SUM_DUEAMT:
            sumData.dueAmt = atof(val.c_str());
            break;
        default:
            break;
        };
    }
    return ERR_NONE;
}

/*
 * This function adds a new sum table table entry
 * initialized with null values
 */
errorType BE_SumManager::addNewItem(int key)
{
    sumData_t sumData;

    sumData.batchNo = "";
    sumData.productName = "";
    sumData.expDate = "";


    sumData.noOfItems = 0;
    sumData.noOfBoxes = 0;
    sumData.purchaseAmt = 0.0;
    sumData.salesAmt = 0.0;
    sumData.profitAmt = 0.0;
    sumData.dueAmt = 0.0;

    sumData.sum_id = key;
    //insert into the tock table map
    this->sumTable.insert(std::pair< int,sumData_t>(key,sumData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function saves sum table table entries
 * into respective data base file
 */
errorType BE_SumManager::saveItems()
{
    ofstream myfile;
    string filename = (string(table_name[TABLE_SUMMARY]) + ".csv" );
    map <int, sumData_t> & mymap = this->sumTable;
    myfile.open (filename.c_str());
    if(myfile.is_open())
    {
        myfile << "SummaryId,BatchNo,ProductName,"
           <<"ExpDate,NoOfBoxes,NoOfItems,"
           <<"PurchaseAmount,SalesAmount,ProfitAmount,DueAmount"<<endl;
        for (map<int,sumData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            myfile << it->first<<","<< it->second.batchNo <<","
               <<it->second.productName<<","<<it->second.expDate<<","
               <<it->second.noOfBoxes<<","<<it->second.noOfItems<<","
               <<it->second.purchaseAmt<<","<<it->second.salesAmt<<","
               <<it->second.profitAmt<<","<<it->second.dueAmt<<endl;
        }
        fout<<"info:BE_SumManager::saveSumManager:saved data:"<<endl;
    }
    else{
        fout<<"error("<<ERR_DB_OPEN<<"):BE_SumManager::saveSumManager:Couln't open file to save data:"<<endl;
        return ERR_DB_OPEN;
    }
    myfile.close();
    return ERR_NONE;
}

/*
 * This function returns the sum table
 */
map <int, sumData_t> & BE_SumManager::getSumTable()
{
    return sumTable;
}

/*
 * This function adds a new sum table table entry
 * initialized with null values
 */
errorType BE_SumManager::deleteItem(int id)
{
    map <int, sumData_t> & mymap = this->sumTable;
    map<int,sumData_t>::iterator it=mymap.find(id);
    if(it != mymap.end())
        mymap.erase (it);
    else{
        fout << "error("<<ERR_WRONG_ID<<"):beSumManager.cpp:deleteItem"<<endl;
        return ERR_WRONG_ID;
    }
    totItems--;
    return ERR_NONE;
}

/*
 * This function adds a new sum table table entry
 * initialized with values read from the file
 */
errorType BE_SumManager::insertSavedItem(CSVRow &row)
{
    sumData_t sumData;
    sumData.sum_id = atoi(row[SUM_ID].c_str());
    sumData.batchNo = row[SUM_BATCH];
    sumData.productName = row[SUM_NAME];
    sumData.expDate = row[SUM_EXP];


    sumData.noOfItems = atoi(row[SUM_NOI].c_str());
    sumData.noOfBoxes = atoi(row[SUM_NOB].c_str());
    sumData.purchaseAmt = atof(row[SUM_PAMT].c_str());
    sumData.salesAmt = atof(row[SUM_SAMT].c_str());
    sumData.profitAmt = atof(row[SUM_PRAMT].c_str());
    sumData.dueAmt = atof(row[SUM_DUEAMT].c_str());
    //insert into the summary table
    this->sumTable.insert(std::pair< int,sumData_t>(sumData.sum_id,sumData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function is used to read the respective database file
 * for a table and insert those items in BusinessManager Tables
 */
errorType BE_SumManager::readFileData(int *totitems)
{
    string filename(string(table_name[TABLE_SUMMARY])+".csv");
    std::ifstream  file(filename.c_str());
    if(!file.good()){
        fout << "info:beSumManager.cpp:readFileData:"<<string(table_name[TABLE_SUMMARY])<<"not exists"<<endl;
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
                *totitems = *totitems + 1;
            }
            fout << "info:beSumManager.cpp:readFileData:from table"
                 <<string(table_name[TABLE_SUMMARY])<<" "<<*totitems<<"entries read"<<endl;
            file.close();
        }
        else{
            fout << "error:beSumManager.cpp:readFileData:"<<string(table_name[TABLE_SUMMARY])
                 <<"could not be opened"<<endl;
            return ERR_SUMMARY_FILE_OPEN;
        }
    }
    return ERR_NONE;
}
