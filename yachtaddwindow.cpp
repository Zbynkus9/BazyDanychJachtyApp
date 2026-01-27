#include "yachtaddwindow.h"
#include "ui_yachtaddwindow.h"
#include <QMessageBox>
#include <qvalidator.h>

YachtAddWindow::YachtAddWindow(QWidget *parent, int userId, QSqlDatabase db)
    : QDialog(parent)
    , ui(new Ui::YachtAddWindow)
    , m_currentUserId(userId) // Save the ID
    , m_db(db)                // Save the Connection
{
    ui->setupUi(this);

    ui->LengthText->setValidator(new QDoubleValidator(0, 100, 2, this));
    ui->Sail_areaText->setValidator(new QDoubleValidator(0, 100, 2, this));
    ui->Draft_maximumText->setValidator(new QDoubleValidator(0, 100, 2, this));
    ui->Draft_minimumText->setValidator(new QDoubleValidator(0, 100, 2, this));

    locale = QLocale::system();
}

YachtAddWindow::~YachtAddWindow()
{
    delete ui;
}

// void YachtAddWindow::on_buttonBox_accepted()
// {
//     // get name and class
//     // prepere and try to add yacht
//     // get added yacht id
//     // prepere and try to add ownership for that yacht for current user
//     QString yName = ui->NameText->text();
//     QString yClass = ui->ClassText->text();
//     QString yLength = ui ->LengthText->text();
//     bool ok;
//     // 2. Próba konwersji tekstu na float zgodnie z ustawieniami systemu
//     float yLengthF = locale.toFloat(yLength, &ok);

//     // 3. Sprawdzenie czy się udało
//     if (!ok) {
//         // Jeśli systemowa konwersja zawiodła, spróbujmy "standardowej C++" (z kropką)
//         // To zabezpieczenie na wypadek, gdyby ktoś w Polskim systemie wpisał jednak kropkę
//         yLengthF = yLength.toFloat(&ok);

//         if (!ok) {
//             QMessageBox::warning(this, "Błąd", "Niepoprawny format liczby w polu Długość!");
//             return;
//         }
//     }
//     QString ySail_area = ui -> Sail_areaText->text();
//     ok = false;
//     // 2. Próba konwersji tekstu na float zgodnie z ustawieniami systemu
//     float ySail_areaF = locale.toFloat(ySail_area, &ok);

//     // 3. Sprawdzenie czy się udało
//     if (!ok) {
//         // Jeśli systemowa konwersja zawiodła, spróbujmy "standardowej C++" (z kropką)
//         // To zabezpieczenie na wypadek, gdyby ktoś w Polskim systemie wpisał jednak kropkę
//         ySail_areaF = ySail_area.toFloat(&ok);

//         if (!ok) {
//             QMessageBox::warning(this, "Błąd", "Niepoprawny format liczby w polu Długość!");
//             return;
//         }
//     }
//     QString yDraft_minimum = ui -> Draft_minimumText->text(); // zanurzenie bez miecza
//     ok = false;
//     // 2. Próba konwersji tekstu na float zgodnie z ustawieniami systemu
//     float yDraft_minimumF = locale.toFloat(yDraft_minimum, &ok);

//     // 3. Sprawdzenie czy się udało
//     if (!ok) {
//         // Jeśli systemowa konwersja zawiodła, spróbujmy "standardowej C++" (z kropką)
//         // To zabezpieczenie na wypadek, gdyby ktoś w Polskim systemie wpisał jednak kropkę
//         yDraft_minimumF = yDraft_minimum.toFloat(&ok);

//         if (!ok) {
//             QMessageBox::warning(this, "Błąd", "Niepoprawny format liczby w polu Długość!");
//             return;
//         }
//     }
//     QString yDraft_maximum = ui -> Draft_maximumText->text(); // zanurzenie z mieczem
//     ok = false;
//     // 2. Próba konwersji tekstu na float zgodnie z ustawieniami systemu
//     float yDraft_maximumF = locale.toFloat(yDraft_maximum, &ok);

