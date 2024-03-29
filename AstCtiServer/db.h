/* Copyright (C) 2007-2010 Bruno Salzano
 * http://centralino-voip.brunosalzano.com
 *
 * Copyright (C) 2007-2010 Lumiss d.o.o.
 * http://www.lumiss.hr
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

#ifndef DB_H
#define DB_H

#define dbConnectionName "AstCtiDB"

#include <QtSql>

typedef QList<QVariantList> QVariantTable;

class DB
{
public:
	static bool buildConnection(const QString& host, const quint16 port,
								const QString& user, const QString& pass, const QString& database);
	static bool openConnection();
	static void closeConnection();
	static void destroyConnection();
	static bool beginTransaction();
	static bool commitTransaction();
	static bool rollbackTransaction();
	static QVariant readScalar(const QString& sql, bool* const ok);
	static QVariant readScalar(const QString& sql, const QVariantList& params, bool* const ok);
	static QVariantList readList(const QString& sql, bool* const ok);
	static QVariantList readList(const QString& sql, const QVariantList& params, bool* const ok);
	static QVariantList readRow(const QString& sql, bool* const ok);
	static QVariantList readRow(const QString& sql, const QVariantList& params, bool* const ok);
	static QVariantTable readTable(const QString& sql, bool* const ok);
	static QVariantTable readTable(const QString& sql, const QVariantList& params, bool* const ok);
	static int executeNonQuery(const QString& sql);
	static int executeNonQuery(const QString& sql, const QVariantList& params);

private:
	static QSqlQuery*     execSQL(const QString& sql, const QVariantList& params, bool* const ok);

	static QSqlDatabase*  connection;
	static bool           supportsTransactions;
	static bool           inTransaction;
};

#endif // DB_H
