#
# File:     Makefile for MyOrgan
# Authors:  Johan Blok
#           Nicholas Appleton (2010-2011)
# Created:  2006

# currently supported targets
GO_TARGET_RELEASE           = grandorgue
GO_TARGET_DEBUG             = grandorgue_d
GO_TARGET_RELEASE_UNICODE   = grandorgue_u
GO_TARGET_DEBUG_UNICODE     = grandorgue_du

# the following "DEBUG_xxx" settings will only be in effect for a debug build
# configuration.

# do you want extended release alignment debugging information dumped to the
# console?
DEBUG_RELEASE_ALIGNMENT     = yes

# when compiling a debug version of GrandOrgue, do you also want to use
# a debug version of wxWidgets?
DEBUG_WX                    = yes

# Jack and Alsa will only be used if compiling on a Linux platform
USING_JACK                  = yes
USING_ALSA                  = yes
USING_OSS                   = no
USING_DIRECTSOUND           = yes
# USING_MM                  = yes (you have no choice at present)
USING_ASIO                  = no # untested!

# objects to use in the build
OBJECTS = \
GOrgueWave.o \
GOrgueControl.o \
GOrgueCoupler.o \
GOrgueDisplayMetrics.o \
GOrgueDivisional.o \
GOrgueDivisionalCoupler.o \
GOrgueDrawStop.o \
GOrgueEnclosure.o \
GOrgueFrameGeneral.o \
GOrgueGeneral.o \
GOrgueLabel.o \
GOrgueLCD.o \
GOrgueManual.o \
GOrgueMidi.o \
GOrgueMeter.o \
GOrguePipe.o \
GOrguePiston.o \
GOrgueProperties.o \
GOrguePushbutton.o \
GOrgueReleaseAlignTable.o \
GOrgueRtHelpers.o \
GOrgueSound.o \
GOrgueSoundCallbackAudio.o \
GOrgueStop.o \
GOrgueTremulant.o \
GOrgueWindchest.o \
GrandOrgue.o \
GrandOrgueFile.o \
GrandOrgueFrame.o \
Images.o \
IniFileConfig.o \
MIDIListenDialog.o \
OrganDocument.o \
OrganPanel.o \
OrganView.o \
RtAudio.o \
RtMidi.o \
SettingsDialog.o \
SplashScreen.o \
wxGaugeAudio.o

# phony targets
.PHONY: help all clean $(GO_TARGET_RELEASE) $(GO_TARGET_DEBUG) $(GO_TARGET_RELEASE_UNICODE) $(GO_TARGET_DEBUG_UNICODE)

# directory to place object files
OBJ_DIR                     = obj

# directory to place executable files
BIN_DIR                     = bin

# detect operating system
UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
SYSTEM = linux
else
SYSTEM = windows
endif

# object directories for different builds
GO_RELEASE_OBJ_DIR         = $(OBJ_DIR)/$(SYSTEM)/$(GO_TARGET_RELEASE)
GO_DEBUG_OBJ_DIR           = $(OBJ_DIR)/$(SYSTEM)/$(GO_TARGET_DEBUG)
GO_RELEASE_UNICODE_OBJ_DIR = $(OBJ_DIR)/$(SYSTEM)/$(GO_TARGET_RELEASE_UNICODE)
GO_DEBUG_UNICODE_OBJ_DIR   = $(OBJ_DIR)/$(SYSTEM)/$(GO_TARGET_DEBUG_UNICODE)

# object filenames for different builds
GO_RELEASE_OBJS            = $(addprefix $(GO_RELEASE_OBJ_DIR)/,$(OBJECTS))
GO_DEBUG_OBJS              = $(addprefix $(GO_DEBUG_OBJ_DIR)/,$(OBJECTS))
GO_RELEASE_UNICODE_OBJS    = $(addprefix $(GO_RELEASE_UNICODE_OBJ_DIR)/,$(OBJECTS))
GO_DEBUG_UNICODE_OBJS      = $(addprefix $(GO_DEBUG_UNICODE_OBJ_DIR)/,$(OBJECTS))

