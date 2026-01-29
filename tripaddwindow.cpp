#include "tripaddwindow.h"
#include "ui_tripaddwindow.h"
#include "signalkimporter.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

TripAddWindow::TripAddWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::TripAddWindow)
    , m_currentUserId(userId)
    , m_db(db)
{
    ui->setupUi(this);

    m_startPath = QDir::homePath();
    m_selectedDirectory.clear();

    // Wczytujemy jachty
    QSqlQuery yachts(m_db);
        yachts.prepare(R"(
        SELECT y.id_yacht AS yId, y.name AS yName
        FROM yachts y
        JOIN yacht_ownership yo
          ON y.id_yacht = yo.yacht_id
        WHERE yo.owner_id = :uid
        ORDER BY y.name
    )");

    yachts.bindValue(":uid", m_currentUserId);

    if (!yachts.exec()) {
        qCritical() << "SQL Error:" << yachts.lastError().text();
        QMessageBox::critical(this, "Database error", "Failed to load yachts");
        return;
    }

    while (yachts.next()) {
        ui->YachtComboBOx->addItem(
            yachts.value("yName").toString(),
            yachts.value("yId").toInt()
            );
    }
}

TripAddWindow::~TripAddWindow()
{
    delete ui;
}

void TripAddWindow::on_buttonBox_accepted()
{
    QString tripName = ui->TripNameText->text().trimmed();
    QString tripDesc = ui->TripDescriptionText->toPlainText().trimmed();

    if (tripName.isEmpty() || tripDesc.isEmpty()) {
        QMessageBox::information(this, "Missing data", "Trip name and description are required.");
        return;
    }

    if (m_selectedDirectory.isEmpty()) {
        QMessageBox::information(this, "Missing data", "Please select a directory with JSON files.");
        return;
    }

    int yachtId = ui->YachtComboBOx
                      ->itemData(ui->YachtComboBOx->currentIndex())
                      .toInt();

    QString visibility = ui->VisibilityComboBox->currentText();

    // Transakcja
    if (!m_db.transaction()) {
        QMessageBox::critical(this, "Database error", "Failed to start transaction.");
        return;
    }

    QSqlQuery trip(m_db);
    trip.prepare(R"(
        INSERT INTO sailing_trips
        (user_id, yacht_id, trip_name, trip_description,
         start_datetime, end_datetime, visibility)
        VALUES
        (:uid, :yid, :name, :desc,
         NOW(), NOW(), :vis)
    )");

    trip.bindValue(":uid", m_currentUserId);
    trip.bindValue(":yid", yachtId);
    trip.bindValue(":name", tripName);
    trip.bindValue(":desc", tripDesc);
    trip.bindValue(":vis", visibility);

    if (!trip.exec()) {
        qCritical() << "SQL Error:" << trip.lastError().text();
        m_db.rollback();
        QMessageBox::critical(this, "Database error", "Failed to create trip.");
        return;
    }

    int tripId = trip.lastInsertId().toInt();

    // Odpalamy SignalKImporter
    QString importError;
    bool importOk = SignalKImporter::importDirectory(
        m_db,
        tripId,
        m_selectedDirectory,
        importError
        );

    if (!importOk) {
        m_db.rollback();
        QMessageBox::critical(this, "Import failed", importError);
        return;
    }

    // Commit
    if (!m_db.commit()) {
        QMessageBox::critical(this, "Database error", "Failed to commit transaction.");
        return;
    }

    accept();
}

void TripAddWindow::on_buttonBox_rejected()
{
    reject();
}

void TripAddWindow::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select folder with Signal K JSON files"),
        m_startPath
        );

    if (!dir.isEmpty()) {
        m_selectedDirectory = dir;
    }
}
