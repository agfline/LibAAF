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




#define attachNewProperty( aafd, Class, Prop, Pid, IsReq ) \
	Prop = malloc( sizeof(aafPropertyDef) );         \
	if ( Prop == NULL )                              \
	{                                                \
		_error( aafd->verb, "%s.\n", strerror( errno ) );        \
		return -1;                                   \
	}                                                \
	Prop->pid         = Pid;                         \
	Prop->isReq       = IsReq;                       \
	Prop->meta        = 0;                           \
	Prop->next        = Class->Properties;           \
	Class->Properties = Prop;                        \




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
	{
		_error( aafd->verb, "%s.\n", strerror( errno ) );
		return NULL;
	}

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
	aafClass *Class = NULL;

	for ( Class = aafd->Classes; Class != NULL; Class = Class->next )
		if ( auidCmp( Class->ID, id ) )
			break;

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
int setDefaultClasses( AAF_Data *aafd )
{
	aafPropertyDef *prop = NULL;

	/* TODO test ENOMEM after each class alloc */


	aafClass *IOC = defineNewClass( aafd, &AAFClassID_InterchangeObject, ABSTRACT, NULL );

	if ( IOC == NULL ) { return -1; }

	attachNewProperty( aafd, IOC,              prop, PID_InterchangeObject_ObjClass,                            REQ );
	attachNewProperty( aafd, IOC,              prop, PID_InterchangeObject_Generation,                          OPT );


	aafClass *Root = defineNewClass( aafd, &AAFClassID_Root, CONCRETE, IOC );

	if ( Root == NULL ) { return -1; }

	attachNewProperty( aafd, Root,             prop, PID_Root_MetaDictionary,                                   REQ );
	attachNewProperty( aafd, Root,             prop, PID_Root_Header,                                           REQ );


	aafClass *Header = defineNewClass( aafd, &AAFClassID_Header, CONCRETE, IOC );

	if ( Header == NULL ) { return -1; }

	attachNewProperty( aafd, Header,           prop, PID_Header_ByteOrder,                                      REQ );
	attachNewProperty( aafd, Header,           prop, PID_Header_LastModified,                                   REQ );
	attachNewProperty( aafd, Header,           prop, PID_Header_Version,                                        REQ );
	attachNewProperty( aafd, Header,           prop, PID_Header_Content,                                        REQ );
	attachNewProperty( aafd, Header,           prop, PID_Header_Dictionary,                                     REQ );
	attachNewProperty( aafd, Header,           prop, PID_Header_IdentificationList,                             REQ );
	attachNewProperty( aafd, Header,           prop, PID_Header_ObjectModelVersion,                             OPT );
	attachNewProperty( aafd, Header,           prop, PID_Header_OperationalPattern,                             OPT );
	attachNewProperty( aafd, Header,           prop, PID_Header_EssenceContainers,                              OPT );
	attachNewProperty( aafd, Header,           prop, PID_Header_DescriptiveSchemes,                             OPT );



	aafClass *Identif = defineNewClass( aafd, &AAFClassID_Identification, CONCRETE, IOC );

	if ( Identif == NULL ) { return -1; }

	attachNewProperty( aafd, Identif,          prop, PID_Identification_CompanyName,                            REQ );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_ProductName,                            REQ );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_ProductVersion,                         OPT );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_ProductVersionString,                   REQ );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_ProductID,                              REQ );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_Date,                                   REQ );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_ToolkitVersion,                         OPT );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_Platform,                               OPT );
	attachNewProperty( aafd, Identif,          prop, PID_Identification_GenerationAUID,                         REQ );


	aafClass * Dictionary = defineNewClass( aafd, &AAFClassID_Dictionary, CONCRETE, IOC );

	if ( Dictionary == NULL ) { return -1; }

	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_OperationDefinitions,                       OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_ParameterDefinitions,                       OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_DataDefinitions,                            OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_PluginDefinitions,                          OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_CodecDefinitions,                           OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_ContainerDefinitions,                       OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_InterpolationDefinitions,                   OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_KLVDataDefinitions,                         OPT );
	attachNewProperty( aafd, Dictionary,       prop, PID_Dictionary_TaggedValueDefinitions,                     OPT );


	aafClass *Content = defineNewClass( aafd, &AAFClassID_ContentStorage, CONCRETE, IOC );

	if ( Content == NULL ) { return -1; }

	attachNewProperty( aafd, Content,          prop, PID_ContentStorage_Mobs,                                   REQ );
	attachNewProperty( aafd, Content,          prop, PID_ContentStorage_EssenceData,                            REQ );



	aafClass *Mob = defineNewClass( aafd, &AAFClassID_Mob, ABSTRACT, IOC );

	if ( Mob == NULL ) { return -1; }

	attachNewProperty( aafd, Mob,              prop, PID_Mob_MobID,                                             REQ );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_Name,                                              OPT );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_Slots,                                             REQ );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_LastModified,                                      REQ );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_CreationTime,                                      REQ );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_UserComments,                                      OPT );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_Attributes,                                        OPT );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_KLVData,                                           OPT );
	attachNewProperty( aafd, Mob,              prop, PID_Mob_UsageCode,                                         OPT );



	aafClass *CompoMob = defineNewClass( aafd, &AAFClassID_CompositionMob, CONCRETE, Mob );

	if ( CompoMob == NULL ) { return -1; }

	attachNewProperty( aafd, CompoMob,         prop, PID_CompositionMob_DefaultFadeLength,                      OPT );
	attachNewProperty( aafd, CompoMob,         prop, PID_CompositionMob_DefFadeType,                            OPT );
	attachNewProperty( aafd, CompoMob,         prop, PID_CompositionMob_DefFadeEditUnit,                        OPT );
	attachNewProperty( aafd, CompoMob,         prop, PID_CompositionMob_Rendering,                              OPT );



	aafClass *MasterMob = defineNewClass( aafd, &AAFClassID_MasterMob, CONCRETE, Mob );

	if ( MasterMob == NULL ) { return -1; }

	/* The MasterMob class does not define any additional properties. */



	aafClass *SourceMob = defineNewClass( aafd, &AAFClassID_SourceMob, CONCRETE, Mob );

	if ( SourceMob == NULL ) { return -1; }

	attachNewProperty( aafd, SourceMob,        prop, PID_SourceMob_EssenceDescription,                          REQ );



	aafClass *MobSlot = defineNewClass( aafd, &AAFClassID_MobSlot, ABSTRACT, IOC );

	if ( MobSlot == NULL ) { return -1; }

	attachNewProperty( aafd, MobSlot,          prop, PID_MobSlot_SlotID,                                        REQ );
	attachNewProperty( aafd, MobSlot,          prop, PID_MobSlot_SlotName,                                      OPT );
	attachNewProperty( aafd, MobSlot,          prop, PID_MobSlot_PhysicalTrackNumber,                           OPT );
	attachNewProperty( aafd, MobSlot,          prop, PID_MobSlot_Segment,                                       REQ );



	aafClass *TimelineMobSlot = defineNewClass( aafd, &AAFClassID_TimelineMobSlot, CONCRETE, MobSlot );

	if ( TimelineMobSlot == NULL ) { return -1; }

	attachNewProperty( aafd, TimelineMobSlot,  prop, PID_TimelineMobSlot_EditRate,                              REQ );
	attachNewProperty( aafd, TimelineMobSlot,  prop, PID_TimelineMobSlot_Origin,                                REQ );
	attachNewProperty( aafd, TimelineMobSlot,  prop, PID_TimelineMobSlot_MarkIn,                                OPT );
	attachNewProperty( aafd, TimelineMobSlot,  prop, PID_TimelineMobSlot_MarkOut,                               OPT );
	attachNewProperty( aafd, TimelineMobSlot,  prop, PID_TimelineMobSlot_UserPos,                               OPT );



	aafClass *EventMobSlot = defineNewClass( aafd, &AAFClassID_EventMobSlot, CONCRETE, MobSlot );

	if ( EventMobSlot == NULL ) { return -1; }

	attachNewProperty( aafd, EventMobSlot,     prop, PID_EventMobSlot_EditRate,                                 REQ );
