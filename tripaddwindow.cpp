#include "tripaddwindow.h"
#include "ui_tripaddwindow.h"

TripAddWindow::TripAddWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::TripAddWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
{
    ui->setupUi(this);
}

TripAddWindow::~TripAddWindow()
{
    delete ui;
}

void TripAddWindow::on_buttonBox_accepted()
{
    /*
     Sprawdzić czy wybrany jest folder i nie jest on pusty (oddzielnie żeby inne błedy pokazać)
        Ogarnąć parsowanie JSON (wydobycie określonych danych [tutaj lat, lon, spd, w_spd, timestamp])
        For loop przez pliki w wybranym folderze i dla każdego !!!!!!(Zrobić to wszystko na tranzakcji żeby wyłapać jak się wykrzaczy i opcjonalnie poinformować na którym pliku się wywaliło)!!!!!!
            - parsowanie danych z JSON i zapisanie ich do zmiennych
            - stworzenie rekordu w tabeli simple_sensor_records z: danymi z JSON'a, sailing_trips.id_trip
            - stworzenie rekordu z tym samym id co rekord w simple_sensor_records w tabeli full_sensor_records z plikiem JSON
    */
    return;
}


void TripAddWindow::on_buttonBox_rejected()
{
    return;
}


void TripAddWindow::on_pushButton_clicked()
{
    // Wybór folderu z JSON'ami i zapisanie go
}

