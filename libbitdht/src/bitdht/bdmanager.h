#ifndef BITDHT_MANAGER_H
#define BITDHT_MANAGER_H

/*
 * bitdht/bdmanager.h
 *
 * BitDHT: An Flexible DHT library.
 *
 * Copyright 2010 by Robert Fernie
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 3 as published by the Free Software Foundation.
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
 * Please report all bugs and problems to "bitdht@lunamutt.com".
 *
 */



/*******
 * Node Manager.
 ******/

/******************************************
 * 1) Maintains a list of ids to search for.
 * 2) Sets up initial search for own node.
 * 3) Checks on status of queries.
 * 4) Callback on successful searches.
 *
 * This is pretty specific to RS requirements.
 ****/

#define BITDHT_PS_MASK_ACTIONS  	(0x000000ff)
#define BITDHT_PS_MASK_STATE    	(0x0000ff00)

#define BITDHT_PS_ACTION_SEARCHING 	(0x00000001)
#define BITDHT_PS_ACTION_WAITING	(0x00000002)
#define BITDHT_PS_ACTION_PINGING	(0x00000004)

#define BITDHT_PS_STATE_UNKNOWN		(0x00000100)
#define BITDHT_PS_STATE_OFFLINE		(0x00000200)
#define BITDHT_PS_STATE_ONLINE		(0x00000400)
#define BITDHT_PS_STATE_CONNECTED	(0x00000800)

#include "bitdht/bdiface.h"
#include "bitdht/bdnode.h"
#include "util/bdbloom.h"



class bdQueryPeer
{
	public:
	bdId mId;
	uint32_t mStatus;
	uint32_t mQFlags;
	//time_t mLastQuery;
	//time_t mLastFound;
	struct sockaddr_in mDhtAddr;
	time_t mCallbackTS;   // for UPDATES flag.
};


#define BITDHT_MGR_STATE_OFF		0
#define BITDHT_MGR_STATE_STARTUP	1
#define BITDHT_MGR_STATE_FINDSELF	2
#define BITDHT_MGR_STATE_ACTIVE 	3
#define BITDHT_MGR_STATE_REFRESH 	4
#define BITDHT_MGR_STATE_QUIET		5
#define BITDHT_MGR_STATE_FAILED		6

#define MAX_STARTUP_TIME 10
#define MAX_REFRESH_TIME 10

#define BITDHT_MGR_QUERY_FAILURE		1
#define BITDHT_MGR_QUERY_PEER_OFFLINE		2
#define BITDHT_MGR_QUERY_PEER_UNREACHABLE	3
#define BITDHT_MGR_QUERY_PEER_ONLINE		4


/*** NB: Nothing in here is protected by mutexes 
 * must be done at a higher level!
 ***/

class bdNodeManager: public bdNode, public BitDhtInterface
{
	public:
        bdNodeManager(bdNodeId *id, std::string dhtVersion, std::string bootfile, const std::string &filterfile, bdDhtFunctions *fns);


void 	iteration();

        /***** Functions to Call down to bdNodeManager ****/


        /* Friend Tracking */
void addBadPeer(const struct sockaddr_in &addr, uint32_t source, uint32_t reason, uint32_t age) override;
void updateKnownPeer(const bdId *id, uint32_t type, uint32_t flags) override;

        /* Request DHT Peer Lookup */
        /* Request Keyword Lookup */
void addFindNode(bdNodeId *id, uint32_t mode) override;
void removeFindNode(bdNodeId *id) override;
void findDhtValue(bdNodeId *id, std::string key, uint32_t mode) override;

        /***** Add / Remove Callback Clients *****/
void addCallback(BitDhtCallback *cb) override;
void removeCallback(BitDhtCallback *cb) override;

        /***** Get Results Details *****/
int getDhtPeerAddress(const bdNodeId *id, struct sockaddr_in &from) override;
int getDhtValue(const bdNodeId *id, std::string key, std::string &value) override;
int getDhtBucket(const int idx, bdBucket &bucket) override;

int getDhtQueries(std::map<bdNodeId, bdQueryStatus> &queries) override;
int getDhtQueryStatus(const bdNodeId *id, bdQuerySummary &query) override;

	/***** Connection Interface ****/
bool ConnectionRequest(struct sockaddr_in *laddr, bdNodeId *target, uint32_t mode, uint32_t delay, uint32_t start) override;
void ConnectionAuth(bdId *srcId, bdId *proxyId, bdId *destId, 
						uint32_t mode, uint32_t loc, uint32_t bandwidth, uint32_t delay, uint32_t answer) override;
void ConnectionOptions(uint32_t allowedModes, uint32_t flags) override;

bool setAttachMode(bool on) override;

	/* stats and Dht state */
int startDht() override;
int stopDht() override;
int stateDht() override; /* STOPPED, STARTING, ACTIVE, FAILED */
uint32_t statsNetworkSize() override;
uint32_t statsBDVersionSize() override; /* same version as us! */

uint32_t setDhtMode(uint32_t dhtFlags) override;

        /******************* Internals *************************/

	// Overloaded from bdnode for external node callback.
void addPeer(const bdId *id, uint32_t peerflags) override;
        // Overloaded from bdnode for external node callback.
void callbackConnect(bdId *srcId, bdId *proxyId, bdId *destId,
                                        int mode, int point, int param, int cbtype, int errcode) override; 

int 	isBitDhtPacket(char *data, int size, struct sockaddr_in &from);

private:


void 	doNodeCallback(const bdId *id, uint32_t peerflags);
void 	doPeerCallback(const bdId *id, uint32_t status);
void 	doValueCallback(const bdNodeId *id, std::string key, uint32_t status);
void    doInfoCallback(const bdId *id, uint32_t type, uint32_t flags, std::string info);

int	status();
int	checkStatus();
int 	checkPingStatus();
int	checkBadPeerStatus();
int 	SearchOutOfDate();
void	startQueries();

int     QueryRandomLocalNet();
void    SearchForLocalNet();

	std::map<bdNodeId, bdQueryPeer>	mActivePeers;
        std::list<BitDhtCallback *> mCallbacks;

	uint32_t mMode;
	time_t   mModeTS;

        time_t mStartTS;
        time_t mSearchTS;
        bool mSearchingDone;

        bdDhtFunctions *mFns;

	uint32_t mNetworkSize;
	uint32_t mBdNetworkSize;

	bdBloom mBloomFilter;

        bool mLocalNetEnhancements;

	/* future node functions */
	//addPeerPing(foundId);
	//clearPing(it->first);
	//PingStatus(it->first);
};

class bdDebugCallback: public BitDhtCallback
{
        public:
        ~bdDebugCallback();
int dhtPeerCallback(const bdId *id, uint32_t status) override;
int dhtValueCallback(const bdNodeId *id, std::string key, uint32_t status) override;
int dhtConnectCallback(const bdId *srcId, const bdId *proxyId, const bdId *destId,             
                         uint32_t mode, uint32_t point, uint32_t param, uint32_t cbtype, uint32_t errcode) override;
int dhtInfoCallback(const bdId *id, uint32_t type, uint32_t flags, std::string info) override;

};


#endif
