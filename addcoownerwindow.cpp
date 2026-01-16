#include "addcoownerwindow.h"
#include "ui_addcoownerwindow.h"
#include <QMessageBox>

AddCoOwnerWindow::AddCoOwnerWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::AddCoOwnerWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
{
    ui->setupUi(this);

    QSqlQuery yachts(m_db);
    yachts.prepare("SELECT yachts.id_yacht AS 'yId', yachts.name AS 'yName' FROM yacht JOIN yachts_ownership ON yachts.id_yacht = yacht_ownership.yacht_id WHERE owner_id = :currentUserId ORDER BY yachts.name");
    yachts.bindValue(":currentUserId", m_currentUserId);

    if (!yachts.exec()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << yachts.lastError().text(); // Senior Dev Tip: Always log the error text!
    }

    while (yachts.next()) {
        QString yName = yachts.value("yName").toString();
        int yId = yachts.value("yId").toInt();
        ui->YachtComboBox->addItem(yName, yId); // Store ID as "UserRole" data
    }
}

AddCoOwnerWindow::~AddCoOwnerWindow()
{
    delete ui;
}

void AddCoOwnerWindow::on_buttonBox_accepted()
{
    QString user = ui->UsernameText->text();
    if (user.length() == 0) { QMessageBox::information(this, "Failed", "Enter username"); }
    // Check if user exists "SELECT id_user, username FROM users"
    // iterate through records
    // if found match check if it's not allready a Owner or CoOwner
    // if not get id as uId and stop iterating
    // get Yacht Id form combobox as yId
    /* QSqlQuery qOwner(m_db);
    qOwner.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) VALUES (:yId, :oId, :oFlag, :uTime)");
    qOwner.bindValue(":yId", yId);
    qOwner.bindValue(":oId", uId);
    qOwner.bindValue(":oFlag", "CoOwner");
    qOwner.bindValue(":uTime", QDateTime::currentDateTime());

    if (!qOwner.exec()) {
        // Show Error
        QMessageBox::information(this, "Failed", "Failed to add record to database");
        return;
    }

    QMessageBox::information(this, "Success", "Successfuly added Yacht and Ownership");
    return;
    */
}


void AddCoOwnerWindow::on_buttonBox_rejected()
{
    return;
}
