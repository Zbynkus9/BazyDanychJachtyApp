#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

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

void LoginScreen::on_LoginBTN_clicked()
{
    QString username = login_ui->LoginLoginText->text();
    QString password = login_ui->LoginPassText->text();

   QSqlQuery query;
    query.prepare("SELECT username, password FROM users_credentials where username = :username AND password = :password");

   query.bindValue(":username", username);
    query.bindValue(":password", password);

   if(!query.exec()) {
       QMessageBox::information(this, "Failed", "Querry Failed");
   }
   else {
       QString usernameFromDB = query.value(0).toString();
       QString passwordFromDB = query.value(1).toString();

       if (usernameFromDB == username && passwordFromDB == password) {
           QMessageBox::information(this, "Success", "Login succesful");
       }
       else {
           QMessageBox::information(this, "Failed", "Login Failed");
       }

   }
}

