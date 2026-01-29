#ifndef TRIPADDWINDOW_H
#define TRIPADDWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QString>

namespace Ui {
class TripAddWindow;
}

class TripAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TripAddWindow(
        QWidget *parent = nullptr,
        int userId = -1,
        QSqlDatabase db = QSqlDatabase()
        );
    ~TripAddWindow();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_clicked();

private:
    Ui::TripAddWindow *ui;

    int m_currentUserId;
    QSqlDatabase m_db;

    // ---- Directory handling ----
    QString m_selectedDirectory;
    QString m_startPath;
};

#endif // TRIPADDWINDOW_H
