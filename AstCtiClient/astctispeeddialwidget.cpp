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

#include <QtWidgets/QStylePainter>

#include "astctispeeddialwidget.h"
#include "ui_astctispeeddialwidget.h"

AstCtiSpeedDialWidget::AstCtiSpeedDialWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::AstCtiSpeedDialWidget)
{
    ui->setupUi(this);

    //this->setAttribute(Qt::WA_OpaquePaintEvent);

    m_busyLampField = false;
    m_extensionState = AstCtiSpeedDialWidget::ExtensionStateUnknown;
    m_doNotDisturb = false;
    m_callWaiting = false;
    m_callForward = AstCtiSpeedDialWidget::CallForwardNone;
    m_voicemail = 0;

    ui->doNotDisturbLabel->hide();
    ui->callForwardLabel->hide();
    ui->callWaitingLabel->hide();
    ui->voicemailLabel->hide();

    ui->captionLabel->installEventFilter(this);
    ui->doNotDisturbLabel->installEventFilter(this);
    ui->callForwardLabel->installEventFilter(this);
    ui->callWaitingLabel->installEventFilter(this);
    ui->voicemailLabel->installEventFilter(this);
}

AstCtiSpeedDialWidget::~AstCtiSpeedDialWidget()
{
    delete ui;
}

const QString& AstCtiSpeedDialWidget::name() const
{
    return this->m_name;
}

void AstCtiSpeedDialWidget::setName(const QString& name)
{
	this->m_name = name;
	this->setCaption();
	this->setStatusText();
}

const QString& AstCtiSpeedDialWidget::number() const
{
    return this->m_number;
}

void AstCtiSpeedDialWidget::setNumber(const QString& number)
{
	this->m_number = number;
	this->setCaption();
	this->setStatusText();
}

const bool AstCtiSpeedDialWidget::showNumber() const
{
    return this->m_showNumber;
}

void AstCtiSpeedDialWidget::setShowNumber(const bool showNumber)
{
	this->m_showNumber = showNumber;
	this->setCaption();
}

const QString& AstCtiSpeedDialWidget::group() const
{
    return this->m_group;
}

void AstCtiSpeedDialWidget::setGroup(const QString& group)
{
	this->m_group = group;
}

const bool AstCtiSpeedDialWidget::busyLampField() const
{
    return this->m_busyLampField;
}

void AstCtiSpeedDialWidget::setBusyLampField(const bool blf)
{
    this->m_busyLampField = blf;

    if (blf) {
        this->ui->doNotDisturbLabel->setVisible(this->m_doNotDisturb);
        this->ui->callWaitingLabel->setVisible(this->m_callWaiting);
		this->ui->callForwardLabel->setVisible(
				this->m_callForward != AstCtiSpeedDialWidget::CallForwardNone);
        this->ui->voicemailLabel->setVisible(this->m_voicemail > 0);
    } else {
        this->ui->doNotDisturbLabel->setVisible(false);
        this->ui->callWaitingLabel->setVisible(false);
        this->ui->callForwardLabel->setVisible(false);
        this->ui->voicemailLabel->setVisible(false);

        this->setExtensionState(AstCtiSpeedDialWidget::ExtensionStateUnknown);
    }

	this->setStatusText();
}

const AstCtiSpeedDialWidget::ExtensionState AstCtiSpeedDialWidget::extensionState() const
{
    return this->m_extensionState;
}

void AstCtiSpeedDialWidget::setExtensionState(const ExtensionState state)
{
    this->m_extensionState = state;

    //Redraw control to reflect state change
    //this->style()->polish(this);
    this->update();

	this->setStatusText();
}

const bool AstCtiSpeedDialWidget::doNotDisturb() const
{
    return this->m_doNotDisturb;
}

void AstCtiSpeedDialWidget::setDoNotDisturb(const bool dnd)
{
    this->m_doNotDisturb = dnd;

	this->ui->doNotDisturbLabel->setVisible(dnd && this->m_busyLampField);

	this->setStatusText();
}

const bool AstCtiSpeedDialWidget::callWaiting() const
{
    return this->m_callWaiting;
}

void AstCtiSpeedDialWidget::setCallWaiting(const bool cw)
{
    this->m_callWaiting = cw;

	this->ui->callWaitingLabel->setVisible(cw && this->m_busyLampField);

	this->setStatusText();
}

const AstCtiSpeedDialWidget::CallForward AstCtiSpeedDialWidget::callForward() const
{
    return this->m_callForward;
}

void AstCtiSpeedDialWidget::setCallForward(const AstCtiSpeedDialWidget::CallForward cf)
{
    this->m_callForward = cf;

	this->ui->callForwardLabel->setVisible(cf != AstCtiSpeedDialWidget::CallForwardNone &&
									 this->m_busyLampField);

	this->setStatusText();
}

const QString& AstCtiSpeedDialWidget::callForwardNumber() const
{
    return this->m_CallForwardNumber;
}

void AstCtiSpeedDialWidget::setCallForwardNumber(const QString& cfNumber)
{
	this->m_CallForwardNumber = cfNumber;

	this->setStatusText();
}

const int AstCtiSpeedDialWidget::voicemail() const
{
    return this->m_voicemail;
}

void AstCtiSpeedDialWidget::setVoicemail(const int vmCount)
{
	this->m_voicemail = vmCount;

	this->ui->voicemailLabel->setVisible(vmCount && this->m_busyLampField);

	this->setStatusText();
}

