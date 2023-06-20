#ifndef __Resolve_h__
#define __Resolve_h__

#include <stdio.h>

#include <libaaf/AAFCore.h>
#include <libaaf/AAFIface.h>
#include <libaaf/AAFIParser.h>


int resolve_AAF( struct AAF_Iface *aafi );
int resolve_parse_aafObject_Selector( struct AAF_Iface *aafi, aafObject *Selector, td *__ptd );

#endif // !__Resolve_h__
