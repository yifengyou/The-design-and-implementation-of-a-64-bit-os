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

# Makefile for the gui component of bochs


.SUFFIXES: .cpp

prefix          = /usr/local
exec_prefix     = ${prefix}
srcdir          = .

bindir          = ${exec_prefix}/bin
libdir          = ${exec_prefix}/lib
datarootdir     = ${prefix}/share
mandir          = ${datarootdir}/man
man1dir         = $(mandir)/man1
man5dir         = $(mandir)/man5
docdir          = $(datarootdir)/doc/bochs
sharedir        = $(datarootdir)/bochs
top_builddir    = ..
top_srcdir      = ..

SHELL = /bin/bash



CXX = g++
CXXFLAGS = $(BX_INCDIRS) -g -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -pthread   $(X_CFLAGS)
LOCAL_CXXFLAGS = 
LDFLAGS = 
LIBS =  -ldl
X_CFLAGS = 
RANLIB = ranlib
PLUGIN_PATH=${exec_prefix}/lib
top_builddir    = ..
LIBTOOL=$(SHELL) $(top_builddir)/libtool
WIN32_DLL_IMPORT_LIBRARY=../
BX_INCDIRS = -I.. -I$(srcdir)/.. -I../iodev -I$(srcdir)/../iodev -I../instrument/stubs -I$(srcdir)/../instrument/stubs

GUI_OBJS_X11  = x.o
GUI_OBJS_SDL = sdl.o
GUI_OBJS_SDL2 = sdl2.o
GUI_OBJS_SVGA = svga.o
GUI_OBJS_WIN32 = win32.o
GUI_OBJS_MACOS = macintosh.o
GUI_OBJS_CARBON = carbon.o
GUI_OBJS_NOGUI = nogui.o
GUI_OBJS_TERM  = term.o
GUI_OBJS_RFB = rfb.o
GUI_OBJS_VNCSRV = vncsrv.o
GUI_OBJS_AMIGAOS = amigaos.o
GUI_OBJS_WX = wx.o
GUI_OBJS_WX_SUPPORT = wxmain.o wxdialog.o

GUI_DLL_TARGETS = 
OBJS_THAT_CANNOT_BE_PLUGINS = keymap.o gui.o siminterface.o paramtree.o textconfig.o  
OBJS_THAT_CAN_BE_PLUGINS = 

X_LIBS = -lX11 -lXpm -lXrandr
X_PRE_LIBS = 
GUI_LINK_OPTS_X = $(X_LIBS) $(X_PRE_LIBS)
GUI_LINK_OPTS_SDL = 
GUI_LINK_OPTS_SDL2 = 
GUI_LINK_OPTS_SVGA =  -lvga -lvgagl
GUI_LINK_OPTS_RFB = 
GUI_LINK_OPTS_RFB_VCPP = user32.lib 
GUI_LINK_OPTS_VNCSRV = 
GUI_LINK_OPTS_AMIGAOS =
GUI_LINK_OPTS_WIN32 = -luser32 -lgdi32 -lcomdlg32 -lcomctl32
GUI_LINK_OPTS_WIN32_VCPP = user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib
GUI_LINK_OPTS_MACOS =
GUI_LINK_OPTS_CARBON = -framework Carbon
GUI_LINK_OPTS_NOGUI =
GUI_LINK_OPTS_TERM = 
GUI_LINK_OPTS_WX = 
GUI_LINK_OPTS =  $(GUI_LINK_OPTS_X)  

NONPLUGIN_OBJS = $(OBJS_THAT_CANNOT_BE_PLUGINS)
PLUGIN_OBJS = $(OBJS_THAT_CAN_BE_PLUGINS)  $(GUI_OBJS_X11)

#
# -------- end configurable options --------------------------
#

all: libgui.a

plugins: plugins_gcc

plugins_gcc: $(PLUGIN_OBJS:%.o=libbx_%.la)

plugins_msvc: $(GUI_DLL_TARGETS)

libgui.a: $(NONPLUGIN_OBJS)
	rm -f  libgui.a
	ar rv $@ $(NONPLUGIN_OBJS)
	$(RANLIB) libgui.a

# standard compile rule for C++ files
.cpp.o:
	$(CXX) -c  $(CXXFLAGS) $(LOCAL_CXXFLAGS) $< -o $@

##### building plugins with libtool
%.lo: %.cpp
	$(LIBTOOL) --mode=compile --tag CXX $(CXX) -c $(CXXFLAGS) $(LOCAL_CXXFLAGS) $< -o $@

libbx_%.la: %.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH)

libbx_x.la: x.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_X)

libbx_sdl.la: sdl.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_SDL)

