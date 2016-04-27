#ifndef BITDHT_STANDARD_DHT_H
#define BITDHT_STANDARD_DHT_H

/*
 * bitdht/bdstddht.h
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


#include "bitdht/bdiface.h"

#define BITDHT_STANDARD_BUCKET_SIZE 10 // 20 too many per query?
#define BITDHT_STANDARD_BUCKET_SIZE_BITS 5

#define BITDHT_STANDARD_N_BUCKETS  BITDHT_KEY_BITLEN

#include <list>
#include <string>
#include <map>
#include <vector>




void bdStdRandomNodeId(bdNodeId *id);
void bdStdZeroNodeId(bdNodeId *id);

void bdStdRandomId(bdId *id);
int bdStdDistance(const bdNodeId *a, const bdNodeId *b, bdMetric *r);
int bdStdBucketDistance(const bdMetric *m);
int bdStdBucketDistance(const bdNodeId *a, const bdNodeId *b);

void bdStdRandomMidId(const bdNodeId *target, const bdNodeId *other, bdNodeId *mid);

int  bdStdLoadNodeId(bdNodeId *id, std::string input);

void bdStdPrintId(std::ostream &out, const bdId *a);
void bdStdPrintId(std::string &out, const bdId *a, bool append);
void bdStdPrintNodeId(std::ostream &out, const bdNodeId *a);
void bdStdPrintNodeId(std::string &out, const bdNodeId *a, bool append);

std::string bdStdConvertToPrintable(std::string input);

//uint32_t bdStdSimilarNode(const bdId*, const bdId*);


class bdStdDht: public bdDhtFunctions
{
	public:

        bdStdDht();
        /* setup variables */
uint16_t bdNumBuckets() override;
uint16_t bdNodesPerBucket() override; /* used for bdspace */
uint16_t bdNumQueryNodes() override; /* used for queries */
uint16_t bdBucketBitSize() override;

int bdDistance(const bdNodeId *n1, const bdNodeId *n2, bdMetric *metric) override;
int bdBucketDistance(const bdNodeId *n1, const bdNodeId *n2) override;
int bdBucketDistance(const bdMetric *metric) override;

bool bdSimilarId(const bdId *id1, const bdId *id2) override;
bool bdUpdateSimilarId(bdId *dest, const bdId *src) override; /* returns true if update was necessary */

void bdRandomMidId(const bdNodeId *target, const bdNodeId *other, bdNodeId *mid) override;

void bdPrintId(std::ostream &out, const bdId *a) override;
void bdPrintNodeId(std::ostream &out, const bdNodeId *a) override;

};

class bdModDht: public bdStdDht
{
	public:
	bdModDht();
virtual void setNodesPerBucket(uint16_t nodesPerBucket);
uint16_t bdNodesPerBucket() override; /* used for bdspace */

	private:
	uint16_t mNodesPerBucket;
};


#endif

