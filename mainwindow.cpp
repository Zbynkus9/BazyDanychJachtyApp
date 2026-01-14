#include "mainwindow.h"
#include "tripaddwindow.h"
#include "tripviewwindow.h"
#include "ui_mainwindow.h"
#include "yachtaddwindow.h"
#include "yachtownershipwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    qDebug() << QSqlDatabase::drivers();
    qDebug() << QDateTime::currentDateTime();

    // UI is now ready.
    // Example: ui->labelUser->setText("User ID: " + QString::number(m_currentUserId));
    ui->WindowLabel->setText("User ID: " + QString::number(m_currentUserId));
    qDebug() << m_currentUserId;

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_YachtsBTN_clicked()
{
    // 1. Create the instance (Stack allocation)
    // Pass 'this' as parent so it centers over the Main Window
    YachtAddWindow dialog(this, m_currentUserId, m_db);

    // 2. Launch it effectively "freezing" the code here until the dialog closes
    int result = dialog.exec();

    // 3. (Optional) Check what happened
    if (result == QDialog::Accepted) {
        // The user clicked "Save" and the transaction worked.
        // Good place to refresh your TableView (which we will build next)!
        qDebug() << "Yacht added!";
    }
}


void MainWindow::on_YachtsOwnershipBTN_clicked()
{
    // 1. Create the instance (Stack allocation)
    // Pass 'this' as parent so it centers over the Main Window
    YachtOwnershipWindow dialog(this, m_currentUserId, m_db);

    // 2. Launch it effectively "freezing" the code here until the dialog closes
   dialog.exec();
}


void MainWindow::on_TripAddBTN_clicked()
{
    // 1. Create the instance (Stack allocation)
    // Pass 'this' as parent so it centers over the Main Window
    TripAddWindow dialog(this, m_currentUserId, m_db);

    // 2. Launch it effectively "freezing" the code here until the dialog closes
    int result = dialog.exec();

    // 3. (Optional) Check what happened
    if (result == QDialog::Accepted) {
        // The user clicked "Save" and the transaction worked.
        // Good place to refresh your TableView (which we will build next)!
        qDebug() << "Trip added!";
    }
}


void MainWindow::on_TripListOwnBTN_clicked()
{
    // 1. Create the instance (Stack allocation)
    // Pass 'this' as parent so it centers over the Main Window
    TripViewWindow dialog(this, m_currentUserId, m_db, 0);

    // 2. Launch it effectively "freezing" the code here until the dialog closes
    dialog.exec();
}


void MainWindow::on_TripListSharedBTN_clicked()
{
    // 1. Create the instance (Stack allocation)
    // Pass 'this' as parent so it centers over the Main Window
    TripViewWindow dialog(this, m_currentUserId, m_db, 1);

    // 2. Launch it effectively "freezing" the code here until the dialog closes
    dialog.exec();
}


void MainWindow::on_TripListPublicBTN_clicked()
{
    // 1. Create the instance (Stack allocation)
    // Pass 'this' as parent so it centers over the Main Window
    TripViewWindow dialog(this, m_currentUserId, m_db, 2);

    // 2. Launch it effectively "freezing" the code here until the dialog closes
    dialog.exec();
}

