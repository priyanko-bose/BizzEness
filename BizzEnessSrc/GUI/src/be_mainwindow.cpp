#include <fstream>
#include "ui_be_mainwindow.h"
#include "GUI/include/be_mainwindow.h"
#include "GUI/include/be_purwindow.h"
#include "common/include/common.h"
#include "Interface/include/ifHandleBM.h"
#include "Interface/include/ifHandleDB.h"
#include "Interface/include/ifHandleGUI.h"
#include "GUI/include/be_guiutils.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>

extern ofstream fout;
extern int isAdmin;

BE_MainWindow* BE_MainWindow::beMainWindowObj = NULL;

/*
 * BE_MainWindow Constructor
 * Set up all the UI components
 */
BE_MainWindow::BE_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    startProdId = 100;
    startSalesId = 100;
    startPLId = 100;
    startSumId = 100;
    startCashFlowId = 100;

    isStockTableEdited = false;
    isPurchaseTableEdited = false;
    isSalesTableEdited = false;
    isPLTableEdited = false;
    isCashFlowTableEdited = false;
    //Set up the main window
    ui->setupUi(this);

    //Prepare Purchase Table dynamically
    ui->purchaseTableWidget->setColumnCount(12);
    for(int col = 0; col <= 11 ; col++){
        QTableWidgetItem *header1 = new QTableWidgetItem();
        header1->setText(table_ui_fields[TABLE_PURCHASE][col]);
        ui->purchaseTableWidget->setHorizontalHeaderItem(col,header1);
    }
    //Show/Don't Show Purchase Table Unique Id
    ui->purchaseTableWidget->setColumnHidden(0,false);


    ui->stockTableWidget->setColumnCount(13);
    for(int col = 0; col <= 12 ; col++){
        QTableWidgetItem *header1 = new QTableWidgetItem();
        header1->setText(table_ui_fields[TABLE_STOCK][col]);
        ui->stockTableWidget->setHorizontalHeaderItem(col,header1);
    }
    //Show/Don't Show Stock Table Unique Id
    ui->stockTableWidget->setColumnHidden(0,false);


    ui->savePushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(false);
    //Select the summary window as current window
    ui->tablesTabWidget->setCurrentIndex(0);
    curTable = ui->sumTableWidget;
    curTableId = TABLE_SUMMARY;


    /*------------------SLOTS & SIGNALS-----------------------*/
    QObject::connect(ui->purPushButton,&QPushButton::clicked, this, &BE_MainWindow::on_purchasePushButton_clicked);
    QObject::connect(ui->exportAllDataPushButton, &QPushButton::clicked, this, &BE_MainWindow::on_exportAllDataPushButton_clicked);
    QObject::connect(ui->exportTablePushButton, &QPushButton::clicked, this, &BE_MainWindow::on_exportTablePushButton_clicked);
    QObject::connect(ui->importAllDataPushButton, &QPushButton::clicked, this, &BE_MainWindow::on_importAllDataPushButton_clicked);
    QObject::connect(ui->printTablePushButton, &QPushButton::clicked, this, &BE_MainWindow::on_printTablePushButton_clicked);
    QObject::connect(ui->previewTablePushButton, &QPushButton::clicked, this, &BE_MainWindow::on_previewPushButton_clicked);
    if(!QObject::connect(this, SIGNAL(signalUpdateStockTable(matrow *)),this, SLOT(on_signalUpdateStockTable(matrow *))))
      qDebug() << "CHECKED_CONNECT failed";
}

/*
 * BE_MainWindow Destructor
 * Delete the GUI components
 */
BE_MainWindow::~BE_MainWindow()
{
    delete ui;
}

/*
 * BE_MainWindow Signals and Slots
 */
