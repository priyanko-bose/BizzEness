#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "BusinessManager/include/bePurchaseManager.h"
#include "common/include/utils.h"

extern ofstream fout;

using namespace std;

/*
 * BE_PurchaseManager Constructor
 */
BE_PurchaseManager::BE_PurchaseManager()
{
    fout<<"info:bePurchaseManager.cpp:Creating BE_PurchaseManager object"<<endl;
    totItems = 0;
}

/*
 * BE_PurchaseManager Destructor
 */
BE_PurchaseManager::~BE_PurchaseManager()
{
    fout<<"info:bePurchaseManager.cpp:Deleting BE_PurchaseManager object"<<endl;
}

/*
 * This function returns the fields of a purchase table entry
 * identified by field name
 */
const char * BE_PurchaseManager::getElement(purchaseData_t &purchaseData, int itemno)
{
    switch(itemno)
    {
    case PUR_ID:
        return  uintToString(purchaseData.purchase_id).c_str();
    case PUR_NO:
        return  purchaseData.purno.c_str();
    case PUR_DATE:
        return purchaseData.date.c_str();
    case PUR_BILLNO:
        return purchaseData.billno.c_str();
    case PUR_BATCH:
        return purchaseData.batchNo.c_str();
    case PUR_PROD:
        return purchaseData.productName.c_str();
    case PUR_SUPP:
        return purchaseData.supplier.c_str();
    case PUR_BOX:
        return  intToString(purchaseData.noOfBoxes).c_str();
    case PUR_ITEMS:
        return  intToString(purchaseData.noOfItems).c_str();
    case PUR_TOTCOST:
        return  doubleToString(purchaseData.totalCost).c_str();
    case PUR_PAID:
        return  doubleToString(purchaseData.paidAmnt).c_str();
    case PUR_DUE:
        return  doubleToString(purchaseData.dueAmnt).c_str();
    case PUR_SUPPADDR:
        return purchaseData.suppaddr.c_str();
    case PUR_CONTACT:
        return purchaseData.contact.c_str();
    case PUR_CONTACTNO:
        return purchaseData.contactno.c_str();
    case PUR_REMARKS:
        return purchaseData.remarks.c_str();
    case PUR_PCSPERBOX:
        return intToString(purchaseData.pcsperbox).c_str();
    case PUR_COSTOFBOX:
        return  doubleToString(purchaseData.costofbox).c_str();
    case PUR_COSTOFPCS:
        return  doubleToString(purchaseData.costofpcs).c_str();
    case PUR_TAX:
        return  doubleToString(purchaseData.tax).c_str();
    case PUR_EXPNS:
        return  doubleToString(purchaseData.expns).c_str();
    case PUR_CASHPAID:
        return  doubleToString(purchaseData.cashpaid).c_str();
    case PUR_CHEQPAID:
        return  doubleToString(purchaseData.chqpaid).c_str();
    default:
        break;
    };
    return "";
}

/*
 * This function sets the fields of a purchase table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_PurchaseManager::setItem(unsigned int key, int itemno, string val)
{
    //get the purchase table entry
    if(this->purchaseTable.find(key) == this->purchaseTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):bePurchaseManager.cpp:setItem"<<endl;
        return ERR_WRONG_ID;
    }
    else{
        purchaseData_t & purchaseData = this->purchaseTable.at(key);

        switch(itemno)
        {
        case PUR_DATE:
            purchaseData.date = val.c_str();
            break;
        case PUR_BILLNO:
            purchaseData.billno = val.c_str();
            break;
        case PUR_BATCH:
            purchaseData.batchNo = val.c_str();
            break;
        case PUR_PROD:
            purchaseData.productName = val.c_str();
            break;
        case PUR_SUPP:
            purchaseData.supplier = val.c_str();
            break;
        case PUR_BOX:
            purchaseData.noOfBoxes = atoi(val.c_str());;
            break;
        case PUR_ITEMS:
            purchaseData.noOfItems = atoi(val.c_str());
            break;
        case PUR_TOTCOST:
            purchaseData.totalCost = atof(val.c_str());
            break;
        case PUR_PAID:
            purchaseData.paidAmnt = atof(val.c_str());
            break;
        case PUR_DUE:
            purchaseData.dueAmnt= atof(val.c_str());
            break;
        case PUR_NO:
            purchaseData.purno = val.c_str();
            break;
        case PUR_SUPPADDR:
            purchaseData.suppaddr = val.c_str();
            break;
        case PUR_CONTACT:
            purchaseData.contact = val.c_str();
            break;
        case PUR_CONTACTNO:
            purchaseData.contactno = val.c_str();
            break;
        case PUR_REMARKS:
            purchaseData.remarks = val.c_str();
            break;
        case PUR_PCSPERBOX:
            purchaseData.pcsperbox = atoi(val.c_str());
            break;
        case PUR_COSTOFBOX:
            purchaseData.costofbox = atof(val.c_str());
            break;
        case PUR_COSTOFPCS:
            purchaseData.costofpcs = atof(val.c_str());
            break;
        case PUR_TAX:
            purchaseData.tax = atof(val.c_str());
            break;
        case PUR_EXPNS:
            purchaseData.expns = atof(val.c_str());
            break;
        case PUR_CASHPAID:
            purchaseData.cashpaid = atof(val.c_str());
            break;
        case PUR_CHEQPAID:
            purchaseData.chqpaid = atof(val.c_str());
            break;
        default:
            break;
        };
    }
    return ERR_NONE;
}

/*
 * This function sets the fields of a Purchase table entry
 * entry is identified by id and fields are identified by field name
 */
