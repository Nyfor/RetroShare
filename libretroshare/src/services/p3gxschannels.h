/*
 * libretroshare/src/services: p3gxschannels.h
 *
 * GxsChannel interface for RetroShare.
 *
 * Copyright 2012-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
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

#ifndef P3_GXSCHANNELS_SERVICE_HEADER
#define P3_GXSCHANNELS_SERVICE_HEADER


#include "retroshare/rsgxschannels.h"
#include "services/p3gxscommon.h"
#include "gxs/rsgenexchange.h"
#include "gxs/gxstokenqueue.h"

#include "util/rstickevent.h"

#include <map>
#include <string>

/* 
 *
 */


class SSGxsChannelGroup
{
	public:
	SSGxsChannelGroup(): mAutoDownload(false), mDownloadDirectory("") {}
	bool load(const std::string &input);
	std::string save() const;

	bool mAutoDownload;
	std::string mDownloadDirectory;
};


class p3GxsChannels: public RsGenExchange, public RsGxsChannels, 
	public GxsTokenQueue,
	public RsTickEvent	/* only needed for testing - remove after */
{
	public:

	p3GxsChannels(RsGeneralDataService* gds, RsNetworkExchangeService* nes, RsGixs* gixs);
RsServiceInfo getServiceInfo() override;

void service_tick() override;

	protected:


	// Overloaded to cache new groups.
RsGenExchange::ServiceCreate_Return service_CreateGroup(RsGxsGrpItem* grpItem, RsTlvSecurityKeySet& keySet) override;

void notifyChanges(std::vector<RsGxsNotify*>& changes) override;

        // Overloaded from RsTickEvent.
void handle_event(uint32_t event_type, const std::string &elabel) override;

	public:

bool getGroupData(const uint32_t &token, std::vector<RsGxsChannelGroup> &groups) override;
bool getPostData(const uint32_t &token, std::vector<RsGxsChannelPost> &posts) override;

bool getRelatedPosts(const uint32_t &token, std::vector<RsGxsChannelPost> &posts) override;

        //////////////////////////////////////////////////////////////////////////////

//virtual bool setMessageStatus(const std::string &msgId, const uint32_t status, const uint32_t statusMask);
//virtual bool setGroupSubscribeFlags(const std::string &groupId, uint32_t subscribeFlags, uint32_t subscribeMask);

//virtual bool groupRestoreKeys(const std::string &groupId);
    bool groupShareKeys(const RsGxsGroupId &groupId, std::set<RsPeerId>& peers) override ;

bool createGroup(uint32_t &token, RsGxsChannelGroup &group) override;
bool createPost(uint32_t &token, RsGxsChannelPost &post) override;

bool updateGroup(uint32_t &token, RsGxsChannelGroup &group) override;

// no tokens... should be cached.
bool setChannelAutoDownload(const RsGxsGroupId &groupId, bool enabled) override;
	bool getChannelAutoDownload(const RsGxsGroupId &groupid, bool& enabled) override;
bool setChannelDownloadDirectory(const RsGxsGroupId &groupId, const std::string& directory) override;
bool getChannelDownloadDirectory(const RsGxsGroupId &groupId, std::string& directory) override;

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


	// Overloaded from RsGxsIface.
bool subscribeToGroup(uint32_t &token, const RsGxsGroupId &groupId, bool subscribe) override;

	// Set Statuses.
virtual void setMessageProcessedStatus(uint32_t& token, const RsGxsGrpMsgIdPair& msgId, bool processed);
void setMessageReadStatus(uint32_t& token, const RsGxsGrpMsgIdPair& msgId, bool read) override;

	// File Interface
bool ExtraFileHash(const std::string &path, std::string filename) override;
bool ExtraFileRemove(const RsFileHash &hash) override;

	protected:

	// Overloaded from GxsTokenQueue for Request callbacks.
void handleResponse(uint32_t token, uint32_t req_type) override;

	private:

static uint32_t channelsAuthenPolicy();

	// Handle Processing.
	void request_AllSubscribedGroups();
	void request_SpecificSubscribedGroups(const std::list<RsGxsGroupId> &groups);
	void load_SubscribedGroups(const uint32_t &token);

	void request_SpecificUnprocessedPosts(std::list<std::pair<RsGxsGroupId, RsGxsMessageId> > &ids);
	void load_SpecificUnprocessedPosts(const uint32_t &token);

	void request_GroupUnprocessedPosts(const std::list<RsGxsGroupId> &grouplist);
	void load_GroupUnprocessedPosts(const uint32_t &token);

	void handleUnprocessedPost(const RsGxsChannelPost &msg);

	// Local Cache of Subscribed Groups. and AutoDownload Flag.
	void updateSubscribedGroup(const RsGroupMetaData &group);
	void clearUnsubscribedGroup(const RsGxsGroupId &id);
	bool setAutoDownload(const RsGxsGroupId &groupId, bool enabled);
	bool autoDownloadEnabled(const RsGxsGroupId &groupId, bool &enabled);



	std::map<RsGxsGroupId, RsGroupMetaData> mSubscribedGroups;


// DUMMY DATA,
virtual bool generateDummyData();

std::string genRandomId();

void 	dummy_tick();

bool generatePost(uint32_t &token, const RsGxsGroupId &grpId);
bool generateComment(uint32_t &token, const RsGxsGroupId &grpId, 
		const RsGxsMessageId &parentId, const RsGxsMessageId &threadId);
bool generateVote(uint32_t &token, const RsGxsGroupId &grpId, 
		const RsGxsMessageId &parentId, const RsGxsMessageId &threadId);

bool generateGroup(uint32_t &token, std::string groupName);

	class ChannelDummyRef
	{
		public:
		ChannelDummyRef() { return; }
		ChannelDummyRef(const RsGxsGroupId &grpId, const RsGxsMessageId &threadId, const RsGxsMessageId &msgId)
		:mGroupId(grpId), mThreadId(threadId), mMsgId(msgId) { return; }

		RsGxsGroupId mGroupId;
		RsGxsMessageId mThreadId;
		RsGxsMessageId mMsgId;
	};

	uint32_t mGenToken;
	bool mGenActive;
	int mGenCount;
	std::vector<ChannelDummyRef> mGenRefs;
	RsGxsMessageId mGenThreadId;

	p3GxsCommentService *mCommentService;	
};

#endif 
