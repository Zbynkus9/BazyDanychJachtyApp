#include "mainwindow.h"
#include "login.h"
#include "databasemanager.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Setup Database Manager
    DatabaseManager dbManager;

    // 2. Connect immediately (or show a simplified "Settings" dialog if it fails)
    if (!dbManager.connect()) {
        // Optional: Show a QMessageBox here saying "Cannot connect to server"
        QMessageBox::information(nullptr, "Failed", "Connection Failed");
        return -1; // Exit app if no DB
    }

    // 3. Create Login Screen, passing the active database
    LoginScreen login(nullptr, dbManager.db());

    // 4. Prepare the Main Window pointer
    MainWindow *w = nullptr;

    // 5. THE WIRING: Connect Login Success Signal to Main Window Creation
    QObject::connect(&login, &LoginScreen::loginSuccessful, [&](int userId){

        // Create Main Window on Heap
        w = new MainWindow(nullptr, userId, dbManager.db());

        // Ensure memory is cleaned up when user closes Main Window
        w->setAttribute(Qt::WA_DeleteOnClose);

        w->show();

        // Login screen will close itself via "accept()" in its own code
    });

    // 6. Show Login and block until it closes
    if (login.exec() == QDialog::Accepted) {
        // If we get here, it means "accept()" was called in Login (Success)
        // The lambda above ^ has already run, so 'w' is open.
        // We enter the event loop for the Main Window.
        return a.exec();
    }
    else {
        // User clicked "Cancel" or closed the login window manually.
        // We quit the app.
        return 0;
    }
}
