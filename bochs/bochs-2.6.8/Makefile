# Copyright (C) 2001-2014  The Bochs Project
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
#
####################################################
# NOTE: To be compatibile with nmake (microsoft vc++) please follow
# the following rules:
#   use $(VAR) not ${VAR}

prefix          = /usr/local
exec_prefix     = ${prefix}
srcdir          = .

bindir          = ${exec_prefix}/bin
libdir          = ${exec_prefix}/lib
plugdir         = ${exec_prefix}/lib/bochs/plugins
datarootdir     = ${prefix}/share
mandir          = ${datarootdir}/man
man1dir         = $(mandir)/man1
man5dir         = $(mandir)/man5
docdir          = $(datarootdir)/doc/bochs
sharedir        = $(datarootdir)/bochs
top_builddir    = .
top_srcdir      = $(srcdir)

DESTDIR =

VERSION=2.6.8
VER_STRING=2.6.8
REL_STRING=Built from SVN snapshot on May 3, 2015
MAN_PAGE_1_LIST=bochs bximage bochs-dlx
MAN_PAGE_5_LIST=bochsrc
INSTALL_LIST_SHARE=bios/BIOS-bochs-* bios/VGABIOS* bios/SeaBIOS* bios/bios.bin-* 
INSTALL_LIST_DOC=CHANGES COPYING LICENSE README TODO misc/slirp.conf
INSTALL_LIST_BIN=bochs bximage
INSTALL_LIST_BIN_OPTIONAL=bochsdbg 
INSTALL_LIST_WIN32=$(INSTALL_LIST_SHARE) $(INSTALL_LIST_DOC) $(INSTALL_LIST_BIN) $(INSTALL_LIST_BIN_OPTIONAL)
INSTALL_LIST_MACOSX=$(INSTALL_LIST_SHARE) $(INSTALL_LIST_DOC) bochs.scpt
# for win32 and macosx, these files get renamed to *.txt in install process
TEXT_FILE_LIST=README CHANGES COPYING LICENSE TODO VGABIOS-elpin-LICENSE VGABIOS-lgpl-README SeaBIOS-README
CP=cp
CAT=cat
RM=rm
MV=mv
LN_S=ln -sf
DLXLINUX_TAR=dlxlinux4.tar.gz
DLXLINUX_TAR_URL=http://bochs.sourceforge.net/guestos/$(DLXLINUX_TAR)
DLXLINUX_ROMFILE=BIOS-bochs-latest
GUNZIP=gunzip
WGET=wget
SED=sed
MKDIR=mkdir
RMDIR=rmdir
TAR=tar
CHMOD=chmod
# the GZIP variable is reserved by gzip program
GZIP_BIN=gzip -9
GUNZIP=gunzip
ZIP=zip
UNIX2DOS=unix2dos
LIBTOOL=$(SHELL) $(top_builddir)/libtool
DLLTOOL=dlltool
RC_CMD=

.SUFFIXES: .cpp

srcdir = .


SHELL = /bin/sh



CC = gcc
CXX = g++
CFLAGS = -g -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -pthread  $(X_CFLAGS) $(MCH_CFLAGS) $(FLA_FLAGS) -DBX_PLUGIN_PATH="\"${plugdir}\"" -DBX_SHARE_PATH='"$(sharedir)"'
CXXFLAGS = -g -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -pthread  $(X_CFLAGS) $(MCH_CFLAGS) $(FLA_FLAGS) -DBX_PLUGIN_PATH="\"${plugdir}\"" -DBX_SHARE_PATH='"$(sharedir)"'

LDFLAGS = 
LIBS =  -ldl
# To compile with readline:
#   linux needs just -lreadline
#   solaris needs -lreadline -lcurses
X_LIBS = -lX11    -lXrandr
X_PRE_LIBS = 
GUI_LINK_OPTS_X = $(X_LIBS) $(X_PRE_LIBS)
GUI_LINK_OPTS_SDL = 
GUI_LINK_OPTS_SDL2 = 
GUI_LINK_OPTS_SVGA =  -lvga -lvgagl
GUI_LINK_OPTS_RFB = 
GUI_LINK_OPTS_VNCSRV = 
GUI_LINK_OPTS_AMIGAOS =
GUI_LINK_OPTS_WIN32 = -luser32 -lgdi32 -lcomdlg32 -lcomctl32 -lshell32
GUI_LINK_OPTS_WIN32_VCPP = user32.lib gdi32.lib winmm.lib \
  comdlg32.lib comctl32.lib advapi32.lib shell32.lib
GUI_LINK_OPTS_MACOS =
GUI_LINK_OPTS_CARBON = -framework Carbon
GUI_LINK_OPTS_NOGUI =
GUI_LINK_OPTS_TERM = 
GUI_LINK_OPTS_WX = 
GUI_LINK_OPTS =  $(GUI_LINK_OPTS_X)
DEVICE_LINK_OPTS = 
RANLIB = ranlib

CFLAGS_CONSOLE = -g -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -pthread $(MCH_CFLAGS) $(FLA_FLAGS)
CXXFLAGS_CONSOLE = -g -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES $(MCH_CFLAGS) $(FLA_FLAGS)
BXIMAGE_LINK_OPTS = 

BX_INCDIRS = -I. -I$(srcdir)/. -Iinstrument/stubs -I$(srcdir)/instrument/stubs

