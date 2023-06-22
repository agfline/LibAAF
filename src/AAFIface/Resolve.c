#include <stdio.h>
#include <stdint.h>

#include <libaaf/AAFIParser.h>
#include <libaaf/AAFDefs/AAFPropertyIDs.h>

#include <libaaf.h>


int resolve_AAF( struct AAF_Iface *aafi )
{
  int probe = 0;

  if ( aafi->aafd->Identification.CompanyName && wcsncmp( aafi->aafd->Identification.CompanyName, L"Blackmagic Design", wcslen( L"Blackmagic Design" ) ) == 0 ) {
    probe++;
  }

  if ( aafi->aafd->Identification.ProductName && wcsncmp( aafi->aafd->Identification.ProductName, L"DaVinci Resolve", wcslen( L"DaVinci Resolve" ) ) == 0 ) {
    probe++;
  }

  if ( probe == 2 ) {
    return 1;
  }

  return 0;
}



int resolve_parse_aafObject_Selector( struct AAF_Iface *aafi, aafObject *Selector, td *__ptd )
{
  /*
   *  Resolve 18.5
   *
   *  The Selector Object was only seen used to describe a disabled clip :
   *    - Selected property Object is an empty Filler
   *    - Alternate keeps track of the original clip.
   *
03411││              ├──◻ AAFClassID_Selector
01926││ Selected --> │    └──◻ AAFClassID_Filler
02359││ Alternate -> │    └──◻ AAFClassID_OperationGroup (OpIdent: AAFOperationDef_MonoAudioGain)
03822││              │         ├──◻ AAFClassID_VaryingValue
02877││              │         └──◻ AAFClassID_SourceClip
02882││              │              └──◻ AAFClassID_MasterMob (UsageCode: n/a) : speech-sample.mp3 - disabled
04460││              │                   └──◻ AAFClassID_TimelineMobSlot
03089││              │                        └──◻ AAFClassID_SourceClip
04167││              │                             └──◻ AAFClassID_SourceMob (UsageCode: n/a) : speech-sample.mp3 - disabled
01249││              │                                  └──◻ AAFClassID_PCMDescriptor
01455││              │                                       └──◻ AAFClassID_NetworkLocator : file:///C:/Users/user/Desktop/libAAF/test/res/speech-sample.mp3
   */

  struct trace_dump __td;
  // struct trace_dump __td2;

  __td_set(__td, __ptd, 0);
  // __td_set(__td2, (&__td), 1);



  aafObject *Selected = aaf_get_propertyValue( Selector, PID_Selector_Selected );

	if ( Selected == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, Selector, &__td, "Missing PID_Selector_Selected" );
		return -1;
	}

	aafObject *Alternates = aaf_get_propertyValue( Selector, PID_Selector_Alternates );

	if ( Alternates == NULL ) /* opt */
	{
		DUMP_OBJ_WARNING( aafi, Selector, &__td, "Missing PID_Selector_Alternates" );
		return -1;
	}



  void *ComponentAttributeList = aaf_get_propertyValue( Selector, 0xfffc /* ComponentAttributeList */ );

	if ( ComponentAttributeList == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Selector, &__td, "Missing AAFClassID_Selector::ComponentAttributeList" );
		return -1;
	}


  DUMP_OBJ( aafi, Selector, &__td );
  // DUMP_OBJ( aafi, Selected, &__td2 );



	// aaf_dump_ObjectProperties( aafi->aafd, Selector );
	// aaf_dump_ObjectProperties( aafi->aafd, ComponentAttributeList );


  int ismuted = 0;
	aafObject *ComponentAttribute = NULL;

	aaf_foreach_ObjectInSet( &ComponentAttribute, ComponentAttributeList, NULL )
	{
		/* TODO implement retrieve_TaggedValue() */

		wchar_t *name = aaf_get_propertyValueWstr( ComponentAttribute, PID_TaggedValue_Name );

		if ( name == NULL ) /* req */
		{
			DUMP_OBJ_ERROR( aafi, ComponentAttribute, &__td, "Missing PID_TaggedValue_Name" );
			continue;
		}


		void *value = aaf_get_propertyIndirectValue( ComponentAttribute, PID_TaggedValue_Value );

		if ( value == NULL ) /* req */
		{
			DUMP_OBJ_ERROR( aafi, ComponentAttribute, &__td, "Missing PID_TaggedValue_Value" );
      free( name );
			continue;
		}


		// printf( "Tagged | Name: %ls    Value : %u\n", name, *value );

    if ( aafi->ctx.options.resolve & RESOLVE_INCLUDE_DISABLED_CLIPS ) {
      if ( wcsncmp( name, L"_DISABLE_CLIP_FLAG", wcslen( L"_DISABLE_CLIP_FLAG" ) ) == 0  &&  *(uint32_t*)value == 1 ) {

        ismuted = 1;
        aafi->ctx.current_clip_is_muted = 1;

        aafObject *Alternate = NULL;

        int i = 0;
        aaf_foreach_ObjectInSet( &Alternate, Alternates, NULL ) {
          if ( i == 0 ) { /* there should be only one Segment in set, but still. Let's be carefull */
            parse_Segment( aafi, Alternate, &__td );
          }
          else {
            DUMP_OBJ_ERROR( aafi, Alternate, &__td, "Multiple Alternates in Davinci Resolve selector" );
          }
          i++;
        }
      }
    }

    free( name );
	}

  /* aafi->ctx.current_clip_is_muted was already reset at this point */
  if ( ismuted == 0 ) {
    return parse_Segment( aafi, Selected, &__td );
  }

  return 0;
}



