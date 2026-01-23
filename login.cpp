#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

LoginScreen::LoginScreen(QWidget *parent, QSqlDatabase db)
    : QDialog(parent)
    , login_ui(new Ui::LoginScreen)
    , m_db(db)
{
    login_ui->setupUi(this);

    // Create a regex that allows digits (0-9) and repeats 1 to 9 times
    QRegularExpression rx("^[0-9]{1,9}$");

    // Create the validator
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    login_ui->RegNrText->setValidator(validator);
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


void LoginScreen::on_RegisterBTN_clicked()
{

    QString email = login_ui->RegEmailText->text();
    if (email.length() == 0) { QMessageBox::information(this, "Failed", "Enter credentials"); }
    else{
        QSqlQuery mailCheck (m_db);

        mailCheck.prepare("SELECT email FROM users WHERE email = :mail");
        mailCheck.bindValue(":mail", email);

        if(!mailCheck.exec()) {
            QMessageBox::critical(
                this,
                "SQL Error",
                mailCheck.lastError().text()
                );
            return;
        }
        else {
            if (mailCheck.next()) {
                // show that mail used
                QMessageBox::information(this, "Failed", "Email already used");
                return;
            }
        }

        QString username = login_ui->RegLoginText->text();

        if (username.length() == 0) { QMessageBox::information(this, "Failed", "Enter credentials"); }
        else {
            QSqlQuery nickCheck (m_db);

            nickCheck.prepare("SELECT username FROM users WHERE username = :username");
            nickCheck.bindValue(":username", username);

            if(!nickCheck.exec()) {
                QMessageBox::information(this, "Failed", "Username Querry Failed");
                return;
            }
            else {
                if (nickCheck.next()) {
                    // show that username used
                    QMessageBox::information(this, "Failed", "Username already used");
                    return;
                }
            }
        }

        QString fname = login_ui->RegFirstNameText->text();
        QString lname = login_ui->RegLastNameText->text();
        QString password = login_ui->RegPassText->text();
        QString phone = login_ui->RegNrText->text();

        if (fname.length() == 0 || lname.length() == 0 || password.length() == 0) { QMessageBox::information(this, "Failed", "Enter credentials"); }
        else {
            //instert record into users
            QSqlQuery registerUser (m_db);
            if (phone.length() == 0){
                registerUser.prepare("INSERT INTO users (first_name, last_name, email, username, password) VALUES (:fname, :lname, :mail, :username, :password)");
            }
            else {
                registerUser.prepare("INSERT INTO users (first_name, last_name, email, phone_number, username, password) VALUES (:fname, :lname, :mail, :phone, :username, :password)");
                registerUser.bindValue(":phone", phone);
            }
            registerUser.bindValue(":fname", fname);
            registerUser.bindValue(":lname", lname);
            registerUser.bindValue(":mail", email);
            registerUser.bindValue(":username", username);
            registerUser.bindValue(":password", password);

            if(!registerUser.exec()) {
                QMessageBox::information(this, "Failed", "Register Querry Failed");
            }
            else {
                QMessageBox::information(this, "Success", "Register succesful");
            }
        }

    }

}
