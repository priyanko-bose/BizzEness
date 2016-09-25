#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "BusinessManager/include/beSalesManager.h"
#include "common/include/utils.h"

extern ofstream fout;

using namespace std;

/*
 * BE_SalesManager Constructor
 */
BE_SalesManager::BE_SalesManager()
{
    fout<<"info:beSalesManager.cpp:Creating BE_SalesManager object"<<endl;
    totItems = 0;
}

/*
 * BE_SalesManager Destructor
 */
BE_SalesManager::~BE_SalesManager()
{
    fout<<"info:beSalesManager.cpp:Deleting BE_SalesManager object"<<endl;
}

/*
 * This function returns the fields of a sales table entry
 * identified by field name
 */
const char * BE_SalesManager::getElement(salesData_t &salesData, int itemno)
{
    switch(itemno)
    {
    case SALES_ID:
        return  intToString(salesData.salesId).c_str();
    case SALES_DATE:
        return salesData.date.c_str();
    case SALES_BILLNO:
        return salesData.billNo.c_str();
    case SALES_PROD:
        return salesData.productName.c_str();
    case SALES_CUST:
        return salesData.customer.c_str();
    case SALES_BOX:
        return intToString(salesData.noOfBoxes).c_str();
    case SALES_PIECE:
        return intToString(salesData.noOfItems).c_str();
    case SALES_TOTCOST:
        return  doubleToString(salesData.totalCost).c_str();
    case SALES_AMTPAID:
        return  doubleToString(salesData.amtPaid).c_str();
    case SALES_AMTDUE:
        return doubleToString(salesData.amtDue).c_str();
    case SALES_EXPS:
        return  doubleToString(salesData.expenses).c_str();
    default:
        break;
    };
    return "";
}

/*
 * This function sets the fields of a sales table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_SalesManager::setItem(int key, int itemno, string val)
{
    //get the sales table entry
    if(this->salesTable.find(key) == this->salesTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):beSalesManager.cpp:setItem"<<endl;
        return ERR_WRONG_ID;
    }
    else{
        salesData_t & salesData = this->salesTable.at(key);

        switch(itemno)
        {
        case SALES_DATE:
            salesData.date = val.c_str();
            break;
        case SALES_BILLNO:
            salesData.billNo = val.c_str();
            break;
        case SALES_PROD:
            salesData.productName = val.c_str();
            break;
        case SALES_CUST:
            salesData.customer = val.c_str();
            break;
        case SALES_BOX:
            salesData.noOfBoxes = atoi(val.c_str());
            break;
        case SALES_PIECE:
            salesData.noOfItems = atoi(val.c_str());
            break;
        case SALES_TOTCOST:
            salesData.totalCost = atof(val.c_str());
            break;
        case SALES_AMTPAID:
            salesData.amtPaid = atof(val.c_str());
            break;
        case SALES_AMTDUE:
            salesData.amtDue = atof(val.c_str());
            break;
        case SALES_EXPS:
            salesData.expenses = atof(val.c_str());
            break;
        default:
            break;
        };
    }
    return ERR_NONE;
}

/*
 * This function sets the fields of a Sales table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_SalesManager::updateItem(unsigned int key, int itemno, string val, int updateQ)
{
    (void)updateQ;
    return setItem(key,itemno, val);
}

/*
 * This function adds a new sales table table entry
 * initialized with null values
 */
