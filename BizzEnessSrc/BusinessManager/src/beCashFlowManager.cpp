#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "BusinessManager/include/beCashFlowManager.h"
#include "common/include/utils.h"

extern ofstream fout;

using namespace std;

/*
 * BE_CashflowManager Constructor
 */
BE_CashflowManager::BE_CashflowManager()
{
    fout<<"info:beCashflowManager.cpp:Creating BE_CashflowManager object"<<endl;
    totItems = 0;
}

/*
 * BE_CashflowManager Destructor
 */
BE_CashflowManager::~BE_CashflowManager()
{
    fout<<"info:beCashflowManager.cpp:Deleting BE_CashflowManager object"<<endl;
}

/*
 * This function returns the fields of a cashflow table entry
 * identified by field name
 */
const char * BE_CashflowManager::getElement(cashflowData_t &cashflowData, int itemno)
{
    switch(itemno)
    {
    case CASHFLOW_ID:
        return  intToString(cashflowData.cashflow_id).c_str();
    case CASHFLOW_BILLNO:
        return cashflowData.billno.c_str();
    case CASHFLOW_BILLEDAMT:
        return doubleToString(cashflowData.billedAmt).c_str();
    case CASHFLOW_PAIDAMT:
        return doubleToString(cashflowData.paidAmt).c_str();
    case CASHFLOW_DUEAMT:
        return doubleToString(cashflowData.dueAmt).c_str();
    case CASHFLOW_REFUNDAMT:
        return doubleToString(cashflowData.refundAmt).c_str();
    case CASHFLOW_FORFEITEDAMT:
        return doubleToString(cashflowData.forfeitedAmt).c_str();
    case CASHFLOW_EXPNS:
        return  doubleToString(cashflowData.expenses).c_str();
    case CASHFLOW_CASHINBANK:
        return  doubleToString(cashflowData.cashinbank).c_str();
    case CASHFLOW_CHEQUEAMT:
        return doubleToString(cashflowData.chequeAmt).c_str();
    case CASHFLOW_CASHINHAND:
        return  doubleToString(cashflowData.cashinHand).c_str();
    default:
        break;
    };
    return "";
}

/*
 * This function updates the fields of a cashflow table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_CashflowManager::setItem(int key, int itemno, string val)
{
    //get the cashflow table entry
    if(this->cashflowTable.find(key) == this->cashflowTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):beCashflowManager.cpp:updateCashflowManger"<<endl;
        return ERR_WRONG_ID;
    }
    else{
        cashflowData_t & cashflowData = this->cashflowTable.at(key);

        switch(itemno)
        {
        case CASHFLOW_BILLNO:
            cashflowData.billno = val.c_str();
            break;
        case CASHFLOW_BILLEDAMT:
            cashflowData.billedAmt = atof(val.c_str());
            break;
        case CASHFLOW_PAIDAMT:
            cashflowData.paidAmt = atof(val.c_str());
            break;
        case CASHFLOW_DUEAMT:
            cashflowData.dueAmt = atof(val.c_str());
            break;
        case CASHFLOW_REFUNDAMT:
            cashflowData.refundAmt = atof(val.c_str());
            break;
        case CASHFLOW_FORFEITEDAMT:
            cashflowData.forfeitedAmt = atof(val.c_str());;
            break;
        case CASHFLOW_EXPNS:
            cashflowData.expenses = atof(val.c_str());
            break;
        case CASHFLOW_CASHINBANK:
            cashflowData.cashinbank = atof(val.c_str());
            break;
        case CASHFLOW_CHEQUEAMT:
            cashflowData.chequeAmt = atof(val.c_str());
            break;
        case CASHFLOW_CASHINHAND:
            cashflowData.cashinHand = atof(val.c_str());
            break;
        default:
            break;
        };
    }
    return ERR_NONE;
}

/*
 * This function sets the fields of a CashFlow table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_CashflowManager::updateItem(unsigned int key, int itemno, string val, int updateQ)
{
    return setItem(key,itemno, val);
}

/*
 * This function adds a new cashflow table table entry
 * initialized with null values
 */
