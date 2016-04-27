#pragma once

/*
 * libretroshare/src/serialiser: rstlvimage.h
 *
 * RetroShare Serialiser.
 *
 * Copyright 2007-2008 by Robert Fernie, Chris Parker
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

/*******************************************************************
 * These are the Compound TLV structures that must be (un)packed.
 *
 ******************************************************************/

#include "serialiser/rstlvitem.h"
#include "serialiser/rstlvbinary.h"
#include "serialiser/rstlvbase.h"
#include "serialiser/rsbaseserial.h"

class RsTlvImage: public RsTlvItem
{
	public:
	 RsTlvImage(); 
	 RsTlvImage(const RsTlvImage& );
	 ~RsTlvImage() override { return; }
uint32_t TlvSize() const override;
void	 TlvClear() override;
bool     SetTlv(void *data, uint32_t size, uint32_t *offset) const override;
bool     GetTlv(void *data, uint32_t size, uint32_t *offset) override;

std::ostream &print(std::ostream &out, uint16_t indent) const override;

	uint32_t        image_type;   // Mandatory: 
	RsTlvBinaryData binData;      // Mandatory: serialised file info
};


