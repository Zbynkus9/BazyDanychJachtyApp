#include "selectyachtwindow.h"
#include "ui_selectyachtwindow.h"
#include "removecoownerwindow.h"

SelectYachtWindow::SelectYachtWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::SelectYachtWindow)
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

SelectYachtWindow::~SelectYachtWindow()
{
    delete ui;
}

void SelectYachtWindow::on_buttonBox_accepted()
{
    // open RemoveCoOwnerWindow QDialog Widget and pass selected Yacht Id
}


void SelectYachtWindow::on_buttonBox_rejected()
{
    return;
}

