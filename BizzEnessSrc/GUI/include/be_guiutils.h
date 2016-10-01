#ifndef GUIUTILS
#define GUIUTILS

#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTableWidget>
#include <common/include/common.h>

void printHeader(QPainter *, QPrinter *, QString &);
int drawDoubleLines(QPainter *painter, int , int , int , int );
int printLine(QPainter *, QPoint , QString , QString , int );
void exportToPdf(QPrinter *, QPainter *, QTableWidget *, QString );
int getTableAreaWidth(QTableView *, int , int );
int getTableAreaHeight(QTableView *, int , int );
QAbstractItemModel * getTableArea(QTableWidget *,int , int , int , int );

#endif // GUIUTILS

