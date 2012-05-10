/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQMLENGINEDEBUGSERVICE_P_H
#define QQMLENGINEDEBUGSERVICE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qqmldebugservice_p.h>

#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QQmlEngine;
class QQmlContext;
class QQmlWatcher;
class QDataStream;
class QQmlDebugStatesDelegate;

class Q_QML_PRIVATE_EXPORT QQmlEngineDebugService : public QQmlDebugService
{
    Q_OBJECT
public:
    QQmlEngineDebugService(QObject * = 0);
    ~QQmlEngineDebugService();

    struct QQmlObjectData {
        QUrl url;
        int lineNumber;
        int columnNumber;
        QString idString;
        QString objectName;
        QString objectType;
        int objectId;
        int contextId;
        int parentId;
    };

    struct QQmlObjectProperty {
        enum Type { Unknown, Basic, Object, List, SignalProperty };
        Type type;
        QString name;
        QVariant value;
        QString valueTypeName;
        QString binding;
        bool hasNotifySignal;
    };

    void addEngine(QQmlEngine *);
    void remEngine(QQmlEngine *);
    void objectCreated(QQmlEngine *, QObject *);

    void setStatesDelegate(QQmlDebugStatesDelegate *);

    static QQmlEngineDebugService *instance();

protected:
    virtual void messageReceived(const QByteArray &);

private Q_SLOTS:
    void processMessage(const QByteArray &msg);
    void propertyChanged(int id, int objectId, const QMetaProperty &property, const QVariant &value);

private:
    void prepareDeferredObjects(QObject *);
    void buildObjectList(QDataStream &, QQmlContext *,
                         const QList<QPointer<QObject> > &instances);
    void buildObjectDump(QDataStream &, QObject *, bool, bool);
    void buildStatesList(bool cleanList, const QList<QPointer<QObject> > &instances);
    QQmlObjectData objectData(QObject *);
    QQmlObjectProperty propertyData(QObject *, int);
    QVariant valueContents(const QVariant &defaultValue) const;
    bool setBinding(int objectId, const QString &propertyName, const QVariant &expression, bool isLiteralValue, QString filename = QString(), int line = -1, int column = 0);
    bool resetBinding(int objectId, const QString &propertyName);
    bool setMethodBody(int objectId, const QString &method, const QString &body);
    void storeObjectIds(QObject *co);

    QList<QQmlEngine *> m_engines;
    QQmlWatcher *m_watch;
    QQmlDebugStatesDelegate *m_statesDelegate;
};
Q_QML_PRIVATE_EXPORT QDataStream &operator<<(QDataStream &, const QQmlEngineDebugService::QQmlObjectData &);
Q_QML_PRIVATE_EXPORT QDataStream &operator>>(QDataStream &, QQmlEngineDebugService::QQmlObjectData &);
Q_QML_PRIVATE_EXPORT QDataStream &operator<<(QDataStream &, const QQmlEngineDebugService::QQmlObjectProperty &);
Q_QML_PRIVATE_EXPORT QDataStream &operator>>(QDataStream &, QQmlEngineDebugService::QQmlObjectProperty &);

QT_END_NAMESPACE

#endif // QQMLENGINEDEBUGSERVICE_P_H

