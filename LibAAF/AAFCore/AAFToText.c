#include <stdio.h>

#include "AAFTypes.h"
#include "AAFUtils.h"

#include "AAFDefs/AAFClassDefUIDs.h"
#include "AAFDefs/AAFPropertyIDs.h"
#include "AAFDefs/AAFCompressionDefs.h"
#include "AAFDefs/AAFDataDefs.h"
#include "AAFDefs/AAFOperationDefs.h"
#include "AAFDefs/AAFParameterDefs.h"
#include "AAFDefs/AAFInterpolatorDefs.h"
#include "AAFDefs/AAFTypeDefUIDs.h"
#include "AAFDefs/AAFExtEnum.h"
#include "AAFDefs/AAFFileKinds.h"
#include "AAFDefs/AAFOPDefs.h"
#include "AAFDefs/AAFContainerDefs.h"

#include "../LibCFB/CFBDump.h"





const char * TimestampToText( aafTimeStamp_t *ts )
{
	static char str[24];

	if ( ts == NULL )
	{
		str[0] = 'n';
		str[1] = '/';
		str[2] = 'a';
		str[3] = '\0';
	}
	else
	{
		snprintf( str, sizeof(str), "%04i-%02u-%02u %02u:%02u:%02u.%02u",
			ts->date.year,
			ts->date.month,
			ts->date.day,
			ts->time.hour,
			ts->time.minute,
			ts->time.second,
			ts->time.fraction );
	}

	return str;
}




const char * VersionToText( aafVersionType_t *vers )
{
	static char str[16];

	if ( vers == NULL )
	{
		str[0] = 'n';
		str[1] = '/';
		str[2] = 'a';
		str[3] = '\0';
	}
	else
	{
		snprintf( str, sizeof(str), "%i.%i",
			vers->major,
			vers->minor );
	}

	return str;
}




const char * ProductVersionToText( aafProductVersion_t *vers )
{
	static char str[64];

	if ( vers == NULL )
	{
		str[0] = 'n';
		str[1] = '/';
		str[2] = 'a';
		str[3] = '\0';
	}
	else
	{
		snprintf( str, sizeof(str), "%u.%u.%u.%u %s (%i)",
			vers->major,
			vers->minor,
		 	vers->tertiary,
			vers->patchLevel,
			ProductReleaseTypeToText( vers->type ),
			vers->type );
	}

	return str;
}




const char * FileKindToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	/* AAFUID_NULL == AAFFileKind_DontCare */
	// if ( auidCmp( auid, &AAFUID_NULL ) )
	// 	return "";

	if ( auidCmp( auid, &AAFFileKind_DontCare ) )
		return "AAFFileKind_DontCare";

	if ( auidCmp( auid, &AAFFileKind_Aaf512Binary ) )
		return "AAFFileKind_Aaf512Binary";

	if ( auidCmp( auid, &AAFFileKind_Aaf4KBinary ) )
		return "AAFFileKind_Aaf4KBinary";

	if ( auidCmp( auid, &AAFFileKind_AafXmlText ) )
		return "AAFFileKind_AafXmlText";

	if ( auidCmp( auid, &AAFFileKind_AafKlvBinary ) )
		return "AAFFileKind_AafKlvBinary";

	if ( auidCmp( auid, &AAFFileKind_AafM512Binary ) )
		return "AAFFileKind_AafM512Binary";

	if ( auidCmp( auid, &AAFFileKind_AafS512Binary ) )
		return "AAFFileKind_AafS512Binary";

	if ( auidCmp( auid, &AAFFileKind_AafG512Binary ) )
		return "AAFFileKind_AafG512Binary";

	if ( auidCmp( auid, &AAFFileKind_AafM4KBinary ) )
		return "AAFFileKind_AafM4KBinary";

	if ( auidCmp( auid, &AAFFileKind_AafS4KBinary ) )
		return "AAFFileKind_AafS4KBinary";

	if ( auidCmp( auid, &AAFFileKind_AafG4KBinary ) )
		return "AAFFileKind_AafG4KBinary";

	if ( auidCmp( auid, &AAFFileKind_Pathological ) )
		return "AAFFileKind_Pathological";

	return "Unknown value";

}


const char * TapeCaseTypeToText( aafTapeCaseType_t t )
{
	switch ( t )
	{
		case AAFTapeCaseNull:					return "AAFTapeCaseNull";
		case AAFThreeFourthInchVideoTape:		return "AAFThreeFourthInchVideoTape";
		case AAFVHSVideoTape:					return "AAFVHSVideoTape";
		case AAF8mmVideoTape:					return "AAF8mmVideoTape";
		case AAFBetacamVideoTape:				return "AAFBetacamVideoTape";
		case AAFCompactCassette:				return "AAFCompactCassette";
		case AAFDATCartridge:					return "AAFDATCartridge";
		case AAFNagraAudioTape:					return "AAFNagraAudioTape";

		default:								return "Unknown value";
	}
}


const char * VideoSignalTypeToText( aafVideoSignalType_t v )
{
	switch ( v )
	{
		case AAFVideoSignalNull:				return "AAFVideoSignalNull";
		case AAFNTSCSignal:						return "AAFNTSCSignal";
		case AAFPALSignal:						return "AAFPALSignal";
		case AAFSECAMSignal:					return "AAFSECAMSignal";

		default:								return "Unknown value";
	}
}


const char * TapeFormatTypeToText( aafTapeFormatType_t t )
{
	switch ( t )
	{
		case AAFTapeFormatNull:					return "AAFTapeFormatNull";
		case AAFBetacamFormat:					return "AAFBetacamFormat";
		case AAFBetacamSPFormat:				return "AAFBetacamSPFormat";
		case AAFVHSFormat:						return "AAFVHSFormat";
		case AAFSVHSFormat:						return "AAFSVHSFormat";
		case AAF8mmFormat:						return "AAF8mmFormat";
		case AAFHi8Format:						return "AAFHi8Format";

		default:								return "Unknown value";
	}
}


const char * FilmTypeToText( aafFilmType_t f )
{
	switch ( f )
	{
		case AAFFtNull:							return "AAFFtNull";
		case AAFFt35MM:							return "AAFFt35MM";
		case AAFFt16MM:							return "AAFFt16MM";
		case AAFFt8MM:							return "AAFFt8MM";
		case AAFFt65MM:							return "AAFFt65MM";

		default:								return "Unknown value";
	}
}


const char * SignalStandardToText( aafSignalStandard_t s )
{
	switch ( s )
	{
		case AAFSignalStandard_None:			return "AAFSignalStandard_None";
		case AAFSignalStandard_ITU601:			return "AAFSignalStandard_ITU601";
		case AAFSignalStandard_ITU1358:			return "AAFSignalStandard_ITU1358";
		case AAFSignalStandard_SMPTE347M:		return "AAFSignalStandard_SMPTE347M";
		case AAFSignalStandard_SMPTE274M:		return "AAFSignalStandard_SMPTE274M";
		case AAFSignalStandard_SMPTE296M:		return "AAFSignalStandard_SMPTE296M";
		case AAFSignalStandard_SMPTE349M:		return "AAFSignalStandard_SMPTE349M";

		default:								return "Unknown value";
	}
}


const char * FieldNumberToText( aafFieldNumber_t f )
{
	switch ( f )
	{
		case AAFUnspecifiedField:				return "AAFUnspecifiedField";
		case AAFFieldOne:						return "AAFFieldOne";
		case AAFFieldTwo:						return "AAFFieldTwo";

		default:								return "Unknown value";
	}
}


const char * AlphaTransparencyToText( aafAlphaTransparency_t a )
{
	switch ( a )
	{
		case AAFMinValueTransparent:			return "AAFMinValueTransparent";
		case AAFMaxValueTransparent:			return "AAFMaxValueTransparent";

		default:								return "Unknown value";
	}
}


const char * FrameLayoutToText( aafFrameLayout_t f )
{
	switch ( f )
	{
		case AAFFullFrame:						return "AAFFullFrame";
		case AAFSeparateFields:					return "AAFSeparateFields";
		case AAFOneField:						return "AAFOneField";
		case AAFMixedFields:					return "AAFMixedFields";
		case AAFSegmentedFrame:					return "AAFSegmentedFrame";

		default:								return "Unknown value";
	}
}


const char * ColorSitingToText( aafColorSiting_t c )
{
	switch ( c )
	{
		case AAFCoSiting:						return "AAFCoSiting";
		case AAFAveraging:						return "AAFAveraging";
		case AAFThreeTap:						return "AAFThreeTap";
		case AAFQuincunx:						return "AAFQuincunx";
		case AAFRec601:							return "AAFRec601";
		case AAFUnknownSiting:					return "AAFUnknownSiting";

		default:								return "Unknown value";
	}
}


const char * ProductReleaseTypeToText( aafProductReleaseType_t t )
{
	switch ( t )
	{
		case AAFVersionUnknown:					return "AAFVersionUnknown";
		case AAFVersionReleased:				return "AAFVersionReleased";
		case AAFVersionDebug:					return "AAFVersionDebug";
		case AAFVersionPatched:					return "AAFVersionPatched";
		case AAFVersionBeta:					return "AAFVersionBeta";
		case AAFVersionPrivateBuild:			return "AAFVersionPrivateBuild";

		default:								return "Unknown value";
	}
}


const char * FadeTypeToText( aafFadeType_t f )
{
	switch ( f )
	{
		case AAFFadeNone:						return "AAFFadeNone";
		case AAFFadeLinearAmp:					return "AAFFadeLinearAmp";
		case AAFFadeLinearPower:				return "AAFFadeLinearPower";

		default:								return "Unknown value";
	}
}


const char * BoolToText( aafBoolean_t b )
{
	switch ( b )
	{
		case 1:									return "True";
		case 0:									return "False";

		default:								return "Unknown value";
	}
}


const char * OperationCategoryToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	if ( auidCmp( auid, &AAFOperationCategory_Effect ) )
		return "AAFOperationCategory_Effect";

	return "Unknown value";
}


const char * PluginCategoryToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	if ( auidCmp( auid, &AAFPluginCategory_Effect ) )
		return "AAFPluginCategory_Effect";

	if ( auidCmp( auid, &AAFPluginCategory_Codec ) )
		return "AAFPluginCategory_Codec";

	if ( auidCmp( auid, &AAFPluginCategory_Interpolation ) )
		return "AAFPluginCategory_Interpolation";

	return "Unknown value";
}


const char * ScanningDirectionToText( aafScanningDirection_t s )
{
	switch ( s )
	{
		case AAFScanningDirection_LeftToRightTopToBottom:
	 return "AAFScanningDirection_LeftToRightTopToBottom";

		case AAFScanningDirection_RightToLeftTopToBottom:
	 return "AAFScanningDirection_RightToLeftTopToBottom";

		case AAFScanningDirection_LeftToRightBottomToTop:
	 return "AAFScanningDirection_LeftToRightBottomToTop";

		case AAFScanningDirection_RightToLeftBottomToTop:
	 return "AAFScanningDirection_RightToLeftBottomToTop";

		case AAFScanningDirection_TopToBottomLeftToRight:
	 return "AAFScanningDirection_TopToBottomLeftToRight";

		case AAFScanningDirection_TopToBottomRightToLeft:
	 return "AAFScanningDirection_TopToBottomRightToLeft";

		case AAFScanningDirection_BottomToTopLeftToRight:
	 return "AAFScanningDirection_BottomToTopLeftToRight";

		case AAFScanningDirection_BottomToTopRightToLeft:
	 return "AAFScanningDirection_BottomToTopRightToLeft";


		default:
	 return "Unknown value";
	}

}


const char * ByteOrderToText( int16_t e )
{
	if ( e == AAF_HEADER_BYTEORDER_LE ||
		 e == AAF_PROPERTIES_BYTEORDER_LE )
			return "Little-Endian";

	if ( e == AAF_HEADER_BYTEORDER_BE ||
		 e == AAF_PROPERTIES_BYTEORDER_BE )
			return "Big-Endian";

	return "Unknown value";
}


const char * ElectroSpatialToText( aafElectroSpatialFormulation_t e )
{
	switch ( e )
	{
		case AAFElectroSpatialFormulation_Default:
	 return "AAFElectroSpatialFormulation_Default";

		case AAFElectroSpatialFormulation_TwoChannelMode:
	 return "AAFElectroSpatialFormulation_TwoChannelMode";

		case AAFElectroSpatialFormulation_SingleChannelMode:
	 return "AAFElectroSpatialFormulation_SingleChannelMode";

		case AAFElectroSpatialFormulation_PrimarySecondaryMode:
	 return "AAFElectroSpatialFormulation_PrimarySecondaryMode";

		case AAFElectroSpatialFormulation_StereophonicMode:
	 return "AAFElectroSpatialFormulation_StereophonicMode";

		case AAFElectroSpatialFormulation_SingleChannelDoubleSamplingFrequencyMode:
	 return "AAFElectroSpatialFormulation_SingleChannelDoubleSamplingFrequencyMode";

		case AAFElectroSpatialFormulation_StereoLeftChannelDoubleSamplingFrequencyMode:
	 return "AAFElectroSpatialFormulation_StereoLeftChannelDoubleSamplingFrequencyMode";

		case AAFElectroSpatialFormulation_StereoRightChannelDoubleSamplingFrequencyMode:
	 return "AAFElectroSpatialFormulation_StereoRightChannelDoubleSamplingFrequencyMode";

		case AAFElectroSpatialFormulation_MultiChannelMode:
	 return "AAFElectroSpatialFormulation_MultiChannelMode";

		default:
	 return "Unknown value";
	}
}


