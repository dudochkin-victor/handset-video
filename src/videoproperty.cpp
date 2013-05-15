/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QUrl>
#include <QDebug>

#include "videoproperty.h"

static QString getMimeTypeSparql = "SELECT nie:mimeType(?uri) WHERE {  ?uri nie:url '%1' }";

QString VideoProperty::getMimeType(const QString &filePath)
{
    QString sparql;
    QUrl url = QUrl::fromLocalFile(filePath);
    sparql = QString(getMimeTypeSparql).arg(QString(url.toEncoded()));

    qDebug() << "VideoProperty::getMimeType " << sparql;
    QVector<QStringList> result = ::tracker()->rawSparqlQuery(sparql);

    if (result.size())
        return result[0][0];
    else
        return QString("");
}