//	attachNewProperty( aafd, EventMobSlot,     prop, PID_EventMobSlot_EventSlotOrigin,                  ??? );



	aafClass *StaticMobSlot = defineNewClass( aafd, &AAFClassID_StaticMobSlot, CONCRETE, MobSlot );

	if ( StaticMobSlot == NULL ) { return -1; }

	/* The StaticMobSlot class does not define any additional properties. */



	aafClass * KLVData = defineNewClass( aafd, &AAFClassID_KLVData, CONCRETE, IOC );

	if ( KLVData == NULL ) { return -1; }

	attachNewProperty( aafd, KLVData,          prop, PID_KLVData_Value,                                         REQ );



	aafClass * TaggedValue = defineNewClass( aafd, &AAFClassID_TaggedValue, CONCRETE, IOC );

	if ( TaggedValue == NULL ) { return -1; }

	attachNewProperty( aafd, TaggedValue,      prop, PID_TaggedValue_Name,                                      REQ );
	attachNewProperty( aafd, TaggedValue,      prop, PID_TaggedValue_Value,                                     REQ );



	aafClass * Parameter = defineNewClass( aafd, &AAFClassID_Parameter, ABSTRACT, IOC );

	if ( Parameter == NULL ) { return -1; }

	attachNewProperty( aafd, Parameter,        prop, PID_Parameter_Definition,                                  REQ );



	aafClass * ConstantValue = defineNewClass( aafd, &AAFClassID_ConstantValue, CONCRETE, Parameter );

	if ( ConstantValue == NULL ) { return -1; }

	attachNewProperty( aafd, ConstantValue,    prop, PID_ConstantValue_Value,                                   REQ );



	aafClass * VaryingValue = defineNewClass( aafd, &AAFClassID_VaryingValue, CONCRETE, Parameter );

	if ( VaryingValue == NULL ) { return -1; }

	attachNewProperty( aafd, VaryingValue,     prop, PID_VaryingValue_Interpolation,                            REQ );
	attachNewProperty( aafd, VaryingValue,     prop, PID_VaryingValue_PointList,                                REQ );



	aafClass * ControlPoint = defineNewClass( aafd, &AAFClassID_ControlPoint, CONCRETE, IOC );

	if ( ControlPoint == NULL ) { return -1; }

	attachNewProperty( aafd, ControlPoint,     prop, PID_ControlPoint_Value,                                    REQ );
	attachNewProperty( aafd, ControlPoint,     prop, PID_ControlPoint_Time,                                     REQ );
	attachNewProperty( aafd, ControlPoint,     prop, PID_ControlPoint_EditHint,                                 OPT );



	aafClass * Locator = defineNewClass( aafd, &AAFClassID_Locator, ABSTRACT, IOC );

	if ( Locator == NULL ) { return -1; }

	/* The Locator class does not define any additional properties. */



	aafClass * NetworkLocator = defineNewClass( aafd, &AAFClassID_NetworkLocator, CONCRETE, Locator );

	if ( NetworkLocator == NULL ) { return -1; }

	attachNewProperty( aafd, NetworkLocator,   prop, PID_NetworkLocator_URLString,                              REQ );



	aafClass * TextLocator = defineNewClass( aafd, &AAFClassID_TextLocator, CONCRETE, Locator );

	if ( TextLocator == NULL ) { return -1; }

	attachNewProperty( aafd, TextLocator,      prop, PID_TextLocator_Name,                                      REQ );

















	aafClass * Component = defineNewClass( aafd, &AAFClassID_Component, ABSTRACT, IOC );

	if ( Component == NULL ) { return -1; }

	attachNewProperty( aafd, Component,        prop, PID_Component_DataDefinition,                              REQ );
	attachNewProperty( aafd, Component,        prop, PID_Component_Length,                                      OPT );
	attachNewProperty( aafd, Component,        prop, PID_Component_KLVData,                                     OPT );
	attachNewProperty( aafd, Component,        prop, PID_Component_UserComments,                                OPT );
	attachNewProperty( aafd, Component,        prop, PID_Component_Attributes,                                  OPT );



	aafClass * Transition = defineNewClass( aafd, &AAFClassID_Transition, CONCRETE, Component );

	if ( Transition == NULL ) { return -1; }

	attachNewProperty( aafd, Transition,       prop, PID_Transition_OperationGroup,                             REQ );
	attachNewProperty( aafd, Transition,       prop, PID_Transition_CutPoint,                                   REQ );



	aafClass * Segment = defineNewClass( aafd, &AAFClassID_Segment, ABSTRACT, Component );

	if ( Segment == NULL ) { return -1; }

	/* The Segment class does not define any additional properties. */



	aafClass * Sequence = defineNewClass( aafd, &AAFClassID_Sequence, CONCRETE, Segment );

	if ( Sequence == NULL ) { return -1; }

	attachNewProperty( aafd, Sequence,         prop, PID_Sequence_Components,                                   REQ );



	aafClass * Filler = defineNewClass( aafd, &AAFClassID_Filler, CONCRETE, Segment );

	if ( Filler == NULL ) { return -1; }

	/* The Filler class does not define any additional properties. */



	aafClass * SourceRef = defineNewClass( aafd, &AAFClassID_SourceReference, ABSTRACT, Segment );

	if ( SourceRef == NULL ) { return -1; }

	attachNewProperty( aafd, SourceRef,        prop, PID_SourceReference_SourceID,                              OPT );
	attachNewProperty( aafd, SourceRef,        prop, PID_SourceReference_SourceMobSlotID,                       REQ );
	attachNewProperty( aafd, SourceRef,        prop, PID_SourceReference_ChannelIDs,                            OPT );
	attachNewProperty( aafd, SourceRef,        prop, PID_SourceReference_MonoSourceSlotIDs,                     OPT );



	aafClass * SourceClip = defineNewClass( aafd, &AAFClassID_SourceClip, CONCRETE, SourceRef );

	if ( SourceClip == NULL ) { return -1; }

	attachNewProperty( aafd, SourceClip,       prop, PID_SourceClip_StartTime,                                  OPT );
	attachNewProperty( aafd, SourceClip,       prop, PID_SourceClip_FadeInLength,                               OPT );
	attachNewProperty( aafd, SourceClip,       prop, PID_SourceClip_FadeInType,                                 OPT );
	attachNewProperty( aafd, SourceClip,       prop, PID_SourceClip_FadeOutLength,                              OPT );
	attachNewProperty( aafd, SourceClip,       prop, PID_SourceClip_FadeOutType,                                OPT );



	aafClass * Event = defineNewClass( aafd, &AAFClassID_Event, ABSTRACT, Segment );

	if ( Event == NULL ) { return -1; }

	attachNewProperty( aafd, Event,            prop, PID_Event_Position,                                        REQ );
	attachNewProperty( aafd, Event,            prop, PID_Event_Comment,                                         OPT );



	aafClass * CommentMarker = defineNewClass( aafd, &AAFClassID_CommentMarker, CONCRETE, Event );

	if ( CommentMarker == NULL ) { return -1; }

	attachNewProperty( aafd, CommentMarker,    prop, PID_CommentMarker_Annotation,                              OPT );



	aafClass * DescriptiveMarker = defineNewClass( aafd, &AAFClassID_DescriptiveMarker, CONCRETE, CommentMarker );

	if ( DescriptiveMarker == NULL ) { return -1; }

	attachNewProperty( aafd, DescriptiveMarker, prop, PID_DescriptiveMarker_DescribedSlots,                     OPT );
	attachNewProperty( aafd, DescriptiveMarker, prop, PID_DescriptiveMarker_Description,                        OPT );



	aafClass * GPITrigger = defineNewClass( aafd, &AAFClassID_GPITrigger, CONCRETE, Event );

	if ( GPITrigger == NULL ) { return -1; }

	attachNewProperty( aafd, GPITrigger,       prop, PID_GPITrigger_ActiveState,                                REQ );



	aafClass * Timecode = defineNewClass( aafd, &AAFClassID_Timecode, CONCRETE, Segment );

	if ( Timecode == NULL ) { return -1; }

	attachNewProperty( aafd, Timecode,         prop, PID_Timecode_Start,                                        REQ );
	attachNewProperty( aafd, Timecode,         prop, PID_Timecode_FPS,                                          REQ );
	attachNewProperty( aafd, Timecode,         prop, PID_Timecode_Drop,                                         REQ );



	aafClass * TCStream = defineNewClass( aafd, &AAFClassID_TimecodeStream, ABSTRACT, Segment );

	if ( TCStream == NULL ) { return -1; }

	attachNewProperty( aafd, TCStream,         prop, PID_TimecodeStream_SampleRate,                             REQ );
	attachNewProperty( aafd, TCStream,         prop, PID_TimecodeStream_Source,                                 REQ );
	attachNewProperty( aafd, TCStream,         prop, PID_TimecodeStream_SourceType,                             REQ );



	aafClass * TCStream12M = defineNewClass( aafd, &AAFClassID_TimecodeStream12M, CONCRETE, TCStream );

	if ( TCStream12M == NULL ) { return -1; }

	attachNewProperty( aafd, TCStream12M,      prop, PID_TimecodeStream12M_IncludeSync,                         OPT );



	aafClass * Edgecode = defineNewClass( aafd, &AAFClassID_Edgecode, CONCRETE, Segment );

	if ( Edgecode == NULL ) { return -1; }

	attachNewProperty( aafd, Edgecode,         prop, PID_EdgeCode_Start,                                        REQ );
	attachNewProperty( aafd, Edgecode,         prop, PID_EdgeCode_FilmKind,                                     REQ );
	attachNewProperty( aafd, Edgecode,         prop, PID_EdgeCode_CodeFormat,                                   REQ );
	attachNewProperty( aafd, Edgecode,         prop, PID_EdgeCode_Header,                                       OPT );



	aafClass * Pulldown = defineNewClass( aafd, &AAFClassID_Pulldown, CONCRETE, Segment );

	if ( Pulldown == NULL ) { return -1; }

	attachNewProperty( aafd, Pulldown,         prop, PID_Pulldown_InputSegment,                                 REQ );
	attachNewProperty( aafd, Pulldown,         prop, PID_Pulldown_PulldownKind,                                 REQ );
	attachNewProperty( aafd, Pulldown,         prop, PID_Pulldown_PulldownDirection,                            REQ );
	attachNewProperty( aafd, Pulldown,         prop, PID_Pulldown_PhaseFrame,                                   REQ );



	aafClass * OperationGroup = defineNewClass( aafd, &AAFClassID_OperationGroup, CONCRETE, Segment );

	if ( OperationGroup == NULL ) { return -1; }

	attachNewProperty( aafd, OperationGroup,   prop, PID_OperationGroup_Operation,                              REQ );
	attachNewProperty( aafd, OperationGroup,   prop, PID_OperationGroup_InputSegments,                          OPT );
	attachNewProperty( aafd, OperationGroup,   prop, PID_OperationGroup_Parameters,                             OPT );
	attachNewProperty( aafd, OperationGroup,   prop, PID_OperationGroup_Rendering,                              OPT );
	attachNewProperty( aafd, OperationGroup,   prop, PID_OperationGroup_BypassOverride,                         OPT );



	aafClass * NestedScope = defineNewClass( aafd, &AAFClassID_NestedScope, CONCRETE, Segment );

	if ( NestedScope == NULL ) { return -1; }

	attachNewProperty( aafd, NestedScope,      prop, PID_NestedScope_Slots,                                     REQ );



	aafClass * ScopeReference = defineNewClass( aafd, &AAFClassID_ScopeReference, CONCRETE, Segment );

	if ( ScopeReference == NULL ) { return -1; }

	attachNewProperty( aafd, ScopeReference,   prop, PID_ScopeReference_RelativeScope,                          REQ );
	attachNewProperty( aafd, ScopeReference,   prop, PID_ScopeReference_RelativeSlot,                           REQ );



	aafClass * Selector = defineNewClass( aafd, &AAFClassID_Selector, CONCRETE, Segment );

	if ( Selector == NULL ) { return -1; }

	attachNewProperty( aafd, Selector,         prop, PID_Selector_Selected,                                     REQ );
	attachNewProperty( aafd, Selector,         prop, PID_Selector_Alternates,                                   OPT );



	aafClass * EssenceGroup = defineNewClass( aafd, &AAFClassID_EssenceGroup, CONCRETE, Segment );

	if ( EssenceGroup == NULL ) { return -1; }

	attachNewProperty( aafd, EssenceGroup,     prop, PID_EssenceGroup_Choices,                                  REQ );
	attachNewProperty( aafd, EssenceGroup,     prop, PID_EssenceGroup_StillFrame,                               OPT );














	aafClass * DescriptiveFramework = defineNewClass( aafd, &AAFClassID_DescriptiveFramework, ABSTRACT, IOC );

	if ( DescriptiveFramework == NULL ) { return -1; }



	aafClass *EssenceDesc = defineNewClass( aafd, &AAFClassID_EssenceDescriptor, ABSTRACT, IOC );

	if ( EssenceDesc == NULL ) { return -1; }

	attachNewProperty( aafd, EssenceDesc,      prop, PID_EssenceDescriptor_Locator,                             OPT );



	aafClass *FileDesc = defineNewClass( aafd, &AAFClassID_FileDescriptor, ABSTRACT, EssenceDesc );

	if ( FileDesc == NULL ) { return -1; }

	attachNewProperty( aafd, FileDesc,         prop, PID_FileDescriptor_SampleRate,                             REQ );
	attachNewProperty( aafd, FileDesc,         prop, PID_FileDescriptor_Length,                                 REQ );
	attachNewProperty( aafd, FileDesc,         prop, PID_FileDescriptor_ContainerFormat,                        OPT );
	attachNewProperty( aafd, FileDesc,         prop, PID_FileDescriptor_CodecDefinition,                        OPT );
