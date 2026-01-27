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

    qDebug() << "Window type: " << view_type;

    // 2. Prepare the complex query
    QSqlQuery query(m_db);
    // Zależnie od view type różne widoki
    switch (view_type) {
    // 0 - widok tylko swoich rejsów (trips.user_id == m_currentUserId)
    case 0:
        qDebug() << "Case 0";
        query.prepare("SELECT "
                  "id_trip, "           // Kolumna 0: Potrzebna dla logiki (ukryjemy ją)
                  "`Nazwa Rejsu`, "     // Kolumna 1
                  "Jacht, "             // Kolumna 2
                  "Start, "             // Kolumna 3
                  "Status, "            // Kolumna 4
                  "`Minuty_Trwania`, "  // Kolumna 5
                  "`Liczba_Próbek` "    // Kolumna 6
                  "FROM view_trip_dashboard "
                  "WHERE id_kapitana = :uId "  // Filtrujemy tylko moje rejsy
                  "ORDER BY Start DESC");
        break;
    // 1 - widok swoich rejsów + rejsów z przypisanym jachtem
    case 1:
        qDebug() << "Case 1";
        query.prepare("SELECT "
                      "id_trip, "           // W widoku kolumny są unikalne, nie musisz pisać nazwy tabeli
                      "`Nazwa Rejsu`, "
                      "Jacht, "
                      "Kapitan, "
                      "Start, "
                      "Status, "
                      "`Liczba_Próbek` "
                      "FROM view_trip_dashboard "
                      "WHERE func_is_active_owner(:uId, id_jachtu) = 1 "
                      "ORDER BY Start DESC");
        break;
    // 2 - widok tylko rejsów udostępnionych przez innych (sailing_trips.visibility == Public)
    case 2:
        qDebug() << "Case 2";
        query.prepare("SELECT "
                      "id_trip, "           // Kolumna 0 (ukryta)
                      "`Nazwa Rejsu`, "
                      "Jacht, "
                      "Kapitan, "           // Tutaj warto widzieć kto płynął Twoim jachtem
                      "Start, "
                      "Status, "
                      "`Liczba_Próbek` "
                      "FROM view_trip_dashboard "
                      "WHERE visibility = 'Public' "
                      "ORDER BY Start DESC");
        break;
    default:
        break;
    }


    query.bindValue(":uId", m_currentUserId);

    if (!query.exec()) {
        // Handle error (qDebug or QMessageBox)
        qCritical() << "SQL Error:" << query.lastError().text(); // Senior Dev Tip: Always log the error text!
    }

    // Konfiguracja tabeli (żeby klikało się przyjemnie)
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Zaznaczaj całe wiersze
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // Tylko jeden rejs na raz
    ui->tableView->setAlternatingRowColors(true); // Paski dla czytelności

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
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (!select) {
        QMessageBox::critical(this, "Error", "No selection model available.");
        return;
    }

    QModelIndexList rows = select->selectedRows(0);

    if (rows.isEmpty()) {
        QMessageBox::information(this, "Failed", "Please select a trip first.");
        return;
    }
        int tripId = rows.first().data().toInt();

        TripDataWindow *window = new TripDataWindow(this, tripId, m_db, 0);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
}


void TripViewWindow::on_ShowComplexDataBTN_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (!select) {
        QMessageBox::critical(this, "Error", "No selection model available.");
        return;
    }

    QModelIndexList rows = select->selectedRows(0); // column 0 = id_trip

    if (rows.isEmpty()) {
        QMessageBox::information(this, "Failed", "Please select a trip first.");
        return;
    }

    QModelIndex index = rows.first();
    int tripId = index.data().toInt();

    TripDataWindow *window = new TripDataWindow(this, tripId, m_db, 1);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}



