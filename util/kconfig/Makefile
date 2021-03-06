# ===========================================================================
# Kernel configuration targets
# These targets are used from top-level makefile

export LC_ALL=C
export LANG=C

DEFCONFIG?=defconfig

PHONY += oldconfig xconfig gconfig menuconfig config silentoldconfig update-po-config \
	localmodconfig localyesconfig

_OS=$(shell uname -s |cut -c-7)
regex-objs=
ifeq ($(_OS),MINGW32)
	regex-objs=regex.o
endif

Kconfig ?= src/Kconfig

xconfig: $(objk)/qconf
	$< $(Kconfig)

gconfig: $(objk)/gconf
	cp -f $(srck)/gconf.glade $(objk)/gconf.glade
	$< $(Kconfig)

menuconfig: $(objk)/mconf
	$< $(Kconfig)

config: $(objk)/conf
	$< --oldaskconfig $(Kconfig)

nconfig: $(objk)/nconf
	$< $(Kconfig)

# Disable strict mode because oldconfig is typically used to clean up
# templates and the like. The second invocation should already have sane data.
oldconfig: $(objk)/conf
	KCONFIG_STRICT= $< --$@ $(Kconfig)
	$< --silentoldconfig $(Kconfig)

silentoldconfig: $(objk)/conf
	$< --$@ $(Kconfig)

# --- UNUSED, ignore ----------------------------------------------------------
localyesconfig localmodconfig: $(obj)/streamline_config.pl $(obj)/conf
	$(Q)mkdir -p include/generated
	$(Q)perl $< --$@ $(srctree) $(Kconfig) > .tmp.config
	$(Q)if [ -f .config ]; then 					\
			cmp -s .tmp.config .config ||			\
			(mv -f .config .config.old.1;			\
			 mv -f .tmp.config .config;			\
			 $(obj)/conf --silentoldconfig $(Kconfig);	\
			 mv -f .config.old.1 .config.old)		\
	else								\
			mv -f .tmp.config .config;			\
			$(obj)/conf --silentoldconfig $(Kconfig);	\
	fi
	$(Q)rm -f .tmp.config

# Create new linux.pot file
# Adjust charset to UTF-8 in .po file to accept UTF-8 in Kconfig files
update-po-config: $(obj)/kxgettext $(obj)/gconf.glade.h
	$(Q)echo "  GEN     config.pot"
	$(Q)xgettext --default-domain=linux                         \
	    --add-comments --keyword=_ --keyword=N_                 \
	    --from-code=UTF-8                                       \
	    --files-from=$(srctree)/scripts/kconfig/POTFILES.in     \
	    --directory=$(srctree) --directory=$(objtree)           \
	    --output $(obj)/config.pot
	$(Q)sed -i s/CHARSET/UTF-8/ $(obj)/config.pot
	$(Q)(for i in `ls $(srctree)/arch/*/Kconfig      \
	    $(srctree)/arch/*/um/Kconfig`;               \
	    do                                           \
		echo "  GEN     $$i";                    \
		$(obj)/kxgettext $$i                     \
		     >> $(obj)/config.pot;               \
	    done )
	$(Q)echo "  GEN     linux.pot"
	$(Q)msguniq --sort-by-file --to-code=UTF-8 $(obj)/config.pot \
	    --output $(obj)/linux.pot
	$(Q)rm -f $(obj)/config.pot
# --- UNUSED, ignore ----------------------------------------------------------

PHONY += allnoconfig allyesconfig allmodconfig alldefconfig randconfig savedefconfig

allnoconfig allyesconfig allmodconfig alldefconfig randconfig: $(objk)/conf
	$< --$@ $(Kconfig)

savedefconfig: $(objk)/conf
	$< --$@=$(DEFCONFIG) $(Kconfig)

defconfig: $(objk)/conf
ifeq ($(KBUILD_DEFCONFIG),)
	$< --defconfig $(Kconfig)
else
	@echo "*** Default configuration is based on '$(KBUILD_DEFCONFIG)'"
	$< --defconfig=$(KBUILD_DEFCONFIG) $(Kconfig)
endif

olddefconfig: $(objk)/conf
	$< --olddefconfig $(Kconfig)

%_defconfig: $(objk)/conf
	$< --defconfig=configs/$@ $(Kconfig)