errorType BE_SalesManager::addNewItem(int key)
{
    salesData_t salesData;

    salesData.billNo = "";
    salesData.customer = "";
    salesData.date = "";
    salesData.productName = "";

    salesData.noOfItems = 0;
    salesData.noOfBoxes = 0;
    salesData.totalCost = 0.0;
    salesData.amtPaid = 0.0;
    salesData.amtDue = 0.0;
    salesData.expenses = 0.0;

    salesData.salesId = key;
    //insert into the tock table map
    this->salesTable.insert(std::pair< int,salesData_t>(key,salesData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function saves sales table table entries
 * into respective data base file
 */
errorType BE_SalesManager::saveItems()
{
    ofstream myfile;
    string filename = (string(table_name[TABLE_SALES]) + ".csv" );
    map <int, salesData_t> & mymap = this->salesTable;
    myfile.open (filename.c_str());
    if(myfile.is_open())
    {
        myfile << "SalesId,Date,BillNo,ProductName,Customer,"
           <<"NoOfBoxes,NoOfItems,"
           <<"TotalCost,AmountPaid,AmountDue,Expenses"<<endl;
        for (map<int,salesData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            myfile << it->first<<","<<it->second.date<<","<<it->second.billNo<<","
               <<it->second.productName<<","<<it->second.customer<<","
               <<it->second.noOfBoxes<<","<<it->second.noOfItems<<","
               <<it->second.totalCost<<","<<it->second.amtPaid<<","
               <<it->second.amtDue<<","<<it->second.expenses<<endl;
        }
        fout<<"info:BE_SalesManager::saveSalesManager:saved data:"<<endl;
    }
    else{
        fout<<"error("<<ERR_DB_OPEN<<"):BE_SalesManager::saveSalesManager:Couln't open file to save data:"<<endl;
        return ERR_DB_OPEN;
    }
    myfile.close();
    return ERR_NONE;
}

/*
 * This function returns the sales table
 */
map <int, salesData_t> & BE_SalesManager::getSalesTable()
{
    return salesTable;
}

/*
 * This function adds a new sales table table entry
 * initialized with null values
 */
errorType BE_SalesManager::deleteItem(int id)
{
    map <int, salesData_t> & mymap = this->salesTable;
    map<int,salesData_t>::iterator it=mymap.find(id);
    if(it != mymap.end())
        mymap.erase (it);
    else{
        fout << "error("<<ERR_WRONG_ID<<"):beSalesManager.cpp:deleteItem"<<endl;
        return ERR_WRONG_ID;
    }
    totItems--;
    return ERR_NONE;
}

/*
 * This function adds a new sales table table entry
 * initialized with values read from the file
 */
errorType BE_SalesManager::insertSavedItem(CSVRow &row)
{
    salesData_t salesData;
    salesData.salesId = atoi(row[SALES_ID].c_str());
    salesData.billNo = row[SALES_BILLNO];
    salesData.date = row[SALES_DATE];
    salesData.customer = row[SALES_CUST];
    salesData.productName = row[SALES_PROD];

    salesData.noOfItems = atoi(row[SALES_PIECE].c_str());
    salesData.noOfBoxes = atoi(row[SALES_BOX].c_str());
    salesData.totalCost = atof(row[SALES_TOTCOST].c_str());
    salesData.amtPaid = atof(row[SALES_AMTPAID].c_str());
    salesData.amtDue = atof(row[SALES_AMTDUE].c_str());
    salesData.expenses = atof(row[SALES_EXPS].c_str());
    //insert into the sales table
    this->salesTable.insert(std::pair< int,salesData_t>(salesData.salesId,salesData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function is used to read the respective database file
 * for a table and insert those items in BusinessManager Tables
 */
errorType BE_SalesManager::readFileData(int *totitems)
{
    string filename(string(table_name[TABLE_SALES])+".csv");
    std::ifstream  file(filename.c_str());
    if(!file.good()){
        fout << "info:beSalesManager.cpp:readFileData:"<<string(table_name[TABLE_SALES])<<"not exists"<<endl;
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
            fout << "info:beSalesManager.cpp:readFileData:from table"
                 <<string(table_name[TABLE_SALES])<<" "<<*totitems<<"entries read"<<endl;
            file.close();
        }
        else{
            fout << "error:beSalesManager.cpp:readFileData:"<<string(table_name[TABLE_SALES])
                 <<"could not be opened"<<endl;
            return ERR_SALES_FILE_OPEN;
        }
    }
    return ERR_NONE;
}
