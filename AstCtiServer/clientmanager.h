/* Copyright (C) 2007-2009 Bruno Salzano
 * http://centralino-voip.brunosalzano.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * This exception applies only to the code released under the name GNU
 * AstCtiServer.  If you copy code from other releases into a copy of GNU
 * AstCtiServer, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for AstCtiServer, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 */

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QTcpSocket>
#include <QTimer>

#include "astctioperator.h"
#include "astctiseat.h"

class ClientManager : public  QObject
{
    Q_OBJECT

public:
	explicit ClientManager(QTcpSocket* const socket, QObject* parent = 0);
    ~ClientManager();

	QTcpSocket* const      getSocket() const;

	AstCtiOperator* const  getActiveOperator() const;
	void                   setActiveOperator(AstCtiOperator* const op);

	AstCtiSeat* const      getActiveSeat() const;
	void                   setActiveSeat(AstCtiSeat* const seat);

	const QString&         getClientOperatingSystem() const;
	void                   setClientOperatingSystem(const QString& os);

	const QString&         getCtiUsername() const;
	void                   setCtiUsername(const QString& username);

	const bool             getIsAuthenticated() const;
	void                   setIsAuthenticated(const bool isAuthenticated);

	const int              getCompressionLevel() const;
	void                   setCompressionLevel(const int compressionLevel);

	const QString&         getLocalIdentifier() const;
	void                   setLocalIdentifier(const QString& localIdentifier);

	void                   setIdleTimeout(const int miliseconds);
	void                   resetIdleTimer();

	// We expose this member directly for convenience
	quint16                blockSize;

private:
	Q_DISABLE_COPY(ClientManager)

	void              idleTimerElapsed();

	QTcpSocket*       socket;
	AstCtiOperator*   activeOperator;
	AstCtiSeat*       activeSeat;
	QString           clientOperatingSystem;
	QString           ctiUsername;
	bool              isAuthenticated;
	int               compressionLevel;
	QString           localIdentifier;
	QTimer            idleTimer;
};

#endif // CLIENTMANAGER_H
