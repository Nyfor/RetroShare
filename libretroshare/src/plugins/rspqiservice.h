#pragma once

#include "services/p3service.h"
#include "plugins/pluginmanager.h"

// The following class abstracts the construction of a peer-to-peer service. The user only has to 
// supply RS with a type ID. If the ID is already in use, RS will complain.
//
class RsPQIService: public p3Service, public p3Config
{
	public:
        RsPQIService(uint16_t type,uint32_t tick_delay_in_seconds, RsPluginHandler* pgHandler) ;

		uint32_t tickDelay() const { return _tick_delay_in_seconds ; }
		int tick() override = 0 ;

		// Functions from p3config
		//
		RsSerialiser *setupSerialiser() override { return NULL ; }
		bool saveList(bool&, std::list<RsItem*>&) override =0 ;
		bool loadList(std::list<RsItem*>&) override =0 ;

	private:
		uint32_t _tick_delay_in_seconds ;
};

