/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2008 Robert Fernie
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

#ifndef _GXSFORUMSDIALOG_H
#define _GXSFORUMSDIALOG_H

#include "gui/gxs/GxsGroupFrameDialog.h"

#define IMAGE_GXSFORUMS         ":/images/konversation.png"

class GxsForumsDialog : public GxsGroupFrameDialog
{
	Q_OBJECT

public:
	GxsForumsDialog(QWidget *parent = 0);
	~GxsForumsDialog() override;

	QIcon iconPixmap() const override { return QIcon(IMAGE_GXSFORUMS) ; } //MainPage
	QString pageName() const override { return tr("Forums") ; } //MainPage
	QString helpText() const override { return ""; } //MainPage

	UserNotify *getUserNotify(QObject *parent) override;

protected:
	QString getHelpString() const override ;
	RetroShareLink::enumType getLinkType() override { return RetroShareLink::TYPE_FORUM; }
	GroupFrameSettings::Type groupFrameSettingsType() override { return GroupFrameSettings::Forum; }
	void groupInfoToGroupItemInfo(const RsGroupMetaData &groupInfo, GroupItemInfo &groupItemInfo, const RsUserdata *userdata) override;

private:
	/* GxsGroupFrameDialog */
	QString text(TextType type) override;
	QString icon(IconType type) override;
	QString settingsGroupName() override { return "ForumsDialog"; }
	GxsGroupDialog *createNewGroupDialog(TokenQueue *tokenQueue) override;
	GxsGroupDialog *createGroupDialog(TokenQueue *tokenQueue, RsTokenService *tokenService, GxsGroupDialog::Mode mode, RsGxsGroupId groupId) override;
	int shareKeyType() override;
	GxsMessageFrameWidget *createMessageFrameWidget(const RsGxsGroupId &groupId) override;
	uint32_t requestGroupSummaryType() override { return GXS_REQUEST_TYPE_GROUP_DATA; } // request complete group data
	void loadGroupSummaryToken(const uint32_t &token, std::list<RsGroupMetaData> &groupInfo, RsUserdata* &userdata) override;
};

#endif
