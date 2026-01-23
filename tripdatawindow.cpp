#include "tripdatawindow.h"
#include "ui_tripdatawindow.h"

TripDataWindow::TripDataWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TripDataWindow)
{
    ui->setupUi(this);
}

TripDataWindow::~TripDataWindow()
{
    delete ui;
}

void TripDataWindow::on_closeBTN_clicked()
{
    return;
}