# Help text used by make help
help_kconfig help::
	@echo  '*** Kconfig Targets ***'
	@echo  '  config	  - Update current config utilising a line-oriented program'
	@echo  '  nconfig         - Update current config utilising a ncurses menu based program'
	@echo  '  menuconfig	  - Update current config utilising a menu based program'
	@echo  '  xconfig	  - Update current config utilising a QT based front-end'
	@echo  '  gconfig	  - Update current config utilising a GTK based front-end'
	@echo  '  oldconfig	  - Update current config utilising a provided .config as base'
	#@echo  '  localmodconfig  - Update current config disabling modules not loaded'
	#@echo  '  localyesconfig  - Update current config converting local mods to core'
	@echo  '  silentoldconfig - Same as oldconfig, but quietly, additionally update deps'
	@echo  '  defconfig	  - New config with default answer to all options'
	@echo  '  savedefconfig   - Save current config as ./defconfig (minimal config)'
	#@echo  '  allnoconfig	  - New config where all options are answered with no'
	#@echo  '  allyesconfig	  - New config where all options are accepted with yes'
	#@echo  '  allmodconfig	  - New config selecting modules when possible'
	#@echo  '  randconfig	  - New config with random answer to all options'
	@echo

# lxdialog stuff
check-lxdialog  := $(srck)/lxdialog/check-lxdialog.sh

# Use recursively expanded variables so we do not call gcc unless
# we really need to do so. (Do not call gcc as part of make mrproper)
HOST_EXTRACFLAGS += $(shell $(CONFIG_SHELL) $(check-lxdialog) -ccflags) \
                    -DLOCALE
HOST_LOADLIBES   = $(shell $(CONFIG_SHELL) $(check-lxdialog) -ldflags $(HOSTCC))


# ===========================================================================
# Shared Makefile for the various kconfig executables:
# conf:	  Used for defconfig, oldconfig and related targets
# nconf:  Used for the nconfig target.
#         Utilizes ncurses
# mconf:  Used for the menuconfig target
#         Utilizes the lxdialog package
# qconf:  Used for the xconfig target
#         Based on QT which needs to be installed to compile it
# gconf:  Used for the gconfig target
#         Based on GTK which needs to be installed to compile it
# object files used by all kconfig flavours

lxdialog := lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o
lxdialog += lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o

conf-objs	:= conf.o  zconf.tab.o $(regex-objs)
mconf-objs     := mconf.o zconf.tab.o $(lxdialog) $(regex-objs)
nconf-objs     := nconf.o zconf.tab.o nconf.gui.o
kxgettext-objs	:= kxgettext.o zconf.tab.o
qconf-cxxobjs	:= qconf.o
qconf-objs	:= zconf.tab.o
gconf-objs	:= gconf.o zconf.tab.o

hostprogs-y := conf

ifeq ($(MAKECMDGOALS),nconfig)
	hostprogs-y += nconf
endif

ifeq ($(MAKECMDGOALS),menuconfig)
	hostprogs-y += mconf
endif

ifeq ($(MAKECMDGOALS),update-po-config)
	hostprogs-y += kxgettext
endif

ifeq ($(MAKECMDGOALS),xconfig)
	qconf-target := 1
endif
ifeq ($(MAKECMDGOALS),gconfig)
	gconf-target := 1
endif


ifeq ($(qconf-target),1)
	hostprogs-y += qconf
endif

ifeq ($(gconf-target),1)
	hostprogs-y += gconf
endif

clean-files	:= qconf.moc .tmp_qtcheck .tmp_gtkcheck
clean-files	+= zconf.tab.c zconf.lex.c zconf.hash.c gconf.glade.h
clean-files     += mconf qconf gconf nconf
clean-files     += config.pot linux.pot

# Check that we have the required ncurses stuff installed for lxdialog (menuconfig)
PHONY += $(objk)/dochecklxdialog
$(addprefix $(obj)/,$(lxdialog)): $(objk)/dochecklxdialog
$(objk)/dochecklxdialog:
	$(CONFIG_SHELL) $(check-lxdialog) -check $(HOSTCC) $(HOST_EXTRACFLAGS) $(HOST_LOADLIBES)

always := dochecklxdialog

# Add environment specific flags
HOST_EXTRACFLAGS += $(shell $(CONFIG_SHELL) $(srck)/check.sh $(HOSTCC) $(HOSTCFLAGS))

# generated files seem to need this to find local include files
HOSTCFLAGS_zconf.lex.o	:= -I$(src)
HOSTCFLAGS_zconf.tab.o	:= -I$(src)

