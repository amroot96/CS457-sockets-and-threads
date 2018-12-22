#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent, shared_ptr<cs457::chatClient> sock) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    clientSocket(sock)
{
    ui->setupUi(this);
    ui->plainTextEdit_2->moveCursor (QTextCursor::End);
    ui->tabWidget->clear();
    ui->tabWidget->addTab(new QWidget(), "general");

}

MainWindow::~MainWindow()
{
    string msgExit("/QUIT");
    clientSocket->sendMessage(msgExit,false);
    workerThread->quit();
    workerThread->wait();
    clientSocket->closeSocket();
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    string sendmsg = ui->lineEdit_2->text().toStdString();
    ui->lineEdit_2->clear();
    ui->plainTextEdit_2->moveCursor (QTextCursor::End);
    string displaymsg = "user: " + sendmsg + "\n";
    ui->plainTextEdit_2->insertPlainText (QString::fromStdString(displaymsg));
    if(sendmsg.length() >= 6 && sendmsg.substr(0, 5).compare("/JOIN") == 0) {
        addTab(sendmsg);
    }
    clientSocket->sendMessage(sendmsg,false);
    clientSocket->sendMessage("getChannels", false);
}

void MainWindow::handleResults(const QString& msg) {
    string temp = msg.toStdString();
    if(temp.length() >= 26 && (temp.substr(0, 26).compare("[SERVER REPLY] getChannels") == 0)) {
        displayChannels(msg.toStdString());
    }else {
        ui->plainTextEdit_2->insertPlainText(msg + "\n");
        ui->plainTextEdit_2->moveCursor (QTextCursor::End);

    }
}

void MainWindow::addTab(string s) {
    vector<string> channels;
    std::stringstream st(s);
    string each;
    while(getline(st, each, ' ')) {
        channels.push_back(each);
    }
    vector<string>::iterator iter = channels.begin();
    ++iter;
    while(iter != channels.end()) {
        ui->tabWidget->addTab(new QWidget(), QString::fromStdString(*iter));
        ++iter;
    }
}

void MainWindow::displayChannels(string s) {
    ui->listWidget->clear();
    vector<string> channels;
    std::stringstream st(s);
    string each;
    while(getline(st, each, ' ')) {
        channels.push_back(each);
    }

    vector<string>::iterator iter = channels.begin();
    iter++;
    iter++;
    iter++;
    if(iter == channels.end()) {
        ui->listWidget->addItem("No channels active.");
    }
    else {
        while(iter != channels.end()) {
            ui->listWidget->addItem(QString::fromStdString(*iter));
            ++iter;
        }
    }
}

void MainWindow::receiverThread() {
    workerThread = new WorkerThread(this->clientSocket);
    QObject::connect(workerThread, SIGNAL(resultReady(const QString&)), this, SLOT(handleResults(const QString&)));
    QObject::connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    workerThread->start();
}

void MainWindow::on_lineEdit_2_returnPressed()
{
    on_pushButton_2_clicked();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QString s = ui->tabWidget->tabText(index);
    clientSocket->sendMessage("/PART " + s.toStdString(), false);
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    string msg = "/JOIN " + item->text().toStdString();
    clientSocket->sendMessage(msg ,false);
    ui->tabWidget->addTab(new QWidget(), item->text());
}