#SUBDIRS = iodev bx_debug

#all install uninstall: config.h#
#        for subdir in $(SUBDIRS); do #
#          echo making $@ in $$subdir; #
#          ($(MAKE) -C $$subdir $(MDEFINES) $@) || exit 1; #
#        done#



# gnu flags for clean up
#CFLAGS  = -ansi -O -g -Wunused -Wuninitialized


NONINLINE_OBJS = \
	logio.o \
	main.o \
	config.o \
	load32bitOShack.o \
	pc_system.o \
	osdep.o \
	plugin.o \
	crc.o \
	

EXTERN_ENVIRONMENT_OBJS = \
	main.o \
	config.o \
	load32bitOShack.o \
	pc_system.o

DEBUGGER_LIB   = bx_debug/libdebug.a
DISASM_LIB     = disasm/libdisasm.a
INSTRUMENT_LIB = instrument/stubs/libinstrument.a
FPU_LIB        = cpu/fpu/libfpu.a
AVX_LIB        = cpu/avx/libavx.a
READLINE_LIB   = 
EXTRA_LINK_OPTS = 

GDBSTUB_OBJS = gdbstub.o

BX_OBJS = $(NONINLINE_OBJS)

BX_INCLUDES = bochs.h config.h osdep.h


.cpp.o:
	$(CXX) -c $(BX_INCDIRS) $(CXXFLAGS) $< -o $@
.c.o:
	$(CC) -c $(BX_INCDIRS) $(CFLAGS) $(FPU_FLAGS) $< -o $@


all: bochs bochs_plugins bximage  



bochs: iodev/libiodev.a      \
		$(DEBUGGER_LIB) cpu/libcpu.a $(AVX_LIB) cpu/cpudb/libcpudb.a memory/libmemory.a \
		gui/libgui.a $(DISASM_LIB)  $(BX_OBJS) \
		$(SIMX86_OBJS) $(FPU_LIB)  ltdl.o
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) -export-dynamic $(BX_OBJS) $(SIMX86_OBJS) \
		iodev/libiodev.a      \
		$(DEBUGGER_LIB) cpu/libcpu.a $(AVX_LIB) cpu/cpudb/libcpudb.a \
		 memory/libmemory.a gui/libgui.a \
		$(DISASM_LIB)  ltdl.o \
		 $(FPU_LIB) \
		 \
		$(DEVICE_LINK_OPTS) \
		$(MCH_LINK_FLAGS) \
		$(SIMX86_LINK_FLAGS) \
		$(READLINE_LIB) \
		$(EXTRA_LINK_OPTS) \
		$(LIBS)

# Special make target for cygwin/mingw using dlltool instead of
# libtool.  This creates a .DEF file, and exports file, an import library,
# and then links bochs.exe with the exports file.
.win32_dll_plugin_target: iodev/libiodev.a     \
		 $(DEBUGGER_LIB) cpu/libcpu.a $(AVX_LIB) cpu/cpudb/libcpudb.a \
		memory/libmemory.a gui/libgui.a $(DISASM_LIB)  \
		$(BX_OBJS) $(SIMX86_OBJS) $(FPU_LIB)  ltdl.o
	$(DLLTOOL) --export-all-symbols --output-def bochs.def \
		$(BX_OBJS) $(SIMX86_OBJS) \
		iodev/libiodev.a      \
		cpu/libcpu.a $(AVX_LIB) cpu/cpudb/libcpudb.a memory/libmemory.a gui/libgui.a \
		$(DEBUGGER_LIB) $(DISASM_LIB)  ltdl.o \
		 $(FPU_LIB)
	$(DLLTOOL) --dllname bochs.exe --def bochs.def --output-lib dllexports.a
	$(DLLTOOL) --dllname bochs.exe --output-exp bochs.exp --def bochs.def
	$(CXX) -o bochs.exe $(CXXFLAGS) $(LDFLAGS) \
	    $(BX_OBJS) bochs.exp $(SIMX86_OBJS) \
		iodev/libiodev.a      \
		cpu/libcpu.a $(AVX_LIB) cpu/cpudb/libcpudb.a memory/libmemory.a gui/libgui.a \
		$(DEBUGGER_LIB) $(DISASM_LIB)  ltdl.o \
		 $(FPU_LIB) \
		$(GUI_LINK_OPTS) \
		$(DEVICE_LINK_OPTS) \
		$(MCH_LINK_FLAGS) \
		$(SIMX86_LINK_FLAGS) \
		$(READLINE_LIB) \
		$(EXTRA_LINK_OPTS) \
		$(LIBS)
	touch .win32_dll_plugin_target

bochs_plugins:
	cd gui && \
	$(MAKE) plugins
	echo done
	cd iodev && \
	$(MAKE) plugins
	echo done
	cd iodev/display && \
	$(MAKE) plugins
	echo done
	cd iodev/hdimage && \
	$(MAKE) plugins
	echo done
	cd iodev/usb && \
	$(MAKE) plugins
	echo done
	cd iodev/network && \
	$(MAKE) plugins
	echo done
	cd iodev/sound && \
	$(MAKE) plugins
	echo done

bximage: misc/bximage.o misc/hdimage.o misc/vmware3.o misc/vmware4.o misc/vpc-img.o misc/vbox.o
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -o $@ $(CXXFLAGS_CONSOLE) $(LDFLAGS) $(BXIMAGE_LINK_OPTS) misc/bximage.o misc/hdimage.o misc/vmware3.o misc/vmware4.o misc/vpc-img.o misc/vbox.o

