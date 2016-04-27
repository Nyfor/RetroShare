/*
 * libretroshare/src/ft: ftserver.h
 *
 * File Transfer for RetroShare.
 *
 * Copyright 2008 by Robert Fernie.
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

#ifndef FT_SERVER_HEADER
#define FT_SERVER_HEADER

/*
 * ftServer.
 *
 * Top level File Transfer interface.
 * (replaces old filedexserver)
 *
 * sets up the whole File Transfer class structure.
 * sets up the File Indexing side of cache system too.
 *
 * provides rsFiles interface for external control.
 *
 */

#include <map>
#include <list>
#include <iostream>

#include "ft/ftdata.h"
#include "turtle/turtleclientservice.h"
#include "services/p3service.h"
#include "retroshare/rsfiles.h"
//#include "dbase/cachestrapper.h"

#include "pqi/pqi.h"
#include "pqi/p3cfgmgr.h"

class p3ConnectMgr;

class CacheStrapper;
class CacheTransfer;

class ftCacheStrapper;
class ftFiStore;
class ftFiMonitor;

class ftController;
class ftExtraList;
class ftFileSearch;

class ftDataMultiplex;
class p3turtle;

class p3PeerMgr;
class p3ServiceControl;

class ftServer: public p3Service, public RsFiles, public ftDataSend, public RsTurtleClientService
{

	public:

		/***************************************************************/
		/******************** Setup ************************************/
		/***************************************************************/

		ftServer(p3PeerMgr *peerMgr, p3ServiceControl *serviceCtrl);
		RsServiceInfo getServiceInfo() override;

		/* Assign important variables */
		void	setConfigDirectory(std::string path);

		/* add Config Items (Extra, Controller) */
		void	addConfigComponents(p3ConfigMgr *mgr);

		CacheStrapper *getCacheStrapper() override;
		CacheTransfer *getCacheTransfer() override;

		const RsPeerId& OwnId();

		/* Final Setup (once everything is assigned) */
		void    SetupFtServer() ;
		void    connectToTurtleRouter(p3turtle *p) override ;

		// Checks that the given hash is well formed. Used to chase 
		// string bugs.
		static bool checkHash(const RsFileHash& hash,std::string& error_string) ;

		// Implements RsTurtleClientService
		//
		bool handleTunnelRequest(const RsFileHash& hash,const RsPeerId& peer_id) override ;
		void receiveTurtleData(RsTurtleGenericTunnelItem *item,const RsFileHash& hash,const RsPeerId& virtual_peer_id,RsTurtleGenericTunnelItem::Direction direction) override ;
		RsTurtleGenericTunnelItem *deserialiseItem(void *data,uint32_t size) const override ;

		void addVirtualPeer(const TurtleFileHash&, const TurtleVirtualPeerId&,RsTurtleGenericTunnelItem::Direction dir) override ;
		void removeVirtualPeer(const TurtleFileHash&, const TurtleVirtualPeerId&) override ;

		/***************************************************************/
		/*************** Control Interface *****************************/
		/************** (Implements RsFiles) ***************************/
		/***************************************************************/

		void StartupThreads();
		void StopThreads();

		// member access

		ftDataMultiplex *getMultiplexer() const { return mFtDataplex ; }
		ftController *getController() const { return mFtController ; }

        /**
         * @see RsFiles::getFileData
         */
        bool getFileData(const RsFileHash& hash, uint64_t offset, uint32_t& requested_size,uint8_t *data) override;

		/***
		 * Control of Downloads
		 ***/
		bool alreadyHaveFile(const RsFileHash& hash, FileInfo &info) override;
		bool FileRequest(const std::string& fname, const RsFileHash& hash, uint64_t size, const std::string& dest, TransferRequestFlags flags, const std::list<RsPeerId>& srcIds) override;
		bool FileCancel(const RsFileHash& hash) override;
		bool FileControl(const RsFileHash& hash, uint32_t flags) override;
		bool FileClearCompleted() override;
		bool setDestinationDirectory(const RsFileHash& hash,const std::string& new_path) override ;
		bool setDestinationName(const RsFileHash& hash,const std::string& new_name) override ;
		bool setChunkStrategy(const RsFileHash& hash,FileChunksInfo::ChunkStrategy s) override ;
		void setDefaultChunkStrategy(FileChunksInfo::ChunkStrategy) override ;
		FileChunksInfo::ChunkStrategy defaultChunkStrategy() override ;
		uint32_t freeDiskSpaceLimit() const override ;
		void setFreeDiskSpaceLimit(uint32_t size_in_mb) override ;


		/***
		 * Control of Downloads Priority.
		 ***/
		uint32_t getMinPrioritizedTransfers() override ;
		void setMinPrioritizedTransfers(uint32_t s) override ;
		uint32_t getQueueSize() override ;
		void setQueueSize(uint32_t s) override ;
		bool changeQueuePosition(const RsFileHash& hash, QueueMove queue_mv) override;
		bool changeDownloadSpeed(const RsFileHash& hash, int speed) override;
		bool getDownloadSpeed(const RsFileHash& hash, int & speed) override;
		bool clearDownload(const RsFileHash& hash) override;
		//virtual void getDwlDetails(std::list<DwlDetails> & details);

		/***
		 * Download/Upload Details
		 ***/
        void FileDownloads(std::list<RsFileHash> &hashs) override;
		bool FileUploads(std::list<RsFileHash> &hashs) override;
		bool FileDetails(const RsFileHash &hash, FileSearchFlags hintflags, FileInfo &info) override;
		bool FileDownloadChunksDetails(const RsFileHash& hash,FileChunksInfo& info) override ;
		bool FileUploadChunksDetails(const RsFileHash& hash,const RsPeerId& peer_id,CompressedChunkMap& map) override ;


