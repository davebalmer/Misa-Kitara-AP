
CPP=arm-linux-g++
CP=arm-linux-gcc
LINKER=arm-linux-ld
MKLIB=arm-linux-ar r

CFLAG=-O2 -march=armv4 -DLinux -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Application -c
CPPFLAG=-O2 -march=armv4 -DLinux -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Application -c

MISACORELIB = Kitara.a

MISAKITARACOREOBJ = ControlScreen.o Graphics.o Midi.o Synth.o
MISAKITARACOREDEP = ControlScreen.cpp Graphics.cpp Midi.cpp Synth.cpp ControlScreen.h Graphics.h Midi.h Synth.h


all: $(MISACORELIB)

$(MISACORELIB): $(MISAKITARACOREOBJ)
	$(MKLIB) $(MISACORELIB) $(MISAKITARACOREOBJ)

$(MISAKITARACOREOBJ): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

