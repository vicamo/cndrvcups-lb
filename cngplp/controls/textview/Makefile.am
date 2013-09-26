noinst_LTLIBRARIES=libufr2textview.la

libufr2textview_la_SOURCES=textview.c textview.h

INCLUDES=-I$(top_srcdir)/cngplpmod\
	 -I$(top_srcdir)\
         @PACKAGE_CFLAGS@\
         @GLADE_PACKAGE_CFLAGS@

AM_CFLAGS= -Wall -O2 -fPIC
libufr2textview_la_LIBADD=@PACKAGE_LIBS@ @GLADE_PACKAGE_LIBS@ #$(top_srcdir)/cngplpmod/libcngplpmod.la
