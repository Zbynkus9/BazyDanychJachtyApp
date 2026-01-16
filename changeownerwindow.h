#ifndef CHANGEOWNERWINDOW_H
#define CHANGEOWNERWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class ChangeOwnerWindow;
}

class ChangeOwnerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeOwnerWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase());
    ~ChangeOwnerWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ChangeOwnerWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
};

#endif // CHANGEOWNERWINDOW_H
