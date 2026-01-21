#ifndef TRIPADDWINDOW_H
#define TRIPADDWINDOW_H

#include <QDialog>
#include <QFileSystemModel>
#include <QDir>
#include "databasemanager.h"

namespace Ui {
class TripAddWindow;
}

class TripAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TripAddWindow(QWidget *parent = nullptr, int userId = -1, QSqlDatabase db = QSqlDatabase());
    ~TripAddWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_clicked();

private:
    Ui::TripAddWindow *ui;
    int m_currentUserId;
    QSqlDatabase m_db;
    QFileSystemModel *m_dirSensorData;
    QString m_selectedDirectory;
    QString dirPath;
    QString startPath;
};

#endif // TRIPADDWINDOW_H
