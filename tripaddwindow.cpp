#include "tripaddwindow.h"
#include "ui_tripaddwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QDateTime>

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

    QString startPath = m_dirSensorData->rootPath();
    QString dirPath = startPath;
}

TripAddWindow::~TripAddWindow()
{
    delete ui;
}

void TripAddWindow::on_buttonBox_accepted()
{
    int selectedYacht = ui->YachtComboBOx->itemData(ui->YachtComboBOx->currentIndex()).toInt();
    QString tripVisibility = ui->VisibilityComboBox->itemData(ui->VisibilityComboBox->currentIndex()).toString();
    QString tName = ui->TripNameText->text();
    QString tDesc = ui->TripDescriptionText->toPlainText();

    // Sprawdzić czy nie są puste pola
    if (tName.length() == 0 || tDesc.length() == 0) {
        QMessageBox::information(this, "Failed", "Enter trip informations");
        return;
    }

    // zrobić dodanie rejsu wpierw
    m_db.transaction();
    QSqlQuery trip(m_db);
    trip.prepare("INSERT INTO sailing_trips (user_id, yacht_id, trip_name, trip_description, start_datetime, end_datetime, trip_data_visibility, visibility) VALUES (:uId, :yId, :tName, :tDesc, NOW(), NOW(), :tDataVis, :tVis);");
    trip.bindValue(":uId", m_currentUserId);
    trip.bindValue(":yId", selectedYacht);
    trip.bindValue(":tName", tName);
    trip.bindValue(":tDesc", tDesc);
    trip.bindValue(":tDataVis", "Username"); // temporary
    trip.bindValue(":tVis", tripVisibility);

    if (!trip.exec()) {
        m_db.rollback(); // Undo the Simple insert too!
        qCritical() << "SQL Error:" << trip.lastError().text();
        return;
    }

    int tId = trip.lastInsertId().toInt();


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
    QDir dir(dirPath);
    // Filter only JSON files
    QStringList filters; filters << "*.json";
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);



    // OPTIONAL: Start a "Global" Transaction here if you want "All or Nothing"


    foreach (QFileInfo fileInfo, fileList) {
        // A. Read File
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) continue;
        QByteArray rawData = file.readAll();

        // B. Parse JSON
        QJsonDocument doc = QJsonDocument::fromJson(rawData);
        QJsonObject obj = doc.object();

        // C. Extract "Simple" Data (Add error checking if keys exist!)

        double lon = -1;
        double lat = -1;
        double speed = -1;

        if (!obj.contains("latitude")) lat = -1;
        else lat = obj["latitude"].toDouble();
        if (!obj.contains("longitude")) lon = -1;
        else lon = obj["longitude"].toDouble();
        if (!obj.contains("speed")) speed = -1;
        else speed = obj["speed"].toDouble();
        // if (!obj.contains("wind")) double windSpeed = -1;
        // else double windSpeed = obj["wind"].toDouble(); // ------------ search what is name for obj for wind speed
        // if (!obj.contains("timestamp")) QDateTime timestamp = 0;
        // else {
        //      QString jsonString = obj["timestamp"].toString("yyyy-MM-dd hh:mm:ss");
        //      QDateTime timestamp = QDateTime::fromString(jsonString, Qt::ISODate);
        // }

        // D. Database Insert (Per-File Transaction is safer for partial imports)

        // Query 1: Insert into SIMPLE (Base)
        QSqlQuery qSimple(m_db);
        qSimple.prepare("INSERT INTO simple_sensors_records (trip_id, timestamp, longitude, latitude, speed, wind_speed) VALUES (:tId, :tStmp, :lon, :lat, :spd, :wSpd)");

        qSimple.bindValue(":tId", tId);
        // qSimple.bindValue(":tStmp", timestamp);
        qSimple.bindValue(":lon", lon);
        qSimple.bindValue(":lat", lat);
        qSimple.bindValue(":spd", speed);
        // qSimple.bindValue(":wSpd", windSpeed);

        if (!qSimple.exec()) {
            m_db.rollback();
            qCritical() << "SQL Error:" << qSimple.lastError().text();
            return;
        }

        // Capture the generated ID
        QVariant newRecordId = qSimple.lastInsertId();

        // Query 2: Insert into FULL (Extension)
        QSqlQuery qFull(m_db);
        // Note: We insert the ID we just got, NOT NULL
        qFull.prepare("INSERT INTO full_sensors_records (id_record, data_json) VALUES (:id, :json)");
        qFull.bindValue(":id", newRecordId);
        qFull.bindValue(":json", rawData); // Store raw bytes or doc.toJson()

        if (!qFull.exec()) {
            m_db.rollback(); // Undo the Simple insert too!
            qDebug() << "Failed to save full record for file:" << fileInfo.fileName();
            return;
        } else {
            m_db.commit(); // Success!
        }
    }

    return;
}


void TripAddWindow::on_buttonBox_rejected()
{
    return;
}


void TripAddWindow::on_pushButton_clicked()
{
    // Wybór folderu z JSON'ami i zapisanie go

    dirPath = QFileDialog::getExistingDirectory(this, tr("Wybierz folder z odczytami"), startPath);
    if (!dirPath.isEmpty()) {
        m_selectedDirectory = dirPath;
    }
    else {
        QMessageBox::information(this, "Failed", "Directory is empty");
        return;
    }
}

