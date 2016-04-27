/*
 * Retroshare Posted Dialog
 *
 * Copyright 2012-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#ifndef MRK_POSTED_DIALOG_H
#define MRK_POSTED_DIALOG_H

#include "gui/gxs/GxsGroupFrameDialog.h"

#define IMAGE_POSTED ":/images/posted_32.png"

class PostedDialog : public GxsGroupFrameDialog
{
	Q_OBJECT

public:
	/** Default Constructor */
	PostedDialog(QWidget *parent = 0);
	/** Default Destructor */
	~PostedDialog() override;

	QIcon iconPixmap() const override { return QIcon(IMAGE_POSTED) ; } //MainPage
	QString pageName() const override { return tr("Posted") ; } //MainPage
	QString helpText() const override { return ""; } //MainPage

	UserNotify *getUserNotify(QObject *parent) override;

protected:
	QString getHelpString() const override ;
    RetroShareLink::enumType getLinkType() override { return RetroShareLink::TYPE_POSTED; }
	GroupFrameSettings::Type groupFrameSettingsType() override { return GroupFrameSettings::Posted; }
	void groupInfoToGroupItemInfo(const RsGroupMetaData &groupInfo, GroupItemInfo &groupItemInfo, const RsUserdata *userdata) override;

private:
	/* GxsGroupFrameDialog */
	QString text(TextType type) override;
	QString icon(IconType type) override;
	QString settingsGroupName() override { return "PostedDialog"; }
	GxsGroupDialog *createNewGroupDialog(TokenQueue *tokenQueue) override;
	GxsGroupDialog *createGroupDialog(TokenQueue *tokenQueue, RsTokenService *tokenService, GxsGroupDialog::Mode mode, RsGxsGroupId groupId) override;
	int shareKeyType() override;
	GxsMessageFrameWidget *createMessageFrameWidget(const RsGxsGroupId &groupId) override;
	RsGxsCommentService *getCommentService() override;
	QWidget *createCommentHeaderWidget(const RsGxsGroupId &grpId, const RsGxsMessageId &msgId) override;
	uint32_t requestGroupSummaryType() override { return GXS_REQUEST_TYPE_GROUP_DATA; } // request complete group data
	void loadGroupSummaryToken(const uint32_t &token, std::list<RsGroupMetaData> &groupInfo, RsUserdata* &userdata) override;
};

#endif