void BE_MainWindow::initializeSignalsSlots()
{
    ui->addPushButton->setEnabled(false);
    ui->delPushButton->setEnabled(false);
    if(isAdmin){
        ui->addPushButton->setEnabled(true);
        ui->delPushButton->setEnabled(true);
        QObject::connect(ui->addPushButton, &QPushButton::clicked, this, &BE_MainWindow::on_addPushButton_clicked);
        QObject::connect(ui->delPushButton,&QPushButton::clicked, this, &BE_MainWindow::on_delPushButton_clicked);
        QObject::connect(ui->savePushButton, &QPushButton::clicked, this, &BE_MainWindow::on_savePushButton_clicked);
        QObject::connect(ui->cancelPushButton,&QPushButton::clicked, this, &BE_MainWindow::on_cancelPushButton_clicked);

        QObject::connect(ui->tablesTabWidget,&QTabWidget::currentChanged,this, &BE_MainWindow::on_tablesTabWidget_tabChanged);
        QObject::connect(ui->cashFlowTableWidget,&QTableWidget::cellChanged, this, &BE_MainWindow::on_cashFlowTableWidget_cellChanged);
        QObject::connect(ui->plTableWidget,&QTableWidget::cellChanged, this, &BE_MainWindow::on_plTableWidget_cellChanged);
        QObject::connect(ui->salesTableWidget,&QTableWidget::cellChanged, this, &BE_MainWindow::on_salesTableWidget_cellChanged);
        QObject::connect(ui->purchaseTableWidget,&QTableWidget::cellChanged, this, &BE_MainWindow::on_purchaseTableWidget_cellChanged);
        QObject::connect(ui->stockTableWidget,&QTableWidget::cellChanged, this, &BE_MainWindow::on_stockTableWidget_cellChanged);
    }
}

/*
 * Returns the StockTable GUI
 */
QTableWidget* BE_MainWindow::getStockTable()
{
    return ui->stockTableWidget;
}

/*
 * Returns the Purchase Table GUI
 */
QTableWidget* BE_MainWindow::getPurchaseTable()
{
    return ui->purchaseTableWidget;
}

/*
 * Returns the Sales Table GUI
 */
QTableWidget* BE_MainWindow::getSalesTable()
{
    return ui->salesTableWidget;
}

/*
 * Returns the Summary Table GUI
 */
QTableWidget* BE_MainWindow::getSummaryTable()
{
    return ui->sumTableWidget;
}

/*
 * Returns the Profit Loss Table GUI
 */
QTableWidget* BE_MainWindow::getPLTable()
{
    return ui->plTableWidget;
}

/*
 * Returns the Cash Flow Table GUI
 */
QTableWidget* BE_MainWindow::getCashFlowTable()
{
    return ui->cashFlowTableWidget;
}



/*
 * Returns the Main window GUI
 */
BE_MainWindow* BE_MainWindow::getBeMainWindow()
{
    fout<<"In BE_MainWindow* BE_MainWindow::getBeMainWindow()"<<endl;
    if(!beMainWindowObj) //check if the singleton object exist or not
    {
           beMainWindowObj = new BE_MainWindow(); //create the object
    }
    return beMainWindowObj; //return the address of the singleton Form Object
}

void BE_MainWindow::diableSaveCancel()
{
    ui->savePushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(false);
}

/*--------------private member functions--------*/
/*
 * Sets the Tool Tip Text with the value
 * of Edited Text data When
 * save button is clicked
 * This saves the changes permanently
 * Table in GUI
 */
