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
    yachts.prepare("SELECT yachts.id_yacht AS 'yId', yachts.name AS 'yName' FROM yachts JOIN yacht_ownership ON yachts.id_yacht = yacht_ownership.yacht_id WHERE owner_id = :currentUserId AND yacht_ownership.ownership_flag IN ('Current', 'CoOwner') ORDER BY yachts.name");
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
    // Pobieramy ID jachtu (zakładam, że jest przechowywane jako UserRole w ComboBox)
    int selectedYacht = ui->YachtComboBox->itemData(ui->YachtComboBox->currentIndex()).toInt();
    int uId = -1;

    if (user.length() == 0) {
        QMessageBox::information(this, "Failed", "Enter username");
        return;
    }

    // 1. Sprawdź czy użytkownik istnieje
    QSqlQuery userCheck(m_db);
    userCheck.prepare("SELECT id_user FROM users WHERE username = :username");
    userCheck.bindValue(":username", user);

    if (!userCheck.exec()) {
        QMessageBox::critical(this, "Database Error", "User Check Query Failed: " + userCheck.lastError().text());
        return;
    }

    if (userCheck.next()) {
        uId = userCheck.value(0).toInt();

        // 2. Sprawdź czy jest już AKTYWNYM właścicielem (Current lub CoOwner)
        QSqlQuery activeOwnerCheck(m_db);
        activeOwnerCheck.prepare("SELECT 1 FROM yacht_ownership WHERE func_is_active_owner(:uId, :yId) = 1");
        activeOwnerCheck.bindValue(":yId", selectedYacht);
        activeOwnerCheck.bindValue(":uId", uId);

        if (!activeOwnerCheck.exec()) {
            QMessageBox::critical(this, "Database Error", "Active Owner Check Failed");
            return;
        }

        if (activeOwnerCheck.next()) {
            // Użytkownik JUŻ JEST właścicielem
            QMessageBox::warning(this, "Error", "User is already an owner or co-owner of this yacht.");
            return;
        }
        else {
            // Użytkownik NIE JEST aktywnym właścicielem.
            // 3. Sprawdź czy był PRZESZŁYM właścicielem (Past)
            QSqlQuery pastOwnerCheck(m_db);
            pastOwnerCheck.prepare("SELECT id_ownership FROM yacht_ownership "
                                   "WHERE yacht_id = :yId AND owner_id = :uId AND ownership_flag = 'Past'");
            pastOwnerCheck.bindValue(":yId", selectedYacht);
            pastOwnerCheck.bindValue(":uId", uId);

            if (!pastOwnerCheck.exec()) {
                QMessageBox::critical(this, "Database Error", "Past Owner Check Failed");
                return;
            }

            if (pastOwnerCheck.next()) {
                // SCENARIUSZ A: Znaleziono rekord 'Past' -> AKTUALIZACJA (Przywrócenie)
                int recordId = pastOwnerCheck.value(0).toInt();

                QSqlQuery updateQuery(m_db);
                updateQuery.prepare("UPDATE yacht_ownership "
                                    "SET ownership_flag = 'CoOwner', update_time = NOW() "
                                    "WHERE id_ownership = :id");
                updateQuery.bindValue(":id", recordId);

                if (!updateQuery.exec()) {
                    QMessageBox::critical(this, "Failed", "Failed to restore past owner: " + updateQuery.lastError().text());
                    return;
                }
                QMessageBox::information(this, "Success", "User restored from Past Owner to CoOwner.");
            }
            else {
                // SCENARIUSZ B: Brak historii -> NOWY WPIS (Insert)
                QSqlQuery insertQuery(m_db);
                insertQuery.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) "
                                    "VALUES (:yId, :oId, 'CoOwner', NOW())");
                insertQuery.bindValue(":yId", selectedYacht);
                insertQuery.bindValue(":oId", uId);

                if (!insertQuery.exec()) {
                    QMessageBox::critical(this, "Failed", "Failed to add record: " + insertQuery.lastError().text());
                    return;
                }
                QMessageBox::information(this, "Success", "Successfully added new CoOwner.");
            }
        }
    } else {
        QMessageBox::warning(this, "Failed", "User does not exist");
    }
}


void AddCoOwnerWindow::on_buttonBox_rejected()
{
    return;
}
