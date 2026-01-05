#ifndef TRIPADDWINDOW_H
#define TRIPADDWINDOW_H

#include <QDialog>

namespace Ui {
class TripAddWindow;
}

class TripAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TripAddWindow(QWidget *parent = nullptr);
    ~TripAddWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::TripAddWindow *ui;
};

#endif // TRIPADDWINDOW_H
