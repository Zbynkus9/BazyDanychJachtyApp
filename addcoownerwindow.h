#ifndef ADDCOOWNERWINDOW_H
#define ADDCOOWNERWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class AddCoOwnerWindow;
}

class AddCoOwnerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddCoOwnerWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase());
    ~AddCoOwnerWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddCoOwnerWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
};

#endif // ADDCOOWNERWINDOW_H
