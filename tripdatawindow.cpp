#include "tripdatawindow.h"
#include "ui_tripdatawindow.h"

#include <QMessageBox>
#include <QSqlError>
#include <qtextedit.h>

TripDataWindow::TripDataWindow(QWidget *parent, int tripId, QSqlDatabase db, int viewMode)
    : QDialog(parent)
    , ui(new Ui::TripDataWindow)
    , m_db(db)                // Save the Connection
    , m_tripId(tripId)
    , m_viewMode(viewMode)
{
    ui->setupUi(this);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &TripDataWindow::showFullJsonDetails);

    QSqlQuery tripN(m_db);
    tripN.prepare("SELECT trip_name FROM sailing_trips WHERE id_trip = :tId");
    tripN.bindValue(":tId", m_tripId);

    if (!tripN.exec() || !tripN.next()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << tripN.lastError().text(); // Senior Dev Tip: Always log the error text!
        return;
    }

    ui->TripNameTxt->setText(tripN.value(0).toString());

    // 1. Create the model (Heap allocation, pass 'this' as parent so it auto-deletes)
    QSqlQueryModel *model = new QSqlQueryModel(this);

    // 2. Prepare the complex query
    QSqlQuery query(m_db);

    switch (m_viewMode) {
    case 0:
        query.prepare(R"(
            SELECT
                timestamp,
                longitude,
                latitude,
                speed,
                wind_speed
            FROM simple_sensors_records
            WHERE trip_id = :tId
            ORDER BY timestamp
        )");
        break;
    case 1:
        query.prepare(R"(
            SELECT
                s.timestamp,
                f.data_json
            FROM full_sensors_records f
            JOIN simple_sensors_records s ON f.id_record = s.id_record
            WHERE s.trip_id = :tId
            ORDER BY s.timestamp
        )");
        break;
    default:
        QMessageBox::warning(this, "Invalid view mode", "Unknown data view mode.");
        return;
    }

    query.bindValue(":tId", m_tripId);

    if (!query.exec()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << query.lastError().text(); // Senior Dev Tip: Always log the error text!
        return;
    }

    // 3. Load the model
    model->setQuery(query);

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
    close();
}

// 2. Dodaj nową funkcję (zadeklaruj ją też w .h)
void TripDataWindow::showFullJsonDetails(const QModelIndex &index)
{
    // Sprawdź czy kliknięto w kolumnę z JSON (zakładam, że to kolumna 1 w case 1)
    if (m_viewMode == 1 && index.column() == 1) {
        QString fullJson = index.data().toString();

        // Wyświetl w ładnym oknie tekstowym
        QDialog *dlg = new QDialog(this);
        dlg->setWindowTitle("Pełny zapis SignalK");
        dlg->resize(600, 400);

        QVBoxLayout *layout = new QVBoxLayout(dlg);
        QTextEdit *textEdit = new QTextEdit(dlg);
        textEdit->setText(fullJson);
        textEdit->setReadOnly(true); // Tylko do odczytu

        layout->addWidget(textEdit);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();
    }
}
