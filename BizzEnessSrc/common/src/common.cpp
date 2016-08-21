#include <QApplication>
#include <QFile>
#include <fstream>
#include <QDebug>
#include <QMessageBox>
#include <map>
#include <list>
#include "GUI/include/be_mainwindow.h"
#include "BusinessManager/include/beBusinessManager.h"
#include "Interface/include/ifHandleDB.h"
#include "common/include/common.h"

extern ofstream fout;
using namespace std;

/*
 * This function is used to open the log file
 */
errorType openLogFile()
{
    fout.open("bizeness.log");
    if(fout.is_open()) //open the Output file
    {
        fout << "ifCommon.cpp:openLogFile:Openend the log file " << endl;
        return ERR_NONE;
    }
    else{
        fout << "ifCommon.cpp:openLogFile:Couldn't Open the log file " << endl;
    }
    return ERR_LOG_OPEN;
}

/*
 * This function is used to close the log file
 */
void closeFile()
{
    fout << "ifCommon.cpp:closeFile:Closing the log file " << endl;
    if(fout.is_open())
        fout.close();
}