void BE_MainWindow::saveTableItems(tableType tbletype, QTableWidget *table)
{
    int rowCount = table->rowCount();
    int colCount = table->columnCount();
    for(int row=0; row< rowCount; row++)
    {
        unsigned int newkey = 0;
        unsigned int oldKey = 0;
        if(!table->item(row, 0))
            continue;
        else
            oldKey = table->item(row, 0)->text().toUInt();
        if(tbletype == TABLE_PURCHASE){
            QString prodName, batchNo, billNo, date;
            if(table->item(row, PUR_PROD))
                 prodName = table->item(row, PUR_PROD)->text().trimmed();
            if(table->item(row, PUR_BATCH))
                batchNo = table->item(row, PUR_BATCH)->text().trimmed();
            if(table->item(row, PUR_BILLNO))
                billNo = table->item(row, PUR_BILLNO)->text().trimmed();
            if(table->item(row, PUR_DATE))
                date = table->item(row, PUR_DATE)->text().trimmed();
            if(prodName.isEmpty() || batchNo.isEmpty() || billNo.isEmpty())
                return;
            newkey = hashCode(prodName + batchNo + billNo + date);
        }
        else if(tbletype == TABLE_STOCK){
            QString prodName, batchNo;
            if(table->item(row, PUR_PROD))
                 prodName = table->item(row, PROD_NAME)->text().trimmed();
            if(table->item(row, PUR_BATCH))
                batchNo = table->item(row, PROD_BATCH)->text().trimmed();
            if(prodName.isEmpty() || batchNo.isEmpty())
                return;
            newkey = hashCode(prodName + batchNo);
        }
        else
            newkey = oldKey = 0;
        if( oldKey != newkey)
        {
            updateItemKeyToBusinessManager(tbletype, oldKey, newkey);
            table->item(row, 0)->setText(QString::number(newkey));
        }
        for (int col = 0; col<= colCount; col++)
        {
            if(table->item(row,col))
                table->item(row,col)->setToolTip(table->item(row,col)->text());
        }
    }
}


/*
 * Sets the Edited Text data with the value
 * of Tool Tip Text When
 * cancel button is clicked
 * This reverts the changes done last
 * time in Table UI
 */
void BE_MainWindow::cancelTableItems(QTableWidget *table)
{
    int rowCount = table->rowCount();
    int colCount = table->colorCount();
    for(int row=0; row < rowCount; row++)
    {
        for(int col=0; col <=colCount;col++){
            if(table->item(row, col))
                table->item(row, col)->setText(table->item(row,col)->toolTip());
        }
    }
}

/*
 * This function populates the data into purchase table on new entry
 *
 */
void BE_MainWindow::populatePurchaseData(BE_PurWindow *item, int result)
{
    QString *tableName = new QString(table_name[TABLE_PURCHASE]);
    int curTab = setCurrentTab(*tableName);
    if(item && result == QDialog::Accepted)
    {
        ui->tablesTabWidget->setCurrentIndex(curTab);
    }
    if(item){
        delete item;
        item='\0';
    }
    delete tableName;
    tableName = '\0';
}
int BE_MainWindow::setCurrentTab(QString & tableName)
{
    int curTabId = -1;
    for(int i=0; i < ui->tablesTabWidget->count(); i++)
    {
        curTabId = i;
        if(tableName == table_name[TABLE_STOCK]){
            curTableId = TABLE_STOCK;
            curTable = ui->stockTableWidget;
            break;
        }
        else if(tableName == table_name[TABLE_PURCHASE]){
            curTableId = TABLE_PURCHASE;
            curTable = ui->purchaseTableWidget;
            break;
        }
        else if(tableName == table_name[TABLE_SALES]){
            curTableId = TABLE_SALES;
            curTable = ui->salesTableWidget;
            break;
        }
        else if(tableName == table_name[TABLE_PL]){
            curTableId = TABLE_PL;
            curTable = ui->plTableWidget;
            break;
        }
        else if(tableName == table_name[TABLE_CASHFLOW]){
            curTableId = TABLE_CASHFLOW;
            curTable = ui->cashFlowTableWidget;
            break;
        }
        else if(tableName == table_name[TABLE_SUMMARY]){
            curTableId = TABLE_SUMMARY;
            curTable = ui->sumTableWidget;
            break;
        }
        else{
            curTable = '\0';
            curTableId = TABLE_NONE;
            break;
        }
    }
    return curTabId;
}

/*------------------SLOTS Definitions-----------------------*/

/*
 * ---------------------------------------------------------------------------------------
 * This SLOTS are called when purchase, sales, order, sales return Buttons
 * are clicked
 *----------------------------------------------------------------------------------------
 */
void BE_MainWindow::on_purchasePushButton_clicked()
{
    BE_PurWindow *purWindow = new BE_PurWindow(this);
    int result = purWindow->exec();
    populatePurchaseData(purWindow, result);
}

/*
 * ---------------------------------------------------------------------------------------
 * This SLOTS are called when Add, Remove, Save, Cancel Buttons
 * are clicked
 *----------------------------------------------------------------------------------------
 */
