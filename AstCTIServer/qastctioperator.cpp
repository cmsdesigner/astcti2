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
 * AstCTIServer.  If you copy code from other releases into a copy of GNU
 * AstCTIServer, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for AstCTIServer, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 */

#include <QtSql>

#include "qastctioperator.h"


QAstCtiOperator::QAstCtiOperator(const int &id)
        : ID_OPERATOR(id), FULL_NAME(""), USER_NAME(""),
        PASS_WORD(""), LAST_SEAT(0), BEGIN_IN_PAUSE(false),
        ENABLED(false), lastSeat(0)
{
    // Let's connect our signals
    connect(this, SIGNAL(LoadComplete(const bool&)), this, SLOT(LoadSeat(const bool&)));
    connect(this, SIGNAL(UpdateComplete(const bool&)), this, SLOT(LoadSeat(const bool&)));
}

bool QAstCtiOperator::Load()
{
    bool retVal = false;
    QSqlDatabase db = QSqlDatabase::database("sqlitedb");
    QSqlQuery query(db);
    query.prepare("SELECT * FROM operators WHERE ID_OPERATOR=:id");
    query.bindValue(":id", this->ID_OPERATOR);
    retVal = query.exec();
    if ( (retVal) &  (query.first()) )
    {
        this->FULL_NAME = query.value(1).toString();
        this->USER_NAME = query.value(2).toString();
        this->PASS_WORD = query.value(3).toString();
        this->LAST_SEAT = query.value(4).toInt(0);
        this->BEGIN_IN_PAUSE = query.value(5).toBool();
        this->ENABLED = query.value(6).toBool();

        query.finish();
    }
    query.clear();

    emit this->LoadComplete(retVal);
    return retVal;
}

void QAstCtiOperator::LoadSeat(const bool &bMayLoad)
{
    if (!bMayLoad) return;

    if (this->LAST_SEAT > 0)
    {
        this->lastSeat = new QAstCTISeat(this->LAST_SEAT);
        this->lastSeat->Load();
    }
}

bool QAstCtiOperator::Save()
{
    bool retVal = false;
    QSqlDatabase db = QSqlDatabase::database("sqlitedb");

    if (db.driver()->hasFeature(QSqlDriver::Transactions))
        db.transaction();

    QSqlQuery query(db);

    // For now we need to update just only the field last_seat in the operators table
    query.prepare("UPDATE operators SET LAST_SEAT=:seat WHERE ID_OPERATOR=:id");
    query.bindValue(":seat", this->LAST_SEAT);
    query.bindValue(":id", this->ID_OPERATOR);
    retVal = query.exec();

    if (db.driver()->hasFeature(QSqlDriver::Transactions))
        retVal ? db.commit() :  db.rollback();

    query.clear();

    emit this->UpdateComplete(retVal);
    return retVal;
}

int  QAstCtiOperator::getIdOperator()
{
    return this->ID_OPERATOR;
}

QString  QAstCtiOperator::getUsername()
{
    return this->USER_NAME;
}

QString  QAstCtiOperator::getFullName()
{
    return this->FULL_NAME;
}

QString  QAstCtiOperator::getPassword()
{
    return this->PASS_WORD;
}

int QAstCtiOperator::getLastSeat()
{
    return this->LAST_SEAT;
}

QAstCTISeat* QAstCtiOperator::getSeat()
{
    return this->lastSeat;
}


void QAstCtiOperator::setLastSeat(const int &newSeat)
{
    this->LAST_SEAT = newSeat;
}

bool QAstCtiOperator::getBeginInPause()
{
    return this->BEGIN_IN_PAUSE;
}

bool QAstCtiOperator::getEnabled()
{
    return this->ENABLED;
}

