#pragma once

#include "ResourceRouter.h"
#include "StateTokenServer.h"
#include "ChatHandler.h"
#include <retroshare/rsnotify.h>
#include <util/rsthreads.h>

class RsPeers;
class RsMsgs;

namespace resource_api
{

class PeersHandler: public ResourceRouter, NotifyClient, Tickable, public UnreadMsgNotify
{
public:
    PeersHandler(StateTokenServer* sts, RsNotify* notify, RsPeers* peers, RsMsgs* msgs);
    ~PeersHandler() override;

    // from NotifyClient
    // note: this may get called from foreign threads
    void notifyListChange(int list, int type) override; // friends list change
    void notifyPeerHasNewAvatar(std::string /*peer_id*/) override;

    // from Tickable
    void tick() override;

    // from UnreadMsgNotify
    // ChatHandler calls this to tell us about unreadmsgs
    // this allows to merge unread msgs info with the peers list
    void notifyUnreadMsgCountChanged(const RsPeerId& peer, uint32_t count) override;

private:
    void handleWildcard(Request& req, Response& resp);
    void handleExamineCert(Request& req, Response& resp);

    // a helper which ensures proper mutex locking
    StateToken getCurrentStateToken();

    StateTokenServer* mStateTokenServer;
    RsNotify* mNotify;
    RsPeers* mRsPeers;
    RsMsgs* mRsMsgs; // required for avatar data

    std::list<RsPeerId> mOnlinePeers;

    RsMutex mMtx;
    StateToken mStateToken; // mutex protected
    std::map<RsPeerId, uint32_t> mUnreadMsgsCounts;
};
} // namespace resource_api
