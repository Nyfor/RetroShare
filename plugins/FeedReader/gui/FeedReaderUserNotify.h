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

#ifndef FEEDREADERUSERNOTIFY_H
#define FEEDREADERUSERNOTIFY_H

#include "gui/common/UserNotify.h"

class FeedReaderDialog;
class RsFeedReader;
class FeedReaderNotify;

class FeedReaderUserNotify : public UserNotify
{
	Q_OBJECT

public:
	FeedReaderUserNotify(FeedReaderDialog *feedReaderDialog, RsFeedReader *feedReader, FeedReaderNotify *notify, QObject *parent);

	bool hasSetting(QString *name, QString *group) override;

private slots:
	void feedChanged(const QString &feedId, int type);

private:
	QIcon getIcon() override;
	QIcon getMainIcon(bool hasNew) override;
	unsigned int getNewCount() override;
	void iconClicked() override;

	FeedReaderDialog *mFeedReaderDialog;
	RsFeedReader *mFeedReader;
	FeedReaderNotify *mNotify;
};

#endif // FEEDREADERUSERNOTIFY_H
