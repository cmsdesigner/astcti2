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
#include <QDebug>

#include "qastctivariable.h"

QAstCTIVariable::QAstCTIVariable(const int& id, QObject* parent) :
        QObject(parent), ID_VARIABLE(id), ID_SERVICE(0), VARNAME("")
{

}

QAstCTIVariable::~QAstCTIVariable()
{
    qDebug() << "In QAstCTIVariable::~QAstCTIVariable()";
}

bool QAstCTIVariable::load()
{
    bool retVal = false;
    QSqlDatabase db = QSqlDatabase::database("sqlitedb");
    QSqlQuery query(db);
    query.prepare("SELECT * FROM services_variables WHERE ID_VARIABLE=:id");
    query.bindValue(":id", this->ID_VARIABLE);
    retVal = query.exec();
    if ( (retVal) &  (query.first()) )
    {
        this->ID_SERVICE = query.value(1).toInt(0);
        this->VARNAME = query.value(2).toString();
        query.finish();
    }
    query.clear();

    emit this->load_complete(retVal);
    return retVal;
}

int QAstCTIVariable::get_id_variable()
{
    return this->ID_VARIABLE;
}

int QAstCTIVariable::get_id_service()
{
    return this->ID_SERVICE;
}

QString QAstCTIVariable::get_var_name()
{
    return this->VARNAME;
}