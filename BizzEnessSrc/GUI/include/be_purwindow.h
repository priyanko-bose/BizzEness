#ifndef BE_PURWINDOW_H
#define BE_PURWINDOW_H

#include <QDialog>
#include <QMap>
#include <QPrinter>
#include <QTableWidgetItem>
#include "common/include/common.h"

namespace Ui {
class Dialog;
}

class BE_PurWindow : public QDialog
{
    Q_OBJECT

public:
    BE_PurWindow(QWidget *parent);
    ~BE_PurWindow();
    Ui::Dialog *memoUi;

signals:
    //void signalUpdateTotalCost();

private:
    QMap <QString, purui_table_flds> columnNameValueMap;
    int getColID(purui_table_flds);
    void initializeSignalsSlots();
    void initColumnNameValueMap();
    purui_table_flds  getColumnNameValue(QString colName);
    void on_purAddPushButton_clicked();
    void on_purDelPushButton_clicked();
    void on_productListButtonBox_accepted();
    void on_productListButtonBox_rejected();
    void on_purMemoUIButtonBox_accepted();
    void on_purMemoUIButtonBox_rejected();
    void on_printPushButton_clicked();
    void on_purTableWidget_cellChanged(int, int);


public slots:
    void print(QPrinter *printer);
    void on_currentIndexChanged_clicked(int);
    void on_totalItemsTextEdited_clicked();
};

class BE_QTableWidgetItem : public QObject, public QTableWidgetItem
{
    Q_OBJECT
public slots:
    void on_taxComboBox_TextChanged(const QString &);
};

#endif // BE_PURWINDOW_H