niclist: misc/niclist.o
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -o $@ $(CXXFLAGS_CONSOLE) $(LDFLAGS) misc/niclist.o

# compile with console CXXFLAGS, not gui CXXFLAGS
misc/bximage.o: $(srcdir)/misc/bximage.cc $(srcdir)/misc/bswap.h \
  $(srcdir)/misc/bxcompat.h $(srcdir)/iodev/hdimage/hdimage.h
	$(CXX) -c $(BX_INCDIRS) $(CXXFLAGS_CONSOLE) $(srcdir)/misc/bximage.cc -o $@

misc/hdimage.o: $(srcdir)/iodev/hdimage/hdimage.cc \
  $(srcdir)/iodev/hdimage/hdimage.h $(srcdir)/misc/bxcompat.h
	$(CXX) -c $(BX_INCDIRS) -DBXIMAGE $(CXXFLAGS_CONSOLE) $(srcdir)/iodev/hdimage/hdimage.cc -o $@

misc/vmware3.o: $(srcdir)/iodev/hdimage/vmware3.cc $(srcdir)/iodev/hdimage/vmware3.h \
  $(srcdir)/iodev/hdimage/hdimage.h $(srcdir)/misc/bxcompat.h
	$(CXX) -c $(BX_INCDIRS) -DBXIMAGE $(CXXFLAGS_CONSOLE) $(srcdir)/iodev/hdimage/vmware3.cc -o $@

misc/vmware4.o: $(srcdir)/iodev/hdimage/vmware4.cc $(srcdir)/iodev/hdimage/vmware4.h \
  $(srcdir)/iodev/hdimage/hdimage.h $(srcdir)/misc/bxcompat.h
	$(CXX) -c $(BX_INCDIRS) -DBXIMAGE $(CXXFLAGS_CONSOLE) $(srcdir)/iodev/hdimage/vmware4.cc -o $@

misc/vpc-img.o: $(srcdir)/iodev/hdimage/vpc-img.cc $(srcdir)/iodev/hdimage/vpc-img.h \
  $(srcdir)/iodev/hdimage/hdimage.h $(srcdir)/misc/bxcompat.h
	$(CXX) -c $(BX_INCDIRS) -DBXIMAGE $(CXXFLAGS_CONSOLE) $(srcdir)/iodev/hdimage/vpc-img.cc -o $@

misc/vbox.o: $(srcdir)/iodev/hdimage/vbox.cc $(srcdir)/iodev/hdimage/vbox.h \
  $(srcdir)/iodev/hdimage/hdimage.h $(srcdir)/misc/bxcompat.h
	$(CXX) -c $(BX_INCDIRS) -DBXIMAGE $(CXXFLAGS_CONSOLE) $(srcdir)/iodev/hdimage/vbox.cc -o $@

# compile with console CFLAGS, not gui CXXFLAGS
misc/niclist.o: $(srcdir)/misc/niclist.c
	$(CC) -c $(BX_INCDIRS) $(CFLAGS_CONSOLE) $(srcdir)/misc/niclist.c -o $@

$(BX_OBJS): $(BX_INCLUDES)

# cannot use -C option to be compatible with Microsoft nmake
iodev/libiodev.a::
	cd iodev && \
	$(MAKE) $(MDEFINES) libiodev.a
	echo done

iodev/display/libdisplay.a::
	cd iodev/display && \
	$(MAKE) $(MDEFINES) libdisplay.a
	echo done

iodev/hdimage/libhdimage.a::
	cd iodev/hdimage && \
	$(MAKE) $(MDEFINES) libhdimage.a
	echo done

iodev/usb/libusb.a::
	cd iodev/usb && \
	$(MAKE) $(MDEFINES) libusb.a
	echo done

iodev/network/libnetwork.a::
	cd iodev/network && \
	$(MAKE) $(MDEFINES) libnetwork.a
	echo done

iodev/sound/libsound.a::
	cd iodev/sound && \
	$(MAKE) $(MDEFINES) libsound.a
	echo done

bx_debug/libdebug.a::
	cd bx_debug && \
	$(MAKE) $(MDEFINES) libdebug.a
	echo done

cpu/libcpu.a::
	cd cpu && \
	$(MAKE) $(MDEFINES) libcpu.a
	echo done

cpu/avx/libavx.a::
	cd cpu/avx && \
	$(MAKE) $(MDEFINES) libavx.a
	echo done

cpu/cpudb/libcpudb.a::
	cd cpu/cpudb && \
	$(MAKE) $(MDEFINES) libcpudb.a
	echo done

cpu/fpu/libfpu.a::
	cd cpu/fpu && \
	$(MAKE) $(MDEFINES) libfpu.a
	echo done

memory/libmemory.a::
	cd memory && \
	$(MAKE) $(MDEFINES) libmemory.a
	echo done

gui/libgui.a::
	cd gui && \
	$(MAKE) $(MDEFINES) libgui.a
	echo done

disasm/libdisasm.a::
	cd disasm && \
	$(MAKE) $(MDEFINES) libdisasm.a
	echo done

instrument/stubs/libinstrument.a::
	cd instrument/stubs && \
	$(MAKE) $(MDEFINES) libinstrument.a
	echo done

