AUTOMAKE_OPTIONS=foreign
ACLOCAL_AMFLAGS = -I build-aux
SUBDIRS=scripts doc test

noinst_LTLIBRARIES = libcommunicate.la libmacosx.la
libcommunicate_la_SOURCES = communicate.c

libmacosx_la_SOURCES = libfakeroot_inode64.c libfakeroot_unix2003.c patchattr.h

lib_LTLIBRARIES=libfakeroot.la
libfakeroot_la_SOURCES=libfakeroot.c statconv/glibc/linux/alpha/stats.h wrapdef.h  wrapstruct.h communicate.h
libfakeroot_la_LDFLAGS=-release 0
libfakeroot_la_DEPENDENCIES=wrapdef.h wrapstruct.h libcommunicate.la $(LTLIBOBJS)
if MACOSX
libfakeroot_la_LIBADD = libcommunicate.la libmacosx.la $(LTLIBOBJS)
else !MACOSX
libfakeroot_la_LIBADD = libcommunicate.la $(LTLIBOBJS)
endif !MACOSX

bin_PROGRAMS=faked
faked_SOURCES = faked.c
faked_LDADD = libcommunicate.la

simple_SOURCES=simple.c
noinst_PROGRAMS=simple

CLEANFILES= wrapdef.h wrapstruct.h wrapped.h wraptmpf.h
DISTCLEANFILES = fakerootconfig.h

EXTRA_DIST=wrapawk wrapfunc.inp                        \
           debian/rules debian/changelog debian/control  \
	   message.h \
	   DEBUG BUGS \
	   setenv.c \
	   preroll

CLEAN_FILES=fakerootconfig.h

wrapped.h wrapdef.h wrapstruct.h wraptmpf.h:wrapawk wrapfunc.inp
	awk -f $(srcdir)/wrapawk < $(srcdir)/wrapfunc.inp

libfakeroot.lo:libfakeroot.c wrapdef.h wrapstruct.h wraptmpf.h

fakerootconfig.h: ./config.status
	CONFIG_FILES= CONFIG_HEADERS= /bin/sh ./config.status


#Local variables:
#mode: makefile
#End:
