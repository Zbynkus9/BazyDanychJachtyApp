#pragma once

#include <QSqlDatabase>
#include <QString>

class SignalKImporter
{
public:
    static bool importDirectory(
        QSqlDatabase& db,
        int tripId,
        const QString& directoryPath,
        QString& errorMessage
        );
};
