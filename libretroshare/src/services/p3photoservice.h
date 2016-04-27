#ifndef P3PHOTOSERVICEV2_H
#define P3PHOTOSERVICEV2_H

/*
 * libretroshare/src/retroshare: rsphoto.h
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2008-2012 by Robert Fernie, Christopher Evi-Parker
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

#include "gxs/rsgenexchange.h"
#include "retroshare/rsphoto.h"

class p3PhotoService : public RsPhoto, public RsGenExchange
{
public:

    p3PhotoService(RsGeneralDataService* gds, RsNetworkExchangeService* nes, RsGixs* gixs);
RsServiceInfo getServiceInfo() override;

static uint32_t photoAuthenPolicy();

public:

    /*!
     * @return true if a change has occured
     */
    bool updated() override;

    /*!
     *
     */
    void service_tick() override;

protected:

    void notifyChanges(std::vector<RsGxsNotify*>& changes) override;
public:

    /** Requests **/

    void groupsChanged(std::list<RsGxsGroupId>& grpIds) override;


    void msgsChanged(std::map<RsGxsGroupId,
                             std::vector<RsGxsMessageId> >& msgs) override;

    RsTokenService* getTokenService() override;

    bool getGroupList(const uint32_t &token,
                              std::list<RsGxsGroupId> &groupIds) override;
    bool getMsgList(const uint32_t &token,
                            GxsMsgIdResult& msgIds) override;

    /* Generic Summary */
    bool getGroupSummary(const uint32_t &token,
                                 std::list<RsGroupMetaData> &groupInfo) override;

    bool getMsgSummary(const uint32_t &token,
                               MsgMetaResult &msgInfo) override;

    /* Specific Service Data */
    bool getAlbum(const uint32_t &token, std::vector<RsPhotoAlbum> &albums) override;
    bool getPhoto(const uint32_t &token, PhotoResult &photos) override;
    bool getPhotoComment(const uint32_t &token, PhotoCommentResult &comments) override;
    bool getPhotoRelatedComment(const uint32_t &token, PhotoRelatedCommentResult &comments) override;

public:

    /** Modifications **/

    /*!
     * submits album, which returns a token that needs
     * to be acknowledge to get album grp id
     * @param token token to redeem for acknowledgement
     * @param album album to be submitted
     */
    bool submitAlbumDetails(uint32_t& token, RsPhotoAlbum &album) override;

    /*!
     * submits photo, which returns a token that needs
     * to be acknowledge to get photo msg-grp id pair
     * @param token token to redeem for acknowledgement
     * @param photo photo to be submitted
     */
    bool submitPhoto(uint32_t& token, RsPhotoPhoto &photo) override;

    /*!
     * submits photo comment, which returns a token that needs
     * to be acknowledged to get photo msg-grp id pair
     * The mParentId needs to be set to an existing msg for which
     * commenting is enabled
     * @param token token to redeem for acknowledgement
     * @param comment comment to be submitted
     */
    bool submitComment(uint32_t& token, RsPhotoComment &photo) override;

    /*!
     * subscribes to group, and returns token which can be used
     * to be acknowledged to get group Id
     * @param token token to redeem for acknowledgement
     * @param grpId the id of the group to subscribe to
     */
    bool subscribeToAlbum(uint32_t& token, const RsGxsGroupId& grpId, bool subscribe) override;

    /*!
     * This allows the client service to acknowledge that their msgs has
     * been created/modified and retrieve the create/modified msg ids
     * @param token the token related to modification/create request
     * @param msgIds map of grpid->msgIds of message created/modified
     * @return true if token exists false otherwise
     */
    bool acknowledgeMsg(const uint32_t& token, std::pair<RsGxsGroupId, RsGxsMessageId>& msgId) override;

    /*!
	 * This allows the client service to acknowledge that their grps has
	 * been created/modified and retrieve the create/modified grp ids
	 * @param token the token related to modification/create request
	 * @param msgIds vector of ids of groups created/modified
	 * @return true if token exists false otherwise
	 */
    bool acknowledgeGrp(const uint32_t& token, RsGxsGroupId& grpId) override;

private:

    std::vector<RsGxsGroupChange*> mGroupChange;
    std::vector<RsGxsMsgChange*> mMsgChange;

    RsMutex mPhotoMutex;
};

#endif // P3PHOTOSERVICEV2_H
