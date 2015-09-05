# @version $Id:$
#
# @brief Makefile for this component.
#
# @author Kenji MINOURA / kenji@kandj.org
#
# Copyright (c) 2012 The KJSD Project. All rights reserved.

TARGET_NAME = kjsd
# EXE, LIB
TARGET_TYPE = LIB
TARGET_MAJOR_VERSION = 1
TARGET_MINOR_VERSION = 0
TARGET_PATCH_LEVEL = 3

SRCDIR = ./src
INCDIR = ./include
TESTDIR = ./test
DOCDIR = ./doc
BINDIR = ./bin

PREFIX = /usr/local
CD = cd
CC = gcc
CXX = g++
LD = $(CXX)
AR = ar
CP = cp
RM = rm -f
FIND = find
TOUCH = touch
XARGS = xargs
TAGS = etags
INSTALL = install
UNAME = uname
#LDCONFIG = ldconfig
LDCONFIG =
DOXYGEN = doxygen
DVIPDFM = dvipdfmx
DOXYGEN_CFG = $(DOCDIR)/doxygen.cfg
DOXYGEN_STY = $(DOCDIR)/doxygen.sty
SVN_STY = $(DOCDIR)/svn.sty
DOXYGEN_LATEXDIR = $(DOCDIR)/latex
TAGS_FILE = ./TAGS
DEPEND_CC = $(CC) -MM -w
DEPEND_CXX = $(CXX) -MM -w
DEPEND_FILE = .depend
LD_LIBRARY_PATH = LD_LIBRARY_PATH

# PLATFORM contains the target os for which the package is executed.
PLATFORM = $(shell $(UNAME))

# Defines
ifeq ($(PLATFORM), Linux)
DEFINES += KJSD_HAVE_POSIX_REALTIME_EXTENSION
DEFINES_TEST += KJSD_HAVE_POSIX_REALTIME_EXTENSION TEST_SPEED
else
DEFINES +=
DEFINES_TEST += TEST_SPEED
endif

ifeq ($(MAKECMDGOALS), release)
DEFINES += NDEBUG
CFLAGS += -O2
else
CFLAGS += -g
endif

ifeq ($(MAKECMDGOALS), static)
LDFLAGS += -static
endif

ifeq ($(MAKECMDGOALS), static_test)
LDFLAGS_TEST += -static
endif

# The Compiler shall use EXT_LIBDIR as a library dir (-L)
EXT_LIBDIR =
EXT_LIBDIR_TEST = $(BINDIR)

# The Compiler shall use EXT_INCDIR as an include dir (-I)
EXT_INCDIR =
EXT_INCDIR_TEST = ./include

# The Compiler shall use EXT_LIB as a library (-l)
EXT_LIB =
EXT_LIB_TEST = $(TARGET_NAME) cppunit

ifneq (,$(findstring KJSD_HAVE_POSIX_REALTIME_EXTENSION, $(DEFINES)))
EXT_LIB += rt
endif
ifneq (,$(findstring KJSD_HAVE_POSIX_REALTIME_EXTENSION, $(DEFINES_TEST)))
EXT_LIB_TEST += rt
endif

# The "install" target shall install binaries (like executables, no libraries)
# to INSTALL_BIN
INSTALL_BIN = $(PREFIX)/bin

# The "install" target shall install libraries to INSTALL_BIN
INSTALL_LIB = $(PREFIX)/lib

# The "install" target shall install header files that are to be 
# exported to INSTALL_HEADER
INSTALL_HEADER = $(PREFIX)/include/$(TARGET_NAME)

# The "install" target shall install miscellaneous files to INSTALL_ETC
INSTALL_ETC = $(PREFIX)/etc

# ----------------------------------------------------------------------------
# The sourcedir will be configured by configure
S = $(SRCDIR)
H = $(INCDIR)
T = $(TESTDIR)
VPATH = $(S) $(T) $(H) $(BINDIR)

HEADERS = $(shell $(FIND) $(H) -name "*.h" -or -name "*.hpp")

SRC_C = $(shell $(FIND) $(S) -name "*.c" -exec basename {} \;)
SRC_C_TEST = $(shell $(FIND) $(T) -name "*.c" -exec basename {} \;)
SRC_CXX = $(shell $(FIND) $(S) -name "*.cpp" -exec basename {} \;)
SRC_CXX_TEST = $(shell $(FIND) $(T) -name "*.cpp" -exec basename {} \;)

SRCS = $(SRC_C) $(SRC_C_TEST) $(SRC_CXX) $(SRC_CXX_TEST)