const char * OPDefToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";


	if ( auidCmp( auid, &AAFOPDef_EditProtocol ) )
		return "AAFOPDef_EditProtocol";

	if ( auidCmp( auid, &AAFOPDef_Unconstrained ) )
		return "AAFOPDef_Unconstrained";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	return "Unknown value";

}


const char * TypeIDToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";


	if ( auidCmp( auid, &AAFTypeID_UInt8 ) )
		return "AAFTypeID_UInt8";

	if ( auidCmp( auid, &AAFTypeID_UInt16 ) )
		return "AAFTypeID_UInt16";

	if ( auidCmp( auid, &AAFTypeID_UInt32 ) )
		return "AAFTypeID_UInt32";

	if ( auidCmp( auid, &AAFTypeID_UInt64 ) )
		return "AAFTypeID_UInt64";

	if ( auidCmp( auid, &AAFTypeID_Int8 ) )
		return "AAFTypeID_Int8";

	if ( auidCmp( auid, &AAFTypeID_Int16 ) )
		return "AAFTypeID_Int16";

	if ( auidCmp( auid, &AAFTypeID_Int32 ) )
		return "AAFTypeID_Int32";

	if ( auidCmp( auid, &AAFTypeID_Int64 ) )
		return "AAFTypeID_Int64";

	if ( auidCmp( auid, &AAFTypeID_PositionType ) )
		return "AAFTypeID_PositionType";

	if ( auidCmp( auid, &AAFTypeID_LengthType ) )
		return "AAFTypeID_LengthType";

	if ( auidCmp( auid, &AAFTypeID_JPEGTableIDType ) )
		return "AAFTypeID_JPEGTableIDType";

	if ( auidCmp( auid, &AAFTypeID_PhaseFrameType ) )
		return "AAFTypeID_PhaseFrameType";

	if ( auidCmp( auid, &AAFTypeID_AUID ) )
		return "AAFTypeID_AUID";

	if ( auidCmp( auid, &AAFTypeID_MobIDType ) )
		return "AAFTypeID_MobIDType";

	if ( auidCmp( auid, &AAFTypeID_Boolean ) )
		return "AAFTypeID_Boolean";

	if ( auidCmp( auid, &AAFTypeID_Character ) )
		return "AAFTypeID_Character";

	if ( auidCmp( auid, &AAFTypeID_String ) )
		return "AAFTypeID_String";

	if ( auidCmp( auid, &AAFTypeID_ProductReleaseType ) )
		return "AAFTypeID_ProductReleaseType";

	if ( auidCmp( auid, &AAFTypeID_TapeFormatType ) )
		return "AAFTypeID_TapeFormatType";

	if ( auidCmp( auid, &AAFTypeID_VideoSignalType ) )
		return "AAFTypeID_VideoSignalType";

	if ( auidCmp( auid, &AAFTypeID_TapeCaseType ) )
		return "AAFTypeID_TapeCaseType";

	if ( auidCmp( auid, &AAFTypeID_ColorSitingType ) )
		return "AAFTypeID_ColorSitingType";

	if ( auidCmp( auid, &AAFTypeID_EditHintType ) )
		return "AAFTypeID_EditHintType";

	if ( auidCmp( auid, &AAFTypeID_FadeType ) )
		return "AAFTypeID_FadeType";

	if ( auidCmp( auid, &AAFTypeID_LayoutType ) )
		return "AAFTypeID_LayoutType";

	if ( auidCmp( auid, &AAFTypeID_TCSource ) )
		return "AAFTypeID_TCSource";

	if ( auidCmp( auid, &AAFTypeID_PulldownDirectionType ) )
		return "AAFTypeID_PulldownDirectionType";

	if ( auidCmp( auid, &AAFTypeID_PulldownKindType ) )
		return "AAFTypeID_PulldownKindType";

	if ( auidCmp( auid, &AAFTypeID_EdgeType ) )
		return "AAFTypeID_EdgeType";

	if ( auidCmp( auid, &AAFTypeID_FilmType ) )
		return "AAFTypeID_FilmType";

	if ( auidCmp( auid, &AAFTypeID_RGBAComponentKind ) )
		return "AAFTypeID_RGBAComponentKind";

	if ( auidCmp( auid, &AAFTypeID_ReferenceType ) )
		return "AAFTypeID_ReferenceType";

	if ( auidCmp( auid, &AAFTypeID_AlphaTransparencyType ) )
		return "AAFTypeID_AlphaTransparencyType";

	if ( auidCmp( auid, &AAFTypeID_FieldNumber ) )
		return "AAFTypeID_FieldNumber";

	if ( auidCmp( auid, &AAFTypeID_ElectroSpatialFormulation ) )
		return "AAFTypeID_ElectroSpatialFormulation";

	if ( auidCmp( auid, &AAFTypeID_EmphasisType ) )
		return "AAFTypeID_EmphasisType";

	if ( auidCmp( auid, &AAFTypeID_AuxBitsModeType ) )
		return "AAFTypeID_AuxBitsModeType";

	if ( auidCmp( auid, &AAFTypeID_ChannelStatusModeType ) )
		return "AAFTypeID_ChannelStatusModeType";

	if ( auidCmp( auid, &AAFTypeID_UserDataModeType ) )
		return "AAFTypeID_UserDataModeType";

	if ( auidCmp( auid, &AAFTypeID_SignalStandardType ) )
		return "AAFTypeID_SignalStandardType";

	if ( auidCmp( auid, &AAFTypeID_ScanningDirectionType ) )
		return "AAFTypeID_ScanningDirectionType";

	if ( auidCmp( auid, &AAFTypeID_ContentScanningType ) )
		return "AAFTypeID_ContentScanningType";

	if ( auidCmp( auid, &AAFTypeID_TitleAlignmentType ) )
		return "AAFTypeID_TitleAlignmentType";

	if ( auidCmp( auid, &AAFTypeID_OperationCategoryType ) )
		return "AAFTypeID_OperationCategoryType";

	if ( auidCmp( auid, &AAFTypeID_TransferCharacteristicType ) )
		return "AAFTypeID_TransferCharacteristicType";

	if ( auidCmp( auid, &AAFTypeID_PluginCategoryType ) )
		return "AAFTypeID_PluginCategoryType";

	if ( auidCmp( auid, &AAFTypeID_UsageType ) )
		return "AAFTypeID_UsageType";

	if ( auidCmp( auid, &AAFTypeID_ColorPrimariesType ) )
		return "AAFTypeID_ColorPrimariesType";

	if ( auidCmp( auid, &AAFTypeID_CodingEquationsType ) )
		return "AAFTypeID_CodingEquationsType";

	if ( auidCmp( auid, &AAFTypeID_Rational ) )
		return "AAFTypeID_Rational";

	if ( auidCmp( auid, &AAFTypeID_ProductVersion ) )
		return "AAFTypeID_ProductVersion";

	if ( auidCmp( auid, &AAFTypeID_VersionType ) )
		return "AAFTypeID_VersionType";

	if ( auidCmp( auid, &AAFTypeID_RGBAComponent ) )
		return "AAFTypeID_RGBAComponent";

	if ( auidCmp( auid, &AAFTypeID_DateStruct ) )
		return "AAFTypeID_DateStruct";

	if ( auidCmp( auid, &AAFTypeID_TimeStruct ) )
		return "AAFTypeID_TimeStruct";

	if ( auidCmp( auid, &AAFTypeID_TimeStamp ) )
		return "AAFTypeID_TimeStamp";

	if ( auidCmp( auid, &AAFTypeID_UInt8Array ) )
		return "AAFTypeID_UInt8Array";

	if ( auidCmp( auid, &AAFTypeID_UInt8Array12 ) )
		return "AAFTypeID_UInt8Array12";

	if ( auidCmp( auid, &AAFTypeID_Int32Array ) )
		return "AAFTypeID_Int32Array";

	if ( auidCmp( auid, &AAFTypeID_Int64Array ) )
		return "AAFTypeID_Int64Array";

	if ( auidCmp( auid, &AAFTypeID_StringArray ) )
		return "AAFTypeID_StringArray";

	if ( auidCmp( auid, &AAFTypeID_AUIDArray ) )
		return "AAFTypeID_AUIDArray";

	if ( auidCmp( auid, &AAFTypeID_PositionArray ) )
		return "AAFTypeID_PositionArray";

	if ( auidCmp( auid, &AAFTypeID_UInt8Array8 ) )
		return "AAFTypeID_UInt8Array8";

	if ( auidCmp( auid, &AAFTypeID_UInt32Array ) )
		return "AAFTypeID_UInt32Array";

	if ( auidCmp( auid, &AAFTypeID_ChannelStatusModeArray ) )
		return "AAFTypeID_ChannelStatusModeArray";

	if ( auidCmp( auid, &AAFTypeID_UserDataModeArray ) )
		return "AAFTypeID_UserDataModeArray";

	if ( auidCmp( auid, &AAFTypeID_RGBALayout ) )
		return "AAFTypeID_RGBALayout";

	if ( auidCmp( auid, &AAFTypeID_AUIDSet ) )
		return "AAFTypeID_AUIDSet";

	if ( auidCmp( auid, &AAFTypeID_UInt32Set ) )
		return "AAFTypeID_UInt32Set";

	if ( auidCmp( auid, &AAFTypeID_DataValue ) )
		return "AAFTypeID_DataValue";

	if ( auidCmp( auid, &AAFTypeID_Stream ) )
		return "AAFTypeID_Stream";

	if ( auidCmp( auid, &AAFTypeID_Indirect ) )
		return "AAFTypeID_Indirect";

	if ( auidCmp( auid, &AAFTypeID_Opaque ) )
		return "AAFTypeID_Opaque";

	if ( auidCmp( auid, &AAFTypeID_ClassDefinitionWeakReference ) )
		return "AAFTypeID_ClassDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_ContainerDefinitionWeakReference ) )
		return "AAFTypeID_ContainerDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionWeakReference ) )
		return "AAFTypeID_DataDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_InterpolationDefinitionWeakReference ) )
		return "AAFTypeID_InterpolationDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_MobWeakReference ) )
		return "AAFTypeID_MobWeakReference";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionWeakReference ) )
		return "AAFTypeID_OperationDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionWeakReference ) )
		return "AAFTypeID_ParameterDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionWeakReference ) )
		return "AAFTypeID_TypeDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionWeakReference ) )
		return "AAFTypeID_PluginDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_CodecDefinitionWeakReference ) )
		return "AAFTypeID_CodecDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionWeakReference ) )
		return "AAFTypeID_PropertyDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_ContentStorageStrongReference ) )
		return "AAFTypeID_ContentStorageStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DictionaryStrongReference ) )
		return "AAFTypeID_DictionaryStrongReference";

	if ( auidCmp( auid, &AAFTypeID_EssenceDescriptorStrongReference ) )
		return "AAFTypeID_EssenceDescriptorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_NetworkLocatorStrongReference ) )
		return "AAFTypeID_NetworkLocatorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_OperationGroupStrongReference ) )
		return "AAFTypeID_OperationGroupStrongReference";

	if ( auidCmp( auid, &AAFTypeID_SegmentStrongReference ) )
		return "AAFTypeID_SegmentStrongReference";

	if ( auidCmp( auid, &AAFTypeID_SourceClipStrongReference ) )
		return "AAFTypeID_SourceClipStrongReference";

	if ( auidCmp( auid, &AAFTypeID_SourceReferenceStrongReference ) )
		return "AAFTypeID_SourceReferenceStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ClassDefinitionStrongReference ) )
		return "AAFTypeID_ClassDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_CodecDefinitionStrongReference ) )
		return "AAFTypeID_CodecDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ComponentStrongReference ) )
		return "AAFTypeID_ComponentStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ContainerDefinitionStrongReference ) )
		return "AAFTypeID_ContainerDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ControlPointStrongReference ) )
		return "AAFTypeID_ControlPointStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionStrongReference ) )
		return "AAFTypeID_DataDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_EssenceDataStrongReference ) )
		return "AAFTypeID_EssenceDataStrongReference";

	if ( auidCmp( auid, &AAFTypeID_IdentificationStrongReference ) )
		return "AAFTypeID_IdentificationStrongReference";

	if ( auidCmp( auid, &AAFTypeID_InterpolationDefinitionStrongReference ) )
		return "AAFTypeID_InterpolationDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_LocatorStrongReference ) )
		return "AAFTypeID_LocatorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_MobStrongReference ) )
		return "AAFTypeID_MobStrongReference";

	if ( auidCmp( auid, &AAFTypeID_MobSlotStrongReference ) )
		return "AAFTypeID_MobSlotStrongReference";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionStrongReference ) )
		return "AAFTypeID_OperationDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ParameterStrongReference ) )
		return "AAFTypeID_ParameterStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionStrongReference ) )
		return "AAFTypeID_ParameterDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionStrongReference ) )
		return "AAFTypeID_PluginDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionStrongReference ) )
		return "AAFTypeID_PropertyDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueStrongReference ) )
		return "AAFTypeID_TaggedValueStrongReference";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionStrongReference ) )
		return "AAFTypeID_TypeDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_KLVDataStrongReference ) )
		return "AAFTypeID_KLVDataStrongReference";

	if ( auidCmp( auid, &AAFTypeID_FileDescriptorStrongReference ) )
		return "AAFTypeID_FileDescriptorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_RIFFChunkStrongReference ) )
		return "AAFTypeID_RIFFChunkStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveFrameworkStrongReference ) )
		return "AAFTypeID_DescriptiveFrameworkStrongReference";

	if ( auidCmp( auid, &AAFTypeID_KLVDataDefinitionStrongReference ) )
		return "AAFTypeID_KLVDataDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueDefinitionStrongReference ) )
		return "AAFTypeID_TaggedValueDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveObjectStrongReference ) )
		return "AAFTypeID_DescriptiveObjectStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionWeakReferenceSet ) )
		return "AAFTypeID_DataDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionWeakReferenceSet ) )
		return "AAFTypeID_ParameterDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionWeakReferenceSet ) )
		return "AAFTypeID_PluginDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionWeakReferenceSet ) )
		return "AAFTypeID_PropertyDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionWeakReferenceVector ) )
		return "AAFTypeID_OperationDefinitionWeakReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionWeakReferenceVector ) )
		return "AAFTypeID_TypeDefinitionWeakReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionWeakReferenceVector ) )
		return "AAFTypeID_DataDefinitionWeakReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_ClassDefinitionStrongReferenceSet ) )
		return "AAFTypeID_ClassDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_CodecDefinitionStrongReferenceSet ) )
		return "AAFTypeID_CodecDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ContainerDefinitionStrongReferenceSet ) )
		return "AAFTypeID_ContainerDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionStrongReferenceSet ) )
		return "AAFTypeID_DataDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_EssenceDataStrongReferenceSet ) )
		return "AAFTypeID_EssenceDataStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_InterpolationDefinitionStrongReferenceSet ) )
		return "AAFTypeID_InterpolationDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_MobStrongReferenceSet ) )
		return "AAFTypeID_MobStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionStrongReferenceSet ) )
		return "AAFTypeID_OperationDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionStrongReferenceSet ) )
		return "AAFTypeID_ParameterDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionStrongReferenceSet ) )
		return "AAFTypeID_PluginDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionStrongReferenceSet ) )
		return "AAFTypeID_PropertyDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionStrongReferenceSet ) )
		return "AAFTypeID_TypeDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_KLVDataDefinitionStrongReferenceSet ) )
		return "AAFTypeID_KLVDataDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueDefinitionStrongReferenceSet ) )
		return "AAFTypeID_TaggedValueDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveObjectStrongReferenceSet ) )
		return "AAFTypeID_DescriptiveObjectStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ComponentStrongReferenceVector ) )
		return "AAFTypeID_ComponentStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_ControlPointStrongReferenceVector ) )
		return "AAFTypeID_ControlPointStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_IdentificationStrongReferenceVector ) )
		return "AAFTypeID_IdentificationStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_LocatorStrongReferenceVector ) )
		return "AAFTypeID_LocatorStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_MobSlotStrongReferenceVector ) )
		return "AAFTypeID_MobSlotStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_SegmentStrongReferenceVector ) )
		return "AAFTypeID_SegmentStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_SourceReferenceStrongReferenceVector ) )
		return "AAFTypeID_SourceReferenceStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueStrongReferenceVector ) )
		return "AAFTypeID_TaggedValueStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_KLVDataStrongReferenceVector ) )
		return "AAFTypeID_KLVDataStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_ParameterStrongReferenceVector ) )
		return "AAFTypeID_ParameterStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_FileDescriptorStrongReferenceVector ) )
		return "AAFTypeID_FileDescriptorStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_RIFFChunkStrongReferenceVector ) )
		return "AAFTypeID_RIFFChunkStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveObjectStrongReferenceVector ) )
		return "AAFTypeID_DescriptiveObjectStrongReferenceVector";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	return "Unknown value";

}


