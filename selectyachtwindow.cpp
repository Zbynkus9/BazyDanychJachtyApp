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
    yachts.prepare("SELECT yachts.id_yacht AS 'yId', yachts.name AS 'yName' FROM yachts JOIN yacht_ownership ON yachts.id_yacht = yacht_ownership.yacht_id WHERE owner_id = :currentUserId AND NOT EXISTS ( SELECT 1 FROM yacht_ownership history WHERE history.owner_id = users.id_user AND history.yacht_id = :yId AND history.ownership_flag = 'Past') ORDER BY yachts.name");
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
    int selectedYacht = ui->YachtComboBox->itemData(ui->YachtComboBox->currentIndex()).toInt();
    // open RemoveCoOwnerWindow QDialog Widget and pass selected Yacht Id
    // 1. Create the instance (Stack allocation)
    // Pass 'this' as parent so it centers over the Main Window
    RemoveCoOwnerWindow dialog(this, m_currentUserId, selectedYacht, m_db);

    // 2. Launch it effectively "freezing" the code here until the dialog closes
    dialog.exec();
}


void SelectYachtWindow::on_buttonBox_rejected()
{
    return;
}

