#include "tripviewwindow.h"
#include "ui_tripviewwindow.h"

TripViewWindow::TripViewWindow(QWidget *parent, int userId, QSqlDatabase db, int type)
    : QDialog(parent)
    , ui(new Ui::TripViewWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
    , view_type(type);
{
    ui->setupUi(this);
}

TripViewWindow::~TripViewWindow()
{
    delete ui;
}

void TripViewWindow::on_pushButton_2_clicked()
{
    TripViewWindow::done(0);
}