const char * DataDefToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFDataDef_Picture ) )
		return "AAFDataDef_Picture";

	if ( auidCmp( auid, &AAFDataDef_LegacyPicture ) )
		return "AAFDataDef_LegacyPicture";

	if ( auidCmp( auid, &AAFDataDef_Matte ) )
		return "AAFDataDef_Matte";

	if ( auidCmp( auid, &AAFDataDef_PictureWithMatte ) )
		return "AAFDataDef_PictureWithMatte";

	if ( auidCmp( auid, &AAFDataDef_Sound ) )
		return "AAFDataDef_Sound";

	if ( auidCmp( auid, &AAFDataDef_LegacySound ) )
		return "AAFDataDef_LegacySound";

	if ( auidCmp( auid, &AAFDataDef_Timecode ) )
		return "AAFDataDef_Timecode";

	if ( auidCmp( auid, &AAFDataDef_LegacyTimecode ) )
		return "AAFDataDef_LegacyTimecode";

	if ( auidCmp( auid, &AAFDataDef_Edgecode ) )
		return "AAFDataDef_Edgecode";

	if ( auidCmp( auid, &AAFDataDef_DescriptiveMetadata ) )
		return "AAFDataDef_DescriptiveMetadata";

	if ( auidCmp( auid, &AAFDataDef_Auxiliary ) )
		return "AAFDataDef_Auxiliary";

	if ( auidCmp( auid, &AAFDataDef_Unknown ) )
		return "AAFDataDef_Unknown";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "AAFUID_NULL";



	static char TEXTDataDef[1024];

	aafObject *DataDefinitions = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_DataDefinitions );
	aafObject *DataDefinition  = NULL;

	aaf_foreach_ObjectInSet( &DataDefinition, DataDefinitions, NULL )
	{
		aafUID_t *DataDefIdent = aaf_get_propertyValue( DataDefinition, PID_DefinitionObject_Identification );

		if ( DataDefIdent && auidCmp( DataDefIdent, auid ) )
		{
			char *name = aaf_get_propertyValueText( DataDefinition, PID_DefinitionObject_Name );
			snprintf( TEXTDataDef, 1024, "%s", name );
			free( name );

			return TEXTDataDef;
		}
	}


	return "Unknown value";

}


const char * OperationDefToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFOperationDef_VideoDissolve ) )
		return "AAFOperationDef_VideoDissolve";

	if ( auidCmp( auid, &AAFOperationDef_SMPTEVideoWipe ) )
		return "AAFOperationDef_SMPTEVideoWipe";

	if ( auidCmp( auid, &AAFOperationDef_VideoSpeedControl ) )
		return "AAFOperationDef_VideoSpeedControl";

	if ( auidCmp( auid, &AAFOperationDef_VideoRepeat ) )
		return "AAFOperationDef_VideoRepeat";

	if ( auidCmp( auid, &AAFOperationDef_Flip ) )
		return "AAFOperationDef_Flip";

	if ( auidCmp( auid, &AAFOperationDef_Flop ) )
		return "AAFOperationDef_Flop";

	if ( auidCmp( auid, &AAFOperationDef_FlipFlop ) )
		return "AAFOperationDef_FlipFlop";

	if ( auidCmp( auid, &AAFOperationDef_VideoPosition ) )
		return "AAFOperationDef_VideoPosition";

	if ( auidCmp( auid, &AAFOperationDef_VideoCrop ) )
		return "AAFOperationDef_VideoCrop";

	if ( auidCmp( auid, &AAFOperationDef_VideoScale ) )
		return "AAFOperationDef_VideoScale";

	if ( auidCmp( auid, &AAFOperationDef_VideoRotate ) )
		return "AAFOperationDef_VideoRotate";

	if ( auidCmp( auid, &AAFOperationDef_VideoCornerPinning ) )
		return "AAFOperationDef_VideoCornerPinning";

	if ( auidCmp( auid, &AAFOperationDef_VideoAlphaWithinVideoKey ) )
		return "AAFOperationDef_VideoAlphaWithinVideoKey";

	if ( auidCmp( auid, &AAFOperationDef_VideoSeparateAlphaKey ) )
		return "AAFOperationDef_VideoSeparateAlphaKey";

	if ( auidCmp( auid, &AAFOperationDef_VideoLuminanceKey ) )
		return "AAFOperationDef_VideoLuminanceKey";

	if ( auidCmp( auid, &AAFOperationDef_VideoChromaKey ) )
		return "AAFOperationDef_VideoChromaKey";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioGain ) )
		return "AAFOperationDef_MonoAudioGain";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioPan ) )
		return "AAFOperationDef_MonoAudioPan";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioDissolve ) )
		return "AAFOperationDef_MonoAudioDissolve";

	if ( auidCmp( auid, &AAFOperationDef_TwoParameterMonoAudioDissolve ) )
		return "AAFOperationDef_TwoParameterMonoAudioDissolve";

	if ( auidCmp( auid, &AAFOperationDef_VideoOpacity ) )
		return "AAFOperationDef_VideoOpacity";

	if ( auidCmp( auid, &AAFOperationDef_VideoTitle ) )
		return "AAFOperationDef_VideoTitle";

	if ( auidCmp( auid, &AAFOperationDef_VideoColor ) )
		return "AAFOperationDef_VideoColor";

	if ( auidCmp( auid, &AAFOperationDef_Unknown ) )
		return "AAFOperationDef_Unknown";

	if ( auidCmp( auid, &AAFOperationDef_VideoFadeToBlack ) )
		return "AAFOperationDef_VideoFadeToBlack";

	if ( auidCmp( auid, &AAFOperationDef_PictureWithMate ) )
		return "AAFOperationDef_PictureWithMate";

	if ( auidCmp( auid, &AAFOperationDef_VideoFrameToMask ) )
		return "AAFOperationDef_VideoFrameToMask";

	if ( auidCmp( auid, &AAFOperationDef_StereoAudioDissolve ) )
		return "AAFOperationDef_StereoAudioDissolve";

	if ( auidCmp( auid, &AAFOperationDef_StereoAudioGain ) )
		return "AAFOperationDef_StereoAudioGain";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioMixdown ) )
		return "AAFOperationDef_MonoAudioMixdown";

	if ( auidCmp( auid, &AAFOperationDef_AudioChannelCombiner ) )
		return "AAFOperationDef_AudioChannelCombiner";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "AAFUID_NULL";



	static char TEXTOperationDef[1024];

	aafObject *OperationDefinitions = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_OperationDefinitions );
	aafObject *OperationDefinition  = NULL;

	aaf_foreach_ObjectInSet( &OperationDefinition, OperationDefinitions, NULL )
	{
		aafUID_t *OpDefIdent = aaf_get_propertyValue( OperationDefinition, PID_DefinitionObject_Identification );

		if ( OpDefIdent && auidCmp( OpDefIdent, auid ) )
		{
			char *name = aaf_get_propertyValueText( OperationDefinition, PID_DefinitionObject_Name );
			snprintf( TEXTOperationDef, 1024, "%s", name );
			free( name );

			return TEXTOperationDef;
		}
	}


	return "Unknown value";

}


const char * InterpolationToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFInterpolationDef_None ) )
		return "AAFInterpolationDef_None";

	if ( auidCmp( auid, &AAFInterpolationDef_Linear ) )
		return "AAFInterpolationDef_Linear";

	if ( auidCmp( auid, &AAFInterpolationDef_Constant ) )
		return "AAFInterpolationDef_Constant";

	if ( auidCmp( auid, &AAFInterpolationDef_BSpline ) )
		return "AAFInterpolationDef_BSpline";

	if ( auidCmp( auid, &AAFInterpolationDef_Log ) )
		return "AAFInterpolationDef_Log";

	if ( auidCmp( auid, &AAFInterpolationDef_Power ) )
		return "AAFInterpolationDef_Power";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	return "Unknown value";
}


