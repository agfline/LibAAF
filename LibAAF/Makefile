
CC = gcc
CFLAGS = -W -Wall -g -O3
AR = ar
AFLAGS = rcs

OBJDIR = ../build

.PHONY: depend clean

SRC_A = $(wildcard LibCFB/*.c)
SRC_B = $(wildcard AAFCore/*.c)
SRC_C = $(wildcard AAFIface/*.c)
SRC_D = $(wildcard AAFIface/thirdparty/*.c)

OBJ_A = $(patsubst LibCFB/%.c, ../build/%.o, $(SRC_A))
OBJ_B = $(patsubst AAFCore/%.c, ../build/%.o, $(SRC_B))
OBJ_C = $(patsubst AAFIface/%.c, ../build/%.o, $(SRC_C))
OBJ_D = $(patsubst AAFIface/thirdparty/%.c, ../build/%.o, $(SRC_D))

SRC = $(SRC_A) $(SRC_B) $(SRC_C) $(SRC_D)
OBJ = $(OBJ_A) $(OBJ_B) $(OBJ_C) $(OBJ_D)



libAAF: depend $(OBJDIR)/libAAF.a

depend:
	makedepend -Y $(SRC) 2>/dev/null




$(OBJDIR)/libAAF.a: $(OBJ)
	$(AR) $(AFLAGS) $@ $^



$(OBJDIR)/%.o: AAFCore/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: AAFIface/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: AAFIface/thirdparty/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: LibCFB/%.c
	$(CC) -c $< -o $@ $(CFLAGS)



clean:
	@echo $(SRC)


# DO NOT DELETE

LibCFB/LibCFB.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFToText.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFToText.o: AAFCore/AAFUtils.h AAFCore/AAFCore.h AAFCore/AAFToText.h
AAFCore/AAFToText.o: AAFCore/AAFClass.h AAFCore/AAFDefs/AAFClassDefUIDs.h
AAFCore/AAFToText.o: AAFCore/AAFTypes.h AAFCore/AAFDefs/AAFPropertyIDs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFCompressionDefs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFDataDefs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFOperationDefs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFParameterDefs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFInterpolatorDefs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFExtEnum.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFFileKinds.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFOPDefs.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFContainerDefs.h
AAFCore/AAFCore.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFCore.o: AAFCore/AAFCore.h AAFCore/AAFClass.h AAFCore/AAFUtils.h
AAFCore/AAFCore.o: AAFCore/AAFToText.h AAFCore/AAFDefs/AAFClassDefUIDs.h
AAFCore/AAFCore.o: AAFCore/AAFTypes.h AAFCore/AAFDefs/AAFPropertyIDs.h
AAFCore/AAFCore.o: AAFCore/AAFDefs/AAFFileKinds.h common/debug.h
AAFCore/AAFClass.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFClass.o: AAFCore/AAFClass.h AAFCore/AAFCore.h AAFCore/AAFUtils.h
AAFCore/AAFClass.o: AAFCore/AAFToText.h AAFCore/AAFDefs/AAFClassDefUIDs.h
AAFCore/AAFClass.o: AAFCore/AAFTypes.h AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFCore/AAFClass.o: AAFCore/AAFDefs/AAFPropertyIDs.h common/debug.h
AAFCore/AAFUtils.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFUtils.o: AAFCore/AAFUtils.h AAFCore/AAFCore.h AAFCore/AAFToText.h
AAFCore/AAFUtils.o: AAFCore/AAFClass.h
AAFIface/AAFIface.o: libAAF.h AAFCore/AAFClass.h AAFCore/AAFTypes.h
AAFIface/AAFIface.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h AAFCore/AAFCore.h
AAFIface/AAFIface.o: AAFCore/AAFCore.h AAFCore/AAFUtils.h AAFCore/AAFToText.h
AAFIface/AAFIface.o: AAFCore/AAFClass.h AAFCore/AAFToText.h
AAFIface/AAFIface.o: AAFIface/AAFIface.h AAFCore/AAFTypes.h
AAFIface/AAFIface.o: AAFCore/AAFClass.h AAFIface/AAFIParser.h
AAFIface/AAFIface.o: AAFIface/AAFIface.h AAFCore/AAFDefs/AAFClassDefUIDs.h
AAFIface/AAFIface.o: AAFCore/AAFTypes.h AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFPropertyIDs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFDataDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFOperationDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFParameterDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFInterpolatorDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFContainerDefs.h common/debug.h
AAFIface/AAFIface.o: AAFIface/thirdparty/libriff.h
AAFIface/AAFIface.o: AAFIface/thirdparty/libwav.h
AAFIface/AAFIParser.o: libAAF.h AAFCore/AAFClass.h AAFCore/AAFTypes.h
AAFIface/AAFIParser.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h AAFCore/AAFCore.h
AAFIface/AAFIParser.o: AAFCore/AAFCore.h AAFCore/AAFUtils.h
AAFIface/AAFIParser.o: AAFCore/AAFToText.h AAFCore/AAFClass.h
AAFIface/AAFIParser.o: AAFCore/AAFToText.h AAFIface/AAFIface.h
AAFIface/AAFIParser.o: AAFCore/AAFTypes.h AAFCore/AAFClass.h
AAFIface/AAFIParser.o: AAFIface/AAFIParser.h AAFIface/AAFIface.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFClassDefUIDs.h AAFCore/AAFTypes.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFPropertyIDs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFDataDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFOperationDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFParameterDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFInterpolatorDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFContainerDefs.h common/debug.h
AAFIface/AAFIParser.o: AAFIface/thirdparty/libriff.h
AAFIface/AAFIParser.o: AAFIface/thirdparty/libwav.h
AAFIface/thirdparty/libbext.o: AAFIface/thirdparty/libbext.h
AAFIface/thirdparty/libriff.o: AAFIface/thirdparty/libriff.h
AAFIface/thirdparty/libwav.o: AAFIface/thirdparty/libriff.h
AAFIface/thirdparty/libwav.o: AAFIface/thirdparty/libwav.h
AAFIface/thirdparty/libwav.o: AAFIface/thirdparty/libbext.h
