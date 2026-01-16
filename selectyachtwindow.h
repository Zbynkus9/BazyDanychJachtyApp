#ifndef SELECTYACHTWINDOW_H
#define SELECTYACHTWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class SelectYachtWindow;
}

class SelectYachtWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SelectYachtWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase());
    ~SelectYachtWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SelectYachtWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
};

#endif // SELECTYACHTWINDOW_H
