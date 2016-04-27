/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2015
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

#pragma once

#include <list>
#include <string>

#include "services/rsVOIPItems.h"
#include "services/p3service.h"
#include "serialiser/rstlvbase.h"
#include "serialiser/rsconfigitems.h"
#include "plugins/rspqiservice.h"
#include <interface/rsVOIP.h>

class p3LinkMgr;
class VOIPNotify ;

class VOIPPeerInfo
{
	public:

    bool initialisePeerInfo(const RsPeerId &id);

    RsPeerId mId;
	double mCurrentPingTS;
	double mCurrentPingCounter;
	bool   mCurrentPongRecvd;

	uint32_t mLostPongs;
	uint32_t mSentPings;
	uint32_t total_bytes_received ;
	uint32_t average_incoming_bandwidth ;

	std::list<RsVOIPPongResult> mPongResults;
	std::list<RsVOIPDataItem*> incoming_queue ;
};


//!The RS VoIP Test service.
 /**
  *
  * This is only used to test Latency for the moment.
  */

class p3VOIP: public RsPQIService, public RsVOIP
// Maybe we inherit from these later - but not needed for now.
//, public p3Config, public pqiMonitor
{
	public:
		p3VOIP(RsPluginHandler *cm,VOIPNotify *);

		/***** overloaded from rsVOIP *****/

		uint32_t getPongResults(const RsPeerId &id, int n, std::list<RsVOIPPongResult> &results) override;

		// Call stuff.
		//

		// Sending data. The client keeps the memory ownership and must delete it after calling this.
		int sendVoipData(const RsPeerId &peer_id,const RsVOIPDataChunk& chunk) override ;

		// The server fill in the data and gives up memory ownership. The client must delete the memory
		// in each chunk once it has been used.
		//
		bool getIncomingData(const RsPeerId& peer_id,std::vector<RsVOIPDataChunk>& chunks) override ;

		int sendVoipHangUpCall(const RsPeerId& peer_id, uint32_t flags) override ;
		int sendVoipRinging(const RsPeerId& peer_id, uint32_t flags) override ;
		int sendVoipAcceptCall(const RsPeerId &peer_id, uint32_t flags) override ;

		/***** overloaded from p3Service *****/
		/*!
		 * This retrieves all chat msg items and also (important!)
		 * processes chat-status items that are in service item queue. chat msg item requests are also processed and not returned
		 * (important! also) notifications sent to notify base  on receipt avatar, immediate status and custom status
		 * : notifyCustomState, notifyChatStatus, notifyPeerHasNewAvatar
		 * @see NotifyBase
		 */
		int   tick() override;
		virtual int   status();
		bool  recvItem(RsItem *item) override;

		/*************** pqiMonitor callback ***********************/
		//virtual void statusChange(const std::list<pqipeer> &plist);

		int  getVoipATransmit() const override  { return _atransmit ; }
		void setVoipATransmit(int) override ;
		int  getVoipVoiceHold() const override  { return _voice_hold ; }
		void setVoipVoiceHold(int) override ;
		int  getVoipfVADmin() const override    { return _vadmin ; }
		void setVoipfVADmin(int) override ;
		int  getVoipfVADmax() const override    { return _vadmax ; } 
		void setVoipfVADmax(int) override ;
		int  getVoipiNoiseSuppress() const override { return _noise_suppress ; }
		void setVoipiNoiseSuppress(int) override ;
		int  getVoipiMinLoudness() const override   { return _min_loudness ; }
		void setVoipiMinLoudness(int) override ;
		bool getVoipEchoCancel() const override 	 { return _echo_cancel ; }
		void setVoipEchoCancel(bool) override ;

		/************* from p3Config *******************/
		RsSerialiser *setupSerialiser() override ;

		/*!
		 * chat msg items and custom status are saved
		 */
		bool saveList(bool& cleanup, std::list<RsItem*>&) override ;
		bool loadList(std::list<RsItem*>& load) override ;
		virtual std::string configurationFileName() const { return "voip.cfg" ; }

		RsServiceInfo getServiceInfo() override ;

	private:
		int   sendPackets();
		void 	sendPingMeasurements();
		void 	sendBandwidthInfo();

		int sendVoipBandwidth(const RsPeerId &peer_id,uint32_t bytes_per_sec) ;

		int 	handlePing(RsVOIPPingItem *item);
		int 	handlePong(RsVOIPPongItem *item);

		int 	storePingAttempt(const RsPeerId &id, double ts, uint32_t mCounter);
		int 	storePongResult(const RsPeerId& id, uint32_t counter, double ts, double rtt, double offset);

		void handleProtocol(RsVOIPProtocolItem*) ;
		void handleData(RsVOIPDataItem*) ;

		RsMutex mVOIPMtx;

		VOIPPeerInfo *locked_GetPeerInfo(const RsPeerId& id);

		static RsTlvKeyValue push_int_value(const std::string& key,int value) ;
		static int pop_int_value(const std::string& s) ;

		std::map<RsPeerId, VOIPPeerInfo> mPeerInfo;
		time_t mSentPingTime;
		time_t mSentBandwidthInfoTime;
		uint32_t mCounter;

		RsServiceControl *mServiceControl;
		VOIPNotify *mNotify ;

		int _atransmit ;
		int _voice_hold ;
		int _vadmin ;
		int _vadmax ;
		int _min_loudness ;
		int _noise_suppress ;
		bool _echo_cancel ;
};
