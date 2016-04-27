/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2012 RetroShare Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#ifndef TRANSFERUSERNOTIFY_H
#define TRANSFERUSERNOTIFY_H

#include "gui/common/UserNotify.h"

class TransferUserNotify : public UserNotify
{
	Q_OBJECT

public:
	TransferUserNotify(QObject *parent = 0);

	bool hasSetting(QString *name, QString *group) override;

private slots:
	void downloadCountChanged(int count);

private:
	QIcon getIcon() override;
	QIcon getMainIcon(bool hasNew) override;
	unsigned int getNewCount() override;
	QString getTrayMessage(bool plural) override;
	QString getNotifyMessage(bool plural) override;
	void iconClicked() override;

	unsigned int newTransferCount;
};

#endif // TRANSFERUSERNOTIFY_H
