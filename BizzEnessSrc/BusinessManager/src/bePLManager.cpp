#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "BusinessManager/include/bePLManager.h"
#include "common/include/utils.h"

extern ofstream fout;

using namespace std;

/*
 * BE_PLManager Constructor
 */
BE_PLManager::BE_PLManager()
{
    fout<<"info:bePLManager.cpp:Creating BE_PLManager object"<<endl;
    totItems = 0;
}

/*
 * BE_PLManager Destructor
 */
BE_PLManager::~BE_PLManager()
{
    fout<<"info:bePLManager.cpp:Deleting BE_PLManager object"<<endl;
}

/*
 * This function returns the fields of a pl table entry
 * identified by field name
 */
const char * BE_PLManager::getElement(plData_t &plData, int itemno)
{
    switch(itemno)
    {
    case PL_ID:
        return  intToString(plData.pl_id).c_str();
    case PL_NO:
        return plData.plNo.c_str();
    case PL_DATE:
        return plData.date.c_str();
    case PL_NAME:
        return plData.productName.c_str();
    case PL_COMP:
        return plData.company.c_str();
    case PL_COST:
        return doubleToString(plData.cost).c_str();
    case PL_COST_TAX:
        return doubleToString(plData.costTax).c_str();
    case PL_SALEAMT:
        return doubleToString(plData.saleAmt).c_str();
    case PL_SALEAMT_TAX:
        return doubleToString(plData.saleAmtTax).c_str();
    case PL_EXPS:
        return  doubleToString(plData.expenses).c_str();
    case PL_PROFIT:
    {
        string data = doubleToString(plData.profit).c_str();
        return data.c_str();
    }
    case PL_LOSS:
        return  doubleToString(plData.loss).c_str();
    default:
        break;
    };
    return "";
}

/*
 * This function updates the fields of a pl table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_PLManager::setItem(int key, int itemno, string val)
{
    //get the pl table entry
    if(this->plTable.find(key) == this->plTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):bePLManager.cpp:updatePLManger"<<endl;
        return ERR_WRONG_ID;
    }
    else{
        plData_t & plData = this->plTable.at(key);

        switch(itemno)
        {
        case PL_NO:
            plData.plNo = val.c_str();
            break;
        case PL_DATE:
            plData.date = val.c_str();
            break;
        case PL_NAME:
            plData.productName = val.c_str();
            break;
        case PL_COMP:
            plData.company = val.c_str();
            break;
        case PL_COST:
            plData.cost = atof(val.c_str());
            break;
        case PL_COST_TAX:
            plData.costTax = atof(val.c_str());;
            break;
        case PL_SALEAMT:
            plData.saleAmt = atof(val.c_str());
            break;
        case PL_SALEAMT_TAX:
            plData.saleAmtTax = atof(val.c_str());
            break;
        case PL_EXPS:
            plData.expenses = atof(val.c_str());
            break;
        case PL_PROFIT:
            plData.profit = atof(val.c_str());
            break;
        case PL_LOSS:
            plData.loss = atof(val.c_str());
            break;
        default:
            break;
        };
    }
    return ERR_NONE;
}

/*
 * This function sets the fields of a Profit & Loss table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_PLManager::updateItem(unsigned int key, int itemno, string val)
{
    return setItem(key,itemno, val);
}

/*
 * This function adds a new pl table table entry
 * initialized with null values
 */
