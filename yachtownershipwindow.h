#ifndef YACHTOWNERSHIPWINDOW_H
#define YACHTOWNERSHIPWINDOW_H

#include <QDialog>

namespace Ui {
class YachtOwnershipWindow;
}

class YachtOwnershipWindow : public QDialog
{
    Q_OBJECT

public:
    explicit YachtOwnershipWindow(QWidget *parent = nullptr);
    ~YachtOwnershipWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::YachtOwnershipWindow *ui;
};

#endif // YACHTOWNERSHIPWINDOW_H
