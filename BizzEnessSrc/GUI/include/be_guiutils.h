#ifndef GUIUTILS
#define GUIUTILS

#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTableWidget>
#include <common/include/common.h>

unsigned int  hashCode(QString text);
void exportToPdf(QPrinter *, QPainter *, QTableWidget *, QString );
#endif // GUIUTILS