void BE_MainWindow::on_tablesTabWidget_tabChanged(int index){

    QString tableName = ui->tablesTabWidget->tabText(index);
    int curTabId = setCurrentTab(tableName);
    if(curTabId < 0)
        return;
}

void BE_MainWindow::on_addPushButton_clicked()
{
    if(!isAdmin)
        return;
    QDateTime now = QDateTime::currentDateTimeUtc();
    unsigned int prod = hashCode(QString::number(now.toTime_t()));
    int row = curTable->rowCount();
    curTable->insertRow(row);
    int id = 0;
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(QString::number(prod));
    item->setToolTip(QString::number(prod));
    curTable->setItem(row, id, item);

    if(curTable->item(row,id))
    {
        addItemToBusinessManager(tableType(curTableId),prod);
    }

    ui->savePushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(false);

    if(tableType(curTableId) == TABLE_STOCK)
        isStockTableEdited = false;
    if(tableType(curTableId) == TABLE_PURCHASE)
        isPurchaseTableEdited = false;
    if(tableType(curTableId) == TABLE_SALES)
        isSalesTableEdited = false;
    if(tableType(curTableId) == TABLE_PL)
        isPLTableEdited = false;
    if(tableType(curTableId) == TABLE_CASHFLOW)
        isCashFlowTableEdited = false;
}
void BE_MainWindow::on_delPushButton_clicked()
{
    if(!isAdmin)
        return;
    QMessageBox msgBox;
    msgBox.setText("Confirm");
    msgBox.setInformativeText("Do you want to delete current record?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if(ret == QMessageBox::No)
        return;
    int row =  -1;
    row = curTable->currentRow();
    if(row == -1)
        return;
    unsigned int id = 0;
    //id signifies the first element of the row which is unique id of the table
    id = curTable->item(row,id)->text().toUInt();
    if(id)
        delItemFromBusinessManager(tableType(curTableId), id);
    curTable->removeRow(row);
    delFromDataBase(tableType(curTableId),id);

    ui->savePushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(false);

    if(tableType(curTableId) == TABLE_STOCK)
        isStockTableEdited = false;
    if(tableType(curTableId) == TABLE_PURCHASE)
        isPurchaseTableEdited = false;
    if(tableType(curTableId) == TABLE_SALES)
        isSalesTableEdited = false;
    if(tableType(curTableId) == TABLE_PL)
        isPLTableEdited = false;
    if(tableType(curTableId) == TABLE_CASHFLOW)
        isCashFlowTableEdited = false;

}
void BE_MainWindow::on_savePushButton_clicked()
{
    if(!isAdmin)
        return;
    if(isStockTableEdited == true){
        saveTableItems(TABLE_STOCK, ui->stockTableWidget);
        saveTableRecords(TABLE_STOCK);
        isStockTableEdited = false;
    }
    if(isPurchaseTableEdited == true){
        saveTableItems(TABLE_PURCHASE, ui->purchaseTableWidget);
        saveTableRecords(TABLE_PURCHASE);
        isPurchaseTableEdited = false;
    }
    if(isSalesTableEdited == true){
        saveTableItems(TABLE_SALES, ui->salesTableWidget);
        //saveToBusinessManager(TABLE_SALES);
        isSalesTableEdited = false;
    }
    if(isPLTableEdited == true){
        saveTableItems(TABLE_PL, ui->plTableWidget);
        //saveToBusinessManager(TABLE_PL);
        isPLTableEdited = false;
    }
    if(isCashFlowTableEdited == true){
        saveTableItems(TABLE_CASHFLOW, ui->cashFlowTableWidget);
        //saveToBusinessManager(TABLE_CASHFLOW);
        isCashFlowTableEdited = false;
    }
    saveTableItems(TABLE_SUMMARY, ui->sumTableWidget);
    //saveToBusinessManager(TABLE_SUMMARY);

    ui->savePushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(false);
}

void BE_MainWindow:: on_cancelPushButton_clicked()
{
    if(!isAdmin)
        return;
    if(isStockTableEdited == true){
        cancelTableItems(ui->stockTableWidget);
        isStockTableEdited = false;
    }
    if(isPurchaseTableEdited == true){
        cancelTableItems(ui->purchaseTableWidget);
        isPurchaseTableEdited = false;
    }
    if(isSalesTableEdited == true){
        cancelTableItems(ui->salesTableWidget);
        isSalesTableEdited = false;
    }
    if(isPLTableEdited == true){
        cancelTableItems(ui->plTableWidget);
        isPLTableEdited = false;
    }
    if(isCashFlowTableEdited == true){
        cancelTableItems(ui->cashFlowTableWidget);
        isCashFlowTableEdited = false;
    }
    ui->savePushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(false);
}

/*
 * ---------------------------------------------------------------------------------------
 * This SLOTS are for import, export to CSV/PDF, print, print preview buttons
 *----------------------------------------------------------------------------------------
 */
void BE_MainWindow::on_exportAllDataPushButton_clicked()
{
    int errorCode = saveRecordsInCSV((tableType)curTableId);
    QMessageBox msg;
    msg.setStandardButtons(QMessageBox::Ok);
    if(!errorCode)
        msg.setText("This table data has been exported to CSV!!!");
    else{
        QString text ("Error exporting table data: err[");
        text = text + errorCode + "]";
        msg.setText(text);
    }
    msg.exec();
}


void BE_MainWindow::on_exportTablePushButton_clicked()
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(QString(table_name[curTableId]) + ".pdf");
    QPainter painter(&printer);
    QString tableName(table_name[curTableId]);
    tableName = tableName + " Table:-";
    exportToPdf(&printer, &painter, curTable, tableName);
    QMessageBox msg;
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setText("This table UI data has been exported to PDF!!!");
    msg.exec();
}

