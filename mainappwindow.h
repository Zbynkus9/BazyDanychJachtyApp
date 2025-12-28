#ifndef MAINAPPWINDOW_H
#define MAINAPPWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class MainAppWindow;
}

class MainAppWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainAppWindow(QWidget *parent = nullptr, int userID = -1, QSqlDatabase db = QSqlDatabase());
    ~MainAppWindow();

private:
    Ui::MainAppWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
};

#endif // MAINAPPWINDOW_H