//     // 3. Sprawdzenie czy się udało
//     if (!ok) {
//         // Jeśli systemowa konwersja zawiodła, spróbujmy "standardowej C++" (z kropką)
//         // To zabezpieczenie na wypadek, gdyby ktoś w Polskim systemie wpisał jednak kropkę
//         yDraft_maximumF = yDraft_maximum.toFloat(&ok);

//         if (!ok) {
//             QMessageBox::warning(this, "Błąd", "Niepoprawny format liczby w polu Długość!");
//             return;
//         }
//     }
//     if (yName.length() == 0 || yClass.length() == 0 || yLength.length() == 0 || ySail_area.length() == 0 || yDraft_minimum.length() == 0 || yDraft_maximum.length() == 0) { QMessageBox::information(this, "Failed", "Enter Yacht Data"); }

//     else {
//         // ---------------- Dodać więcej danych do zapisywania dla dodawanych jachtów -----------------


//         m_db.transaction();

//         QSqlQuery qYacht(m_db);
//         qYacht.prepare("INSERT INTO yachts (name, class, length, sail_area, draft_minimum, draft_maximum) VALUES (:yName, :yClass, :yLength, :ySail_area, :yDraft_minimum, :yDraft_maximum)");

//         qYacht.bindValue(":yName", yName);
//         qYacht.bindValue(":yClass", yClass);
//         qYacht.bindValue(":yLength", yLengthF);
//         qYacht.bindValue(":ySail_area", ySail_areaF);
//         qYacht.bindValue(":yDraft_minimum", yDraft_minimumF);
//         qYacht.bindValue(":yDraft_maximum", yDraft_maximumF);

//         if (!qYacht.exec()) {
//             m_db.rollback();

//             QString errorText =
//                 "Failed to add yacht:\n\n" +
//                 qYacht.lastError().text();

//             qDebug() << errorText;

//             QMessageBox::critical(this, "Database Error", errorText);
//             return;
//         }


//         int newYachtId = qYacht.lastInsertId().toInt();

//         QSqlQuery qOwner(m_db);
//         qOwner.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) VALUES (:yId, :oId, :oFlag, NOW())");
//         qOwner.bindValue(":yId", newYachtId);
//         qOwner.bindValue(":oId", m_currentUserId);
//         qOwner.bindValue(":oFlag", "Current");

//         if (!qOwner.exec()) {
//             m_db.rollback();
//             // Show Error
//             QMessageBox::information(this, "Failed", "Failed to add record to database");
//             return;
//         }

//         m_db.commit();
//         QMessageBox::information(this, "Success", "Successfuly added Yacht and Ownership");
//         return;
//     }

// }

