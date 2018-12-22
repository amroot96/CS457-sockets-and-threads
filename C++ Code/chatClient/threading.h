#ifndef THREADING_H
#define THREADING_H

#include <QThread>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <QObject>
#include <QString>

#include "chatClient.h"

using namespace std;

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(std::shared_ptr<cs457::chatClient> client):client(client){}

public slots:
    void run();

signals:
    void resultReady(const QString& s);

private:
    std::shared_ptr<cs457::chatClient> client;
};

#endif // THREADING_H
