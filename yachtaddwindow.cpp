#include "yachtaddwindow.h"
#include "ui_yachtaddwindow.h"
#include <QMessageBox>

YachtAddWindow::YachtAddWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::YachtAddWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
{
    ui->setupUi(this);
}

YachtAddWindow::~YachtAddWindow()
{
    delete ui;
}

void YachtAddWindow::on_buttonBox_accepted()
{
    // get name and class
    // prepere and try to add yacht
    // get added yacht id
    // prepere and try to add ownership for that yacht for current user
    QString yName = ui->NameText->text();
    QString yClass = ui->ClassText->text();
    QString yLength = ui ->LengthText->text();
    QString ySail_area = ui -> Sail_areaText->text();
    QString yDraft_minimum = ui -> Draft_minimumText->text(); // zanurzenie bez miecza
    QString yDraft_maximum = ui -> Draft_maximumText->text(); // zanurzenie z mieczem
    if (yName.length() == 0 || yClass.length() == 0 || yLength.length() == 0 || ySail_area.length() == 0 || yDraft_minimum.length() == 0 || yDraft_maximum.length() == 0) { QMessageBox::information(this, "Failed", "Enter Yacht Data"); }

    else {
        // ---------------- Dodać więcej danych do zapisywania dla dodawanych jachtów -----------------
        QString yDesc = ui->YachtDescText->toPlainText();


        m_db.transaction();

        QSqlQuery qYacht(m_db);
        qYacht.prepare("INSERT INTO yachts (name, class, length, sail_area, draft_minimum, draft_maximum) VALUES (:yName, :yClass, :yLength, :ySail_area, :yDraft_minimum, :yDraft_maximum)");

        qYacht.bindValue(":yName", yName);
        qYacht.bindValue(":yClass", yClass);
        qYacht.bindValue(":yLength", yLength);
        qYacht.bindValue(":ySail_area", ySail_area);
        qYacht.bindValue(":yDraft_minimum", yDraft_minimum);
        qYacht.bindValue(":yDraft_maximum", yDraft_maximum);

        if (!qYacht.exec()) {
            m_db.rollback();

            QString errorText =
                "Failed to add yacht:\n\n" +
                qYacht.lastError().text();

            qDebug() << errorText;

            QMessageBox::critical(this, "Database Error", errorText);
            return;
        }


        int newYachtId = qYacht.lastInsertId().toInt();

        QSqlQuery qOwner(m_db);
        qOwner.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) VALUES (:yId, :oId, :oFlag, NOW())");
        qOwner.bindValue(":yId", newYachtId);
        qOwner.bindValue(":oId", m_currentUserId);
        qOwner.bindValue(":oFlag", "Current");

        if (!qOwner.exec()) {
            m_db.rollback();
            // Show Error
            QMessageBox::information(this, "Failed", "Failed to add record to database");
            return;
        }

        m_db.commit();
        QMessageBox::information(this, "Success", "Successfuly added Yacht and Ownership");
        return;
    }

}


void YachtAddWindow::on_buttonBox_rejected()
{
    // just close and do nothing
    return;
}

