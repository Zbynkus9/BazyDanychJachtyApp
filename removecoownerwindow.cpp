#include "removecoownerwindow.h"
#include "ui_removecoownerwindow.h"
#include <QMessageBox>

RemoveCoOwnerWindow::RemoveCoOwnerWindow(QWidget *parent, int userId, int selectedYachtId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::RemoveCoOwnerWindow)
    , m_currentUserId(userId)
    , m_db(db)
    , m_selectedYachtId(selectedYachtId)
{
    ui->setupUi(this);

    QSqlQuery query(m_db);
    query.prepare("SELECT users.id_user, users.username FROM users JOIN yacht_ownership ON users.id_user = yacht_ownership.owner_id WHERE yacht_ownership.yacht_id = :yId AND yacht_ownership.ownership_flag = 'CoOwner'");

    query.bindValue(":yId", m_selectedYachtId); // Przekazujemy ID

    if (!query.exec()) {
        qCritical() << "SQL Error:" << query.lastError().text();
    }

    while(query.next()) {
        int uId = query.value("id_user").toInt();
        QString uName = query.value("username").toString();

        if (uId != m_currentUserId) {
            ui->UserComboBox->addItem(uName, uId);
        }
    }



}

RemoveCoOwnerWindow::~RemoveCoOwnerWindow()
{
    delete ui;
}

void RemoveCoOwnerWindow::on_buttonBox_accepted()
{
    // Aktualizujemy user na PastOwner z CoOwner
    int selectedUser = ui->UserComboBox->itemData(ui->UserComboBox->currentIndex()).toInt();
    QSqlQuery qOwner(m_db);
    qOwner.prepare("UPDATE yacht_ownership SET ownership_flag = 'Past', update_time = NOW() WHERE yacht_id = :yId AND owner_id = :oId;");
    qOwner.bindValue(":yId", m_selectedYachtId);
    qOwner.bindValue(":oId", selectedUser);

    if (!qOwner.exec()) {
        qCritical() << "SQL Error:" << qOwner.lastError().text();
    }
    else {
        QMessageBox::information(this, "Success", "Selected user ownership removed");
        this->accept();
    }

    return;
}


void RemoveCoOwnerWindow::on_buttonBox_rejected()
{
    return;
}