libbx_sdl2.la: sdl2.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_SDL2)

libbx_svga.la: svga.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_SVGA)

libbx_rfb.la: rfb.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_RFB)

libbx_vncsrv.la: vncsrv.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_VNCSRV)

libbx_amigaos.la: amigaos.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_AMIGAOS)

libbx_win32.la: win32.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_WIN32)

libbx_macos.la: macos.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_MACOS)

libbx_carbon.la: carbon.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_CARBON)

libbx_nogui.la: nogui.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_NOGUI)

libbx_term.la: term.lo
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $< -o $@ -rpath $(PLUGIN_PATH) $(GUI_LINK_OPTS_TERM)

# special link rules for plugins that require more than one object file
libbx_wx.la: $(GUI_OBJS_WX:.o=.lo) $(GUI_OBJS_WX_SUPPORT:.o=.lo)
	$(LIBTOOL) --mode=link --tag CXX $(CXX) -module $(GUI_OBJS_WX:.o=.lo) $(GUI_OBJS_WX_SUPPORT:.o=.lo) -o libbx_wx.la -rpath $(PLUGIN_PATH)  $(GUI_LINK_OPTS_WX)

#### building DLLs for win32 (Cygwin and MinGW/MSYS)
bx_%.dll: %.o
	$(CXX) $(CXXFLAGS) -shared -o $@ $< $(WIN32_DLL_IMPORT_LIBRARY)  $(GUI_LINK_OPTS_WIN32)

bx_wx.dll: $(GUI_OBJS_WX) $(GUI_OBJS_WX_SUPPORT)
	$(CXX) $(CXXFLAGS) -shared -o bx_wx.dll $(GUI_OBJS_WX) $(GUI_OBJS_WX_SUPPORT) $(WIN32_DLL_IMPORT_LIBRARY) `wx-config --libs` -luser32 -lgdi32 -lcomdlg32 -lcomctl32

bx_sdl.dll: $(GUI_OBJS_SDL)
	$(CXX) $(CXXFLAGS) -shared -o bx_sdl.dll $(GUI_OBJS_SDL) $(WIN32_DLL_IMPORT_LIBRARY) $(GUI_LINK_OPTS_SDL)

bx_sdl2.dll: $(GUI_OBJS_SDL2)
	$(CXX) $(CXXFLAGS) -shared -o bx_sdl2.dll $(GUI_OBJS_SDL2) $(WIN32_DLL_IMPORT_LIBRARY) $(GUI_LINK_OPTS_SDL2)

bx_vncsrv.dll: $(GUI_OBJS_VNCSRV)
	$(CXX) $(CXXFLAGS) -shared -o bx_vncsrv.dll $(GUI_OBJS_VNCSRV) $(WIN32_DLL_IMPORT_LIBRARY) $(GUI_LINK_OPTS_VNCSRV)

# special link rules for plugins with Cygwin, MinGW/MSYS and MSVC nmake
bx_nogui.dll: $(GUI_OBJS_NOGUI)
	 $(GUI_OBJS_NOGUI) $(WIN32_DLL_IMPORT_LIBRARY)

bx_rfb.dll: $(GUI_OBJS_RFB)
	 $(GUI_OBJS_RFB) $(WIN32_DLL_IMPORT_LIBRARY) $(GUI_LINK_OPTS_RFB)

bx_win32.dll: $(GUI_OBJS_WIN32)
	 $(GUI_OBJS_WIN32) $(WIN32_DLL_IMPORT_LIBRARY) $(GUI_LINK_OPTS_WIN32)

##### end DLL section

clean:
	rm -f  -rf .libs *.la *.a *.lo *.o *.dll *.exp *.lib

dist-clean: clean
	rm -f  Makefile

###########################################
# all other dependencies generated by
#  gcc -MM -I.. -I../iodev -I../instrument/stubs *.cc | \
#     sed -e 's/\.cc/.cpp/g'
#  gcc -MM -I.. -I../iodev -I../instrument/stubs *.cc | \
#     sed -e 's/\.cc/.cpp/g' -e 's/\.o:/.lo:/g'
#
# This means that every source file is listed twice, once with a .o rule
# and then again with an identical .lo rule.  The .lo rules are used when
# building plugins.
###########################################
amigaos.o: amigaos.cpp ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h ../bxversion.h \
 ../param_names.h ../iodev/iodev.h ../plugin.h ../extplugin.h
carbon.o: carbon.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h keymap.h ../iodev/iodev.h ../plugin.h \
 ../extplugin.h ../param_names.h
