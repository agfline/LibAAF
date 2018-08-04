#ifndef __CFBDump_h__
#define __CFBDump_h__


#include <stdio.h>

#include "LibCFB.h"
#include "CFBTypes.h"


const wchar_t * CLSIDToText( cfbCLSID_t *clsid );


void cfb_dump_node( CFB_Data *cfbd, cfbNode *node, int print_stream );

void cfb_dump_nodePath( CFB_Data *cfbd, const wchar_t *path, int print_stream );

void cfb_dump_nodeStream( CFB_Data *cfbd, cfbNode *node );

void cfb_dump_nodePathStream( CFB_Data *cfbd, const wchar_t *path );

void cfb_dump_nodePaths( CFB_Data *cfbd, uint32_t prevPath, char strArray[][CFB_PATH_NAME_SZ], uint32_t *str_i, cfbNode *node );


void cfb_dump_header( CFB_Data *cfbd );


void cfb_dump_FAT( CFB_Data *cfbd );

void cfb_dump_MiniFAT( CFB_Data *cfbd );

void cfb_dump_DiFAT( CFB_Data *cfbd );



#endif // ! __CFBDump_h__
