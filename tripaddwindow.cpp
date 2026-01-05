#include "tripaddwindow.h"
#include "ui_tripaddwindow.h"

TripAddWindow::TripAddWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TripAddWindow)
{
    ui->setupUi(this);
}

TripAddWindow::~TripAddWindow()
{
    delete ui;
}

void TripAddWindow::on_buttonBox_accepted()
{
    return;
}


void TripAddWindow::on_buttonBox_rejected()
{
    return;
}