errorType BE_CashflowManager::addNewItem(int key)
{
    cashflowData_t cashflowData;

    cashflowData.billno = "";

    cashflowData.billedAmt = 0.0;
    cashflowData.paidAmt = 0.0;
    cashflowData.dueAmt = 0.0;
    cashflowData.refundAmt = 0.0;
    cashflowData.forfeitedAmt = 0.0;
    cashflowData.expenses = 0;
    cashflowData.cashinbank = 0;
    cashflowData.chequeAmt = 0;
    cashflowData.cashinHand = 0.0;

    cashflowData.cashflow_id = key;
    //insert into the tock table map
    this->cashflowTable.insert(std::pair< int,cashflowData_t>(key,cashflowData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function saves cashflow table table entries
 * into respective data base file
 */
errorType BE_CashflowManager::saveItems()
{
    ofstream myfile;
    string filename = (string(table_name[TABLE_CASHFLOW]) + ".csv" );
    map <int, cashflowData_t> & mymap = this->cashflowTable;
    myfile.open (filename.c_str());
    if(myfile.is_open())
    {
        myfile << "CashFlowId,BillNo,BilledAmt,PaidAmt,DueAmt,"
           <<"RefundAmt,ForfeitedAmt,Expenses,CashInBank,"
           <<"ChequeAmt,CashInHand"<<endl;
        for (map<int,cashflowData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            myfile << it->first<<","<< it->second.billno<<","<< it->second.billedAmt <<","
               <<it->second.paidAmt<<","<<it->second.dueAmt<<","
               <<it->second.refundAmt<<","<<it->second.forfeitedAmt<<","
               <<it->second.expenses<<","<<it->second.cashinbank<<","
               <<it->second.chequeAmt<<","<<it->second.cashinHand<<endl;
        }
        fout<<"info:BE_CashflowManager::saveCashflowManager:saved data:"<<endl;
    }
    else{
        fout<<"error("<<ERR_DB_OPEN<<"):BE_CashflowManager::saveCashflowManager:Couln't open file to save data:"<<endl;
        return ERR_DB_OPEN;
    }
    myfile.close();
    return ERR_NONE;
}

/*
 * This function returns the cashflow table
 */
map <int, cashflowData_t> & BE_CashflowManager::getCashflowTable()
{
    return cashflowTable;
}

/*
 * This function adds a new cashflow table table entry
 * initialized with null values
 */
errorType BE_CashflowManager::deleteItem(int id)
{
    map <int, cashflowData_t> & mymap = this->cashflowTable;
    map<int,cashflowData_t>::iterator it=mymap.find(id);
    if(it != mymap.end())
        mymap.erase (it);
    else{
        fout << "error("<<ERR_WRONG_ID<<"):beCashflowManager.cpp:deleteItem"<<endl;
        return ERR_WRONG_ID;
    }
    totItems--;
    return ERR_NONE;
}

/*
 * This function adds a new cashflow table table entry
 * initialized with values read from the file
 */
errorType BE_CashflowManager::insertSavedItem(CSVRow &row)
{
    cashflowData_t cashflowData;
    cashflowData.cashflow_id = atoi(row[CASHFLOW_ID].c_str());
    cashflowData.billno = row[CASHFLOW_BILLNO];

    cashflowData.billedAmt = atof(row[CASHFLOW_BILLEDAMT].c_str());
    cashflowData.paidAmt = atof(row[CASHFLOW_PAIDAMT].c_str());
    cashflowData.dueAmt = atof(row[CASHFLOW_DUEAMT].c_str());
    cashflowData.refundAmt = atof(row[CASHFLOW_REFUNDAMT].c_str());
    cashflowData.forfeitedAmt = atof(row[CASHFLOW_FORFEITEDAMT].c_str());
    cashflowData.expenses = atof(row[CASHFLOW_EXPNS].c_str());
    cashflowData.cashinbank = atof(row[CASHFLOW_CASHINBANK].c_str());
    cashflowData.chequeAmt = atof(row[CASHFLOW_CHEQUEAMT].c_str());
    cashflowData.cashinHand = atof(row[CASHFLOW_CASHINHAND].c_str());

    //insert into the cash flow table
    this->cashflowTable.insert(std::pair< int,cashflowData_t>(cashflowData.cashflow_id,cashflowData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function is used to read the respective database file
 * for a table and insert those items in BusinessManager Tables
 */
errorType BE_CashflowManager::readFileData(int *totitems)
{
    string filename(string(table_name[TABLE_CASHFLOW])+".csv");
    std::ifstream  file(filename.c_str());
    if(!file.good()){
        fout << "info:beCashflowManager.cpp:readFileData:"<<string(table_name[TABLE_CASHFLOW])<<"not exists"<<endl;
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
            fout << "info:beCashflowManager.cpp:readFileData:from table"
                 <<string(table_name[TABLE_CASHFLOW])<<" "<<*totitems<<"entries read"<<endl;
            file.close();
        }
        else{
            fout << "error:beCashflowManager.cpp:readFileData:"<<string(table_name[TABLE_CASHFLOW])
                 <<"could not be opened"<<endl;
            return ERR_CASHFLOW_FILE_OPEN;
        }
    }
    return ERR_NONE;
}
