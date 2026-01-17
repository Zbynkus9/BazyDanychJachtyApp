#include "changeownerwindow.h"
#include "ui_changeownerwindow.h"
#include <QMessageBox>

ChangeOwnerWindow::ChangeOwnerWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::ChangeOwnerWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
{
    ui->setupUi(this);

    QSqlQuery yachts(m_db);
    yachts.prepare("SELECT yachts.id_yacht AS 'yId', yachts.name AS 'yName' FROM yachts JOIN yacht_ownership ON yachts.id_yacht = yacht_ownership.yacht_id WHERE owner_id = :currentUserId AND yacht_ownership.ownership_flag = 'Current' AND NOT EXISTS ( SELECT 1 FROM yacht_ownership history WHERE history.owner_id = users.id_user AND history.yacht_id = :yId AND history.ownership_flag = 'Past') ORDER BY yachts.name");
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

ChangeOwnerWindow::~ChangeOwnerWindow()
{
    delete ui;
}

void ChangeOwnerWindow::on_buttonBox_accepted()
{
    QString user = ui->UsernameText->text();
    int selectedYacht = ui->YachtComboBox->itemData(ui->YachtComboBox->currentIndex()).toInt();
    int uId = -1;
    if (user.length() == 0) { QMessageBox::information(this, "Failed", "Enter username"); }
    else {
        // Check if user exists "SELECT id_user, username FROM users"
        QSqlQuery userCheck (m_db);

        userCheck.prepare("SELECT id_user, username FROM users WHERE username = :username");
        userCheck.bindValue(":username", user);

        if(!userCheck.exec()) {
            QMessageBox::information(this, "Failed", "Username Querry Failed");
            return;
        }
        else {
            if (userCheck.next()) {
                // if found match check if it's not allready a Owner or CoOwner
                uId = userCheck.value(0).toInt();

                QSqlQuery ownershipCheck (m_db);

                ownershipCheck.prepare("SELECT 1 FROM yacht_ownership WHERE yacht_id = :yId AND owner_id = :uId AND ownership_flag IN ('Current', 'CoOwner')");
                ownershipCheck.bindValue(":yId", selectedYacht);
                ownershipCheck.bindValue(":uId", uId);

                if(!ownershipCheck.exec()) {
                    QMessageBox::information(this, "Failed", "Username Querry Failed");
                    return;
                }
                else {
                    if (ownershipCheck.next()) {
                        QMessageBox::warning(this, "Error", "User is already an owner or co-owner of this yacht.");
                        return; // Stop execution
                    }
                    else {
                        // ----------------------------- create transaction -----------------------------------
                        m_db.transaction();
                        // 1. change all current CoOwners and Owner to Past
                        QSqlQuery archiveQuery(m_db);
                        archiveQuery.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) SELECT yacht_id, owner_id, 'Past', NOW() FROM yacht_ownership WHERE yacht_id = :yId AND ownership_flag IN ('Current', 'CoOwner')");

                        archiveQuery.bindValue(":yId", selectedYacht);

                        if (!archiveQuery.exec()) {
                            m_db.rollback(); // If you are inside a transaction
                            qCritical() << "Failed to archive owners:" << archiveQuery.lastError().text();
                            return;
                        }
                        else {
                            QSqlQuery qOwner(m_db);
                            qOwner.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) VALUES (:yId, :oId, :oFlag, NOW())");
                            qOwner.bindValue(":yId", selectedYacht);
                            qOwner.bindValue(":oId", uId);
                            qOwner.bindValue(":oFlag", "Current");

                            if (!qOwner.exec()) {
                                // Show Error
                                m_db.rollback(); // If you are inside a transaction
                                QMessageBox::information(this, "Failed", "Failed to transfer ownership");
                                return;
                            }

                            m_db.commit();
                            QMessageBox::information(this, "Success", "Successfuly transfered Ownership");
                            return;
                        }
                    }
                }

                return;
            }
            else {
                QMessageBox::information(this, "Failed", "User don't exitst");
                return;
            }
        }
    }

    //
    // 2. make selected user new Owner
    /* QSqlQuery qOwner(m_db);
    qOwner.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) VALUES (:yId, :oId, :oFlag, :uTime)");
    qOwner.bindValue(":yId", yId);
    qOwner.bindValue(":oId", uId);
    qOwner.bindValue(":oFlag", "Current");
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


void ChangeOwnerWindow::on_buttonBox_rejected()
{
    return;
}

