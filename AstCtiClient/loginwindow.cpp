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

#include <QAbstractButton>
#include <QCryptographicHash>

#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget* parent) : QDialog(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

	ui->dialogButtonBox->buttons().at(0)->setIcon(
				QIcon(QPixmap(QStringLiteral(":/res/res/ok.png"))));
	ui->dialogButtonBox->buttons().at(1)->setIcon(
				QIcon(QPixmap(QStringLiteral(":/res/res/cancel.png"))));

    connect(this->ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(accepting()));
    connect(this->ui->dialogButtonBox, SIGNAL(rejected()), this, SIGNAL(rejected()));

	/* CODE TESTING START */
    ui->usernameLineEdit->setText("oper1");
    ui->passwordLineEdit->setText("pass");
	/* CODE TESTING END */
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoginWindow::showMessage(const QString& message, const bool connectionLost)
{
    this->ui->dialogButtonBox->buttons().at(0)->setEnabled(!connectionLost);
    this->ui->messageLabel->setText(message);
}

void LoginWindow::accepting()
{
    if (this->ui->usernameLineEdit->text().isEmpty()) {
		this->showMessage(tr("Username is required to continue. Please enter your username."),
						  false);
        this->ui->usernameLineEdit->setFocus();
    } else {
        this->hide();

        QString pass = this->ui->passwordLineEdit->text();
        if (!pass.isEmpty()) {
            QCryptographicHash md5(QCryptographicHash::Md5);
			md5.addData(QByteArray(pass.toLatin1()));
            pass = QString(md5.result().toHex());
        }

        emit accepted(this->ui->usernameLineEdit->text(), pass);
    }
}
