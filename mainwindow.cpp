#include "mainwindow.h"
#include "tripaddwindow.h"
#include "tripviewwindow.h"
#include "ui_mainwindow.h"
#include "yachtaddwindow.h"
#include "yachtownershipwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent, int userId, QSqlDatabase db, QString uName)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentUserId(userId)
    , m_db(db)
    , m_uName(uName)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    qDebug() << QSqlDatabase::drivers();
    qDebug() << QDateTime::currentDateTime();

    ui->WindowLabel->setText("Welcome: " + m_uName);
    qDebug() << m_currentUserId;

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_YachtsBTN_clicked()
{
    YachtAddWindow dialog(this, m_currentUserId, m_db);

    int result = dialog.exec();

    // Sprawdzamy, czy transakcja się udała
    if (result == QDialog::Accepted) {
        qDebug() << "Yacht added!";
    }
}


void MainWindow::on_YachtsOwnershipBTN_clicked()
{
    YachtOwnershipWindow dialog(this, m_currentUserId, m_db);

   dialog.exec();
}


void MainWindow::on_TripAddBTN_clicked()
{
    TripAddWindow dialog(this, m_currentUserId, m_db);

    int result = dialog.exec();

    // Sprawdzamy, czy transakcja się udała
    if (result == QDialog::Accepted) {
        qDebug() << "Trip added!";
    }
}


void MainWindow::on_TripListOwnBTN_clicked()
{
    TripViewWindow dialog(this, m_currentUserId, m_db, 0);

    dialog.exec();
}


void MainWindow::on_TripListSharedBTN_clicked()
{
    TripViewWindow dialog(this, m_currentUserId, m_db, 1);

    dialog.exec();
}


void MainWindow::on_TripListPublicBTN_clicked()
{
    TripViewWindow dialog(this, m_currentUserId, m_db, 2);

    dialog.exec();
}
