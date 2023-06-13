#ifndef __AAFDump_h__
#define __AAFDump_h__

//
// #include <libaaf/AAFTypes.h>
#include <libaaf/AAFCore.h>
// typedef struct AAF_Data AAF_Data;
// typedef struct aafPropertyIndexHeader_t aafPropertyIndexHeader_t;
// typedef struct aafObject aafObject;
// typedef struct cfbNode cfbNode;



void aaf_dump_Header( AAF_Data *aafd );

void aaf_dump_Identification( AAF_Data *aafd );


void aaf_dump_rawProperties( AAF_Data *aafd, aafPropertyIndexHeader_t *PropHeader );

void aaf_dump_ObjectProperties( AAF_Data *aafd, aafObject *Obj );

void aaf_dump_nodeStreamProperties( AAF_Data *aafd, cfbNode *node );


void aaf_dump_MetaDictionary( AAF_Data *aafd );

void aaf_dump_Classes( AAF_Data *aafd );


#endif // ! __AAFDump_h__
