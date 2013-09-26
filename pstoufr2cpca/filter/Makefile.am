filterdir = $(libdir)/cups/filter

filter_PROGRAMS = pstoufr2cpca

pstoufr2cpca_SOURCES = \
	pstoufr2cpca.c paramlist.c paramlist.h

pstoufr2cpca_LDADD = -lbuftool -lcups 

AM_CFLAGS = -O2 -Wall -fPIC
