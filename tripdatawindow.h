#ifndef TRIPDATAWINDOW_H
#define TRIPDATAWINDOW_H

#include <QDialog>

namespace Ui {
class TripDataWindow;
}

class TripDataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TripDataWindow(QWidget *parent = nullptr);
    ~TripDataWindow();

private slots:
    void on_closeBTN_clicked();

private:
    Ui::TripDataWindow *ui;
};

#endif // TRIPDATAWINDOW_H