const char * ParameterToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFParameterDef_Level ) )
		return "AAFParameterDef_Level";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEWipeNumber ) )
		return "AAFParameterDef_SMPTEWipeNumber";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEReverse ) )
		return "AAFParameterDef_SMPTEReverse";

	if ( auidCmp( auid, &AAFParameterDef_SpeedRatio ) )
		return "AAFParameterDef_SpeedRatio";

	if ( auidCmp( auid, &AAFParameterDef_PositionOffsetX ) )
		return "AAFParameterDef_PositionOffsetX";

	if ( auidCmp( auid, &AAFParameterDef_PositionOffsetY ) )
		return "AAFParameterDef_PositionOffsetY";

	if ( auidCmp( auid, &AAFParameterDef_CropLeft ) )
		return "AAFParameterDef_CropLeft";

	if ( auidCmp( auid, &AAFParameterDef_CropRight ) )
		return "AAFParameterDef_CropRight";

	if ( auidCmp( auid, &AAFParameterDef_CropTop ) )
		return "AAFParameterDef_CropTop";

	if ( auidCmp( auid, &AAFParameterDef_CropBottom ) )
		return "AAFParameterDef_CropBottom";

	if ( auidCmp( auid, &AAFParameterDef_ScaleX ) )
		return "AAFParameterDef_ScaleX";

	if ( auidCmp( auid, &AAFParameterDef_ScaleY ) )
		return "AAFParameterDef_ScaleY";

	if ( auidCmp( auid, &AAFParameterDef_Rotation ) )
		return "AAFParameterDef_Rotation";

	if ( auidCmp( auid, &AAFParameterDef_PinTopLeftX ) )
		return "AAFParameterDef_PinTopLeftX";

	if ( auidCmp( auid, &AAFParameterDef_PinTopLeftY ) )
		return "AAFParameterDef_PinTopLeftY";

	if ( auidCmp( auid, &AAFParameterDef_PinTopRightX ) )
		return "AAFParameterDef_PinTopRightX";

	if ( auidCmp( auid, &AAFParameterDef_PinTopRightY ) )
		return "AAFParameterDef_PinTopRightY";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomLeftX ) )
		return "AAFParameterDef_PinBottomLeftX";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomLeftY ) )
		return "AAFParameterDef_PinBottomLeftY";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomRightX ) )
		return "AAFParameterDef_PinBottomRightX";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomRightY ) )
		return "AAFParameterDef_PinBottomRightY";

	if ( auidCmp( auid, &AAFParameterDef_AlphaKeyInvertAlpha ) )
		return "AAFParameterDef_AlphaKeyInvertAlpha";

	if ( auidCmp( auid, &AAFParameterDef_LumKeyLevel ) )
		return "AAFParameterDef_LumKeyLevel";

	if ( auidCmp( auid, &AAFParameterDef_LumKeyClip ) )
		return "AAFParameterDef_LumKeyClip";

	if ( auidCmp( auid, &AAFParameterDef_Amplitude ) )
		return "AAFParameterDef_Amplitude";

	if ( auidCmp( auid, &AAFParameterDef_Pan ) )
		return "AAFParameterDef_Pan";

	if ( auidCmp( auid, &AAFParameterDef_OutgoingLevel ) )
		return "AAFParameterDef_OutgoingLevel";

	if ( auidCmp( auid, &AAFParameterDef_IncomingLevel ) )
		return "AAFParameterDef_IncomingLevel";

	if ( auidCmp( auid, &AAFParameterDef_OpacityLevel ) )
		return "AAFParameterDef_OpacityLevel";

	if ( auidCmp( auid, &AAFParameterDef_TitleText ) )
		return "AAFParameterDef_TitleText";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontName ) )
		return "AAFParameterDef_TitleFontName";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontSize ) )
		return "AAFParameterDef_TitleFontSize";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontColorR ) )
		return "AAFParameterDef_TitleFontColorR";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontColorG ) )
		return "AAFParameterDef_TitleFontColorG";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontColorB ) )
		return "AAFParameterDef_TitleFontColorB";

	if ( auidCmp( auid, &AAFParameterDef_TitleAlignment ) )
		return "AAFParameterDef_TitleAlignment";

	if ( auidCmp( auid, &AAFParameterDef_TitleBold ) )
		return "AAFParameterDef_TitleBold";

	if ( auidCmp( auid, &AAFParameterDef_TitleItalic ) )
		return "AAFParameterDef_TitleItalic";

	if ( auidCmp( auid, &AAFParameterDef_TitlePositionX ) )
		return "AAFParameterDef_TitlePositionX";

	if ( auidCmp( auid, &AAFParameterDef_TitlePositionY ) )
		return "AAFParameterDef_TitlePositionY";

	if ( auidCmp( auid, &AAFParameterDef_ColorSlopeR ) )
		return "AAFParameterDef_ColorSlopeR";

	if ( auidCmp( auid, &AAFParameterDef_ColorSlopeG ) )
		return "AAFParameterDef_ColorSlopeG";

	if ( auidCmp( auid, &AAFParameterDef_ColorSlopeB ) )
		return "AAFParameterDef_ColorSlopeB";

	if ( auidCmp( auid, &AAFParameterDef_ColorOffsetR ) )
		return "AAFParameterDef_ColorOffsetR";

	if ( auidCmp( auid, &AAFParameterDef_ColorOffsetG ) )
		return "AAFParameterDef_ColorOffsetG";

	if ( auidCmp( auid, &AAFParameterDef_ColorOffsetB ) )
		return "AAFParameterDef_ColorOffsetB";

	if ( auidCmp( auid, &AAFParameterDef_ColorPowerR ) )
		return "AAFParameterDef_ColorPowerR";

	if ( auidCmp( auid, &AAFParameterDef_ColorPowerG ) )
		return "AAFParameterDef_ColorPowerG";

	if ( auidCmp( auid, &AAFParameterDef_ColorPowerB ) )
		return "AAFParameterDef_ColorPowerB";

	if ( auidCmp( auid, &AAFParameterDef_ColorSaturation ) )
		return "AAFParameterDef_ColorSaturation";

	if ( auidCmp( auid, &AAFParameterDef_ColorCorrectionDescription ) )
		return "AAFParameterDef_ColorCorrectionDescription";

	if ( auidCmp( auid, &AAFParameterDef_ColorInputDescription ) )
		return "AAFParameterDef_ColorInputDescription";

	if ( auidCmp( auid, &AAFParameterDef_ColorViewingDescription ) )
		return "AAFParameterDef_ColorViewingDescription";

	if ( auidCmp( auid, &AAFParameterDef_SMPTESoft ) )
		return "AAFParameterDef_SMPTESoft";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEBorder ) )
		return "AAFParameterDef_SMPTEBorder";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEPosition ) )
		return "AAFParameterDef_SMPTEPosition";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEModulator ) )
		return "AAFParameterDef_SMPTEModulator";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEShadow ) )
		return "AAFParameterDef_SMPTEShadow";

	if ( auidCmp( auid, &AAFParameterDef_SMPTETumble ) )
		return "AAFParameterDef_SMPTETumble";

	if ( auidCmp( auid, &AAFParameterDef_SMPTESpotlight ) )
		return "AAFParameterDef_SMPTESpotlight";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEReplicationH ) )
		return "AAFParameterDef_SMPTEReplicationH";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEReplicationV ) )
		return "AAFParameterDef_SMPTEReplicationV";

	if ( auidCmp( auid, &AAFParameterDef_SMPTECheckerboard ) )
		return "AAFParameterDef_SMPTECheckerboard";

	if ( auidCmp( auid, &AAFParameterDef_PhaseOffset ) )
		return "AAFParameterDef_PhaseOffset";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "AAFUID_NULL";



	static char TEXTParameterDef[1024];

	aafObject *ParameterDefinitions = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_ParameterDefinitions );
	aafObject *ParameterDefinition  = NULL;

	aaf_foreach_ObjectInSet( &ParameterDefinition, ParameterDefinitions, NULL )
	{
		aafUID_t *ParamDefIdent = aaf_get_propertyValue( ParameterDefinition, PID_DefinitionObject_Identification );

		if ( ParamDefIdent && auidCmp( ParamDefIdent, auid ) )
		{
			char *name = aaf_get_propertyValueText( ParameterDefinition, PID_DefinitionObject_Name );
			snprintf( TEXTParameterDef, 1024, "%s", name );
			free( name );

			// printf("Description : %s\n", aaf_get_propertyValueText( ParameterDefinition, PID_DefinitionObject_Description ) );

			return TEXTParameterDef;
		}
	}


	return "Unknown value";
}


const char * TransferCharacteristicToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFTransferCharacteristic_ITU470_PAL ) )
		         return "AAFTransferCharacteristic_ITU470_PAL";

	if ( auidCmp( auid, &AAFTransferCharacteristic_ITU709 ) )
		         return "AAFTransferCharacteristic_ITU709";

	if ( auidCmp( auid, &AAFTransferCharacteristic_SMPTE240M ) )
		         return "AAFTransferCharacteristic_SMPTE240M";

	if ( auidCmp( auid, &AAFTransferCharacteristic_274M_296M ) )
		         return "AAFTransferCharacteristic_274M_296M";

	if ( auidCmp( auid, &AAFTransferCharacteristic_ITU1361 ) )
		         return "AAFTransferCharacteristic_ITU1361";

	if ( auidCmp( auid, &AAFTransferCharacteristic_linear ) )
		         return "AAFTransferCharacteristic_linear";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	return "Unknown value";
}


const char * CodingEquationsToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFCodingEquations_ITU601 ) )
		         return "AAFCodingEquations_ITU601";

	if ( auidCmp( auid, &AAFCodingEquations_ITU709 ) )
		         return "AAFCodingEquations_ITU709";

	if ( auidCmp( auid, &AAFCodingEquations_SMPTE240M ) )
		         return "AAFCodingEquations_SMPTE240M";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	return "Unknown value";
}


const char * ColorPrimariesToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFColorPrimaries_SMPTE170M ) )
		         return "AAFColorPrimaries_SMPTE170M";

	if ( auidCmp( auid, &AAFColorPrimaries_ITU470_PAL ) )
		         return "AAFColorPrimaries_ITU470_PAL";

	if ( auidCmp( auid, &AAFColorPrimaries_ITU709 ) )
		         return "AAFColorPrimaries_ITU709";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	return "Unknown value";
}


const char * UsageCodeToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFUsage_SubClip ) )
		         return "AAFUsage_SubClip";

	if ( auidCmp( auid, &AAFUsage_AdjustedClip ) )
		         return "AAFUsage_AdjustedClip";

	if ( auidCmp( auid, &AAFUsage_TopLevel ) )
		         return "AAFUsage_TopLevel";

	if ( auidCmp( auid, &AAFUsage_LowerLevel ) )
		         return "AAFUsage_LowerLevel";

	if ( auidCmp( auid, &AAFUsage_Template ) )
		         return "AAFUsage_Template";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return "";

	return "Unknown value";
}


const char * StoredFormToText( uint16_t sf )
{
	switch ( sf )
	{
		case SF_DATA:
	 return "SF_DATA";
		case SF_DATA_STREAM:
	 return "SF_DATA_STREAM";
		case SF_STRONG_OBJECT_REFERENCE:
	 return "SF_STRONG_OBJECT_REFERENCE";
		case SF_STRONG_OBJECT_REFERENCE_VECTOR:
	 return "SF_STRONG_OBJECT_REFERENCE_VECTOR";
		case SF_STRONG_OBJECT_REFERENCE_SET:
	 return "SF_STRONG_OBJECT_REFERENCE_SET";
		case SF_WEAK_OBJECT_REFERENCE:
	 return "SF_WEAK_OBJECT_REFERENCE";
		case SF_WEAK_OBJECT_REFERENCE_VECTOR:
	 return "SF_WEAK_OBJECT_REFERENCE_VECTOR";
		case SF_WEAK_OBJECT_REFERENCE_SET:
	 return "SF_WEAK_OBJECT_REFERENCE_SET";
		case SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID:
	 return "SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID";
		case SF_UNIQUE_OBJECT_ID:
	 return "SF_UNIQUE_OBJECT_ID";
		case SF_OPAQUE_STREAM:
	 return "SF_OPAQUE_STREAM";

		default:
			return "Unknown value";

	}
}


