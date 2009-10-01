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

#ifndef QASTCTIOPERATOR_H
#define QASTCTIOPERATOR_H

#include <QObject>
#include <QCryptographicHash>
#include <QHash>

#include "qastctiseat.h"
#include "qastctioperatorservices.h"

class QAstCTIOperator : public QObject
{

    Q_OBJECT

public:
    QAstCTIOperator(const int& id, QObject* parent);
    ~QAstCTIOperator();

    int                 get_id_operator();
    QString             get_full_name();
    QString             get_user_name();
    QString             get_pass_word();
    int                 get_last_seat();
    void                set_last_seat(const int& newSeat);
    bool                get_begin_in_pause();
    bool                get_enabled();
    bool                check_password(const QString& password);
    QAstCTISeat*        get_seat();
    QHash<QString,int>* get_list_of_services();
    static bool         check_password_match(const QString& password, const QString& check_password_match);

public slots:
    bool load();
    bool save();
    void load_seat(const bool& bMayLoad);

signals:
    void load_complete(const bool& result);
    void update_complete(const bool& result);


private:    
    int ID_OPERATOR;
    QString FULL_NAME;
    QString USER_NAME;
    QString PASS_WORD;
    int LAST_SEAT;
    bool BEGIN_IN_PAUSE;
    bool ENABLED;

    QAstCTISeat *lastSeat;
    QAstCTIOperatorServices *list_of_services;

private slots:
    void load_list_of_services();
};

#endif // QASTCTIOPERATOR_H