#ifndef BE_MAINWINDOW_H
#define BE_MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDialogButtonBox>
#include <QPrinter>
#include "GUI/include/be_purwindow.h"

namespace Ui {
class MainWindow;
}

class BE_MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    ~BE_MainWindow();
    static BE_MainWindow * getBeMainWindow();
    int startProdId;
    int startSalesId;
    int startPLId;
    int startSumId;
    int startCashFlowId;

    bool isStockTableEdited;
    bool isPurchaseTableEdited;
    bool isSalesTableEdited;
    bool isPLTableEdited;
    bool isCashFlowTableEdited;

    QTableWidget* getStockTable();
    QTableWidget* getPurchaseTable();
    QTableWidget* getSalesTable();
    QTableWidget* getCashFlowTable();
    QTableWidget* getPLTable();
    QTableWidget* getSummaryTable();

    void initializeSignalsSlots();
    void populatePurchaseData(BE_PurWindow *, int);
    int setCurrentTab(QString & );

private slots:


private:
    BE_MainWindow(QWidget *parent = 0);
    static BE_MainWindow *beMainWindowObj;
    Ui::MainWindow *ui;
    QTableWidget *curTable;
    int curTableId;
    void saveTableItems(QTableWidget *);
    void cancelTableItems(QTableWidget *);
    void print(QPrinter *printer);

    void on_addPushButton_clicked();
    void on_delPushButton_clicked();
    void on_savePushButton_clicked();
    void on_cancelPushButton_clicked();
    void on_exportAllDataPushButton_clicked();
    void on_cashFlowTableWidget_cellChanged(int, int);
    void on_salesTableWidget_cellChanged(int , int );
    void on_stockTableWidget_cellChanged(int, int);
    void on_plTableWidget_cellChanged(int , int );
    void on_purchaseTableWidget_cellChanged(int , int );
    void on_tablesTabWidget_tabChanged(int);
    void on_purchasePushButton_clicked();
    void on_exportTablePushButton_clicked();
    void on_importAllDataPushButton_clicked();
    void on_printTablePushButton_clicked();
    void on_previewPushButton_clicked();
};

#endif // BE_MAINWINDOW_H