const char * PIDToText( AAF_Data *aafd, aafPID_t pid )
{
	switch( pid )
	{
		case PID_Root_MetaDictionary:
			return "PID_Root_MetaDictionary";
		case PID_Root_Header:
			return "PID_Root_Header";
		case PID_InterchangeObject_ObjClass:
			return "PID_InterchangeObject_ObjClass";
		case PID_InterchangeObject_Generation:
			return "PID_InterchangeObject_Generation";
		case PID_Component_DataDefinition:
			return "PID_Component_DataDefinition";
		case PID_Component_Length:
			return "PID_Component_Length";
		case PID_Component_KLVData:
			return "PID_Component_KLVData";
		case PID_Component_UserComments:
			return "PID_Component_UserComments";
		case PID_Component_Attributes:
			return "PID_Component_Attributes";
		case PID_EdgeCode_Start:
			return "PID_EdgeCode_Start";
		case PID_EdgeCode_FilmKind:
			return "PID_EdgeCode_FilmKind";
		case PID_EdgeCode_CodeFormat:
			return "PID_EdgeCode_CodeFormat";
		case PID_EdgeCode_Header:
			return "PID_EdgeCode_Header";
		case PID_EssenceGroup_Choices:
			return "PID_EssenceGroup_Choices";
		case PID_EssenceGroup_StillFrame:
			return "PID_EssenceGroup_StillFrame";
		case PID_Event_Position:
			return "PID_Event_Position";
		case PID_Event_Comment:
			return "PID_Event_Comment";
		case PID_GPITrigger_ActiveState:
			return "PID_GPITrigger_ActiveState";
		case PID_CommentMarker_Annotation:
			return "PID_CommentMarker_Annotation";
		case PID_OperationGroup_Operation:
			return "PID_OperationGroup_Operation";
		case PID_OperationGroup_InputSegments:
			return "PID_OperationGroup_InputSegments";
		case PID_OperationGroup_Parameters:
			return "PID_OperationGroup_Parameters";
		case PID_OperationGroup_BypassOverride:
			return "PID_OperationGroup_BypassOverride";
		case PID_OperationGroup_Rendering:
			return "PID_OperationGroup_Rendering";
		case PID_NestedScope_Slots:
			return "PID_NestedScope_Slots";
		case PID_Pulldown_InputSegment:
			return "PID_Pulldown_InputSegment";
		case PID_Pulldown_PulldownKind:
			return "PID_Pulldown_PulldownKind";
		case PID_Pulldown_PulldownDirection:
			return "PID_Pulldown_PulldownDirection";
		case PID_Pulldown_PhaseFrame:
			return "PID_Pulldown_PhaseFrame";
		case PID_ScopeReference_RelativeScope:
			return "PID_ScopeReference_RelativeScope";
		case PID_ScopeReference_RelativeSlot:
			return "PID_ScopeReference_RelativeSlot";
		case PID_Selector_Selected:
			return "PID_Selector_Selected";
		case PID_Selector_Alternates:
			return "PID_Selector_Alternates";
		case PID_Sequence_Components:
			return "PID_Sequence_Components";
		case PID_SourceReference_SourceID:
			return "PID_SourceReference_SourceID";
		case PID_SourceReference_SourceMobSlotID:
			return "PID_SourceReference_SourceMobSlotID";
		case PID_SourceReference_ChannelIDs:
			return "PID_SourceReference_ChannelIDs";
		case PID_SourceReference_MonoSourceSlotIDs:
			return "PID_SourceReference_MonoSourceSlotIDs";
		case PID_SourceClip_StartTime:
			return "PID_SourceClip_StartTime";
		case PID_SourceClip_FadeInLength:
			return "PID_SourceClip_FadeInLength";
		case PID_SourceClip_FadeInType:
			return "PID_SourceClip_FadeInType";
		case PID_SourceClip_FadeOutLength:
			return "PID_SourceClip_FadeOutLength";
		case PID_SourceClip_FadeOutType:
			return "PID_SourceClip_FadeOutType";
		case PID_HTMLClip_BeginAnchor:
			return "PID_HTMLClip_BeginAnchor";
		case PID_HTMLClip_EndAnchor:
			return "PID_HTMLClip_EndAnchor";
		case PID_Timecode_Start:
			return "PID_Timecode_Start";
		case PID_Timecode_FPS:
			return "PID_Timecode_FPS";
		case PID_Timecode_Drop:
			return "PID_Timecode_Drop";
		case PID_TimecodeStream_SampleRate:
			return "PID_TimecodeStream_SampleRate";
		case PID_TimecodeStream_Source:
			return "PID_TimecodeStream_Source";
		case PID_TimecodeStream_SourceType:
			return "PID_TimecodeStream_SourceType";
		case PID_TimecodeStream12M_IncludeSync:
			return "PID_TimecodeStream12M_IncludeSync";
		case PID_Transition_OperationGroup:
			return "PID_Transition_OperationGroup";
		case PID_Transition_CutPoint:
			return "PID_Transition_CutPoint";
		case PID_ContentStorage_Mobs:
			return "PID_ContentStorage_Mobs";
		case PID_ContentStorage_EssenceData:
			return "PID_ContentStorage_EssenceData";
		case PID_ControlPoint_Value:
			return "PID_ControlPoint_Value";
		case PID_ControlPoint_Time:
			return "PID_ControlPoint_Time";
		case PID_ControlPoint_EditHint:
			return "PID_ControlPoint_EditHint";
		case PID_DefinitionObject_Identification:
			return "PID_DefinitionObject_Identification";
		case PID_DefinitionObject_Name:
			return "PID_DefinitionObject_Name";
		case PID_DefinitionObject_Description:
			return "PID_DefinitionObject_Description";
		case PID_OperationDefinition_DataDefinition:
			return "PID_OperationDefinition_DataDefinition";
		case PID_OperationDefinition_IsTimeWarp:
			return "PID_OperationDefinition_IsTimeWarp";
		case PID_OperationDefinition_DegradeTo:
			return "PID_OperationDefinition_DegradeTo";
		case PID_OperationDefinition_OperationCategory:
			return "PID_OperationDefinition_OperationCategory";
		case PID_OperationDefinition_NumberInputs:
			return "PID_OperationDefinition_NumberInputs";
		case PID_OperationDefinition_Bypass:
			return "PID_OperationDefinition_Bypass";
		case PID_OperationDefinition_ParametersDefined:
			return "PID_OperationDefinition_ParametersDefined";
		case PID_ParameterDefinition_Type:
			return "PID_ParameterDefinition_Type";
		case PID_ParameterDefinition_DisplayUnits:
			return "PID_ParameterDefinition_DisplayUnits";
		case PID_PluginDefinition_PluginCategory:
			return "PID_PluginDefinition_PluginCategory";
		case PID_PluginDefinition_VersionNumber:
			return "PID_PluginDefinition_VersionNumber";
		case PID_PluginDefinition_VersionString:
			return "PID_PluginDefinition_VersionString";
		case PID_PluginDefinition_Manufacturer:
			return "PID_PluginDefinition_Manufacturer";
		case PID_PluginDefinition_ManufacturerInfo:
			return "PID_PluginDefinition_ManufacturerInfo";
		case PID_PluginDefinition_ManufacturerID:
			return "PID_PluginDefinition_ManufacturerID";
		case PID_PluginDefinition_Platform:
			return "PID_PluginDefinition_Platform";
		case PID_PluginDefinition_MinPlatformVersion:
			return "PID_PluginDefinition_MinPlatformVersion";
		case PID_PluginDefinition_MaxPlatformVersion:
			return "PID_PluginDefinition_MaxPlatformVersion";
		case PID_PluginDefinition_Engine:
			return "PID_PluginDefinition_Engine";
		case PID_PluginDefinition_MinEngineVersion:
			return "PID_PluginDefinition_MinEngineVersion";
		case PID_PluginDefinition_MaxEngineVersion:
			return "PID_PluginDefinition_MaxEngineVersion";
		case PID_PluginDefinition_PluginAPI:
			return "PID_PluginDefinition_PluginAPI";
		case PID_PluginDefinition_MinPluginAPI:
			return "PID_PluginDefinition_MinPluginAPI";
		case PID_PluginDefinition_MaxPluginAPI:
			return "PID_PluginDefinition_MaxPluginAPI";
		case PID_PluginDefinition_SoftwareOnly:
			return "PID_PluginDefinition_SoftwareOnly";
		case PID_PluginDefinition_Accelerator:
			return "PID_PluginDefinition_Accelerator";
		case PID_PluginDefinition_Locators:
			return "PID_PluginDefinition_Locators";
		case PID_PluginDefinition_Authentication:
			return "PID_PluginDefinition_Authentication";
		case PID_PluginDefinition_DefinitionObject:
			return "PID_PluginDefinition_DefinitionObject";
		case PID_CodecDefinition_FileDescriptorClass:
			return "PID_CodecDefinition_FileDescriptorClass";
		case PID_CodecDefinition_DataDefinitions:
			return "PID_CodecDefinition_DataDefinitions";
		case PID_ContainerDefinition_EssenceIsIdentified:
			return "PID_ContainerDefinition_EssenceIsIdentified";
		case PID_Dictionary_OperationDefinitions:
			return "PID_Dictionary_OperationDefinitions";
		case PID_Dictionary_ParameterDefinitions:
			return "PID_Dictionary_ParameterDefinitions";
		case PID_Dictionary_DataDefinitions:
			return "PID_Dictionary_DataDefinitions";
		case PID_Dictionary_PluginDefinitions:
			return "PID_Dictionary_PluginDefinitions";
		case PID_Dictionary_CodecDefinitions:
			return "PID_Dictionary_CodecDefinitions";
		case PID_Dictionary_ContainerDefinitions:
			return "PID_Dictionary_ContainerDefinitions";
		case PID_Dictionary_InterpolationDefinitions:
			return "PID_Dictionary_InterpolationDefinitions";
		case PID_Dictionary_KLVDataDefinitions:
			return "PID_Dictionary_KLVDataDefinitions";
		case PID_Dictionary_TaggedValueDefinitions:
			return "PID_Dictionary_TaggedValueDefinitions";
		case PID_EssenceData_MobID:
			return "PID_EssenceData_MobID";
		case PID_EssenceData_Data:
			return "PID_EssenceData_Data";
		case PID_EssenceData_SampleIndex:
			return "PID_EssenceData_SampleIndex";
		case PID_EssenceDescriptor_Locator:
			return "PID_EssenceDescriptor_Locator";
		case PID_FileDescriptor_SampleRate:
			return "PID_FileDescriptor_SampleRate";
		case PID_FileDescriptor_Length:
			return "PID_FileDescriptor_Length";
		case PID_FileDescriptor_ContainerFormat:
			return "PID_FileDescriptor_ContainerFormat";
		case PID_FileDescriptor_CodecDefinition:
			return "PID_FileDescriptor_CodecDefinition";
		case PID_FileDescriptor_LinkedSlotID:
			return "PID_FileDescriptor_LinkedSlotID";
		case PID_AIFCDescriptor_Summary:
			return "PID_AIFCDescriptor_Summary";
		case PID_DigitalImageDescriptor_Compression:
			return "PID_DigitalImageDescriptor_Compression";
		case PID_DigitalImageDescriptor_StoredHeight:
			return "PID_DigitalImageDescriptor_StoredHeight";
		case PID_DigitalImageDescriptor_StoredWidth:
			return "PID_DigitalImageDescriptor_StoredWidth";
		case PID_DigitalImageDescriptor_SampledHeight:
			return "PID_DigitalImageDescriptor_SampledHeight";
		case PID_DigitalImageDescriptor_SampledWidth:
			return "PID_DigitalImageDescriptor_SampledWidth";
		case PID_DigitalImageDescriptor_SampledXOffset:
			return "PID_DigitalImageDescriptor_SampledXOffset";
		case PID_DigitalImageDescriptor_SampledYOffset:
			return "PID_DigitalImageDescriptor_SampledYOffset";
		case PID_DigitalImageDescriptor_DisplayHeight:
			return "PID_DigitalImageDescriptor_DisplayHeight";
		case PID_DigitalImageDescriptor_DisplayWidth:
			return "PID_DigitalImageDescriptor_DisplayWidth";
		case PID_DigitalImageDescriptor_DisplayXOffset:
			return "PID_DigitalImageDescriptor_DisplayXOffset";
		case PID_DigitalImageDescriptor_DisplayYOffset:
			return "PID_DigitalImageDescriptor_DisplayYOffset";
		case PID_DigitalImageDescriptor_FrameLayout:
			return "PID_DigitalImageDescriptor_FrameLayout";
		case PID_DigitalImageDescriptor_VideoLineMap:
			return "PID_DigitalImageDescriptor_VideoLineMap";
		case PID_DigitalImageDescriptor_ImageAspectRatio:
			return "PID_DigitalImageDescriptor_ImageAspectRatio";
		case PID_DigitalImageDescriptor_AlphaTransparency:
			return "PID_DigitalImageDescriptor_AlphaTransparency";
		case PID_DigitalImageDescriptor_TransferCharacteristic:
			return "PID_DigitalImageDescriptor_TransferCharacteristic";
		case PID_DigitalImageDescriptor_ColorPrimaries:
			return "PID_DigitalImageDescriptor_ColorPrimaries";
		case PID_DigitalImageDescriptor_CodingEquations:
			return "PID_DigitalImageDescriptor_CodingEquations";
		case PID_DigitalImageDescriptor_ImageAlignmentFactor:
			return "PID_DigitalImageDescriptor_ImageAlignmentFactor";
		case PID_DigitalImageDescriptor_FieldDominance:
			return "PID_DigitalImageDescriptor_FieldDominance";
		case PID_DigitalImageDescriptor_FieldStartOffset:
			return "PID_DigitalImageDescriptor_FieldStartOffset";
		case PID_DigitalImageDescriptor_FieldEndOffset:
			return "PID_DigitalImageDescriptor_FieldEndOffset";
		case PID_DigitalImageDescriptor_SignalStandard:
			return "PID_DigitalImageDescriptor_SignalStandard";
		case PID_DigitalImageDescriptor_StoredF2Offset:
			return "PID_DigitalImageDescriptor_StoredF2Offset";
		case PID_DigitalImageDescriptor_DisplayF2Offset:
			return "PID_DigitalImageDescriptor_DisplayF2Offset";
		case PID_DigitalImageDescriptor_ActiveFormatDescriptor:
			return "PID_DigitalImageDescriptor_ActiveFormatDescriptor";
		case PID_CDCIDescriptor_ComponentWidth:
			return "PID_CDCIDescriptor_ComponentWidth";
		case PID_CDCIDescriptor_HorizontalSubsampling:
			return "PID_CDCIDescriptor_HorizontalSubsampling";
		case PID_CDCIDescriptor_ColorSiting:
			return "PID_CDCIDescriptor_ColorSiting";
		case PID_CDCIDescriptor_BlackReferenceLevel:
			return "PID_CDCIDescriptor_BlackReferenceLevel";
		case PID_CDCIDescriptor_WhiteReferenceLevel:
			return "PID_CDCIDescriptor_WhiteReferenceLevel";
		case PID_CDCIDescriptor_ColorRange:
			return "PID_CDCIDescriptor_ColorRange";
		case PID_CDCIDescriptor_PaddingBits:
			return "PID_CDCIDescriptor_PaddingBits";
		case PID_CDCIDescriptor_VerticalSubsampling:
			return "PID_CDCIDescriptor_VerticalSubsampling";
		case PID_CDCIDescriptor_AlphaSamplingWidth:
			return "PID_CDCIDescriptor_AlphaSamplingWidth";
		case PID_CDCIDescriptor_ReversedByteOrder:
			return "PID_CDCIDescriptor_ReversedByteOrder";
		case PID_RGBADescriptor_PixelLayout:
			return "PID_RGBADescriptor_PixelLayout";
		case PID_RGBADescriptor_Palette:
			return "PID_RGBADescriptor_Palette";
		case PID_RGBADescriptor_PaletteLayout:
			return "PID_RGBADescriptor_PaletteLayout";
		case PID_RGBADescriptor_ScanningDirection:
			return "PID_RGBADescriptor_ScanningDirection";
		case PID_RGBADescriptor_ComponentMaxRef:
			return "PID_RGBADescriptor_ComponentMaxRef";
		case PID_RGBADescriptor_ComponentMinRef:
			return "PID_RGBADescriptor_ComponentMinRef";
		case PID_RGBADescriptor_AlphaMaxRef:
			return "PID_RGBADescriptor_AlphaMaxRef";
		case PID_RGBADescriptor_AlphaMinRef:
			return "PID_RGBADescriptor_AlphaMinRef";
		case PID_TIFFDescriptor_IsUniform:
			return "PID_TIFFDescriptor_IsUniform";
		case PID_TIFFDescriptor_IsContiguous:
			return "PID_TIFFDescriptor_IsContiguous";
		case PID_TIFFDescriptor_LeadingLines:
			return "PID_TIFFDescriptor_LeadingLines";
		case PID_TIFFDescriptor_TrailingLines:
			return "PID_TIFFDescriptor_TrailingLines";
		case PID_TIFFDescriptor_JPEGTableID:
			return "PID_TIFFDescriptor_JPEGTableID";
		case PID_TIFFDescriptor_Summary:
			return "PID_TIFFDescriptor_Summary";
		case PID_WAVEDescriptor_Summary:
			return "PID_WAVEDescriptor_Summary";
		case PID_FilmDescriptor_FilmFormat:
			return "PID_FilmDescriptor_FilmFormat";
		case PID_FilmDescriptor_FrameRate:
			return "PID_FilmDescriptor_FrameRate";
		case PID_FilmDescriptor_PerforationsPerFrame:
			return "PID_FilmDescriptor_PerforationsPerFrame";
		case PID_FilmDescriptor_FilmAspectRatio:
			return "PID_FilmDescriptor_FilmAspectRatio";
		case PID_FilmDescriptor_Manufacturer:
			return "PID_FilmDescriptor_Manufacturer";
		case PID_FilmDescriptor_Model:
			return "PID_FilmDescriptor_Model";
		case PID_FilmDescriptor_FilmGaugeFormat:
			return "PID_FilmDescriptor_FilmGaugeFormat";
		case PID_FilmDescriptor_FilmBatchNumber:
			return "PID_FilmDescriptor_FilmBatchNumber";
		case PID_TapeDescriptor_FormFactor:
			return "PID_TapeDescriptor_FormFactor";
		case PID_TapeDescriptor_VideoSignal:
			return "PID_TapeDescriptor_VideoSignal";
		case PID_TapeDescriptor_TapeFormat:
			return "PID_TapeDescriptor_TapeFormat";
		case PID_TapeDescriptor_Length:
			return "PID_TapeDescriptor_Length";
		case PID_TapeDescriptor_ManufacturerID:
			return "PID_TapeDescriptor_ManufacturerID";
		case PID_TapeDescriptor_Model:
			return "PID_TapeDescriptor_Model";
		case PID_TapeDescriptor_TapeBatchNumber:
			return "PID_TapeDescriptor_TapeBatchNumber";
		case PID_TapeDescriptor_TapeStock:
			return "PID_TapeDescriptor_TapeStock";
		case PID_Header_ByteOrder:
			return "PID_Header_ByteOrder";
		case PID_Header_LastModified:
			return "PID_Header_LastModified";
		case PID_Header_Content:
			return "PID_Header_Content";
		case PID_Header_Dictionary:
			return "PID_Header_Dictionary";
		case PID_Header_Version:
			return "PID_Header_Version";
		case PID_Header_IdentificationList:
			return "PID_Header_IdentificationList";
		case PID_Header_ObjectModelVersion:
			return "PID_Header_ObjectModelVersion";
		case PID_Header_OperationalPattern:
			return "PID_Header_OperationalPattern";
		case PID_Header_EssenceContainers:
			return "PID_Header_EssenceContainers";
		case PID_Header_DescriptiveSchemes:
			return "PID_Header_DescriptiveSchemes";
		case PID_Identification_CompanyName:
			return "PID_Identification_CompanyName";
		case PID_Identification_ProductName:
			return "PID_Identification_ProductName";
		case PID_Identification_ProductVersion:
			return "PID_Identification_ProductVersion";
		case PID_Identification_ProductVersionString:
			return "PID_Identification_ProductVersionString";
		case PID_Identification_ProductID:
			return "PID_Identification_ProductID";
		case PID_Identification_Date:
			return "PID_Identification_Date";
		case PID_Identification_ToolkitVersion:
			return "PID_Identification_ToolkitVersion";
		case PID_Identification_Platform:
			return "PID_Identification_Platform";
		case PID_Identification_GenerationAUID:
			return "PID_Identification_GenerationAUID";
		case PID_NetworkLocator_URLString:
			return "PID_NetworkLocator_URLString";
		case PID_TextLocator_Name:
			return "PID_TextLocator_Name";
		case PID_Mob_MobID:
			return "PID_Mob_MobID";
		case PID_Mob_Name:
			return "PID_Mob_Name";
		case PID_Mob_Slots:
			return "PID_Mob_Slots";
		case PID_Mob_LastModified:
			return "PID_Mob_LastModified";
		case PID_Mob_CreationTime:
			return "PID_Mob_CreationTime";
		case PID_Mob_UserComments:
			return "PID_Mob_UserComments";
		case PID_Mob_KLVData:
			return "PID_Mob_KLVData";
		case PID_Mob_Attributes:
			return "PID_Mob_Attributes";
		case PID_Mob_UsageCode:
			return "PID_Mob_UsageCode";
		case PID_CompositionMob_DefaultFadeLength:
			return "PID_CompositionMob_DefaultFadeLength";
		case PID_CompositionMob_DefFadeType:
			return "PID_CompositionMob_DefFadeType";
		case PID_CompositionMob_DefFadeEditUnit:
			return "PID_CompositionMob_DefFadeEditUnit";
		case PID_CompositionMob_Rendering:
			return "PID_CompositionMob_Rendering";
		case PID_SourceMob_EssenceDescription:
			return "PID_SourceMob_EssenceDescription";
		case PID_MobSlot_SlotID:
			return "PID_MobSlot_SlotID";
		case PID_MobSlot_SlotName:
			return "PID_MobSlot_SlotName";
		case PID_MobSlot_Segment:
			return "PID_MobSlot_Segment";
		case PID_MobSlot_PhysicalTrackNumber:
			return "PID_MobSlot_PhysicalTrackNumber";
		case PID_EventMobSlot_EditRate:
			return "PID_EventMobSlot_EditRate";
		case PID_EventMobSlot_EventSlotOrigin:
			return "PID_EventMobSlot_EventSlotOrigin";
		case PID_TimelineMobSlot_EditRate:
			return "PID_TimelineMobSlot_EditRate";
		case PID_TimelineMobSlot_Origin:
			return "PID_TimelineMobSlot_Origin";
		case PID_TimelineMobSlot_MarkIn:
			return "PID_TimelineMobSlot_MarkIn";
		case PID_TimelineMobSlot_MarkOut:
			return "PID_TimelineMobSlot_MarkOut";
		case PID_TimelineMobSlot_UserPos:
			return "PID_TimelineMobSlot_UserPos";
		case PID_Parameter_Definition:
			return "PID_Parameter_Definition";
		case PID_ConstantValue_Value:
			return "PID_ConstantValue_Value";
		case PID_VaryingValue_Interpolation:
			return "PID_VaryingValue_Interpolation";
		case PID_VaryingValue_PointList:
			return "PID_VaryingValue_PointList";
		case PID_TaggedValue_Name:
			return "PID_TaggedValue_Name";
		case PID_TaggedValue_Value:
			return "PID_TaggedValue_Value";
		case PID_KLVData_Value:
			return "PID_KLVData_Value";
		case PID_DescriptiveMarker_DescribedSlots:
			return "PID_DescriptiveMarker_DescribedSlots";
		case PID_DescriptiveMarker_Description:
			return "PID_DescriptiveMarker_Description";
		case PID_SoundDescriptor_AudioSamplingRate:
			return "PID_SoundDescriptor_AudioSamplingRate";
		case PID_SoundDescriptor_Locked:
			return "PID_SoundDescriptor_Locked";
		case PID_SoundDescriptor_AudioRefLevel:
			return "PID_SoundDescriptor_AudioRefLevel";
		case PID_SoundDescriptor_ElectroSpatial:
			return "PID_SoundDescriptor_ElectroSpatial";
		case PID_SoundDescriptor_Channels:
			return "PID_SoundDescriptor_Channels";
		case PID_SoundDescriptor_QuantizationBits:
			return "PID_SoundDescriptor_QuantizationBits";
		case PID_SoundDescriptor_DialNorm:
			return "PID_SoundDescriptor_DialNorm";
		case PID_SoundDescriptor_Compression:
			return "PID_SoundDescriptor_Compression";
		case PID_DataEssenceDescriptor_DataEssenceCoding:
			return "PID_DataEssenceDescriptor_DataEssenceCoding";
		case PID_MultipleDescriptor_FileDescriptors:
			return "PID_MultipleDescriptor_FileDescriptors";
		case PID_DescriptiveClip_DescribedSlotIDs:
			return "PID_DescriptiveClip_DescribedSlotIDs";
		case PID_AES3PCMDescriptor_Emphasis:
			return "PID_AES3PCMDescriptor_Emphasis";
		case PID_AES3PCMDescriptor_BlockStartOffset:
			return "PID_AES3PCMDescriptor_BlockStartOffset";
		case PID_AES3PCMDescriptor_AuxBitsMode:
			return "PID_AES3PCMDescriptor_AuxBitsMode";
		case PID_AES3PCMDescriptor_ChannelStatusMode:
			return "PID_AES3PCMDescriptor_ChannelStatusMode";
		case PID_AES3PCMDescriptor_FixedChannelStatusData:
			return "PID_AES3PCMDescriptor_FixedChannelStatusData";
		case PID_AES3PCMDescriptor_UserDataMode:
			return "PID_AES3PCMDescriptor_UserDataMode";
		case PID_AES3PCMDescriptor_FixedUserData:
			return "PID_AES3PCMDescriptor_FixedUserData";
		case PID_PCMDescriptor_BlockAlign:
			return "PID_PCMDescriptor_BlockAlign";
		case PID_PCMDescriptor_SequenceOffset:
			return "PID_PCMDescriptor_SequenceOffset";
		case PID_PCMDescriptor_AverageBPS:
			return "PID_PCMDescriptor_AverageBPS";
		case PID_PCMDescriptor_ChannelAssignment:
			return "PID_PCMDescriptor_ChannelAssignment";
		case PID_PCMDescriptor_PeakEnvelopeVersion:
			return "PID_PCMDescriptor_PeakEnvelopeVersion";
		case PID_PCMDescriptor_PeakEnvelopeFormat:
			return "PID_PCMDescriptor_PeakEnvelopeFormat";
		case PID_PCMDescriptor_PointsPerPeakValue:
			return "PID_PCMDescriptor_PointsPerPeakValue";
		case PID_PCMDescriptor_PeakEnvelopeBlockSize:
			return "PID_PCMDescriptor_PeakEnvelopeBlockSize";
		case PID_PCMDescriptor_PeakChannels:
			return "PID_PCMDescriptor_PeakChannels";
		case PID_PCMDescriptor_PeakFrames:
			return "PID_PCMDescriptor_PeakFrames";
		case PID_PCMDescriptor_PeakOfPeaksPosition:
			return "PID_PCMDescriptor_PeakOfPeaksPosition";
		case PID_PCMDescriptor_PeakEnvelopeTimestamp:
			return "PID_PCMDescriptor_PeakEnvelopeTimestamp";
		case PID_PCMDescriptor_PeakEnvelopeData:
			return "PID_PCMDescriptor_PeakEnvelopeData";
		case PID_KLVDataDefinition_KLVDataType:
			return "PID_KLVDataDefinition_KLVDataType";
		case PID_AuxiliaryDescriptor_MimeType:
			return "PID_AuxiliaryDescriptor_MimeType";
		case PID_AuxiliaryDescriptor_CharSet:
			return "PID_AuxiliaryDescriptor_CharSet";
		case PID_RIFFChunk_ChunkID:
			return "PID_RIFFChunk_ChunkID";
		case PID_RIFFChunk_ChunkLength:
			return "PID_RIFFChunk_ChunkLength";
		case PID_RIFFChunk_ChunkData:
			return "PID_RIFFChunk_ChunkData";
		case PID_BWFImportDescriptor_QltyFileSecurityReport:
			return "PID_BWFImportDescriptor_QltyFileSecurityReport";
		case PID_BWFImportDescriptor_QltyFileSecurityWave:
			return "PID_BWFImportDescriptor_QltyFileSecurityWave";
		case PID_BWFImportDescriptor_BextCodingHistory:
			return "PID_BWFImportDescriptor_BextCodingHistory";
		case PID_BWFImportDescriptor_QltyBasicData:
			return "PID_BWFImportDescriptor_QltyBasicData";
		case PID_BWFImportDescriptor_QltyStartOfModulation:
			return "PID_BWFImportDescriptor_QltyStartOfModulation";
		case PID_BWFImportDescriptor_QltyQualityEvent:
			return "PID_BWFImportDescriptor_QltyQualityEvent";
		case PID_BWFImportDescriptor_QltyEndOfModulation:
			return "PID_BWFImportDescriptor_QltyEndOfModulation";
		case PID_BWFImportDescriptor_QltyQualityParameter:
			return "PID_BWFImportDescriptor_QltyQualityParameter";
		case PID_BWFImportDescriptor_QltyOperatorComment:
			return "PID_BWFImportDescriptor_QltyOperatorComment";
		case PID_BWFImportDescriptor_QltyCueSheet:
			return "PID_BWFImportDescriptor_QltyCueSheet";
		case PID_BWFImportDescriptor_UnknownBWFChunks:
			return "PID_BWFImportDescriptor_UnknownBWFChunks";

		case 0x0000:

		// case PID_MPEGVideoDescriptor_SingleSequence:
		// case PID_MPEGVideoDescriptor_ConstantBPictureCount:
		// case PID_MPEGVideoDescriptor_CodedContentScanning:
		// case PID_MPEGVideoDescriptor_LowDelay:
		// case PID_MPEGVideoDescriptor_ClosedGOP:
		// case PID_MPEGVideoDescriptor_IdenticalGOP:
		// case PID_MPEGVideoDescriptor_MaxGOP:
		// case PID_MPEGVideoDescriptor_MaxBPictureCount:
		// case PID_MPEGVideoDescriptor_BitRate:
		// case PID_MPEGVideoDescriptor_ProfileAndLevel:

			return "MULTIPLE_VALUE_MATCHES : see libaaf.h";

			// return "PID_MPEGVideoDescriptor_SingleSequence";
			// return "PID_MPEGVideoDescriptor_ConstantBPictureCount";
			// return "PID_MPEGVideoDescriptor_CodedContentScanning";
			// return "PID_MPEGVideoDescriptor_LowDelay";
			// return "PID_MPEGVideoDescriptor_ClosedGOP";
			// return "PID_MPEGVideoDescriptor_IdenticalGOP";
			// return "PID_MPEGVideoDescriptor_MaxGOP";
			// return "PID_MPEGVideoDescriptor_MaxBPictureCount";
			// return "PID_MPEGVideoDescriptor_BitRate";
			// return "PID_MPEGVideoDescriptor_ProfileAndLevel";


		case PID_ClassDefinition_ParentClass:
			return "PID_ClassDefinition_ParentClass";
		case PID_ClassDefinition_Properties:
			return "PID_ClassDefinition_Properties";
		case PID_ClassDefinition_IsConcrete:
			return "PID_ClassDefinition_IsConcrete";
		case PID_PropertyDefinition_Type:
			return "PID_PropertyDefinition_Type";
		case PID_PropertyDefinition_IsOptional:
			return "PID_PropertyDefinition_IsOptional";
		case PID_PropertyDefinition_LocalIdentification:
			return "PID_PropertyDefinition_LocalIdentification";
		case PID_PropertyDefinition_IsUniqueIdentifier:
			return "PID_PropertyDefinition_IsUniqueIdentifier";
		case PID_TypeDefinitionInteger_Size:
			return "PID_TypeDefinitionInteger_Size";
		case PID_TypeDefinitionInteger_IsSigned:
			return "PID_TypeDefinitionInteger_IsSigned";
		case PID_TypeDefinitionStrongObjectReference_ReferencedType:
			return "PID_TypeDefinitionStrongObjectReference_ReferencedType";
		case PID_TypeDefinitionWeakObjectReference_ReferencedType:
			return "PID_TypeDefinitionWeakObjectReference_ReferencedType";
		case PID_TypeDefinitionWeakObjectReference_TargetSet:
			return "PID_TypeDefinitionWeakObjectReference_TargetSet";
		case PID_TypeDefinitionEnumeration_ElementType:
			return "PID_TypeDefinitionEnumeration_ElementType";
		case PID_TypeDefinitionEnumeration_ElementNames:
			return "PID_TypeDefinitionEnumeration_ElementNames";
		case PID_TypeDefinitionEnumeration_ElementValues:
			return "PID_TypeDefinitionEnumeration_ElementValues";
		case PID_TypeDefinitionFixedArray_ElementType:
			return "PID_TypeDefinitionFixedArray_ElementType";
		case PID_TypeDefinitionFixedArray_ElementCount:
			return "PID_TypeDefinitionFixedArray_ElementCount";
		case PID_TypeDefinitionVariableArray_ElementType:
			return "PID_TypeDefinitionVariableArray_ElementType";
		case PID_TypeDefinitionSet_ElementType:
			return "PID_TypeDefinitionSet_ElementType";
		case PID_TypeDefinitionString_ElementType:
			return "PID_TypeDefinitionString_ElementType";
		case PID_TypeDefinitionRecord_MemberTypes:
			return "PID_TypeDefinitionRecord_MemberTypes";
		case PID_TypeDefinitionRecord_MemberNames:
			return "PID_TypeDefinitionRecord_MemberNames";
		case PID_TypeDefinitionRename_RenamedType:
			return "PID_TypeDefinitionRename_RenamedType";
		case PID_TypeDefinitionExtendibleEnumeration_ElementNames:
			return "PID_TypeDefinitionExtendibleEnumeration_ElementNames";
		case PID_TypeDefinitionExtendibleEnumeration_ElementValues:
			return "PID_TypeDefinitionExtendibleEnumeration_ElementValues";
		case PID_MetaDefinition_Identification:
			return "PID_MetaDefinition_Identification";
		case PID_MetaDefinition_Name:
			return "PID_MetaDefinition_Name";
		case PID_MetaDefinition_Description:
			return "PID_MetaDefinition_Description";
		case PID_MetaDictionary_ClassDefinitions:
			return "PID_MetaDictionary_ClassDefinitions";
		case PID_MetaDictionary_TypeDefinitions:
			return "PID_MetaDictionary_TypeDefinitions";

		// default:
		// 	return "Unknown value";
	}


	static char PIDText[1024];

	aafClass *Class = NULL;


	foreachClass( Class, aafd->Classes )
	{
		aafPropertyDef *PDef = NULL;

		foreachPropertyDefinition( PDef, Class->Properties )
		{
			if ( PDef->pid == pid )
			{
				snprintf( PIDText, 1024, "%s%s%s",
			 		(PDef->meta) ? ANSI_COLOR_YELLOW : "",
				 	 PDef->name,
				 	(PDef->meta) ? ANSI_COLOR_RESET : "" );

				return PIDText;
			}
		}

	}

	return "Unknown value";
}


