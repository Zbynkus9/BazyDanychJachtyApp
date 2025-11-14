#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QDialog
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

private:
    Ui::LoginScreen *login_ui;
};

#endif // LOGIN_H
