#pragma once

#include <retroshare/rsnotify.h>
#include <util/rsthreads.h>

#include "ResourceRouter.h"
#include "StateTokenServer.h"

class RsIdentity;

namespace resource_api
{

class IdentityHandler: public ResourceRouter, NotifyClient
{
public:
    IdentityHandler(StateTokenServer* sts, RsNotify* notify, RsIdentity* identity);
    ~IdentityHandler() override;

    // from NotifyClient
    // note: this may get called from foreign threads
    void notifyGxsChange(const RsGxsChanges &changes) override;

private:
    void handleWildcard(Request& req, Response& resp);
    ResponseTask *handleOwn(Request& req, Response& resp);
    ResponseTask *handleCreateIdentity(Request& req, Response& resp);

    StateTokenServer* mStateTokenServer;
    RsNotify* mNotify;
    RsIdentity* mRsIdentity;

    RsMutex mMtx;
    StateToken mStateToken; // mutex protected
};
} // namespace resource_api
