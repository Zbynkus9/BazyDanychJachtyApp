#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QDialog
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~LoginScreen();

public slots:
    void loginSuccessful(int userId);

private slots:
    void on_LoginBTN_clicked();

private:
    Ui::LoginScreen *login_ui;
    QSqlDatabase m_db;
};

#endif // LOGIN_H
