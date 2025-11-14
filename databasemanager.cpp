#include "databasemanager.h"

DatabaseManager::DatabaseManager() {
    loadConfig(QDir::currentPath() + "/../../config.json");
}

bool DatabaseManager::loadConfig(const QString& path) {
    QFile file(path);
    if (!file.exists()){ qWarning() << "No file: " << path;}
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open DB config file:" << path;
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();

    host = obj["host"].toString();
    port = obj["port"].toInt();
    databaseName = obj["database"].toString();
    username = obj["username"].toString();
    password = obj["password"].toString();

    file.close();
    return true;
}

bool DatabaseManager::connect() {
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(username);
    m_db.setPassword(password);


    if (!m_db.open()) {
        qCritical() << "Database connection failed:" << m_db.lastError().text();
        return false;
    }

    qInfo() << "Connected to MySQL successfully!";
    return true;
}

QSqlDatabase DatabaseManager::db() const {
    return m_db;
}
