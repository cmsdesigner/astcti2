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
 * AstCtiClient.  If you copy code from other releases into a copy of GNU
 * AstCtiClient, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for AstCtiClient, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 */

#include <QMouseEvent>
#include <QSettings>

#include "compactwindow.h"
#include "ui_compactwindow.h"
#include "managerwindow.h"

CompactWindow::CompactWindow(AstCtiConfiguration* config) :
	CtiClientWindow(config), ui(new Ui::CompactWindow)
{
	this->ui->setupUi(this);

    this->setWindowFlags((this->windowFlags() | Qt::Window
                                              | Qt::FramelessWindowHint
                                              | Qt::WindowStaysOnTopHint
                                              | Qt::CustomizeWindowHint)
                                              & ~Qt::WindowSystemMenuHint
                                              & ~Qt::WindowTitleHint
                                              & ~Qt::WindowMinMaxButtonsHint
                                              & ~Qt::WindowCloseButtonHint);

    this->ui->moveLabel->installEventFilter(this);
    this->ui->statusLabel->installEventFilter(this);
    this->ui->sizeLabel->installEventFilter(this);

	this->paused = false;

	this->newConfig(config);
	this->enableControls(false);
    this->connectSlots();
    this->readSettings();
}

CompactWindow::~CompactWindow()
{
    delete ui;
}

void CompactWindow::newConfig(AstCtiConfiguration* config)
{
	CtiClientWindow::newConfig(config);

	this->extension = config->extensions.at(0);
}

void CompactWindow::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool CompactWindow::eventFilter(QObject* object, QEvent* e)
{
    bool accepted = false;

    QEvent::Type type = e->type();

	QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(e);

    if (isValidDrag(mouseEvent)) {
        this->dragOrigin = mouseEvent->globalPos();
        e->accept();
        accepted = true;
    } else if (type == QEvent::MouseButtonRelease) {
        this->dragOrigin = QPoint(-1, -1);
        e->accept();
        accepted = true;
    } else if (type == QEvent::MouseMove) {
        if (this->dragOrigin != QPoint(-1, -1)) {
            if (object == this->ui->sizeLabel) {
                int newWidth = this->width() + (mouseEvent->globalX() - this->dragOrigin.x());
                if (newWidth >= this->minimumWidth())
                    this->resize(newWidth, this->height());
            } else {
                this->move(this->pos() + (mouseEvent->globalPos() - this->dragOrigin));
            }
            this->dragOrigin = mouseEvent->globalPos();
            e->accept();
            accepted = true;
        }
    }

    return accepted;
}

void CompactWindow::connectSlots()
{
	connect(this->ui->callButton, SIGNAL(clicked()),
			this, SLOT(placeCall()));
	connect(this->ui->aboutButton, SIGNAL(clicked()),
			this, SLOT(about()));
	connect(this->ui->startPauseButton, SIGNAL(clicked()),
			this, SLOT(agentStartPause()));
	connect(this->ui->minimizeButton, SIGNAL(clicked()),
			this, SLOT(minimizeToTray()));
	connect(this->ui->passwordButton, SIGNAL(clicked()),
			this, SIGNAL(changePassword()));
	connect(this->ui->quitButton, SIGNAL(clicked(bool)),
			this, SLOT(quit(bool)));
	connect(this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void CompactWindow::enableControls(const bool enable)
{
	this->ui->callComboBox->setEditable(enable);
	this->ui->callButton->setEnabled(enable);
	this->ui->passwordButton->setEnabled(enable);
}

void CompactWindow::readSettings()
{
    QSettings settings(APP_NAME);

	settings.beginGroup("CompactWindow." + this->config->userName);
	this->restoreGeometry(settings.value(QStringLiteral("geometry")).toByteArray());
    settings.endGroup();
}

void CompactWindow::writeSettings()
{
    QSettings settings(APP_NAME);

	settings.beginGroup("CompactWindow." + this->config->userName);
	settings.setValue(QStringLiteral("geometry"), this->saveGeometry());
    settings.endGroup();
}

// Returns true if status is the same in all queues, otherwise false
// Paused and LoggedOut are considered to be the same for this purpose
bool CompactWindow::setAgentStatus(const QString &queue, const QString &channelName,
								   AstCtiAgentStatus& status)
{
	bool allSet = CtiClientWindow::setAgentStatus(queue, channelName, status);

	// If we are logged-in even in one queue, we consider us to be "started"
	switch (status) {
	case AgentStatusLoggedIn:
		this->toggleStartPauseButton(this->ui->startPauseButton, false);
		this->paused = false;
		break;
	case AgentStatusLoggedOut:
	case AgentStatusPaused:
		if (allSet) {
			this->toggleStartPauseButton(this->ui->startPauseButton, true);
			this->paused = true;
		}
		break;
	}

	return allSet;
}

void CompactWindow::setStatus(bool status)
{
	CtiClientWindow::setStatus(status);
    if (status) {
		this->ui->statusLabel->setPixmap(QPixmap(QStringLiteral(":/res/res/greenled.png")));
        this->ui->statusLabel->setToolTip(statusMsgOK);
    } else {
		this->ui->statusLabel->setPixmap(QPixmap(QStringLiteral(":/res/res/redled.png")));
        this->ui->statusLabel->setToolTip(statusMsgNotOK);
    }
}

void CompactWindow::handleChannelEvent(AstCtiChannel* channel)
{
	const QString eventName = channel->lastEvent;

	//TODO
}

void CompactWindow::agentStartPause()
{
	this->ui->startPauseButton->setEnabled(false);

	//Send start/pause request for all queues this operator belongs to
	for (AstCtiAgentStatusHash::const_iterator i = this->extension->queues.constBegin();
		 i != this->extension->queues.constEnd();
		 i++) {
		const QString queue = i.key();
		if (this->paused)
			if (i.value() == AgentStatusPaused)
				emit this->pauseRequest(queue, this->extension->channelName);
			else
				emit this->startRequest(queue, this->extension->channelName);
		else
			if (i.value() == AgentStatusLoggedIn)
				emit this->pauseRequest(queue, this->extension->channelName);
	}
}

void CompactWindow::agentStatusChanged(const QString& queue, const QString& channelName,
									   AstCtiAgentStatus status) {
	if (this->extension->channelName != channelName)
		return;

	CtiClientWindow::agentStatusChanged(queue, channelName, status);
}
