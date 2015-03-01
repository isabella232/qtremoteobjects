/****************************************************************************
**
** Copyright (C) 2014 Ford Motor Company
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QREMOTEOBJECTSOURCEIO_P_H
#define QREMOTEOBJECTSOURCEIO_P_H

#include "qconnectionserverfactory_p.h"
#include "qtremoteobjectglobal.h"

#include <QIODevice>
#include <QScopedPointer>
#include <QSignalMapper>

QT_BEGIN_NAMESPACE

class QRemoteObjectSourcePrivate;
class SourceApiMap;

class QRemoteObjectSourceIo : public QObject
{
    Q_OBJECT
public:
    explicit QRemoteObjectSourceIo(const QUrl &address);
    ~QRemoteObjectSourceIo();

    bool enableRemoting(QObject *object, const QMetaObject *meta, const QString &name);
    bool enableRemoting(QObject *object, const SourceApiMap *api, QObject *adapter = Q_NULLPTR);
    bool disableRemoting(QRemoteObjectSourcePrivate *pp);

    QUrl serverAddress() const;

public Q_SLOTS:
    void handleConnection();
    void onServerDisconnect(QObject *obj = Q_NULLPTR);
    void onServerRead(QObject *obj);
    void clearRemoteObjectSource(const QString &name);

Q_SIGNALS:
    void remoteObjectAdded(const QRemoteObjectSourceLocation &);
    void remoteObjectRemoved(const QRemoteObjectSourceLocation &);
    void serverRemoved(const QUrl& url);

private:
    friend class QRemoteObjectNodePrivate;
    QHash<QIODevice*, quint32> m_readSize;
    QConnectionServerFactory m_factory;
    QSet<ServerIoDevice*> m_connections;
    QMap<QString, QRemoteObjectSourcePrivate*> m_remoteObjects;
    QSignalMapper m_serverDelete;
    QSignalMapper m_serverRead;
    QSignalMapper m_remoteObjectDestroyed;
    QHash<ServerIoDevice*, QUrl> m_registryMapping;
    QScopedPointer<QConnectionAbstractServer> m_server;
};

QT_END_NAMESPACE

#endif
