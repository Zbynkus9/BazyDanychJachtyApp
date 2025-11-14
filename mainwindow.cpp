#include "mainwindow.h"
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

void MainWindow::on_pushButton_clicked()
{
    if (dbManager.connect()) {
        QMessageBox::information(this, "Connection", "Connected to MySQL successfully!");
    } else {
        QMessageBox::critical(this, "Connection", "Failed to connect to MySQL. Check logs.");
    }
}

