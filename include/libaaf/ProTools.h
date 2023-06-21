#ifndef __ProTools_h__
#define __ProTools_h__

typedef struct AAF_Iface AAF_Iface;

enum protools_options {
  PROTOOLS_REMOVE_SAMPLE_ACCURATE_EDIT = 1<<0,
  PROTOOLS_REPLACE_CLIP_FADES          = 1<<1,
};

#define PROTOOLS_ALL (PROTOOLS_REMOVE_SAMPLE_ACCURATE_EDIT | PROTOOLS_REPLACE_CLIP_FADES)


int protools_AAF( struct AAF_Iface *aafi );
int protools_post_processing( AAF_Iface *aafi /*, enum protools_options flags*/ );

#endif // ! __ProTools_h__
