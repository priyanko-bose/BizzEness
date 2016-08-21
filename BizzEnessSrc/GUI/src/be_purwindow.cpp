#include "GUI/include/be_mainwindow.h"
#include "ui_be_purchase.h"
#include "common/include/common.h"
#include "Interface/include/ifHandleGUI.h"
#include "Interface/include/ifHandleBM.h"
#include "Interface/include/ifHandleDB.h"
#include "GUI/include/be_guiutils.h"
#include <QMessageBox>
/*
 * BE_PurWindow Constructor
 * Set up all the UI components
 */

extern int isAdmin;

BE_PurWindow::BE_PurWindow(QWidget *parent):
    QDialog(parent),
    memoUi(new Ui::Dialog)
{
    memoUi->setupUi(this);
    initializeSignalsSlots();
    initColumnNameValueMap();
    QListWidget *list = memoUi->prodListWidget;
    QStringList *items = getProductList();
    if(items){
        list->addItems(*items);
        delete items;
        items='\0';
    }
    if(int totaItems = list->count())
    {
        for(int row=0; row < totaItems; row++)
        {
            QListWidgetItem *item = list->item(row);
            item->setCheckState( Qt::Unchecked );
        }
    }
}

/*
 * BE_PurWindow Destructor
 * Delete the GUI components
 */
BE_PurWindow::~BE_PurWindow()
{
    delete memoUi;
}

/*------------------Private Functions-----------------------*/
/*
 * Insert the column ID with respect to the column name
 */
void BE_PurWindow :: initColumnNameValueMap()
{
    columnNameValueMap.insert("ProductName", PURUI_NAME);
    columnNameValueMap.insert("Batch No.", PURUI_BATCH);
    columnNameValueMap.insert("No. of Box", PURUI_BOXNO);
    columnNameValueMap.insert("No. of Pcs.", PURUI_PCSNO);
    columnNameValueMap.insert("Pcs. per Box", PURUI_PCSPERBOX);
    columnNameValueMap.insert("Cost of Box", PURUI_COSTOFBOX);
    columnNameValueMap.insert("Cost of Pcs.", PURUI_COSTOFPCS);
    columnNameValueMap.insert("Tax", PURUI_TAX);
    columnNameValueMap.insert("Expenses", PURUI_EXPNS);
    columnNameValueMap.insert("Total Cost", PURUI_TOTALCOST);
    columnNameValueMap.insert("Paid by Cash", PURUI_CASHPAID);
    columnNameValueMap.insert("Paid by cheque", PURUI_CHEQPAID);
    columnNameValueMap.insert("Total Paid", PURUI_TOTALPAID);
    columnNameValueMap.insert("Due",PURUI_DUE);
}

/*
 * Get the column ID with respect to the column name
 */
purui_table_flds BE_PurWindow :: getColumnNameValue(QString colName){
    if(columnNameValueMap.contains(colName))
       return (columnNameValueMap.value(colName));
    else
       return PURUI_END;
}

/*
 * Get the column id which contains
 * the given purchase memo table column type
 */

int BE_PurWindow :: getColID(purui_table_flds flds)
{
    for(int j=0; j < memoUi->purTableWidget->columnCount(); j++ )
    {
        if(getColumnNameValue(memoUi->purTableWidget->horizontalHeaderItem(j)->text().trimmed()) == flds)
        {
            return j;
        }
        else
            continue;
    }
    return -1;
}

/*
 * BE_PurWindow initialization of Signals and Slots
 */
void BE_PurWindow::initializeSignalsSlots()
{
    /*------------------SLOTS & SIGNALS-----------------------*/
    connect(memoUi->purAddPushButton,&QPushButton::clicked,this,&BE_PurWindow::on_purAddPushButton_clicked);
    connect(memoUi->purDelPushButton,&QPushButton::clicked,this,&BE_PurWindow::on_purDelPushButton_clicked);
    connect(memoUi->productListButtonBox,&QDialogButtonBox::accepted,this,&BE_PurWindow::on_productListButtonBox_accepted);
    connect(memoUi->productListButtonBox,&QDialogButtonBox::rejected,this,&BE_PurWindow::on_productListButtonBox_rejected);
    connect(memoUi->purMemoUIButtonBox,&QDialogButtonBox::accepted,this,&BE_PurWindow::on_purMemoUIButtonBox_accepted);
    connect(memoUi->purMemoUIButtonBox,&QDialogButtonBox::rejected,this,&BE_PurWindow::on_purMemoUIButtonBox_rejected);
}


