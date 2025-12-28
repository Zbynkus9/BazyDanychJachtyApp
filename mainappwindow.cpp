#include "mainappwindow.h"
#include "ui_mainappwindow.h"

MainAppWindow::MainAppWindow(QWidget *parent, int userID, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::MainAppWindow)
    , m_currentUserId(userID)
    , m_db(db)
{
    ui->setupUi(this);
}

MainAppWindow::~MainAppWindow()
{
    delete ui;
}