enh_dbg.o: enh_dbg.cpp ../config.h
gtk_enh_dbg_osdep.o: gtk_enh_dbg_osdep.cpp ../config.h
gui.o: gui.cpp ../iodev/iodev.h ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h ../plugin.h ../extplugin.h \
 ../param_names.h ../iodev/virt_timer.h keymap.h ../gui/bitmaps/floppya.h \
 ../gui/bitmaps/floppyb.h ../gui/bitmaps/mouse.h ../gui/bitmaps/reset.h \
 ../gui/bitmaps/power.h ../gui/bitmaps/snapshot.h ../gui/bitmaps/copy.h \
 ../gui/bitmaps/paste.h ../gui/bitmaps/configbutton.h \
 ../gui/bitmaps/cdromd.h ../gui/bitmaps/userbutton.h \
 ../gui/bitmaps/saverestore.h
keymap.o: keymap.cpp ../param_names.h ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h keymap.h
macintosh.o: macintosh.cpp ../param_names.h ../bochs.h ../config.h \
 ../osdep.h ../bx_debug/debug.h ../config.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h \
 ../pc_system.h ../gui/gui.h ../instrument/stubs/instrument.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h
nogui.o: nogui.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../plugin.h ../extplugin.h \
 ../param_names.h icon_bochs.h
paramtree.o: paramtree.cpp ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h paramtree.h
rfb.o: rfb.cpp ../param_names.h ../iodev/iodev.h ../bochs.h ../config.h \
 ../osdep.h ../bx_debug/debug.h ../config.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h \
 ../pc_system.h ../gui/gui.h ../instrument/stubs/instrument.h ../plugin.h \
 ../extplugin.h keymap.h icon_bochs.h font/vga.bitmap.h sdl.h rfb.h \
 rfbkeys.h
scrollwin.o: scrollwin.cpp ../config.h
sdl2.o: sdl2.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h
sdl.o: sdl.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h icon_bochs.h sdl.h sdlkeys.h
siminterface.o: siminterface.cpp ../param_names.h ../iodev/iodev.h \
 ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h ../config.h \
 ../osdep.h ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h \
 ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../plugin.h ../extplugin.h \
 ../iodev/virt_timer.h
svga.o: svga.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h ../iodev/iodev.h \
 ../plugin.h ../extplugin.h
term.o: term.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h ../iodev/iodev.h \
 ../plugin.h ../extplugin.h
textconfig.o: textconfig.cpp ../config.h ../osdep.h ../param_names.h \
 textconfig.h siminterface.h ../cpudb.h paramtree.h ../extplugin.h
vncsrv.o: vncsrv.cpp ../param_names.h ../iodev/iodev.h ../bochs.h \
 ../config.h ../osdep.h ../bx_debug/debug.h ../config.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h \
 ../pc_system.h ../gui/gui.h ../instrument/stubs/instrument.h ../plugin.h \
 ../extplugin.h keymap.h icon_bochs.h font/vga.bitmap.h sdl.h rfb.h \
 rfbkeys.h
win32.o: win32.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h
win32dialog.o: win32dialog.cpp win32dialog.h ../config.h
win32_enh_dbg_osdep.o: win32_enh_dbg_osdep.cpp ../config.h
win32paramdlg.o: win32paramdlg.cpp win32dialog.h ../config.h
wx.o: wx.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h font/vga.bitmap.h wxmain.h
wxdialog.o: wxdialog.cpp ../config.h ../param_names.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../bxversion.h \
 wxdialog.h wxmain.h
wxmain.o: wxmain.cpp ../config.h ../param_names.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../bxversion.h \
 wxdialog.h wxmain.h ../extplugin.h bitmaps/cdromd.xpm bitmaps/copy.xpm \
 bitmaps/floppya.xpm bitmaps/floppyb.xpm bitmaps/paste.xpm \
 bitmaps/power.xpm bitmaps/reset.xpm bitmaps/snapshot.xpm \
 bitmaps/mouse.xpm bitmaps/userbutton.xpm bitmaps/saverestore.xpm
x.o: x.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h enh_dbg.h icon_bochs.xpm \
 font/vga.bitmap.h
amigaos.lo: amigaos.cpp ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h ../bxversion.h \
 ../param_names.h ../iodev/iodev.h ../plugin.h ../extplugin.h
carbon.lo: carbon.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h keymap.h ../iodev/iodev.h ../plugin.h \
 ../extplugin.h ../param_names.h
