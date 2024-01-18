/*
 * Communicator.cpp
 *
 *  Created on: 2023. 12. 22.
 *      Author: Suhwan
 */

#include "ddc/communicator/Communicator.h"

namespace inet{

    Communicator::Communicator(){}

    Communicator::Communicator(std::string pipe_name) {
        // TODO Auto-generated constructor stub
        this->lpszPipename = TEXT((char*)pipe_name.c_str()); // TODO decide pipe name

        this->hPipe = CreateFile(
            lpszPipename,   // pipe name
            GENERIC_READ |  // read and write access
            GENERIC_WRITE,
            0,              // no sharing
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe
            0,              // default attributes
            NULL
        );
    }

    Communicator::~Communicator() {
        // TODO Auto-generated destructor stub
    }

    DWORD Communicator::sendPythonMessage(std::string msg){
        if (INVALID_HANDLE_VALUE == this->hPipe) {
            throw runtime_error(string("Python not connected"));
        }
        DWORD cbWritten = 0;
        LPTSTR szMsg = (LPTSTR)msg.c_str();
        DWORD size = msg.size();

        bool fSuccess = WriteFile(
            this->hPipe,                  // pipe handle
            szMsg,                  // message
            size,                   // message length
            &cbWritten,             // bytes written
            NULL);

        return cbWritten;
    }

    std::string Communicator::getPythonMessage(){
        if (INVALID_HANDLE_VALUE == this->hPipe) {
            throw runtime_error(string("Python not connected"));
        }
        TCHAR pchRequest[BUF_SIZE];
        DWORD cbBytesRead = 0;

        bool fSuccess = ReadFile(
            this->hPipe,        // handle to pipe
            pchRequest,    // buffer to receive data
            BUF_SIZE*sizeof(TCHAR), // size of buffer
            &cbBytesRead, // number of bytes read
            NULL);        // not overlapped I/O

        std::string response((char *)pchRequest, cbBytesRead);

        return response;
    }
}