libbochs.a:
	-rm -f libbochs.a
	ar rv libbochs.a $(EXTERN_ENVIRONMENT_OBJS)
	$(RANLIB) libbochs.a

# for wxWidgets port, on win32 platform
wxbochs_resources.o: wxbochs.rc
	windres $(srcdir)/wxbochs.rc -o $@ --include-dir=`not_found --prefix`/include

# for win32 gui
win32res.o: win32res.rc bxversion.rc
	$(RC_CMD)$@ $(srcdir)/win32res.rc

#####################################################################
# Install target for all platforms.
#####################################################################

install: all install_unix

#####################################################################
# Install target for win32
#
# This is intended to be run in cygwin, since it has better scripting
# tools.
#####################################################################

install_win32: download_dlx dl_docbook
	-mkdir -p $(INSTDIR)
	-cp -p obj-release/*.exe .
	for i in $(INSTALL_LIST_WIN32); do if test -f $$i; then cp -p $$i $(INSTDIR); else cp -p $(srcdir)/$$i $(INSTDIR); fi; done
	cp $(srcdir)/misc/sb16/sb16ctrl.example $(INSTDIR)/sb16ctrl.txt
	cp -p $(srcdir)/misc/sb16/sb16ctrl.exe $(INSTDIR)
	cp $(srcdir)/.bochsrc $(INSTDIR)/bochsrc-sample.txt
	cp $(srcdir)/TESTFORM.txt $(INSTDIR)
	-mkdir $(INSTDIR)/keymaps
	cp -p $(srcdir)/gui/keymaps/*.map $(INSTDIR)/keymaps
	cat $(DLXLINUX_TAR) | (cd $(INSTDIR) && tar xzvf -)
	echo '..\bochs -q' > $(INSTDIR)/dlxlinux/run.bat
	dlxrc=$(INSTDIR)/dlxlinux/bochsrc; mv $$dlxrc.txt $$dlxrc.orig && sed < $$dlxrc.orig 's/$$BXSHARE/../' > $$dlxrc.bxrc && rm -f $$dlxrc.orig
	mv $(INSTDIR)/README $(INSTDIR)/README.orig
	cat $(srcdir)/build/win32/README.win32-binary $(INSTDIR)/README.orig > $(INSTDIR)/README
	rm -f $(INSTDIR)/README.orig
	for i in $(TEXT_FILE_LIST); do mv $(INSTDIR)/$$i $(INSTDIR)/$$i.txt; done
	cd $(INSTDIR); $(UNIX2DOS) *.txt */*.txt
	-mkdir -p $(INSTDIR)/docs
	$(GUNZIP) -c $(srcdir)/doc/docbook/bochsdoc.tar.gz | $(TAR) -xvf - -C $(INSTDIR)/docs
	cd $(INSTDIR); NAME=`pwd|$(SED) 's/.*\///'`; (cd ..; $(ZIP) $$NAME.zip -r $$NAME); ls -l ../$$NAME.zip

#####################################################################
# install target for unix
#####################################################################

install_unix: install_bin install_libtool_plugins install_man install_share install_doc 

install_bin::
	for i in $(DESTDIR)$(bindir); do mkdir -p $$i && test -d $$i && test -w $$i; done
	for i in $(INSTALL_LIST_BIN); do if test -f $$i; then install $$i $(DESTDIR)$(bindir); else install $(srcdir)/$$i $(DESTDIR)$(bindir); fi; done
	-for i in $(INSTALL_LIST_BIN_OPTIONAL); do if test -f $$i; then install $$i $(DESTDIR)$(bindir); else install $(srcdir)/$$i $(DESTDIR)$(bindir); fi; done

install_libtool_plugins::
	for i in $(DESTDIR)$(plugdir); do mkdir -p $$i && test -d $$i && test -w $$i; done
	for i in gui iodev; do \
		find $$i -type f -name '*.la' -exec $(LIBTOOL) --mode=install install '{}' $(DESTDIR)$(plugdir) ';'; done
	$(LIBTOOL) --finish $(DESTDIR)$(plugdir)

install_dll_plugins::
	for i in $(DESTDIR)$(plugdir); do mkdir -p $$i && test -d $$i && test -w $$i; done
	for i in gui iodev; do \
		find $$i -type f -name '*.dll' -exec cp '{}' $(DESTDIR)$(plugdir) ';'; done

