#include "tripaddwindow.h"
#include "ui_tripaddwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileSystemModel>

TripAddWindow::TripAddWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::TripAddWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
    , m_dirSensorData(new QFileSystemModel(this))
{
    ui->setupUi(this);
    // ---------------------------------------Ogarnąć filtrowanie tylko plików JSON--------------------------------------------------
    //m_dirSensorData->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    //m_dirSensorData->setNameFilters(QStringList() << "*.json");
    //m_dirSensorData->setNameFilterDisables(false);

    // -------------------------------------Pomyśleć czy TripDataVisibility nie zrobić na checkBox'ach------------------------------------------------

    QSqlQuery yachts(m_db);
    yachts.prepare("SELECT yachts.id_yacht AS 'yId', yachts.name AS 'yName' FROM yacht JOIN yachts_ownership ON yachts.id_yacht = yacht_ownership.yacht_id WHERE owner_id = :currentUserId ORDER BY yachts.name");
    yachts.bindValue(":currentUserId", m_currentUserId);

    if (!yachts.exec()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << yachts.lastError().text(); // Senior Dev Tip: Always log the error text!
    }

    while (yachts.next()) {
        QString yName = yachts.value("yName").toString();
        int yId = yachts.value("yId").toInt();
        ui->YachtComboBOx->addItem(yName, yId); // Store ID as "UserRole" data
    }
}

TripAddWindow::~TripAddWindow()
{
    delete ui;
}

void TripAddWindow::on_buttonBox_accepted()
{
    /*
     Sprawdzić czy wybrany jest folder i nie jest on pusty (oddzielnie żeby inne błedy pokazać) (jeśli błąd jest to nie kontynuować dodawania ale też nie wywalać okna)
     Pobrać który jacht jest wybrany
     Pobrać jakie TripVisibility jest wybrane (jeśli private to olać TripDataVisibility)
     Pobrać jakie TripDataVisibility są zaznaczone (aktualizowane na bierząco do listy????)
     Pobrać dane z pól tekstowych (Title, Desc)
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
    QString startPath = m_dirSensorData->rootPath();
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Wybierz folder"), startPath);
    if (!dirPath.isEmpty()) {
        m_selectedDirectory = dirPath;
    }
    else {
        QMessageBox::information(this, "Failed", "Directory is empty");
    }
}