errorType BE_PurchaseManager::updateItem(unsigned int key, int itemno, string val, int updateQ)
{
    return setItem(key,itemno, val);
}

/*
 * This function adds a new purchase table table entry
 * initialized with null values
 */
errorType BE_PurchaseManager::addNewItem(unsigned int key)
{
    if(this->purchaseTable.find(key) == this->purchaseTable.end()){
        purchaseData_t purchaseData;

        purchaseData.billno = "";
        purchaseData.batchNo = "";
        purchaseData.supplier = "";
        purchaseData.date = "";
        purchaseData.productName = "";

        purchaseData.noOfItems = 0;
        purchaseData.noOfBoxes = 0;
        purchaseData.totalCost = 0.0;
        purchaseData.paidAmnt = 0.0;
        purchaseData.dueAmnt = 0.0;

        purchaseData.purno = "";
        purchaseData.suppaddr = "";
        purchaseData.contact = "";
        purchaseData.contactno = "";
        purchaseData.remarks = "";

        purchaseData.pcsperbox = 0;

        purchaseData.costofbox = 0.0;
        purchaseData.costofpcs = 0.0;
        purchaseData.tax = 0.0;
        purchaseData.expns = 0.0;
        purchaseData.cashpaid = 0.0;
        purchaseData.chqpaid = 0.0;

        purchaseData.purchase_id = key;
        //insert into the tock table map
        this->purchaseTable.insert(std::pair<unsigned int,purchaseData_t>(key,purchaseData));
        totItems++;
    }
    else{
        fout << "Key Already Exists:beStockManager.cpp:addNewItem"<<endl;
        return ERR_DUP_ID;
    }
    return ERR_NONE;
}

/*
 * This function saves purchase table table entries
 * into respective data base file
 */
errorType BE_PurchaseManager::saveItems()
{
    ofstream myfile;
    string filename = (string(table_name[TABLE_PURCHASE]) + ".csv" );
    map <unsigned int, purchaseData_t> & mymap = this->purchaseTable;
    myfile.open (filename.c_str());
    if(myfile.is_open())
    {
        for(int iter =  PUR_ID; iter < PUR_END; iter++)
            myfile << table_fields[TABLE_PURCHASE][iter] << ",";
        myfile<<endl;
        for (map<unsigned int,purchaseData_t>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            myfile << it->second.purchase_id<<","<<it->second.purno<<","
               <<it->second.date<<","<< it->second.billno <<","
               <<it->second.batchNo<<","<<it->second.productName<<","<<it->second.supplier<<","
               <<it->second.noOfBoxes<<","<<it->second.noOfItems<<","<<it->second.totalCost<<","
               <<it->second.paidAmnt<<","<<it->second.dueAmnt<<","
               <<it->second.suppaddr<<","
               <<it->second.contact<<","<<it->second.contactno<<","
               <<it->second.remarks<<","<<it->second.pcsperbox<<","
               <<it->second.costofbox<<","<<it->second.costofpcs<<","
               <<it->second.tax<<","<<it->second.expns<<","
               <<it->second.cashpaid<<","<<it->second.chqpaid<<endl;
        }
        fout<<"info:BE_PurchaseManager::savePurchaseManager:saved data:"<<endl;
    }
    else{
        fout<<"error("<<ERR_DB_OPEN<<"):BE_PurchaseManager::savePurchaseManager:Couln't open file to save data:"<<endl;
        return ERR_DB_OPEN;
    }
    myfile.close();
    return ERR_NONE;
}

/*
 * This function returns the purchase table
 */
map <unsigned int, purchaseData_t> & BE_PurchaseManager::getPurchaseTable()
{
    return purchaseTable;
}

/*
 * This function adds a new purchase table table entry
 * initialized with null values
 */