void BE_MainWindow::on_importAllDataPushButton_clicked()
{
    QMessageBox msg;
    msg.setStandardButtons(QMessageBox::Ok);
    QString path = QFileDialog::getOpenFileName(0,tr("Select File"),".");
    QFileInfo fileInfo = QFileInfo(path);
    bool found = false;
    if(!isAdmin){
        msg.setText("Please Start as Admin");
        goto End;
    }
    if(path.isEmpty() || path.isNull())
        return;

    if(!fileInfo.exists() || !fileInfo.isReadable())
        msg.setText("File doesn't exist or is not readable!!!");
    else{
        for(int tbl = TABLE_NONE + 1 ; tbl < TABLE_END; tbl++  ){
            QString name(table_name[tbl]);
            name = name + ".csv";
            if(QFileInfo(path).fileName() == name ){
                found = true;
                break;
            }
        }
        if(found){
            path = QFileInfo(path).absoluteDir().absolutePath() + QDir::separator();
            errorType errorCode = importTableData(path.toStdString());

            if(!errorCode)
                msg.setText("This table data has been imported successfully!!!");
            else{
                QString text ("Error importing table data: err[");
                text = text + errorCode + "]";
                msg.setText(text);
            }
        }
        else
            msg.setText("File is not valid!!!");
    }
End:
    msg.exec();
    ui->savePushButton->setEnabled(false);
    ui->cancelPushButton->setEnabled(false);
}

void BE_MainWindow::print(QPrinter *printer)
{
    QPainter painter(printer);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    QString tableName(table_name[curTableId]);
    tableName = tableName + " Table:-";
    exportToPdf(printer, &painter, curTable, tableName);
}

void BE_MainWindow::on_previewPushButton_clicked()
{
    QPrinter printer;
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowFlags ( Qt::Window );
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(print(QPrinter *)));
    preview.exec();
}