const char * ClassIDToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFClassID_Root ) )
		return "AAFClassID_Root";
	else if ( auidCmp( auid, &AAFClassID_InterchangeObject ) )
		return "AAFClassID_InterchangeObject";
	else if ( auidCmp( auid, &AAFClassID_Component ) )
		return "AAFClassID_Component";
	else if ( auidCmp( auid, &AAFClassID_Segment ) )
		return "AAFClassID_Segment";
	else if ( auidCmp( auid, &AAFClassID_EdgeCode ) )
		return "AAFClassID_EdgeCode";
	else if ( auidCmp( auid, &AAFClassID_EssenceGroup ) )
		return "AAFClassID_EssenceGroup";
	else if ( auidCmp( auid, &AAFClassID_Event ) )
		return "AAFClassID_Event";
	else if ( auidCmp( auid, &AAFClassID_GPITrigger ) )
		return "AAFClassID_GPITrigger";
	else if ( auidCmp( auid, &AAFClassID_CommentMarker ) )
		return "AAFClassID_CommentMarker";
	else if ( auidCmp( auid, &AAFClassID_Filler ) )
		return "AAFClassID_Filler";
	else if ( auidCmp( auid, &AAFClassID_OperationGroup ) )
		return "AAFClassID_OperationGroup";
	else if ( auidCmp( auid, &AAFClassID_NestedScope ) )
		return "AAFClassID_NestedScope";
	else if ( auidCmp( auid, &AAFClassID_Pulldown ) )
		return "AAFClassID_Pulldown";
	else if ( auidCmp( auid, &AAFClassID_ScopeReference ) )
		return "AAFClassID_ScopeReference";
	else if ( auidCmp( auid, &AAFClassID_Selector ) )
		return "AAFClassID_Selector";
	else if ( auidCmp( auid, &AAFClassID_Sequence ) )
		return "AAFClassID_Sequence";
	else if ( auidCmp( auid, &AAFClassID_SourceReference ) )
		return "AAFClassID_SourceReference";
	else if ( auidCmp( auid, &AAFClassID_SourceClip ) )
		return "AAFClassID_SourceClip";
	else if ( auidCmp( auid, &AAFClassID_TextClip ) )
		return "AAFClassID_TextClip";
	else if ( auidCmp( auid, &AAFClassID_HTMLClip ) )
		return "AAFClassID_HTMLClip";
	else if ( auidCmp( auid, &AAFClassID_Timecode ) )
		return "AAFClassID_Timecode";
	else if ( auidCmp( auid, &AAFClassID_TimecodeStream ) )
		return "AAFClassID_TimecodeStream";
	else if ( auidCmp( auid, &AAFClassID_TimecodeStream12M ) )
		return "AAFClassID_TimecodeStream12M";
	else if ( auidCmp( auid, &AAFClassID_Transition ) )
		return "AAFClassID_Transition";
	else if ( auidCmp( auid, &AAFClassID_ContentStorage ) )
		return "AAFClassID_ContentStorage";
	else if ( auidCmp( auid, &AAFClassID_ControlPoint ) )
		return "AAFClassID_ControlPoint";
	else if ( auidCmp( auid, &AAFClassID_DefinitionObject ) )
		return "AAFClassID_DefinitionObject";
	else if ( auidCmp( auid, &AAFClassID_DataDefinition ) )
		return "AAFClassID_DataDefinition";
	else if ( auidCmp( auid, &AAFClassID_OperationDefinition ) )
		return "AAFClassID_OperationDefinition";
	else if ( auidCmp( auid, &AAFClassID_ParameterDefinition ) )
		return "AAFClassID_ParameterDefinition";
	else if ( auidCmp( auid, &AAFClassID_PluginDefinition ) )
		return "AAFClassID_PluginDefinition";
	else if ( auidCmp( auid, &AAFClassID_CodecDefinition ) )
		return "AAFClassID_CodecDefinition";
	else if ( auidCmp( auid, &AAFClassID_ContainerDefinition ) )
		return "AAFClassID_ContainerDefinition";
	else if ( auidCmp( auid, &AAFClassID_InterpolationDefinition ) )
		return "AAFClassID_InterpolationDefinition";
	else if ( auidCmp( auid, &AAFClassID_Dictionary ) )
		return "AAFClassID_Dictionary";
	else if ( auidCmp( auid, &AAFClassID_EssenceData ) )
		return "AAFClassID_EssenceData";
	else if ( auidCmp( auid, &AAFClassID_EssenceDescriptor ) )
		return "AAFClassID_EssenceDescriptor";
	else if ( auidCmp( auid, &AAFClassID_FileDescriptor ) )
		return "AAFClassID_FileDescriptor";
	else if ( auidCmp( auid, &AAFClassID_AIFCDescriptor ) )
		return "AAFClassID_AIFCDescriptor";
	else if ( auidCmp( auid, &AAFClassID_DigitalImageDescriptor ) )
		return "AAFClassID_DigitalImageDescriptor";
	else if ( auidCmp( auid, &AAFClassID_CDCIDescriptor ) )
		return "AAFClassID_CDCIDescriptor";
	else if ( auidCmp( auid, &AAFClassID_RGBADescriptor ) )
		return "AAFClassID_RGBADescriptor";
	else if ( auidCmp( auid, &AAFClassID_HTMLDescriptor ) )
		return "AAFClassID_HTMLDescriptor";
	else if ( auidCmp( auid, &AAFClassID_TIFFDescriptor ) )
		return "AAFClassID_TIFFDescriptor";
	else if ( auidCmp( auid, &AAFClassID_WAVEDescriptor ) )
		return "AAFClassID_WAVEDescriptor";
	else if ( auidCmp( auid, &AAFClassID_FilmDescriptor ) )
		return "AAFClassID_FilmDescriptor";
	else if ( auidCmp( auid, &AAFClassID_TapeDescriptor ) )
		return "AAFClassID_TapeDescriptor";
	else if ( auidCmp( auid, &AAFClassID_Header ) )
		return "AAFClassID_Header";
	else if ( auidCmp( auid, &AAFClassID_Identification ) )
		return "AAFClassID_Identification";
	else if ( auidCmp( auid, &AAFClassID_Locator ) )
		return "AAFClassID_Locator";
	else if ( auidCmp( auid, &AAFClassID_NetworkLocator ) )
		return "AAFClassID_NetworkLocator";
	else if ( auidCmp( auid, &AAFClassID_TextLocator ) )
		return "AAFClassID_TextLocator";
	else if ( auidCmp( auid, &AAFClassID_Mob ) )
		return "AAFClassID_Mob";
	else if ( auidCmp( auid, &AAFClassID_CompositionMob ) )
		return "AAFClassID_CompositionMob";
	else if ( auidCmp( auid, &AAFClassID_MasterMob ) )
		return "AAFClassID_MasterMob";
	else if ( auidCmp( auid, &AAFClassID_SourceMob ) )
		return "AAFClassID_SourceMob";
	else if ( auidCmp( auid, &AAFClassID_MobSlot ) )
		return "AAFClassID_MobSlot";
	else if ( auidCmp( auid, &AAFClassID_EventMobSlot ) )
		return "AAFClassID_EventMobSlot";
	else if ( auidCmp( auid, &AAFClassID_StaticMobSlot ) )
		return "AAFClassID_StaticMobSlot";
	else if ( auidCmp( auid, &AAFClassID_TimelineMobSlot ) )
		return "AAFClassID_TimelineMobSlot";
	else if ( auidCmp( auid, &AAFClassID_Parameter ) )
		return "AAFClassID_Parameter";
	else if ( auidCmp( auid, &AAFClassID_ConstantValue ) )
		return "AAFClassID_ConstantValue";
	else if ( auidCmp( auid, &AAFClassID_VaryingValue ) )
		return "AAFClassID_VaryingValue";
	else if ( auidCmp( auid, &AAFClassID_TaggedValue ) )
		return "AAFClassID_TaggedValue";
	else if ( auidCmp( auid, &AAFClassID_KLVData ) )
		return "AAFClassID_KLVData";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveMarker ) )
		return "AAFClassID_DescriptiveMarker";
	else if ( auidCmp( auid, &AAFClassID_SoundDescriptor ) )
		return "AAFClassID_SoundDescriptor";
	else if ( auidCmp( auid, &AAFClassID_DataEssenceDescriptor ) )
		return "AAFClassID_DataEssenceDescriptor";
	else if ( auidCmp( auid, &AAFClassID_MultipleDescriptor ) )
		return "AAFClassID_MultipleDescriptor";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveClip ) )
		return "AAFClassID_DescriptiveClip";
	else if ( auidCmp( auid, &AAFClassID_AES3PCMDescriptor ) )
		return "AAFClassID_AES3PCMDescriptor";
	else if ( auidCmp( auid, &AAFClassID_PCMDescriptor ) )
		return "AAFClassID_PCMDescriptor";
	else if ( auidCmp( auid, &AAFClassID_PhysicalDescriptor ) )
		return "AAFClassID_PhysicalDescriptor";
	else if ( auidCmp( auid, &AAFClassID_ImportDescriptor ) )
		return "AAFClassID_ImportDescriptor";
	else if ( auidCmp( auid, &AAFClassID_RecordingDescriptor ) )
		return "AAFClassID_RecordingDescriptor";
	else if ( auidCmp( auid, &AAFClassID_TaggedValueDefinition ) )
		return "AAFClassID_TaggedValueDefinition";
	else if ( auidCmp( auid, &AAFClassID_KLVDataDefinition ) )
		return "AAFClassID_KLVDataDefinition";
	else if ( auidCmp( auid, &AAFClassID_AuxiliaryDescriptor ) )
		return "AAFClassID_AuxiliaryDescriptor";
	else if ( auidCmp( auid, &AAFClassID_RIFFChunk ) )
		return "AAFClassID_RIFFChunk";
	else if ( auidCmp( auid, &AAFClassID_BWFImportDescriptor ) )
		return "AAFClassID_BWFImportDescriptor";
	else if ( auidCmp( auid, &AAFClassID_MPEGVideoDescriptor ) )
		return "AAFClassID_MPEGVideoDescriptor";
	else if ( auidCmp( auid, &AAFClassID_ClassDefinition ) )
		return "AAFClassID_ClassDefinition";
	else if ( auidCmp( auid, &AAFClassID_PropertyDefinition ) )
		return "AAFClassID_PropertyDefinition";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinition ) )
		return "AAFClassID_TypeDefinition";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionInteger ) )
		return "AAFClassID_TypeDefinitionInteger";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionStrongObjectReference ) )
		return "AAFClassID_TypeDefinitionStrongObjectReference";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionWeakObjectReference ) )
		return "AAFClassID_TypeDefinitionWeakObjectReference";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionEnumeration ) )
		return "AAFClassID_TypeDefinitionEnumeration";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionFixedArray ) )
		return "AAFClassID_TypeDefinitionFixedArray";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionVariableArray ) )
		return "AAFClassID_TypeDefinitionVariableArray";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionSet ) )
		return "AAFClassID_TypeDefinitionSet";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionString ) )
		return "AAFClassID_TypeDefinitionString";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionStream ) )
		return "AAFClassID_TypeDefinitionStream";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionRecord ) )
		return "AAFClassID_TypeDefinitionRecord";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionRename ) )
		return "AAFClassID_TypeDefinitionRename";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionExtendibleEnumeration ) )
		return "AAFClassID_TypeDefinitionExtendibleEnumeration";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionIndirect ) )
		return "AAFClassID_TypeDefinitionIndirect";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionOpaque ) )
		return "AAFClassID_TypeDefinitionOpaque";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionCharacter ) )
		return "AAFClassID_TypeDefinitionCharacter";
	else if ( auidCmp( auid, &AAFClassID_MetaDefinition ) )
		return "AAFClassID_MetaDefinition";
	else if ( auidCmp( auid, &AAFClassID_MetaDictionary ) )
		return "AAFClassID_MetaDictionary";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveObject ) )
		return "AAFClassID_DescriptiveObject";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveFramework ) )
		return "AAFClassID_DescriptiveFramework";


	if ( auidCmp( auid, &AUID_NULL ) )
		return "AUID_NULL";



	static char ClassIDText[1024];

	ClassIDText[0] = '\0';

	aafClass *Class = NULL;

	foreachClass( Class, aafd->Classes )
	{

		if ( auidCmp( Class->ID, auid ) )
		{
			snprintf( ClassIDText, 1024, "%s%s%s",
		 		(Class->meta) ? ANSI_COLOR_YELLOW : "",
			 	 Class->name,
			 	(Class->meta) ? ANSI_COLOR_RESET : "" );

			return ClassIDText;
		}

	}

	return "Unknown value";
}