install_share::
	for i in $(DESTDIR)$(sharedir);	do mkdir -p $$i && test -d $$i && test -w $$i; done
	for i in $(INSTALL_LIST_SHARE); do if test -f $$i; then install -m 644 $$i $(DESTDIR)$(sharedir); else install -m 644 $(srcdir)/$$i $(DESTDIR)$(sharedir); fi; done
	-mkdir $(DESTDIR)$(sharedir)/keymaps
	for i in $(srcdir)/gui/keymaps/*.map; do install -m 644 $$i $(DESTDIR)$(sharedir)/keymaps/; done

install_doc::
	for i in $(DESTDIR)$(docdir); do mkdir -p $$i && test -d $$i && test -w $$i; done
	for i in $(INSTALL_LIST_DOC); do if test -f $$i; then install -m 644 $$i $(DESTDIR)$(docdir); else install -m 644 $(srcdir)/$$i $(DESTDIR)$(docdir); fi; done
	$(RM) -f $(DESTDIR)$(docdir)/README
	$(CAT) $(srcdir)/build/linux/README.linux-binary $(srcdir)/README > $(DESTDIR)$(docdir)/README
	install -m 644 $(srcdir)/.bochsrc $(DESTDIR)$(docdir)/bochsrc-sample.txt


# docbook section: the -C option can be used here
build_docbook::
	$(MAKE) -C doc/docbook

dl_docbook::
	$(MAKE) -C doc/docbook dl_docs

install_docbook: build_docbook
	$(MAKE) -C doc/docbook install

clean_docbook::
	$(MAKE) -C doc/docbook clean

install_man::
	-mkdir -p $(DESTDIR)$(man1dir)
	-mkdir -p $(DESTDIR)$(man5dir)
	for i in $(MAN_PAGE_1_LIST); do cat $(srcdir)/doc/man/$$i.1 | $(SED) 's/@version@/$(VERSION)/g' | $(GZIP_BIN) -c >  $(DESTDIR)$(man1dir)/$$i.1.gz; chmod 644 $(DESTDIR)$(man1dir)/$$i.1.gz; done
	for i in $(MAN_PAGE_5_LIST); do cat $(srcdir)/doc/man/$$i.5 | $(GZIP_BIN) -c >  $(DESTDIR)$(man5dir)/$$i.5.gz; chmod 644 $(DESTDIR)$(man5dir)/$$i.5.gz; done

download_dlx: $(DLXLINUX_TAR)

$(DLXLINUX_TAR):
	$(RM) -f $(DLXLINUX_TAR)
	$(WGET) $(DLXLINUX_TAR_URL)
	test -f $(DLXLINUX_TAR)

unpack_dlx: $(DLXLINUX_TAR)
	rm -rf dlxlinux
	$(GUNZIP) -c $(DLXLINUX_TAR) | $(TAR) -xvf -
	test -d dlxlinux
	(cd dlxlinux; $(MV) bochsrc.txt bochsrc.txt.orig; $(SED) -e "s/1\.1\.2/$(VERSION)/g"  -e 's,/usr/local/bochs/latest,$(prefix)/share/bochs,g' < bochsrc.txt.orig > bochsrc.txt; rm -f bochsrc.txt.orig)

install_dlx:
	$(RM) -rf $(DESTDIR)$(sharedir)/dlxlinux
	cp -r dlxlinux $(DESTDIR)$(sharedir)/dlxlinux
	$(CHMOD) 755 $(DESTDIR)$(sharedir)/dlxlinux
	$(GZIP_BIN) $(DESTDIR)$(sharedir)/dlxlinux/hd10meg.img
	$(CHMOD) 644 $(DESTDIR)$(sharedir)/dlxlinux/*
	for i in bochs-dlx; do cp $(srcdir)/build/linux/$$i $(DESTDIR)$(bindir)/$$i; $(CHMOD) 755 $(DESTDIR)$(bindir)/$$i; done

uninstall::
	$(RM) -rf $(DESTDIR)$(sharedir)
	$(RM) -rf $(DESTDIR)$(docdir)
	$(RM) -rf $(DESTDIR)$(libdir)/bochs
	for i in $(INSTALL_LIST_BIN); do rm -f $(DESTDIR)$(bindir)/$$i; done
	-for i in $(INSTALL_LIST_BIN_OPTIONAL); do rm -f $(DESTDIR)$(bindir)/$$i; done
	for i in $(MAN_PAGE_1_LIST); do $(RM) -f $(man1dir)/$$i.1.gz; done
	for i in $(MAN_PAGE_5_LIST); do $(RM) -f $(man5dir)/$$i.5.gz; done

VS2013_WORKSPACE_ZIP=build/win32/vs2013ex-workspace.zip
VS2013_WORKSPACE_FILES=vs2013/bochs.sln vs2013/*.vcxproj

vs2013workspace:
	zip $(VS2013_WORKSPACE_ZIP) $(VS2013_WORKSPACE_FILES)

########
# the win32_snap target is used to create a ZIP of bochs sources configured
# for VC++.  This ZIP is stuck on the website every once in a while to make
# it easier for VC++ users to compile bochs.  First, you should
# run "sh .conf.win32-vcpp" to configure the source code, then do
# "make win32_snap" to unzip the workspace files and create the ZIP.
########
win32_snap:
	unzip $(VS2013_WORKSPACE_ZIP)
	$(MAKE) zip

tar:
	NAME=`pwd|$(SED) 's/.*\///'`; (cd ..; $(RM) -f $$NAME.zip; tar cf - $$NAME | $(GZIP_BIN) > $$NAME.tar.gz); ls -l ../$$NAME.tar.gz

zip:
	NAME=`pwd|$(SED) 's/.*\///'`; (cd ..; $(RM) -f $$NAME-msvc-src.zip; $(ZIP) $$NAME-msvc-src.zip -r $$NAME -x \*.svn\* ); ls -l ../$$NAME-msvc-src.zip

clean:
	rm -f  *.o
	rm -f  *.a
	rm -f  bochs
	rm -f  bochs.exe
	rm -f  bximage
	rm -f  bximage.exe
	rm -f  niclist
	rm -f  niclist.exe
	rm -f  bochs.out
	rm -f  bochsout.txt
	rm -f  *.exp *.lib
	rm -f  bochs.def
	rm -f  bochs.scpt
	rm -f  -rf bochs.app
	rm -f  -rf .libs
	rm -f  .win32_dll_plugin_target

local-dist-clean: clean
	rm -f  config.h config.status config.log config.cache
	rm -f  .dummy `find . -name '*.dsp' -o -name '*.dsw' -o -name '*.opt' -o -name '.DS_Store'`
	rm -f  bxversion.h bxversion.rc build/linux/bochs-dlx _rpm_top *.rpm
	rm -f  build/win32/nsis/Makefile build/win32/nsis/bochs.nsi
	rm -f  build/macosx/Info.plist build/macosx/script_compiled.rsrc
	rm -f  libtool
	rm -f  ltdlconf.h

clean_pcidev::
	cd host/linux/pcidev && \
	$(MAKE) clean
	echo done

all-clean: clean  
	cd iodev && \
	$(MAKE) clean
	echo done
	cd iodev/display && \
	$(MAKE) clean
	echo done
	cd iodev/hdimage && \
	$(MAKE) clean
	echo done
	cd iodev/usb && \
	$(MAKE) clean
	echo done
	cd iodev/network && \
	$(MAKE) clean
	echo done
	cd iodev/sound && \
	$(MAKE) clean
	echo done
	cd bx_debug && \
	$(MAKE) clean
	echo done
	cd cpu && \
	$(MAKE) clean
	echo done
	cd cpu/avx && \
	$(MAKE) clean
	echo done
	cd cpu/cpudb && \
	$(MAKE) clean
	echo done
	cd cpu/fpu && \
	$(MAKE) clean
	echo done
	cd memory && \
	$(MAKE) clean
	echo done
	cd gui && \
	$(MAKE) clean
	echo done
	cd disasm && \
	$(MAKE) clean
	echo done
	cd instrument/stubs && \
	$(MAKE) clean
	echo done
	cd misc && \
	$(MAKE) clean
	echo done

dist-clean: local-dist-clean
	cd iodev && \
	$(MAKE) dist-clean
	echo done
	cd iodev/display && \
	$(MAKE) dist-clean
	echo done
	cd iodev/hdimage && \
	$(MAKE) dist-clean
	echo done
	cd iodev/usb && \
	$(MAKE) dist-clean
	echo done
	cd iodev/network && \
	$(MAKE) dist-clean
	echo done
	cd iodev/sound && \
	$(MAKE) dist-clean
	echo done
	cd bx_debug && \
	$(MAKE) dist-clean
	echo done
	cd bios && \
	$(MAKE) dist-clean
	echo done
	cd cpu && \
	$(MAKE) dist-clean
	echo done
	cd cpu/avx && \
	$(MAKE) dist-clean
	echo done
	cd cpu/cpudb && \
	$(MAKE) dist-clean
	echo done
	cd cpu/fpu && \
	$(MAKE) dist-clean
	echo done
	cd memory && \
	$(MAKE) dist-clean
	echo done
	cd gui && \
	$(MAKE) dist-clean
	echo done
	cd disasm && \
	$(MAKE) dist-clean
	echo done
	cd instrument/stubs && \
	$(MAKE) dist-clean
	echo done
	cd misc && \
	$(MAKE) dist-clean
	echo done
	cd doc/docbook && \
	$(MAKE) dist-clean
	echo done
	cd host/linux/pcidev && \
	$(MAKE) dist-clean
	echo done
	rm -f  Makefile

###########################################
# Build app on MacOS X
###########################################
MACOSX_STUFF=build/macosx
MACOSX_STUFF_SRCDIR=$(srcdir)/$(MACOSX_STUFF)
APP=bochs.app
APP_PLATFORM=MacOS
SCRIPT_EXEC=bochs.scpt
SCRIPT_DATA=$(MACOSX_STUFF_SRCDIR)/script.data
SCRIPT_R=$(MACOSX_STUFF_SRCDIR)/script.r
SCRIPT_APPLESCRIPT=$(MACOSX_STUFF_SRCDIR)/bochs.applescript
SCRIPT_COMPILED_RSRC=$(MACOSX_STUFF)/script_compiled.rsrc
REZ=/Developer/Tools/Rez
CPMAC=/Developer/Tools/CpMac
RINCLUDES=/System/Library/Frameworks/Carbon.framework/Libraries/RIncludes
REZ_ARGS=-append -i $RINCLUDES -d SystemSevenOrLater=1 -useDF
STANDALONE_LIBDIR=`pwd`/$(APP)/Contents/$(APP_PLATFORM)/lib
OSACOMPILE=/usr/bin/osacompile
SETFILE=/Developer/Tools/SetFile

# On a MacOS X machine, you run rez, osacompile, and setfile to
# produce the script executable, which has both a data fork and a
# resource fork.  Ideally, we would just recompile the whole
# executable at build time, but unfortunately this cannot be done on
# the SF compile farm through an ssh connection because osacompile
# needs to be run locally for some reason.  Solution: If the script
# sources are changed, rebuild the executable on a MacOSX machine,
# split it into its data and resource forks and check them into SVN
# as separate files.  Then at release time, all that's left to do is
# put the data and resource forks back together to make a working script.
# (This can be done through ssh.)
#
# Sources:
# 1. script.r: resources for the script
# 2. script.data: binary data for the script
# 3. bochs.applescript: the source of the script
#
# NOTE: All of this will fail if you aren't building on an HFS+
# filesystem!  On the SF compile farm building in your user directory
# will fail, while doing the build in /tmp will work ok.

# check if this filesystem supports resource forks at all
test_hfsplus:
	$(RM) -rf test_hfsplus
	echo data > test_hfsplus
	# if you get "Not a directory", then this filesystem doesn't support resources
	echo resource > test_hfsplus/rsrc
	# test succeeded
	$(RM) -rf test_hfsplus

# Step 1 (must be done locally on MacOSX, only when sources change)
# Compile and pull out just the resource fork.  The resource fork is
# checked into SVN as script_compiled.rsrc.  Note that we don't need
# to check in the data fork of tmpscript because it is identical to the
# script.data input file.
$(SCRIPT_COMPILED_RSRC): $(SCRIPT_R) $(SCRIPT_APPLESCRIPT)
	$(RM) -f tmpscript
	$(CP) -f $(SCRIPT_DATA) tmpscript
	$(REZ) -append $(SCRIPT_R) -o tmpscript
	$(OSACOMPILE) -o tmpscript $(SCRIPT_APPLESCRIPT)
	$(CP) tmpscript/rsrc $(SCRIPT_COMPILED_RSRC)
	$(RM) -f tmpscript

# Step 2 (can be done locally or remotely on MacOSX)
# Combine the data fork and resource fork, and set attributes.
$(SCRIPT_EXEC): $(SCRIPT_DATA) $(SCRIPT_COMPILED_RSRC)
	rm -f $(SCRIPT_EXEC)
	$(CP) $(SCRIPT_DATA) $(SCRIPT_EXEC)
	if test ! -f $(SCRIPT_COMPILED_RSRC); then $(CP) $(srcdir)/$(SCRIPT_COMPILED_RSRC) $(SCRIPT_COMPILED_RSRC); fi
	$(CP) $(SCRIPT_COMPILED_RSRC) $(SCRIPT_EXEC)/rsrc
	$(SETFILE) -t "APPL" -c "aplt" $(SCRIPT_EXEC)

$(APP)/.build: bochs test_hfsplus $(SCRIPT_EXEC)
	rm -f $(APP)/.build
	$(MKDIR) -p $(APP)
	$(MKDIR) -p $(APP)/Contents
	$(CP) -f $(MACOSX_STUFF)/Info.plist $(APP)/Contents
	$(CP) -f $(MACOSX_STUFF_SRCDIR)/pbdevelopment.plist $(APP)/Contents
	echo -n "APPL????"  > $(APP)/Contents/PkgInfo
	$(MKDIR) -p $(APP)/Contents/$(APP_PLATFORM)
	$(CP) bochs $(APP)/Contents/$(APP_PLATFORM)
	$(MKDIR) -p $(APP)/Contents/Resources
	$(REZ) $(REZ_ARGS) $(MACOSX_STUFF_SRCDIR)/bochs.r -o $(APP)/Contents/Resources/bochs.rsrc
	$(CP) -f $(MACOSX_STUFF_SRCDIR)/bochs-icn.icns $(APP)/Contents/Resources
	ls -ld $(APP) $(SCRIPT_EXEC) $(SCRIPT_EXEC)/rsrc
	touch $(APP)/.build

$(APP)/.build_plugins: $(APP)/.build bochs_plugins
	rm -f $(APP)/.build_plugins
	$(MKDIR) -p $(STANDALONE_LIBDIR);
	list=`cd gui && echo *.la`; for i in $$list; do $(LIBTOOL) cp gui/$$i $(STANDALONE_LIBDIR); done;
	list=`cd iodev && echo *.la`; for i in $$list; do $(LIBTOOL) cp iodev/$$i $(STANDALONE_LIBDIR); done;
	$(LIBTOOL) --finish $(STANDALONE_LIBDIR);
	touch $(APP)/.build_plugins

install_macosx: all download_dlx install_man 
	-mkdir -p $(DESTDIR)$(sharedir)
	for i in $(INSTALL_LIST_MACOSX); do if test -e $$i; then $(CPMAC) -r $$i $(DESTDIR)$(sharedir); else $(CPMAC) -r $(srcdir)/$$i $(DESTDIR)$(sharedir); fi; done
	$(CPMAC) $(srcdir)/.bochsrc $(DESTDIR)$(sharedir)/bochsrc-sample.txt
	-mkdir $(DESTDIR)$(sharedir)/keymaps
	$(CPMAC) $(srcdir)/gui/keymaps/*.map $(DESTDIR)$(sharedir)/keymaps
	cat $(DLXLINUX_TAR) | (cd $(DESTDIR)$(sharedir) && tar xzvf -)
	dlxrc=$(DESTDIR)$(sharedir)/dlxlinux/bochsrc.txt; mv "$$dlxrc" "$$dlxrc.orig" && sed < "$$dlxrc.orig" 's/\/usr\/local\/bochs\/latest/../' > "$$dlxrc" && rm -f "$$dlxrc.orig"
	mv $(srcdir)/README $(srcdir)/README.orig
	cat $(srcdir)/build/macosx/README.macosx-binary $(srcdir)/README.orig > $(DESTDIR)$(sharedir)/README
	rm -f $(DESTDIR)$(sharedir)/README.orig
	$(CPMAC) $(SCRIPT_EXEC) $(DESTDIR)$(sharedir)/dlxlinux
#	for i in $(TEXT_FILE_LIST); do mv $(srcdir)/$$i $(DESTDIR)$(sharedir)/$$i.txt; done

###########################################
# dependencies generated by
#  gcc -MM -I. -Iinstrument/stubs *.cc | sed -e 's/\.cc/.cpp/g' -e 's,cpu/,cpu/,g'
###########################################
config.o: config.cpp bochs.h config.h osdep.h bx_debug/debug.h config.h \
 osdep.h gui/siminterface.h cpudb.h gui/paramtree.h memory/memory.h \
 pc_system.h gui/gui.h instrument/stubs/instrument.h bxversion.h \
 iodev/iodev.h bochs.h plugin.h extplugin.h param_names.h param_names.h \
 cpudb.h
crc.o: crc.cpp config.h
gdbstub.o: gdbstub.cpp bochs.h config.h osdep.h bx_debug/debug.h config.h \
 osdep.h gui/siminterface.h cpudb.h gui/paramtree.h memory/memory.h \
 pc_system.h gui/gui.h instrument/stubs/instrument.h param_names.h \
 cpu/cpu.h cpu/cpuid.h cpu/crregs.h cpu/descriptor.h cpu/instr.h \
 cpu/ia_opcodes.h cpu/lazy_flags.h cpu/icache.h cpu/apic.h cpu/i387.h \
 cpu/fpu/softfloat.h cpu/fpu/tag_w.h cpu/fpu/status_w.h \
 cpu/fpu/control_w.h cpu/xmm.h cpu/vmx.h cpu/stack.h
load32bitOShack.o: load32bitOShack.cpp bochs.h config.h osdep.h \
 bx_debug/debug.h config.h osdep.h gui/siminterface.h cpudb.h \
 gui/paramtree.h memory/memory.h pc_system.h gui/gui.h \
 instrument/stubs/instrument.h param_names.h cpu/cpu.h cpu/cpuid.h \
 cpu/crregs.h cpu/descriptor.h cpu/instr.h cpu/ia_opcodes.h \
 cpu/lazy_flags.h cpu/icache.h cpu/apic.h cpu/i387.h cpu/fpu/softfloat.h \
 cpu/fpu/tag_w.h cpu/fpu/status_w.h cpu/fpu/control_w.h cpu/xmm.h \
 cpu/vmx.h iodev/iodev.h bochs.h plugin.h extplugin.h param_names.h
logio.o: logio.cpp bochs.h config.h osdep.h bx_debug/debug.h config.h \
 osdep.h gui/siminterface.h cpudb.h gui/paramtree.h memory/memory.h \
 pc_system.h gui/gui.h instrument/stubs/instrument.h cpu/cpu.h \
 cpu/cpuid.h cpu/crregs.h cpu/descriptor.h cpu/instr.h cpu/ia_opcodes.h \
 cpu/lazy_flags.h cpu/icache.h cpu/apic.h cpu/i387.h cpu/fpu/softfloat.h \
 cpu/fpu/tag_w.h cpu/fpu/status_w.h cpu/fpu/control_w.h cpu/xmm.h \
 cpu/vmx.h
main.o: main.cpp bochs.h config.h osdep.h bx_debug/debug.h config.h \
 osdep.h gui/siminterface.h cpudb.h gui/paramtree.h memory/memory.h \
 pc_system.h gui/gui.h instrument/stubs/instrument.h bxversion.h \
 param_names.h gui/textconfig.h cpu/cpu.h cpu/cpuid.h cpu/crregs.h \
 cpu/descriptor.h cpu/instr.h cpu/ia_opcodes.h cpu/lazy_flags.h \
 cpu/icache.h cpu/apic.h cpu/i387.h cpu/fpu/softfloat.h cpu/fpu/tag_w.h \
 cpu/fpu/status_w.h cpu/fpu/control_w.h cpu/xmm.h cpu/vmx.h iodev/iodev.h \
 bochs.h plugin.h extplugin.h param_names.h
osdep.o: osdep.cpp bochs.h config.h osdep.h bx_debug/debug.h config.h \
 osdep.h gui/siminterface.h cpudb.h gui/paramtree.h memory/memory.h \
 pc_system.h gui/gui.h instrument/stubs/instrument.h
pc_system.o: pc_system.cpp bochs.h config.h osdep.h bx_debug/debug.h \
 config.h osdep.h gui/siminterface.h cpudb.h gui/paramtree.h \
 memory/memory.h pc_system.h gui/gui.h instrument/stubs/instrument.h \
 cpu/cpu.h cpu/cpuid.h cpu/crregs.h cpu/descriptor.h cpu/instr.h \
 cpu/ia_opcodes.h cpu/lazy_flags.h cpu/icache.h cpu/apic.h cpu/i387.h \
 cpu/fpu/softfloat.h cpu/fpu/tag_w.h cpu/fpu/status_w.h \
 cpu/fpu/control_w.h cpu/xmm.h cpu/vmx.h iodev/iodev.h bochs.h plugin.h \
 extplugin.h param_names.h
plugin.o: plugin.cpp bochs.h config.h osdep.h bx_debug/debug.h config.h \
 osdep.h gui/siminterface.h cpudb.h gui/paramtree.h memory/memory.h \
 pc_system.h gui/gui.h instrument/stubs/instrument.h iodev/iodev.h \
 bochs.h plugin.h extplugin.h param_names.h plugin.h