void AstCtiSpeedDialWidget::paintEvent(QPaintEvent* )
{
    QStylePainter painter(this);

    QStyleOption option;
    option.initFrom(this);

    QColor color;
    switch (this->m_extensionState) {
    case AstCtiSpeedDialWidget::ExtensionStateUnknown:
        color = QColor::fromRgb(255, 255, 255);
        break;
    case AstCtiSpeedDialWidget::ExtensionStateDisconnected:
        color = QColor::fromRgb(211, 211, 211);
        break;
    case AstCtiSpeedDialWidget::ExtensionStateIdle:
        color = QColor::fromRgb(63, 255, 0);
        break;
    case AstCtiSpeedDialWidget::ExtensionStateRinging:
        color = QColor::fromRgb(255, 255, 127);
        break;
    case AstCtiSpeedDialWidget::ExtensionStateBusy:
        color = QColor::fromRgb(255, 128, 128);
        break;
    }

    QPalette palette(option.palette);

    palette.setBrush(QPalette::Window, QBrush(color));
    palette.setBrush(QPalette::WindowText, QBrush(Qt::black));
    option.palette = palette;
    painter.drawPrimitive(QStyle::PE_Widget, option);
}

void AstCtiSpeedDialWidget::resizeEvent(QResizeEvent* )
{
    int iconWidth = this->ui->callForwardLabel->width();

    this->ui->captionLabel->resize(this->size());
    this->ui->callForwardLabel->move(this->width() - iconWidth, 0);
    this->ui->callWaitingLabel->move(0, this->height() - iconWidth);
    this->ui->voicemailLabel->move(this->width() - iconWidth, this->height() - iconWidth);
}

bool AstCtiSpeedDialWidget::eventFilter(QObject* , QEvent* e)
{
    bool accepted = false;

    QEvent::Type type = e->type();

    if (type == QEvent::MouseButtonRelease) {
        if (this->underMouse()) {
            if (this->m_extensionState == AstCtiSpeedDialWidget::ExtensionStateRinging)
                emit takeCall(this->m_number);
            else
                emit dial(this->m_number);
            e->accept();
            accepted = true;
        }
    }

    return accepted;
}

void AstCtiSpeedDialWidget::setCaption()
{
    if (this->m_showNumber && this->m_number.length() > 0)
        this->ui->captionLabel->setText(this->name() + "\n" + this->m_number);
    else
        this->ui->captionLabel->setText(this->name());
}

void AstCtiSpeedDialWidget::setStatusText()
{
    QString s;
	QString status = tr("Name: ") + this->m_name;
    if (this->m_number.length() > 0)
		status += "\n" + tr("Number: ") + this->m_number;
	status += "\n" + tr("Status: ") + extensionStateToString();
	if (this->m_doNotDisturb && this->m_busyLampField) {
		s = tr("Do not disturb: on");
        this->ui->doNotDisturbLabel->setToolTip(s);
        status += "\n" + s;
    }
	if (this->m_callWaiting && this->m_busyLampField) {
		s = tr("Call waiting: on");
        this->ui->callWaitingLabel->setToolTip(s);
        status += "\n" + s;
    }
	if (this->m_callForward != AstCtiSpeedDialWidget::CallForwardNone && this->m_busyLampField) {
		s = tr("Call forward ") + callForwardToString()
		  + tr(" to ") + this->m_CallForwardNumber;
        this->ui->callForwardLabel->setToolTip(s);
        status += "\n" + s;
    }
	if (this->m_voicemail > 0 && this->m_busyLampField) {
		s = tr("New voicemail messages: %1").arg(this->m_voicemail);
        this->ui->voicemailLabel->setToolTip(s);
        status += "\n" + s;
    }

    this->ui->captionLabel->setToolTip(status);
}

const QString AstCtiSpeedDialWidget::extensionStateToString()
{
	//We use a variable to exploit NRVO
	QString stateName;

	switch (this->m_extensionState) {
    case AstCtiSpeedDialWidget::ExtensionStateDisconnected:
		stateName = tr("Disconnected");
		break;
	case AstCtiSpeedDialWidget::ExtensionStateIdle:
		stateName = tr("Idle");
		break;
	case AstCtiSpeedDialWidget::ExtensionStateRinging:
		stateName = tr("Ringing");
		break;
	case AstCtiSpeedDialWidget::ExtensionStateBusy:
		stateName = tr("Busy");
		break;
	default:
		stateName = tr("Unknown");
		break;
	}

	return stateName;
}

const QString AstCtiSpeedDialWidget::callForwardToString()
{
	//We use a variable to exploit NRVO
	QString cfName;

	if (this->m_callForward & AstCtiSpeedDialWidget::CallForwardUnconditional)
		cfName = tr("unconditional");
	if (this->m_callForward & AstCtiSpeedDialWidget::CallForwardOnBusy &&
		AstCtiSpeedDialWidget::CallForwardOnNoAnswer)
		cfName = tr("on busy and on no answer");
    if (this->m_callForward & AstCtiSpeedDialWidget::CallForwardOnBusy)
		cfName = tr("on busy");
    if (this->m_callForward & AstCtiSpeedDialWidget::CallForwardOnNoAnswer)
		cfName = tr("on no answer");
	else
		cfName = QStringLiteral("");

	return cfName;
}
