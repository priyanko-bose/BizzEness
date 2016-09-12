/*-----------------------------------------------------------------
 *  main.cpp: Basic Architecture
 *         main
 *          ||
 *  GUI<->interface<->BusinessManager<->Database
 *-----------------------------------------------------------------
 */


#include <fstream>
#include "common/include/common.h"
#include "Interface/include/ifHandleDB.h"
#include "Interface/include/ifHandleBM.h"
#include "Interface/include/ifHandleGUI.h"

ofstream fout;
int isAdmin = 0;
//Application Entry Point
int main(int argc, char *argv[])
{
    int errorCode = ERR_NONE;
    if((errorCode = openLogFile()) != ERR_NONE)
    {
        return errorCode; //open the Output file
    }
    if((errorCode = initializeBManager()) == ERR_NONE) //Initialize the business manager working in background
    {
        if ((errorCode = initializeDBManager()) == ERR_NONE)//Read the saved data and populate in GUI tables
        {
            if((errorCode = initializeGUI(argc,argv)) == ERR_NONE)//Initialize the GUI objects and run the application
            {
                deinitializeGUI();//De-Init the GUI objects
            }
            else{
                fout << "Problem initializing GUI:error("<<errorCode <<")"<<endl;
            }
            /*if(isAdmin)
                errorCode |= saveRecordsInDB();*/
            deinitializeDBManager();
        }
        else{
            fout << "Problem initializing Database Manger:error("<<errorCode <<")"<<endl;
        }
        deinitializeBManager();// De-Int the background business manager
    }
    else{
        fout << "Problem initializing Business Manager:error("<<errorCode <<")"<<endl;
    }

    fout << "Exiting with:error code("<<errorCode <<")"<<endl;

    closeFile(); //Close the log file
    return errorCode;
}

