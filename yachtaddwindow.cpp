#include "yachtaddwindow.h"
#include "ui_yachtaddwindow.h"

YachtAddWindow::YachtAddWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::YachtAddWindow)
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


}


void YachtAddWindow::on_buttonBox_rejected()
{
    // just close and do nothing
    YachtAddWindow::~YachtAddWindow();
}

