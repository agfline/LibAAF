
export CC = gcc
export CFLAGS = -W -Wall -g -O3

OBJDIR = ../build
BINDIR = ../bin

#SRC = AAFExtract.c \
#      AAFInfo.c    \


.PHONY: clean

.SUFFIXES:



all: clean $(BINDIR)/AAFInfo $(BINDIR)/AAFExtract #$(BINDIR)/AAF2Ardour

#depend:
#	makedepend -Y $(SRC) 2>/dev/null

clean:
	rm -f $(BINDIR)/AAFInfo $(BINDIR)/AAFExtract $(BINDIR)/AAF2Ardour $(BINDIR)/Ardour/session/Ardour.ardour $(BINDIR)/Ardour/session/interchange/testF3/audiofiles/* $(BINDIR)/Ardour/session/peaks/*


$(BINDIR)/AAFInfo: AAFInfo.c thirdparty/libTC.c
	$(CC) -o $@ $^ -L$(OBJDIR) $(CFLAGS) -lsndfile -lAAF -lm

$(BINDIR)/AAFExtract: AAFExtract.c
	$(CC) -o $@ $^ -L$(OBJDIR) $(CFLAGS) -lsndfile -lAAF -lm

#$(BINDIR)/AAF2Ardour: $(OBJDIR)/Ardour.o AAF2Ardour.c
#	$(CC) -o $@ $^ -L$(OBJDIR) $(CFLAGS) -lsndfile -lAAF -lm


### Ardour
#$(OBJDIR)/Ardour.o: thirdparty/Ardour/Ardour.c
#	$(CC) -c -o $@ $< $(CFLAGS)