errorType BE_PurchaseManager::deleteItem(unsigned int id)
{
    map <unsigned int, purchaseData_t> & mymap = this->purchaseTable;
    map <unsigned int, purchaseData_t>::iterator it=mymap.find(id);
    if(it != mymap.end())
        mymap.erase (it);
    else{
        fout << "error("<<ERR_WRONG_ID<<"):bePurchaseManager.cpp:deleteItem"<<endl;
        return ERR_WRONG_ID;
    }
    totItems--;
    return ERR_NONE;
}

/*
 * This function adds a new purchase table table entry
 * initialized with values read from the file
 */
errorType BE_PurchaseManager::insertSavedItem(matrow *row)
{
    purchaseData_t purchaseData;
    purchaseData.purchase_id = atoi(row->at(PUR_ID).c_str());
    purchaseData.purno = row->at(PUR_NO);
    purchaseData.date = row->at(PUR_DATE);
    purchaseData.billno = row->at(PUR_BILLNO);
    purchaseData.batchNo = row->at(PUR_BATCH);
    purchaseData.productName = row->at(PUR_PROD);
    purchaseData.supplier = row->at(PUR_SUPP);
    purchaseData.noOfBoxes = atoi(row->at(PUR_BOX).c_str());
    purchaseData.noOfItems = atoi(row->at(PUR_ITEMS).c_str());
    purchaseData.totalCost = atof(row->at(PUR_TOTCOST).c_str());
    purchaseData.paidAmnt = atof(row->at(PUR_PAID).c_str());
    purchaseData.dueAmnt = atof(row->at(PUR_DUE).c_str());

    purchaseData.suppaddr = row->at(PUR_SUPPADDR);
    purchaseData.contact = row->at(PUR_CONTACT);
    purchaseData.contactno = row->at(PUR_CONTACTNO);
    purchaseData.remarks = row->at(PUR_REMARKS);
    purchaseData.pcsperbox = atoi(row->at(PUR_PCSPERBOX).c_str());
    purchaseData.costofbox = atof(row->at(PUR_COSTOFBOX).c_str());
    purchaseData.costofpcs = atof(row->at(PUR_COSTOFPCS).c_str());
    purchaseData.tax = atof(row->at(PUR_TAX).c_str());
    purchaseData.expns = atof(row->at(PUR_EXPNS).c_str());
    purchaseData.cashpaid = atof(row->at(PUR_CASHPAID).c_str());
    purchaseData.chqpaid = atof(row->at(PUR_CHEQPAID).c_str());
    //insert into the purchase table
    this->purchaseTable.insert(std::pair<unsigned int,purchaseData_t>(purchaseData.purchase_id,purchaseData));
    row->clear();
    return ERR_NONE;
}

/*
 * This function adds a new purchase table table entry
 * initialized with values read from the file
 */
errorType BE_PurchaseManager::insertCSVData(CSVRow &row)
{
    purchaseData_t purchaseData;
    purchaseData.purchase_id = atoi(row[PUR_ID].c_str());
    purchaseData.purno = row[PUR_NO];
    purchaseData.date = row[PUR_DATE];
    purchaseData.billno = row[PUR_BILLNO];
    purchaseData.batchNo = row[PUR_BATCH];
    purchaseData.productName = row[PUR_PROD];
    purchaseData.supplier = row[PUR_SUPP];
    purchaseData.noOfBoxes = atoi(row[PUR_BOX].c_str());
    purchaseData.noOfItems = atoi(row[PUR_ITEMS].c_str());
    purchaseData.totalCost = atof(row[PUR_TOTCOST].c_str());
    purchaseData.paidAmnt = atof(row[PUR_PAID].c_str());
    purchaseData.dueAmnt = atof(row[PUR_DUE].c_str());

    purchaseData.suppaddr = row[PUR_SUPPADDR];
    purchaseData.contact = row[PUR_CONTACT];
    purchaseData.contactno = row[PUR_CONTACTNO];
    purchaseData.remarks = row[PUR_REMARKS];
    purchaseData.pcsperbox = atoi(row[PUR_PCSPERBOX].c_str());
    purchaseData.costofbox = atof(row[PUR_COSTOFBOX].c_str());
    purchaseData.costofpcs = atof(row[PUR_COSTOFPCS].c_str());
    purchaseData.tax = atof(row[PUR_TAX].c_str());
    purchaseData.expns = atof(row[PUR_EXPNS].c_str());
    purchaseData.cashpaid = atof(row[PUR_CASHPAID].c_str());
    purchaseData.chqpaid = atof(row[PUR_CHEQPAID].c_str());
    //insert into the purchase table
    this->purchaseTable.insert(std::pair<unsigned int,purchaseData_t>(purchaseData.purchase_id,purchaseData));
    return ERR_NONE;
}

