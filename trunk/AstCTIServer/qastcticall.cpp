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

#include "qastcticall.h"

QAstCTICall::QAstCTICall(QObject *parent)
        : QObject(parent), channel(""), parsedChannel(""),
        callerIdNum(""), callerIdName(""),
        uniqueId(""), context(""), state("")
{
    this->variables = new QHash <QString,QString>;

}

QAstCTICall::~QAstCTICall()
{
    qDebug() << "In QAstCTICall::~QAstCTICall()";
    if (this->variables != 0) delete(this->variables);


}

QString &QAstCTICall::getChannel()
{
    return this->channel;
}
void QAstCTICall::setChannel(QString channel)
{
    this->channel = channel;
    this->parseChannel();
}

QString &QAstCTICall::getParsedChannel()
{
    return this->parsedChannel;
}

void QAstCTICall::setParsedChannel(QString parsedChannel)
{
    this->parsedChannel = parsedChannel;
}

QString &QAstCTICall::getDestChannel()
{
    return this->destChannel;
}

void QAstCTICall::setDestChannel(QString channel)
{
    this->destChannel = channel;
    this->parseDestChannel();
}

QString &QAstCTICall::getParsedDestChannel()
{
    return this->parsedDestChannel;
}

void QAstCTICall::setParsedDestChannel(QString parsedChannel)
{
    this->parsedDestChannel = parsedChannel;
}

QString &QAstCTICall::getCalleridNum()
{
    return this->callerIdNum;
}

void QAstCTICall::setCalleridNum(QString callerIdNum)
{
    this->addVariable("CALLERID", callerIdNum);
    this->callerIdNum = callerIdNum;
}
QString &QAstCTICall::getCalleridName()
{
    return this->callerIdName;
}
void QAstCTICall::setCalleridName(QString callerIdName)
{
    this->addVariable("CALLERIDNAME", callerIdName);
    this->callerIdName = callerIdName;
}
QString &QAstCTICall::getUniqueId()
{
    return this->uniqueId;
}
void QAstCTICall::setUniqueId(QString uniqueId)
{
    this->uniqueId = uniqueId;
}

QString &QAstCTICall::getDestUniqueId()
{
    return this->destUniqueId;
}
void QAstCTICall::setDestUniqueId(QString uniqueId)
{
    this->destUniqueId = uniqueId;
}

QString &QAstCTICall::getContext()
{
    return this->context;
}
void QAstCTICall::setContext(QString context)
{
    this->context = context;
}
QString &QAstCTICall::getState()
{
    return this->state;
}
void QAstCTICall::setState(QString state)
{
    this->state = state;
}

void QAstCTICall::addVariable(QString key, QString value)
{
    // We need only one variable with the same name..
    if (this->variables->contains(key)) {
        this->variables->remove(key);
    }
    this->variables->insert(key, value);
}

void QAstCTICall::parseChannel()
{
    // Channels are usually built like SIP/200-0899e2c8
    // So we need to extract channel by split the string
    if (this->channel.contains('-')) {
        QStringList parts = this->channel.split('-');
        if (parts.count() == 2) {
            QString parsedChan = parts.at(0);
            this->setParsedChannel(parsedChan);
        }
    }
}

void QAstCTICall::parseDestChannel()
{
    // Channels are usually built like SIP/200-0899e2c8
    // So we need to extract channel by split the string
    if (this->destChannel.contains('-')) {
        QStringList parts = this->destChannel.split('-');
        if (parts.count() == 2) {
            QString parsedChan = parts.at(0);
            this->setParsedDestChannel(parsedChan);
        }
    }
}

void QAstCTICall::setActions(QHash<int, QAstCTIAction *> *callActions) {

    this->actions = callActions;
}

void QAstCTICall::setOperatingSystem(QString operatingSystem)
{
    this->clientOperatingSystem = operatingSystem;

}

void QAstCTICall::parseActionsParameters()
{
    if ( (this->actions != 0) && (this->variables->count() > 0) ) {
        QHash<int, QAstCTIAction *>::const_iterator actionList = this->actions->constBegin();
        while (actionList != this->actions->constEnd()) {
            QString parameters = ((QAstCTIAction *)actionList.value())->getActionParameters();
            QHash<QString, QString>::const_iterator varList = this->variables->constBegin();
            while (varList != this->variables->constEnd()) {
                QString theKey = QString("{%1}").arg( ((QString)varList.key()).toUpper());
                parameters = parameters.replace(theKey, varList.value());

                varList++;
            }
            ((QAstCTIAction *)actionList.value())->setActionParameters(parameters);
            actionList++;
        }
    }
}

