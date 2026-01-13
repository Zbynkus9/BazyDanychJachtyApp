#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase());
    ~MainWindow();

private slots:

    void on_YachtsBTN_clicked();

    void on_YachtsOwnershipBTN_clicked();

    void on_TripAddBTN_clicked();

    void on_TripListOwnBTN_clicked();

    void on_TripListSharedBTN_clicked();

    void on_TripListPublicBTN_clicked();

private:
    Ui::MainWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
};
#endif // MAINWINDOW_H
