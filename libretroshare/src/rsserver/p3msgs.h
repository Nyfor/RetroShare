#ifndef RS_P3MSG_INTERFACE_H
#define RS_P3MSG_INTERFACE_H

/*
 * libretroshare/src/rsserver: p3msgs.h
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2007-2008 by Robert Fernie.
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

#include "retroshare/rsmsgs.h"
#include "retroshare/rsgxsifacetypes.h"

class p3MsgService;
class p3ChatService;

class RsChatMsgItem;

//! provides retroshares chatservice and messaging service
/*!
 * Provides rs with the ability to send/receive messages, immediate status,
 * custom status, avatar and
 * chats (public(group) and private) to peers
 */
class p3Msgs: public RsMsgs 
{
   public:

          p3Msgs(p3MsgService *p3m, p3ChatService *p3c)
                 :mMsgSrv(p3m), mChatSrv(p3c) { return; }
	  ~p3Msgs() override { return; }

	  /****************************************/
	  /* Message Items */

	  /*!
	   * @param msgList ref to list summarising client's msgs
	   */
	  bool getMessageSummaries(std::list<Rs::Msgs::MsgInfoSummary> &msgList) override;
	  bool getMessage(const std::string &mId, Rs::Msgs::MessageInfo &msg) override;
	  void getMessageCount(unsigned int *pnInbox, unsigned int *pnInboxNew, unsigned int *pnOutbox, unsigned int *pnDraftbox, unsigned int *pnSentbox, unsigned int *pnTrashbox) override;

	  bool MessageSend(Rs::Msgs::MessageInfo &info) override;
	  bool SystemMessage(const std::string &title, const std::string &message, uint32_t systemFlag) override;
	  bool MessageToDraft(Rs::Msgs::MessageInfo &info, const std::string &msgParentId) override;
	  bool MessageToTrash(const std::string &mid, bool bTrash) override;
	  bool MessageDelete(const std::string &mid) override;
	  bool MessageRead(const std::string &mid, bool unreadByUser) override;
	  bool MessageReplied(const std::string &mid, bool replied) override;
	  bool MessageForwarded(const std::string &mid, bool forwarded) override;
	  bool MessageStar(const std::string &mid, bool star) override;
	  bool MessageLoadEmbeddedImages(const std::string &mid, bool load) override;
	  bool getMsgParentId(const std::string &msgId, std::string &msgParentId) override;

	  bool getMessageTagTypes(Rs::Msgs::MsgTagType& tags) override;
	  bool setMessageTagType(uint32_t tagId, std::string& text, uint32_t rgb_color) override;
	  bool removeMessageTagType(uint32_t tagId) override;

	  bool getMessageTag(const std::string &msgId, Rs::Msgs::MsgTagInfo& info) override;
	  /* set == false && tagId == 0 --> remove all */
	  bool setMessageTag(const std::string &msgId, uint32_t tagId, bool set) override;

	  bool resetMessageStandardTagTypes(Rs::Msgs::MsgTagType& tags) override;

	  uint32_t getDistantMessagingPermissionFlags() override ;
	  void setDistantMessagingPermissionFlags(uint32_t flags) override ;

	  /*!
	   * gets avatar from peer, image data in jpeg format
	   */
	  void getAvatarData(const RsPeerId& pid,unsigned char *& data,int& size) override;

	  /*!
	   * sets clients avatar, image data should be in jpeg format
	   */
	  void setOwnAvatarData(const unsigned char *data,int size) override;

	  /*!
	   * retrieve clients avatar, image data in jpeg format
	   */
	  void getOwnAvatarData(unsigned char *& data,int& size) override;

	  /*!
	   * sets clients custom status (e.g. "i'm tired")
	   */
	  void setCustomStateString(const std::string&  status_string) override ;

	  /*!
	   * retrieves client's custom status
	   */
	  std::string getCustomStateString() override ;

	  /*!
	   * retrieves peer's custom status
	   */
	  std::string getCustomStateString(const RsPeerId& peer_id) override ;
	  

	  /*!
       * Send a chat message.
       * @param destination where to send the chat message
       * @param msg the message
       * @see ChatId
	   */
      bool sendChat(ChatId destination, std::string msg) override ;

	  /*!
	   * Return the max message size for security forwarding
	   */
	  uint32_t getMaxMessageSecuritySize(int type) override;

    /*!
     * sends immediate status string to a specific peer, e.g. in a private chat
     * @param chat_id chat id to send status string to
     * @param status_string immediate status to send
     */
    void    sendStatusString(const ChatId& id, const std::string& status_string) override ;

    /**
     * @brief clearChatLobby: Signal chat was cleared by GUI.
     * @param id: Chat id cleared.
     */
    void clearChatLobby(const ChatId &id) override;

	  /****************************************/

      bool joinVisibleChatLobby(const ChatLobbyId& id, const RsGxsId &own_id) override ;
	  void getListOfNearbyChatLobbies(std::vector<VisibleChatLobbyRecord>& public_lobbies) override ;
      void getChatLobbyList(std::list<ChatLobbyId>& cl_list) override ;
      bool getChatLobbyInfo(const ChatLobbyId& id,ChatLobbyInfo& info) override ;
      void invitePeerToLobby(const ChatLobbyId&, const RsPeerId&) override ;
      bool acceptLobbyInvite(const ChatLobbyId& id, const RsGxsId &gxs_id) override ;
	  void denyLobbyInvite(const ChatLobbyId& id) override ;
	  void getPendingChatLobbyInvites(std::list<ChatLobbyInvite>& invites) override ;
	  void unsubscribeChatLobby(const ChatLobbyId& lobby_id) override ;
      bool setIdentityForChatLobby(const ChatLobbyId& lobby_id,const RsGxsId&) override ;
      bool getIdentityForChatLobby(const ChatLobbyId&,RsGxsId& nick) override ;
      bool setDefaultIdentityForChatLobby(const RsGxsId&) override ;
      void getDefaultIdentityForChatLobby(RsGxsId& nick) override ;
    void setLobbyAutoSubscribe(const ChatLobbyId& lobby_id, const bool autoSubscribe) override;
    bool getLobbyAutoSubscribe(const ChatLobbyId& lobby_id) override;
      ChatLobbyId createChatLobby(const std::string& lobby_name,const RsGxsId& lobby_identity,const std::string& lobby_topic,const std::set<RsPeerId>& invited_friends,ChatLobbyFlags privacy_type) override ;

      bool initiateDistantChatConnexion(const RsGxsId& to_gxs_id, const RsGxsId& from_gxs_id, DistantChatPeerId &pid, uint32_t& error_code) override ;
      bool getDistantChatStatus(const DistantChatPeerId& gxs_id,DistantChatPeerInfo& info) override;
      bool closeDistantChatConnexion(const DistantChatPeerId &pid) override ;

    uint32_t getDistantChatPermissionFlags() override ;
    bool setDistantChatPermissionFlags(uint32_t flags) override ;
    
   private:

	  p3MsgService  *mMsgSrv;
	  p3ChatService *mChatSrv;
};


#endif
