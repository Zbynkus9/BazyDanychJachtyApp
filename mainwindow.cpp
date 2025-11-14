#include "mainwindow.h"
#include "login.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << QSqlDatabase::drivers();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ConnectBTN_clicked()
{
    if (dbManager.connect()) {
        QMessageBox::information(this, "Connection", "Connected to MySQL successfully!");
        this->hide();
        LoginScreen lgs;
        lgs.setModal(true);
        lgs.exec();

    } else {
        QMessageBox::critical(this, "Connection", "Failed to connect to MySQL. Check logs.");
    }
}

