##
## Author: John Ogness
##
## This file has been put into the public domain.
## You can do whatever you want with this file.
##

SUBDIRS = api dbus

if COND_COREINJECT
SUBDIRS += coreinject
endif

if COND_MINICOREDUMPER
SUBDIRS += minicoredumper
endif

if COND_MINICOREDUMPER_DBUSD
SUBDIRS += minicoredumper_dbusd
endif

if COND_LIBMINICOREDUMPER
SUBDIRS += libminicoredumper
endif

if COND_MINICOREDUMPER_DEMO
SUBDIRS += minicoredumper_demo
endif

if COND_MINICOREDUMPER_TRIGGER
SUBDIRS += minicoredumper_trigger
endif

SUBDIRS += example example++