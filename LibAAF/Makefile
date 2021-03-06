
CC = gcc
CFLAGS = -W -Wall -g -O3 -fPIC
AR = ar
AFLAGS = rcs

OBJDIR = ../build
BINDIR = ../bin

.PHONY: depend clean

SRC_A = $(wildcard LibCFB/*.c)
SRC_B = $(wildcard AAFCore/*.c)
SRC_C = $(wildcard AAFIface/*.c)
SRC_D = $(wildcard common/*.c)

OBJ_A = $(patsubst LibCFB/%.c, ../build/%.o, $(SRC_A))
OBJ_B = $(patsubst AAFCore/%.c, ../build/%.o, $(SRC_B))
OBJ_C = $(patsubst AAFIface/%.c, ../build/%.o, $(SRC_C))
OBJ_D = $(patsubst common/%.c, ../build/%.o, $(SRC_D))

SRC = $(SRC_A) $(SRC_B) $(SRC_C) $(SRC_D)
OBJ = $(OBJ_A) $(OBJ_B) $(OBJ_C) $(OBJ_D)



libAAF: depend $(OBJDIR)/libAAF.a $(BINDIR)/libAAF.so

depend:
	makedepend -Y $(SRC) 2>/dev/null




$(OBJDIR)/libAAF.a: $(OBJ)
	$(AR) $(AFLAGS) $@ $^

$(BINDIR)/libAAF.so: $(OBJ)
	$(CC) -shared $^ -o $@



$(OBJDIR)/%.o: AAFCore/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: AAFIface/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: common/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: LibCFB/%.c
	$(CC) -c $< -o $@ $(CFLAGS)



clean:
	@echo $(SRC)


# DO NOT DELETE

LibCFB/LibCFB.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
LibCFB/LibCFB.o: common/debug.h common/utils.h
LibCFB/CFBDump.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
LibCFB/CFBDump.o: common/utils.h
AAFCore/AAFToText.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFToText.o: AAFCore/AAFUtils.h AAFCore/AAFCore.h AAFCore/AAFToText.h
AAFCore/AAFToText.o: AAFCore/AAFClass.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
AAFCore/AAFToText.o: LibCFB/LibCFB.h common/utils.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFClassDefUIDs.h AAFCore/AAFTypes.h
AAFCore/AAFToText.o: AAFCore/AAFDefs/AAFPropertyIDs.h
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
AAFCore/AAFDump.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFDump.o: AAFCore/AAFUtils.h AAFCore/AAFCore.h AAFCore/AAFToText.h
AAFCore/AAFDump.o: AAFCore/AAFClass.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
AAFCore/AAFDump.o: LibCFB/LibCFB.h common/utils.h
AAFCore/AAFCore.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFCore.o: AAFCore/AAFCore.h AAFCore/AAFClass.h AAFCore/AAFUtils.h
AAFCore/AAFCore.o: AAFCore/AAFToText.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
AAFCore/AAFCore.o: LibCFB/LibCFB.h common/utils.h AAFCore/AAFDump.h
AAFCore/AAFCore.o: AAFCore/AAFDefs/AAFClassDefUIDs.h AAFCore/AAFTypes.h
AAFCore/AAFCore.o: AAFCore/AAFDefs/AAFPropertyIDs.h
AAFCore/AAFCore.o: AAFCore/AAFDefs/AAFFileKinds.h
AAFCore/AAFCore.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h common/debug.h
AAFCore/AAFClass.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFClass.o: AAFCore/AAFClass.h AAFCore/AAFCore.h AAFCore/AAFUtils.h
AAFCore/AAFClass.o: AAFCore/AAFToText.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
AAFCore/AAFClass.o: LibCFB/LibCFB.h common/utils.h
AAFCore/AAFClass.o: AAFCore/AAFDefs/AAFClassDefUIDs.h AAFCore/AAFTypes.h
AAFCore/AAFClass.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFCore/AAFClass.o: AAFCore/AAFDefs/AAFPropertyIDs.h common/debug.h
AAFCore/AAFUtils.o: AAFCore/AAFTypes.h LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFCore/AAFUtils.o: AAFCore/AAFUtils.h AAFCore/AAFCore.h AAFCore/AAFToText.h
AAFCore/AAFUtils.o: AAFCore/AAFClass.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
AAFCore/AAFUtils.o: LibCFB/LibCFB.h common/utils.h
AAFIface/AAFIAudioFiles.o: libAAF.h AAFCore/AAFClass.h AAFCore/AAFTypes.h
AAFIface/AAFIAudioFiles.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFCore.h AAFCore/AAFCore.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFUtils.h AAFCore/AAFToText.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFClass.h LibCFB/CFBTypes.h
AAFIface/AAFIAudioFiles.o: LibCFB/CFBDump.h LibCFB/LibCFB.h common/utils.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFToText.h AAFIface/AAFIface.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFTypes.h AAFCore/AAFClass.h
AAFIface/AAFIAudioFiles.o: AAFIface/AAFIParser.h AAFIface/AAFIface.h
AAFIface/AAFIAudioFiles.o: AAFIface/AAFIAudioFiles.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFClassDefUIDs.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFTypes.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFPropertyIDs.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFDataDefs.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFOperationDefs.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFParameterDefs.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFInterpolatorDefs.h
AAFIface/AAFIAudioFiles.o: AAFCore/AAFDefs/AAFContainerDefs.h
AAFIface/AAFIAudioFiles.o: LibCFB/CFBDump.h AAFCore/AAFDump.h common/debug.h
AAFIface/ProTools.o: libAAF.h AAFCore/AAFClass.h AAFCore/AAFTypes.h
AAFIface/ProTools.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h AAFCore/AAFCore.h
AAFIface/ProTools.o: AAFCore/AAFCore.h AAFCore/AAFUtils.h AAFCore/AAFToText.h
AAFIface/ProTools.o: AAFCore/AAFClass.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
AAFIface/ProTools.o: LibCFB/LibCFB.h common/utils.h AAFCore/AAFToText.h
AAFIface/ProTools.o: AAFIface/AAFIface.h AAFCore/AAFTypes.h
AAFIface/ProTools.o: AAFCore/AAFClass.h AAFIface/AAFIParser.h
AAFIface/ProTools.o: AAFIface/AAFIface.h AAFIface/AAFIAudioFiles.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFClassDefUIDs.h AAFCore/AAFTypes.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFPropertyIDs.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFDataDefs.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFOperationDefs.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFParameterDefs.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFInterpolatorDefs.h
AAFIface/ProTools.o: AAFCore/AAFDefs/AAFContainerDefs.h LibCFB/CFBDump.h
AAFIface/ProTools.o: AAFCore/AAFDump.h AAFIface/./ProTools.h
AAFIface/AAFIface.o: libAAF.h AAFCore/AAFClass.h AAFCore/AAFTypes.h
AAFIface/AAFIface.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h AAFCore/AAFCore.h
AAFIface/AAFIface.o: AAFCore/AAFCore.h AAFCore/AAFUtils.h AAFCore/AAFToText.h
AAFIface/AAFIface.o: AAFCore/AAFClass.h LibCFB/CFBTypes.h LibCFB/CFBDump.h
AAFIface/AAFIface.o: LibCFB/LibCFB.h common/utils.h AAFCore/AAFToText.h
AAFIface/AAFIface.o: AAFIface/AAFIface.h AAFCore/AAFTypes.h
AAFIface/AAFIface.o: AAFCore/AAFClass.h AAFIface/AAFIParser.h
AAFIface/AAFIface.o: AAFIface/AAFIface.h AAFIface/AAFIAudioFiles.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFClassDefUIDs.h AAFCore/AAFTypes.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFPropertyIDs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFDataDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFOperationDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFParameterDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFInterpolatorDefs.h
AAFIface/AAFIface.o: AAFCore/AAFDefs/AAFContainerDefs.h LibCFB/CFBDump.h
AAFIface/AAFIface.o: AAFCore/AAFDump.h common/debug.h
AAFIface/AAFIParser.o: libAAF.h AAFCore/AAFClass.h AAFCore/AAFTypes.h
AAFIface/AAFIParser.o: LibCFB/LibCFB.h LibCFB/CFBTypes.h AAFCore/AAFCore.h
AAFIface/AAFIParser.o: AAFCore/AAFCore.h AAFCore/AAFUtils.h
AAFIface/AAFIParser.o: AAFCore/AAFToText.h AAFCore/AAFClass.h
AAFIface/AAFIParser.o: LibCFB/CFBTypes.h LibCFB/CFBDump.h LibCFB/LibCFB.h
AAFIface/AAFIParser.o: common/utils.h AAFCore/AAFToText.h AAFIface/AAFIface.h
AAFIface/AAFIParser.o: AAFCore/AAFTypes.h AAFCore/AAFClass.h
AAFIface/AAFIParser.o: AAFIface/AAFIParser.h AAFIface/AAFIface.h
AAFIface/AAFIParser.o: AAFIface/AAFIAudioFiles.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFClassDefUIDs.h AAFCore/AAFTypes.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFTypeDefUIDs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFPropertyIDs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFDataDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFOperationDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFParameterDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFInterpolatorDefs.h
AAFIface/AAFIParser.o: AAFCore/AAFDefs/AAFContainerDefs.h LibCFB/CFBDump.h
AAFIface/AAFIParser.o: AAFCore/AAFDump.h AAFIface/./ProTools.h common/debug.h