/*------------------SLOTS Definitions-----------------------*/
/*
 * This function adds a row at the products table
 */
void BE_PurWindow::on_purAddPushButton_clicked()
{
   int count = memoUi->purTableWidget->rowCount();
   memoUi->purTableWidget->insertRow(count);
}
/*
 * This function deletes a row at the products table
 */
void BE_PurWindow::on_purDelPushButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Confirm");
    msgBox.setInformativeText("Do you want to delete current record?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if(ret == QMessageBox::No)
        return;
   int row = memoUi->purTableWidget->currentRow();
   QListWidget *list = memoUi->prodListWidget;
   QString prodname = "";
   if(memoUi->purTableWidget->item(row,0))
       prodname = memoUi->purTableWidget->item(row,0)->text();//rectify
   memoUi->purTableWidget->removeRow(row);
   int totaItems = list->count();
   if((totaItems) && (prodname == ""))
   {
       for(int row=0; row < totaItems; row++)
       {
           QListWidgetItem *item = list->item(row);
           if(item->text() == prodname){
               item->setHidden(false);
               item->setCheckState( Qt::Unchecked );
           }
       }
   }
}
/*
 * This fucntion accepts the checked products and polpulates to table
 */
void BE_PurWindow::on_productListButtonBox_accepted()
{
    QListWidget *list = memoUi->prodListWidget;
    if(int totaItems = list->count())
    {
        for(int row=0; row < totaItems; row++)
        {
            QListWidgetItem *item = list->item(row);
            if(item->checkState())
            {
                //insert each product in product table widget
                int count = memoUi->purTableWidget->rowCount();
                memoUi->purTableWidget->insertRow(count);
                QTableWidgetItem *tItem = new QTableWidgetItem();
                tItem->setText(item->text());
                memoUi->purTableWidget->setItem(count,0,tItem);
                item->setHidden(true);
            }
            item->setCheckState( Qt::Unchecked );
        }
    }

}
void BE_PurWindow::on_productListButtonBox_rejected()
{
    QListWidget *list = memoUi->prodListWidget;
    if(int totaItems = list->count())
    {
        for(int row=0; row < totaItems; row++)
        {
            QListWidgetItem *item = list->item(row);
            if(item->checkState())
                item->setCheckState( Qt::Unchecked );
        }
    }
}

/*
 * ---------------------------------------------------------------------------------------
 * This SLOTS are called when purchase memo are accepted or rejected
 *----------------------------------------------------------------------------------------
 */
