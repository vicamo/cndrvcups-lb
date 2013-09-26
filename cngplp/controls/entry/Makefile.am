noinst_LTLIBRARIES=libufr2entry.la

libufr2entry_la_SOURCES=entry.c entry.h

INCLUDES=-I$(top_srcdir)/cngplpmod\
	 -I$(top_srcdir)\
         @PACKAGE_CFLAGS@\
         @GLADE_PACKAGE_CFLAGS@

AM_CFLAGS= -Wall -O2 -fPIC
libufr2entry_la_LIBADD=@PACKAGE_LIBS@ @GLADE_PACKAGE_LIBS@ #$(top_srcdir)/cngplpmod/libcngplpmod.la
