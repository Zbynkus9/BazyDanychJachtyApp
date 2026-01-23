#ifndef TRIPDATAWINDOW_H
#define TRIPDATAWINDOW_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class TripDataWindow;
}

class TripDataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TripDataWindow(QWidget *parent = nullptr, int tripId = -1, QSqlDatabase db = QSqlDatabase(), int viewMode = -1);
    ~TripDataWindow();

private slots:
    void on_closeBTN_clicked();

private:
    Ui::TripDataWindow *ui;
    QSqlDatabase m_db;
    int m_tripId;
    int m_viewMode;
};

#endif // TRIPDATAWINDOW_H
