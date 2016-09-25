#ifndef BECOMMON_H
#define BECOMMON_H

#define MAX_CONTACTS 200

#include <string>
#include <vector>

using namespace std;

typedef vector <string> matrow;
typedef vector <matrow> matrix;

const static int tableHeaderH = 70;
const static int topLeftX = 5;
const static int topLeftY = 5;
const static int linespace = 5;
const static int areamargin = 5;
const static int row_per_page = 10;
const static int maxFontHeight = 20;
const char * const myCompanyName = "ABC PVt. Ltd.";
const char * const myCompanyAddr = "AC 16/3 D B Nagar Kolkata-59";
const char * const myCompanyPhone = "(033)25766253";
const char * const myCcompanyEmail = "Email: abc@abc.com";

enum updateOP{
    SUB = -1,
    NOOP = 0,
    ADD = 1
};

enum status{
    NO_OP=0,
    SAVED=1,
    EDITED=2,
    DELETED=3,
    CANCELLED=4
};

enum errorType {
    ERR_INVALID = -1,
    ERR_NONE = 0,
    ERR_BMANAGER_INIT = 1,
    ERR_UI_INIT ,
    ERR_QAPP,
    ERR_LOG_OPEN,
    ERR_BACKEND_SAVE=5,
    ERR_ROW_DEL,
    ERR_TABLENAME,
    ERR_STOCK_FILE_OPEN,
    ERR_PURCHASE_FILE_OPEN,
    ERR_SALES_FILE_OPEN = 10,
    ERR_SUMMARY_FILE_OPEN,
    ERR_CASHFLOW_FILE_OPEN,
    ERR_PL_FILE_OPEN,
    ERR_DBMANAGER_INIT,
    ERR_DB_OPEN = 15,
    ERR_DB_CREATE,
    ERR_DB_READ,
    ERR_DB_ADD,
    ERR_DB_EDIT,
    ERR_DB_DEL,
    ERR_DB_DEL_ALL,
    ERR_DB_INVALID,
    ERR_PUR_DB_CREATE,
    ERR_DUP_ID,
    ERR_WRONG_ID
};
enum tableType{
    TABLE_INVALID = -1,
    TABLE_NONE = 0,
    TABLE_SUMMARY = 1,
    TABLE_PURCHASE = 2,
    TABLE_SALES = 3,
    TABLE_CASHFLOW = 4,
    TABLE_PL = 5,
    TABLE_STOCK = 6,
    TABLE_ORDER = 7,
    TABLE_RETURN =8,
    TABLE_END
};

enum purui_table_flds{
    //Fields for individual items
    PURUI_NAME,
    PURUI_BATCH,
    PURUI_BOXNO,
    PURUI_PCSNO,
    PURUI_PCSPERBOX,
    PURUI_COSTOFBOX,
    PURUI_COSTOFPCS,
    PURUI_TOTAL,
    PURUI_TAX,
    //Common  part for all items in the same purchase bill
    PURUI_EXPNS,
    PURUI_CASHPAID,
    PURUI_CHEQPAID,
    PURUI_TOTALPAID,
    PURUI_TOTALDUE,
    PURUI_BILL,
    PURUI_PURNO,
    PURUI_DATE,
    PURUI_COMP,
    PURUI_ADDR,
    PURUI_CONTACT,
    PUTRUI_CONTACTNO,
    PURUI_REMARKS,
    PURUI_TAXPER,
    PURUI_GRANDTOTAL,
    PURUI_END
};

enum pur_table_flds{
    PUR_ID,
    PUR_NO,
    PUR_DATE ,
    PUR_BILLNO,
    PUR_SUPP ,
    PUR_TOTCOST ,
    PUR_PAID ,
    PUR_DUE,
    //Other items
    PUR_PROD,
    PUR_BATCH,
    PUR_BOX ,
    PUR_ITEMS ,
    PUR_SUPPADDR,
    PUR_CONTACT,
    PUR_CONTACTNO,
    PUR_REMARKS,
    PUR_PCSPERBOX,
    PUR_COSTOFBOX,
    PUR_COSTOFPCS,
    PUR_TAX,
    PUR_EXPNS,
    PUR_CASHPAID,
    PUR_CHEQPAID,
    PUR_GRANDTOTAL,
    PUR_END
};

enum stock_table_flds{
    PROD_ID = 0,
    PROD_DATE = 1,
    PROD_BATCH = 2,
    PROD_NAME = 3,
    PROD_COMP = 4,
    PROD_MFG = 5,
    PROD_EXP = 6,
    PROD_NOB = 7,
    PROD_NOI = 8,
    PROD_NOTI = 9,
    PROD_CPB = 10,
    PROD_CPP = 11,
    PROD_PPB = 12,
    PROD_END
};

const char * const table_name[9] = {
    "",
    "Summary" ,
    "Purchase",
    "Sales",
    "CashFlow",
    "ProfitLoss",
    "Stock" ,
    "Orders" ,
    "SalesReturn",
} ;


const char * const table_fields[9][24] = {
    {""},
    {"SummaryId"} ,
    /* Purchase table key = purNo + date + billNo + prodname */
    {"PurchaseId","PurchaseNo","Date","BillNo","Supplier","TotalCost", "Paid","Due","ProductName","BatchNo","Box","Pieces","SupplierAddress","Contact","ContactNo", "Remarks","PcsPerBox","CostPerBox", "CostPerPcs","Tax","Expenses","CashPaid","ChequePaid", "GrandTotal"},
    {"SalesId"},
    {"CashFlowId"},
    {"ProfitLossId"},
    /* Stock table key = prodName + batchNo */
    {"StockId", "LastDate", "BatchNo", "ProductName", "Company", "MfgDate", "ExpDate", "Box", "Pieces", "TotalPieces", "CostPerBox", "CostPerPcs", "PcsPerBox" },
    {"OrdersId" },
    {"SalesReturnId"},
} ;

const char * const table_fields_desc[9][24] = {
    {""},
    {""} ,
    {"","10","16","10","32","16","16","16","32","10","8","10","50","25","16","50","8","16", "16","16","16","16","16","16"},
    {""},
    {""},
    {""},
    {"", "16","10","32", "32", "16","16","8","10","16","16","16","16" },
    {"" },
    {""},
} ;

const static int table_ui_no_fields[9] = {
    0,
    0 ,
    8,
    0,
    0,
    0,
    13,
    0,
    0,
} ;

const char * const table_ui_fields[9][13] = {
    {""},
    {""} ,
    /* Purchase table key = purNo + date + billNo */
    {"purid","PurchaseNo","Date","Bill No.","Supplier","Total Cost", "Paid","Due","", "", "", "", ""},
    {""},
    {""},
    {""},
    /* Stock table key = ProductName + Batch No. */
    {"stockid", "LastDate", "Batch No.", "ProductName", "Company", "MfgDate", "ExpDate", "Box", "Items", "TotalItems", "Cost/Box", "Cost/Pcs", "Pcs/Box" },
    {"" },
    {""},
} ;

const char * const table_ui_detail_fields [9][5] = {
    {""},
    {""} ,
    {"purdid","ProductName","Batch No.","Box","Pieces"},
    {""},
    {""},
    {""},
    {""},
    {"" },
    {""},
};

const static int table_ui_no_detail_fields[9] = {
    0,
    0 ,
    5,
    0,
    0,
    0,
    0,
    0,
    0,
} ;
errorType openLogFile();
void closeFile();
#endif