void BE_MainWindow::on_printTablePushButton_clicked()
{
    QPrinter printer;
    printer.setResolution(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    QPrintDialog   dialog( &printer, this );
    if ( dialog.exec() == QDialog::Accepted ) this->print( &printer );
}

/*
 * ---------------------------------------------------------------------------------------
 * This SLOTS are called when any cell of a table are edited
 * Every table has different SLOT assigned to it
 *
 *----------------------------------------------------------------------------------------
 */


void BE_MainWindow::on_cashFlowTableWidget_cellChanged(int row, int col)
{
    if(!isAdmin)
        return;
    isCashFlowTableEdited = true;
    int id = 0;
    QTableWidget *table = ui->cashFlowTableWidget;
    if(row >= 0 && col > 0)
    {
        if(table->item(row,col) && table->item(row,id))
            setItemToBusinessManager(TABLE_CASHFLOW, table->item(row,id)->text().toInt(),
                               col, table->item(row,col)->text().toStdString());
    }

    ui->savePushButton->setEnabled(true);
    ui->cancelPushButton->setEnabled(true);
}
void BE_MainWindow::on_salesTableWidget_cellChanged(int row, int col)
{
    if(!isAdmin)
        return;
    isSalesTableEdited = true;
    int id = 0;
    QTableWidget *table = ui->salesTableWidget;
    if(row >= 0 && col > 0)
    {
        if(table->item(row,col) && table->item(row,id))
            setItemToBusinessManager(TABLE_SALES, table->item(row,id)->text().toInt(),
                               col, table->item(row,col)->text().toStdString());
    }
    ui->savePushButton->setEnabled(true);
    ui->cancelPushButton->setEnabled(true);
}
void BE_MainWindow::on_stockTableWidget_cellChanged(int row, int col)
{
    if(!isAdmin)
        return;
    isStockTableEdited = true;
    int id = 0;
    QTableWidget *table = ui->stockTableWidget;
    if(row >= 0 && col > 0)
    {
        if(table->item(row,col) && table->item(row,id))
            setItemToBusinessManager(TABLE_STOCK, table->item(row,id)->text().toUInt(),
                               col, table->item(row,col)->text().toStdString());
    }
    ui->savePushButton->setEnabled(true);
    ui->cancelPushButton->setEnabled(true);
}
void BE_MainWindow::on_plTableWidget_cellChanged(int row, int col)
{
    if(!isAdmin)
        return;
    isPLTableEdited = true;
    int id = 0;
    QTableWidget *table = ui->plTableWidget;
    if(row >= 0 && col > 0)
    {
        if(table->item(row,col) && table->item(row,id))
            setItemToBusinessManager(TABLE_PL, table->item(row,id)->text().toInt(),
                               col, table->item(row,col)->text().toStdString());
    }
    ui->savePushButton->setEnabled(true);
    ui->cancelPushButton->setEnabled(true);
}
void BE_MainWindow::on_purchaseTableWidget_cellChanged(int row, int col)
{
    if(!isAdmin)
        return;
    isPurchaseTableEdited = true;
    int id = 0;
    QTableWidget *table = ui->purchaseTableWidget;
    if(row >= 0 && col > 0)
    {
        if(table->item(row,col) && table->item(row,id))
            setItemToBusinessManager(TABLE_PURCHASE, table->item(row,id)->text().toUInt(),
                               col, table->item(row,col)->text().toStdString());
    }
    ui->savePushButton->setEnabled(true);
    ui->cancelPushButton->setEnabled(true);
}

/*
 * ---------------------------------------------------------------------------------------
 * This SLOTS are for user defined Signals used to update different tables
 *----------------------------------------------------------------------------------------
 */

void BE_MainWindow::on_signalUpdateStockTable(matrow *record)
{
    QTableWidget *stockTable = BE_MainWindow::getBeMainWindow()->getStockTable();
    int rowCount = stockTable->rowCount();
    for(int iter=0; iter < rowCount; iter++)
    {
        unsigned int id = atol(record->at(PROD_ID).c_str());
        if(stockTable->item(iter,PROD_ID) &&
             (id == stockTable->item(iter,PROD_ID)->text().toUInt()))
        {
            for(int col = PROD_ID + 1; col < PROD_END; col++)
            {
                if(!stockTable->item(iter,col)){
                    QTableWidgetItem *item = new QTableWidgetItem();
                    stockTable->setItem(iter, col, item);
                }
                stockTable->item(iter,col)->setText(QString::fromStdString(record->at(col)));
            }
            break;
        }
        else
            continue;

    }
    isStockTableEdited = false;
    BE_MainWindow::getBeMainWindow()->diableSaveCancel();
}
