/****************************************************************
 *  RetroShare GUI is distributed under the following license:
 *
 *  Copyright (C) 2012 by Thunder
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

#ifndef P3_FEEDREADER
#define P3_FEEDREADER

#include "retroshare/rsplugin.h"
#include "plugins/rspqiservice.h"
#include "interface/rsFeedReader.h"

#include "retroshare/rsgxsifacetypes.h"

class RsFeedReaderFeed;
class RsFeedReaderMsg;
class p3FeedReaderThread;

class RsGxsForums;
class RsGxsForumGroup;

class p3FeedReader : public RsPQIService, public RsFeedReader
{
public:
	p3FeedReader(RsPluginHandler *pgHandler, RsGxsForums *forums);

	/****************** FeedReader Interface *************/
	void stop() override;
	void setNotify(RsFeedReaderNotify *notify) override;

	uint32_t getStandardStorageTime() override;
	void     setStandardStorageTime(uint32_t storageTime) override;
	uint32_t getStandardUpdateInterval() override;
	void     setStandardUpdateInterval(uint32_t updateInterval) override;
	bool     getStandardProxy(std::string &proxyAddress, uint16_t &proxyPort) override;
	void     setStandardProxy(bool useProxy, const std::string &proxyAddress, uint16_t proxyPort) override;
	bool     getSaveInBackground() override;
	void     setSaveInBackground(bool saveInBackground) override;

	RsFeedAddResult addFolder(const std::string parentId, const std::string &name, std::string &feedId) override;
	RsFeedAddResult setFolder(const std::string &feedId, const std::string &name) override;
	RsFeedAddResult addFeed(const FeedInfo &feedInfo, std::string &feedId) override;
	RsFeedAddResult setFeed(const std::string &feedId, const FeedInfo &feedInfo) override;
	bool            removeFeed(const std::string &feedId) override;
	bool            addPreviewFeed(const FeedInfo &feedInfo, std::string &feedId) override;
	void            getFeedList(const std::string &parentId, std::list<FeedInfo> &feedInfos) override;
	bool            getFeedInfo(const std::string &feedId, FeedInfo &feedInfo) override;
	bool            getMsgInfo(const std::string &feedId, const std::string &msgId, FeedMsgInfo &msgInfo) override;
	bool            removeMsg(const std::string &feedId, const std::string &msgId) override;
	bool            removeMsgs(const std::string &feedId, const std::list<std::string> &msgIds) override;
	bool            getMessageCount(const std::string &feedId, uint32_t *msgCount, uint32_t *newCount, uint32_t *unreadCount) override;
	bool            getFeedMsgList(const std::string &feedId, std::list<FeedMsgInfo> &msgInfos) override;
	bool            getFeedMsgIdList(const std::string &feedId, std::list<std::string> &msgIds) override;
	bool            processFeed(const std::string &feedId) override;
	bool            setMessageRead(const std::string &feedId, const std::string &msgId, bool read) override;
	bool            retransformMsg(const std::string &feedId, const std::string &msgId) override;
	bool            clearMessageCache(const std::string &feedId) override;

	RsFeedReaderErrorState processXPath(const std::list<std::string> &xpathsToUse, const std::list<std::string> &xpathsToRemove, std::string &description, std::string &errorString) override;
	RsFeedReaderErrorState processXslt(const std::string &xslt, std::string &description, std::string &errorString) override;

	/****************** p3Service STUFF ******************/
	int tick() override;
	RsServiceInfo getServiceInfo() override ;

	/****************** internal STUFF *******************/
	bool getFeedToDownload(RsFeedReaderFeed &feed, const std::string &neededFeedId);
	void onDownloadSuccess(const std::string &feedId, const std::string &content, std::string &icon);
	void onDownloadError(const std::string &feedId, RsFeedReaderErrorState result, const std::string &errorString);
	void onProcessSuccess_filterMsg(const std::string &feedId, std::list<RsFeedReaderMsg*> &msgs);
	void onProcessSuccess_addMsgs(const std::string &feedId, std::list<RsFeedReaderMsg*> &msgs, bool single);
	void onProcessError(const std::string &feedId, RsFeedReaderErrorState result, const std::string &errorString);

	bool getFeedToProcess(RsFeedReaderFeed &feed, const std::string &neededFeedId);

	void setFeedInfo(const std::string &feedId, const std::string &name, const std::string &description);

	bool getForumGroup(const RsGxsGroupId &groupId, RsGxsForumGroup &forumGroup);
	bool updateForumGroup(const RsGxsForumGroup &forumGroup, const std::string &groupName, const std::string &groupDescription);
	bool waitForToken(uint32_t token);

protected:
	/****************** p3Config STUFF *******************/
	RsSerialiser *setupSerialiser() override;
	bool saveList(bool &cleanup, std::list<RsItem *>&) override;
	bool loadList(std::list<RsItem *>& load) override;
	void saveDone() override;

private:
	void cleanFeeds();
	void deleteAllMsgs_locked(RsFeedReaderFeed *fi);
	void stopPreviewThreads_locked();

private:
	time_t   mLastClean;
	RsGxsForums *mForums;
	RsFeedReaderNotify *mNotify;
	volatile bool mStopped;

	RsMutex mFeedReaderMtx;
	std::list<RsItem*> cleanSaveData;
	bool mSaveInBackground;
	std::list<p3FeedReaderThread*> mThreads;
	uint32_t mNextFeedId;
	uint32_t mNextMsgId;
	int32_t mNextPreviewFeedId;
	int32_t mNextPreviewMsgId;
	uint32_t mStandardUpdateInterval;
	uint32_t mStandardStorageTime;
	bool mStandardUseProxy;
	std::string mStandardProxyAddress;
	uint16_t mStandardProxyPort;
	std::map<std::string, RsFeedReaderFeed*> mFeeds;

	RsMutex mDownloadMutex;
	std::list<std::string> mDownloadFeeds;

	RsMutex mProcessMutex;
	std::list<std::string> mProcessFeeds;

	RsMutex mPreviewMutex;
	p3FeedReaderThread *mPreviewDownloadThread;
	p3FeedReaderThread *mPreviewProcessThread;
};

#endif 