		/***
		 * Extra List Access
		 ***/
		virtual bool ExtraFileAdd(std::string fname, const RsFileHash& hash, uint64_t size, uint32_t period, TransferRequestFlags flags);
		bool ExtraFileRemove(const RsFileHash& hash, TransferRequestFlags flags) override;
		bool ExtraFileHash(std::string localpath, uint32_t period, TransferRequestFlags flags) override;
		bool ExtraFileStatus(std::string localpath, FileInfo &info) override;
		bool ExtraFileMove(std::string fname, const RsFileHash& hash, uint64_t size, std::string destpath) override;


		/***
		 * Directory Listing / Search Interface
		 ***/
		int RequestDirDetails(const RsPeerId& uid, const std::string& path, DirDetails &details) override;
		int RequestDirDetails(void *ref, DirDetails &details, FileSearchFlags flags) override;
		uint32_t getType(void *ref,FileSearchFlags flags) override ;

		int SearchKeywords(std::list<std::string> keywords, std::list<DirDetails> &results,FileSearchFlags flags) override;
		int SearchKeywords(std::list<std::string> keywords, std::list<DirDetails> &results,FileSearchFlags flags,const RsPeerId& peer_id) override;
		int SearchBoolExp(Expression * exp, std::list<DirDetails> &results,FileSearchFlags flags) override;
		int SearchBoolExp(Expression * exp, std::list<DirDetails> &results,FileSearchFlags flags,const RsPeerId& peer_id) override;

		/***
		 * Utility Functions
		 ***/
		bool ConvertSharedFilePath(std::string path, std::string &fullpath) override;
		void ForceDirectoryCheck() override;
		void updateSinceGroupPermissionsChanged() override ;
		bool InDirectoryCheck() override;
		bool copyFile(const std::string& source, const std::string& dest) override;

		/***
		 * Directory Handling
		 ***/
		void	setDownloadDirectory(std::string path) override;
		void	setPartialsDirectory(std::string path) override;
		std::string getDownloadDirectory() override;
		std::string getPartialsDirectory() override;

		bool	getSharedDirectories(std::list<SharedDirInfo> &dirs) override;
		virtual bool	setSharedDirectories(std::list<SharedDirInfo> &dirs);
		bool 	addSharedDirectory(const SharedDirInfo& dir) override;
		bool   updateShareFlags(const SharedDirInfo& dir) override; 	// updates the flags. The directory should already exist !
		bool 	removeSharedDirectory(std::string dir) override;

		bool	getShareDownloadDirectory() override;
		bool 	shareDownloadDirectory(bool share) override;

		void	setRememberHashFilesDuration(uint32_t days) override ;
		uint32_t rememberHashFilesDuration() const override ;
		bool rememberHashFiles() const override ;
		void setRememberHashFiles(bool) override ;
		void   clearHashCache() override ;
		void setWatchPeriod(int minutes) override ;
		int watchPeriod() const override ;

		/***************************************************************/
		/*************** Control Interface *****************************/
		/***************************************************************/

		/***************************************************************/
		/*************** Data Transfer Interface ***********************/
		/***************************************************************/
	public:
        bool sendData(const RsPeerId& peerId, const RsFileHash& hash, uint64_t size, uint64_t offset, uint32_t chunksize, void *data) override;
        bool sendDataRequest(const RsPeerId& peerId, const RsFileHash& hash, uint64_t size, uint64_t offset, uint32_t chunksize) override;
        bool sendChunkMapRequest(const RsPeerId& peer_id,const RsFileHash& hash,bool is_client) override ;
        bool sendChunkMap(const RsPeerId& peer_id,const RsFileHash& hash,const CompressedChunkMap& cmap,bool is_client) override ;
        bool sendSingleChunkCRCRequest(const RsPeerId& peer_id,const RsFileHash& hash,uint32_t chunk_number) override ;
        bool sendSingleChunkCRC(const RsPeerId& peer_id,const RsFileHash& hash,uint32_t chunk_number,const Sha1CheckSum& crc) override ;

		/*************** Internal Transfer Fns *************************/
		int tick() override;

		/* Configuration */
		bool	addConfiguration(p3ConfigMgr *cfgmgr);
		bool	ResumeTransfers();

		/******************* p3 Config Overload ************************/
	protected:
		/* Key Functions to be overloaded for Full Configuration */
		virtual RsSerialiser *setupSerialiser();
		virtual bool saveList(bool &cleanup, std::list<RsItem *>&);
		virtual bool loadList(std::list<RsItem *>& load);

	private:
		bool  loadConfigMap(std::map<std::string, std::string> &configMap);
		/******************* p3 Config Overload ************************/

		/*************************** p3 Config Overload ********************/

	protected:
		int handleIncoming() ;
		bool handleCacheData() ;

	private:

		/**** INTERNAL FUNCTIONS ***/
		//virtual int 	reScanDirs();
		//virtual int 	check_dBUpdate();

	private:

		/* no need for Mutex protection -
		 * as each component is protected independently.
		 */
		p3PeerMgr *mPeerMgr;
		p3ServiceControl *mServiceCtrl;

		ftCacheStrapper *mCacheStrapper;
		ftFiStore 	*mFiStore;
		ftFiMonitor   	*mFiMon;

		ftController  *mFtController;
		ftExtraList   *mFtExtra;

		ftDataMultiplex *mFtDataplex;
		p3turtle *mTurtleRouter ;

		ftFileSearch   *mFtSearch;

		RsMutex srvMutex;
		std::string mConfigPath;
		std::string mDownloadPath;
		std::string mPartialsPath;
};



#endif
