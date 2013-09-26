noinst_LTLIBRARIES=libufr2label.la

libufr2label_la_SOURCES=label.c label.h

INCLUDES=-I$(top_srcdir)/cngplpmod\
	 -I$(top_srcdir)\
         @PACKAGE_CFLAGS@\
         @GLADE_PACKAGE_CFLAGS@

AM_CFLAGS= -Wall -O2 -fPIC
libufr2label_la_LIBADD=@PACKAGE_LIBS@ @GLADE_PACKAGE_LIBS@ #$(top_srcdir)/cngplpmod/libcngplpmod.la
