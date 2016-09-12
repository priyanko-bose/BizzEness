#include "GUI/include/be_mainwindow.h"
#include "ui_be_purchase.h"
#include "common/include/common.h"
#include "Interface/include/ifHandleGUI.h"
#include "Interface/include/ifHandleBM.h"
#include "Interface/include/ifHandleDB.h"
#include "GUI/include/be_guiutils.h"
#include <QMessageBox>
#include <QPrintPreviewDialog>
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

    QStringList *comps = getCompanyList();
    if(comps){
        memoUi->companyComboBox->addItems(*comps);
        memoUi->companyComboBox->setCurrentIndex(0);
        delete comps;
        comps = '\0';
    }
    QStringList *compProds = getFilteredList(TABLE_STOCK, PROD_COMP, PROD_NAME,memoUi->companyComboBox->itemText(0));

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
            QRegularExpression re("(.*)\\((\\S+)\\)$");
            QRegularExpressionMatch match = re.match(item->text());
            QString prodname;
            if (match.hasMatch()) {
                prodname = match.captured(1); // extract product name
                if(!compProds->contains(prodname))
                    item->setHidden(true);
                else
                    item->setHidden(false);
            }

        }
    }
    if(compProds){
        delete compProds;
        compProds='\0';
    }
    QString *compAddr = '\0';
    compAddr = getTableItem(TABLE_PURCHASE, PUR_SUPP, memoUi->companyComboBox->itemText(0), PUR_SUPPADDR);
    if(compAddr){
        memoUi->addrTextEdit->setPlainText(*compAddr);
        delete compAddr;
        compAddr='\0';
    }
    connect(memoUi->companyComboBox, SIGNAL(currentIndexChanged(int)),this,SLOT(on_currentIndexChanged_clicked(int)));
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
    connect(memoUi->purTableWidget,&QTableWidget::cellChanged,this,&BE_PurWindow::on_purTableWidget_cellChanged);
    connect(memoUi->printPushButton,&QPushButton::clicked,this,&BE_PurWindow::on_printPushButton_clicked);

}


/*------------------SLOTS Definitions-----------------------*/
/*
 * This function adds a row at the products table
 */
