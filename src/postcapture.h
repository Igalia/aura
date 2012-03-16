/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef POSTCAPTURE_H
#define POSTCAPTURE_H

#include <QObject>
#include <QDeclarativeItem>
#include <QDeclarativeEngine>

class PostCapture : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QString file
               READ file
               WRITE setFile
               NOTIFY fileChanged)

 public:
    PostCapture(QDeclarativeItem *parent = 0);
    ~PostCapture();
    QString file() { return m_file; };

 public slots:
    void setFile(const QString &file);

 signals:
    void fileChanged(const QString &file);
    void clicked();

 private slots:
    void show();

 private:
    QDeclarativeEngine m_engine;
    QString m_file;
};
#endif