QString QAstCTICall::toXml()
{
    // Here we go for parameter substitution
    this->parseActionsParameters();

    QDomDocument doc("AstCTICallXML");
    QDomElement root = doc.createElement("call");
    root.setAttribute("uniqueid", this->uniqueId);
    doc.appendChild( root );

    // DestUniqueId
    if (this->destUniqueId.length() > 0) {
        QDomElement tag = doc.createElement("DestUniqueId");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->destUniqueId);
        tag.appendChild(t);
    }

    // Context
    if (this->context.length() > 0) {
        QDomElement tag = doc.createElement("Context");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->context);
        tag.appendChild(t);
    }

    // Channel
    if (this->channel.length() > 0) {
        QDomElement tag = doc.createElement("Channel");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->channel);
        tag.appendChild(t);
    }

    // ParsedChannel
    if (this->parsedChannel.length() > 0) {
        QDomElement tag = doc.createElement("ParsedChannel");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->parsedChannel);
        tag.appendChild(t);
    }

    // DestChannel
    if (this->destChannel.length() > 0) {
        QDomElement tag = doc.createElement("DestChannel");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->destChannel);
        tag.appendChild(t);
    }

    // ParsedChannelDest
    if (this->parsedDestChannel.length() > 0) {
        QDomElement tag = doc.createElement("ParsedDestChannel");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->parsedDestChannel);
        tag.appendChild(t);
    }

    // Callerid Name
    if (this->callerIdName.length() > 0) {
        QDomElement tag = doc.createElement("CallerIdName");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->callerIdName);
        tag.appendChild(t);
    }

    // Callerid Num
    if (this->callerIdNum.length() > 0) {
        QDomElement tag = doc.createElement("CallerIdNum");
        root.appendChild(tag);
        QDomText t = doc.createTextNode(this->callerIdNum);
        tag.appendChild(t);
    }

    if (this->variables->count() > 0) {
        QDomElement xmlVars = doc.createElement("Variables");
        xmlVars.setAttribute("count", this->variables->count());
        root.appendChild(xmlVars);

        QHash<QString, QString>::const_iterator varList = this->variables->constBegin();
        while (varList != this->variables->constEnd()) {
            QDomElement tag = doc.createElement(varList.key());
            xmlVars.appendChild(tag);
            QDomText t = doc.createTextNode(varList.value() );
            tag.appendChild(t);
            varList++;
        }
    }

     if ( (this->actions != 0) && (this->actions->count() > 0) ){
        int actionsCount = 0;
        QDomElement xmlActions = doc.createElement("Actions");


        QHash<int, QAstCTIAction *>::const_iterator actionsList = this->actions->constBegin();
        while (actionsList != this->actions->constEnd()) {
            QAstCTIAction *activeAction = (QAstCTIAction *)actionsList.value();
            if ( (activeAction->getActionOsType() == "ALL") ||
                 (activeAction->getActionOsType() == this->clientOperatingSystem) ) {
                actionsCount++;
                switch(QAstCTIAction::getActionTypes()[activeAction->getActionType()]) {
                case ActionApplication:
                    {
                        QDomElement xmlApplication = doc.createElement("Application");

                        // What's the path?
                        QDomElement xmlAppPath = doc.createElement("Path");
                        xmlApplication.appendChild(xmlAppPath);
                        QDomText xmlAppPathVal = doc.createTextNode(activeAction->getActionDestination());
                        xmlAppPath.appendChild(xmlAppPathVal);

                        // What are the parameters?
                        QDomElement xmlAppParams = doc.createElement("Parameters");
                        xmlApplication.appendChild(xmlAppParams);
                        QDomText xmlAppPathParmVal = doc.createTextNode(activeAction->getActionParameters());
                        xmlAppParams.appendChild(xmlAppPathParmVal);

                        // Append all to xmlAction
                        xmlActions.appendChild(xmlApplication);
                    }
                    break;
                case ActionBrowser:
                    {
                        QDomElement xmlApplication = doc.createElement("Browser");

                        // What are the parameters?
                        QDomElement xmlAppParams = doc.createElement("Url");
                        xmlApplication.appendChild(xmlAppParams);
                        QDomText xmlAppPathParmVal = doc.createTextNode(activeAction->getActionParameters());
                        xmlAppParams.appendChild(xmlAppPathParmVal);

                        // Append all to xmlAction
                        xmlActions.appendChild(xmlApplication);
                    }
                    break;
                case ActionInternalBrowser:
                    {
                        QDomElement xmlApplication = doc.createElement("InternalBrowser");

                        // What are the parameters?
                        QDomElement xmlAppParams = doc.createElement("Url");
                        xmlApplication.appendChild(xmlAppParams);
                        QDomText xmlAppPathParmVal = doc.createTextNode(activeAction->getActionParameters());
                        xmlAppParams.appendChild(xmlAppPathParmVal);

                        // Append all to xmlAction
                        xmlActions.appendChild(xmlApplication);
                    }
                    break;
                case ActionTcpMessage:
                    {
                        QStringList serverInfo = activeAction->getActionDestination().split(':');
                        if (serverInfo.length() == 2) {
                            QDomElement xmlApplication = doc.createElement("TcpMessage");

                            QDomElement xmlAppServer = doc.createElement("Server");
                            xmlApplication.appendChild(xmlAppServer);
                            QDomText xmlAppServerVal = doc.createTextNode(serverInfo.at(0));
                            xmlAppServer.appendChild(xmlAppServerVal);

                            QDomElement xmlAppServerPort = doc.createElement("Port");
                            xmlApplication.appendChild(xmlAppServerPort);
                            QDomText xmlAppServerPortVal = doc.createTextNode(serverInfo.at(1));
                            xmlAppServerPort.appendChild(xmlAppServerPortVal);


                            QDomElement xmlAppMessage = doc.createElement("Message");
                            xmlApplication.appendChild(xmlAppMessage);
                            QDomText xmlAppMessageVal = doc.createTextNode(activeAction->getActionParameters());
                            xmlAppMessage.appendChild(xmlAppMessageVal);

                            QDomElement xmlAppEncoding = doc.createElement("Encoding");
                            xmlApplication.appendChild(xmlAppEncoding);
                            QDomText xmlAppEncodingVal = doc.createTextNode(activeAction->getActionMessageEncoding());
                            xmlAppEncoding.appendChild(xmlAppEncodingVal);

                            // Append all to xmlAction
                            xmlActions.appendChild(xmlApplication);
                        }
                    }
                    break;
                case ActionUdpMessage:
                    {
                        QStringList serverInfo = activeAction->getActionDestination().split(':');
                        if (serverInfo.length() == 2) {
                            QDomElement xmlApplication = doc.createElement("UdpMessage");

                            QDomElement xmlAppServer = doc.createElement("Server");
                            xmlApplication.appendChild(xmlAppServer);
                            QDomText xmlAppServerVal = doc.createTextNode(serverInfo.at(0));
                            xmlAppServer.appendChild(xmlAppServerVal);

                            QDomElement xmlAppServerPort = doc.createElement("Port");
                            xmlApplication.appendChild(xmlAppServerPort);
                            QDomText xmlAppServerPortVal = doc.createTextNode(serverInfo.at(1));
                            xmlAppServerPort.appendChild(xmlAppServerPortVal);


                            QDomElement xmlAppMessage = doc.createElement("Message");
                            xmlApplication.appendChild(xmlAppMessage);
                            QDomText xmlAppMessageVal = doc.createTextNode(activeAction->getActionParameters());
                            xmlAppMessage.appendChild(xmlAppMessageVal);

                            QDomElement xmlAppEncoding = doc.createElement("Encoding");
                            xmlApplication.appendChild(xmlAppEncoding);
                            QDomText xmlAppEncodingVal = doc.createTextNode(activeAction->getActionMessageEncoding());
                            xmlAppEncoding.appendChild(xmlAppEncodingVal);

                            // Append all to xmlAction
                            xmlActions.appendChild(xmlApplication);
                        }
                    }
                    break;
                }

            }
            actionsList++;

            xmlActions.setAttribute("count", QString("%1").arg((int)actionsCount));
            root.appendChild(xmlActions);            
        }
    }
    return doc.toString();

}