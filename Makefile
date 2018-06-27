
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
