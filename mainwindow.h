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

private:
    Ui::MainWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
};
#endif // MAINWINDOW_H
