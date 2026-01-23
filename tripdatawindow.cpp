#include "tripdatawindow.h"
#include "ui_tripdatawindow.h"

TripDataWindow::TripDataWindow(QWidget *parent, int tripId, QSqlDatabase db, int viewMode)
    : QDialog(parent)
    , ui(new Ui::TripDataWindow)
    , m_db(db)                // Save the Connection
    , m_tripId(tripId)
    , m_viewMode(viewMode)
{
    ui->setupUi(this);

    QSqlQuery tripN(m_db);
    tripN.prepare("SELECT trip_name FROM sailing_trips WHERE id_trip = :tId");

    if (!tripN.exec()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << tripN.lastError().text(); // Senior Dev Tip: Always log the error text!
    }

    QString tName = tripN.value(0).toString();


    ui->TripNameTxt->setText(tName);

    // 1. Create the model (Heap allocation, pass 'this' as parent so it auto-deletes)
    QSqlQueryModel *model = new QSqlQueryModel(this);

    // 2. Prepare the complex query
    QSqlQuery query(m_db);

    switch (m_viewMode) {
    case 0:
        query.prepare("SELECT timestamp, longitude, latitude, speed, wind_speed FROM simple_sensor_records WHERE trip_id = :tId");
        break;
    case 1:
        query.prepare("SELECT simple_sensor_records.timestamp, full_sensor_records.data_json FROM full_sensor_records JOIN simple_sensor_records ON full_sensor_records.id_record = simple_sensor_records.id_record WHERE simple_sensor_records.trip_id = :tId");
        break;
    default:
        break;
    }

    query.bindValue(":tId", m_tripId);

    if (!query.exec()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << query.lastError().text(); // Senior Dev Tip: Always log the error text!
        return;
    }

    // 3. Load the model
    model->setQuery(std::move(query));

    // 4. Glue Model to View
    ui->tableView->setModel(model);

    // Optional: Adjust column widths to look nice
    ui->tableView->resizeColumnsToContents();
}

TripDataWindow::~TripDataWindow()
{
    delete ui;
}

void TripDataWindow::on_closeBTN_clicked()
{
    return;
}