enh_dbg.lo: enh_dbg.cpp ../config.h
gtk_enh_dbg_osdep.lo: gtk_enh_dbg_osdep.cpp ../config.h
gui.lo: gui.cpp ../iodev/iodev.h ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h ../plugin.h ../extplugin.h \
 ../param_names.h ../iodev/virt_timer.h keymap.h ../gui/bitmaps/floppya.h \
 ../gui/bitmaps/floppyb.h ../gui/bitmaps/mouse.h ../gui/bitmaps/reset.h \
 ../gui/bitmaps/power.h ../gui/bitmaps/snapshot.h ../gui/bitmaps/copy.h \
 ../gui/bitmaps/paste.h ../gui/bitmaps/configbutton.h \
 ../gui/bitmaps/cdromd.h ../gui/bitmaps/userbutton.h \
 ../gui/bitmaps/saverestore.h
keymap.lo: keymap.cpp ../param_names.h ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h keymap.h
macintosh.lo: macintosh.cpp ../param_names.h ../bochs.h ../config.h \
 ../osdep.h ../bx_debug/debug.h ../config.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h \
 ../pc_system.h ../gui/gui.h ../instrument/stubs/instrument.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h
nogui.lo: nogui.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../plugin.h ../extplugin.h \
 ../param_names.h icon_bochs.h
paramtree.lo: paramtree.cpp ../bochs.h ../config.h ../osdep.h \
 ../bx_debug/debug.h ../config.h ../osdep.h ../gui/siminterface.h \
 ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h \
 ../gui/gui.h ../instrument/stubs/instrument.h paramtree.h
rfb.lo: rfb.cpp ../param_names.h ../iodev/iodev.h ../bochs.h ../config.h \
 ../osdep.h ../bx_debug/debug.h ../config.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h \
 ../pc_system.h ../gui/gui.h ../instrument/stubs/instrument.h ../plugin.h \
 ../extplugin.h keymap.h icon_bochs.h font/vga.bitmap.h sdl.h rfb.h \
 rfbkeys.h
scrollwin.lo: scrollwin.cpp ../config.h
sdl2.lo: sdl2.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h
sdl.lo: sdl.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h icon_bochs.h sdl.h sdlkeys.h
siminterface.lo: siminterface.cpp ../param_names.h ../iodev/iodev.h \
 ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h ../config.h \
 ../osdep.h ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h \
 ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../plugin.h ../extplugin.h \
 ../iodev/virt_timer.h
svga.lo: svga.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h ../iodev/iodev.h \
 ../plugin.h ../extplugin.h
term.lo: term.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h ../iodev/iodev.h \
 ../plugin.h ../extplugin.h
textconfig.lo: textconfig.cpp ../config.h ../osdep.h ../param_names.h \
 textconfig.h siminterface.h ../cpudb.h paramtree.h ../extplugin.h
vncsrv.lo: vncsrv.cpp ../param_names.h ../iodev/iodev.h ../bochs.h \
 ../config.h ../osdep.h ../bx_debug/debug.h ../config.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../memory/memory-bochs.h \
 ../pc_system.h ../gui/gui.h ../instrument/stubs/instrument.h ../plugin.h \
 ../extplugin.h keymap.h icon_bochs.h font/vga.bitmap.h sdl.h rfb.h \
 rfbkeys.h
win32.lo: win32.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h
win32dialog.lo: win32dialog.cpp win32dialog.h ../config.h
win32_enh_dbg_osdep.lo: win32_enh_dbg_osdep.cpp ../config.h
win32paramdlg.lo: win32paramdlg.cpp win32dialog.h ../config.h
wx.lo: wx.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h font/vga.bitmap.h wxmain.h
wxdialog.lo: wxdialog.cpp ../config.h ../param_names.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../bxversion.h \
 wxdialog.h wxmain.h
wxmain.lo: wxmain.cpp ../config.h ../param_names.h ../osdep.h \
 ../gui/siminterface.h ../cpudb.h ../gui/paramtree.h ../bxversion.h \
 wxdialog.h wxmain.h ../extplugin.h bitmaps/cdromd.xpm bitmaps/copy.xpm \
 bitmaps/floppya.xpm bitmaps/floppyb.xpm bitmaps/paste.xpm \
 bitmaps/power.xpm bitmaps/reset.xpm bitmaps/snapshot.xpm \
 bitmaps/mouse.xpm bitmaps/userbutton.xpm bitmaps/saverestore.xpm
x.lo: x.cpp ../bochs.h ../config.h ../osdep.h ../bx_debug/debug.h \
 ../config.h ../osdep.h ../gui/siminterface.h ../cpudb.h \
 ../gui/paramtree.h ../memory/memory-bochs.h ../pc_system.h ../gui/gui.h \
 ../instrument/stubs/instrument.h ../param_names.h keymap.h \
 ../iodev/iodev.h ../plugin.h ../extplugin.h enh_dbg.h icon_bochs.xpm \
 font/vga.bitmap.h