errorType BE_PLManager::addNewItem(int key)
{
    plData_t plData;

    plData.company = "";
    plData.plNo = "";
    plData.date = "";
    plData.productName = "";

    plData.cost = 0.0;
    plData.costTax = 0.0;
    plData.saleAmt = 0.0;
    plData.saleAmtTax = 0.0;
    plData.expenses = 0.0;
    plData.profit = 0.0;
    plData.loss = 0.0;

    plData.pl_id = key;
    //insert into the tock table map
    this->plTable.insert(std::pair< int,plData_t>(key,plData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function saves pl table table entries
 * into respective data base file
 */
errorType BE_PLManager::saveItems()
{
    ofstream myfile;
    string filename = (string(table_name[TABLE_PL]) + ".csv" );
    map <int, plData_t> & mymap = this->plTable;
    myfile.open (filename.c_str());
    if(myfile.is_open())
    {
        for(int i=PL_ID; i < PL_END - 1; i++)
        {
            myfile << pl_table_col_name[i] << ",";
        }
        myfile << pl_table_col_name[PL_END - 1]<<endl;
        /*myfile << "PL_Id,Date,ProductName,Company,"
           <<"Area,Cost,SaleAmt,"
           <<"Expenses,Profit,Loss"<<endl;*/
        for (map<int,plData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            for(int i=PL_ID; i < PL_END - 1; i++)
            {
                string data(getElement(it->second, i));
                myfile << data << ",";
            }
            myfile << getElement(it->second,(PL_END - 1)) <<endl;
            /*myfile << it->first<<","<< it->second.date<<","
               <<it->second.productName<<","<<it->second.company<<","
               <<it->second.area<<","<<it->second.cost<<","
               <<it->second.saleAmt<<","<<it->second.expenses<<","
               <<it->second.profit<<","<<it->second.loss<<endl;*/
        }
        fout<<"info:BE_PLManager::savePLManager:saved data:"<<endl;
    }
    else{
        fout<<"error("<<ERR_DB_OPEN<<"):BE_PLManager::savePLManager:Couln't open file to save data:"<<endl;
        return ERR_DB_OPEN;
    }
    myfile.close();
    return ERR_NONE;
}

/*
 * This function returns the pl table
 */
map <int, plData_t> & BE_PLManager::getPLTable()
{
    return plTable;
}

/*
 * This function adds a new pl table table entry
 * initialized with null values
 */
errorType BE_PLManager::deleteItem(int id)
{
    map <int, plData_t> & mymap = this->plTable;
    map<int,plData_t>::iterator it=mymap.find(id);
    if(it != mymap.end())
        mymap.erase (it);
    else{
        fout << "error("<<ERR_WRONG_ID<<"):bePLManager.cpp:deleteItem"<<endl;
        return ERR_WRONG_ID;
    }
    totItems--;
    return ERR_NONE;
}

/*
 * This function adds a new pl table table entry
 * initialized with values read from the file
 */
errorType BE_PLManager::insertSavedItem(CSVRow &row)
{
    plData_t plData;
    plData.pl_id = atoi(row[PL_ID].c_str());
    plData.plNo = row[PL_NO];
    plData.date = row[PL_DATE];
    plData.company = row[PL_COMP];
    plData.productName = row[PL_NAME];

    plData.cost = atof(row[PL_COST].c_str());
    plData.costTax = atof(row[PL_COST_TAX].c_str());
    plData.saleAmt = atof(row[PL_SALEAMT].c_str());
    plData.saleAmtTax = atof(row[PL_SALEAMT_TAX].c_str());
    plData.expenses = atof(row[PL_EXPS].c_str());
    plData.profit = atof(row[PL_PROFIT].c_str());
    plData.loss = atof(row[PL_LOSS].c_str());
    //insert into the pl table
    this->plTable.insert(std::pair< int,plData_t>(plData.pl_id,plData));
    totItems++;
    return ERR_NONE;
}

/*
 * This function is used to read the respective database file
 * for a table and insert those items in BusinessManager Tables
 */
errorType BE_PLManager::readFileData(int *totitems)
{
    string filename(string(table_name[TABLE_PL])+".csv");
    std::ifstream  file(filename.c_str());
    if(!file.good()){
        fout << "info:bePLManager.cpp:readFileData:"<<string(table_name[TABLE_PL])<<"not exists"<<endl;
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
            fout << "info:bePLManager.cpp:readFileData:from table"
                 <<string(table_name[TABLE_PL])<<" "<<*totitems<<"entries read"<<endl;
            file.close();
        }
        else{
            fout << "error:bePLManager.cpp:readFileData:"<<string(table_name[TABLE_PL])
                 <<"could not be opened"<<endl;
            return ERR_PL_FILE_OPEN;
        }
    }
    return ERR_NONE;
}
