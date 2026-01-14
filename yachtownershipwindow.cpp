#include "yachtownershipwindow.h"
#include "ui_yachtownershipwindow.h"

YachtOwnershipWindow::YachtOwnershipWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::YachtOwnershipWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
{
    ui->setupUi(this);
    // 1. Create the model (Heap allocation, pass 'this' as parent so it auto-deletes)
    QSqlQueryModel *model = new QSqlQueryModel(this);

    // 2. Prepare the complex query
    QSqlQuery query(m_db);
    query.prepare("SELECT yachts.name AS 'Yacht Name', users.username, yacht_ownership.ownership_flag, yacht_ownership.update_time FROM yacht_ownership JOIN yachts ON yacht_ownership.yacht_id = yachts.id_yacht JOIN users ON yacht_ownership.owner_id = users.id_user WHERE yacht_ownership.yacht_id IN ( SELECT yacht_id FROM yacht_ownership WHERE owner_id = :currentUserId ) ORDER BY yachts.name");
    query.bindValue(":currentUserId", m_currentUserId);

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
}

YachtOwnershipWindow::~YachtOwnershipWindow()
{
    delete ui;
}

void YachtOwnershipWindow::on_CloseBTN_clicked()
{
    YachtOwnershipWindow::done(0);
}


void YachtOwnershipWindow::on_AddOwnerBTN_clicked()
{
    /*
        Otwiera QDialog z wyborem jachtów których jesteśmy obecnym właścicielem oraz wpisaniem Username który jest sprawdzany czy istnieje i czy już nie jest obecnym właścicielem a następnie dodaje odpowiedni rekord właściciela
    */
}


void YachtOwnershipWindow::on_DeleteOwnerBTN_clicked()
{
    /*
        Otwiera QDialog z wyborem jachtów których jesteśmy obecnym właścicielem oraz z wyborem pasujących innych właścicieli wybranego jachtu (poza głóWnym właścicielem) i potwierdzeniem i anulacją usunięcia (aka zmiany na przeszłego właściciela)
    */
}


void YachtOwnershipWindow::on_ChangeOwnerBTN_clicked()
{
    /*
        Otwiera QDialog z wyborem jachtów których jesteśmy obecnym głównym właścicielem oraz wpisaniem Username który jest sprawdzany czy istnieje.
        Po potwierdzeniu i poprawności Username, aktualizowani są wszyscy właściciele razem z głównym wybranego jachtu na przeszłych właścicieli i wpisany Username jest ustawiany jako główny właściciel
    */
}