void BE_PurWindow::on_purMemoUIButtonBox_accepted()
{
    QTableWidget *curTable = BE_MainWindow::getBeMainWindow()->getPurchaseTable();
    int rowCount = memoUi->purTableWidget->rowCount();
    for(int rowi =0 ;rowi<rowCount; rowi++)
    {
        QString prodName = "";
        QString batchNo = "";
        QString billNo = "";
        QString date = "";
        unsigned int encodedPass = 0;
        QTableWidgetItem *item = '\0';
        matrow *record = '\0';
        item = memoUi->purTableWidget->item(rowi, getColID(PURUI_NAME));
        if(item)
        {
            /* Update business manager */
            prodName = item->text().trimmed(); //get product name
            item = '\0';
            item = memoUi->purTableWidget->item(rowi, getColID(PURUI_BATCH));
            if(item)
                batchNo = item->text().trimmed(); //get product batch no.
            item = '\0';
            billNo = memoUi->billNoLineEdit->text().trimmed(); //get the bill no
            date = memoUi->dateEdit->text().trimmed();  //get the date
            QString purNo = memoUi->purNoLineEdit->text().trimmed();
            encodedPass = hashCode(prodName + batchNo + billNo + date);

            addItemToBusinessManager(TABLE_PURCHASE,encodedPass);

            for(int col = PURUI_NAME; col <= PURUI_DUE; col++)
            {
                if(memoUi->purTableWidget->item(rowi,col))
                    populatePurWindowData(encodedPass, col,
                        memoUi->purTableWidget->item(rowi,getColID((purui_table_flds)col))->text().trimmed().toStdString());
            }
            populatePurWindowData(encodedPass, PURUI_BILL,
               billNo.toStdString());
            populatePurWindowData(encodedPass, PURUI_PURNO,
               purNo.toStdString());
            populatePurWindowData(encodedPass, PURUI_DATE,
               memoUi->dateEdit->text().trimmed().toStdString());
            populatePurWindowData(encodedPass, PURUI_COMP,
               memoUi->companyLineEdit->text().trimmed().toStdString());
            populatePurWindowData(encodedPass, PURUI_ADDR,
               memoUi->addrTextEdit->toPlainText().trimmed().toStdString());
            populatePurWindowData(encodedPass, PURUI_CONTACT,
               memoUi->contactLineEdit->text().trimmed().toStdString());
            populatePurWindowData(encodedPass, PUTRUI_CONTACTNO,
               memoUi->contactNoLineEdit->text().trimmed().toStdString());
            populatePurWindowData(encodedPass, PURUI_REMARKS,
               memoUi->remarksTextEdit->toPlainText().trimmed().toStdString());

            int row = curTable->rowCount();
            curTable->insertRow(row);
            for (int col = 0; col < curTable->columnCount(); col++)
            {
                QTableWidgetItem *data = new QTableWidgetItem();
                if(!isAdmin || col == PUR_ID)
                     data->setFlags(data->flags() & ~Qt::ItemIsEditable);
                if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][0])
                {
                    data->setText(QString::number(encodedPass));
                    curTable->setItem(row,col, data);
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][1])
                {
                    data->setText(purNo);
                    curTable->setItem(row,col, data);
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][2])
                {
                    data->setText(memoUi->dateEdit->text().trimmed());
                    curTable->setItem(row,col, data);
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][3])
                {
                    data->setText(memoUi->billNoLineEdit->text().trimmed());
                    curTable->setItem(row,col, data);
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][4])
                {
                    data->setText(batchNo);
                    curTable->setItem(row,col, data);
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][5])
                {
                    data->setText(prodName);
                    curTable->setItem(row,col, data);
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][6])
                {
                    data->setText(memoUi->companyLineEdit->text().trimmed());
                    curTable->setItem(row,col, data);
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][7])
                {
                    if(memoUi->purTableWidget->item(rowi,getColID(PURUI_BOXNO))){
                        data->setText(memoUi->purTableWidget->item(rowi,getColID(PURUI_BOXNO))->text().trimmed());
                        curTable->setItem(row,col, data);
                    }
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][8])
                {
                    if(memoUi->purTableWidget->item(rowi,getColID(PURUI_PCSNO))){
                        data->setText(memoUi->purTableWidget->item(rowi,getColID(PURUI_PCSNO))->text().trimmed());
                        curTable->setItem(row,col, data);
                    }
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][9])
                {
                    if(memoUi->purTableWidget->item(rowi,getColID(PURUI_TOTALCOST))){
                        data->setText(memoUi->purTableWidget->item(rowi,getColID(PURUI_TOTALCOST))->text().trimmed());
                        curTable->setItem(row,col, data);
                    }
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][10])
                {
                    if(memoUi->purTableWidget->item(rowi,getColID(PURUI_TOTALPAID))){
                        data->setText(memoUi->purTableWidget->item(rowi,getColID(PURUI_TOTALPAID))->text().trimmed());
                        curTable->setItem(row,col, data);
                    }
                }
                else if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][11])
                {
                    if(memoUi->purTableWidget->item(rowi,getColID(PURUI_DUE))){
                        data->setText(memoUi->purTableWidget->item(rowi,getColID(PURUI_DUE))->text().trimmed());
                        curTable->setItem(row,col, data);
                    }
                }
                else{
                    continue;
                }
                if(curTable->item(row,col))
                    curTable->item(row,col)->setToolTip(curTable->item(row,col)->text());
            }
            prepareRecord(TABLE_PURCHASE, encodedPass, &record);
            saveRecordInDB(TABLE_PURCHASE, encodedPass, record);
        }
    }
}

void BE_PurWindow::on_purMemoUIButtonBox_rejected()
{

}