# the C++ compiler
CXX = g++

# common compilation flags applying to all build modes
CXXFLAGS         = -fomit-frame-pointer -ffast-math -Wall

ifeq ($(SYSTEM),linux)

BIN_EXTENSION =

ifeq ($(USING_JACK),yes)
CXXFLAGS += -D__UNIX_JACK__
LDFLAGS += -ljack
endif

ifeq ($(USING_ALSA),yes)
CXXFLAGS += -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__
LDFLAGS += -lasound
endif

ifeq ($(USING_OSS),yes)
CXXFLAGS += -D__LINUX_OSS__
LDFLAGS += -laoss
endif

LDFLAGS += -lpthread

else

BIN_EXTENSION = .exe

ifeq ($(USING_DIRECTSOUND),yes)
CXXFLAGS += -D__WINDOWS_DS__ -Idirectx/Include
LDFLAGS += -ldsound
endif

ifeq ($(USING_ASIO),yes)
CXXFLAGS += -D__WINDOWS_ASIO__
endif

# most of these libraries were taken straight from the Windows CodeBlocks
# project file. winmm and ole32 are necessary, I'm not sure about the rest...
LDFLAGS += -lwinmm -lcomdlg32 -lshell32 -lcomctl32 -ladvapi32 -lole32 -loleaut32 -luuid
CXXFLAGS += -D__WINDOWS_MM__ # needed for RtMidi

endif

# get common compilation flags
RELEASE_CXXFLAGS = -O2 -mmmx -msse -msse2 -msse3 -DNDEBUG
DEBUG_CXXFLAGS   = -g -O0 -mmmx -msse -msse2 -msse3 -funit-at-a-time
ifeq ($(DEBUG_RELEASE_ALIGNMENT),yes)
DEBUG_CXXFLAGS  += -DPALIGN_DEBUG
endif
ifeq ($(DEBUG_WX),yes)
WX_CONFIG_DEBUG := --debug=yes
else
WX_CONFIG_DEBUG := --debug=no
endif

# build configuration specific compiler flags
GO_RELEASE_CXXFLAGS            = $(CXXFLAGS) $(RELEASE_CXXFLAGS) `wx-config --debug=no --unicode=no --cxxflags`
GO_DEBUG_CXXFLAGS              = $(CXXFLAGS) $(DEBUG_CXXFLAGS) `wx-config $(WX_CONFIG_DEBUG) --unicode=no --cxxflags`
GO_RELEASE_UNICODE_CXXFLAGS    = $(CXXFLAGS) $(RELEASE_CXXFLAGS) `wx-config --debug=no --unicode=yes --cxxflags`
GO_DEBUG_UNICODE_CXXFLAGS      = $(CXXFLAGS) $(DEBUG_CXXFLAGS) `wx-config $(WX_CONFIG_DEBUG) --unicode=yes --cxxflags`

# build configuration specific linker flags
GO_RELEASE_LDFLAGS             = $(LDFLAGS) `wx-config --debug=no --unicode=no --libs`
GO_DEBUG_LDFLAGS               = $(LDFLAGS) `wx-config $(WX_CONFIG_DEBUG) --unicode=no --libs`
GO_RELEASE_UNICODE_LDFLAGS     = $(LDFLAGS) `wx-config --debug=no --unicode=yes --libs`
GO_DEBUG_UNICODE_LDFLAGS       = $(LDFLAGS) `wx-config $(WX_CONFIG_DEBUG) --unicode=yes --libs`

