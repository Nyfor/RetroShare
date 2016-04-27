/*
 * "$Id: pqibin.h,v 1.2 2007-02-18 21:46:49 rmf24 Exp $"
 *
 * 3P/PQI network interface for RetroShare.
 *
 * Copyright 2004-2006 by Robert Fernie.
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



#ifndef PQI_BIN_INTERFACE_HEADER
#define PQI_BIN_INTERFACE_HEADER


#include "pqi/pqi_base.h"
#include "pqi/pqihash.h"

#include <stdio.h>


//! handles writing to binary files
/*!
 * This allows for handling of binary file within Rs, In particular writing
 * binary data to file declared in constructor
 */
class BinFileInterface: public BinInterface
{
public:
	BinFileInterface(const char *fname, int flags);
~BinFileInterface() override;

int     tick() override { return 1; }

int	senddata(void *data, int len) override;
int	readdata(void *data, int len) override;
int	netstatus() override { return 1;}
int	isactive() override  { return (buf != NULL);}
bool	moretoread(uint32_t /* usec */ ) override 
	{ 
        if ((buf) && (bin_flags & BIN_FLAGS_READABLE))
		{
			if ((size - ftell(buf)) > 0)
			{
				return true;
			}
		}
		return false;
	}

int	close() override;
bool 	cansend(uint32_t /* usec */) override 
	{ 
        return (bin_flags & BIN_FLAGS_WRITEABLE);
	}
bool    bandwidthLimited() override { return false; }

//! if HASHing is switched on
RsFileHash gethash() override;
uint64_t bytecount() override;

protected:
	virtual int getFileSize();

private:
	int   bin_flags;
	FILE *buf;
	int   size;
	pqihash *hash;
	uint64_t bcount;
};

/*!
 * use this for writing encrypted data to file using user's (ownid) ssl key
 * hash for encrypted data is calculated, not the unencrypted data
 */
class BinEncryptedFileInterface : public BinFileInterface
{
public:

	BinEncryptedFileInterface(const char *fname, int flags);
	~BinEncryptedFileInterface() override;

	/*!
	 * pls note if hashing is on, it is the hash of the encrypted data that is calculated
	 * also note, sif data is sent more than once, in a single instance of this object, then you will need to store the
	 * encrypted file size for each send externally as they are encrypted with different random keys.
	 * @param data data to be sent
	 * @param len length of data in bytes
	 * @return -1 if failed to write data, if not number bytes sent to file is returned
	 */
	int	senddata(void *data, int len) override;

	/*!
	 * uses the hash of the encrypted data
	 * @param location to place data
	 * @param the length of data to be read
	 * @return -1 if failed to write data, if not number of bytes read is returned
	 */
	int	readdata(void *data, int len) override;

	/*!
	 * this releases resources held by an instance
	 */
	int	close() override;

	uint64_t bytecount() override;
	bool	moretoread(uint32_t usec) override;

private:

	char* data;
	bool haveData;
	int sizeData;
	uint64_t cpyCount;

};

//! handles writing to reading/writing to memory
/*!
 * This provide a memory interface for storing/retrieving information in memory
 * to be read/sent elsewhere
 */
class BinMemInterface: public BinInterface
{
public:
	BinMemInterface(int defsize, int flags);
	BinMemInterface(const void *data, const int size, int flags);
~BinMemInterface() override;

	/* Extra Interfaces */
int	fseek(int loc);
int	memsize() { return recvsize; }
void   *memptr() { return buf; }

	/* file interface */
bool	writetofile(const char *fname);
bool	readfromfile(const char *fname);

int     tick() override { return 1; }

int	senddata(void *data, int len) override;
int	readdata(void *data, int len) override;
int	netstatus() override { return 1; }
int	isactive() override  { return 1; }
bool	moretoread(uint32_t /* usec */) override 
	{ 
        if ((buf) && (bin_flags & BIN_FLAGS_READABLE ))
		{
			if (readloc < recvsize)
			{
				return true;
			}
		}
		return false;
	}

int	close() override;
bool 	cansend(uint32_t /* usec */) override    
	{ 
        return (bin_flags & BIN_FLAGS_WRITEABLE);
	}
bool    bandwidthLimited() override { return false; }

RsFileHash gethash() override;
uint64_t bytecount() override;

	private:
	int   bin_flags;
	void *buf;
	int   size;
	int   recvsize;
	int   readloc;
	pqihash *hash;
	uint64_t bcount;
};


class NetBinDummy: public NetBinInterface
{
public:
	NetBinDummy(PQInterface *parent, const RsPeerId& id, uint32_t t);
~NetBinDummy() override { return; }

	// Net Interface
int connect(const struct sockaddr_storage &raddr) override;
int listen() override;
int stoplistening() override;
int disconnect() override;
int reset() override;
bool connect_parameter(uint32_t type, uint32_t value) override 
	{ 
		(void) type; /* suppress unused parameter warning */
		(void) value; /* suppress unused parameter warning */
		return false; 
	}
int getConnectAddress(struct sockaddr_storage &raddr) override 
	{
		(void) raddr; /* suppress unused parameter warning */
		return 0;
	}

	// Bin Interface.
int     tick() override;

int     senddata(void *data, int len) override;
int     readdata(void *data, int len) override;
int     netstatus() override;
int     isactive() override;
bool    moretoread(uint32_t usec) override;
bool    cansend(uint32_t usec) override;
int	close() override;

RsFileHash gethash() override;

	private:
	uint32_t type;
	bool 	 dummyConnected;
        bool     toConnect;
	uint32_t connectDelta;
	time_t   connectTS;
};


#endif // PQI_BINARY_INTERFACES_HEADER

