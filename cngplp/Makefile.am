confdir = /usr/share/cngplp

lib_LTLIBRARIES=libuictlufr2.la
SUBDIRS=po cngplpmod controls files

INCLUDES=-DPACKAGE_CONFIG_DIR=\""$(confdir)/"\"\
	 -DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	 -I$(top_srcdir)/cngplpmod\
	 -I$(top_srcdir)/controls/combo\
	 -I$(top_srcdir)/controls/checkbutton\
	 -I$(top_srcdir)/controls/dialog\
	 -I$(top_srcdir)/controls/radiobutton\
	 -I$(top_srcdir)/controls/button\
	 -I$(top_srcdir)/controls/spinbutton\
	 -I$(top_srcdir)/controls/entry\
	 -I$(top_srcdir)/controls/label\
	 -I$(top_srcdir)/controls/textview\
	 @PACKAGE_CFLAGS@  @GLADE_PACKAGE_CFLAGS@

libuictlufr2_la_LDFLAGS = \
	-shared \
	-version-info 1:0:0

libuictlufr2_la_SOURCES=configurefile.c configurefile.h\
			   controller.c controller.h\
			   localize.c localize.h\
			   widgets.c widgets.h\
			   cngplpmodIF.h cngplpmodIF.c

AM_CFLAGS=-O2 -Wall -fPIC
libuictlufr2_la_LIBADD = @PACKAGE_LIBS@ @GLADE_PACKAGE_LIBS@ -lcups\
			$(top_srcdir)/cngplpmod/libcngplpmod.la\
			$(top_srcdir)/controls/button/libufr2button.la\
			$(top_srcdir)/controls/combo/libufr2combo.la\
			$(top_srcdir)/controls/entry/libufr2entry.la\
			$(top_srcdir)/controls/spinbutton/libufr2spinbutton.la\
			$(top_srcdir)/controls/checkbutton/libufr2checkbutton.la\
			$(top_srcdir)/controls/dialog/libufr2dialog.la\
			$(top_srcdir)/controls/label/libufr2label.la\
			$(top_srcdir)/controls/radiobutton/libufr2radiobutton.la\
			$(top_srcdir)/controls/textview/libufr2textview.la