const char * ContainerToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFContainerDef_External ) )
		return "AAFContainerDef_External";

	if ( auidCmp( auid, &AAFContainerDef_OMF ) )
		return "AAFContainerDef_OMF";

	if ( auidCmp( auid, &AAFContainerDef_AAF ) )
		return "AAFContainerDef_AAF";

	if ( auidCmp( auid, &AAFContainerDef_AAFMSS ) )
		return "AAFContainerDef_AAFMSS";

	if ( auidCmp( auid, &AAFContainerDef_AAFKLV ) )
		return "AAFContainerDef_AAFKLV";

	if ( auidCmp( auid, &AAFContainerDef_AAFXML ) )
		return "AAFContainerDef_AAFXML";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_DefinedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_ExtendedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_PictureOnly ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_DefinedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_ExtendedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_PictureOnly ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_DefinedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_ExtendedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_PictureOnly ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_DefinedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_ExtendedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_PictureOnly ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_DefinedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_ExtendedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_PictureOnly ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_DefinedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_ExtendedTemplate ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_PictureOnly ) )
		return "AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps_SMPTE322M ) )
		return "AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps_SMPTE322M ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps_SMPTE322M ) )
		return "AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps_SMPTE322M ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_UndefinedSource_25Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_IECDV_UndefinedSource_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_UndefinedSource_25Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_IECDV_UndefinedSource_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_25Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_50Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_50Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_50Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_50Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x5994I_100Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x5994I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x5994I_100Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x5994I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x50I_100Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x50I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x50I_100Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x50I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_720x5994P_100Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_720x5994P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x5994P_100Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x5994P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_720x50P_100Mbps ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_720x50P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x50P_100Mbps ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x50P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_UndefinedSource ) )
		return "AAFContainerDef_MXFGC_Framewrapped_DVbased_UndefinedSource";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_UndefinedSource ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_DVbased_UndefinedSource";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_MPEGES_VideoStream0_SID ) )
		return "AAFContainerDef_MXFGC_Framewrapped_MPEGES_VideoStream0_SID";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_CustomClosedGOPwrapped_MPEGES_VideoStream1_SID ) )
		return "AAFContainerDef_MXFGC_CustomClosedGOPwrapped_MPEGES_VideoStream1_SID";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994I_720_422 ) )
		return "AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994I_720_422 ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994I_720_422 ) )
		return "AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50I_720_422 ) )
		return "AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50I_720_422 ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50I_720_422 ) )
		return "AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994P_960_422 ) )
		return "AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994P_960_422 ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994P_960_422 ) )
		return "AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50P_960_422 ) )
		return "AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50P_960_422 ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50P_960_422 ) )
		return "AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Broadcast_Wave_audio_data ) )
		return "AAFContainerDef_MXFGC_Framewrapped_Broadcast_Wave_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Broadcast_Wave_audio_data ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_Broadcast_Wave_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_AES3_audio_data ) )
		return "AAFContainerDef_MXFGC_Framewrapped_AES3_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_AES3_audio_data ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_AES3_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Alaw_Audio ) )
		return "AAFContainerDef_MXFGC_Framewrapped_Alaw_Audio";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Alaw_Audio ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_Alaw_Audio";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Customwrapped_Alaw_Audio ) )
		return "AAFContainerDef_MXFGC_Customwrapped_Alaw_Audio";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_AVCbytestream_VideoStream0_SID ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_AVCbytestream_VideoStream0_SID";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_VC3 ) )
		return "AAFContainerDef_MXFGC_Framewrapped_VC3";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_VC3 ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_VC3";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_VC1 ) )
		return "AAFContainerDef_MXFGC_Framewrapped_VC1";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_VC1 ) )
		return "AAFContainerDef_MXFGC_Clipwrapped_VC1";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Generic_Essence_Multiple_Mappings ) )
		return "AAFContainerDef_MXFGC_Generic_Essence_Multiple_Mappings";

	if ( auidCmp( auid, &AAFContainerDef_RIFFWAVE ) )
		return "AAFContainerDef_RIFFWAVE";

	if ( auidCmp( auid, &AAFContainerDef_JFIF ) )
		return "AAFContainerDef_JFIF";

	if ( auidCmp( auid, &AAFContainerDef_AIFFAIFC ) )
		return "AAFContainerDef_AIFFAIFC";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220X_1080p ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_220X_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_1080p ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_145_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220_1080p ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_220_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_36_1080p ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_36_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220X_1080i ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_220X_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_1080i ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_145_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220_1080i ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_220_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_1440_1080i ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_145_1440_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220X_720p ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_220X_720p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220_720p ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_220_720p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_720p ) )
		return "AAFContainerDef_MXFGC_Avid_DNX_145_720p";


	return "Unknown value";
}


