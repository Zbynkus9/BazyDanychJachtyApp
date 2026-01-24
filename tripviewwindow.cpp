#include "tripviewwindow.h"
#include "ui_tripviewwindow.h"
#include "tripdatawindow.h"
#include <QMessageBox>

TripViewWindow::TripViewWindow(QWidget *parent, int userId, QSqlDatabase db, int type)
    : QDialog(parent)
    , ui(new Ui::TripViewWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
    , view_type(type)
{
    ui->setupUi(this);
    // 1. Create the model (Heap allocation, pass 'this' as parent so it auto-deletes)
    QSqlQueryModel *model = new QSqlQueryModel(this);

    // 2. Prepare the complex query
    QSqlQuery query(m_db);
    // Zależnie od view type różne widoki
    switch (view_type) {
    // 0 - widok tylko swoich rejsów (trips.user_id == m_currentUserId)
    case 0:
        query.prepare("SELECT sailing_trips.id_trip, sailing_trips.trip_name AS 'Trip title', yachts.name AS 'Yacht', sailing_trips.start_datetime AS 'Date' FROM sailing_trips JOIN yachts ON sailing_trips.yacht_id = yachts.id_yacht WHERE sailing_trips.user_id = :uId ORDER BY sailing_trips.start_datetime DESC");
        break;
    // 1 - widok swoich rejsów + rejsów z przypisanym jachtem
    case 1:
        query.prepare("SELECT sailing_trips.id_trip, sailing_trips.trip_name AS 'Trip title',  users.username AS 'Captain', yachts.name AS 'Yacht', sailing_trips.start_datetime AS 'Date' FROM sailing_trips JOIN users ON sailing_trips.user_id = users.id_user JOIN yachts ON sailing_trips.yacht_id = yachts.id_yacht JOIN yacht_ownership ON sailing_trips.yacht_id = yacht_ownership.yacht_id WHERE yacht_ownership.owner_id = :uId AND yacht_ownership.ownership_flag IN ('Current', 'CoOwner') ORDER BY sailing_trips.start_datetime DESC");
        break;
    // 2 - widok tylko rejsów udostępnionych przez innych (sailing_trips.visibility == Public)
    case 2:
        query.prepare("SELECT sailing_trips.id_trip, sailing_trips.trip_name AS 'Trip title',  users.username AS 'Captain', yachts.name AS 'Yacht', sailing_trips.start_datetime AS 'Date' FROM sailing_trips JOIN users ON sailing_trips.user_id = users.id_user JOIN yachts ON sailing_trips.yacht_id = yachts.id_yacht WHERE sailing_trips.user_id != :uId AND sailing_trips.visibility = 'Public' ORDER BY sailing_trips.start_datetime DESC");
        break;
    default:
        break;
    }


    query.bindValue(":uId", m_currentUserId);

    if (!query.exec()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << query.lastError().text(); // Senior Dev Tip: Always log the error text!
    }

    // 3. Load the model
    model->setQuery(std::move(query));

    // 4. Glue Model to View
    ui->tableView->setModel(model);

    // Optional: Adjust column widths to look nice
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnHidden(0, true); // hide id
}

TripViewWindow::~TripViewWindow()
{
    delete ui;
}

void TripViewWindow::on_pushButton_2_clicked()
{
    TripViewWindow::done(0);
}


void TripViewWindow::on_ShowDataBTN_clicked()
{
    // 1. Get the list of selected rows
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (!select->hasSelection()) {
        // Warning: "Please select a trip first."
         QMessageBox::information(this, "Failed", "Please select a trip first.");
    }
    else {
        // 2. Get the specific ModelIndex of the ID column (Column 0) for the selected row
        QModelIndex index = select->selectedRows(0).at(0); // 0 = Column Index for id_trip

        // 3. Extract the data
        int tripId = index.data().toInt();

        // 4. Open the Data Dialog ---------- dodać QDialog Widget -------------
        TripDataWindow *window = new TripDataWindow(this, tripId, m_db, 0);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
    }
}


void TripViewWindow::on_ShowComplexDataBTN_clicked()
{
    // 1. Get the list of selected rows
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (!select->hasSelection()) {
        // Warning: "Please select a trip first."
        QMessageBox::information(this, "Failed", "Please select a trip first.");
    }
    else {
        // 2. Get the specific ModelIndex of the ID column (Column 0) for the selected row
        QModelIndex index = select->selectedRows(0).at(0); // 0 = Column Index for id_trip

        // 3. Extract the data
        int tripId = index.data().toInt();

        // 4. Open the Data Dialog ---------- dodać QDialog Widget -------------
        TripDataWindow *window = new TripDataWindow(this, tripId, m_db, 1);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
    }
}

