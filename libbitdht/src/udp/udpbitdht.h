#ifndef UDP_BIT_DHT_CLASS_H
#define UDP_BIT_DHT_CLASS_H

/*
 * bitdht/udpbitdht.h
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


#include <iosfwd>
#include <map>
#include <string>

#include "udp/udpstack.h"
#include "bitdht/bdiface.h"
#include "bitdht/bdmanager.h"

/* 
 * This implements a UdpSubReceiver class to allow the DHT to talk to the network.
 * The parser is very strict - and will try to not pick up anyone else's messages.
 *
 * Mutexes are implemented at this level protecting the whole of the DHT code.
 * This class is also a thread - enabling it to do callback etc.
 */

// class BitDhtCallback defined in bdiface.h 

	
class UdpBitDht: public UdpSubReceiver, public bdThread, public BitDhtInterface
{
	public:

    UdpBitDht(UdpPublisher *pub, bdNodeId *id, std::string dhtVersion, std::string bootstrapfile, const std::string& filteredipfile,bdDhtFunctions *fns);
~UdpBitDht() override;


	/*********** External Interface to the World (BitDhtInterface) ************/

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

        /***** Connections Requests *****/
bool ConnectionRequest(struct sockaddr_in *laddr, bdNodeId *target, uint32_t mode, uint32_t delay, uint32_t start) override;
void ConnectionAuth(bdId *srcId, bdId *proxyId, bdId *destId, uint32_t mode, uint32_t loc, 
											uint32_t bandwidth, uint32_t delay, uint32_t answer) override;
void ConnectionOptions(uint32_t allowedModes, uint32_t flags) override;
bool setAttachMode(bool on) override;

        /***** Get Results Details *****/
int getDhtPeerAddress(const bdNodeId *id, struct sockaddr_in &from) override;
int getDhtValue(const bdNodeId *id, std::string key, std::string &value) override;
int getDhtBucket(const int idx, bdBucket &bucket) override;

int getDhtQueries(std::map<bdNodeId, bdQueryStatus> &queries) override;
int getDhtQueryStatus(const bdNodeId *id, bdQuerySummary &query) override;

    virtual bool isAddressBanned(const sockaddr_in &raddr) ;
    virtual bool getListOfBannedIps(std::list<bdFilteredPeer> &ipl);

        /* stats and Dht state */
int startDht() override;
int stopDht() override;
int stateDht() override; 
uint32_t statsNetworkSize() override;
uint32_t statsBDVersionSize() override; 
uint32_t setDhtMode(uint32_t dhtFlags) override;

void getDataTransferred(uint32_t &read, uint32_t &write);

	/******************* Internals *************************/
	/***** Iteration / Loop Management *****/

	/*** Overloaded from UdpSubReceiver ***/
int recvPkt(void *data, int size, struct sockaddr_in &from) override;
int status(std::ostream &out) override;


	/*** Overloaded from iThread ***/
void run() override;

	/**** do whats to be done ***/
int tick();
private:

void clearDataTransferred();

	bdMutex dhtMtx; /* for all class data (below) */
	bdNodeManager *mBitDhtManager;
	bdDhtFunctions *mFns;


	uint32_t mReadBytes;
	uint32_t mWriteBytes;


};


#endif
