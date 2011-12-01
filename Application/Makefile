
-include ../MisaConfiguration.mak

CPP=arm-linux-g++
CP=arm-linux-gcc
LINKER=arm-linux-ld
MKLIB=arm-linux-ar r

SYSINC=-I/usr/local/arm/3.4.6/arm-linux/usr/include \
	 -I/usr/local/arm/3.4.6/arm-linux/usr/X11R6/include \
	-I/usr/local/arm/3.4.6/arm-linux/usr/include/libxml2

#CFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Main -DLinux -c
#CPPFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Main -DLinux -c

CFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -DLinux -c
CPPFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -DLinux -c

ifdef MISA_APPLICATION
CFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -DLinux -c
CPPFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -DLinux -c

CFLAG+=-DMISA_APPLICATION
CPPFLAG+=-DMISA_APPLICATION
else
CFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Main -DLinux -c
CPPFLAG=-O2 -march=armv4 -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Main -DLinux -c
endif

ifdef MISA_FB0
CPPFLAG+=-DUSE_FRAMEBUFFER0
CFLAG+=-DUSE_FRAMEBUFFER0
endif

ifdef MISA_USED2D
CPPFLAG+=-DD2DUPDATE
CFLAG+=-DD2DUPDATE
endif

SOFLAG=-shared -fPIC -o
LDFLAG=-lrt -L. -L/usr/test/

DEBUGPRT=-DMISA_DEBUG_PRINT

MAIN=misa
MENULIB=misa.a
SOLIB = Resource.so
SPELIB = SpecialResource.so
MMSOLIB = Mainmenu.so
QSSOLIB = Quickset.so

OBJS = main.o \
	ucGuiMain.o SystemScreen.o MainMenu.o PresetsScreen.o SynthScreen.o BitcrusherScreen.o DistortionScreen.o CompressionScreen.o \
	DelayScreen.o ModulationScreen.o ModulationPresets.o ReverbScreen.o MixerScreen.o EqScreen.o ControlAssignments.o ScenesScreen.o \
	MIDIControlScreen.o VoiceScreen.o MisaQuickSetScreen.o QuickScreen.o Keyboard.o PresetSlideList.o\
	MisaWidget.o MisaAppAssi.o MisamenuConfig.o MisaDialogueBox.o Touchpanel.o
LIBOBJS = ucGuiMain.o SystemScreen.o MainMenu.o PresetsScreen.o SynthScreen.o BitcrusherScreen.o DistortionScreen.o CompressionScreen.o \
	DelayScreen.o ModulationScreen.o ModulationPresets.o ReverbScreen.o MixerScreen.o EqScreen.o ControlAssignments.o ScenesScreen.o \
	MIDIControlScreen.o VoiceScreen.o MisaQuickSetScreen.o QuickScreen.o Keyboard.o PresetSlideList.o\
	MisaWidget.o MisaAppAssi.o MisamenuConfig.o MisaDialogueBox.o
SOBJS = CommonRes.o
SPEOBJS = SpecialRes.o
MMSOBJS = MainmenuRes.o KeyboardRes.o ScenesRes.o
SYTHMIDIOBJS = SynthMIDIRes.o ControlassignmentsRes.o
QUICKSETOBJS = QuickMenuRes.o

ifdef MISA_APPLICATION

all: $(MAIN)

$(MAIN): $(OBJS) $(SOLIB) $(SPELIB) $(MMSOLIB) $(QSSOLIB)
	$(CPP) $(LDFLAG) -o $(MAIN) $(OBJS) $(SOLIB) $(SPELIB) $(MMSOLIB) $(QSSOLIB) ucGUI.a
	
$(OBJS): %.o: %.cpp
	$(CPP) $(CPPFLAG) $(DEBUGPRT) $<
# 2> log.txt

else

all: $(MENULIB)

$(MENULIB): $(LIBOBJS) $(SOLIB) $(SPELIB) $(MMSOLIB) $(QSSOLIB)
	$(MKLIB) $(MENULIB) $(LIBOBJS)

$(LIBOBJS): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

endif
# -v -Wl,-Map,mem.map 

$(SOLIB): $(SOBJS)
	$(CPP) $(SOFLAG) $(SOLIB) $(SOBJS)

$(SPELIB): $(SPEOBJS) $(SYTHMIDIOBJS)
	$(CPP) $(SOFLAG) $(SPELIB) $(SPEOBJS) $(SYTHMIDIOBJS)

$(MMSOLIB): $(MMSOBJS)
	$(CPP) $(SOFLAG) $(MMSOLIB) $(MMSOBJS)

$(QSSOLIB): $(QUICKSETOBJS)
	$(CPP) $(SOFLAG) $(QSSOLIB) $(QUICKSETOBJS)

$(SOBJS): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

$(SPEOBJS): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

$(MMSOBJS): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

$(SYTHMIDIOBJS): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

$(QUICKSETOBJS): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

clean:
	rm $(LIBOBJS) $(MENULIB)

cleanall:
	rm $(OBJS) $(SOBJS) $(SPEOBJS) $(MMSOBJS) $(QUICKSETOBJS) $(SOLIB) $(SPELIB) $(MMSOLIB) $(QSSOLIB) $(MAIN)

