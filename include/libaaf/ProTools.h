#ifndef __ProTools_h__
#define __ProTools_h__


typedef struct AAF_Iface AAF_Iface;


#define PROTOOLS_PP_REMOVE_SAMPLE_ACCURATE_EDIT     0x0001
#define PROTOOLS_PP_REPLACE_RENDERED_CLIP_FADES     0x0002


int protools_post_processing( AAF_Iface *aafi, uint32_t flags );


#endif // ! __ProTools_h__
