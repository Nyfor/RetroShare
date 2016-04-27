#ifndef LIBRETROSHARE_CONFIG_IMPLEMENTATION_H
#define LIBRETROSHARE_CONFIG_IMPLEMENTATION_H

/*
 * libretroshare/src/rsserver: p3serverconfig.h
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2011-2011 by Robert Fernie.
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


#include "retroshare/rsconfig.h"
#include "pqi/p3peermgr.h"
#include "pqi/p3linkmgr.h"
#include "pqi/p3netmgr.h"
#include "pqi/p3cfgmgr.h"
#include "pqi/pqihandler.h"


#define RS_CONFIG_ADVANCED_STRING       "AdvMode"



class p3ServerConfig: public RsServerConfig
{
	public:

	p3ServerConfig(p3PeerMgr *peerMgr, p3LinkMgr *linkMgr, p3NetMgr *netMgr, pqihandler *pqih, p3GeneralConfig *genCfg);
~p3ServerConfig() override;

	void load_config();

	/* From RsIface::RsConfig */

int 	getConfigNetStatus(RsConfigNetStatus &status) override;
virtual int 	getConfigStartup(RsConfigStartup &params);
//virtual int 	getConfigDataRates(RsConfigDataRates &params);

                /***** for RsConfig -> p3BandwidthControl ****/

int getTotalBandwidthRates(RsConfigDataRates &rates) override;
int getAllBandwidthRates(std::map<RsPeerId, RsConfigDataRates> &ratemap) override;
    int getTrafficInfo(std::list<RSTrafficClue>& out_lst, std::list<RSTrafficClue> &in_lst) override ;

	/* From RsInit */

virtual std::string      RsConfigDirectory();
virtual std::string      RsConfigKeysDirectory();

virtual std::string  RsProfileConfigDirectory();
virtual bool         getStartMinimised();
virtual std::string  getRetroShareLink();

virtual bool getAutoLogin();
virtual void setAutoLogin(bool autoLogin);
virtual bool RsClearAutoLogin();

virtual std::string getRetroshareDataDirectory();

	/* New Stuff */

uint32_t getUserLevel() override;

uint32_t getNetState() override;
uint32_t getNetworkMode() override;
uint32_t getNatTypeMode() override;
uint32_t getNatHoleMode() override;
uint32_t getConnectModes() override;

bool getConfigurationOption(uint32_t key, std::string &opt) override;
bool setConfigurationOption(uint32_t key, const std::string &opt) override;

	/* Operating Mode */
uint32_t getOperatingMode() override;
bool     setOperatingMode(uint32_t opMode) override;

int SetMaxDataRates( int downKb, int upKb ) override;
int GetMaxDataRates( int &downKb, int &upKb ) override;
int GetCurrentDataRates( float &inKb, float &outKb ) override;

/********************* ABOVE is RsConfig Interface *******/

	private:

bool switchToOperatingMode(uint32_t opMode);

bool findConfigurationOption(uint32_t key, std::string &keystr);

	p3PeerMgr *mPeerMgr;
	p3LinkMgr *mLinkMgr;
	p3NetMgr  *mNetMgr;
	pqihandler *mPqiHandler;
	p3GeneralConfig *mGeneralConfig;

	RsMutex configMtx;
	uint32_t mUserLevel; // store last one... will later be a config Item too.
	float mRateDownload;
	float mRateUpload;

	uint32_t mOpMode;
};

#endif
