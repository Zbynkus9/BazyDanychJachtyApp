#include "tripviewwindow.h"
#include "ui_tripviewwindow.h"
#include "tripdatawindow.h"
#include <QMessageBox>

TripViewWindow::TripViewWindow(QWidget *parent, int userId, QSqlDatabase db, int type)
    : QDialog(parent)
    , ui(new Ui::TripViewWindow)
    , m_currentUserId(userId)
    , m_db(db)
    , view_type(type)
{
    ui->setupUi(this);
    QSqlQueryModel *model = new QSqlQueryModel(this);

    qDebug() << "Window type: " << view_type;

    QSqlQuery query(m_db);
    // Zależnie od view type różne wersje
    switch (view_type) {
    // Widok tylko swoich rejsów (trips.user_id == m_currentUserId)
    case 0:
        qDebug() << "Case 0";
        query.prepare("SELECT "
                  "id_trip, "
                  "`Nazwa Rejsu`, "
                  "Jacht, "
                  "Start, "
                  "Status, "
                  "`Minuty_Trwania`, "
                  "`Liczba_Próbek` "
                  "FROM view_trip_dashboard "
                  "WHERE id_kapitana = :uId "  // Filtrujemy tylko nasze rejsy
                  "ORDER BY Start DESC");
        break;
    // Widok swoich rejsów + rejsów z przypisanym jachtem
    case 1:
        qDebug() << "Case 1";
        query.prepare("SELECT "
                      "id_trip, "
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
    // Widok tylko rejsów udostępnionych przez innych (sailing_trips.visibility == Public)
    case 2:
        qDebug() << "Case 2";
        query.prepare("SELECT "
                      "id_trip, "
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
        qCritical() << "SQL Error:" << query.lastError().text();
    }

    // Tabela wyboru rejsu
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Zaznaczamy cały wiersz
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // Tylko jeden rejs na raz
    ui->tableView->setAlternatingRowColors(true);

    model->setQuery(std::move(query));

    ui->tableView->setModel(model);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnHidden(0, true);
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



