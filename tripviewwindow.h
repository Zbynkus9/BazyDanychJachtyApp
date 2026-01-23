#ifndef TRIPVIEWWINDOW_H
#define TRIPVIEWWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class TripViewWindow;
}

class TripViewWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TripViewWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase(), int type = 0);
    ~TripViewWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_ShowDataBTN_clicked();

    void on_ShowComplexDataBTN_clicked();

private:
    Ui::TripViewWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
    int view_type;
};

#endif // TRIPVIEWWINDOW_H
