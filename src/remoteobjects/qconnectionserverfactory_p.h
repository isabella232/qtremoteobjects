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

#ifndef QCONNECTIONSERVERFACTORY_P_H
#define QCONNECTIONSERVERFACTORY_P_H

#include "qconnectionabstractserver_p.h"

#include <QLocalServer>
#include <QTcpServer>
#include <QUrl>

QT_BEGIN_NAMESPACE

class LocalServerIo : public ServerIoDevice
{
public:
    explicit LocalServerIo(QLocalSocket *conn, QObject *parent = Q_NULLPTR);

    QIODevice *connection() const Q_DECL_OVERRIDE;
protected:
    void doClose() Q_DECL_OVERRIDE;

private:
    QLocalSocket *m_connection;
};


class TcpServerIo : public ServerIoDevice
{
public:
    explicit TcpServerIo(QTcpSocket *conn, QObject *parent = Q_NULLPTR);

    QIODevice *connection() const Q_DECL_OVERRIDE;
protected:
    void doClose() Q_DECL_OVERRIDE;

private:
    QTcpSocket *m_connection;
};


class LocalServerImpl : public QConnectionAbstractServer
{
    Q_OBJECT
    Q_DISABLE_COPY(LocalServerImpl)

public:
    explicit LocalServerImpl(QObject *parent);
    ~LocalServerImpl();

    bool hasPendingConnections() const Q_DECL_OVERRIDE;
    ServerIoDevice *_nextPendingConnection() Q_DECL_OVERRIDE;
    QUrl address() const Q_DECL_OVERRIDE;
    bool listen(const QUrl &address) Q_DECL_OVERRIDE;
    QAbstractSocket::SocketError serverError() const Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;

private:
    QLocalServer m_server;
};


class TcpServerImpl : public QConnectionAbstractServer
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpServerImpl)

public:
    explicit TcpServerImpl(QObject *parent);
    ~TcpServerImpl();

    bool hasPendingConnections() const Q_DECL_OVERRIDE;
    ServerIoDevice *_nextPendingConnection() Q_DECL_OVERRIDE;
    QUrl address() const Q_DECL_OVERRIDE;
    bool listen(const QUrl &address) Q_DECL_OVERRIDE;
    QAbstractSocket::SocketError serverError() const Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;

private:
    QTcpServer m_server;
    QUrl m_originalUrl; // necessary because of a QHostAddress bug
};

struct QtROServerFactory {
    static QConnectionAbstractServer *create(const QUrl &url, QObject *parent = Q_NULLPTR) { // creates an object from a string
        Creators_t::const_iterator iter = static_creators().constFind(url.scheme());
        return iter == static_creators().constEnd() ? 0 : (*iter)(parent); // if found, execute the creator function pointer
    }

private:
    typedef QConnectionAbstractServer *Creator_t(QObject *); // function pointer to create QConnectionAbstractServer
    typedef QHash<QString, Creator_t*> Creators_t; // map from id to creator
    static Creators_t& static_creators() { static Creators_t s_creators; return s_creators; } // static instance of map
    template<class T = int> struct Register {
        static QConnectionAbstractServer *create(QObject *parent) { return new T(parent); }
        static Creator_t *init_creator(const QString &id) { return static_creators()[id] = create; }
        static Creator_t *creator;
    };
};

#define REGISTER_QTRO_SERVER(T, STR) template<> QtROServerFactory::Creator_t* QtROServerFactory::Register<T>::creator = QtROServerFactory::Register<T>::init_creator(QLatin1String(STR))

QT_END_NAMESPACE

#endif
