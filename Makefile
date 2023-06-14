TARGET = EEDDExample.so
GATEWAY_TARGET =

SDKDIR = ../../../..
SOURCES 	= 	EDDAlarm.cpp	\
			EDDBrowser.cpp	\
			EDDConfig.cpp	\
			EDDDataItem.cpp	\
			EDDDriver.cpp	\
			EDDEvent.cpp	\
			EDDExports.cpp	\
			EDDMgr.cpp	\
			EDDUpload.cpp	\
			SimulatedDataItem.cpp
			

DEBUG = 0
GATEWAY = 0

include $(SDKDIR)/Rules.make
