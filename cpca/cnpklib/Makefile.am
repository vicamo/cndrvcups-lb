lib_LTLIBRARIES = libcnpkufr2.la

libcnpkufr2_la_SOURCES = cnpklib.c cnpklibufr2.h cnpkopt.c cnpkopt.h cnpkproc.c cnpkproc.h

libcnpkufr2_la_LIBADD = -lbuftool

libcnpkufr2_la_LDFLAGS = \
	-shared \
	-version-info 1:0:0

AM_CFLAGS = -O2 -Wall -fPIC -D_UFR2_

# for Install headers (not for Build)
include_HEADERS = cnpklibufr2.h