ifeq ($(TARGET_TYPE), EXE)
SRC_C_EXE = $(SRC_C)
SRC_CXX_EXE = $(SRC_CXX)
endif
ifeq ($(TARGET_TYPE), LIB)
SRC_C_LIB = $(SRC_C)
SRC_CXX_LIB = $(SRC_CXX)
endif

# The final Target
ifeq ($(TARGET_TYPE), EXE)
TARGET = $(TARGET_NAME)
endif

ifeq ($(TARGET_TYPE), LIB)
TARGET_LIBNAME = lib$(TARGET_NAME)
TARGET_LIB = $(TARGET_LIBNAME).a

TARGET_VERSION_FULL = $(TARGET_MAJOR_VERSION).$(TARGET_MINOR_VERSION).$(TARGET_PATCH_LEVEL)
TARGET_VERSION_BCOMPAT = $(TARGET_MAJOR_VERSION).$(TARGET_MINOR_VERSION)
TARGET_VERSION_COMPAT = $(TARGET_MAJOR_VERSION)

ifeq ($(PLATFORM), Linux)
TARGET_SO = $(TARGET_SONAME).$(TARGET_VERSION_FULL)
TARGET_BCOMPAT = $(TARGET_SONAME).$(TARGET_VERSION_BCOMPAT)
TARGET_COMPAT = $(TARGET_SONAME).$(TARGET_VERSION_COMPAT)
TARGET_SONAME = $(TARGET_LIBNAME).so
endif
ifeq ($(PLATFORM), Darwin)
TARGET_SO = $(TARGET_LIBNAME).$(TARGET_VERSION_FULL).dylib
TARGET_BCOMPAT = $(TARGET_LIBNAME).$(TARGET_VERSION_BCOMPAT).dylib
TARGET_COMPAT = $(TARGET_LIBNAME).$(TARGET_VERSION_COMPAT).dylib
TARGET_SONAME = $(TARGET_LIBNAME).dylib
LD_LIBRARY_PATH = DYLD_LIBRARY_PATH
endif

endif

TARGET_TEST = test_$(TARGET_NAME)

# ----------------------------------------------------------------------------
# 
# ----------------------------------------------------------------------------

# A list of intermediate object files, derived from the source file names
OBJ_C_EXE = $(addsuffix .o, $(basename $(SRC_C_EXE)))
OBJ_C_LIB = $(addsuffix .o, $(basename $(SRC_C_LIB)))
OBJ_C_TEST = $(addsuffix .o, $(basename $(SRC_C_TEST)))
OBJ_CXX_EXE = $(addsuffix .o, $(basename $(SRC_CXX_EXE)))
OBJ_CXX_LIB = $(addsuffix .o, $(basename $(SRC_CXX_LIB)))
OBJ_CXX_TEST = $(addsuffix .o, $(basename $(SRC_CXX_TEST)))

OBJ = $(OBJ_C_EXE) $(OBJ_C_LIB) $(OBJ_CXX_EXE) $(OBJ_CXX_LIB)
OBJ_TEST = $(OBJ_C_TEST) $(OBJ_CXX_TEST)

# Flags for the C-Compiler
CFLAGS +=  -Wall $(addprefix -D, $(DEFINES)) $(addprefix -I, $(EXT_INCDIR))
CFLAGS_LIB += -fPIC $(CFLAGS)
CFLAGS_TEST += -Wall $(addprefix -D, $(DEFINES_TEST)) $(addprefix -I, $(EXT_INCDIR_TEST))
CXXFLAGS += $(CFLAGS) -Wno-return-type
CXXFLAGS_LIB += $(CFLAGS_LIB)
CXXFLAGS_TEST += $(CFLAGS_TEST) -Wno-return-type

# Flags for the linker.
LDFLAGS += $(addprefix -L, $(EXT_LIBDIR)) $(addprefix -l, $(EXT_LIB))

ifeq ($(PLATFORM), Linux)
LDFLAGS_LIB += -shared -Wl,-soname,$(TARGET_COMPAT) $(LDFLAGS)
endif
ifeq ($(PLATFORM), Darwin)
LDFLAGS_LIB += -dynamiclib -install_name $(TARGET_COMPAT) \
	-compatibility_version $(TARGET_VERSION_BCOMPAT) \
	-current_version $(TARGET_VERSION_FULL) \
	$(LDFLAGS)
endif

LDFLAGS_TEST += $(addprefix -L, $(EXT_LIBDIR_TEST)) $(addprefix -l, $(EXT_LIB_TEST))


# ----------------------------------------------------------------------------

.PHONY: all release test clean doc install tags depend

# ------------- make interface ------------------------------------------
ALL = $(TARGET) $(TARGET_SO) $(TARGET_LIB)
ALL_TEST = $(TARGET_TEST)

