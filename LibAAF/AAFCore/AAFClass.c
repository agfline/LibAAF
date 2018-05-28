/*
 *	This file is part of LibAAF.
 *
 *	Copyright (c) 2017 Adrien Gesta-Fline
 *
 *	LibAAF is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Affero General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	any later version.
 *
 *	LibAAF is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Affero General Public License for more details.
 *
 *	You should have received a copy of the GNU Affero General Public License
 *	along with LibAAF. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *	@brief AAF core functions.
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "AAFTypes.h"
#include "AAFClass.h"
#include "AAFCore.h"
#include "AAFUtils.h"
#include "AAFToText.h"

#include "AAFDefs/AAFClassDefUIDs.h"
#include "AAFDefs/AAFTypeDefUIDs.h"
#include "AAFDefs/AAFPropertyIDs.h"

#include "../common/debug.h"





int classExists( AAF_Data *aafd, aafUID_t *ClassID )
{
	aafClass *Class = NULL;

	foreachClass( Class, aafd->Classes )
		if ( auidCmp( Class->ID, ClassID ) )
			break;

	if ( Class == NULL )
		return 0;

	return 1;
}




/**
 *	Allocates and initializes a new aafClass structure, adds it
 *	to the aafd->class  list and returns a pointer to the newly
 *	allocated Class.
 *
 *	@param  aafd       pointer to the AAF_Data structure.
 *	@param  id         pointer to the ClassID.
 *	@param  isConcrete boolean to set the Class as either an ABSTRACT or a CONCRETE Class.
 *	@param  parent     pointer to the parent Class.
 *	@return            pointer to the newly allocated aafClass.
 */