int resolve_parse_aafObject_DescriptiveMarker( struct AAF_Iface *aafi, aafObject *DescriptiveMarker, td *__ptd )
{
  /*
   *  Resolve 18.5
   */

  struct trace_dump __td;
  __td_set(__td, __ptd, 0);


  aafPosition_t *start = (int64_t *)aaf_get_propertyValue( DescriptiveMarker, PID_Event_Position );

  if ( start == NULL ) /* req (TODO: conditional) */
  {
    DUMP_OBJ_ERROR( aafi, DescriptiveMarker, &__td, "Missing PID_Event_Position" );
    return -1;
  }


  aafPosition_t *length = (int64_t *)aaf_get_propertyValue( DescriptiveMarker, PID_Component_Length );

  if ( length == NULL ) /* opt */
  {
    // DUMP_OBJ_ERROR( aafi, Selector, &__td, "Missing PID_Event_Position" );
    // return -1;
  }


  wchar_t *comment = aaf_get_propertyValueWstr( DescriptiveMarker, PID_Event_Comment );

  wchar_t *name = aaf_get_propertyValueWstr( DescriptiveMarker, 0xfffa /* CommentMarkerUser */ );

  uint16_t *RVBColor = (uint16_t*)aaf_get_propertyValue( DescriptiveMarker, 0xfff9 /* CommentMarkerColor */ );


  if ( RVBColor ) {
    /* big endian to little endian */

    RVBColor[0] = (RVBColor[0]>>8) | (RVBColor[0]<<8);
    RVBColor[1] = (RVBColor[1]>>8) | (RVBColor[1]<<8);
    RVBColor[2] = (RVBColor[2]>>8) | (RVBColor[2]<<8);

    // RVBColor[0] = (RVBColor[0]>>8 != 0) ? (RVBColor[0]>>8) : RVBColor[0];
    // RVBColor[1] = (RVBColor[1]>>8 != 0) ? (RVBColor[1]>>8) : RVBColor[1];
    // RVBColor[2] = (RVBColor[2]>>8 != 0) ? (RVBColor[2]>>8) : RVBColor[2];

    // printf("%02x\n", RVBColor[0] );
    // printf("%02x\n", RVBColor[1] );
    // printf("%02x\n", RVBColor[2] );

    // DUMP_OBJ_NO_SUPPORT( aafi, DescriptiveMarker, &__td );
  }

  aafi_newMarker( aafi, aafi->ctx.current_markers_edit_rate, *start, ((length) ? *length : 0), name, comment, &RVBColor );

  return 0;
}