//	attachNewProperty( aafd, FileDesc,         prop, PID_FileDescriptor_LinkedSlotID,                           ??? );



	aafClass *DigitalImageDesc = defineNewClass( aafd, &AAFClassID_DigitalImageDescriptor, ABSTRACT, FileDesc );

	if ( DigitalImageDesc == NULL ) { return -1; }

	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_Compression,                    OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_StoredHeight,                   REQ );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_StoredWidth,                    REQ );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_StoredF2Offset,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledHeight,                  OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledWidth,                   OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledXOffset,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_SampledYOffset,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayHeight,                  OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayWidth,                   OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayXOffset,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayYOffset,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_DisplayF2Offset,                OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_FrameLayout,                    REQ );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_VideoLineMap,                   REQ );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_ImageAspectRatio,               REQ );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_ActiveFormatDescriptor,         OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_AlphaTransparency,              OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_ImageAlignmentFactor,           OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_FieldDominance,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_FieldStartOffset,               OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_FieldEndOffset,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_ColorPrimaries,                 OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_CodingEquations,                OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_TransferCharacteristic,         OPT );
	attachNewProperty( aafd, DigitalImageDesc, prop, PID_DigitalImageDescriptor_SignalStandard,                 OPT );



	aafClass * CDCIDesc = defineNewClass( aafd, &AAFClassID_CDCIDescriptor, CONCRETE, DigitalImageDesc );

	if ( CDCIDesc == NULL ) { return -1; }

	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_HorizontalSubsampling,                  REQ );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_VerticalSubsampling,                    OPT );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_ComponentWidth,                         REQ );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_AlphaSamplingWidth,                     OPT );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_PaddingBits,                            OPT );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_ColorSiting,                            OPT );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_BlackReferenceLevel,                    OPT );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_WhiteReferenceLevel,                    OPT );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_ColorRange,                             OPT );
	attachNewProperty( aafd, CDCIDesc,         prop, PID_CDCIDescriptor_ReversedByteOrder,                      OPT );



	aafClass * RGBADesc = defineNewClass( aafd, &AAFClassID_RGBADescriptor, CONCRETE, DigitalImageDesc );

	if ( RGBADesc == NULL ) { return -1; }

	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_PixelLayout,                            REQ );
	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_Palette,                                OPT );
	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_PaletteLayout,                          OPT );
	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_ComponentMinRef,                        OPT );
	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_ComponentMaxRef,                        OPT );
	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_AlphaMinRef,                            OPT );
	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_AlphaMaxRef,                            OPT );
	attachNewProperty( aafd, RGBADesc,         prop, PID_RGBADescriptor_ScanningDirection,                      OPT );



	aafClass * TapeDesc = defineNewClass( aafd, &AAFClassID_TapeDescriptor, CONCRETE, EssenceDesc );

	if ( TapeDesc == NULL ) { return -1; }

	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_FormFactor,                             OPT );
	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_VideoSignal,                            OPT );
	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_TapeFormat,                             OPT );
	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_Length,                                 OPT );
	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_ManufacturerID,                         OPT );
	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_Model,                                  OPT );
	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_TapeBatchNumber,                        OPT );
	attachNewProperty( aafd, TapeDesc,         prop, PID_TapeDescriptor_TapeStock,                              OPT );



	aafClass * FilmDesc = defineNewClass( aafd, &AAFClassID_FilmDescriptor, CONCRETE, EssenceDesc );

	if ( FilmDesc == NULL ) { return -1; }

	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_FilmFormat,                             OPT );
	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_FrameRate,                              OPT );
	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_PerforationsPerFrame,                   OPT );
	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_FilmAspectRatio,                        OPT );
	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_Manufacturer,                           OPT );
	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_Model,                                  OPT );
	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_FilmGaugeFormat,                        OPT );
	attachNewProperty( aafd, FilmDesc,         prop, PID_FilmDescriptor_FilmBatchNumber,                        OPT );



	aafClass * WAVEDesc = defineNewClass( aafd, &AAFClassID_WAVEDescriptor, CONCRETE, FileDesc );

	if ( WAVEDesc == NULL ) { return -1; }

	attachNewProperty( aafd, WAVEDesc,         prop, PID_WAVEDescriptor_Summary,                                REQ );



	aafClass * AIFCDesc = defineNewClass( aafd, &AAFClassID_AIFCDescriptor, CONCRETE, FileDesc );

	if ( AIFCDesc == NULL ) { return -1; }

	attachNewProperty( aafd, AIFCDesc,         prop, PID_AIFCDescriptor_Summary,                                REQ );



	aafClass * TIFFDesc = defineNewClass( aafd, &AAFClassID_TIFFDescriptor, CONCRETE, FileDesc );

	if ( TIFFDesc == NULL ) { return -1; }

	attachNewProperty( aafd, TIFFDesc,         prop, PID_TIFFDescriptor_IsUniform,                              REQ );
	attachNewProperty( aafd, TIFFDesc,         prop, PID_TIFFDescriptor_IsContiguous,                           REQ );
	attachNewProperty( aafd, TIFFDesc,         prop, PID_TIFFDescriptor_LeadingLines,                           OPT );
	attachNewProperty( aafd, TIFFDesc,         prop, PID_TIFFDescriptor_TrailingLines,                          OPT );
	attachNewProperty( aafd, TIFFDesc,         prop, PID_TIFFDescriptor_JPEGTableID,                            OPT );
	attachNewProperty( aafd, TIFFDesc,         prop, PID_TIFFDescriptor_Summary,                                REQ );



	aafClass * SoundDesc = defineNewClass( aafd, &AAFClassID_SoundDescriptor, CONCRETE, FileDesc );

	if ( SoundDesc == NULL ) { return -1; }

	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_AudioSamplingRate,                     REQ );
	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_Locked,                                OPT );
	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_AudioRefLevel,                         OPT );
	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_ElectroSpatial,                        OPT );
	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_Channels,                              REQ );
	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_QuantizationBits,                      REQ );
	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_DialNorm,                              OPT );
	attachNewProperty( aafd, SoundDesc,        prop, PID_SoundDescriptor_Compression,                           OPT );



	aafClass * PCMDesc = defineNewClass( aafd, &AAFClassID_PCMDescriptor, CONCRETE, SoundDesc );

	if ( PCMDesc == NULL ) { return -1; }

	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_BlockAlign,                              REQ );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_SequenceOffset,                          OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_AverageBPS,                              REQ );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_ChannelAssignment,                       OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeVersion,                     OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeFormat,                      OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PointsPerPeakValue,                      OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeBlockSize,                   OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakChannels,                            OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakFrames,                              OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakOfPeaksPosition,                     OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeTimestamp,                   OPT );
	attachNewProperty( aafd, PCMDesc,          prop, PID_PCMDescriptor_PeakEnvelopeData,                        OPT );



	aafClass * PhysicalDesc = defineNewClass( aafd, &AAFClassID_PhysicalDescriptor, ABSTRACT, EssenceDesc );

	if ( PhysicalDesc == NULL ) { return -1; }

	/* The PhysicalDescriptor class does not define any additional properties. */



	aafClass * ImportDesc = defineNewClass( aafd, &AAFClassID_ImportDescriptor, CONCRETE, PhysicalDesc );

	if ( ImportDesc == NULL ) { return -1; }

	/* The ImportDescriptor class does not define any additional properties. */



	aafClass * RecordingDesc = defineNewClass( aafd, &AAFClassID_RecordingDescriptor, CONCRETE, PhysicalDesc );

	if ( RecordingDesc == NULL ) { return -1; }

	/* The RecordingDescriptor class does not define any additional properties. */



	aafClass * AuxiliaryDesc = defineNewClass( aafd, &AAFClassID_AuxiliaryDescriptor, CONCRETE, PhysicalDesc );

	if ( AuxiliaryDesc == NULL ) { return -1; }

	attachNewProperty( aafd, AuxiliaryDesc,    prop, PID_AuxiliaryDescriptor_MimeType,                          REQ );
	attachNewProperty( aafd, AuxiliaryDesc,    prop, PID_AuxiliaryDescriptor_CharSet,                           OPT );















	aafClass * DefObject = defineNewClass( aafd, &AAFClassID_DefinitionObject, ABSTRACT, IOC );

	if ( DefObject == NULL ) { return -1; }

	attachNewProperty( aafd, DefObject,        prop, PID_DefinitionObject_Identification,                       REQ );
	attachNewProperty( aafd, DefObject,        prop, PID_DefinitionObject_Name,                                 REQ );
	attachNewProperty( aafd, DefObject,        prop, PID_DefinitionObject_Description,                          OPT );



	aafClass * DataDef = defineNewClass( aafd, &AAFClassID_DataDefinition, CONCRETE, DefObject );

	if ( DataDef == NULL ) { return -1; }

	/* The DataDefinition class does not define any additional properties. */



	aafClass * ContainerDef = defineNewClass( aafd, &AAFClassID_ContainerDefinition, CONCRETE, DefObject );

	if ( ContainerDef == NULL ) { return -1; }

	attachNewProperty( aafd, ContainerDef,     prop, PID_ContainerDefinition_EssenceIsIdentified,               OPT );



	aafClass * OperationDef = defineNewClass( aafd, &AAFClassID_OperationDefinition, CONCRETE, DefObject );

	if ( OperationDef == NULL ) { return -1; }

	attachNewProperty( aafd, OperationDef,     prop, PID_OperationDefinition_DataDefinition,                    REQ );
	attachNewProperty( aafd, OperationDef,     prop, PID_OperationDefinition_IsTimeWarp,                        OPT );
	attachNewProperty( aafd, OperationDef,     prop, PID_OperationDefinition_DegradeTo,                         OPT );
	attachNewProperty( aafd, OperationDef,     prop, PID_OperationDefinition_OperationCategory,                 OPT );
	attachNewProperty( aafd, OperationDef,     prop, PID_OperationDefinition_NumberInputs,                      REQ );
	attachNewProperty( aafd, OperationDef,     prop, PID_OperationDefinition_Bypass,                            OPT );
	attachNewProperty( aafd, OperationDef,     prop, PID_OperationDefinition_ParametersDefined,                 OPT );



	aafClass * ParameterDef = defineNewClass( aafd, &AAFClassID_ParameterDefinition, CONCRETE, DefObject );

	if ( ParameterDef == NULL ) { return -1; }

	attachNewProperty( aafd, ParameterDef,     prop, PID_ParameterDefinition_Type,                              REQ );
	attachNewProperty( aafd, ParameterDef,     prop, PID_ParameterDefinition_DisplayUnits,                      OPT );



	aafClass * InterpolationDef = defineNewClass( aafd, &AAFClassID_InterpolationDefinition, CONCRETE, DefObject );

	if ( InterpolationDef == NULL ) { return -1; }

	/* The InterpolationDefinition class does not define any additional properties. */



	aafClass * CodecDef = defineNewClass( aafd, &AAFClassID_CodecDefinition, CONCRETE, DefObject );

	if ( CodecDef == NULL ) { return -1; }

	attachNewProperty( aafd, CodecDef,         prop, PID_CodecDefinition_FileDescriptorClass,                   REQ );
	attachNewProperty( aafd, CodecDef,         prop, PID_CodecDefinition_DataDefinitions,                       REQ );



	aafClass * PluginDef = defineNewClass( aafd, &AAFClassID_PluginDefinition, CONCRETE, DefObject );

	if ( PluginDef == NULL ) { return -1; }

	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_PluginCategory,                       REQ );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_VersionNumber,                        REQ );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_VersionString,                        OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_Manufacturer,                         OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_ManufacturerInfo,                     OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_ManufacturerID,                       OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_Platform,                             OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_MinPlatformVersion,                   OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_MaxPlatformVersion,                   OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_Engine,                               OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_MinEngineVersion,                     OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_MaxEngineVersion,                     OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_PluginAPI,                            OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_MinPluginAPI,                         OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_MaxPluginAPI,                         OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_SoftwareOnly,                         OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_Accelerator,                          OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_Locators,                             OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_Authentication,                       OPT );
	attachNewProperty( aafd, PluginDef,        prop, PID_PluginDefinition_DefinitionObject,                     OPT );


	aafClass * TaggedValueDef = defineNewClass( aafd, &AAFClassID_TaggedValueDefinition, CONCRETE, DefObject );

	if ( TaggedValueDef == NULL ) { return -1; }

	/* The TaggedValueDefinition class does not define any additional properties. */



	aafClass * KLVDataDef = defineNewClass( aafd, &AAFClassID_KLVDataDefinition, CONCRETE, DefObject );

	if ( KLVDataDef == NULL ) { return -1; }

	attachNewProperty( aafd, KLVDataDef,       prop, PID_KLVDataDefinition_KLVDataType,                         OPT );



	aafClass * EssenceData = defineNewClass( aafd, &AAFClassID_EssenceData, CONCRETE, IOC );

	if ( EssenceData == NULL ) { return -1; }

	attachNewProperty( aafd, EssenceData,      prop, PID_EssenceData_MobID,                                     REQ );
	attachNewProperty( aafd, EssenceData,      prop, PID_EssenceData_Data,                                      REQ );
	attachNewProperty( aafd, EssenceData,      prop, PID_EssenceData_SampleIndex,                               OPT );













	aafClass * MetaDefinition = defineNewClass( aafd, &AAFClassID_MetaDefinition, ABSTRACT, NULL );

	if ( MetaDefinition == NULL ) { return -1; }

	attachNewProperty( aafd, MetaDefinition,   prop, PID_MetaDefinition_Identification,                         REQ );
	attachNewProperty( aafd, MetaDefinition,   prop, PID_MetaDefinition_Name,                                   REQ );
	attachNewProperty( aafd, MetaDefinition,   prop, PID_MetaDefinition_Description,                            OPT );



	aafClass * ClassDefinition = defineNewClass( aafd, &AAFClassID_ClassDefinition, CONCRETE, MetaDefinition );

	if ( ClassDefinition == NULL ) { return -1; }

	attachNewProperty( aafd, ClassDefinition,  prop, PID_ClassDefinition_ParentClass,                           REQ );
	attachNewProperty( aafd, ClassDefinition,  prop, PID_ClassDefinition_Properties,                            OPT );
	attachNewProperty( aafd, ClassDefinition,  prop, PID_ClassDefinition_IsConcrete,                            REQ );



	aafClass * PropertyDefinition = defineNewClass( aafd, &AAFClassID_PropertyDefinition, CONCRETE, MetaDefinition );

	if ( PropertyDefinition == NULL ) { return -1; }

	attachNewProperty( aafd, PropertyDefinition, prop, PID_PropertyDefinition_Type,                             REQ );
	attachNewProperty( aafd, PropertyDefinition, prop, PID_PropertyDefinition_IsOptional,                       REQ );
	attachNewProperty( aafd, PropertyDefinition, prop, PID_PropertyDefinition_LocalIdentification,              REQ );
	attachNewProperty( aafd, PropertyDefinition, prop, PID_PropertyDefinition_IsUniqueIdentifier,               OPT );



	aafClass * TypeDef = defineNewClass( aafd, &AAFClassID_TypeDefinition, ABSTRACT, MetaDefinition );

	if ( TypeDef == NULL ) { return -1; }

	/* The TypeDefinition class does not define any additional properties. */



	aafClass * TypeDefCharacter = defineNewClass( aafd, &AAFClassID_TypeDefinitionCharacter, CONCRETE, TypeDef );

	if ( TypeDefCharacter == NULL ) { return -1; }

	/* The TypeDefinitionCharacter class does not define any additional properties. */



	aafClass * TypeDefEnum = defineNewClass( aafd, &AAFClassID_TypeDefinitionEnumeration, CONCRETE, TypeDef );

	if ( TypeDefEnum == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefEnum,      prop, PID_TypeDefinitionEnumeration_ElementType,                 REQ );
	attachNewProperty( aafd, TypeDefEnum,      prop, PID_TypeDefinitionEnumeration_ElementNames,                REQ );
	attachNewProperty( aafd, TypeDefEnum,      prop, PID_TypeDefinitionEnumeration_ElementValues,               REQ );



	aafClass * TypeDefExtEnum = defineNewClass( aafd, &AAFClassID_TypeDefinitionExtendibleEnumeration, CONCRETE, TypeDef );

	if ( TypeDefExtEnum == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefExtEnum,   prop, PID_TypeDefinitionExtendibleEnumeration_ElementNames,      REQ );
	attachNewProperty( aafd, TypeDefExtEnum,   prop, PID_TypeDefinitionExtendibleEnumeration_ElementValues,     REQ );



	aafClass * TypeDefFixedArray = defineNewClass( aafd, &AAFClassID_TypeDefinitionFixedArray, CONCRETE, TypeDef );

	if ( TypeDefFixedArray == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefFixedArray, prop, PID_TypeDefinitionFixedArray_ElementType,                 REQ );
	attachNewProperty( aafd, TypeDefFixedArray, prop, PID_TypeDefinitionFixedArray_ElementCount,                REQ );



	aafClass * TypeDefIndirect = defineNewClass( aafd, &AAFClassID_TypeDefinitionIndirect, CONCRETE, TypeDef );

	if ( TypeDefIndirect == NULL ) { return -1; }

	/* The TypeDefinitionIndirect class does not define any additional properties. */



	aafClass * TypeDefInt = defineNewClass( aafd, &AAFClassID_TypeDefinitionInteger, CONCRETE, TypeDef );

	if ( TypeDefInt == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefInt,       prop, PID_TypeDefinitionInteger_Size,                            REQ );
	attachNewProperty( aafd, TypeDefInt,       prop, PID_TypeDefinitionInteger_IsSigned,                        REQ );



	aafClass * TypeDefOpaque = defineNewClass( aafd, &AAFClassID_TypeDefinitionOpaque, CONCRETE, TypeDef );

	if ( TypeDefOpaque == NULL ) { return -1; }

	/* The TypeDefinitionOpaque class does not define any additional properties. */



	aafClass * TypeDefRecord = defineNewClass( aafd, &AAFClassID_TypeDefinitionRecord, CONCRETE, TypeDef );

	if ( TypeDefRecord == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefRecord,    prop, PID_TypeDefinitionRecord_MemberTypes,                      REQ );
	attachNewProperty( aafd, TypeDefRecord,    prop, PID_TypeDefinitionRecord_MemberNames,                      REQ );



	aafClass * TypeDefRename = defineNewClass( aafd, &AAFClassID_TypeDefinitionRename, CONCRETE, TypeDef );

	if ( TypeDefRename == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefRename,    prop, PID_TypeDefinitionRename_RenamedType,                      REQ );



	aafClass * TypeDefSet = defineNewClass( aafd, &AAFClassID_TypeDefinitionSet, CONCRETE, TypeDef );

	if ( TypeDefSet == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefSet,       prop, PID_TypeDefinitionSet_ElementType,                         REQ );



	aafClass * TypeDefStream = defineNewClass( aafd, &AAFClassID_TypeDefinitionStream, CONCRETE, TypeDef );

	if ( TypeDefStream == NULL ) { return -1; }

	/* The TypeDefinitionStream class does not define any additional properties. */



	aafClass * TypeDefString = defineNewClass( aafd, &AAFClassID_TypeDefinitionString, CONCRETE, TypeDef );

	if ( TypeDefString == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefString,    prop, PID_TypeDefinitionString_ElementType,                      REQ );



	aafClass * TypeDefStrongObjRef = defineNewClass( aafd, &AAFClassID_TypeDefinitionStrongObjectReference, CONCRETE, TypeDef );

	if ( TypeDefStrongObjRef == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefStrongObjRef, prop, PID_TypeDefinitionStrongObjectReference_ReferencedType, REQ );



	aafClass * TypeDefVariableArray = defineNewClass( aafd, &AAFClassID_TypeDefinitionVariableArray, CONCRETE, TypeDef );

	if ( TypeDefVariableArray == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefVariableArray, prop, PID_TypeDefinitionVariableArray_ElementType,           REQ );



	aafClass * TypeDefWeakObjRef = defineNewClass( aafd, &AAFClassID_TypeDefinitionWeakObjectReference, CONCRETE, TypeDef );

	if ( TypeDefWeakObjRef == NULL ) { return -1; }

	attachNewProperty( aafd, TypeDefWeakObjRef, prop, PID_TypeDefinitionWeakObjectReference_ReferencedType,     REQ );
	attachNewProperty( aafd, TypeDefWeakObjRef, prop, PID_TypeDefinitionWeakObjectReference_TargetSet,          REQ );



	aafClass * MetaDictionary = defineNewClass( aafd, &AAFClassID_MetaDictionary, CONCRETE, NULL );

	if ( MetaDictionary == NULL ) { return -1; }

	attachNewProperty( aafd, MetaDictionary,   prop, PID_MetaDictionary_ClassDefinitions,                       OPT );
	attachNewProperty( aafd, MetaDictionary,   prop, PID_MetaDictionary_TypeDefinitions,                        OPT );


	return 0;
}