/* Get the record string with purchase data*/
errorType BE_PurchaseManager:: getOneRecord(purchaseData_t &purchaseData, matrow *row)
{
    row->insert(row->begin() + PUR_ID, uintToString(purchaseData.purchase_id).c_str());
    row->insert(row->begin() + PUR_NO , purchaseData.purno.c_str());
    row->insert(row->begin() + PUR_DATE , purchaseData.date.c_str());
    row->insert(row->begin() + PUR_BILLNO , purchaseData.billno.c_str());
    row->insert(row->begin() + PUR_BATCH , purchaseData.batchNo.c_str());
    row->insert(row->begin() + PUR_PROD , purchaseData.productName.c_str());
    row->insert(row->begin() + PUR_SUPP , purchaseData.supplier.c_str());
    row->insert(row->begin() + PUR_BOX , intToString(purchaseData.noOfBoxes).c_str());
    row->insert(row->begin() + PUR_ITEMS , intToString(purchaseData.noOfItems).c_str());
    row->insert(row->begin() + PUR_TOTCOST , doubleToString(purchaseData.totalCost).c_str());
    row->insert(row->begin() + PUR_PAID , doubleToString(purchaseData.paidAmnt).c_str());
    row->insert(row->begin() + PUR_DUE , doubleToString(purchaseData.dueAmnt).c_str());
    row->insert(row->begin() + PUR_SUPPADDR , purchaseData.suppaddr.c_str());
    row->insert(row->begin() + PUR_CONTACT , purchaseData.contact.c_str());
    row->insert(row->begin() + PUR_CONTACTNO , purchaseData.contactno.c_str());
    row->insert(row->begin() + PUR_REMARKS , purchaseData.remarks.c_str());
    row->insert(row->begin() + PUR_PCSPERBOX , intToString(purchaseData.pcsperbox).c_str());
    row->insert(row->begin() + PUR_COSTOFBOX , doubleToString(purchaseData.costofbox).c_str());
    row->insert(row->begin() + PUR_COSTOFPCS , doubleToString(purchaseData.costofpcs).c_str());
    row->insert(row->begin() + PUR_TAX , doubleToString(purchaseData.tax).c_str());
    row->insert(row->begin() + PUR_EXPNS , doubleToString(purchaseData.expns).c_str());
    row->insert(row->begin() + PUR_CASHPAID , doubleToString(purchaseData.cashpaid).c_str());
    row->insert(row->begin() + PUR_CHEQPAID , doubleToString(purchaseData.chqpaid).c_str());
    return ERR_NONE;
}

errorType BE_PurchaseManager :: getRecord(unsigned int id,matrow *record)
{
    //get the purchase table entry
    if(this->purchaseTable.find(id) == this->purchaseTable.end()){
        fout << "error("<<ERR_WRONG_ID<<"):bePurchaseManager.cpp:setItem"<<endl;
        return ERR_WRONG_ID;
    }
    map <unsigned int, purchaseData_t> & mymap = this->purchaseTable;
    getOneRecord(mymap.at(id),record);
    return ERR_NONE;
}

/*
 * This function is used to read the respective BusinessManager Tables
 * for a table and insert those items in a row of strings
 */
errorType BE_PurchaseManager::readBMData(matrix **records, int *totitems)
{
    int err = 0;
    map <unsigned int, purchaseData_t> & mymap = this->purchaseTable;
    map<unsigned int,purchaseData_t>::iterator it;
    *totitems = mymap.size();
    *records = new matrix(*totitems, std::vector<string>());
    int row=0;
    for(it=mymap.begin(); it!=mymap.end(); ++it){
        getOneRecord(it->second, &((*records)->at(row++)));
    }
    fout << "info:bePurchaseManager.cpp:readBMData:from purchase manager map"
                 <<string(table_name[TABLE_PURCHASE])<<" "<<*totitems<<"entries read"<<endl;
    return errorType(err);
}

/*
 * This function is used to read the respective database file
 * for a table and insert those items in BusinessManager Tables
 */
errorType BE_PurchaseManager::readFileData(string path, int *totitems)
{
    string filename(string(table_name[TABLE_PURCHASE])+".csv");
    filename = path + filename;
    std::ifstream  file(filename.c_str());
    if(!file.good()){
        fout << "info:BE_PurchaseManager:readFileData:"<<string(table_name[TABLE_PURCHASE])<<"not exists"<<endl;
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
            fout << "info:BE_PurchaseManager:readFileData:from table"
                 <<string(table_name[TABLE_PURCHASE])<<" "<<*totitems<<"entries read"<<endl;
            file.close();
        }
        else{
            fout << "error:BE_PurchaseManager:readFileData:"<<string(table_name[TABLE_PURCHASE])
                 <<"could not be opened"<<endl;
            return ERR_STOCK_FILE_OPEN;
        }
    }
    return ERR_NONE;
}

/* Delete all Purchase Manager Table Data */
errorType BE_PurchaseManager:: deleteAllItems()
{
    purchaseTable.clear();
    return ERR_NONE;
}
