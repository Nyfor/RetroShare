/*
 * libretroshare/src/services: p3posted.h
 *
 * GxsChannel interface for RetroShare.
 *
 * Copyright 2012-2013 by Robert Fernie.
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

#ifndef P3_POSTED_SERVICE_HEADER
#define P3_POSTED_SERVICE_HEADER


#include "retroshare/rsposted.h"
#include "services/p3postbase.h"

#include <retroshare/rsidentity.h>

#include <map>
#include <string>
#include <list>

/* 
 *
 */

class p3Posted: public p3PostBase, public RsPosted
{
	public:

	p3Posted(RsGeneralDataService* gds, RsNetworkExchangeService* nes, RsGixs* gixs);
RsServiceInfo getServiceInfo() override;

	protected:

void notifyChanges(std::vector<RsGxsNotify*>& changes) override
{
	return p3PostBase::notifyChanges(changes);
}

	public:

void receiveHelperChanges(std::vector<RsGxsNotify*>& changes) override
{
	return RsGxsIfaceHelper::receiveChanges(changes);
}

	// Posted Specific DataTypes.
bool getGroupData(const uint32_t &token, std::vector<RsPostedGroup> &groups) override;
bool getPostData(const uint32_t &token, std::vector<RsPostedPost> &posts) override;
bool getRelatedPosts(const uint32_t &token, std::vector<RsPostedPost> &posts) override;

bool createGroup(uint32_t &token, RsPostedGroup &group) override;
bool createPost(uint32_t &token, RsPostedPost &post) override;

bool updateGroup(uint32_t &token, RsPostedGroup &group) override;
bool groupShareKeys(const RsGxsGroupId &group, const std::set<RsPeerId>& peers) override;

        //////////////////////////////////////////////////////////////////////////////
	// WRAPPERS due to the separate Interface.

void setMessageReadStatus(uint32_t& token, const RsGxsGrpMsgIdPair& msgId, bool read) override
	{
		return p3PostBase::setMessageReadStatus(token, msgId, read);
	}


	/* Comment service - Provide RsGxsCommentService - redirect to p3GxsCommentService */
bool getCommentData(const uint32_t &token, std::vector<RsGxsComment> &msgs) override
	{
        	return mCommentService->getGxsCommentData(token, msgs);
	}

bool getRelatedComments(const uint32_t &token, std::vector<RsGxsComment> &msgs) override
	{
		return mCommentService->getGxsRelatedComments(token, msgs);
	}

bool createComment(uint32_t &token, RsGxsComment &msg) override
	{
		return mCommentService->createGxsComment(token, msg);
	}

bool createVote(uint32_t &token, RsGxsVote &msg) override
	{
		return mCommentService->createGxsVote(token, msg);
	}

bool acknowledgeComment(const uint32_t& token, std::pair<RsGxsGroupId, RsGxsMessageId>& msgId) override
	{
		return acknowledgeMsg(token, msgId);
	}

bool acknowledgeVote(const uint32_t& token, std::pair<RsGxsGroupId, RsGxsMessageId>& msgId) override
	{
		if (mCommentService->acknowledgeVote(token, msgId))
		{
			return true;
		}
		return acknowledgeMsg(token, msgId);
	}
};

#endif 
