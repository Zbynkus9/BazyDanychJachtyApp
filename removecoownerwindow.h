#ifndef REMOVECOOWNERWINDOW_H
#define REMOVECOOWNERWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class RemoveCoOwnerWindow;
}

class RemoveCoOwnerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveCoOwnerWindow(QWidget *parent = nullptr, int userId = -1, int selectedYachtId = -1, QSqlDatabase db = QSqlDatabase());
    ~RemoveCoOwnerWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::RemoveCoOwnerWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
    int m_selectedYachtId;
};

#endif // REMOVECOOWNERWINDOW_H
