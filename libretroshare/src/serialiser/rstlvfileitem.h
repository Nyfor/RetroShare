#pragma once

/*
 * libretroshare/src/serialiser: rstlvfileitem.h
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
#include "serialiser/rstlvidset.h"
#include "serialiser/rstlvbinary.h"

class RsTlvFileItem: public RsTlvItem
{
	public:
	 RsTlvFileItem();
~RsTlvFileItem() override { return; }
uint32_t TlvSize() const override;
void	 TlvClear() override;
bool     SetTlv(void *data, uint32_t size, uint32_t *offset) const override; 
bool     GetTlv(void *data, uint32_t size, uint32_t *offset) override; 
std::ostream &print(std::ostream &out, uint16_t indent) const override;

	uint64_t filesize; /// Mandatory: size of file to be downloaded
    RsFileHash hash;  /// Mandatory: to find file
	std::string name;  /// Optional: name of file
	std::string path;  /// Optional: path on host computer
	uint32_t    pop;   /// Optional: Popularity of file
	uint32_t    age;   /// Optional: age of file
	// For chunk hashing.
	uint32_t    piecesize;  /// Optional: bytes/piece for hashset.
	RsTlvHashSet hashset; /// Optional: chunk hashes.

};

class RsTlvFileSet: public RsTlvItem
{
	public:
	 RsTlvFileSet() { return; }
~RsTlvFileSet() override { return; }
uint32_t TlvSize() const override;
void	 TlvClear() override;
bool     SetTlv(void *data, uint32_t size, uint32_t *offset) const override; 
bool     GetTlv(void *data, uint32_t size, uint32_t *offset) override; 
std::ostream &print(std::ostream &out, uint16_t indent) const override;

	std::list<RsTlvFileItem> items; /// Mandatory 
	std::string title;   		/// Optional: title of file set
	std::string comment; 		/// Optional: comments for file
};


class RsTlvFileData: public RsTlvItem
{
	public:
	 RsTlvFileData(); 
	 ~RsTlvFileData() override { return; }
uint32_t TlvSize() const override;
void	 TlvClear() override;
bool     SetTlv(void *data, uint32_t size, uint32_t *offset) const override; 
bool     GetTlv(void *data, uint32_t size, uint32_t *offset) override; 
std::ostream &print(std::ostream &out, uint16_t indent) const override;

	RsTlvFileItem   file;         /// Mandatory: file information	
	uint64_t        file_offset;  /// Mandatory: where to start in bin data
	RsTlvBinaryData binData;      /// Mandatory: serialised file info
};


