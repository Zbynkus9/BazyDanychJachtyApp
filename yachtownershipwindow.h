#ifndef YACHTOWNERSHIPWINDOW_H
#define YACHTOWNERSHIPWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class YachtOwnershipWindow;
}

class YachtOwnershipWindow : public QDialog
{
    Q_OBJECT

public:
    explicit YachtOwnershipWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase());
    ~YachtOwnershipWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::YachtOwnershipWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
};

#endif // YACHTOWNERSHIPWINDOW_H
