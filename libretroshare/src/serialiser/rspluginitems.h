/*
 * libretroshare/src/services: p3turtle.h
 *
 * Services for RetroShare.
 *
 * Copyright 2009 by Cyril Soler
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
 * Please report all bugs and problems to "csoler@users.sourceforge.net".
 *
 */


#pragma once

#include "serialiser/rsserial.h"
#include "serialiser/rsconfigitems.h"
#include "serialiser/rsbaseserial.h"

#if 0
#include "serialiser/rstlvbase.h"
#include "serialiser/rstlvtypes.h"
#endif

const uint8_t RS_PKT_CLASS_PLUGIN_SUBTYPE_HASHSET = 0x01 ;

class RsPluginItem: public RsItem
{
	public:
		RsPluginItem(uint8_t plugin_item_subtype): RsItem(RS_PKT_VERSION1,RS_PKT_CLASS_CONFIG,RS_PKT_TYPE_PLUGIN_CONFIG,plugin_item_subtype) {}
		~RsPluginItem() override {}

		virtual bool serialise(void *data,uint32_t& size) = 0 ;	// Isn't it better that items can serialise themselves ?
		virtual uint32_t serial_size() = 0 ; 							// deserialise is handled using a constructor

		void clear() override {} 
};

class RsPluginSerialiser: public RsSerialType
{
	public:
		RsPluginSerialiser() : RsSerialType(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG, RS_PKT_TYPE_PLUGIN_CONFIG) {}

		uint32_t 	size (RsItem *item) override 
		{ 
			return dynamic_cast<RsPluginItem *>(item)->serial_size() ;
		}
		bool serialise(RsItem *item, void *data, uint32_t *size) override 
		{ 
			return dynamic_cast<RsPluginItem *>(item)->serialise(data,*size) ;
		}
		RsItem *deserialise (void *data, uint32_t *size) override ;
};

class RsPluginHashSetItem: public RsPluginItem
{
	public:
		RsPluginHashSetItem() : RsPluginItem(RS_PKT_CLASS_PLUGIN_SUBTYPE_HASHSET) {}
		RsPluginHashSetItem(void *data,uint32_t size) ;

		RsTlvHashSet hashes ;

		std::ostream& print(std::ostream& o, uint16_t) override ;

	protected:
		bool serialise(void *data,uint32_t& size) override ;
		uint32_t serial_size() override ;
};