LEX_PREFIX_zconf	:= zconf
YACC_PREFIX_zconf	:= zconf

HOSTLOADLIBES_qconf	= $(KC_QT_LIBS) -ldl
HOSTCXXFLAGS_qconf.o	= $(KC_QT_CFLAGS)

HOSTLOADLIBES_gconf	= `pkg-config --libs gtk+-2.0 gmodule-2.0 libglade-2.0`
HOSTCFLAGS_gconf.o	= `pkg-config --cflags gtk+-2.0 gmodule-2.0 libglade-2.0` \
                          -Wno-missing-prototypes

HOSTLOADLIBES_mconf   = $(shell $(CONFIG_SHELL) $(check-lxdialog) -ldflags $(HOSTCC))

HOSTLOADLIBES_nconf	= $(shell \
				pkg-config --libs menu panel ncurses 2>/dev/null \
				|| echo "-lmenu -lpanel -lncurses"  )

$(objk)/qconf.o: $(objk)/.tmp_qtcheck

ifeq ($(qconf-target),1)
$(objk)/.tmp_qtcheck: $(srck)/Makefile
-include $(objk)/.tmp_qtcheck

# QT needs some extra effort...
$(objk)/.tmp_qtcheck:
	@set -e; echo "  CHECK   qt"; dir=""; pkg=""; \
	if pkg-config --exists Qt5Core; then \
	  cflags="-std=c++11 -fPIC `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets`"; \
	  libs=`pkg-config --libs Qt5Core Qt5Gui Qt5Widgets`; \
	  moc=`pkg-config --variable=host_bins Qt5Core`/moc; \
	elif pkg-config --exists QtCore; then \
	  cflags=`pkg-config --cflags QtCore QtGui`; \
	  libs=`pkg-config --libs QtCore QtGui`; \
	  moc=`pkg-config --variable=moc_location QtCore`; \
	else \
	  echo >&2 "*"; \
	  echo >&2 "* Could not find Qt via pkg-config."; \
	  echo >&2 "* Please install either Qt 4.8 or 5.x. and make sure it's in PKG_CONFIG_PATH"; \
	  echo >&2 "*"; \
	  exit 1; \
	fi; \
	echo "KC_QT_CFLAGS=$$cflags" > $@; \
	echo "KC_QT_LIBS=$$libs" >> $@; \
	echo "KC_QT_MOC=$$moc" >> $@
endif

$(objk)/gconf.o: $(objk)/.tmp_gtkcheck

ifeq ($(gconf-target),1)
-include $(objk)/.tmp_gtkcheck

# GTK needs some extra effort, too...
$(objk)/.tmp_gtkcheck:
	@if `pkg-config --exists gtk+-2.0 gmodule-2.0 libglade-2.0`; then		\
		if `pkg-config --atleast-version=2.0.0 gtk+-2.0`; then			\
			touch $@;								\
		else									\
			echo >&2 "*"; 							\
			echo >&2 "* GTK+ is present but version >= 2.0.0 is required.";	\
			echo >&2 "*";							\
			false;								\
		fi									\
	else										\
		echo >&2 "*"; 								\
		echo >&2 "* Unable to find the GTK+ installation. Please make sure that"; 	\
		echo >&2 "* the GTK+ 2.0 development package is correctly installed..."; 	\
		echo >&2 "* You need gtk+-2.0, glib-2.0 and libglade-2.0."; 		\
		echo >&2 "*"; 								\
		false;									\
	fi
endif

# --- UNUSED, ignore ----------------------------------------------------------
ifdef UNUSED
$(obj)/zconf.tab.o: $(obj)/zconf.lex.c $(obj)/zconf.hash.c

$(obj)/qconf.o: $(obj)/qconf.moc $(obj)/lkc_defs.h

$(obj)/gconf.o: $(obj)/lkc_defs.h

$(obj)/%.moc: $(srck)/%.h
	$(KC_QT_MOC) -i $< -o $@

$(obj)/lkc_defs.h: $(src)/lkc_proto.h
	sed < $< > $@ 's/P(\([^,]*\),.*/#define \1 (\*\1_p)/'

# Extract gconf menu items for I18N support
$(obj)/gconf.glade.h: $(obj)/gconf.glade
	intltool-extract --type=gettext/glade $(obj)/gconf.glade
endif
# --- UNUSED, ignore ----------------------------------------------------------

