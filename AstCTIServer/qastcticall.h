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

#ifndef QASTCTICALL_H
#define QASTCTICALL_H

#include <QObject>
#include <QStringList>
#include <QDomDocument>
#include <QHash>

#include "qastctiapplication.h"
#include "qastctiservicesapplications.h"

class QAstCTICall : public QObject
{
    
    Q_OBJECT

public:
    QAstCTICall(QObject* parent);
    ~QAstCTICall();

    QString&                get_channel();
    void                    set_channel(QString channel);
    QString&                get_parsed_channel();
    void                    set_parsed_channel(QString parsedchannel);

    QString&                get_dest_channel();
    void                    set_dest_channel(QString channel);
    QString&                get_parsed_dest_channel();
    void                    set_parsed_dest_channel(QString parsedchannel);

    QString&                get_callerid_num();
    void                    set_callerid_num(QString calleridnum);
    QString&                get_callerid_name();
    void                    set_callerid_name(QString calleridname);
    QString&                get_uniqueid();
    void                    set_uniqueid(QString uniqueid);

    QString&                get_dest_uniqueid();
    void                    set_dest_uniqueid(QString uniqueid);

    QString&                get_context();
    void                    set_context(QString context);
    QString&                get_state();
    void                    set_state(QString state);
    void                    add_variable(QString key, QString value);

    void                    set_applications(QAstCTIServicesApplications* applications);
    void                    set_operating_system(QString operating_system);


    QString                 to_xml();
    
private:
    QString                         channel;
    QString                         parsed_channel;
    QString                         dest_channel;
    QString                         parsed_dest_channel;
    QString                         callerid_num;
    QString                         callerid_name;
    QString                         uniqueid;
    QString                         dest_uniqueid;
    QString                         context;
    QString                         state;
    QString                         clientOperatingSystem;

    QHash<QString, QString>*        variables;
    QAstCTIServicesApplications*    applications;
    QAstCTIApplication*             application;

    void                            parse_channel();
    void                            parse_dest_channel();
    void                            set_application();
    void                            parse_application_parameters();
};

#endif // QASTCTICALL_H