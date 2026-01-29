#include "mainwindow.h"
#include "login.h"
#include "databasemanager.h"
#include <QApplication>
#include <QMessageBox>
#include "theme.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Dark Mode
    Theme::applyDarkPalette(a);

    // DB Manager
    DatabaseManager dbManager;

    if (!dbManager.connect()) {
        QMessageBox::information(nullptr, "Failed", "Connection Failed");
        return -1; // Wyłącza aplikację, jeżeli DB nie została wykryta
    }

    LoginScreen login(nullptr, dbManager.db());

    MainWindow *w = nullptr;

    QObject::connect(&login, &LoginScreen::loginSuccessful, [&](int userId, QString username){

        w = new MainWindow(nullptr, userId, dbManager.db(), username);

        // Czyszczenie pamięci
        w->setAttribute(Qt::WA_DeleteOnClose);

        w->show();
    });

    // Blokujemy ekran, dopóki się użytkownik nie zaloguje
    if (login.exec() == QDialog::Accepted) {
        return a.exec();
    }
    else {
        return 0;
    }
}
