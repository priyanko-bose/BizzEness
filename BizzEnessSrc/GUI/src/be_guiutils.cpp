#include <QChar>
#include <QStandardItemModel>
#include <QPainter>
#include <QRect>
#include <QTableView>
#include <QHeaderView>
#include <QModelIndex>
#include <QComboBox>
#include <QDateTime>
#include <QLineEdit>
#include <QDebug>
#include "GUI/include/be_guiutils.h"

QAbstractItemModel * getTableArea(QTableWidget *curTable,int startRow, int startCol,
                                     int endRow, int endCol){
    QAbstractItemModel *model = new QStandardItemModel(row_per_page, endCol);
    for(int ifrom = startRow, ito = 0; ifrom < endRow ; ito++, ifrom++){
        for(int jfrom=startCol; jfrom < endCol; jfrom++){
            if(ifrom == startRow)
                model->setHeaderData(jfrom, Qt::Horizontal, curTable->horizontalHeaderItem(jfrom)->text());
            if(curTable->model()->index(ifrom, jfrom).isValid())
            {
                if(curTable->cellWidget(ifrom,jfrom) &&
                        strcmp(curTable->cellWidget(ifrom,jfrom)->metaObject()->className(), "QComboBox") == 0)
                {
                    QComboBox *box = qobject_cast<QComboBox *>(curTable->cellWidget(ifrom,jfrom));
                    model->setData(model->index(ito, jfrom) , box->lineEdit()->text());
                }
                else
                    model->setData(model->index(ito, jfrom) ,curTable->model()->index(ifrom, jfrom).data().toString());
            }
        }
    }
    return model;
}

void printHeader(QPainter *painter, QPrinter *printer, QString &tableName){
    int printAreaW = printer->pageRect().width();
    painter->save();
    QFont font( "Helvetica" );
    font.setPointSize( 24 );
    painter->setFont( font );
    QRect trect = QRect(QPoint(topLeftX,topLeftY),QPoint(printAreaW,tableHeaderH));
    painter->drawText(trect.bottomLeft(),tableName);
    painter->restore();
}

int drawDoubleLines(QPainter *painter, int topLX, int topLY, int btmRX, int btmRY)
{
    QPen pen( Qt::black );
    pen.setStyle( Qt::DashLine );
    painter->save();
    painter->setPen(pen);
    painter->drawLine(QPoint(topLX,topLY), QPoint(btmRX, btmRY));
    topLY = topLY + linespace;
    btmRY = btmRY + linespace;
    painter->drawLine(QPoint(topLX,topLY), QPoint(btmRX, btmRY));
    painter->restore();
    return topLY;
}

int printLine(QPainter *painter, QPoint point,
                    QString text, QString fontName, int fontSize)
{
    painter->save();
    QFont font(fontName);
    font.setPointSize(fontSize);
    painter->setFont( font );
    painter->drawText(point,text);
    painter->restore();
    int printAreaHOffset = point.y() + font.pixelSize() + linespace;
    return printAreaHOffset;
}

int getTableAreaWidth(QTableView *pTableView, int colCount, int vHeaderWidth){
    int width = 0;
    for( int i = 0; i < colCount; ++i ) {
        width += pTableView->columnWidth(i);
    }
    width += vHeaderWidth;
    return width;
}

int getTableAreaHeight(QTableView *pTableView, int rowCount, int hHeaderHeight){
    int height = 0;
    for( int i = 0; i < rowCount; ++i ) {
        height += pTableView->rowHeight(i);
    }
    height += hHeaderHeight;
    return height;
}

void adjustPainterScale(QPainter *painter, QPrinter *printer, int width, int height)
{
    int printAreaW = printer->pageRect().width();
    int printAreaH = printer->pageRect().height();
    double xscale = (printAreaW - (topLeftX*2))/double(width);
    double yscale = (printAreaH - (topLeftY + tableHeaderH + linespace))/double(height);
    double scale =   qMin(xscale, yscale);

    painter->translate(qreal(topLeftX), qreal(topLeftY + tableHeaderH + linespace));
    painter->scale(double(int(scale * 100)/100.0), double(int(scale * 100)/100.0));
}

void exportToPdf(QPrinter *printer, QPainter *painter, QTableWidget *curTable, QString tableName){

    //Initialize variables to select the portion of Table Widget
    int count = 0;
    int columns = curTable->columnCount();
    int rows = curTable->rowCount();
    int left = rows;
    int start = 0;
    int end = 0;

    if(rows <= 0 )
        printHeader(painter, printer, tableName);
    rows = row_per_page;
    while(left > 0 ){
        printHeader(painter, printer, tableName);

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
        model = getTableArea(curTable, start, 0, end, columns);
        if(!model)
            return;
        QTableView pTableView;
        pTableView.setModel(model);
        pTableView.resizeColumnsToContents();
        int width = getTableAreaWidth(&pTableView, columns, curTable->verticalHeader()->width());
        int height = getTableAreaHeight(&pTableView, rows, curTable->horizontalHeader()->height());
        pTableView.setFixedSize(width, height);
        pTableView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pTableView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //Adjust painter object to accomodate full printing page
        painter->save();
        adjustPainterScale(painter, printer, width, height);
        pTableView.render(painter);
        if(left > 0)
            printer->newPage();
        delete model;
        model = '\0';
        painter->restore();
    }

}
