#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <QtCore>
#include <QApplication>

#include "chatClient.h"
#include "mainwindow.h"
#include "threading.h"

using namespace std;

string ip;
uint port;
bool debug;
string lFile;
bool logFile;
QStringList conf;
QString path = "";

void init() {
    path.append(QString("/chatClient/chatclient.conf"));
    cout << path.toStdString() << endl;
    QFile file(path);

    if(file.open(QIODevice::ReadOnly));
    QTextStream in(&file);

    while(!in.atEnd()) {
        QString str = in.readLine();
        QStringList fin = str.split(QRegExp("\\s+"));
        conf << fin;
    }

    for(int i = 0; i < conf.size(); i++) {
        if(conf.at(i) == "last_server_used"){
            ip = (conf.at(i+1)).toStdString();
        }
        else if(conf.at(i) == "port")
            port = (conf.at(i+1)).toUInt();
        else if(conf.at(i) == "default_debug_mode"){
            if(conf.at(i+1) == "False") { debug = false; }
            else { debug = true; }
        }
        else if(conf.at(i) == "log"){
            if(conf.at(i+1) == "False") { logFile = false; }
            else { logFile = true; }
        }
        else if(conf.at(i) == "default_log_file")
            lFile = (conf.at(i+1)).toStdString();
    }
}

int main(int argc, char * argv[])
{
    QFileInfo info(argv[0]);
    path = QDir::currentPath();
    QApplication q(argc, argv);
    init();
    shared_ptr<cs457::chatClient> sock = make_shared<cs457::chatClient>(ip, port);
    sock->connectToServer();
    ssize_t val = -1;
    string msg;

    MainWindow win(nullptr, sock);
    win.receiverThread();
    win.show();

    return q.exec();
}
