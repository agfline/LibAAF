
export CC = gcc
export CFLAGS_W = -W -Wall
export CFLAGS_O = -g -O3

export AR=ar

OBJDIR = ./build
BINDIR = ./bin

.PHONY: clean doc tools libAAF

.SUFFIXES:



all: libAAF tools



libAAF:
	+$(MAKE) --no-print-directory -C LibAAF

tools: libAAF
	+$(MAKE) --no-print-directory -C tools

doc:
	+$(MAKE) --no-print-directory -C doc


clean:
	-rm -f $(OBJDIR)/*.o $(OBJDIR)/*.a $(BINDIR)/AAFExtract $(BINDIR)/AAFInfo $(BINDIR)/AAF2Ardour
	@echo

install:
	cp $(BINDIR)/AAFInfo /usr/local/bin/
	cp $(BINDIR)/AAFExtract /usr/local/bin/
	# cp $(BINDIR)/AAF2Ardour /usr/local/bin/
	cp $(BINDIR)/libAAF.so /usr/local/lib/
	mkdir -p /usr/local/include/libAAF/
	cp -r LibAAF/libAAF.h /usr/local/include/libAAF/
	cp -r LibAAF/LibCFB /usr/local/include/libAAF/
	cp -r LibAAF/AAFCore /usr/local/include/libAAF/
	cp -r LibAAF/AAFIface /usr/local/include/libAAF/
	cp -r LibAAF/common/ /usr/local/include/libAAF/
	ldconfig

# DO NOT DELETE
