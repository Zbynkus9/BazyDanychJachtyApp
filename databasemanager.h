#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>
#include <QSqlError>
#include <QDir>
#include <QDateTime>
#include <QSqlQueryModel>

class DatabaseManager {
public:
    DatabaseManager();
    bool connect();
    QSqlDatabase db() const;

private:
    QString host;
    int port;
    QString databaseName;
    QString username;
    QString password;

    QSqlDatabase m_db;
    bool loadConfig(const QString& path);
};

#endif // DATABASEMANAGER_H
