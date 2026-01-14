#include "tripviewwindow.h"
#include "ui_tripviewwindow.h"

TripViewWindow::TripViewWindow(QWidget *parent, int userId, QSqlDatabase db, int type)
    : QDialog(parent)
    , ui(new Ui::TripViewWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
    , view_type(type)
{
    ui->setupUi(this);
    // QSqlQuery query(m_db);
    /* Zależnie od view type różne widoki
    0 - widok tylko swoich rejsów (trips.user_id == m_currentUserId)
        query.prepare("SELECT sailing_trips.name AS 'Trip title', sailing_trips.description, users.username, yachts.name AS 'Yacht name', sailing_trips.start_datetime AS 'Trip Date' FROM sailing_trips JOIN users ON sailing_trips.user_id = users.id_user JOIN yachts ON sailing_trips.yacht_id = yachts.id_yacht WHERE sailing_trips.id_trip IN ( SELECT id_trip FROM sailing_trips WHERE user_id = :currentUserId) ORDER BY sailing_trips.start_datetime ASC") ----------------- możliwy błąd pod koniec query

    1 - widok swoich rejsów + rejsów z przypisanym jachtem (trips.yacht_id) którego się jest / było właścicielem (yacht_ownership.ownership_flag == Current || Past)
    Wymyślić gdzie i jak zrobić warunek (WHERE yacht_ownership.ownership_flag == Current || Past) (WHERE sailing_trips.user_id == 'coś z yacht_ownership.owner_id WHERE :currentUserId WHERE yacht_ownership.ownership_flag == Current || Past')
        query.prepare()

    2 - widok tylko rejsów udostępnionych przez innych (sailing_trips.visibility == Public)
    Chyba to samo co w (0 - swoje) ale zamiast sprawdzać user_id sprawdzić sailing_trips.visibility
        query.prepare()

    */
    /*
    query.bindValue(":currentUserId", m_currentUserId);

    */
}

TripViewWindow::~TripViewWindow()
{
    delete ui;
}

void TripViewWindow::on_pushButton_2_clicked()
{
    TripViewWindow::done(0);
}

