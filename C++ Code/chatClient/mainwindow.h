#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTabWidget>
#include "chatClient.h"
#include "threading.h"
#include <iostream>
#include <string>

using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    WorkerThread* workerThread;
public:
    explicit MainWindow(QWidget *parent = nullptr, shared_ptr<cs457::chatClient> sock = nullptr);
    ~MainWindow();
//    cs457::tcpClientSocket clientSocket;
    void receiverThread();
    void updateChannels();
    void displayChannels(string s);
    void addTab(string s);

private slots:
    void on_pushButton_2_clicked();
    void handleResults(const QString& msg);

    void on_lineEdit_2_returnPressed();

    void on_tabWidget_tabCloseRequested(int index);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    shared_ptr<cs457::chatClient> clientSocket;
};

#endif // MAINWINDOW_H
