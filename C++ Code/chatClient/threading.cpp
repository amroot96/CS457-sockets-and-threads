#include "threading.h"

void WorkerThread::run() {
    string msg = "";
    int val = -1;
    while(val != 0) {
        tie(msg, val) = client->recvString(4096, false);

        emit resultReady(QString::fromStdString(msg));
    }
}
