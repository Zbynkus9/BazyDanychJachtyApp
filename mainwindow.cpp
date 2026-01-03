#include "mainwindow.h"
#include "ui_mainwindow.h"
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

    // UI is now ready.
    // Example: ui->labelUser->setText("User ID: " + QString::number(m_currentUserId));
    ui->WindowLabel->setText("User ID: " + QString::number(m_currentUserId));
    qDebug() << m_currentUserId;

}

MainWindow::~MainWindow()
{
    delete ui;
}


