#ifndef __Resolve_h__
#define __Resolve_h__

typedef struct AAF_Iface AAF_Iface;

enum resolve_options {
  RESOLVE_INCLUDE_DISABLED_CLIPS = 1<<0,
};

#define RESOLVE_ALL (RESOLVE_INCLUDE_DISABLED_CLIPS)


int resolve_AAF( struct AAF_Iface *aafi );
int resolve_parse_aafObject_Selector( struct AAF_Iface *aafi, aafObject *Selector, td *__ptd );
int resolve_parse_aafObject_DescriptiveMarker( struct AAF_Iface *aafi, aafObject *DescriptiveMarker, td *__ptd );

#endif // !__Resolve_h__