###
# The following requires flex/bison/gperf
# By default we use the _shipped versions, uncomment the following line if
# you are modifying the flex/bison src.
# LKC_GENPARSER := 1

ifdef LKC_GENPARSER

# --- UNUSED, ignore ----------------------------------------------------------
$(obj)/zconf.tab.c: $(src)/zconf.y
$(obj)/zconf.lex.c: $(src)/zconf.l
$(obj)/zconf.hash.c: $(src)/zconf.gperf

%.tab.c: %.y
	bison -t -l -b $* -p $(notdir $*) $<

%.tab.c_shipped: %.tab.c
	cp $< $@

%.lex.c: %.l
	flex -L -P$(notdir $*) -o$@ $<

%.lex.c_shipped: %.lex.c
	cp $< $@

%.hash.c: %.gperf
	gperf < $< > $@
	cp $@ $@_shipped
# --- UNUSED, ignore ----------------------------------------------------------

endif

$(objk)/qconf: $(patsubst %,$(objk)/%,$(qconf-cxxobjs)) \
	       $(patsubst %,$(objk)/%,$(qconf-objs))
	$(HOSTCXX) $(HOSTCXXFLAGS) -o $@ $^ $(HOSTLOADLIBES_qconf)
$(objk)/gconf: $(patsubst %,$(objk)/%,$(gconf-objs))
	$(HOSTCC) $(HOSTCFLAGS) -o $@ $^ $(HOSTLOADLIBES_gconf)
$(objk)/mconf: $(patsubst %,$(objk)/%,$(mconf-objs))
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) $^ -o $@ $(HOST_LOADLIBES)
$(objk)/nconf: $(patsubst %,$(objk)/%,$(nconf-objs))
	$(HOSTCC) $(HOSTCFLAGS) -o $@ $^ $(HOSTLOADLIBES_nconf)
$(objk)/conf: $(patsubst %,$(objk)/%,$(conf-objs))
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) -o $@ $^

$(objk)/mconf.o: $(srck)/mconf.c
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) -c -o $@ $<
$(objk)/nconf.o: $(srck)/nconf.c
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) -c -o $@ $<
$(objk)/nconf.gui.o: $(srck)/nconf.gui.c
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) -c -o $@ $<
$(objk)/conf.o: $(srck)/conf.c
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) -c -o $@ $<
$(objk)/regex.o: $(srck)/regex.c
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) -DHAVE_STRING_H -c -o $@ $<

$(objk)/zconf.tab.o: $(objk)/zconf.tab.c $(objk)/zconf.lex.c \
		     $(objk)/zconf.hash.c
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) -c -o $@ $(objk)/zconf.tab.c
$(objk)/qconf.o: $(srck)/qconf.cc $(objk)/qconf.moc $(objk)/lkc_defs.h
	$(HOSTCXX) $(HOSTCXXFLAGS) $(HOSTCXXFLAGS_qconf.o) -c -o $@ $<
$(objk)/gconf.o: $(srck)/gconf.c $(objk)/lkc_defs.h
	$(HOSTCC) $(HOSTCFLAGS) $(HOSTCFLAGS_gconf.o) -c -o $@ $<
$(objk)/%.moc: $(srck)/%.h
	$(KC_QT_MOC) -i $< -o $@
$(objk)/lkc_defs.h: $(srck)/lkc_proto.h
	sed < $< > $@ 's/P(\([^,]*\),.*/#define \1 (\*\1_p)/'

$(objk)/zconf.lex.c: $(srck)/zconf.lex.c_shipped
	cp $< $@
$(objk)/zconf.hash.c: $(srck)/zconf.hash.c_shipped
	cp $< $@
$(objk)/zconf.tab.c: $(srck)/zconf.tab.c_shipped
	cp $< $@

$(objk)/lxdialog/lxdialog: $(objk)/dochecklxdialog \
			   $(patsubst %,$(objk)/lxdialog/%,$(lxdialog))
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) \
		$(patsubst %,$(objk)/lxdialog/%,$(lxdialog)) $(HOST_LOADLIBES) -o $@
$(objk)/lxdialog/%.o: $(srck)/lxdialog/%.c
	$(HOSTCC) $(HOSTCFLAGS) $(HOST_EXTRACFLAGS) $^ -c -o $@

$(objk)/%.o: HOSTCFLAGS+=-I$(srck) -I$(objk)
$(objk)/%.o: HOSTCXXFLAGS+=-I$(srck) -I$(objk)
