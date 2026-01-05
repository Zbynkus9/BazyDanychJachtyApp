#include "yachtownershipwindow.h"
#include "ui_yachtownershipwindow.h"

YachtOwnershipWindow::YachtOwnershipWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::YachtOwnershipWindow)
{
    ui->setupUi(this);
}

YachtOwnershipWindow::~YachtOwnershipWindow()
{
    delete ui;
}

void YachtOwnershipWindow::on_pushButton_clicked()
{
    YachtOwnershipWindow::done(0);
}

