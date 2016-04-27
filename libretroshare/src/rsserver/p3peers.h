#ifndef RETROSHARE_P3_PEER_INTERFACE_H
#define RETROSHARE_P3_PEER_INTERFACE_H

/*
 * libretroshare/src/rsserver: p3peers.h
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2004-2008 by Robert Fernie.
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

#include "retroshare/rspeers.h"
class p3LinkMgr;
class p3PeerMgr;
class p3NetMgr;
struct sockaddr_storage;


class p3Peers: public RsPeers 
{
public:

	p3Peers(p3LinkMgr *lm, p3PeerMgr *pm, p3NetMgr *nm);
	~p3Peers() override {}

	/* Updates ... */
	virtual bool FriendsChanged();
	virtual bool OthersChanged();

	/* Peer Details (Net & Auth) */
	const RsPeerId& getOwnId() override;

	bool haveSecretKey(const RsPgpId& gpg_id) override ;

	bool getOnlineList(std::list<RsPeerId> &ids) override;
	bool getFriendList(std::list<RsPeerId> &ids) override;
	bool getPeerCount (unsigned int *friendCount, unsigned int *onlineCount, bool ssl) override;

	bool isOnline(const RsPeerId &id) override;
	bool isFriend(const RsPeerId &id) override;
	bool isGPGAccepted(const RsPgpId &gpg_id_is_friend) override;
	std::string getGPGName(const RsPgpId &gpg_id) override;
	std::string getPeerName(const RsPeerId& ssl_or_gpg_id) override;
	bool getPeerDetails(const RsPeerId& ssl_or_gpg_id, RsPeerDetails &d) override;

	/* Using PGP Ids */
	const RsPgpId& getGPGOwnId() override;
	RsPgpId getGPGId(const RsPeerId &ssl_id) override;
	bool isKeySupported(const RsPgpId& ids) override;
	bool getGPGAcceptedList(std::list<RsPgpId> &ids) override;
	bool getGPGSignedList(std::list<RsPgpId> &ids) override;
	bool getGPGValidList(std::list<RsPgpId> &ids) override;
	bool getGPGAllList(std::list<RsPgpId> &ids) override;
	bool getGPGDetails(const RsPgpId &id, RsPeerDetails &d) override;
	bool getAssociatedSSLIds(const RsPgpId& gpg_id, std::list<RsPeerId> &ids) override;
	bool gpgSignData(const void *data, const uint32_t len, unsigned char *sign, unsigned int *signlen) override ;

	/* Add/Remove Friends */
		bool addFriend(const RsPeerId &ssl_id, const RsPgpId &gpg_id,ServicePermissionFlags flags = RS_NODE_PERM_DEFAULT) override;
		bool removeFriend(const RsPgpId& gpgid) override;
	bool removeFriendLocation(const RsPeerId& sslId) override;

	/* keyring management */
	bool removeKeysFromPGPKeyring(const std::set<RsPgpId> &pgp_ids,std::string& backup_file,uint32_t& error_code) override;

	/* Network Stuff */
		bool connectAttempt(const RsPeerId &id) override;
	bool setLocation(const RsPeerId &ssl_id, const std::string &location) override;//location is shown in the gui to differentiate ssl certs
	bool setHiddenNode(const RsPeerId &id, const std::string &hidden_node_address) override;
	bool setHiddenNode(const RsPeerId &id, const std::string &address, uint16_t port) override;

		bool setLocalAddress(const RsPeerId &id, const std::string &addr, uint16_t port) override;
		bool setExtAddress(const RsPeerId &id, const std::string &addr, uint16_t port) override;
		bool setDynDNS(const RsPeerId &id, const std::string &dyndns) override;
		bool setNetworkMode(const RsPeerId &id, uint32_t netMode) override;
	bool setVisState(const RsPeerId &id, uint16_t vs_disc, uint16_t vs_dht) override;

	bool getProxyServer(const uint32_t type, std::string &addr, uint16_t &port,uint32_t& status) override;
	bool setProxyServer(const uint32_t type, const std::string &addr, const uint16_t port) override;
	virtual bool isProxyAddress(const uint32_t type, const sockaddr_storage &addr);

	void getIPServersList(std::list<std::string>& ip_servers) override;
	void allowServerIPDetermination(bool) override;
	bool getAllowServerIPDetermination() override;
	bool resetOwnExternalAddressList() override;

	/* Auth Stuff */
	// Get the invitation (GPG cert + local/ext address + SSL id for the given peer)
		std::string GetRetroshareInvite(const RsPeerId& ssl_id,bool include_signatures) override;
		std::string getPGPKey(const RsPgpId& pgp_id,bool include_signatures) override ;

	// same but for own id
		std::string GetRetroshareInvite(bool include_signatures) override;
	bool GetPGPBase64StringAndCheckSum(const RsPgpId& gpg_id,std::string& gpg_base64_string,std::string& gpg_base64_checksum) override;

	bool hasExportMinimal() override;

		bool loadCertificateFromString(const std::string& cert, RsPeerId& ssl_id,RsPgpId& pgp_id, std::string& error_string) override;
		bool loadDetailsFromStringCert(const std::string &cert, RsPeerDetails &pd, uint32_t& error_code) override;

		bool cleanCertificate(const std::string &certstr, std::string &cleanCert,int& error_code) override;
		bool saveCertificateToFile(const RsPeerId &id, const std::string &fname) override;
		std::string saveCertificateToString(const RsPeerId &id) override;

		bool signGPGCertificate(const RsPgpId &id) override;
		bool trustGPGCertificate(const RsPgpId &id, uint32_t trustlvl) override;

	/* Group Stuff */
	bool addGroup(RsGroupInfo &groupInfo) override;
	bool editGroup(const std::string &groupId, RsGroupInfo &groupInfo) override;
	bool removeGroup(const std::string &groupId) override;
	bool getGroupInfo(const std::string &groupId, RsGroupInfo &groupInfo) override;
	bool getGroupInfoList(std::list<RsGroupInfo> &groupInfoList) override;
	bool assignPeerToGroup(const std::string &groupId, const RsPgpId &peerId, bool assign) override;
	bool assignPeersToGroup(const std::string &groupId, const std::list<RsPgpId>& peerIds, bool assign) override;

	FileSearchFlags computePeerPermissionFlags(const RsPeerId& peer_id,FileStorageFlags share_flags,const std::list<std::string>& parent_groups) override;

	// service permission stuff

	ServicePermissionFlags servicePermissionFlags(const RsPgpId& gpg_id) override;
	ServicePermissionFlags servicePermissionFlags(const RsPeerId & ssl_id) override;
	void setServicePermissionFlags(const RsPgpId& gpg_id,const ServicePermissionFlags& flags) override;

private:

	p3LinkMgr *mLinkMgr;
	p3PeerMgr *mPeerMgr;
	p3NetMgr *mNetMgr;
};

#endif
