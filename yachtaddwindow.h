#ifndef YACHTADDWINDOW_H
#define YACHTADDWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class YachtAddWindow;
}

class YachtAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit YachtAddWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase());
    ~YachtAddWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::YachtAddWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
    QLocale locale;
};

#endif // YACHTADDWINDOW_H