void YachtAddWindow::on_buttonBox_accepted()
{
    // 1. Pobierz surowe teksty
    QString sName = ui->NameText->text();
    QString sClass = ui->ClassText->text();
    QString sLength = ui->LengthText->text();
    QString sSail = ui->Sail_areaText->text();
    QString sDraftMin = ui->Draft_minimumText->text();
    QString sDraftMax = ui->Draft_maximumText->text();

    // 2. Walidacja pustych pól
    if (sName.isEmpty() || sLength.isEmpty() /* ... dodaj resztę */) {
        QMessageBox::warning(this, "Błąd", "Wypełnij wszystkie pola!");
        return;
    }

    // 3. PANCERNA KONWERSJA (Lambda helper)
    // To zamienia "12,5" na "12.5" i "12.5" zostawia jako "12.5"
    bool conversionOk = true;
    auto toFloat = [&](QString str) -> float {
        str.replace(",", "."); // Zamień przecinek na kropkę
        bool ok;
        float val = str.toFloat(&ok);
        if (!ok) conversionOk = false; // Ustaw flagę błędu
        return val;
    };

    // Konwertujemy wszystko na liczby PRZED zapytaniem
    float valLength = toFloat(sLength);
    float valSail = toFloat(sSail);
    float valDraftMin = toFloat(sDraftMin);
    float valDraftMax = toFloat(sDraftMax);

    // Sprawdź czy konwersja się udała
    if (!conversionOk) {
        QMessageBox::warning(this, "Błąd formatu", "Wpisz poprawne liczby (np. 12.5 lub 12,5)!");
        return;
    }

    // 4. Transakcja i Zapytanie
    m_db.transaction();

    QSqlQuery qYacht(m_db);
    qYacht.prepare("INSERT INTO yachts (name, class, length, sail_area, draft_minimum, draft_maximum) "
                   "VALUES (:n, :c, :l, :s, :dmin, :dmax)");

    // Bindowanie
    qYacht.bindValue(":n", sName);
    qYacht.bindValue(":c", sClass);

    // UWAGA: Tu musisz podać zmienne float (val...), a nie QStringi (s...)!
    qYacht.bindValue(":l", valLength);
    qYacht.bindValue(":s", valSail);
    qYacht.bindValue(":dmin", valDraftMin);
    qYacht.bindValue(":dmax", valDraftMax);

    // 5. Wykonanie i Debugowanie błędów
    if (!qYacht.exec()) {
        m_db.rollback(); // Cofamy transakcję

        // WYPISZ DOKŁADNY BŁĄD
        QString dbErr = qYacht.lastError().text();
        QString driverErr = qYacht.lastError().driverText();

        qDebug() << "SQL Error:" << dbErr;
        qDebug() << "Driver Error:" << driverErr;

        QMessageBox::critical(this, "Database Error",
                              "Nie udało się dodać jachtu.\n\nBłąd SQL: " + dbErr);
        return;
    }

    // 1. SPRAWDZENIE ID JACHTU
    // Czy na pewno dostaliśmy poprawne ID? Jeśli to jest 0 lub invalid, INSERT ownership padnie.
    int newYachtId = qYacht.lastInsertId().toInt();

    // Sprawdź w konsoli co tu wyszło
    qDebug() << "DEBUG: Nowe ID Jachtu:" << newYachtId;
    qDebug() << "DEBUG: ID Usera:" << m_currentUserId;

    if (newYachtId == 0) {
        m_db.rollback();
        QMessageBox::critical(this, "Błąd", "Nie udało się pobrać ID dodanego jachtu (lastInsertId zwróciło 0).");
        return;
    }

    // 2. PRZYGOTOWANIE ZAPYTANIA OWNERSHIP
    QSqlQuery qOwner(m_db);
    qOwner.prepare("INSERT INTO yacht_ownership (yacht_id, owner_id, ownership_flag, update_time) "
                   "VALUES (:yId, :oId, :oFlag, NOW())");

    qOwner.bindValue(":yId", newYachtId);
    qOwner.bindValue(":oId", m_currentUserId);
    qOwner.bindValue(":oFlag", "Current");

    // 3. WYKONANIE Z PEŁNĄ DIAGNOSTYKĄ
    if (!qOwner.exec()) {
        m_db.rollback();

        // Pobierz błąd z zapytania
        QString queryErr = qOwner.lastError().text();

        // Pobierz błąd z SAMEJ BAZY DANYCH (często tu kryje się prawda przy pustym queryErr)
        QString dbErr = m_db.lastError().text();

        qDebug() << "--- BŁĄD INSERT OWNERSHIP ---";
        qDebug() << "Query Error:" << queryErr;
        qDebug() << "Database Error:" << dbErr;
        qDebug() << "Driver Text:" << qOwner.lastError().driverText();

        QString fullError = "Query Error: " + queryErr + "\nDB Error: " + dbErr;
        QMessageBox::critical(this, "Failed", "Nie udało się przypisać właściciela:\n" + fullError);
        return;
    }

    // Jeśli przeszło:
    m_db.commit();
    QMessageBox::information(this, "Success", "Successfully added Yacht and Ownership");
    this->accept();

    m_db.commit();
    QMessageBox::information(this, "Sukces", "Dodano jacht!");
    this->accept();
}


void YachtAddWindow::on_buttonBox_rejected()
{
    // just close and do nothing
    return;
}

