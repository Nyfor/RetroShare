#ifndef RSPOSTEDITEMS_H
#define RSPOSTEDITEMS_H

#include "serialiser/rsserviceids.h"
#include "serialiser/rsserial.h"
//#include "serialiser/rstlvtypes.h"

#include "serialiser/rsgxscommentitems.h"

#include "rsgxsitems.h"
#include "retroshare/rsposted.h"

const uint8_t RS_PKT_SUBTYPE_POSTED_GRP_ITEM = 0x02;
const uint8_t RS_PKT_SUBTYPE_POSTED_POST_ITEM = 0x03;

class RsGxsPostedGroupItem : public RsGxsGrpItem
{
public:
	RsGxsPostedGroupItem() : RsGxsGrpItem(RS_SERVICE_GXS_TYPE_POSTED,
		 RS_PKT_SUBTYPE_POSTED_GRP_ITEM) { return; }
	~RsGxsPostedGroupItem() override { return; }

	void clear() override;
	std::ostream &print(std::ostream &out, uint16_t indent = 0) override;

	RsPostedGroup mGroup;
};

class RsGxsPostedPostItem : public RsGxsMsgItem
{
public:
	RsGxsPostedPostItem() : RsGxsMsgItem(RS_SERVICE_GXS_TYPE_POSTED,
		 RS_PKT_SUBTYPE_POSTED_POST_ITEM) {return ; }
	~RsGxsPostedPostItem() override { return; }

	void clear() override;
	std::ostream &print(std::ostream &out, uint16_t indent = 0) override;

	RsPostedPost mPost;
};

class RsGxsPostedSerialiser : public RsGxsCommentSerialiser
{

public:

	RsGxsPostedSerialiser()
	:RsGxsCommentSerialiser(RS_SERVICE_GXS_TYPE_POSTED)
	{ return; }

	~RsGxsPostedSerialiser() override { return; }

	uint32_t size(RsItem *item) override;
	bool serialise(RsItem *item, void *data, uint32_t *size) override;
	RsItem* deserialise(void *data, uint32_t *size) override;

private:

	uint32_t sizeGxsPostedGroupItem(RsGxsPostedGroupItem* item);
	bool serialiseGxsPostedGroupItem(RsGxsPostedGroupItem* item, void* data, uint32_t *size);
	RsGxsPostedGroupItem* deserialiseGxsPostedGroupItem(void *data, uint32_t *size);

	uint32_t sizeGxsPostedPostItem(RsGxsPostedPostItem* item);
	bool serialiseGxsPostedPostItem(RsGxsPostedPostItem* item, void* data, uint32_t *size);
	RsGxsPostedPostItem* deserialiseGxsPostedPostItem(void *data, uint32_t *size);
};


#endif // RSPOSTEDITEMS_H
