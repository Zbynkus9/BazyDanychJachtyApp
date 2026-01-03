#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

LoginScreen::LoginScreen(QWidget *parent, QSqlDatabase db)
    : QDialog(parent)
    , login_ui(new Ui::LoginScreen)
    , m_db(db)
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
    if (username.length() == 0 || password.length() == 0) { QMessageBox::information(this, "Failed", "Enter username and/or password"); }
    else {
        QString usernameFromDB = "";
        QString passwordFromDB = "";

        QSqlQuery query(m_db);
        query.prepare("SELECT username, password, id_user FROM users where username = :username AND password = :password");

       query.bindValue(":username", username);
        query.bindValue(":password", password);

       if(!query.exec()) {
           QMessageBox::information(this, "Failed", "Querry Failed");
       }
       else {
           if (query.next()) {
               usernameFromDB = query.value(0).toString();
               passwordFromDB = query.value(1).toString();
           }
           else {
               QMessageBox::information(this, "Failed", "User not found");
           }

           if (usernameFromDB == username && passwordFromDB == password) {
               QMessageBox::information(this, "Success", "Login succesful");
               int userID = query.value(2).toInt();
               emit loginSuccessful(userID);

               // Close the login screen so main.cpp knows to move on
               this->accept();
            }
           else {
               QMessageBox::information(this, "Failed", "Login Failed");
           }

       }
   }
}

