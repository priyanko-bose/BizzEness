#include <QChar>
#include <QStandardItemModel>
#include <QPainter>
#include <QRect>
#include <QTableView>
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>
#include "GUI/include/be_guiutils.h"

unsigned int  hashCode(QString text){
    unsigned int hash = 0;
    int strlen = text.length(), i;
    QChar character;
    if (strlen == 0)
        return hash;
    for (i = 0; i < strlen; i++) {
        character = text.at(i);
        hash = (31 * hash) + (character.toLatin1());
    }
    return hash;
}

QAbstractItemModel * getTableArea(QTableWidget *curTable,int startRow, int startCol,
                                     int endRow, int endCol){
    QAbstractItemModel *model = new QStandardItemModel(row_per_page, endCol);
    for(int ifrom = startRow, ito = 0; ifrom < endRow ; ito++, ifrom++){
        for(int jfrom=startCol; jfrom < endCol; jfrom++){
            if(ifrom == startRow)
                model->setHeaderData(jfrom, Qt::Horizontal, curTable->horizontalHeaderItem(jfrom)->text());
            if(curTable->model()->index(ifrom, jfrom).isValid())
                model->setData(model->index(ito, jfrom) ,curTable->model()->index(ifrom, jfrom).data().toString());
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
    double yscale = (printAreaH - (topLeftY + tableHeaderH + space))/double(height);
    double scale =   qMin(xscale, yscale);

    painter->translate(qreal(topLeftX), qreal(topLeftY + tableHeaderH + space));
    painter->scale(double(int(scale * 100)/100.0), double(int(scale * 100)/100.0));
}

void exportToPdf(QPrinter *printer, QPainter *painter, QTableWidget *curTable, QString tableName){

    //Declare & Initialize painter object
    //QPainter painter(printer);

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
void print( QPrinter* printer )
{
  // create painter for drawing print page
  QPainter painter( printer );
  int      w = printer->pageRect().width();
  int      h = printer->pageRect().height();
  QRect    page( 0, 0, w, h );

  // create a font appropriate to page size
  QFont    font = painter.font();
  font.setPixelSize( (w+h) / 100 );
  painter.setFont( font );

  // draw labels in corners of page
  painter.drawText( page, Qt::AlignTop    | Qt::AlignLeft, "QSimulate" );
  painter.drawText( page, Qt::AlignBottom | Qt::AlignLeft, QString(getenv("USER")) );
  painter.drawText( page, Qt::AlignBottom | Qt::AlignRight,
                    QDateTime::currentDateTime().toString( Qt::DefaultLocaleShortDate ) );

  // draw simulated landscape
  page.adjust( w/20, h/20, -w/20, -h/20 );
  //m_scene->render( &painter, page );
}