# executable filenames
GO_TARGET_RELEASE_EXE          = $(BIN_DIR)/$(SYSTEM)/$(GO_TARGET_RELEASE)$(BIN_EXTENSION)
GO_TARGET_DEBUG_EXE            = $(BIN_DIR)/$(SYSTEM)/$(GO_TARGET_DEBUG)$(BIN_EXTENSION)
GO_TARGET_RELEASE_UNICODE_EXE  = $(BIN_DIR)/$(SYSTEM)/$(GO_TARGET_RELEASE_UNICODE)$(BIN_EXTENSION)
GO_TARGET_DEBUG_UNICODE_EXE    = $(BIN_DIR)/$(SYSTEM)/$(GO_TARGET_DEBUG_UNICODE)$(BIN_EXTENSION)

# phony targets
help:
	@echo "GrandOrgue Makefile"
	@echo "Type:"
	@echo "  make help          To display this message."
	@echo "  make all           Will build all currently supported configurations"
	@echo "                     of GrandOrgue."
	@echo "  make grandorgue    Will build an ansi release version of GrandOrgue."
	@echo "  make grandorgue_d  Will build an ansi debug version of GrandOrgue."
	@echo "  make grandorgue_ru Will build a unicode release version of GrandOrgue."
	@echo "  make grandorgue_du Will build a unicode debug version of GrandOrgue."
	@echo "  make clean         Will erase all compiled object files and all built"
	@echo "                     executables for this system."

all: $(GO_TARGET_RELEASE_EXE) $(GO_TARGET_DEBUG_EXE) # the unicode targets should be added once supported

$(GO_TARGET_RELEASE): $(GO_TARGET_RELEASE_EXE)

$(GO_TARGET_DEBUG): $(GO_TARGET_DEBUG_EXE)

$(GO_TARGET_RELEASE_UNICODE): $(GO_TARGET_RELEASE_UNICODE_EXE)

$(GO_TARGET_DEBUG_UNICODE): $(GO_TARGET_DEBUG_UNICODE_EXE)

clean: 
	rm -f $(GO_TARGET_RELEASE_EXE) $(GO_TARGET_DEBUG_EXE) $(GO_TARGET_RELEASE_UNICODE_EXE) $(GO_TARGET_DEBUG_UNICODE_EXE) $(GO_RELEASE_OBJS) $(GO_DEBUG_OBJS) $(GO_RELEASE_UNICODE_OBJS) $(GO_DEBUG_UNICODE_OBJS)

# executable targets
$(GO_TARGET_RELEASE_EXE): $(GO_RELEASE_OBJS)
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -o $(GO_TARGET_RELEASE_EXE) $(GO_RELEASE_OBJS) $(GO_RELEASE_LDFLAGS)

$(GO_TARGET_DEBUG_EXE): $(GO_DEBUG_OBJS)
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -o $(GO_TARGET_DEBUG_EXE) $(GO_DEBUG_OBJS) $(GO_DEBUG_LDFLAGS)

$(GO_TARGET_RELEASE_UNICODE_EXE): $(GO_RELEASE_UNICODE_OBJS)
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -o $(GO_TARGET_RELEASE_UNICODE_EXE) $(GO_RELEASE_UNICODE_OBJS) $(GO_RELEASE_UNICODE_LDFLAGS)

$(GO_TARGET_DEBUG_UNICODE_EXE): $(GO_DEBUG_UNICODE_OBJS)
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -o $(GO_TARGET_DEBUG_UNICODE_EXE) $(GO_DEBUG_UNICODE_OBJS) $(GO_DEBUG_UNICODE_LDFLAGS)

# object targets
$(GO_RELEASE_OBJ_DIR)/%.o: %.cpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -c $(GO_RELEASE_CXXFLAGS) -o $@ $<

$(GO_DEBUG_OBJ_DIR)/%.o: %.cpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -c $(GO_DEBUG_CXXFLAGS) -o $@ $<
	
$(GO_RELEASE_UNICODE_OBJ_DIR)/%.o: %.cpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -c $(GO_RELEASE_UNICODE_CXXFLAGS) -o $@ $<
	
$(GO_DEBUG_UNICODE_OBJ_DIR)/%.o: %.cpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) -c $(GO_DEBUG_UNICODE_CXXFLAGS) -o $@ $<
