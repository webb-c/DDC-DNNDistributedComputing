/*
 * Communicator.h
 *
 *  Created on: 2023. 12. 22.
 *      Author: Suhwan
 */

#ifndef __DDC_COMMUNICATOR_H_
#define __DDC_COMMUNICATOR_H_
#include <iostream>
#include <omnetpp.h>
#include <windows.h>

using namespace std;

#define BUF_SIZE 200000

namespace inet {
class Communicator {

private:
    TCHAR  chBuf[BUF_SIZE];
    BOOL   fSuccess = false;
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    LPTSTR lpszPipename; // TODO decide pipe name
    HANDLE hPipe;

public:
    Communicator();
    Communicator(string pipe_name);
    ~Communicator();

    DWORD sendPythonMessage(string msg);
    string getPythonMessage();
};

}


#endif /* __DDC_COMMUNICATOR_H_ */