all: $(ALL)
release: clean $(ALL)
static: $(ALL)
test: $(ALL_TEST)
	$(LD_LIBRARY_PATH)=bin bin/test_kjsd

ifneq ($(MAKECMDGOALS), clean)
include $(DEPEND_FILE)
endif

$(OBJ_C_EXE): %.o: %.c
	$(CC) $(CFLAGS) -c -o $(BINDIR)/$@ $<
$(OBJ_C_LIB): %.o: %.c
	$(CC) $(CFLAGS_LIB) -c -o $(BINDIR)/$@ $<
$(OBJ_C_TEST): %.o: %.c
	$(CC) $(CFLAGS_TEST) -c -o $(BINDIR)/$@ $<
$(OBJ_CXX_EXE): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BINDIR)/$@ $<
$(OBJ_CXX_LIB): %.o: %.cpp
	$(CXX) $(CXXFLAGS_LIB) -c -o $(BINDIR)/$@ $<
$(OBJ_CXX_TEST): %.o: %.cpp
	$(CXX) $(CXXFLAGS_TEST) -c -o $(BINDIR)/$@ $<

%.a: $(OBJ)
	$(AR) rcsv $(BINDIR)/$@ $(addprefix $(BINDIR)/, $(OBJ))

$(TARGET): $(OBJ)
	$(LD) -o $(BINDIR)/$(TARGET)  $(addprefix $(BINDIR)/, $(OBJ)) $(LDFLAGS)

$(TARGET_SO): $(OBJ)
	$(LD) -o $(BINDIR)/$(TARGET_SO)  $(addprefix $(BINDIR)/, $(OBJ)) $(LDFLAGS_LIB)
	($(CD) $(BINDIR); \
	ln -fs $(TARGET_SO) $(TARGET_COMPAT); \
	ln -fs $(TARGET_COMPAT) $(TARGET_SONAME))

$(ALL_TEST): $(ALL) $(OBJ_TEST)
	$(LD) -o $(BINDIR)/$(ALL_TEST) $(addprefix $(BINDIR)/, $(OBJ_TEST)) $(LDFLAGS_TEST) 

doc: $(DOXYGEN_CFG)
	$(DOXYGEN) $(DOXYGEN_CFG)
	$(CP) $(SVN_STY) $(DOXYGEN_LATEXDIR)
	($(CD) $(DOXYGEN_LATEXDIR) && make && $(DVIPDFM) refman)

clean:
	$(RM) -rf $(BINDIR)/*

tags: $(TAGS_FILE)

$(TAGS_FILE): $(SRCS)
	$(FIND) . -regex '.*\.\(cpp\|c\|h\)' -print | $(XARGS) $(TAGS)

install: install-etc install-header install-lib install-target

install-etc: $(ETCS)
	install -d $(INSTALL_ETC)
	for i in $^; do \
		install $$i $(INSTALL_ETC); \
	done

install-target: $(TARGET)
	install -d $(INSTALL_BIN)
	for i in $^; do \
		install $$i $(INSTALL_BIN); \
	done

install-lib: $(TARGET_SO) $(TARGET_LIB)
	install -d $(INSTALL_LIB)
	for i in $^; do \
		install $$i $(INSTALL_LIB); \
	done
	($(CD) $(INSTALL_LIB); \
	ln -fs $(TARGET_SO) $(TARGET_COMPAT); \
	ln -fs $(TARGET_COMPAT) $(TARGET_SONAME))
	$(LDCONFIG)

install-header: $(HEADERS)
	install -d $(INSTALL_HEADER)
	for i in $^; do \
		install $$i $(INSTALL_HEADER); \
	done

depend: $(DEPEND_FILE)

$(DEPEND_FILE): $(SRCS)
	@$(RM) $(DEPEND_FILE)
	@$(TOUCH) $(DEPEND_FILE)
	@for i in $(SRC_C); do \
		$(FIND) $(S) -name $$i -exec $(DEPEND_CC) $(CFLAGS) {} \; >> $(DEPEND_FILE); \
	done
	@for i in $(SRC_C_TEST); do \
		$(FIND) $(T) -name $$i -exec $(DEPEND_CC) $(CFLAGS_TEST) {} \; >> $(DEPEND_FILE); \
	done
	@for i in $(SRC_CXX); do \
		$(FIND) $(S) -name $$i -exec $(DEPEND_CXX) $(CXXFLAGS) {} \; >> $(DEPEND_FILE); \
	done
	@for i in $(SRC_CXX_TEST); do \
		$(FIND) $(T) -name $$i -exec $(DEPEND_CXX) $(CXXFLAGS_TEST) {} \; >> $(DEPEND_FILE); \
	done
