#include "changeownerwindow.h"
#include "ui_changeownerwindow.h"
#include <QMessageBox>

ChangeOwnerWindow::ChangeOwnerWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::ChangeOwnerWindow)
    , m_currentUserId(userId)
    , m_db(db)
{
    ui->setupUi(this);

    QSqlQuery yachts(m_db);
    yachts.prepare("SELECT yachts.id_yacht AS 'yId', yachts.name AS 'yName' FROM yachts JOIN yacht_ownership ON yachts.id_yacht = yacht_ownership.yacht_id WHERE owner_id = :currentUserId AND yacht_ownership.ownership_flag = 'Current';");
    yachts.bindValue(":currentUserId", m_currentUserId);

    if (!yachts.exec()) {
        qCritical() << "SQL Error:" << yachts.lastError().text();
    }

    while (yachts.next()) {
        QString yName = yachts.value("yName").toString();
        int yId = yachts.value("yId").toInt();
        ui->YachtComboBox->addItem(yName, yId);
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
        // Sprawdzamy, czy użytkownik istnieje
        QSqlQuery userCheck (m_db);

        userCheck.prepare("SELECT id_user, username FROM users WHERE username = :username");
        userCheck.bindValue(":username", user);

        if(!userCheck.exec()) {
            QMessageBox::information(this, "Failed", "Username Querry Failed");
            return;
        }
        else {
            if (userCheck.next()) {
                uId = userCheck.value(0).toInt();

                    QSqlQuery query(m_db);
                    query.prepare("CALL proc_transfer_ownership(:yId, :oId)");
                    query.bindValue(":yId", selectedYacht);
                    query.bindValue(":oId", uId);

                    if (!query.exec()) {
                        QMessageBox::information(this, "Failed", "Failed to transfer ownership");
                        return;
                    }

                    QMessageBox::information(this, "Success", "Successfuly transfered Ownership");
                    return;
            }
            else {
                QMessageBox::information(this, "Failed", "User don't exitst");
                return;
            }
        }
    }

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

