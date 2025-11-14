#include "login.h"
#include "ui_login.h"

LoginScreen::LoginScreen(QWidget *parent)
    : QDialog(parent)
    , login_ui(new Ui::LoginScreen)
{
    login_ui->setupUi(this);
}

LoginScreen::~LoginScreen()
{
    delete login_ui;
}