aafClass * defineNewClass( AAF_Data *aafd, const aafUID_t *id, uint8_t isConcrete, aafClass *parent )
{
	aafClass *Class = malloc( sizeof(aafClass) );

	if ( Class == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	Class->ID         = id;
	Class->Parent     = parent;
	Class->Properties = NULL;
	Class->isConcrete = isConcrete;

	Class->meta       = 0;
	Class->name       = NULL;

	Class->next       = aafd->Classes;
	aafd->Classes     = Class;

	return Class;
}




/**
 *	Retrieve an aafClass for a given ClassID.
 *
 *	@param  aafd pointer to the AAF_Data structure.
 *	@param  id   pointer to the ClassID to search for.
 *	@return      pointer to the retrieved aafClass structure, or NULL if not found.
 */

aafClass * getClassByID( AAF_Data *aafd, const aafUID_t *id )
{
	aafClass *Class = aafd->Classes;

	for ( Class = aafd->Classes; Class != NULL; Class = Class->next )
		if ( auidCmp( Class->ID, id ) )
			break;

//	if ( Class == NULL )
//		_fatal( "Could not retrieve Class ID (%s)\n", ClassIDToText( id ) );

	return Class;
}






aafPropertyDef * getPropertyDefinitionByID( aafClass *Classes, aafPID_t PID )
{
	aafClass       *Class = NULL;
	aafPropertyDef *PDef  = NULL;

	foreachClassInheritance( Class, Classes )
		foreachPropertyDefinition( PDef, Class->Properties )
			if ( PDef->pid == PID )
				return PDef;

	return NULL;
}


/*
void printClasses( aafClass *Class, int depth )
{

	aafPropertyDef *p = NULL;


	for ( ; Class != NULL; Class = Class->next )
	{
		if ( Class->isConcrete == ABSTRACT )
			continue;

		printf( "%*c::%s\n", depth, ' ', ClassIDToText( Class->id ) );


		p = Class->properties;

		while ( p != NULL )
		{
			printf( "%*c   +%s\n", depth, ' ', PIDToText( p->pid ) );

			p = p->next;
		}

		if ( Class->parent != NULL )
			printClasses( Class->parent, depth+6 );

		printf( "\n" );

	}

}
*/


/**
 *	Defines each Class with its properties according to
 *	the standard. All the Classes are then hold by the
 *	AAF_Data.Class list.
 *
 *	We define the Classes at runtime, so we can later
 *	add any custom class defined in the MetaDictionary.
 *	This is not yet implemented though.
 *
 *	@param aafd The AAF_Data struct pointer.
 */
void setDefaultClasses( AAF_Data *aafd )
{
	aafPropertyDef *prop = NULL;



	aafClass *IOC = defineNewClass( aafd, &AAFClassID_InterchangeObject, ABSTRACT, NULL );

	attachNewProperty( IOC,              prop, PID_InterchangeObject_ObjClass,                    REQ );
	attachNewProperty( IOC,              prop, PID_InterchangeObject_Generation,                  OPT );


	aafClass *Root = defineNewClass( aafd, &AAFClassID_Root, CONCRETE, IOC );

	attachNewProperty( Root,             prop, PID_Root_MetaDictionary,                           REQ );
	attachNewProperty( Root,             prop, PID_Root_Header,                                   REQ );


	aafClass *Header = defineNewClass( aafd, &AAFClassID_Header, CONCRETE, IOC );

	attachNewProperty( Header,           prop, PID_Header_ByteOrder,                              REQ );
	attachNewProperty( Header,           prop, PID_Header_LastModified,                           REQ );
	attachNewProperty( Header,           prop, PID_Header_Version,                                REQ );
	attachNewProperty( Header,           prop, PID_Header_Content,                                REQ );
	attachNewProperty( Header,           prop, PID_Header_Dictionary,                             REQ );
	attachNewProperty( Header,           prop, PID_Header_IdentificationList,                     REQ );
	attachNewProperty( Header,           prop, PID_Header_ObjectModelVersion,                     OPT );
	attachNewProperty( Header,           prop, PID_Header_OperationalPattern,                     OPT );
	attachNewProperty( Header,           prop, PID_Header_EssenceContainers,                      OPT );
	attachNewProperty( Header,           prop, PID_Header_DescriptiveSchemes,                     OPT );



	aafClass *Identif = defineNewClass( aafd, &AAFClassID_Identification, CONCRETE, IOC );

	attachNewProperty( Identif,          prop, PID_Identification_CompanyName,                    REQ );
	attachNewProperty( Identif,          prop, PID_Identification_ProductName,                    REQ );
	attachNewProperty( Identif,          prop, PID_Identification_ProductVersion,                 OPT );
	attachNewProperty( Identif,          prop, PID_Identification_ProductVersionString,           REQ );
	attachNewProperty( Identif,          prop, PID_Identification_ProductID,                      REQ );
	attachNewProperty( Identif,          prop, PID_Identification_Date,                           REQ );
	attachNewProperty( Identif,          prop, PID_Identification_ToolkitVersion,                 OPT );
	attachNewProperty( Identif,          prop, PID_Identification_Platform,                       OPT );
	attachNewProperty( Identif,          prop, PID_Identification_GenerationAUID,                 REQ );


	aafClass * Dictionary = defineNewClass( aafd, &AAFClassID_Dictionary, CONCRETE, IOC );

	attachNewProperty( Dictionary,       prop, PID_Dictionary_OperationDefinitions,               OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_ParameterDefinitions,               OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_DataDefinitions,                    OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_PluginDefinitions,                  OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_CodecDefinitions,                   OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_ContainerDefinitions,               OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_InterpolationDefinitions,           OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_KLVDataDefinitions,                 OPT );
	attachNewProperty( Dictionary,       prop, PID_Dictionary_TaggedValueDefinitions,             OPT );


	aafClass *Content = defineNewClass( aafd, &AAFClassID_ContentStorage, CONCRETE, IOC );

	attachNewProperty( Content,          prop, PID_ContentStorage_Mobs,                           REQ );
	attachNewProperty( Content,          prop, PID_ContentStorage_EssenceData,                    REQ );



	aafClass *Mob = defineNewClass( aafd, &AAFClassID_Mob, ABSTRACT, IOC );

	attachNewProperty( Mob,              prop, PID_Mob_MobID,                                     REQ );
	attachNewProperty( Mob,              prop, PID_Mob_Name,                                      OPT );
	attachNewProperty( Mob,              prop, PID_Mob_Slots,                                     REQ );
	attachNewProperty( Mob,              prop, PID_Mob_LastModified,                              REQ );
	attachNewProperty( Mob,              prop, PID_Mob_CreationTime,                              REQ );
	attachNewProperty( Mob,              prop, PID_Mob_UserComments,                              OPT );
	attachNewProperty( Mob,              prop, PID_Mob_Attributes,                                OPT );
	attachNewProperty( Mob,              prop, PID_Mob_KLVData,                                   OPT );
	attachNewProperty( Mob,              prop, PID_Mob_UsageCode,                                 OPT );



	aafClass *CompoMob = defineNewClass( aafd, &AAFClassID_CompositionMob, CONCRETE, Mob );

	attachNewProperty( CompoMob,         prop, PID_CompositionMob_DefaultFadeLength,              OPT );
	attachNewProperty( CompoMob,         prop, PID_CompositionMob_DefFadeType,                    OPT );
	attachNewProperty( CompoMob,         prop, PID_CompositionMob_DefFadeEditUnit,                OPT );
	attachNewProperty( CompoMob,         prop, PID_CompositionMob_Rendering,                      OPT );



	/*aafClass *MasterMob =*/ defineNewClass( aafd, &AAFClassID_MasterMob, CONCRETE, Mob );

	/* The MasterMob class does not define any additional properties. */



	aafClass *SourceMob = defineNewClass( aafd, &AAFClassID_SourceMob, CONCRETE, Mob );

	attachNewProperty( SourceMob,        prop, PID_SourceMob_EssenceDescription,                  REQ );



	aafClass *MobSlot = defineNewClass( aafd, &AAFClassID_MobSlot, ABSTRACT, IOC );

	attachNewProperty( MobSlot,          prop, PID_MobSlot_SlotID,                                REQ );
	attachNewProperty( MobSlot,          prop, PID_MobSlot_SlotName,                              OPT );
	attachNewProperty( MobSlot,          prop, PID_MobSlot_PhysicalTrackNumber,                   OPT );
	attachNewProperty( MobSlot,          prop, PID_MobSlot_Segment,                               REQ );



	aafClass *TimelineMobSlot = defineNewClass( aafd, &AAFClassID_TimelineMobSlot, CONCRETE, MobSlot );

	attachNewProperty( TimelineMobSlot,  prop, PID_TimelineMobSlot_EditRate,                      REQ );
	attachNewProperty( TimelineMobSlot,  prop, PID_TimelineMobSlot_Origin,                        REQ );
	attachNewProperty( TimelineMobSlot,  prop, PID_TimelineMobSlot_MarkIn,                        OPT );
	attachNewProperty( TimelineMobSlot,  prop, PID_TimelineMobSlot_MarkOut,                       OPT );
	attachNewProperty( TimelineMobSlot,  prop, PID_TimelineMobSlot_UserPos,                       OPT );



	aafClass *EventMobSlot = defineNewClass( aafd, &AAFClassID_EventMobSlot, CONCRETE, MobSlot );

	attachNewProperty( EventMobSlot,     prop, PID_EventMobSlot_EditRate,                         REQ );
//	attachNewProperty( EventMobSlot,     prop, PID_EventMobSlot_EventSlotOrigin,                  ??? );



	/*aafClass *StaticMobSlot =*/ defineNewClass( aafd, &AAFClassID_StaticMobSlot, CONCRETE, MobSlot );

	/* The StaticMobSlot class does not define any additional properties. */



	aafClass * KLVData = defineNewClass( aafd, &AAFClassID_KLVData, CONCRETE, IOC );

	attachNewProperty( KLVData,          prop, PID_KLVData_Value,                                 REQ );



	aafClass * TaggedValue = defineNewClass( aafd, &AAFClassID_TaggedValue, CONCRETE, IOC );

	attachNewProperty( TaggedValue,      prop, PID_TaggedValue_Name,                              REQ );
	attachNewProperty( TaggedValue,      prop, PID_TaggedValue_Value,                             REQ );



	aafClass * Parameter = defineNewClass( aafd, &AAFClassID_Parameter, ABSTRACT, IOC );

	attachNewProperty( Parameter,        prop, PID_Parameter_Definition,                          REQ );



	aafClass * ConstantValue = defineNewClass( aafd, &AAFClassID_ConstantValue, CONCRETE, Parameter );

	attachNewProperty( ConstantValue,    prop, PID_ConstantValue_Value,                           REQ );



	aafClass * VaryingValue = defineNewClass( aafd, &AAFClassID_VaryingValue, CONCRETE, Parameter );

	attachNewProperty( VaryingValue,     prop, PID_VaryingValue_Interpolation,                    REQ );
	attachNewProperty( VaryingValue,     prop, PID_VaryingValue_PointList,                        REQ );



	aafClass * ControlPoint = defineNewClass( aafd, &AAFClassID_ControlPoint, CONCRETE, IOC );

	attachNewProperty( ControlPoint,     prop, PID_ControlPoint_Value,                            REQ );
	attachNewProperty( ControlPoint,     prop, PID_ControlPoint_Time,                             REQ );
	attachNewProperty( ControlPoint,     prop, PID_ControlPoint_EditHint,                         OPT );



	aafClass * Locator = defineNewClass( aafd, &AAFClassID_Locator, ABSTRACT, IOC );

	/* The Locator class does not define any additional properties. */



	aafClass * NetworkLocator = defineNewClass( aafd, &AAFClassID_NetworkLocator, CONCRETE, Locator );

	attachNewProperty( NetworkLocator,   prop, PID_NetworkLocator_URLString,                      REQ );



	aafClass * TextLocator = defineNewClass( aafd, &AAFClassID_TextLocator, CONCRETE, Locator );

	attachNewProperty( TextLocator,      prop, PID_TextLocator_Name,                              REQ );


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
	aafClass * Component = defineNewClass( aafd, &AAFClassID_Component, ABSTRACT, IOC );

	attachNewProperty( Component,        prop, PID_Component_DataDefinition,                      REQ );
	attachNewProperty( Component,        prop, PID_Component_Length,                              OPT );
	attachNewProperty( Component,        prop, PID_Component_KLVData,                             OPT );
	attachNewProperty( Component,        prop, PID_Component_UserComments,                        OPT );
	attachNewProperty( Component,        prop, PID_Component_Attributes,                          OPT );



	aafClass * Transition = defineNewClass( aafd, &AAFClassID_Transition, CONCRETE, Component );

	attachNewProperty( Transition,       prop, PID_Transition_OperationGroup,                     REQ );
	attachNewProperty( Transition,       prop, PID_Transition_CutPoint,                           REQ );



	aafClass * Segment = defineNewClass( aafd, &AAFClassID_Segment, ABSTRACT, Component );

	/* The Segment class does not define any additional properties. */



	aafClass * Sequence = defineNewClass( aafd, &AAFClassID_Sequence, CONCRETE, Segment );

	attachNewProperty( Sequence,         prop, PID_Sequence_Components,                           REQ );



	/*aafClass * Filler =*/ defineNewClass( aafd, &AAFClassID_Filler, CONCRETE, Segment );

	/* The Filler class does not define any additional properties. */



	aafClass * SourceRef = defineNewClass( aafd, &AAFClassID_SourceReference, ABSTRACT, Segment );

	attachNewProperty( SourceRef,        prop, PID_SourceReference_SourceID,                      OPT );
	attachNewProperty( SourceRef,        prop, PID_SourceReference_SourceMobSlotID,               REQ );
	attachNewProperty( SourceRef,        prop, PID_SourceReference_ChannelIDs,                    OPT );
	attachNewProperty( SourceRef,        prop, PID_SourceReference_MonoSourceSlotIDs,             OPT );



	aafClass * SourceClip = defineNewClass( aafd, &AAFClassID_SourceClip, CONCRETE, SourceRef );

	attachNewProperty( SourceClip,       prop, PID_SourceClip_StartTime,                          OPT );
	attachNewProperty( SourceClip,       prop, PID_SourceClip_FadeInLength,                       OPT );
	attachNewProperty( SourceClip,       prop, PID_SourceClip_FadeInType,                         OPT );
	attachNewProperty( SourceClip,       prop, PID_SourceClip_FadeOutLength,                      OPT );
	attachNewProperty( SourceClip,       prop, PID_SourceClip_FadeOutType,                        OPT );



	aafClass * Event = defineNewClass( aafd, &AAFClassID_Event, ABSTRACT, Segment );

	attachNewProperty( Event,            prop, PID_Event_Position,                                REQ );
	attachNewProperty( Event,            prop, PID_Event_Comment,                                 OPT );



	aafClass * CommentMarker = defineNewClass( aafd, &AAFClassID_CommentMarker, CONCRETE, Event );

	attachNewProperty( CommentMarker,    prop, PID_CommentMarker_Annotation,                      OPT );



	aafClass * DescriptiveMarker = defineNewClass( aafd, &AAFClassID_DescriptiveMarker, CONCRETE, CommentMarker );

	attachNewProperty( DescriptiveMarker, prop, PID_DescriptiveMarker_DescribedSlots,             OPT );
	attachNewProperty( DescriptiveMarker, prop, PID_DescriptiveMarker_Description,                OPT );



	aafClass * GPITrigger = defineNewClass( aafd, &AAFClassID_GPITrigger, CONCRETE, Event );

	attachNewProperty( GPITrigger,       prop, PID_GPITrigger_ActiveState,                        REQ );



	aafClass * Timecode = defineNewClass( aafd, &AAFClassID_Timecode, CONCRETE, Segment );

	attachNewProperty( Timecode,         prop, PID_Timecode_Start,                                REQ );
	attachNewProperty( Timecode,         prop, PID_Timecode_FPS,                                  REQ );
	attachNewProperty( Timecode,         prop, PID_Timecode_Drop,                                 REQ );



	aafClass * TCStream = defineNewClass( aafd, &AAFClassID_TimecodeStream, ABSTRACT, Segment );

	attachNewProperty( TCStream,         prop, PID_TimecodeStream_SampleRate,                     REQ );
	attachNewProperty( TCStream,         prop, PID_TimecodeStream_Source,                         REQ );
	attachNewProperty( TCStream,         prop, PID_TimecodeStream_SourceType,                     REQ );



	aafClass * TCStream12M = defineNewClass( aafd, &AAFClassID_TimecodeStream12M, CONCRETE, TCStream );

	attachNewProperty( TCStream12M,      prop, PID_TimecodeStream12M_IncludeSync,                 OPT );



	aafClass * Edgecode = defineNewClass( aafd, &AAFClassID_Edgecode, CONCRETE, Segment );

	attachNewProperty( Edgecode,         prop, PID_EdgeCode_Start,                                REQ );
	attachNewProperty( Edgecode,         prop, PID_EdgeCode_FilmKind,                             REQ );
	attachNewProperty( Edgecode,         prop, PID_EdgeCode_CodeFormat,                           REQ );
	attachNewProperty( Edgecode,         prop, PID_EdgeCode_Header,                               OPT );



	aafClass * Pulldown = defineNewClass( aafd, &AAFClassID_Pulldown, CONCRETE, Segment );

	attachNewProperty( Pulldown,         prop, PID_Pulldown_InputSegment,                         REQ );
	attachNewProperty( Pulldown,         prop, PID_Pulldown_PulldownKind,                         REQ );
	attachNewProperty( Pulldown,         prop, PID_Pulldown_PulldownDirection,                    REQ );
	attachNewProperty( Pulldown,         prop, PID_Pulldown_PhaseFrame,                           REQ );



	aafClass * OperationGroup = defineNewClass( aafd, &AAFClassID_OperationGroup, CONCRETE, Segment );

	attachNewProperty( OperationGroup,   prop, PID_OperationGroup_Operation,                      REQ );
	attachNewProperty( OperationGroup,   prop, PID_OperationGroup_InputSegments,                  OPT );
	attachNewProperty( OperationGroup,   prop, PID_OperationGroup_Parameters,                     OPT );
	attachNewProperty( OperationGroup,   prop, PID_OperationGroup_Rendering,                      OPT );
	attachNewProperty( OperationGroup,   prop, PID_OperationGroup_BypassOverride,                 OPT );



	aafClass * NestedScope = defineNewClass( aafd, &AAFClassID_NestedScope, CONCRETE, Segment );

	attachNewProperty( NestedScope,      prop, PID_NestedScope_Slots,                             REQ );



	aafClass * ScopeReference = defineNewClass( aafd, &AAFClassID_ScopeReference, CONCRETE, Segment );

	attachNewProperty( ScopeReference,   prop, PID_ScopeReference_RelativeScope,                  REQ );
	attachNewProperty( ScopeReference,   prop, PID_ScopeReference_RelativeSlot,                   REQ );



	aafClass * Selector = defineNewClass( aafd, &AAFClassID_Selector, CONCRETE, Segment );

	attachNewProperty( Selector,         prop, PID_Selector_Selected,                             REQ );
	attachNewProperty( Selector,         prop, PID_Selector_Alternates,                           OPT );



	aafClass * EssenceGroup = defineNewClass( aafd, &AAFClassID_EssenceGroup, CONCRETE, Segment );

	attachNewProperty( EssenceGroup,     prop, PID_EssenceGroup_Choices,                          REQ );
	attachNewProperty( EssenceGroup,     prop, PID_EssenceGroup_StillFrame,                       OPT );














	/*aafClass * DescriptiveFramework =*/ defineNewClass( aafd, &AAFClassID_DescriptiveFramework, ABSTRACT, IOC );



	aafClass *EssenceDesc = defineNewClass( aafd, &AAFClassID_EssenceDescriptor, ABSTRACT, IOC );

	attachNewProperty( EssenceDesc,      prop, PID_EssenceDescriptor_Locator,                     OPT );



	aafClass *FileDesc = defineNewClass( aafd, &AAFClassID_FileDescriptor, ABSTRACT, EssenceDesc );

	attachNewProperty( FileDesc,         prop, PID_FileDescriptor_SampleRate,                     REQ );
	attachNewProperty( FileDesc,         prop, PID_FileDescriptor_Length,                         REQ );
	attachNewProperty( FileDesc,         prop, PID_FileDescriptor_ContainerFormat,                OPT );
	attachNewProperty( FileDesc,         prop, PID_FileDescriptor_CodecDefinition,                OPT );
//	attachNewProperty( FileDesc,         prop, PID_FileDescriptor_LinkedSlotID,                   ??? );



	aafClass *DigitalImageDesc = defineNewClass( aafd, &AAFClassID_DigitalImageDescriptor, ABSTRACT, FileDesc );

	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_Compression,            OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_StoredHeight,           REQ );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_StoredWidth,            REQ );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_StoredF2Offset,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledHeight,          OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledWidth,           OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledXOffset,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledYOffset,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayHeight,          OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayWidth,           OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayXOffset,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayYOffset,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayF2Offset,        OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_FrameLayout,            REQ );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_VideoLineMap,           REQ );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_ImageAspectRatio,       REQ );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_ActiveFormatDescriptor, OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_AlphaTransparency,      OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_ImageAlignmentFactor,   OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_FieldDominance,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_FieldStartOffset,       OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_FieldEndOffset,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_ColorPrimaries,         OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_CodingEquations,        OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_TransferCharacteristic, OPT );
	attachNewProperty( DigitalImageDesc, prop, PID_DigitalImageDescriptor_SignalStandard,         OPT );



	aafClass * CDCIDesc = defineNewClass( aafd, &AAFClassID_CDCIDescriptor, CONCRETE, DigitalImageDesc );

	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_HorizontalSubsampling,          REQ );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_VerticalSubsampling,            OPT );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_ComponentWidth,                 REQ );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_AlphaSamplingWidth,             OPT );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_PaddingBits,                    OPT );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_ColorSiting,                    OPT );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_BlackReferenceLevel,            OPT );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_WhiteReferenceLevel,            OPT );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_ColorRange,                     OPT );
	attachNewProperty( CDCIDesc,         prop, PID_CDCIDescriptor_ReversedByteOrder,              OPT );



	aafClass * RGBADesc = defineNewClass( aafd, &AAFClassID_RGBADescriptor, CONCRETE, DigitalImageDesc );

	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_PixelLayout,                    REQ );
	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_Palette,                        OPT );
	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_PaletteLayout,                  OPT );
	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_ComponentMinRef,                OPT );
	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_ComponentMaxRef,                OPT );
	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_AlphaMinRef,                    OPT );
	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_AlphaMaxRef,                    OPT );
	attachNewProperty( RGBADesc,         prop, PID_RGBADescriptor_ScanningDirection,              OPT );



	aafClass * TapeDesc = defineNewClass( aafd, &AAFClassID_TapeDescriptor, CONCRETE, EssenceDesc );

	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_FormFactor,                     OPT );
	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_VideoSignal,                    OPT );
	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_TapeFormat,                     OPT );
	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_Length,                         OPT );
	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_ManufacturerID,                 OPT );
	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_Model,                          OPT );
	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_TapeBatchNumber,                OPT );
	attachNewProperty( TapeDesc,         prop, PID_TapeDescriptor_TapeStock,                      OPT );



	aafClass * FilmDesc = defineNewClass( aafd, &AAFClassID_FilmDescriptor, CONCRETE, EssenceDesc );

	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_FilmFormat,                     OPT );
	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_FrameRate,                      OPT );
	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_PerforationsPerFrame,           OPT );
	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_FilmAspectRatio,                OPT );
	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_Manufacturer,                   OPT );
	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_Model,                          OPT );
	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_FilmGaugeFormat,                OPT );
	attachNewProperty( FilmDesc,         prop, PID_FilmDescriptor_FilmBatchNumber,                OPT );



	aafClass * WAVEDesc = defineNewClass( aafd, &AAFClassID_WAVEDescriptor, CONCRETE, FileDesc );

	attachNewProperty( WAVEDesc,         prop, PID_WAVEDescriptor_Summary,                        REQ );



	aafClass * AIFCDesc = defineNewClass( aafd, &AAFClassID_AIFCDescriptor, CONCRETE, FileDesc );

	attachNewProperty( AIFCDesc,         prop, PID_AIFCDescriptor_Summary,                        REQ );



	aafClass * TIFFDesc = defineNewClass( aafd, &AAFClassID_TIFFDescriptor, CONCRETE, FileDesc );

	attachNewProperty( TIFFDesc,         prop, PID_TIFFDescriptor_IsUniform,                      REQ );
	attachNewProperty( TIFFDesc,         prop, PID_TIFFDescriptor_IsContiguous,                   REQ );
	attachNewProperty( TIFFDesc,         prop, PID_TIFFDescriptor_LeadingLines,                   OPT );
	attachNewProperty( TIFFDesc,         prop, PID_TIFFDescriptor_TrailingLines,                  OPT );
	attachNewProperty( TIFFDesc,         prop, PID_TIFFDescriptor_JPEGTableID,                    OPT );
	attachNewProperty( TIFFDesc,         prop, PID_TIFFDescriptor_Summary,                        REQ );



	aafClass * SoundDesc = defineNewClass( aafd, &AAFClassID_SoundDescriptor, CONCRETE, FileDesc );

	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_AudioSamplingRate,             REQ );
	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_Locked,                        OPT );
	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_AudioRefLevel,                 OPT );
	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_ElectroSpatial,                OPT );
	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_Channels,                      REQ );
	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_QuantizationBits,              REQ );
	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_DialNorm,                      OPT );
	attachNewProperty( SoundDesc,        prop, PID_SoundDescriptor_Compression,                   OPT );



	aafClass * PCMDesc = defineNewClass( aafd, &AAFClassID_PCMDescriptor, CONCRETE, SoundDesc );

	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_BlockAlign,                      REQ );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_SequenceOffset,                  OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_AverageBPS,                      REQ );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_ChannelAssignment,               OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeVersion,             OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeFormat,              OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PointsPerPeakValue,              OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeBlockSize,           OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakChannels,                    OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakFrames,                      OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakOfPeaksPosition,             OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeTimestamp,           OPT );
	attachNewProperty( PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeData,                OPT );



	aafClass * PhysicalDesc = defineNewClass( aafd, &AAFClassID_PhysicalDescriptor, ABSTRACT, EssenceDesc );

	/* The PhysicalDescriptor class does not define any additional properties. */



	/*aafClass * ImportDesc =*/ defineNewClass( aafd, &AAFClassID_ImportDescriptor, CONCRETE, PhysicalDesc );

	/* The ImportDescriptor class does not define any additional properties. */



	/*aafClass * RecordingDesc =*/ defineNewClass( aafd, &AAFClassID_RecordingDescriptor, CONCRETE, PhysicalDesc );

	/* The RecordingDescriptor class does not define any additional properties. */



	aafClass * AuxiliaryDesc = defineNewClass( aafd, &AAFClassID_AuxiliaryDescriptor, CONCRETE, PhysicalDesc );

	attachNewProperty( AuxiliaryDesc,    prop, PID_AuxiliaryDescriptor_MimeType,                  REQ );
	attachNewProperty( AuxiliaryDesc,    prop, PID_AuxiliaryDescriptor_CharSet,                   OPT );















	aafClass * DefObject = defineNewClass( aafd, &AAFClassID_DefinitionObject, ABSTRACT, IOC );

	attachNewProperty( DefObject,        prop, PID_DefinitionObject_Identification,               REQ );
	attachNewProperty( DefObject,        prop, PID_DefinitionObject_Name,                         REQ );
	attachNewProperty( DefObject,        prop, PID_DefinitionObject_Description,                  OPT );



	/*aafClass * DataDef =*/ defineNewClass( aafd, &AAFClassID_DataDefinition, CONCRETE, DefObject );

	/* The DataDefinition class does not define any additional properties. */



	aafClass * ContainerDef = defineNewClass( aafd, &AAFClassID_ContainerDefinition, CONCRETE, DefObject );

	attachNewProperty( ContainerDef,     prop, PID_ContainerDefinition_EssenceIsIdentified,       OPT );



	aafClass * OperationDef = defineNewClass( aafd, &AAFClassID_OperationDefinition, CONCRETE, DefObject );

	attachNewProperty( OperationDef,     prop, PID_OperationDefinition_DataDefinition,            REQ );
	attachNewProperty( OperationDef,     prop, PID_OperationDefinition_IsTimeWarp,                OPT );
	attachNewProperty( OperationDef,     prop, PID_OperationDefinition_DegradeTo,                 OPT );
	attachNewProperty( OperationDef,     prop, PID_OperationDefinition_OperationCategory,         OPT );
	attachNewProperty( OperationDef,     prop, PID_OperationDefinition_NumberInputs,              REQ );
	attachNewProperty( OperationDef,     prop, PID_OperationDefinition_Bypass,                    OPT );
	attachNewProperty( OperationDef,     prop, PID_OperationDefinition_ParametersDefined,         OPT );



	aafClass * ParameterDef = defineNewClass( aafd, &AAFClassID_ParameterDefinition, CONCRETE, DefObject );

	attachNewProperty( ParameterDef,     prop, PID_ParameterDefinition_Type,                      REQ );
	attachNewProperty( ParameterDef,     prop, PID_ParameterDefinition_DisplayUnits,              OPT );



	/*aafClass * InterpolationDef =*/ defineNewClass( aafd, &AAFClassID_InterpolationDefinition, CONCRETE, DefObject );

	/* The InterpolationDefinition class does not define any additional properties. */



	aafClass * CodecDef = defineNewClass( aafd, &AAFClassID_CodecDefinition, CONCRETE, DefObject );

	attachNewProperty( CodecDef,         prop, PID_CodecDefinition_FileDescriptorClass,           REQ );
	attachNewProperty( CodecDef,         prop, PID_CodecDefinition_DataDefinitions,               REQ );



	aafClass * PluginDef = defineNewClass( aafd, &AAFClassID_PluginDefinition, CONCRETE, DefObject );

	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_PluginCategory,               REQ );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_VersionNumber,                REQ );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_VersionString,                OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_Manufacturer,                 OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_ManufacturerInfo,             OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_ManufacturerID,               OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_Platform,                     OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_MinPlatformVersion,           OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_MaxPlatformVersion,           OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_Engine,                       OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_MinEngineVersion,             OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_MaxEngineVersion,             OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_PluginAPI,                    OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_MinPluginAPI,                 OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_MaxPluginAPI,                 OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_SoftwareOnly,                 OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_Accelerator,                  OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_Locators,                     OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_Authentication,               OPT );
	attachNewProperty( PluginDef,        prop, PID_PluginDefinition_DefinitionObject,             OPT );


	/*aafClass * TaggedValueDef =*/ defineNewClass( aafd, &AAFClassID_TaggedValueDefinition, CONCRETE, DefObject );

	/* The TaggedValueDefinition class does not define any additional properties. */



	aafClass * KLVDataDef = defineNewClass( aafd, &AAFClassID_KLVDataDefinition, CONCRETE, DefObject );

	attachNewProperty( KLVDataDef,       prop, PID_KLVDataDefinition_KLVDataType,                 OPT );



	aafClass * EssenceData = defineNewClass( aafd, &AAFClassID_EssenceData, CONCRETE, IOC );

	attachNewProperty( EssenceData,      prop, PID_EssenceData_MobID,                             REQ );
	attachNewProperty( EssenceData,      prop, PID_EssenceData_Data,                              REQ );
	attachNewProperty( EssenceData,      prop, PID_EssenceData_SampleIndex,                       OPT );













	aafClass * MetaDefinition = defineNewClass( aafd, &AAFClassID_MetaDefinition, ABSTRACT, NULL );

	attachNewProperty( MetaDefinition,   prop, PID_MetaDefinition_Identification,                 REQ );
	attachNewProperty( MetaDefinition,   prop, PID_MetaDefinition_Name,                           REQ );
	attachNewProperty( MetaDefinition,   prop, PID_MetaDefinition_Description,                    OPT );



	aafClass * ClassDefinition = defineNewClass( aafd, &AAFClassID_ClassDefinition, CONCRETE, MetaDefinition );

	attachNewProperty( ClassDefinition,  prop, PID_ClassDefinition_ParentClass,                   REQ );
	attachNewProperty( ClassDefinition,  prop, PID_ClassDefinition_Properties,                    OPT );
	attachNewProperty( ClassDefinition,  prop, PID_ClassDefinition_IsConcrete,                    REQ );



	aafClass * PropertyDefinition = defineNewClass( aafd, &AAFClassID_PropertyDefinition, CONCRETE, MetaDefinition );

	attachNewProperty( PropertyDefinition, prop, PID_PropertyDefinition_Type,                     REQ );
	attachNewProperty( PropertyDefinition, prop, PID_PropertyDefinition_IsOptional,               REQ );
	attachNewProperty( PropertyDefinition, prop, PID_PropertyDefinition_LocalIdentification,      REQ );
	attachNewProperty( PropertyDefinition, prop, PID_PropertyDefinition_IsUniqueIdentifier,       OPT );



	aafClass * TypeDef = defineNewClass( aafd, &AAFClassID_TypeDefinition, ABSTRACT, MetaDefinition );

	/* The TypeDefinition class does not define any additional properties. */



	/*aafClass * TypeDefCharacter =*/ defineNewClass( aafd, &AAFClassID_TypeDefinitionCharacter, CONCRETE, TypeDef );

	/* The TypeDefinitionCharacter class does not define any additional properties. */



	aafClass * TypeDefEnum = defineNewClass( aafd, &AAFClassID_TypeDefinitionEnumeration, CONCRETE, TypeDef );

	attachNewProperty( TypeDefEnum,      prop, PID_TypeDefinitionEnumeration_ElementType,         REQ );
	attachNewProperty( TypeDefEnum,      prop, PID_TypeDefinitionEnumeration_ElementNames,        REQ );
	attachNewProperty( TypeDefEnum,      prop, PID_TypeDefinitionEnumeration_ElementValues,       REQ );



	aafClass * TypeDefExtEnum = defineNewClass( aafd, &AAFClassID_TypeDefinitionExtendibleEnumeration, CONCRETE, TypeDef );

	attachNewProperty( TypeDefExtEnum,   prop, PID_TypeDefinitionExtendibleEnumeration_ElementNames,  REQ );
	attachNewProperty( TypeDefExtEnum,   prop, PID_TypeDefinitionExtendibleEnumeration_ElementValues, REQ );



	aafClass * TypeDefFixedArray = defineNewClass( aafd, &AAFClassID_TypeDefinitionFixedArray, CONCRETE, TypeDef );

	attachNewProperty( TypeDefFixedArray, prop, PID_TypeDefinitionFixedArray_ElementType,         REQ );
	attachNewProperty( TypeDefFixedArray, prop, PID_TypeDefinitionFixedArray_ElementCount,        REQ );



	/*aafClass * TypeDefIndirect =*/ defineNewClass( aafd, &AAFClassID_TypeDefinitionIndirect, CONCRETE, TypeDef );

	/* The TypeDefinitionIndirect class does not define any additional properties. */



	aafClass * TypeDefInt = defineNewClass( aafd, &AAFClassID_TypeDefinitionInteger, CONCRETE, TypeDef );

	attachNewProperty( TypeDefInt,       prop, PID_TypeDefinitionInteger_Size,                    REQ );
	attachNewProperty( TypeDefInt,       prop, PID_TypeDefinitionInteger_IsSigned,                REQ );



	/*aafClass * TypeDefOpaque =*/ defineNewClass( aafd, &AAFClassID_TypeDefinitionOpaque, CONCRETE, TypeDef );

	/* The TypeDefinitionOpaque class does not define any additional properties. */



	aafClass * TypeDefRecord = defineNewClass( aafd, &AAFClassID_TypeDefinitionRecord, CONCRETE, TypeDef );

	attachNewProperty( TypeDefRecord,    prop, PID_TypeDefinitionRecord_MemberTypes,              REQ );
	attachNewProperty( TypeDefRecord,    prop, PID_TypeDefinitionRecord_MemberNames,              REQ );



	aafClass * TypeDefRename = defineNewClass( aafd, &AAFClassID_TypeDefinitionRename, CONCRETE, TypeDef );

	attachNewProperty( TypeDefRename,    prop, PID_TypeDefinitionRename_RenamedType,              REQ );



	aafClass * TypeDefSet = defineNewClass( aafd, &AAFClassID_TypeDefinitionSet, CONCRETE, TypeDef );

	attachNewProperty( TypeDefSet,       prop, PID_TypeDefinitionSet_ElementType,                 REQ );



	/*aafClass * TypeDefStream =*/ defineNewClass( aafd, &AAFClassID_TypeDefinitionStream, CONCRETE, TypeDef );

	/* The TypeDefinitionStream class does not define any additional properties. */



	aafClass * TypeDefString = defineNewClass( aafd, &AAFClassID_TypeDefinitionString, CONCRETE, TypeDef );

	attachNewProperty( TypeDefString,    prop, PID_TypeDefinitionString_ElementType,              REQ );



	aafClass * TypeDefStrongObjRef = defineNewClass( aafd, &AAFClassID_TypeDefinitionStrongObjectReference, CONCRETE, TypeDef );

	attachNewProperty( TypeDefStrongObjRef, prop, PID_TypeDefinitionStrongObjectReference_ReferencedType, REQ );



	aafClass * TypeDefVariableArray = defineNewClass( aafd, &AAFClassID_TypeDefinitionVariableArray, CONCRETE, TypeDef );

	attachNewProperty( TypeDefVariableArray, prop, PID_TypeDefinitionVariableArray_ElementType,   REQ );



	aafClass * TypeDefWeakObjRef = defineNewClass( aafd, &AAFClassID_TypeDefinitionWeakObjectReference, CONCRETE, TypeDef );

	attachNewProperty( TypeDefWeakObjRef, prop, PID_TypeDefinitionWeakObjectReference_ReferencedType, REQ );
	attachNewProperty( TypeDefWeakObjRef, prop, PID_TypeDefinitionWeakObjectReference_TargetSet,      REQ );



	aafClass * MetaDictionary = defineNewClass( aafd, &AAFClassID_MetaDictionary, CONCRETE, NULL );

	attachNewProperty( MetaDictionary,   prop, PID_MetaDictionary_ClassDefinitions,               OPT );
	attachNewProperty( MetaDictionary,   prop, PID_MetaDictionary_TypeDefinitions,                OPT );

}

