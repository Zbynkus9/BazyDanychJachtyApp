#include "signalkimporter.h"

#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>

bool SignalKImporter::importDirectory(
    QSqlDatabase& db,
    int tripId,
    const QString& directoryPath,
    QString& errorMessage
    ) {
    QDir dir(directoryPath);
    if (!dir.exists()) {
        errorMessage = "Directory does not exist";
        return false;
    }

    QStringList filters;
    filters << "*.json";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    if (files.isEmpty()) {
        errorMessage = "No JSON files found";
        return false;
    }

    if (!db.transaction()) {
        errorMessage = "Failed to start DB transaction";
        return false;
    }

    for (const QFileInfo& fileInfo : files) {

        // ---------- Read file ----------
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) {
            errorMessage = "Cannot open file: " + fileInfo.fileName();
            db.rollback();
            return false;
        }

        QByteArray rawJson = file.readAll();
        file.close();

        // ---------- Parse JSON ----------
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(rawJson, &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            errorMessage = "Invalid JSON in file: " + fileInfo.fileName();
            db.rollback();
            return false;
        }

        QJsonObject root = doc.object();
        QJsonObject vessels = root["vessels"].toObject();
        QJsonObject self = vessels["self"].toObject();

        if (self.isEmpty()) {
            errorMessage = "Missing vessels.self in file: " + fileInfo.fileName();
            db.rollback();
            return false;
        }

        // ---------- Navigation ----------
        QJsonObject nav = self["navigation"].toObject();
        QJsonObject pos =
            nav["position"].toObject()["value"].toObject();

        double latitude  = pos["latitude"].toDouble(0.0);
        double longitude = pos["longitude"].toDouble(0.0);

        double speed =
            nav["speedOverGround"].toObject()["value"].toDouble(0.0);

        QString tsString =
            nav["datetime"].toObject()["value"].toString();

        QDateTime ts =
            QDateTime::fromString(tsString, Qt::ISODate);

        if (!ts.isValid())
            ts = QDateTime::currentDateTimeUtc();

        // ---------- Wind ----------
        double windSpeed =
            self["environment"].toObject()
                ["wind"].toObject()
                        ["speedApparent"].toObject()
                        ["value"].toDouble(0.0);

        // ---------- Insert SIMPLE ----------
        QSqlQuery qSimple(db);
        qSimple.prepare(R"(
            INSERT INTO simple_sensors_records
            (trip_id, timestamp, longitude, latitude, speed, wind_speed)
            VALUES (:tripId, :ts, :lon, :lat, :spd, :wspd)
        )");

        qSimple.bindValue(":tripId", tripId);
        qSimple.bindValue(":ts", ts);
        qSimple.bindValue(":lon", longitude);
        qSimple.bindValue(":lat", latitude);
        qSimple.bindValue(":spd", speed);
        qSimple.bindValue(":wspd", windSpeed);

        if (!qSimple.exec()) {
            errorMessage = qSimple.lastError().text();
            db.rollback();
            return false;
        }

        int sensorDataId = qSimple.lastInsertId().toInt();

        // ---------- Insert FULL ----------
        QSqlQuery qFull(db);
        qFull.prepare(R"(
            INSERT INTO full_sensors_records
            (id_record, data_json)
            VALUES (:rId, :json)
        )");

        QString jsonText = QString::fromUtf8(rawJson);

        qFull.bindValue(":json", jsonText);
        qFull.bindValue(":rId", sensorDataId);

        if (!qFull.exec()) {
            errorMessage = qFull.lastError().text();
            db.rollback();
            return false;
        }
    }

    if (!db.commit()) {
        errorMessage = "Commit failed";
        return false;
    }

    return true;
}
