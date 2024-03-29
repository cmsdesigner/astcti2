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

#ifndef ASTCTIOPERATOR_H
#define ASTCTIOPERATOR_H

#include <QObject>
#include <QHash>
#include <QMap>

#include "astctiservice.h"
#include "astctiseat.h"
#include "astctiextension.h"

class AstCtiOperator : public QObject
{

    Q_OBJECT

public:
	explicit AstCtiOperator(int id, const QString& fullName, const QString& username,
							const QString& password, const bool beginInPause, const int seatID,
							const bool canMonitor, const bool canAlterSpeedDials,
							const bool canRecord, QObject* parent = 0);
	~AstCtiOperator();

	const int                   getId() const;
	const QString&              getFullName() const;
	const QString&              getUsername() const;
	const QString&              getPassword() const;
	const bool                  getBeginInPause() const;
	const int                   getSeatId() const;
	const bool                  getCanMonitor() const;
	const bool                  isCallCenter() const;

	const bool                  changePassword(const QString& newPassword);
	const bool                  checkPassword(const QString& password) const;

	const bool                  loadSpeedDials();
	const bool                  loadServices(const AstCtiServiceHash* const serviceList);
	const AstCtiServiceRevHash& getServices() const;
	const bool                  compareServices(const AstCtiServiceRevHash& newServices) const;

	const QString               toXml(AstCtiSeat* const seat) const;

private:
	Q_DISABLE_COPY(AstCtiOperator)
	int                   id;
	QString               fullName;
	QString               username;
	QString               password;
	bool                  beginInPause;
	int                   seatId;
	bool                  canMonitor;
	bool                  canAlterSpeedDials;
	bool                  canRecord;
	AstCtiSpeedDialMap    speedDials;
	AstCtiServiceRevHash  services;
};

typedef QHash<QString, AstCtiOperator*> AstCtiOperatorHash;

#endif // ASTCTIOPERATOR_H
