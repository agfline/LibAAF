

#SRCDIR = AAFCore

SRC += AAFClass.c       \
       AAFCore.c        \
       AAFNavigation.c  \
       AAFUtils.c       \
       AAFToText.c      \


# Generate object list from source files
OBJ += $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

#SRC  = $(patsubst %.c, AAFCore/%.c, $(TMP))


#       $(OBJDIR)/LibCFB.o                    
#       $(OBJDIR)/libriff.o

#.PHONY: depend #libcfb aafiface

#.SUFFIXES:




#all: depend $(OBJDIR)/libAAF.a

#depend:
#	makedepend -Y $(SRC) 2>/dev/null
#	@echo



#$(OBJDIR)/libAAF.a: $(OBJ)
#	ar rcs $@ $^


#$(OBJDIR)/%.o: %.c
#	gcc -c -o $@ $< $(CFLAGS)


#$(OBJDIR)/LibCFB.o:
#	+$(MAKE) --environment-overrides --directory LibCFB
#	@echo


#$(OBJDIR)/AAFIface.o:
#	+$(MAKE) --environment-overrides --directory AAFIface
#	@echo



