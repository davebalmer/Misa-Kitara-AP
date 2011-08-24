
-include ../MisaConfiguration.mak

MISAKITARA=main

ifdef MISAKITARA

CPP=arm-linux-g++
CP=arm-linux-gcc
LINKER=arm-linux-ld
MKLIB=arm-linux-ar r

CFLAG=-O2 -march=armv4 -DLinux -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Application -c
CPPFLAG=-O2 -march=armv4 -DLinux -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Application -c

LDFLAG=-lrt

MISACORELIB = Kitara.a
MISATOOLLIB = BaseSys.a
MISAAP = ap.tar.gz

MISAKITARACOREOBJ = ControlScreen.o Graphics.o Midi.o Synth.o
MISAKITARACOREDEP = ControlScreen.cpp Graphics.cpp Midi.cpp Synth.cpp ControlScreen.h Graphics.h Midi.h Synth.h
MISAKITARATOOLOBJCPP = Neck.o Touchpanel.o
MISAKITARATOOLOBJC = bitmap.o ebmlib.o mapm.o
MISAKITARAMAINOBJ = main.o
TINYXMLOBJ = tinyxml/tinystr.o tinyxml/tinyxmlerror.o tinyxml/tinyxml.o tinyxml/tinyxmlparser.o

MISAMENU=../Application/misa.a
ifdef MISA_USED2D
GUIFW=../Application/ucGUI_D2D.a
else
#GUIFW=../Application/ucGUI.a
GUIFW=../Application/ucGUI_X.a
endif

RES=Resource.so \
	SpecialResource.so \
	Mainmenu.so \
	Quickset.so

all: $(MISAKITARA)
FORCE:
$(MISAKITARA): $(MISAKITARAMAINOBJ) $(MISACORELIB) $(MISATOOLLIB) $(MISAMENU) $(GUIFW) $(RES) $(TINYXMLOBJ)
	$(CPP) $(LDFLAG) -o $(MISAKITARA) $(MISAKITARAMAINOBJ) $(MISACORELIB) $(MISATOOLLIB) $(MISAMENU) $(GUIFW) $(RES) $(TINYXMLOBJ)

$(MISAKITARAMAINOBJ): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

$(MISATOOLLIB):  $(MISAKITARATOOLOBJCPP) $(MISAKITARATOOLOBJC)
	$(MKLIB) $(MISATOOLLIB) $(MISAKITARATOOLOBJCPP) $(MISAKITARATOOLOBJC)

$(MISAKITARATOOLOBJCPP): %.o: %.cpp
	$(CPP) $(CPPFLAG) $<

$(MISAKITARATOOLOBJC): %.o: %.c
	$(CPP) $(CPPFLAG) $<

$(MISACORELIB):
	make -f core.mak

$(MISAAP):
	rm -rf ap
	mkdir ap
	cp ../Application/*.so ap
	cp presets.tar.gz ap
	cd ap && tar zxvf presets.tar.gz
	rm ap/presets.tar.gz
	cp wavetable.txt ap
	cp wavetable.bin ap
	cp 3716-FW_MISA.bin ap
	cp main ap
	rm -f ap.tar.gz
	sleep 1
	tar zcvf ap.tar.gz ap

rebuild:
	make cleanall
	make

core:
	cd tinyxml && make
	make -f core.mak

menu:
	cd ../Application && make
	cp ../Application/*.so .

clean:
	cd tinyxml && make clean
	rm $(MISAKITARA) $(MISAKITARAMAINOBJ) $(MISATOOLLIB) $(MISAKITARATOOLOBJC) $(MISAKITARATOOLOBJCPP) $(MISAKITARACOREOBJ)

cleanall:
	cd tinyxml && make clean
	rm $(MISAKITARA) $(MISAKITARAMAINOBJ) $(MISACORELIB) $(MISATOOLLIB) $(MISAKITARACOREOBJ) $(MISAKITARATOOLOBJC) $(MISAKITARATOOLOBJCPP)

else

CPP=arm-linux-g++
CPPFLAG=-O2 -march=armv4 -DLinux -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Application -lrt -o main

MISAMENU=../Application/misa.a
ifdef MISA_USED2D
GUIFW=../Application/ucGUI_D2D.a
else
#GUIFW=../Application/ucGUI.a
GUIFW=../Application/ucGUI_X.a
endif
#RES=../Application/Resource.so \
#	../Application/Compression.so \
#	../Application/Distortion.so \
#	../Application/Mainmenu.so

RES=Resource.so \
	SpecialResource.so \
	Mainmenu.so
all:
#	arm-linux-g++ -O2 -march=armv4 -DLinux -I../Config -I../GUI/Core -I../GUI/WM -I../GUI/Widget -I../Application -lrt -o main *.cpp *.c misa.a ucGUI.a Resource.so Compression.so Distortion.so Mainmenu.so
	$(CPP) $(CPPFLAG) *.cpp *.c $(MISAMENU) $(GUIFW) $(RES)
# 2> log.txt

menu:
	cd ../Application && make

clean:
	rm main
endif