const char * CompressionToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return "n/a";

	if ( auidCmp( auid, &AAFCompressionDef_AAF_CMPR_FULL_JPEG ) )
		return "AAFCompressionDef_AAF_CMPR_FULL_JPEG";

	if ( auidCmp( auid, &AAFCompressionDef_AAF_CMPR_AUNC422 ) )
		return "AAFCompressionDef_AAF_CMPR_AUNC422";

	if ( auidCmp( auid, &AAFCompressionDef_LegacyDV ) )
		return "AAFCompressionDef_LegacyDV";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_50Mbps_625x50I ) )
		return "AAFCompressionDef_SMPTE_D10_50Mbps_625x50I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_50Mbps_525x5994I ) )
		return "AAFCompressionDef_SMPTE_D10_50Mbps_525x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_40Mbps_625x50I ) )
		return "AAFCompressionDef_SMPTE_D10_40Mbps_625x50I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_40Mbps_525x5994I ) )
		return "AAFCompressionDef_SMPTE_D10_40Mbps_525x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_30Mbps_625x50I ) )
		return "AAFCompressionDef_SMPTE_D10_30Mbps_625x50I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_30Mbps_525x5994I ) )
		return "AAFCompressionDef_SMPTE_D10_30Mbps_525x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_IEC_DV_525_60 ) )
		return "AAFCompressionDef_IEC_DV_525_60";

	if ( auidCmp( auid, &AAFCompressionDef_IEC_DV_625_50 ) )
		return "AAFCompressionDef_IEC_DV_625_50";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_25Mbps_525_60 ) )
		return "AAFCompressionDef_DV_Based_25Mbps_525_60";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_25Mbps_625_50 ) )
		return "AAFCompressionDef_DV_Based_25Mbps_625_50";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_50Mbps_525_60 ) )
		return "AAFCompressionDef_DV_Based_50Mbps_525_60";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_50Mbps_625_50 ) )
		return "AAFCompressionDef_DV_Based_50Mbps_625_50";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_1080x5994I ) )
		return "AAFCompressionDef_DV_Based_100Mbps_1080x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_1080x50I ) )
		return "AAFCompressionDef_DV_Based_100Mbps_1080x50I";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_720x5994P ) )
		return "AAFCompressionDef_DV_Based_100Mbps_720x5994P";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_720x50P ) )
		return "AAFCompressionDef_DV_Based_100Mbps_720x50P";

	if ( auidCmp( auid, &AAFCompressionDef_VC3_1 ) )
		return "AAFCompressionDef_VC3_1";

	if ( auidCmp( auid, &AAFCompressionDef_Avid_DNxHD_Legacy ) )
		return "AAFCompressionDef_Avid_DNxHD_Legacy";


	if ( auidCmp( auid, &AUID_NULL ) )
		return "";

	return "Unknown value";
}
