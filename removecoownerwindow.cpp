#include "removecoownerwindow.h"
#include "ui_removecoownerwindow.h"
#include <QMessageBox>

RemoveCoOwnerWindow::RemoveCoOwnerWindow(QWidget *parent, int userId, int selectedYachtId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::RemoveCoOwnerWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
    , m_selectedYachtId(selectedYachtId)
{
    ui->setupUi(this);

    // use passed yacht id to throught query get all CoOwners excluding self
    // add all of them as elements to combobox with their id attached

}

RemoveCoOwnerWindow::~RemoveCoOwnerWindow()
{
    delete ui;
}

void RemoveCoOwnerWindow::on_buttonBox_accepted()
{
    // update selected user to be Past owner form CoOwner
}


void RemoveCoOwnerWindow::on_buttonBox_rejected()
{
    return;
}