void BE_PurWindow::on_purAddPushButton_clicked()
{
   int count = memoUi->purTableWidget->rowCount();
   memoUi->purTableWidget->insertRow(count);
   QTableWidgetItem *tItem = new QTableWidgetItem();
   tItem->setFlags(tItem->flags() & ~Qt::ItemIsEditable);
   memoUi->purTableWidget->setItem(count, getColID(PURUI_TOTALCOST), tItem);

   tItem = new QTableWidgetItem();
   tItem->setFlags(tItem->flags() & ~Qt::ItemIsEditable);
   memoUi->purTableWidget->setItem(count, getColID(PURUI_TOTALPAID), tItem);

   tItem = new QTableWidgetItem();
   tItem->setFlags(tItem->flags() & ~Qt::ItemIsEditable);
   memoUi->purTableWidget->setItem(count, getColID(PURUI_DUE), tItem);
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
                QRegularExpression re("(.*)\\((\\S+)\\)$");
                QRegularExpressionMatch match = re.match(item->text());
                QString prodname, batchno;
                if (match.hasMatch()) {
                    prodname = match.captured(1); // extract product name
                    batchno= match.captured(2); // extract batch no
                }
                int count = memoUi->purTableWidget->rowCount();
                memoUi->purTableWidget->insertRow(count);

                QTableWidgetItem *tItem = new QTableWidgetItem();
                tItem->setFlags(tItem->flags() & ~Qt::ItemIsEditable);
                memoUi->purTableWidget->setItem(count, getColID(PURUI_TOTALCOST), tItem);

                tItem = new QTableWidgetItem();
                tItem->setFlags(tItem->flags() & ~Qt::ItemIsEditable);
                memoUi->purTableWidget->setItem(count, getColID(PURUI_TOTALPAID), tItem);

                tItem = new QTableWidgetItem();
                tItem->setFlags(tItem->flags() & ~Qt::ItemIsEditable);
                memoUi->purTableWidget->setItem(count, getColID(PURUI_DUE), tItem);

                tItem = new QTableWidgetItem();
                tItem->setText(prodname);
                memoUi->purTableWidget->setItem(count,getColID(PURUI_NAME),tItem);

                tItem = new QTableWidgetItem();
                tItem->setText(batchno);
                memoUi->purTableWidget->setItem(count,getColID(PURUI_BATCH),tItem);

                matrow *record = '\0';
                prepareRecord(TABLE_STOCK, hashCode(prodname + batchno), &record);

                tItem = new QTableWidgetItem();
                tItem->setText(QString::fromStdString(record->at(PROD_CPB)));
                memoUi->purTableWidget->setItem(count,getColID(PURUI_COSTOFBOX),tItem);

                tItem = new QTableWidgetItem();
                tItem->setText(QString::fromStdString(record->at(PROD_CPP)));
                memoUi->purTableWidget->setItem(count,getColID(PURUI_COSTOFPCS),tItem);

                tItem = new QTableWidgetItem();
                tItem->setText(QString::fromStdString(record->at(PROD_PPB)));
                memoUi->purTableWidget->setItem(count,getColID(PURUI_PCSPERBOX),tItem);

                if(record){
                    delete record;
                    record = '\0';
                }
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
 * When company list items are selected/ chganged
 */
void BE_PurWindow::on_currentIndexChanged_clicked(int index)
{
    QStringList *compProds = getFilteredList(TABLE_STOCK, PROD_COMP, PROD_NAME,memoUi->companyComboBox->itemText(index));

    QListWidget *list = memoUi->prodListWidget;

    if(int totaItems = list->count())
    {
        for(int row=0; row < totaItems; row++)
        {
            QListWidgetItem *item = list->item(row);
            item->setCheckState( Qt::Unchecked );
            QRegularExpression re("(.*)\\((\\S+)\\)$");
            QRegularExpressionMatch match = re.match(item->text());
            QString prodname;
            if (match.hasMatch()) {
                prodname = match.captured(1); // extract product name
                if(!compProds->contains(prodname))
                    item->setHidden(true);
                else
                    item->setHidden(false);
            }

        }
    }
    if(compProds){
        delete compProds;
        compProds='\0';
    }
    QString *compAddr = '\0';
    compAddr = getTableItem(TABLE_PURCHASE, PUR_SUPP, memoUi->companyComboBox->itemText(index), PUR_SUPPADDR);
    if(compAddr){
        memoUi->addrTextEdit->setPlainText(*compAddr);
        delete compAddr;
        compAddr='\0';
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
        unsigned int uniqueIds [10] = {0,0,0,0,0,0,0,0,0,0};
        QTableWidgetItem *item = '\0';
        matrow *record = '\0';
        item = memoUi->purTableWidget->item(rowi, getColID(PURUI_NAME));
        if(item)
        {
            prodName = item->text().trimmed(); //get product name
            item = '\0';
            item = memoUi->purTableWidget->item(rowi, getColID(PURUI_BATCH));
            if(item)
                batchNo = item->text().trimmed(); //get product batch no.
            item = '\0';
            billNo = memoUi->billNoLineEdit->text().trimmed(); //get the bill no
            date = memoUi->dateEdit->text().trimmed();  //get the date
            QString purNo = memoUi->purNoLineEdit->text().trimmed();

            /* Update business manager -- start*/
            uniqueIds[TABLE_PURCHASE] = hashCode(prodName + batchNo + billNo + date);
            addItemToBusinessManager(TABLE_PURCHASE,uniqueIds[TABLE_PURCHASE]);

            uniqueIds[TABLE_STOCK] = hashCode(prodName + batchNo);
            int stockIdStatus = addItemToBusinessManager(TABLE_STOCK, uniqueIds[TABLE_STOCK]);

            for(int col = PURUI_NAME; col <= PURUI_DUE; col++)
            {
                if(memoUi->purTableWidget->item(rowi,col)){
                    if(col == PURUI_PCSPERBOX)
                    {
                        updateOP stockOP = ADD;
                        int noOfTotItems = memoUi->purTableWidget->item(rowi,getColID(PURUI_BOXNO))->text().trimmed().toInt() \
                                            * memoUi->purTableWidget->item(rowi,getColID((purui_table_flds)col))->text().trimmed().toInt();
                        updateToBusinessManager(TABLE_STOCK, uniqueIds[TABLE_STOCK], PROD_NOTI, QString::number(noOfTotItems).toStdString(), stockOP);
                    }
                    populatePurWindowData(uniqueIds, col,
                        memoUi->purTableWidget->item(rowi,getColID((purui_table_flds)col))->text().trimmed().toStdString());
                }
            }
            populatePurWindowData(uniqueIds, PURUI_BILL,
               billNo.toStdString());
            populatePurWindowData(uniqueIds, PURUI_PURNO,
               purNo.toStdString());
            populatePurWindowData(uniqueIds, PURUI_DATE,
               memoUi->dateEdit->text().trimmed().toStdString());
            populatePurWindowData(uniqueIds, PURUI_COMP,
               memoUi->companyComboBox->lineEdit()->text().trimmed().toStdString());
            populatePurWindowData(uniqueIds, PURUI_ADDR,
               memoUi->addrTextEdit->toPlainText().trimmed().toStdString());
            populatePurWindowData(uniqueIds, PURUI_CONTACT,
               memoUi->contactLineEdit->text().trimmed().toStdString());
            populatePurWindowData(uniqueIds, PUTRUI_CONTACTNO,
               memoUi->contactNoLineEdit->text().trimmed().toStdString());
            populatePurWindowData(uniqueIds, PURUI_REMARKS,
               memoUi->remarksTextEdit->toPlainText().trimmed().toStdString());
            /* Update business manager -- end*/

            /* Update UI -- start*/
            int row = curTable->rowCount();
            curTable->insertRow(row);
            for (int col = 0; col < curTable->columnCount(); col++)
            {
                QTableWidgetItem *data = new QTableWidgetItem();
                if(!isAdmin || col == PUR_ID)
                     data->setFlags(data->flags() & ~Qt::ItemIsEditable);
                if(curTable->horizontalHeaderItem(col)->text().trimmed() == table_ui_fields[TABLE_PURCHASE][0])
                {
                    data->setText(QString::number(uniqueIds[TABLE_PURCHASE]));
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
                    data->setText(memoUi->companyComboBox->lineEdit()->text().trimmed());
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
            /* Update UI -- end*/

            /* Update Database -- start*/
            prepareRecord(TABLE_PURCHASE, uniqueIds[TABLE_PURCHASE], &record);
            saveRecordInDB(TABLE_PURCHASE, uniqueIds[TABLE_PURCHASE], record);
            if(record){
                delete record;
                record = '\0';
            }
            prepareRecord(TABLE_STOCK, uniqueIds[TABLE_STOCK], &record);
            if(stockIdStatus == ERR_DUP_ID){
                updateRecordInDB(TABLE_STOCK, uniqueIds[TABLE_STOCK], record);
            }
            else{
                saveRecordInDB(TABLE_STOCK, uniqueIds[TABLE_STOCK], record);
            }
            emit BE_MainWindow::getBeMainWindow()->signalUpdateStockTable(record);
            /* Update Database -- end*/
            if(record){
                delete record;
                record = '\0';
            }
        }
        else
           continue;
    }
    BE_MainWindow::getBeMainWindow()->diableSaveCancel();
    BE_MainWindow::getBeMainWindow()->isStockTableEdited = false;
    BE_MainWindow::getBeMainWindow()->isPurchaseTableEdited = false;
}

void BE_PurWindow::on_purMemoUIButtonBox_rejected()
{

}


void BE_PurWindow :: on_purTableWidget_cellChanged(int row, int col){
    QTableWidget *table = memoUi->purTableWidget;
    int rowCount = memoUi->purTableWidget->rowCount();
    double cost = 0.0, paid = 0.0, due = 0.0;
    double totalcost=0.0, totaltax=0.0, totalpaid=0.0, totaldue=0.0, grandtotal=0.0;
    if((col == getColID(PURUI_PCSNO)) || (col == getColID(PURUI_COSTOFPCS))
            || (col == getColID(PURUI_BOXNO)) || (col == getColID(PURUI_COSTOFBOX))
                || (col == getColID(PURUI_EXPNS)) || (col == getColID(PURUI_TAX))){

        if(table->item(row, getColID(PURUI_PCSNO)) && table->item(row,getColID(PURUI_COSTOFPCS)))
            cost = cost + table->item(row, getColID(PURUI_PCSNO))->text().toInt() *
                       table->item(row,getColID(PURUI_COSTOFPCS))->text().toDouble();

        if(table->item(row, getColID(PURUI_BOXNO)) && table->item(row,getColID(PURUI_COSTOFBOX)))
            cost = cost + table->item(row, getColID(PURUI_BOXNO))->text().toInt() *
                       table->item(row,getColID(PURUI_COSTOFBOX))->text().toDouble();

        if(table->item(row, getColID(PURUI_EXPNS)))
            cost = cost + table->item(row,getColID(PURUI_EXPNS))->text().toDouble();

        if(table->item(row, getColID(PURUI_TAX))){
            cost = cost + table->item(row,getColID(PURUI_TAX))->text().toDouble();
            for(int iter = 0; iter < rowCount; iter++ ){
                if(table->item(iter, getColID(PURUI_TAX)))
                    totaltax = totaltax + table->item(iter, getColID(PURUI_TAX))->text().toDouble();
            }
            memoUi->totalTaxLineEdit->setText(QString::number(totaltax));
        }


        if(!table->item(row, getColID(PURUI_TOTALCOST))){
            QTableWidgetItem *item = new QTableWidgetItem();
            table->setItem(row, getColID(PURUI_TOTALCOST), item);
        }
        table->item(row, getColID(PURUI_TOTALCOST))->setText(QString::number(cost));
    }

    if((col == getColID(PURUI_CASHPAID)) || (col == getColID(PURUI_CHEQPAID))){
        if(table->item(row, getColID(PURUI_CASHPAID)))
            paid = paid  + table->item(row,getColID(PURUI_CASHPAID))->text().toDouble();
        if(table->item(row, getColID(PURUI_CHEQPAID)))
            paid = paid  + table->item(row,getColID(PURUI_CHEQPAID))->text().toDouble();

        if(!table->item(row, getColID(PURUI_TOTALPAID))){
            QTableWidgetItem *item = new QTableWidgetItem();
            table->setItem(row, getColID(PURUI_TOTALPAID), item);
        }
        table->item(row, getColID(PURUI_TOTALPAID))->setText(QString::number(paid));
        for(int iter = 0; iter < rowCount; iter++ ){
            if(table->item(iter, getColID(PURUI_TOTALPAID)))
                totalpaid = totalpaid + table->item(iter, getColID(PURUI_TOTALPAID))->text().toDouble();
        }
        memoUi->totalPaidLineEdit->setText(QString::number(totalpaid));
    }

    if((col == getColID(PURUI_TOTALCOST)) || (col == getColID(PURUI_TOTALPAID))){
        if(table->item(row, getColID(PURUI_TOTALCOST)))
            due = due + table->item(row,getColID(PURUI_TOTALCOST))->text().toDouble();
        if(table->item(row, getColID(PURUI_TOTALPAID)))
            due = due - table->item(row,getColID(PURUI_TOTALPAID))->text().toDouble();

        if(!table->item(row, getColID(PURUI_DUE))){
            QTableWidgetItem *item = new QTableWidgetItem();
            table->setItem(row, getColID(PURUI_DUE), item);
        }
        table->item(row, getColID(PURUI_DUE))->setText(QString::number(due));
        for(int iter = 0; iter < rowCount; iter++ ){
            if(table->item(iter, getColID(PURUI_TOTALCOST)))
                grandtotal = grandtotal + table->item(iter, getColID(PURUI_TOTALCOST))->text().toDouble();
            if(table->item(iter, getColID(PURUI_TOTALPAID)))
                totalpaid = totalpaid + table->item(iter, getColID(PURUI_TOTALPAID))->text().toDouble();
            if(table->item(iter, getColID(PURUI_DUE)))
                totaldue = totaldue + table->item(iter, getColID(PURUI_DUE))->text().toDouble();
            totalcost = grandtotal - memoUi->totalTaxLineEdit->text().toDouble();
        }
        memoUi->grandTotalLineEdit->setText(QString::number(grandtotal));
        memoUi->totalCostLineEdit->setText(QString::number(totalcost));
        memoUi->totalDueLineEdit->setText(QString::number(totaldue));
    }
}

void BE_PurWindow::print(QPrinter *printer)
{
    QPainter painter(printer);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    QTableWidget *table = memoUi->purTableWidget;
    int printAreaW = printer->pageRect().width();
    int printAreaH = printer->pageRect().height();

    int offsetH = 10 * topLeftY;
    int offsetW = 10 * topLeftX;

    offsetH = drawDoubleLines(&painter, offsetW - areamargin, offsetH, printAreaW - offsetW + areamargin, offsetH) + linespace;

    QRect rect = QRect(QPoint(offsetW,offsetH),QPoint((printAreaW - offsetW)/3, offsetH + maxFontHeight));
    printLine(&painter, rect.bottomLeft(), "PurchaseNo: " + memoUi->purNoLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.6);

    rect = QRect(QPoint((printAreaW - offsetW)/3, offsetH),QPoint(((printAreaW - offsetW)*2)/3, offsetH + maxFontHeight));
    printLine(&painter, rect.bottomLeft(), "Bill No: " + memoUi->billNoLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.6);

    rect = QRect(QPoint(((printAreaW - offsetW)*2)/3, offsetH),QPoint(printAreaW - offsetW, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Dated: " + memoUi->dateEdit->text(),
              "Times New Roman", maxFontHeight * 0.6) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint(printAreaW - offsetW, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Company Name: " + memoUi->companyComboBox->lineEdit()->text(),
              "Helvetica", maxFontHeight * 0.9) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint(printAreaW - offsetW, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Address: " + memoUi->addrTextEdit->toPlainText(),
              "Times New Roman", maxFontHeight * 0.75) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint(printAreaW - offsetW, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Contact Person: " + memoUi->contactLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.75) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint(printAreaW - (offsetW), offsetH + maxFontHeight));

    offsetH = printLine(&painter, rect.bottomLeft(), "Contact: " + memoUi->contactNoLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.75) + linespace;

    offsetH = drawDoubleLines(&painter, offsetW,offsetH,printAreaW - offsetW, offsetH);

    int count = 0;
    int columns = table->columnCount();
    int rows = table->rowCount();
    int left = rows;
    int start = 0;
    int end = 0;

    rows = row_per_page;
    while(left > 0 ){
         //Logic to select the portion of Table Widget
        start = count * row_per_page;
        end = start + (left > row_per_page ? row_per_page :left);
        count++;
        if(left > row_per_page){
            left = left - row_per_page;
        }
        else{
            left = 0;
        }

        //Copy the required part from the QTableWidget to for Qtable view fot rendering
        QAbstractItemModel *model = '\0';
        model = getTableArea(table, start, 0, end, columns);
        if(!model)
            return;
        QTableView pTableView;
        pTableView.setModel(model);
        pTableView.resizeColumnsToContents();
        int width = getTableAreaWidth(&pTableView, columns, table->verticalHeader()->width());
        int height = getTableAreaHeight(&pTableView, rows, table->horizontalHeader()->height());
        pTableView.setFixedSize(width, height);
        pTableView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pTableView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //Adjust painter object to accomodate full printing page
        painter.save();
        double xscale = (printAreaW - (offsetW * 2))/double(width);
        double yscale = (printAreaH - offsetH)/double(height);
        double scale =   qMin(xscale, yscale);
        painter.translate(qreal(offsetW * 1.0), qreal(offsetH * 1.0));
        painter.scale(double(int(scale * 100)/100.0), double(int(scale * 100)/100.0));
        pTableView.render(&painter);
        offsetH = offsetH + height  * double(int(scale * 100)/100.0);
        if(left > 0){
            printer->newPage();
            offsetW = 10 * topLeftX;
            offsetH = 10 * topLeftY;
        }
        delete model;
        model = '\0';
        painter.restore();
    }

    offsetH = drawDoubleLines(&painter, offsetW,offsetH,printAreaW - offsetW, offsetH) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint((printAreaW - offsetW)/2, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Total(incl expn): " + memoUi->totalCostLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.75) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint((printAreaW - offsetW)/2, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Tax: " + memoUi->totalTaxLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.75) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint((printAreaW - offsetW)/2, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Grand Total(incl expn): " + memoUi->grandTotalLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.75) + linespace;

    rect = QRect(QPoint(offsetW,offsetH),QPoint((printAreaW - offsetW)/2, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Total Paid: " + memoUi->totalPaidLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.75);

    rect = QRect(QPoint(offsetW,offsetH),QPoint((printAreaW - offsetW)/2, offsetH + maxFontHeight));
    offsetH = printLine(&painter, rect.bottomLeft(), "Total Due: " + memoUi->totalDueLineEdit->text(),
              "Times New Roman", maxFontHeight * 0.75) + linespace;

    offsetH = drawDoubleLines(&painter, offsetW - areamargin,offsetH, printAreaW - offsetW + areamargin, offsetH);
}

void BE_PurWindow::on_printPushButton_clicked(){

    QPrinter printer;
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowFlags ( Qt::Window );
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(print(QPrinter *)));
    preview.exec();
}
