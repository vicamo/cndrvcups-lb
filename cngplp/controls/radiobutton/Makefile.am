noinst_LTLIBRARIES=libufr2radiobutton.la

libufr2radiobutton_la_SOURCES=radiobutton.c radiobutton.h

INCLUDES=-I$(top_srcdir)/cngplpmod\
	 -I$(top_srcdir)\
         @PACKAGE_CFLAGS@\
         @GLADE_PACKAGE_CFLAGS@

AM_CFLAGS= -Wall -O2 -fPIC
libufr2radiobutton_la_LIBADD=@PACKAGE_LIBS@ @GLADE_PACKAGE_LIBS@ #$(top_srcdir)/cngplpmod/libcngplpmod.la
