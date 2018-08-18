#include <stdio.h>
#include <wchar.h>

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






const wchar_t * MobIDToText( aafMobID_t *mobid )
{
	static wchar_t str[127];

	uint32_t i = 0;
    uint32_t offset = 0;

	for ( i = 0; i < sizeof(aafMobID_t); i++ )
    {
		if ( i == 12 )	offset += swprintf( str+offset, (2 * sizeof(aafMobID_t)), L" - " );
		if ( i == 13 )	offset += swprintf( str+offset, (2 * sizeof(aafMobID_t)), L" - " );
		if ( i == 14 )	offset += swprintf( str+offset, (2 * sizeof(aafMobID_t)), L" - " );
		if ( i == 15 )	offset += swprintf( str+offset, (2 * sizeof(aafMobID_t)), L" - " );

		offset += swprintf( str+offset, 127, L"%02x", ((unsigned char*)mobid)[i] );

		if ( i == 15 )
		{
			offset += swprintf( str+offset, 127, L" - " );
			break;
		}
    }

	offset += swprintf( str+offset, 127, L"%ls", AUIDToText((aafUID_t*)((unsigned char*)mobid+i) ) );

	return str;
}




const wchar_t * TimestampToText( aafTimeStamp_t *ts )
{
	static wchar_t str[24];

	if ( ts == NULL )
	{
		str[0] = 'n';
		str[1] = '/';
		str[2] = 'a';
		str[3] = '\0';
	}
	else
	{
		swprintf( str, sizeof(str), L"%04i-%02u-%02u %02u:%02u:%02u.%02u",
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




const wchar_t * VersionToText( aafVersionType_t *vers )
{
	static wchar_t str[16];

	if ( vers == NULL )
	{
		str[0] = 'n';
		str[1] = '/';
		str[2] = 'a';
		str[3] = '\0';
	}
	else
	{
		swprintf( str, sizeof(str), L"%i.%i",
			vers->major,
			vers->minor );
	}

	return str;
}




const wchar_t * ProductVersionToText( aafProductVersion_t *vers )
{
	static wchar_t str[64];

	if ( vers == NULL )
	{
		str[0] = 'n';
		str[1] = '/';
		str[2] = 'a';
		str[3] = '\0';
	}
	else
	{
		swprintf( str, sizeof(str), L"%u.%u.%u.%u %ls (%i)",
			vers->major,
			vers->minor,
		 	vers->tertiary,
			vers->patchLevel,
			ProductReleaseTypeToText( vers->type ),
			vers->type );
	}

	return str;
}




const wchar_t * FileKindToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	/* AAFUID_NULL == AAFFileKind_DontCare */
	// if ( auidCmp( auid, &AAFUID_NULL ) )
	// 	return L"";

	if ( auidCmp( auid, &AAFFileKind_DontCare ) )
		return L"AAFFileKind_DontCare";

	if ( auidCmp( auid, &AAFFileKind_Aaf512Binary ) )
		return L"AAFFileKind_Aaf512Binary";

	if ( auidCmp( auid, &AAFFileKind_Aaf4KBinary ) )
		return L"AAFFileKind_Aaf4KBinary";

	if ( auidCmp( auid, &AAFFileKind_AafXmlText ) )
		return L"AAFFileKind_AafXmlText";

	if ( auidCmp( auid, &AAFFileKind_AafKlvBinary ) )
		return L"AAFFileKind_AafKlvBinary";

	if ( auidCmp( auid, &AAFFileKind_AafM512Binary ) )
		return L"AAFFileKind_AafM512Binary";

	if ( auidCmp( auid, &AAFFileKind_AafS512Binary ) )
		return L"AAFFileKind_AafS512Binary";

	if ( auidCmp( auid, &AAFFileKind_AafG512Binary ) )
		return L"AAFFileKind_AafG512Binary";

	if ( auidCmp( auid, &AAFFileKind_AafM4KBinary ) )
		return L"AAFFileKind_AafM4KBinary";

	if ( auidCmp( auid, &AAFFileKind_AafS4KBinary ) )
		return L"AAFFileKind_AafS4KBinary";

	if ( auidCmp( auid, &AAFFileKind_AafG4KBinary ) )
		return L"AAFFileKind_AafG4KBinary";

	if ( auidCmp( auid, &AAFFileKind_Pathological ) )
		return L"AAFFileKind_Pathological";

	return L"Unknown value";

}


const wchar_t * TapeCaseTypeToText( aafTapeCaseType_t t )
{
	switch ( t )
	{
		case AAFTapeCaseNull:					return L"AAFTapeCaseNull";
		case AAFThreeFourthInchVideoTape:		return L"AAFThreeFourthInchVideoTape";
		case AAFVHSVideoTape:					return L"AAFVHSVideoTape";
		case AAF8mmVideoTape:					return L"AAF8mmVideoTape";
		case AAFBetacamVideoTape:				return L"AAFBetacamVideoTape";
		case AAFCompactCassette:				return L"AAFCompactCassette";
		case AAFDATCartridge:					return L"AAFDATCartridge";
		case AAFNagraAudioTape:					return L"AAFNagraAudioTape";

		default:								return L"Unknown value";
	}
}


const wchar_t * VideoSignalTypeToText( aafVideoSignalType_t v )
{
	switch ( v )
	{
		case AAFVideoSignalNull:				return L"AAFVideoSignalNull";
		case AAFNTSCSignal:						return L"AAFNTSCSignal";
		case AAFPALSignal:						return L"AAFPALSignal";
		case AAFSECAMSignal:					return L"AAFSECAMSignal";

		default:								return L"Unknown value";
	}
}


const wchar_t * TapeFormatTypeToText( aafTapeFormatType_t t )
{
	switch ( t )
	{
		case AAFTapeFormatNull:					return L"AAFTapeFormatNull";
		case AAFBetacamFormat:					return L"AAFBetacamFormat";
		case AAFBetacamSPFormat:				return L"AAFBetacamSPFormat";
		case AAFVHSFormat:						return L"AAFVHSFormat";
		case AAFSVHSFormat:						return L"AAFSVHSFormat";
		case AAF8mmFormat:						return L"AAF8mmFormat";
		case AAFHi8Format:						return L"AAFHi8Format";

		default:								return L"Unknown value";
	}
}


const wchar_t * FilmTypeToText( aafFilmType_t f )
{
	switch ( f )
	{
		case AAFFtNull:							return L"AAFFtNull";
		case AAFFt35MM:							return L"AAFFt35MM";
		case AAFFt16MM:							return L"AAFFt16MM";
		case AAFFt8MM:							return L"AAFFt8MM";
		case AAFFt65MM:							return L"AAFFt65MM";

		default:								return L"Unknown value";
	}
}


const wchar_t * SignalStandardToText( aafSignalStandard_t s )
{
	switch ( s )
	{
		case AAFSignalStandard_None:			return L"AAFSignalStandard_None";
		case AAFSignalStandard_ITU601:			return L"AAFSignalStandard_ITU601";
		case AAFSignalStandard_ITU1358:			return L"AAFSignalStandard_ITU1358";
		case AAFSignalStandard_SMPTE347M:		return L"AAFSignalStandard_SMPTE347M";
		case AAFSignalStandard_SMPTE274M:		return L"AAFSignalStandard_SMPTE274M";
		case AAFSignalStandard_SMPTE296M:		return L"AAFSignalStandard_SMPTE296M";
		case AAFSignalStandard_SMPTE349M:		return L"AAFSignalStandard_SMPTE349M";

		default:								return L"Unknown value";
	}
}


const wchar_t * FieldNumberToText( aafFieldNumber_t f )
{
	switch ( f )
	{
		case AAFUnspecifiedField:				return L"AAFUnspecifiedField";
		case AAFFieldOne:						return L"AAFFieldOne";
		case AAFFieldTwo:						return L"AAFFieldTwo";

		default:								return L"Unknown value";
	}
}


const wchar_t * AlphaTransparencyToText( aafAlphaTransparency_t a )
{
	switch ( a )
	{
		case AAFMinValueTransparent:			return L"AAFMinValueTransparent";
		case AAFMaxValueTransparent:			return L"AAFMaxValueTransparent";

		default:								return L"Unknown value";
	}
}


const wchar_t * FrameLayoutToText( aafFrameLayout_t f )
{
	switch ( f )
	{
		case AAFFullFrame:						return L"AAFFullFrame";
		case AAFSeparateFields:					return L"AAFSeparateFields";
		case AAFOneField:						return L"AAFOneField";
		case AAFMixedFields:					return L"AAFMixedFields";
		case AAFSegmentedFrame:					return L"AAFSegmentedFrame";

		default:								return L"Unknown value";
	}
}


const wchar_t * ColorSitingToText( aafColorSiting_t c )
{
	switch ( c )
	{
		case AAFCoSiting:						return L"AAFCoSiting";
		case AAFAveraging:						return L"AAFAveraging";
		case AAFThreeTap:						return L"AAFThreeTap";
		case AAFQuincunx:						return L"AAFQuincunx";
		case AAFRec601:							return L"AAFRec601";
		case AAFUnknownSiting:					return L"AAFUnknownSiting";

		default:								return L"Unknown value";
	}
}


const wchar_t * ProductReleaseTypeToText( aafProductReleaseType_t t )
{
	switch ( t )
	{
		case AAFVersionUnknown:					return L"AAFVersionUnknown";
		case AAFVersionReleased:				return L"AAFVersionReleased";
		case AAFVersionDebug:					return L"AAFVersionDebug";
		case AAFVersionPatched:					return L"AAFVersionPatched";
		case AAFVersionBeta:					return L"AAFVersionBeta";
		case AAFVersionPrivateBuild:			return L"AAFVersionPrivateBuild";

		default:								return L"Unknown value";
	}
}


const wchar_t * FadeTypeToText( aafFadeType_t f )
{
	switch ( f )
	{
		case AAFFadeNone:						return L"AAFFadeNone";
		case AAFFadeLinearAmp:					return L"AAFFadeLinearAmp";
		case AAFFadeLinearPower:				return L"AAFFadeLinearPower";

		default:								return L"Unknown value";
	}
}


const wchar_t * BoolToText( aafBoolean_t b )
{
	switch ( b )
	{
		case 1:									return L"True";
		case 0:									return L"False";

		default:								return L"Unknown value";
	}
}


const wchar_t * OperationCategoryToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	if ( auidCmp( auid, &AAFOperationCategory_Effect ) )
		return L"AAFOperationCategory_Effect";

	return L"Unknown value";
}


const wchar_t * PluginCategoryToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	if ( auidCmp( auid, &AAFPluginCategory_Effect ) )
		return L"AAFPluginCategory_Effect";

	if ( auidCmp( auid, &AAFPluginCategory_Codec ) )
		return L"AAFPluginCategory_Codec";

	if ( auidCmp( auid, &AAFPluginCategory_Interpolation ) )
		return L"AAFPluginCategory_Interpolation";

	return L"Unknown value";
}


const wchar_t * ScanningDirectionToText( aafScanningDirection_t s )
{
	switch ( s )
	{
		case AAFScanningDirection_LeftToRightTopToBottom:
	 return L"AAFScanningDirection_LeftToRightTopToBottom";

		case AAFScanningDirection_RightToLeftTopToBottom:
	 return L"AAFScanningDirection_RightToLeftTopToBottom";

		case AAFScanningDirection_LeftToRightBottomToTop:
	 return L"AAFScanningDirection_LeftToRightBottomToTop";

		case AAFScanningDirection_RightToLeftBottomToTop:
	 return L"AAFScanningDirection_RightToLeftBottomToTop";

		case AAFScanningDirection_TopToBottomLeftToRight:
	 return L"AAFScanningDirection_TopToBottomLeftToRight";

		case AAFScanningDirection_TopToBottomRightToLeft:
	 return L"AAFScanningDirection_TopToBottomRightToLeft";

		case AAFScanningDirection_BottomToTopLeftToRight:
	 return L"AAFScanningDirection_BottomToTopLeftToRight";

		case AAFScanningDirection_BottomToTopRightToLeft:
	 return L"AAFScanningDirection_BottomToTopRightToLeft";


		default:
	 return L"Unknown value";
	}

}


const wchar_t * ByteOrderToText( int16_t e )
{
	if ( e == AAF_HEADER_BYTEORDER_LE ||
		 e == AAF_PROPERTIES_BYTEORDER_LE )
			return L"Little-Endian";

	if ( e == AAF_HEADER_BYTEORDER_BE ||
		 e == AAF_PROPERTIES_BYTEORDER_BE )
			return L"Big-Endian";

	return L"Unknown value";
}


const wchar_t * ElectroSpatialToText( aafElectroSpatialFormulation_t e )
{
	switch ( e )
	{
		case AAFElectroSpatialFormulation_Default:
	 return L"AAFElectroSpatialFormulation_Default";

		case AAFElectroSpatialFormulation_TwoChannelMode:
	 return L"AAFElectroSpatialFormulation_TwoChannelMode";

		case AAFElectroSpatialFormulation_SingleChannelMode:
	 return L"AAFElectroSpatialFormulation_SingleChannelMode";

		case AAFElectroSpatialFormulation_PrimarySecondaryMode:
	 return L"AAFElectroSpatialFormulation_PrimarySecondaryMode";

		case AAFElectroSpatialFormulation_StereophonicMode:
	 return L"AAFElectroSpatialFormulation_StereophonicMode";

		case AAFElectroSpatialFormulation_SingleChannelDoubleSamplingFrequencyMode:
	 return L"AAFElectroSpatialFormulation_SingleChannelDoubleSamplingFrequencyMode";

		case AAFElectroSpatialFormulation_StereoLeftChannelDoubleSamplingFrequencyMode:
	 return L"AAFElectroSpatialFormulation_StereoLeftChannelDoubleSamplingFrequencyMode";

		case AAFElectroSpatialFormulation_StereoRightChannelDoubleSamplingFrequencyMode:
	 return L"AAFElectroSpatialFormulation_StereoRightChannelDoubleSamplingFrequencyMode";

		case AAFElectroSpatialFormulation_MultiChannelMode:
	 return L"AAFElectroSpatialFormulation_MultiChannelMode";

		default:
	 return L"Unknown value";
	}
}


const wchar_t * OPDefToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";


	if ( auidCmp( auid, &AAFOPDef_EditProtocol ) )
		return L"AAFOPDef_EditProtocol";

	if ( auidCmp( auid, &AAFOPDef_Unconstrained ) )
		return L"AAFOPDef_Unconstrained";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	return L"Unknown value";

}


const wchar_t * TypeIDToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";


	if ( auidCmp( auid, &AAFTypeID_UInt8 ) )
		return L"AAFTypeID_UInt8";

	if ( auidCmp( auid, &AAFTypeID_UInt16 ) )
		return L"AAFTypeID_UInt16";

	if ( auidCmp( auid, &AAFTypeID_UInt32 ) )
		return L"AAFTypeID_UInt32";

	if ( auidCmp( auid, &AAFTypeID_UInt64 ) )
		return L"AAFTypeID_UInt64";

	if ( auidCmp( auid, &AAFTypeID_Int8 ) )
		return L"AAFTypeID_Int8";

	if ( auidCmp( auid, &AAFTypeID_Int16 ) )
		return L"AAFTypeID_Int16";

	if ( auidCmp( auid, &AAFTypeID_Int32 ) )
		return L"AAFTypeID_Int32";

	if ( auidCmp( auid, &AAFTypeID_Int64 ) )
		return L"AAFTypeID_Int64";

	if ( auidCmp( auid, &AAFTypeID_PositionType ) )
		return L"AAFTypeID_PositionType";

	if ( auidCmp( auid, &AAFTypeID_LengthType ) )
		return L"AAFTypeID_LengthType";

	if ( auidCmp( auid, &AAFTypeID_JPEGTableIDType ) )
		return L"AAFTypeID_JPEGTableIDType";

	if ( auidCmp( auid, &AAFTypeID_PhaseFrameType ) )
		return L"AAFTypeID_PhaseFrameType";

	if ( auidCmp( auid, &AAFTypeID_AUID ) )
		return L"AAFTypeID_AUID";

	if ( auidCmp( auid, &AAFTypeID_MobIDType ) )
		return L"AAFTypeID_MobIDType";

	if ( auidCmp( auid, &AAFTypeID_Boolean ) )
		return L"AAFTypeID_Boolean";

	if ( auidCmp( auid, &AAFTypeID_Character ) )
		return L"AAFTypeID_Character";

	if ( auidCmp( auid, &AAFTypeID_String ) )
		return L"AAFTypeID_String";

	if ( auidCmp( auid, &AAFTypeID_ProductReleaseType ) )
		return L"AAFTypeID_ProductReleaseType";

	if ( auidCmp( auid, &AAFTypeID_TapeFormatType ) )
		return L"AAFTypeID_TapeFormatType";

	if ( auidCmp( auid, &AAFTypeID_VideoSignalType ) )
		return L"AAFTypeID_VideoSignalType";

	if ( auidCmp( auid, &AAFTypeID_TapeCaseType ) )
		return L"AAFTypeID_TapeCaseType";

	if ( auidCmp( auid, &AAFTypeID_ColorSitingType ) )
		return L"AAFTypeID_ColorSitingType";

	if ( auidCmp( auid, &AAFTypeID_EditHintType ) )
		return L"AAFTypeID_EditHintType";

	if ( auidCmp( auid, &AAFTypeID_FadeType ) )
		return L"AAFTypeID_FadeType";

	if ( auidCmp( auid, &AAFTypeID_LayoutType ) )
		return L"AAFTypeID_LayoutType";

	if ( auidCmp( auid, &AAFTypeID_TCSource ) )
		return L"AAFTypeID_TCSource";

	if ( auidCmp( auid, &AAFTypeID_PulldownDirectionType ) )
		return L"AAFTypeID_PulldownDirectionType";

	if ( auidCmp( auid, &AAFTypeID_PulldownKindType ) )
		return L"AAFTypeID_PulldownKindType";

	if ( auidCmp( auid, &AAFTypeID_EdgeType ) )
		return L"AAFTypeID_EdgeType";

	if ( auidCmp( auid, &AAFTypeID_FilmType ) )
		return L"AAFTypeID_FilmType";

	if ( auidCmp( auid, &AAFTypeID_RGBAComponentKind ) )
		return L"AAFTypeID_RGBAComponentKind";

	if ( auidCmp( auid, &AAFTypeID_ReferenceType ) )
		return L"AAFTypeID_ReferenceType";

	if ( auidCmp( auid, &AAFTypeID_AlphaTransparencyType ) )
		return L"AAFTypeID_AlphaTransparencyType";

	if ( auidCmp( auid, &AAFTypeID_FieldNumber ) )
		return L"AAFTypeID_FieldNumber";

	if ( auidCmp( auid, &AAFTypeID_ElectroSpatialFormulation ) )
		return L"AAFTypeID_ElectroSpatialFormulation";

	if ( auidCmp( auid, &AAFTypeID_EmphasisType ) )
		return L"AAFTypeID_EmphasisType";

	if ( auidCmp( auid, &AAFTypeID_AuxBitsModeType ) )
		return L"AAFTypeID_AuxBitsModeType";

	if ( auidCmp( auid, &AAFTypeID_ChannelStatusModeType ) )
		return L"AAFTypeID_ChannelStatusModeType";

	if ( auidCmp( auid, &AAFTypeID_UserDataModeType ) )
		return L"AAFTypeID_UserDataModeType";

	if ( auidCmp( auid, &AAFTypeID_SignalStandardType ) )
		return L"AAFTypeID_SignalStandardType";

	if ( auidCmp( auid, &AAFTypeID_ScanningDirectionType ) )
		return L"AAFTypeID_ScanningDirectionType";

	if ( auidCmp( auid, &AAFTypeID_ContentScanningType ) )
		return L"AAFTypeID_ContentScanningType";

	if ( auidCmp( auid, &AAFTypeID_TitleAlignmentType ) )
		return L"AAFTypeID_TitleAlignmentType";

	if ( auidCmp( auid, &AAFTypeID_OperationCategoryType ) )
		return L"AAFTypeID_OperationCategoryType";

	if ( auidCmp( auid, &AAFTypeID_TransferCharacteristicType ) )
		return L"AAFTypeID_TransferCharacteristicType";

	if ( auidCmp( auid, &AAFTypeID_PluginCategoryType ) )
		return L"AAFTypeID_PluginCategoryType";

	if ( auidCmp( auid, &AAFTypeID_UsageType ) )
		return L"AAFTypeID_UsageType";

	if ( auidCmp( auid, &AAFTypeID_ColorPrimariesType ) )
		return L"AAFTypeID_ColorPrimariesType";

	if ( auidCmp( auid, &AAFTypeID_CodingEquationsType ) )
		return L"AAFTypeID_CodingEquationsType";

	if ( auidCmp( auid, &AAFTypeID_Rational ) )
		return L"AAFTypeID_Rational";

	if ( auidCmp( auid, &AAFTypeID_ProductVersion ) )
		return L"AAFTypeID_ProductVersion";

	if ( auidCmp( auid, &AAFTypeID_VersionType ) )
		return L"AAFTypeID_VersionType";

	if ( auidCmp( auid, &AAFTypeID_RGBAComponent ) )
		return L"AAFTypeID_RGBAComponent";

	if ( auidCmp( auid, &AAFTypeID_DateStruct ) )
		return L"AAFTypeID_DateStruct";

	if ( auidCmp( auid, &AAFTypeID_TimeStruct ) )
		return L"AAFTypeID_TimeStruct";

	if ( auidCmp( auid, &AAFTypeID_TimeStamp ) )
		return L"AAFTypeID_TimeStamp";

	if ( auidCmp( auid, &AAFTypeID_UInt8Array ) )
		return L"AAFTypeID_UInt8Array";

	if ( auidCmp( auid, &AAFTypeID_UInt8Array12 ) )
		return L"AAFTypeID_UInt8Array12";

	if ( auidCmp( auid, &AAFTypeID_Int32Array ) )
		return L"AAFTypeID_Int32Array";

	if ( auidCmp( auid, &AAFTypeID_Int64Array ) )
		return L"AAFTypeID_Int64Array";

	if ( auidCmp( auid, &AAFTypeID_StringArray ) )
		return L"AAFTypeID_StringArray";

	if ( auidCmp( auid, &AAFTypeID_AUIDArray ) )
		return L"AAFTypeID_AUIDArray";

	if ( auidCmp( auid, &AAFTypeID_PositionArray ) )
		return L"AAFTypeID_PositionArray";

	if ( auidCmp( auid, &AAFTypeID_UInt8Array8 ) )
		return L"AAFTypeID_UInt8Array8";

	if ( auidCmp( auid, &AAFTypeID_UInt32Array ) )
		return L"AAFTypeID_UInt32Array";

	if ( auidCmp( auid, &AAFTypeID_ChannelStatusModeArray ) )
		return L"AAFTypeID_ChannelStatusModeArray";

	if ( auidCmp( auid, &AAFTypeID_UserDataModeArray ) )
		return L"AAFTypeID_UserDataModeArray";

	if ( auidCmp( auid, &AAFTypeID_RGBALayout ) )
		return L"AAFTypeID_RGBALayout";

	if ( auidCmp( auid, &AAFTypeID_AUIDSet ) )
		return L"AAFTypeID_AUIDSet";

	if ( auidCmp( auid, &AAFTypeID_UInt32Set ) )
		return L"AAFTypeID_UInt32Set";

	if ( auidCmp( auid, &AAFTypeID_DataValue ) )
		return L"AAFTypeID_DataValue";

	if ( auidCmp( auid, &AAFTypeID_Stream ) )
		return L"AAFTypeID_Stream";

	if ( auidCmp( auid, &AAFTypeID_Indirect ) )
		return L"AAFTypeID_Indirect";

	if ( auidCmp( auid, &AAFTypeID_Opaque ) )
		return L"AAFTypeID_Opaque";

	if ( auidCmp( auid, &AAFTypeID_ClassDefinitionWeakReference ) )
		return L"AAFTypeID_ClassDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_ContainerDefinitionWeakReference ) )
		return L"AAFTypeID_ContainerDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionWeakReference ) )
		return L"AAFTypeID_DataDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_InterpolationDefinitionWeakReference ) )
		return L"AAFTypeID_InterpolationDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_MobWeakReference ) )
		return L"AAFTypeID_MobWeakReference";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionWeakReference ) )
		return L"AAFTypeID_OperationDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionWeakReference ) )
		return L"AAFTypeID_ParameterDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionWeakReference ) )
		return L"AAFTypeID_TypeDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionWeakReference ) )
		return L"AAFTypeID_PluginDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_CodecDefinitionWeakReference ) )
		return L"AAFTypeID_CodecDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionWeakReference ) )
		return L"AAFTypeID_PropertyDefinitionWeakReference";

	if ( auidCmp( auid, &AAFTypeID_ContentStorageStrongReference ) )
		return L"AAFTypeID_ContentStorageStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DictionaryStrongReference ) )
		return L"AAFTypeID_DictionaryStrongReference";

	if ( auidCmp( auid, &AAFTypeID_EssenceDescriptorStrongReference ) )
		return L"AAFTypeID_EssenceDescriptorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_NetworkLocatorStrongReference ) )
		return L"AAFTypeID_NetworkLocatorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_OperationGroupStrongReference ) )
		return L"AAFTypeID_OperationGroupStrongReference";

	if ( auidCmp( auid, &AAFTypeID_SegmentStrongReference ) )
		return L"AAFTypeID_SegmentStrongReference";

	if ( auidCmp( auid, &AAFTypeID_SourceClipStrongReference ) )
		return L"AAFTypeID_SourceClipStrongReference";

	if ( auidCmp( auid, &AAFTypeID_SourceReferenceStrongReference ) )
		return L"AAFTypeID_SourceReferenceStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ClassDefinitionStrongReference ) )
		return L"AAFTypeID_ClassDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_CodecDefinitionStrongReference ) )
		return L"AAFTypeID_CodecDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ComponentStrongReference ) )
		return L"AAFTypeID_ComponentStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ContainerDefinitionStrongReference ) )
		return L"AAFTypeID_ContainerDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ControlPointStrongReference ) )
		return L"AAFTypeID_ControlPointStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionStrongReference ) )
		return L"AAFTypeID_DataDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_EssenceDataStrongReference ) )
		return L"AAFTypeID_EssenceDataStrongReference";

	if ( auidCmp( auid, &AAFTypeID_IdentificationStrongReference ) )
		return L"AAFTypeID_IdentificationStrongReference";

	if ( auidCmp( auid, &AAFTypeID_InterpolationDefinitionStrongReference ) )
		return L"AAFTypeID_InterpolationDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_LocatorStrongReference ) )
		return L"AAFTypeID_LocatorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_MobStrongReference ) )
		return L"AAFTypeID_MobStrongReference";

	if ( auidCmp( auid, &AAFTypeID_MobSlotStrongReference ) )
		return L"AAFTypeID_MobSlotStrongReference";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionStrongReference ) )
		return L"AAFTypeID_OperationDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ParameterStrongReference ) )
		return L"AAFTypeID_ParameterStrongReference";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionStrongReference ) )
		return L"AAFTypeID_ParameterDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionStrongReference ) )
		return L"AAFTypeID_PluginDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionStrongReference ) )
		return L"AAFTypeID_PropertyDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueStrongReference ) )
		return L"AAFTypeID_TaggedValueStrongReference";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionStrongReference ) )
		return L"AAFTypeID_TypeDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_KLVDataStrongReference ) )
		return L"AAFTypeID_KLVDataStrongReference";

	if ( auidCmp( auid, &AAFTypeID_FileDescriptorStrongReference ) )
		return L"AAFTypeID_FileDescriptorStrongReference";

	if ( auidCmp( auid, &AAFTypeID_RIFFChunkStrongReference ) )
		return L"AAFTypeID_RIFFChunkStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveFrameworkStrongReference ) )
		return L"AAFTypeID_DescriptiveFrameworkStrongReference";

	if ( auidCmp( auid, &AAFTypeID_KLVDataDefinitionStrongReference ) )
		return L"AAFTypeID_KLVDataDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueDefinitionStrongReference ) )
		return L"AAFTypeID_TaggedValueDefinitionStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveObjectStrongReference ) )
		return L"AAFTypeID_DescriptiveObjectStrongReference";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionWeakReferenceSet ) )
		return L"AAFTypeID_DataDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionWeakReferenceSet ) )
		return L"AAFTypeID_ParameterDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionWeakReferenceSet ) )
		return L"AAFTypeID_PluginDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionWeakReferenceSet ) )
		return L"AAFTypeID_PropertyDefinitionWeakReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionWeakReferenceVector ) )
		return L"AAFTypeID_OperationDefinitionWeakReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionWeakReferenceVector ) )
		return L"AAFTypeID_TypeDefinitionWeakReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionWeakReferenceVector ) )
		return L"AAFTypeID_DataDefinitionWeakReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_ClassDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_ClassDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_CodecDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_CodecDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ContainerDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_ContainerDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_DataDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_DataDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_EssenceDataStrongReferenceSet ) )
		return L"AAFTypeID_EssenceDataStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_InterpolationDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_InterpolationDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_MobStrongReferenceSet ) )
		return L"AAFTypeID_MobStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_OperationDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_OperationDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ParameterDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_ParameterDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PluginDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_PluginDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_PropertyDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_PropertyDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_TypeDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_TypeDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_KLVDataDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_KLVDataDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueDefinitionStrongReferenceSet ) )
		return L"AAFTypeID_TaggedValueDefinitionStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveObjectStrongReferenceSet ) )
		return L"AAFTypeID_DescriptiveObjectStrongReferenceSet";

	if ( auidCmp( auid, &AAFTypeID_ComponentStrongReferenceVector ) )
		return L"AAFTypeID_ComponentStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_ControlPointStrongReferenceVector ) )
		return L"AAFTypeID_ControlPointStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_IdentificationStrongReferenceVector ) )
		return L"AAFTypeID_IdentificationStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_LocatorStrongReferenceVector ) )
		return L"AAFTypeID_LocatorStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_MobSlotStrongReferenceVector ) )
		return L"AAFTypeID_MobSlotStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_SegmentStrongReferenceVector ) )
		return L"AAFTypeID_SegmentStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_SourceReferenceStrongReferenceVector ) )
		return L"AAFTypeID_SourceReferenceStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_TaggedValueStrongReferenceVector ) )
		return L"AAFTypeID_TaggedValueStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_KLVDataStrongReferenceVector ) )
		return L"AAFTypeID_KLVDataStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_ParameterStrongReferenceVector ) )
		return L"AAFTypeID_ParameterStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_FileDescriptorStrongReferenceVector ) )
		return L"AAFTypeID_FileDescriptorStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_RIFFChunkStrongReferenceVector ) )
		return L"AAFTypeID_RIFFChunkStrongReferenceVector";

	if ( auidCmp( auid, &AAFTypeID_DescriptiveObjectStrongReferenceVector ) )
		return L"AAFTypeID_DescriptiveObjectStrongReferenceVector";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	return L"Unknown value";

}


const wchar_t * DataDefToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFDataDef_Picture ) )
		return L"AAFDataDef_Picture";

	if ( auidCmp( auid, &AAFDataDef_LegacyPicture ) )
		return L"AAFDataDef_LegacyPicture";

	if ( auidCmp( auid, &AAFDataDef_Matte ) )
		return L"AAFDataDef_Matte";

	if ( auidCmp( auid, &AAFDataDef_PictureWithMatte ) )
		return L"AAFDataDef_PictureWithMatte";

	if ( auidCmp( auid, &AAFDataDef_Sound ) )
		return L"AAFDataDef_Sound";

	if ( auidCmp( auid, &AAFDataDef_LegacySound ) )
		return L"AAFDataDef_LegacySound";

	if ( auidCmp( auid, &AAFDataDef_Timecode ) )
		return L"AAFDataDef_Timecode";

	if ( auidCmp( auid, &AAFDataDef_LegacyTimecode ) )
		return L"AAFDataDef_LegacyTimecode";

	if ( auidCmp( auid, &AAFDataDef_Edgecode ) )
		return L"AAFDataDef_Edgecode";

	if ( auidCmp( auid, &AAFDataDef_DescriptiveMetadata ) )
		return L"AAFDataDef_DescriptiveMetadata";

	if ( auidCmp( auid, &AAFDataDef_Auxiliary ) )
		return L"AAFDataDef_Auxiliary";

	if ( auidCmp( auid, &AAFDataDef_Unknown ) )
		return L"AAFDataDef_Unknown";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"AAFUID_NULL";



	static wchar_t TEXTDataDef[1024];

	aafObject *DataDefinitions = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_DataDefinitions );
	aafObject *DataDefinition  = NULL;

	aaf_foreach_ObjectInSet( &DataDefinition, DataDefinitions, NULL )
	{
		aafUID_t *DataDefIdent = aaf_get_propertyValue( DataDefinition, PID_DefinitionObject_Identification );

		if ( DataDefIdent && auidCmp( DataDefIdent, auid ) )
		{
			wchar_t *name = aaf_get_propertyValueWstr( DataDefinition, PID_DefinitionObject_Name );
			swprintf( TEXTDataDef, 1024, L"%ls", name );
			free( name );

			return TEXTDataDef;
		}
	}


	return L"Unknown value";

}


const wchar_t * OperationDefToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFOperationDef_VideoDissolve ) )
		return L"AAFOperationDef_VideoDissolve";

	if ( auidCmp( auid, &AAFOperationDef_SMPTEVideoWipe ) )
		return L"AAFOperationDef_SMPTEVideoWipe";

	if ( auidCmp( auid, &AAFOperationDef_VideoSpeedControl ) )
		return L"AAFOperationDef_VideoSpeedControl";

	if ( auidCmp( auid, &AAFOperationDef_VideoRepeat ) )
		return L"AAFOperationDef_VideoRepeat";

	if ( auidCmp( auid, &AAFOperationDef_Flip ) )
		return L"AAFOperationDef_Flip";

	if ( auidCmp( auid, &AAFOperationDef_Flop ) )
		return L"AAFOperationDef_Flop";

	if ( auidCmp( auid, &AAFOperationDef_FlipFlop ) )
		return L"AAFOperationDef_FlipFlop";

	if ( auidCmp( auid, &AAFOperationDef_VideoPosition ) )
		return L"AAFOperationDef_VideoPosition";

	if ( auidCmp( auid, &AAFOperationDef_VideoCrop ) )
		return L"AAFOperationDef_VideoCrop";

	if ( auidCmp( auid, &AAFOperationDef_VideoScale ) )
		return L"AAFOperationDef_VideoScale";

	if ( auidCmp( auid, &AAFOperationDef_VideoRotate ) )
		return L"AAFOperationDef_VideoRotate";

	if ( auidCmp( auid, &AAFOperationDef_VideoCornerPinning ) )
		return L"AAFOperationDef_VideoCornerPinning";

	if ( auidCmp( auid, &AAFOperationDef_VideoAlphaWithinVideoKey ) )
		return L"AAFOperationDef_VideoAlphaWithinVideoKey";

	if ( auidCmp( auid, &AAFOperationDef_VideoSeparateAlphaKey ) )
		return L"AAFOperationDef_VideoSeparateAlphaKey";

	if ( auidCmp( auid, &AAFOperationDef_VideoLuminanceKey ) )
		return L"AAFOperationDef_VideoLuminanceKey";

	if ( auidCmp( auid, &AAFOperationDef_VideoChromaKey ) )
		return L"AAFOperationDef_VideoChromaKey";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioGain ) )
		return L"AAFOperationDef_MonoAudioGain";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioPan ) )
		return L"AAFOperationDef_MonoAudioPan";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioDissolve ) )
		return L"AAFOperationDef_MonoAudioDissolve";

	if ( auidCmp( auid, &AAFOperationDef_TwoParameterMonoAudioDissolve ) )
		return L"AAFOperationDef_TwoParameterMonoAudioDissolve";

	if ( auidCmp( auid, &AAFOperationDef_VideoOpacity ) )
		return L"AAFOperationDef_VideoOpacity";

	if ( auidCmp( auid, &AAFOperationDef_VideoTitle ) )
		return L"AAFOperationDef_VideoTitle";

	if ( auidCmp( auid, &AAFOperationDef_VideoColor ) )
		return L"AAFOperationDef_VideoColor";

	if ( auidCmp( auid, &AAFOperationDef_Unknown ) )
		return L"AAFOperationDef_Unknown";

	if ( auidCmp( auid, &AAFOperationDef_VideoFadeToBlack ) )
		return L"AAFOperationDef_VideoFadeToBlack";

	if ( auidCmp( auid, &AAFOperationDef_PictureWithMate ) )
		return L"AAFOperationDef_PictureWithMate";

	if ( auidCmp( auid, &AAFOperationDef_VideoFrameToMask ) )
		return L"AAFOperationDef_VideoFrameToMask";

	if ( auidCmp( auid, &AAFOperationDef_StereoAudioDissolve ) )
		return L"AAFOperationDef_StereoAudioDissolve";

	if ( auidCmp( auid, &AAFOperationDef_StereoAudioGain ) )
		return L"AAFOperationDef_StereoAudioGain";

	if ( auidCmp( auid, &AAFOperationDef_MonoAudioMixdown ) )
		return L"AAFOperationDef_MonoAudioMixdown";

	if ( auidCmp( auid, &AAFOperationDef_AudioChannelCombiner ) )
		return L"AAFOperationDef_AudioChannelCombiner";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"AAFUID_NULL";



	static wchar_t TEXTOperationDef[1024];

	aafObject *OperationDefinitions = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_OperationDefinitions );
	aafObject *OperationDefinition  = NULL;

	aaf_foreach_ObjectInSet( &OperationDefinition, OperationDefinitions, NULL )
	{
		aafUID_t *OpDefIdent = aaf_get_propertyValue( OperationDefinition, PID_DefinitionObject_Identification );

		if ( OpDefIdent && auidCmp( OpDefIdent, auid ) )
		{
			wchar_t *name = aaf_get_propertyValueWstr( OperationDefinition, PID_DefinitionObject_Name );
			swprintf( TEXTOperationDef, 1024, L"%ls", name );
			free( name );

			return TEXTOperationDef;
		}
	}


	return L"Unknown value";

}


const wchar_t * InterpolationToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFInterpolationDef_None ) )
		return L"AAFInterpolationDef_None";

	if ( auidCmp( auid, &AAFInterpolationDef_Linear ) )
		return L"AAFInterpolationDef_Linear";

	if ( auidCmp( auid, &AAFInterpolationDef_Constant ) )
		return L"AAFInterpolationDef_Constant";

	if ( auidCmp( auid, &AAFInterpolationDef_BSpline ) )
		return L"AAFInterpolationDef_BSpline";

	if ( auidCmp( auid, &AAFInterpolationDef_Log ) )
		return L"AAFInterpolationDef_Log";

	if ( auidCmp( auid, &AAFInterpolationDef_Power ) )
		return L"AAFInterpolationDef_Power";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	return L"Unknown value";
}


const wchar_t * ParameterToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFParameterDef_Level ) )
		return L"AAFParameterDef_Level";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEWipeNumber ) )
		return L"AAFParameterDef_SMPTEWipeNumber";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEReverse ) )
		return L"AAFParameterDef_SMPTEReverse";

	if ( auidCmp( auid, &AAFParameterDef_SpeedRatio ) )
		return L"AAFParameterDef_SpeedRatio";

	if ( auidCmp( auid, &AAFParameterDef_PositionOffsetX ) )
		return L"AAFParameterDef_PositionOffsetX";

	if ( auidCmp( auid, &AAFParameterDef_PositionOffsetY ) )
		return L"AAFParameterDef_PositionOffsetY";

	if ( auidCmp( auid, &AAFParameterDef_CropLeft ) )
		return L"AAFParameterDef_CropLeft";

	if ( auidCmp( auid, &AAFParameterDef_CropRight ) )
		return L"AAFParameterDef_CropRight";

	if ( auidCmp( auid, &AAFParameterDef_CropTop ) )
		return L"AAFParameterDef_CropTop";

	if ( auidCmp( auid, &AAFParameterDef_CropBottom ) )
		return L"AAFParameterDef_CropBottom";

	if ( auidCmp( auid, &AAFParameterDef_ScaleX ) )
		return L"AAFParameterDef_ScaleX";

	if ( auidCmp( auid, &AAFParameterDef_ScaleY ) )
		return L"AAFParameterDef_ScaleY";

	if ( auidCmp( auid, &AAFParameterDef_Rotation ) )
		return L"AAFParameterDef_Rotation";

	if ( auidCmp( auid, &AAFParameterDef_PinTopLeftX ) )
		return L"AAFParameterDef_PinTopLeftX";

	if ( auidCmp( auid, &AAFParameterDef_PinTopLeftY ) )
		return L"AAFParameterDef_PinTopLeftY";

	if ( auidCmp( auid, &AAFParameterDef_PinTopRightX ) )
		return L"AAFParameterDef_PinTopRightX";

	if ( auidCmp( auid, &AAFParameterDef_PinTopRightY ) )
		return L"AAFParameterDef_PinTopRightY";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomLeftX ) )
		return L"AAFParameterDef_PinBottomLeftX";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomLeftY ) )
		return L"AAFParameterDef_PinBottomLeftY";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomRightX ) )
		return L"AAFParameterDef_PinBottomRightX";

	if ( auidCmp( auid, &AAFParameterDef_PinBottomRightY ) )
		return L"AAFParameterDef_PinBottomRightY";

	if ( auidCmp( auid, &AAFParameterDef_AlphaKeyInvertAlpha ) )
		return L"AAFParameterDef_AlphaKeyInvertAlpha";

	if ( auidCmp( auid, &AAFParameterDef_LumKeyLevel ) )
		return L"AAFParameterDef_LumKeyLevel";

	if ( auidCmp( auid, &AAFParameterDef_LumKeyClip ) )
		return L"AAFParameterDef_LumKeyClip";

	if ( auidCmp( auid, &AAFParameterDef_Amplitude ) )
		return L"AAFParameterDef_Amplitude";

	if ( auidCmp( auid, &AAFParameterDef_Pan ) )
		return L"AAFParameterDef_Pan";

	if ( auidCmp( auid, &AAFParameterDef_OutgoingLevel ) )
		return L"AAFParameterDef_OutgoingLevel";

	if ( auidCmp( auid, &AAFParameterDef_IncomingLevel ) )
		return L"AAFParameterDef_IncomingLevel";

	if ( auidCmp( auid, &AAFParameterDef_OpacityLevel ) )
		return L"AAFParameterDef_OpacityLevel";

	if ( auidCmp( auid, &AAFParameterDef_TitleText ) )
		return L"AAFParameterDef_TitleText";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontName ) )
		return L"AAFParameterDef_TitleFontName";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontSize ) )
		return L"AAFParameterDef_TitleFontSize";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontColorR ) )
		return L"AAFParameterDef_TitleFontColorR";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontColorG ) )
		return L"AAFParameterDef_TitleFontColorG";

	if ( auidCmp( auid, &AAFParameterDef_TitleFontColorB ) )
		return L"AAFParameterDef_TitleFontColorB";

	if ( auidCmp( auid, &AAFParameterDef_TitleAlignment ) )
		return L"AAFParameterDef_TitleAlignment";

	if ( auidCmp( auid, &AAFParameterDef_TitleBold ) )
		return L"AAFParameterDef_TitleBold";

	if ( auidCmp( auid, &AAFParameterDef_TitleItalic ) )
		return L"AAFParameterDef_TitleItalic";

	if ( auidCmp( auid, &AAFParameterDef_TitlePositionX ) )
		return L"AAFParameterDef_TitlePositionX";

	if ( auidCmp( auid, &AAFParameterDef_TitlePositionY ) )
		return L"AAFParameterDef_TitlePositionY";

	if ( auidCmp( auid, &AAFParameterDef_ColorSlopeR ) )
		return L"AAFParameterDef_ColorSlopeR";

	if ( auidCmp( auid, &AAFParameterDef_ColorSlopeG ) )
		return L"AAFParameterDef_ColorSlopeG";

	if ( auidCmp( auid, &AAFParameterDef_ColorSlopeB ) )
		return L"AAFParameterDef_ColorSlopeB";

	if ( auidCmp( auid, &AAFParameterDef_ColorOffsetR ) )
		return L"AAFParameterDef_ColorOffsetR";

	if ( auidCmp( auid, &AAFParameterDef_ColorOffsetG ) )
		return L"AAFParameterDef_ColorOffsetG";

	if ( auidCmp( auid, &AAFParameterDef_ColorOffsetB ) )
		return L"AAFParameterDef_ColorOffsetB";

	if ( auidCmp( auid, &AAFParameterDef_ColorPowerR ) )
		return L"AAFParameterDef_ColorPowerR";

	if ( auidCmp( auid, &AAFParameterDef_ColorPowerG ) )
		return L"AAFParameterDef_ColorPowerG";

	if ( auidCmp( auid, &AAFParameterDef_ColorPowerB ) )
		return L"AAFParameterDef_ColorPowerB";

	if ( auidCmp( auid, &AAFParameterDef_ColorSaturation ) )
		return L"AAFParameterDef_ColorSaturation";

	if ( auidCmp( auid, &AAFParameterDef_ColorCorrectionDescription ) )
		return L"AAFParameterDef_ColorCorrectionDescription";

	if ( auidCmp( auid, &AAFParameterDef_ColorInputDescription ) )
		return L"AAFParameterDef_ColorInputDescription";

	if ( auidCmp( auid, &AAFParameterDef_ColorViewingDescription ) )
		return L"AAFParameterDef_ColorViewingDescription";

	if ( auidCmp( auid, &AAFParameterDef_SMPTESoft ) )
		return L"AAFParameterDef_SMPTESoft";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEBorder ) )
		return L"AAFParameterDef_SMPTEBorder";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEPosition ) )
		return L"AAFParameterDef_SMPTEPosition";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEModulator ) )
		return L"AAFParameterDef_SMPTEModulator";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEShadow ) )
		return L"AAFParameterDef_SMPTEShadow";

	if ( auidCmp( auid, &AAFParameterDef_SMPTETumble ) )
		return L"AAFParameterDef_SMPTETumble";

	if ( auidCmp( auid, &AAFParameterDef_SMPTESpotlight ) )
		return L"AAFParameterDef_SMPTESpotlight";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEReplicationH ) )
		return L"AAFParameterDef_SMPTEReplicationH";

	if ( auidCmp( auid, &AAFParameterDef_SMPTEReplicationV ) )
		return L"AAFParameterDef_SMPTEReplicationV";

	if ( auidCmp( auid, &AAFParameterDef_SMPTECheckerboard ) )
		return L"AAFParameterDef_SMPTECheckerboard";

	if ( auidCmp( auid, &AAFParameterDef_PhaseOffset ) )
		return L"AAFParameterDef_PhaseOffset";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"AAFUID_NULL";



	static wchar_t TEXTParameterDef[1024];

	aafObject *ParameterDefinitions = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_ParameterDefinitions );
	aafObject *ParameterDefinition  = NULL;

	aaf_foreach_ObjectInSet( &ParameterDefinition, ParameterDefinitions, NULL )
	{
		aafUID_t *ParamDefIdent = aaf_get_propertyValue( ParameterDefinition, PID_DefinitionObject_Identification );

		if ( ParamDefIdent && auidCmp( ParamDefIdent, auid ) )
		{
			wchar_t *name = aaf_get_propertyValueWstr( ParameterDefinition, PID_DefinitionObject_Name );
			swprintf( TEXTParameterDef, 1024, L"%ls", name );
			free( name );

			// printf("Description : %ls\n", aaf_get_propertyValueText( ParameterDefinition, PID_DefinitionObject_Description ) );

			return TEXTParameterDef;
		}
	}


	return L"Unknown value";
}


const wchar_t * TransferCharacteristicToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFTransferCharacteristic_ITU470_PAL ) )
		         return L"AAFTransferCharacteristic_ITU470_PAL";

	if ( auidCmp( auid, &AAFTransferCharacteristic_ITU709 ) )
		         return L"AAFTransferCharacteristic_ITU709";

	if ( auidCmp( auid, &AAFTransferCharacteristic_SMPTE240M ) )
		         return L"AAFTransferCharacteristic_SMPTE240M";

	if ( auidCmp( auid, &AAFTransferCharacteristic_274M_296M ) )
		         return L"AAFTransferCharacteristic_274M_296M";

	if ( auidCmp( auid, &AAFTransferCharacteristic_ITU1361 ) )
		         return L"AAFTransferCharacteristic_ITU1361";

	if ( auidCmp( auid, &AAFTransferCharacteristic_linear ) )
		         return L"AAFTransferCharacteristic_linear";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	return L"Unknown value";
}


const wchar_t * CodingEquationsToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFCodingEquations_ITU601 ) )
		         return L"AAFCodingEquations_ITU601";

	if ( auidCmp( auid, &AAFCodingEquations_ITU709 ) )
		         return L"AAFCodingEquations_ITU709";

	if ( auidCmp( auid, &AAFCodingEquations_SMPTE240M ) )
		         return L"AAFCodingEquations_SMPTE240M";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	return L"Unknown value";
}


const wchar_t * ColorPrimariesToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFColorPrimaries_SMPTE170M ) )
		         return L"AAFColorPrimaries_SMPTE170M";

	if ( auidCmp( auid, &AAFColorPrimaries_ITU470_PAL ) )
		         return L"AAFColorPrimaries_ITU470_PAL";

	if ( auidCmp( auid, &AAFColorPrimaries_ITU709 ) )
		         return L"AAFColorPrimaries_ITU709";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	return L"Unknown value";
}


const wchar_t * UsageCodeToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFUsage_SubClip ) )
		         return L"AAFUsage_SubClip";

	if ( auidCmp( auid, &AAFUsage_AdjustedClip ) )
		         return L"AAFUsage_AdjustedClip";

	if ( auidCmp( auid, &AAFUsage_TopLevel ) )
		         return L"AAFUsage_TopLevel";

	if ( auidCmp( auid, &AAFUsage_LowerLevel ) )
		         return L"AAFUsage_LowerLevel";

	if ( auidCmp( auid, &AAFUsage_Template ) )
		         return L"AAFUsage_Template";


	if ( auidCmp( auid, &AAFUID_NULL ) )
		return L"";

	return L"Unknown value";
}


const wchar_t * StoredFormToText( uint16_t sf )
{
	switch ( sf )
	{
		case SF_DATA:
	 return L"SF_DATA";
		case SF_DATA_STREAM:
	 return L"SF_DATA_STREAM";
		case SF_STRONG_OBJECT_REFERENCE:
	 return L"SF_STRONG_OBJECT_REFERENCE";
		case SF_STRONG_OBJECT_REFERENCE_VECTOR:
	 return L"SF_STRONG_OBJECT_REFERENCE_VECTOR";
		case SF_STRONG_OBJECT_REFERENCE_SET:
	 return L"SF_STRONG_OBJECT_REFERENCE_SET";
		case SF_WEAK_OBJECT_REFERENCE:
	 return L"SF_WEAK_OBJECT_REFERENCE";
		case SF_WEAK_OBJECT_REFERENCE_VECTOR:
	 return L"SF_WEAK_OBJECT_REFERENCE_VECTOR";
		case SF_WEAK_OBJECT_REFERENCE_SET:
	 return L"SF_WEAK_OBJECT_REFERENCE_SET";
		case SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID:
	 return L"SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID";
		case SF_UNIQUE_OBJECT_ID:
	 return L"SF_UNIQUE_OBJECT_ID";
		case SF_OPAQUE_STREAM:
	 return L"SF_OPAQUE_STREAM";

		default:
			return L"Unknown value";

	}
}


const wchar_t * PIDToText( AAF_Data *aafd, aafPID_t pid )
{
	switch( pid )
	{
		case PID_Root_MetaDictionary:
			return L"PID_Root_MetaDictionary";
		case PID_Root_Header:
			return L"PID_Root_Header";
		case PID_InterchangeObject_ObjClass:
			return L"PID_InterchangeObject_ObjClass";
		case PID_InterchangeObject_Generation:
			return L"PID_InterchangeObject_Generation";
		case PID_Component_DataDefinition:
			return L"PID_Component_DataDefinition";
		case PID_Component_Length:
			return L"PID_Component_Length";
		case PID_Component_KLVData:
			return L"PID_Component_KLVData";
		case PID_Component_UserComments:
			return L"PID_Component_UserComments";
		case PID_Component_Attributes:
			return L"PID_Component_Attributes";
		case PID_EdgeCode_Start:
			return L"PID_EdgeCode_Start";
		case PID_EdgeCode_FilmKind:
			return L"PID_EdgeCode_FilmKind";
		case PID_EdgeCode_CodeFormat:
			return L"PID_EdgeCode_CodeFormat";
		case PID_EdgeCode_Header:
			return L"PID_EdgeCode_Header";
		case PID_EssenceGroup_Choices:
			return L"PID_EssenceGroup_Choices";
		case PID_EssenceGroup_StillFrame:
			return L"PID_EssenceGroup_StillFrame";
		case PID_Event_Position:
			return L"PID_Event_Position";
		case PID_Event_Comment:
			return L"PID_Event_Comment";
		case PID_GPITrigger_ActiveState:
			return L"PID_GPITrigger_ActiveState";
		case PID_CommentMarker_Annotation:
			return L"PID_CommentMarker_Annotation";
		case PID_OperationGroup_Operation:
			return L"PID_OperationGroup_Operation";
		case PID_OperationGroup_InputSegments:
			return L"PID_OperationGroup_InputSegments";
		case PID_OperationGroup_Parameters:
			return L"PID_OperationGroup_Parameters";
		case PID_OperationGroup_BypassOverride:
			return L"PID_OperationGroup_BypassOverride";
		case PID_OperationGroup_Rendering:
			return L"PID_OperationGroup_Rendering";
		case PID_NestedScope_Slots:
			return L"PID_NestedScope_Slots";
		case PID_Pulldown_InputSegment:
			return L"PID_Pulldown_InputSegment";
		case PID_Pulldown_PulldownKind:
			return L"PID_Pulldown_PulldownKind";
		case PID_Pulldown_PulldownDirection:
			return L"PID_Pulldown_PulldownDirection";
		case PID_Pulldown_PhaseFrame:
			return L"PID_Pulldown_PhaseFrame";
		case PID_ScopeReference_RelativeScope:
			return L"PID_ScopeReference_RelativeScope";
		case PID_ScopeReference_RelativeSlot:
			return L"PID_ScopeReference_RelativeSlot";
		case PID_Selector_Selected:
			return L"PID_Selector_Selected";
		case PID_Selector_Alternates:
			return L"PID_Selector_Alternates";
		case PID_Sequence_Components:
			return L"PID_Sequence_Components";
		case PID_SourceReference_SourceID:
			return L"PID_SourceReference_SourceID";
		case PID_SourceReference_SourceMobSlotID:
			return L"PID_SourceReference_SourceMobSlotID";
		case PID_SourceReference_ChannelIDs:
			return L"PID_SourceReference_ChannelIDs";
		case PID_SourceReference_MonoSourceSlotIDs:
			return L"PID_SourceReference_MonoSourceSlotIDs";
		case PID_SourceClip_StartTime:
			return L"PID_SourceClip_StartTime";
		case PID_SourceClip_FadeInLength:
			return L"PID_SourceClip_FadeInLength";
		case PID_SourceClip_FadeInType:
			return L"PID_SourceClip_FadeInType";
		case PID_SourceClip_FadeOutLength:
			return L"PID_SourceClip_FadeOutLength";
		case PID_SourceClip_FadeOutType:
			return L"PID_SourceClip_FadeOutType";
		case PID_HTMLClip_BeginAnchor:
			return L"PID_HTMLClip_BeginAnchor";
		case PID_HTMLClip_EndAnchor:
			return L"PID_HTMLClip_EndAnchor";
		case PID_Timecode_Start:
			return L"PID_Timecode_Start";
		case PID_Timecode_FPS:
			return L"PID_Timecode_FPS";
		case PID_Timecode_Drop:
			return L"PID_Timecode_Drop";
		case PID_TimecodeStream_SampleRate:
			return L"PID_TimecodeStream_SampleRate";
		case PID_TimecodeStream_Source:
			return L"PID_TimecodeStream_Source";
		case PID_TimecodeStream_SourceType:
			return L"PID_TimecodeStream_SourceType";
		case PID_TimecodeStream12M_IncludeSync:
			return L"PID_TimecodeStream12M_IncludeSync";
		case PID_Transition_OperationGroup:
			return L"PID_Transition_OperationGroup";
		case PID_Transition_CutPoint:
			return L"PID_Transition_CutPoint";
		case PID_ContentStorage_Mobs:
			return L"PID_ContentStorage_Mobs";
		case PID_ContentStorage_EssenceData:
			return L"PID_ContentStorage_EssenceData";
		case PID_ControlPoint_Value:
			return L"PID_ControlPoint_Value";
		case PID_ControlPoint_Time:
			return L"PID_ControlPoint_Time";
		case PID_ControlPoint_EditHint:
			return L"PID_ControlPoint_EditHint";
		case PID_DefinitionObject_Identification:
			return L"PID_DefinitionObject_Identification";
		case PID_DefinitionObject_Name:
			return L"PID_DefinitionObject_Name";
		case PID_DefinitionObject_Description:
			return L"PID_DefinitionObject_Description";
		case PID_OperationDefinition_DataDefinition:
			return L"PID_OperationDefinition_DataDefinition";
		case PID_OperationDefinition_IsTimeWarp:
			return L"PID_OperationDefinition_IsTimeWarp";
		case PID_OperationDefinition_DegradeTo:
			return L"PID_OperationDefinition_DegradeTo";
		case PID_OperationDefinition_OperationCategory:
			return L"PID_OperationDefinition_OperationCategory";
		case PID_OperationDefinition_NumberInputs:
			return L"PID_OperationDefinition_NumberInputs";
		case PID_OperationDefinition_Bypass:
			return L"PID_OperationDefinition_Bypass";
		case PID_OperationDefinition_ParametersDefined:
			return L"PID_OperationDefinition_ParametersDefined";
		case PID_ParameterDefinition_Type:
			return L"PID_ParameterDefinition_Type";
		case PID_ParameterDefinition_DisplayUnits:
			return L"PID_ParameterDefinition_DisplayUnits";
		case PID_PluginDefinition_PluginCategory:
			return L"PID_PluginDefinition_PluginCategory";
		case PID_PluginDefinition_VersionNumber:
			return L"PID_PluginDefinition_VersionNumber";
		case PID_PluginDefinition_VersionString:
			return L"PID_PluginDefinition_VersionString";
		case PID_PluginDefinition_Manufacturer:
			return L"PID_PluginDefinition_Manufacturer";
		case PID_PluginDefinition_ManufacturerInfo:
			return L"PID_PluginDefinition_ManufacturerInfo";
		case PID_PluginDefinition_ManufacturerID:
			return L"PID_PluginDefinition_ManufacturerID";
		case PID_PluginDefinition_Platform:
			return L"PID_PluginDefinition_Platform";
		case PID_PluginDefinition_MinPlatformVersion:
			return L"PID_PluginDefinition_MinPlatformVersion";
		case PID_PluginDefinition_MaxPlatformVersion:
			return L"PID_PluginDefinition_MaxPlatformVersion";
		case PID_PluginDefinition_Engine:
			return L"PID_PluginDefinition_Engine";
		case PID_PluginDefinition_MinEngineVersion:
			return L"PID_PluginDefinition_MinEngineVersion";
		case PID_PluginDefinition_MaxEngineVersion:
			return L"PID_PluginDefinition_MaxEngineVersion";
		case PID_PluginDefinition_PluginAPI:
			return L"PID_PluginDefinition_PluginAPI";
		case PID_PluginDefinition_MinPluginAPI:
			return L"PID_PluginDefinition_MinPluginAPI";
		case PID_PluginDefinition_MaxPluginAPI:
			return L"PID_PluginDefinition_MaxPluginAPI";
		case PID_PluginDefinition_SoftwareOnly:
			return L"PID_PluginDefinition_SoftwareOnly";
		case PID_PluginDefinition_Accelerator:
			return L"PID_PluginDefinition_Accelerator";
		case PID_PluginDefinition_Locators:
			return L"PID_PluginDefinition_Locators";
		case PID_PluginDefinition_Authentication:
			return L"PID_PluginDefinition_Authentication";
		case PID_PluginDefinition_DefinitionObject:
			return L"PID_PluginDefinition_DefinitionObject";
		case PID_CodecDefinition_FileDescriptorClass:
			return L"PID_CodecDefinition_FileDescriptorClass";
		case PID_CodecDefinition_DataDefinitions:
			return L"PID_CodecDefinition_DataDefinitions";
		case PID_ContainerDefinition_EssenceIsIdentified:
			return L"PID_ContainerDefinition_EssenceIsIdentified";
		case PID_Dictionary_OperationDefinitions:
			return L"PID_Dictionary_OperationDefinitions";
		case PID_Dictionary_ParameterDefinitions:
			return L"PID_Dictionary_ParameterDefinitions";
		case PID_Dictionary_DataDefinitions:
			return L"PID_Dictionary_DataDefinitions";
		case PID_Dictionary_PluginDefinitions:
			return L"PID_Dictionary_PluginDefinitions";
		case PID_Dictionary_CodecDefinitions:
			return L"PID_Dictionary_CodecDefinitions";
		case PID_Dictionary_ContainerDefinitions:
			return L"PID_Dictionary_ContainerDefinitions";
		case PID_Dictionary_InterpolationDefinitions:
			return L"PID_Dictionary_InterpolationDefinitions";
		case PID_Dictionary_KLVDataDefinitions:
			return L"PID_Dictionary_KLVDataDefinitions";
		case PID_Dictionary_TaggedValueDefinitions:
			return L"PID_Dictionary_TaggedValueDefinitions";
		case PID_EssenceData_MobID:
			return L"PID_EssenceData_MobID";
		case PID_EssenceData_Data:
			return L"PID_EssenceData_Data";
		case PID_EssenceData_SampleIndex:
			return L"PID_EssenceData_SampleIndex";
		case PID_EssenceDescriptor_Locator:
			return L"PID_EssenceDescriptor_Locator";
		case PID_FileDescriptor_SampleRate:
			return L"PID_FileDescriptor_SampleRate";
		case PID_FileDescriptor_Length:
			return L"PID_FileDescriptor_Length";
		case PID_FileDescriptor_ContainerFormat:
			return L"PID_FileDescriptor_ContainerFormat";
		case PID_FileDescriptor_CodecDefinition:
			return L"PID_FileDescriptor_CodecDefinition";
		case PID_FileDescriptor_LinkedSlotID:
			return L"PID_FileDescriptor_LinkedSlotID";
		case PID_AIFCDescriptor_Summary:
			return L"PID_AIFCDescriptor_Summary";
		case PID_DigitalImageDescriptor_Compression:
			return L"PID_DigitalImageDescriptor_Compression";
		case PID_DigitalImageDescriptor_StoredHeight:
			return L"PID_DigitalImageDescriptor_StoredHeight";
		case PID_DigitalImageDescriptor_StoredWidth:
			return L"PID_DigitalImageDescriptor_StoredWidth";
		case PID_DigitalImageDescriptor_SampledHeight:
			return L"PID_DigitalImageDescriptor_SampledHeight";
		case PID_DigitalImageDescriptor_SampledWidth:
			return L"PID_DigitalImageDescriptor_SampledWidth";
		case PID_DigitalImageDescriptor_SampledXOffset:
			return L"PID_DigitalImageDescriptor_SampledXOffset";
		case PID_DigitalImageDescriptor_SampledYOffset:
			return L"PID_DigitalImageDescriptor_SampledYOffset";
		case PID_DigitalImageDescriptor_DisplayHeight:
			return L"PID_DigitalImageDescriptor_DisplayHeight";
		case PID_DigitalImageDescriptor_DisplayWidth:
			return L"PID_DigitalImageDescriptor_DisplayWidth";
		case PID_DigitalImageDescriptor_DisplayXOffset:
			return L"PID_DigitalImageDescriptor_DisplayXOffset";
		case PID_DigitalImageDescriptor_DisplayYOffset:
			return L"PID_DigitalImageDescriptor_DisplayYOffset";
		case PID_DigitalImageDescriptor_FrameLayout:
			return L"PID_DigitalImageDescriptor_FrameLayout";
		case PID_DigitalImageDescriptor_VideoLineMap:
			return L"PID_DigitalImageDescriptor_VideoLineMap";
		case PID_DigitalImageDescriptor_ImageAspectRatio:
			return L"PID_DigitalImageDescriptor_ImageAspectRatio";
		case PID_DigitalImageDescriptor_AlphaTransparency:
			return L"PID_DigitalImageDescriptor_AlphaTransparency";
		case PID_DigitalImageDescriptor_TransferCharacteristic:
			return L"PID_DigitalImageDescriptor_TransferCharacteristic";
		case PID_DigitalImageDescriptor_ColorPrimaries:
			return L"PID_DigitalImageDescriptor_ColorPrimaries";
		case PID_DigitalImageDescriptor_CodingEquations:
			return L"PID_DigitalImageDescriptor_CodingEquations";
		case PID_DigitalImageDescriptor_ImageAlignmentFactor:
			return L"PID_DigitalImageDescriptor_ImageAlignmentFactor";
		case PID_DigitalImageDescriptor_FieldDominance:
			return L"PID_DigitalImageDescriptor_FieldDominance";
		case PID_DigitalImageDescriptor_FieldStartOffset:
			return L"PID_DigitalImageDescriptor_FieldStartOffset";
		case PID_DigitalImageDescriptor_FieldEndOffset:
			return L"PID_DigitalImageDescriptor_FieldEndOffset";
		case PID_DigitalImageDescriptor_SignalStandard:
			return L"PID_DigitalImageDescriptor_SignalStandard";
		case PID_DigitalImageDescriptor_StoredF2Offset:
			return L"PID_DigitalImageDescriptor_StoredF2Offset";
		case PID_DigitalImageDescriptor_DisplayF2Offset:
			return L"PID_DigitalImageDescriptor_DisplayF2Offset";
		case PID_DigitalImageDescriptor_ActiveFormatDescriptor:
			return L"PID_DigitalImageDescriptor_ActiveFormatDescriptor";
		case PID_CDCIDescriptor_ComponentWidth:
			return L"PID_CDCIDescriptor_ComponentWidth";
		case PID_CDCIDescriptor_HorizontalSubsampling:
			return L"PID_CDCIDescriptor_HorizontalSubsampling";
		case PID_CDCIDescriptor_ColorSiting:
			return L"PID_CDCIDescriptor_ColorSiting";
		case PID_CDCIDescriptor_BlackReferenceLevel:
			return L"PID_CDCIDescriptor_BlackReferenceLevel";
		case PID_CDCIDescriptor_WhiteReferenceLevel:
			return L"PID_CDCIDescriptor_WhiteReferenceLevel";
		case PID_CDCIDescriptor_ColorRange:
			return L"PID_CDCIDescriptor_ColorRange";
		case PID_CDCIDescriptor_PaddingBits:
			return L"PID_CDCIDescriptor_PaddingBits";
		case PID_CDCIDescriptor_VerticalSubsampling:
			return L"PID_CDCIDescriptor_VerticalSubsampling";
		case PID_CDCIDescriptor_AlphaSamplingWidth:
			return L"PID_CDCIDescriptor_AlphaSamplingWidth";
		case PID_CDCIDescriptor_ReversedByteOrder:
			return L"PID_CDCIDescriptor_ReversedByteOrder";
		case PID_RGBADescriptor_PixelLayout:
			return L"PID_RGBADescriptor_PixelLayout";
		case PID_RGBADescriptor_Palette:
			return L"PID_RGBADescriptor_Palette";
		case PID_RGBADescriptor_PaletteLayout:
			return L"PID_RGBADescriptor_PaletteLayout";
		case PID_RGBADescriptor_ScanningDirection:
			return L"PID_RGBADescriptor_ScanningDirection";
		case PID_RGBADescriptor_ComponentMaxRef:
			return L"PID_RGBADescriptor_ComponentMaxRef";
		case PID_RGBADescriptor_ComponentMinRef:
			return L"PID_RGBADescriptor_ComponentMinRef";
		case PID_RGBADescriptor_AlphaMaxRef:
			return L"PID_RGBADescriptor_AlphaMaxRef";
		case PID_RGBADescriptor_AlphaMinRef:
			return L"PID_RGBADescriptor_AlphaMinRef";
		case PID_TIFFDescriptor_IsUniform:
			return L"PID_TIFFDescriptor_IsUniform";
		case PID_TIFFDescriptor_IsContiguous:
			return L"PID_TIFFDescriptor_IsContiguous";
		case PID_TIFFDescriptor_LeadingLines:
			return L"PID_TIFFDescriptor_LeadingLines";
		case PID_TIFFDescriptor_TrailingLines:
			return L"PID_TIFFDescriptor_TrailingLines";
		case PID_TIFFDescriptor_JPEGTableID:
			return L"PID_TIFFDescriptor_JPEGTableID";
		case PID_TIFFDescriptor_Summary:
			return L"PID_TIFFDescriptor_Summary";
		case PID_WAVEDescriptor_Summary:
			return L"PID_WAVEDescriptor_Summary";
		case PID_FilmDescriptor_FilmFormat:
			return L"PID_FilmDescriptor_FilmFormat";
		case PID_FilmDescriptor_FrameRate:
			return L"PID_FilmDescriptor_FrameRate";
		case PID_FilmDescriptor_PerforationsPerFrame:
			return L"PID_FilmDescriptor_PerforationsPerFrame";
		case PID_FilmDescriptor_FilmAspectRatio:
			return L"PID_FilmDescriptor_FilmAspectRatio";
		case PID_FilmDescriptor_Manufacturer:
			return L"PID_FilmDescriptor_Manufacturer";
		case PID_FilmDescriptor_Model:
			return L"PID_FilmDescriptor_Model";
		case PID_FilmDescriptor_FilmGaugeFormat:
			return L"PID_FilmDescriptor_FilmGaugeFormat";
		case PID_FilmDescriptor_FilmBatchNumber:
			return L"PID_FilmDescriptor_FilmBatchNumber";
		case PID_TapeDescriptor_FormFactor:
			return L"PID_TapeDescriptor_FormFactor";
		case PID_TapeDescriptor_VideoSignal:
			return L"PID_TapeDescriptor_VideoSignal";
		case PID_TapeDescriptor_TapeFormat:
			return L"PID_TapeDescriptor_TapeFormat";
		case PID_TapeDescriptor_Length:
			return L"PID_TapeDescriptor_Length";
		case PID_TapeDescriptor_ManufacturerID:
			return L"PID_TapeDescriptor_ManufacturerID";
		case PID_TapeDescriptor_Model:
			return L"PID_TapeDescriptor_Model";
		case PID_TapeDescriptor_TapeBatchNumber:
			return L"PID_TapeDescriptor_TapeBatchNumber";
		case PID_TapeDescriptor_TapeStock:
			return L"PID_TapeDescriptor_TapeStock";
		case PID_Header_ByteOrder:
			return L"PID_Header_ByteOrder";
		case PID_Header_LastModified:
			return L"PID_Header_LastModified";
		case PID_Header_Content:
			return L"PID_Header_Content";
		case PID_Header_Dictionary:
			return L"PID_Header_Dictionary";
		case PID_Header_Version:
			return L"PID_Header_Version";
		case PID_Header_IdentificationList:
			return L"PID_Header_IdentificationList";
		case PID_Header_ObjectModelVersion:
			return L"PID_Header_ObjectModelVersion";
		case PID_Header_OperationalPattern:
			return L"PID_Header_OperationalPattern";
		case PID_Header_EssenceContainers:
			return L"PID_Header_EssenceContainers";
		case PID_Header_DescriptiveSchemes:
			return L"PID_Header_DescriptiveSchemes";
		case PID_Identification_CompanyName:
			return L"PID_Identification_CompanyName";
		case PID_Identification_ProductName:
			return L"PID_Identification_ProductName";
		case PID_Identification_ProductVersion:
			return L"PID_Identification_ProductVersion";
		case PID_Identification_ProductVersionString:
			return L"PID_Identification_ProductVersionString";
		case PID_Identification_ProductID:
			return L"PID_Identification_ProductID";
		case PID_Identification_Date:
			return L"PID_Identification_Date";
		case PID_Identification_ToolkitVersion:
			return L"PID_Identification_ToolkitVersion";
		case PID_Identification_Platform:
			return L"PID_Identification_Platform";
		case PID_Identification_GenerationAUID:
			return L"PID_Identification_GenerationAUID";
		case PID_NetworkLocator_URLString:
			return L"PID_NetworkLocator_URLString";
		case PID_TextLocator_Name:
			return L"PID_TextLocator_Name";
		case PID_Mob_MobID:
			return L"PID_Mob_MobID";
		case PID_Mob_Name:
			return L"PID_Mob_Name";
		case PID_Mob_Slots:
			return L"PID_Mob_Slots";
		case PID_Mob_LastModified:
			return L"PID_Mob_LastModified";
		case PID_Mob_CreationTime:
			return L"PID_Mob_CreationTime";
		case PID_Mob_UserComments:
			return L"PID_Mob_UserComments";
		case PID_Mob_KLVData:
			return L"PID_Mob_KLVData";
		case PID_Mob_Attributes:
			return L"PID_Mob_Attributes";
		case PID_Mob_UsageCode:
			return L"PID_Mob_UsageCode";
		case PID_CompositionMob_DefaultFadeLength:
			return L"PID_CompositionMob_DefaultFadeLength";
		case PID_CompositionMob_DefFadeType:
			return L"PID_CompositionMob_DefFadeType";
		case PID_CompositionMob_DefFadeEditUnit:
			return L"PID_CompositionMob_DefFadeEditUnit";
		case PID_CompositionMob_Rendering:
			return L"PID_CompositionMob_Rendering";
		case PID_SourceMob_EssenceDescription:
			return L"PID_SourceMob_EssenceDescription";
		case PID_MobSlot_SlotID:
			return L"PID_MobSlot_SlotID";
		case PID_MobSlot_SlotName:
			return L"PID_MobSlot_SlotName";
		case PID_MobSlot_Segment:
			return L"PID_MobSlot_Segment";
		case PID_MobSlot_PhysicalTrackNumber:
			return L"PID_MobSlot_PhysicalTrackNumber";
		case PID_EventMobSlot_EditRate:
			return L"PID_EventMobSlot_EditRate";
		case PID_EventMobSlot_EventSlotOrigin:
			return L"PID_EventMobSlot_EventSlotOrigin";
		case PID_TimelineMobSlot_EditRate:
			return L"PID_TimelineMobSlot_EditRate";
		case PID_TimelineMobSlot_Origin:
			return L"PID_TimelineMobSlot_Origin";
		case PID_TimelineMobSlot_MarkIn:
			return L"PID_TimelineMobSlot_MarkIn";
		case PID_TimelineMobSlot_MarkOut:
			return L"PID_TimelineMobSlot_MarkOut";
		case PID_TimelineMobSlot_UserPos:
			return L"PID_TimelineMobSlot_UserPos";
		case PID_Parameter_Definition:
			return L"PID_Parameter_Definition";
		case PID_ConstantValue_Value:
			return L"PID_ConstantValue_Value";
		case PID_VaryingValue_Interpolation:
			return L"PID_VaryingValue_Interpolation";
		case PID_VaryingValue_PointList:
			return L"PID_VaryingValue_PointList";
		case PID_TaggedValue_Name:
			return L"PID_TaggedValue_Name";
		case PID_TaggedValue_Value:
			return L"PID_TaggedValue_Value";
		case PID_KLVData_Value:
			return L"PID_KLVData_Value";
		case PID_DescriptiveMarker_DescribedSlots:
			return L"PID_DescriptiveMarker_DescribedSlots";
		case PID_DescriptiveMarker_Description:
			return L"PID_DescriptiveMarker_Description";
		case PID_SoundDescriptor_AudioSamplingRate:
			return L"PID_SoundDescriptor_AudioSamplingRate";
		case PID_SoundDescriptor_Locked:
			return L"PID_SoundDescriptor_Locked";
		case PID_SoundDescriptor_AudioRefLevel:
			return L"PID_SoundDescriptor_AudioRefLevel";
		case PID_SoundDescriptor_ElectroSpatial:
			return L"PID_SoundDescriptor_ElectroSpatial";
		case PID_SoundDescriptor_Channels:
			return L"PID_SoundDescriptor_Channels";
		case PID_SoundDescriptor_QuantizationBits:
			return L"PID_SoundDescriptor_QuantizationBits";
		case PID_SoundDescriptor_DialNorm:
			return L"PID_SoundDescriptor_DialNorm";
		case PID_SoundDescriptor_Compression:
			return L"PID_SoundDescriptor_Compression";
		case PID_DataEssenceDescriptor_DataEssenceCoding:
			return L"PID_DataEssenceDescriptor_DataEssenceCoding";
		case PID_MultipleDescriptor_FileDescriptors:
			return L"PID_MultipleDescriptor_FileDescriptors";
		case PID_DescriptiveClip_DescribedSlotIDs:
			return L"PID_DescriptiveClip_DescribedSlotIDs";
		case PID_AES3PCMDescriptor_Emphasis:
			return L"PID_AES3PCMDescriptor_Emphasis";
		case PID_AES3PCMDescriptor_BlockStartOffset:
			return L"PID_AES3PCMDescriptor_BlockStartOffset";
		case PID_AES3PCMDescriptor_AuxBitsMode:
			return L"PID_AES3PCMDescriptor_AuxBitsMode";
		case PID_AES3PCMDescriptor_ChannelStatusMode:
			return L"PID_AES3PCMDescriptor_ChannelStatusMode";
		case PID_AES3PCMDescriptor_FixedChannelStatusData:
			return L"PID_AES3PCMDescriptor_FixedChannelStatusData";
		case PID_AES3PCMDescriptor_UserDataMode:
			return L"PID_AES3PCMDescriptor_UserDataMode";
		case PID_AES3PCMDescriptor_FixedUserData:
			return L"PID_AES3PCMDescriptor_FixedUserData";
		case PID_PCMDescriptor_BlockAlign:
			return L"PID_PCMDescriptor_BlockAlign";
		case PID_PCMDescriptor_SequenceOffset:
			return L"PID_PCMDescriptor_SequenceOffset";
		case PID_PCMDescriptor_AverageBPS:
			return L"PID_PCMDescriptor_AverageBPS";
		case PID_PCMDescriptor_ChannelAssignment:
			return L"PID_PCMDescriptor_ChannelAssignment";
		case PID_PCMDescriptor_PeakEnvelopeVersion:
			return L"PID_PCMDescriptor_PeakEnvelopeVersion";
		case PID_PCMDescriptor_PeakEnvelopeFormat:
			return L"PID_PCMDescriptor_PeakEnvelopeFormat";
		case PID_PCMDescriptor_PointsPerPeakValue:
			return L"PID_PCMDescriptor_PointsPerPeakValue";
		case PID_PCMDescriptor_PeakEnvelopeBlockSize:
			return L"PID_PCMDescriptor_PeakEnvelopeBlockSize";
		case PID_PCMDescriptor_PeakChannels:
			return L"PID_PCMDescriptor_PeakChannels";
		case PID_PCMDescriptor_PeakFrames:
			return L"PID_PCMDescriptor_PeakFrames";
		case PID_PCMDescriptor_PeakOfPeaksPosition:
			return L"PID_PCMDescriptor_PeakOfPeaksPosition";
		case PID_PCMDescriptor_PeakEnvelopeTimestamp:
			return L"PID_PCMDescriptor_PeakEnvelopeTimestamp";
		case PID_PCMDescriptor_PeakEnvelopeData:
			return L"PID_PCMDescriptor_PeakEnvelopeData";
		case PID_KLVDataDefinition_KLVDataType:
			return L"PID_KLVDataDefinition_KLVDataType";
		case PID_AuxiliaryDescriptor_MimeType:
			return L"PID_AuxiliaryDescriptor_MimeType";
		case PID_AuxiliaryDescriptor_CharSet:
			return L"PID_AuxiliaryDescriptor_CharSet";
		case PID_RIFFChunk_ChunkID:
			return L"PID_RIFFChunk_ChunkID";
		case PID_RIFFChunk_ChunkLength:
			return L"PID_RIFFChunk_ChunkLength";
		case PID_RIFFChunk_ChunkData:
			return L"PID_RIFFChunk_ChunkData";
		case PID_BWFImportDescriptor_QltyFileSecurityReport:
			return L"PID_BWFImportDescriptor_QltyFileSecurityReport";
		case PID_BWFImportDescriptor_QltyFileSecurityWave:
			return L"PID_BWFImportDescriptor_QltyFileSecurityWave";
		case PID_BWFImportDescriptor_BextCodingHistory:
			return L"PID_BWFImportDescriptor_BextCodingHistory";
		case PID_BWFImportDescriptor_QltyBasicData:
			return L"PID_BWFImportDescriptor_QltyBasicData";
		case PID_BWFImportDescriptor_QltyStartOfModulation:
			return L"PID_BWFImportDescriptor_QltyStartOfModulation";
		case PID_BWFImportDescriptor_QltyQualityEvent:
			return L"PID_BWFImportDescriptor_QltyQualityEvent";
		case PID_BWFImportDescriptor_QltyEndOfModulation:
			return L"PID_BWFImportDescriptor_QltyEndOfModulation";
		case PID_BWFImportDescriptor_QltyQualityParameter:
			return L"PID_BWFImportDescriptor_QltyQualityParameter";
		case PID_BWFImportDescriptor_QltyOperatorComment:
			return L"PID_BWFImportDescriptor_QltyOperatorComment";
		case PID_BWFImportDescriptor_QltyCueSheet:
			return L"PID_BWFImportDescriptor_QltyCueSheet";
		case PID_BWFImportDescriptor_UnknownBWFChunks:
			return L"PID_BWFImportDescriptor_UnknownBWFChunks";

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

			return L"MULTIPLE_VALUE_MATCHES : see libaaf.h";

			// return L"PID_MPEGVideoDescriptor_SingleSequence";
			// return L"PID_MPEGVideoDescriptor_ConstantBPictureCount";
			// return L"PID_MPEGVideoDescriptor_CodedContentScanning";
			// return L"PID_MPEGVideoDescriptor_LowDelay";
			// return L"PID_MPEGVideoDescriptor_ClosedGOP";
			// return L"PID_MPEGVideoDescriptor_IdenticalGOP";
			// return L"PID_MPEGVideoDescriptor_MaxGOP";
			// return L"PID_MPEGVideoDescriptor_MaxBPictureCount";
			// return L"PID_MPEGVideoDescriptor_BitRate";
			// return L"PID_MPEGVideoDescriptor_ProfileAndLevel";


		case PID_ClassDefinition_ParentClass:
			return L"PID_ClassDefinition_ParentClass";
		case PID_ClassDefinition_Properties:
			return L"PID_ClassDefinition_Properties";
		case PID_ClassDefinition_IsConcrete:
			return L"PID_ClassDefinition_IsConcrete";
		case PID_PropertyDefinition_Type:
			return L"PID_PropertyDefinition_Type";
		case PID_PropertyDefinition_IsOptional:
			return L"PID_PropertyDefinition_IsOptional";
		case PID_PropertyDefinition_LocalIdentification:
			return L"PID_PropertyDefinition_LocalIdentification";
		case PID_PropertyDefinition_IsUniqueIdentifier:
			return L"PID_PropertyDefinition_IsUniqueIdentifier";
		case PID_TypeDefinitionInteger_Size:
			return L"PID_TypeDefinitionInteger_Size";
		case PID_TypeDefinitionInteger_IsSigned:
			return L"PID_TypeDefinitionInteger_IsSigned";
		case PID_TypeDefinitionStrongObjectReference_ReferencedType:
			return L"PID_TypeDefinitionStrongObjectReference_ReferencedType";
		case PID_TypeDefinitionWeakObjectReference_ReferencedType:
			return L"PID_TypeDefinitionWeakObjectReference_ReferencedType";
		case PID_TypeDefinitionWeakObjectReference_TargetSet:
			return L"PID_TypeDefinitionWeakObjectReference_TargetSet";
		case PID_TypeDefinitionEnumeration_ElementType:
			return L"PID_TypeDefinitionEnumeration_ElementType";
		case PID_TypeDefinitionEnumeration_ElementNames:
			return L"PID_TypeDefinitionEnumeration_ElementNames";
		case PID_TypeDefinitionEnumeration_ElementValues:
			return L"PID_TypeDefinitionEnumeration_ElementValues";
		case PID_TypeDefinitionFixedArray_ElementType:
			return L"PID_TypeDefinitionFixedArray_ElementType";
		case PID_TypeDefinitionFixedArray_ElementCount:
			return L"PID_TypeDefinitionFixedArray_ElementCount";
		case PID_TypeDefinitionVariableArray_ElementType:
			return L"PID_TypeDefinitionVariableArray_ElementType";
		case PID_TypeDefinitionSet_ElementType:
			return L"PID_TypeDefinitionSet_ElementType";
		case PID_TypeDefinitionString_ElementType:
			return L"PID_TypeDefinitionString_ElementType";
		case PID_TypeDefinitionRecord_MemberTypes:
			return L"PID_TypeDefinitionRecord_MemberTypes";
		case PID_TypeDefinitionRecord_MemberNames:
			return L"PID_TypeDefinitionRecord_MemberNames";
		case PID_TypeDefinitionRename_RenamedType:
			return L"PID_TypeDefinitionRename_RenamedType";
		case PID_TypeDefinitionExtendibleEnumeration_ElementNames:
			return L"PID_TypeDefinitionExtendibleEnumeration_ElementNames";
		case PID_TypeDefinitionExtendibleEnumeration_ElementValues:
			return L"PID_TypeDefinitionExtendibleEnumeration_ElementValues";
		case PID_MetaDefinition_Identification:
			return L"PID_MetaDefinition_Identification";
		case PID_MetaDefinition_Name:
			return L"PID_MetaDefinition_Name";
		case PID_MetaDefinition_Description:
			return L"PID_MetaDefinition_Description";
		case PID_MetaDictionary_ClassDefinitions:
			return L"PID_MetaDictionary_ClassDefinitions";
		case PID_MetaDictionary_TypeDefinitions:
			return L"PID_MetaDictionary_TypeDefinitions";

		// default:
		// 	return L"Unknown value";
	}


	static wchar_t PIDText[1024];

	aafClass *Class = NULL;


	foreachClass( Class, aafd->Classes )
	{
		aafPropertyDef *PDef = NULL;

		foreachPropertyDefinition( PDef, Class->Properties )
		{
			if ( PDef->pid == pid )
			{
				swprintf( PIDText, 1024, L"%s%ls%s",
			 		(PDef->meta) ? ANSI_COLOR_YELLOW : "",
				 	 PDef->name,
				 	(PDef->meta) ? ANSI_COLOR_RESET : "" );

				return PIDText;
			}
		}

	}

	return L"Unknown value";
}


const wchar_t * ClassIDToText( AAF_Data *aafd, const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFClassID_Root ) )
		return L"AAFClassID_Root";
	else if ( auidCmp( auid, &AAFClassID_InterchangeObject ) )
		return L"AAFClassID_InterchangeObject";
	else if ( auidCmp( auid, &AAFClassID_Component ) )
		return L"AAFClassID_Component";
	else if ( auidCmp( auid, &AAFClassID_Segment ) )
		return L"AAFClassID_Segment";
	else if ( auidCmp( auid, &AAFClassID_EdgeCode ) )
		return L"AAFClassID_EdgeCode";
	else if ( auidCmp( auid, &AAFClassID_EssenceGroup ) )
		return L"AAFClassID_EssenceGroup";
	else if ( auidCmp( auid, &AAFClassID_Event ) )
		return L"AAFClassID_Event";
	else if ( auidCmp( auid, &AAFClassID_GPITrigger ) )
		return L"AAFClassID_GPITrigger";
	else if ( auidCmp( auid, &AAFClassID_CommentMarker ) )
		return L"AAFClassID_CommentMarker";
	else if ( auidCmp( auid, &AAFClassID_Filler ) )
		return L"AAFClassID_Filler";
	else if ( auidCmp( auid, &AAFClassID_OperationGroup ) )
		return L"AAFClassID_OperationGroup";
	else if ( auidCmp( auid, &AAFClassID_NestedScope ) )
		return L"AAFClassID_NestedScope";
	else if ( auidCmp( auid, &AAFClassID_Pulldown ) )
		return L"AAFClassID_Pulldown";
	else if ( auidCmp( auid, &AAFClassID_ScopeReference ) )
		return L"AAFClassID_ScopeReference";
	else if ( auidCmp( auid, &AAFClassID_Selector ) )
		return L"AAFClassID_Selector";
	else if ( auidCmp( auid, &AAFClassID_Sequence ) )
		return L"AAFClassID_Sequence";
	else if ( auidCmp( auid, &AAFClassID_SourceReference ) )
		return L"AAFClassID_SourceReference";
	else if ( auidCmp( auid, &AAFClassID_SourceClip ) )
		return L"AAFClassID_SourceClip";
	else if ( auidCmp( auid, &AAFClassID_TextClip ) )
		return L"AAFClassID_TextClip";
	else if ( auidCmp( auid, &AAFClassID_HTMLClip ) )
		return L"AAFClassID_HTMLClip";
	else if ( auidCmp( auid, &AAFClassID_Timecode ) )
		return L"AAFClassID_Timecode";
	else if ( auidCmp( auid, &AAFClassID_TimecodeStream ) )
		return L"AAFClassID_TimecodeStream";
	else if ( auidCmp( auid, &AAFClassID_TimecodeStream12M ) )
		return L"AAFClassID_TimecodeStream12M";
	else if ( auidCmp( auid, &AAFClassID_Transition ) )
		return L"AAFClassID_Transition";
	else if ( auidCmp( auid, &AAFClassID_ContentStorage ) )
		return L"AAFClassID_ContentStorage";
	else if ( auidCmp( auid, &AAFClassID_ControlPoint ) )
		return L"AAFClassID_ControlPoint";
	else if ( auidCmp( auid, &AAFClassID_DefinitionObject ) )
		return L"AAFClassID_DefinitionObject";
	else if ( auidCmp( auid, &AAFClassID_DataDefinition ) )
		return L"AAFClassID_DataDefinition";
	else if ( auidCmp( auid, &AAFClassID_OperationDefinition ) )
		return L"AAFClassID_OperationDefinition";
	else if ( auidCmp( auid, &AAFClassID_ParameterDefinition ) )
		return L"AAFClassID_ParameterDefinition";
	else if ( auidCmp( auid, &AAFClassID_PluginDefinition ) )
		return L"AAFClassID_PluginDefinition";
	else if ( auidCmp( auid, &AAFClassID_CodecDefinition ) )
		return L"AAFClassID_CodecDefinition";
	else if ( auidCmp( auid, &AAFClassID_ContainerDefinition ) )
		return L"AAFClassID_ContainerDefinition";
	else if ( auidCmp( auid, &AAFClassID_InterpolationDefinition ) )
		return L"AAFClassID_InterpolationDefinition";
	else if ( auidCmp( auid, &AAFClassID_Dictionary ) )
		return L"AAFClassID_Dictionary";
	else if ( auidCmp( auid, &AAFClassID_EssenceData ) )
		return L"AAFClassID_EssenceData";
	else if ( auidCmp( auid, &AAFClassID_EssenceDescriptor ) )
		return L"AAFClassID_EssenceDescriptor";
	else if ( auidCmp( auid, &AAFClassID_FileDescriptor ) )
		return L"AAFClassID_FileDescriptor";
	else if ( auidCmp( auid, &AAFClassID_AIFCDescriptor ) )
		return L"AAFClassID_AIFCDescriptor";
	else if ( auidCmp( auid, &AAFClassID_DigitalImageDescriptor ) )
		return L"AAFClassID_DigitalImageDescriptor";
	else if ( auidCmp( auid, &AAFClassID_CDCIDescriptor ) )
		return L"AAFClassID_CDCIDescriptor";
	else if ( auidCmp( auid, &AAFClassID_RGBADescriptor ) )
		return L"AAFClassID_RGBADescriptor";
	else if ( auidCmp( auid, &AAFClassID_HTMLDescriptor ) )
		return L"AAFClassID_HTMLDescriptor";
	else if ( auidCmp( auid, &AAFClassID_TIFFDescriptor ) )
		return L"AAFClassID_TIFFDescriptor";
	else if ( auidCmp( auid, &AAFClassID_WAVEDescriptor ) )
		return L"AAFClassID_WAVEDescriptor";
	else if ( auidCmp( auid, &AAFClassID_FilmDescriptor ) )
		return L"AAFClassID_FilmDescriptor";
	else if ( auidCmp( auid, &AAFClassID_TapeDescriptor ) )
		return L"AAFClassID_TapeDescriptor";
	else if ( auidCmp( auid, &AAFClassID_Header ) )
		return L"AAFClassID_Header";
	else if ( auidCmp( auid, &AAFClassID_Identification ) )
		return L"AAFClassID_Identification";
	else if ( auidCmp( auid, &AAFClassID_Locator ) )
		return L"AAFClassID_Locator";
	else if ( auidCmp( auid, &AAFClassID_NetworkLocator ) )
		return L"AAFClassID_NetworkLocator";
	else if ( auidCmp( auid, &AAFClassID_TextLocator ) )
		return L"AAFClassID_TextLocator";
	else if ( auidCmp( auid, &AAFClassID_Mob ) )
		return L"AAFClassID_Mob";
	else if ( auidCmp( auid, &AAFClassID_CompositionMob ) )
		return L"AAFClassID_CompositionMob";
	else if ( auidCmp( auid, &AAFClassID_MasterMob ) )
		return L"AAFClassID_MasterMob";
	else if ( auidCmp( auid, &AAFClassID_SourceMob ) )
		return L"AAFClassID_SourceMob";
	else if ( auidCmp( auid, &AAFClassID_MobSlot ) )
		return L"AAFClassID_MobSlot";
	else if ( auidCmp( auid, &AAFClassID_EventMobSlot ) )
		return L"AAFClassID_EventMobSlot";
	else if ( auidCmp( auid, &AAFClassID_StaticMobSlot ) )
		return L"AAFClassID_StaticMobSlot";
	else if ( auidCmp( auid, &AAFClassID_TimelineMobSlot ) )
		return L"AAFClassID_TimelineMobSlot";
	else if ( auidCmp( auid, &AAFClassID_Parameter ) )
		return L"AAFClassID_Parameter";
	else if ( auidCmp( auid, &AAFClassID_ConstantValue ) )
		return L"AAFClassID_ConstantValue";
	else if ( auidCmp( auid, &AAFClassID_VaryingValue ) )
		return L"AAFClassID_VaryingValue";
	else if ( auidCmp( auid, &AAFClassID_TaggedValue ) )
		return L"AAFClassID_TaggedValue";
	else if ( auidCmp( auid, &AAFClassID_KLVData ) )
		return L"AAFClassID_KLVData";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveMarker ) )
		return L"AAFClassID_DescriptiveMarker";
	else if ( auidCmp( auid, &AAFClassID_SoundDescriptor ) )
		return L"AAFClassID_SoundDescriptor";
	else if ( auidCmp( auid, &AAFClassID_DataEssenceDescriptor ) )
		return L"AAFClassID_DataEssenceDescriptor";
	else if ( auidCmp( auid, &AAFClassID_MultipleDescriptor ) )
		return L"AAFClassID_MultipleDescriptor";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveClip ) )
		return L"AAFClassID_DescriptiveClip";
	else if ( auidCmp( auid, &AAFClassID_AES3PCMDescriptor ) )
		return L"AAFClassID_AES3PCMDescriptor";
	else if ( auidCmp( auid, &AAFClassID_PCMDescriptor ) )
		return L"AAFClassID_PCMDescriptor";
	else if ( auidCmp( auid, &AAFClassID_PhysicalDescriptor ) )
		return L"AAFClassID_PhysicalDescriptor";
	else if ( auidCmp( auid, &AAFClassID_ImportDescriptor ) )
		return L"AAFClassID_ImportDescriptor";
	else if ( auidCmp( auid, &AAFClassID_RecordingDescriptor ) )
		return L"AAFClassID_RecordingDescriptor";
	else if ( auidCmp( auid, &AAFClassID_TaggedValueDefinition ) )
		return L"AAFClassID_TaggedValueDefinition";
	else if ( auidCmp( auid, &AAFClassID_KLVDataDefinition ) )
		return L"AAFClassID_KLVDataDefinition";
	else if ( auidCmp( auid, &AAFClassID_AuxiliaryDescriptor ) )
		return L"AAFClassID_AuxiliaryDescriptor";
	else if ( auidCmp( auid, &AAFClassID_RIFFChunk ) )
		return L"AAFClassID_RIFFChunk";
	else if ( auidCmp( auid, &AAFClassID_BWFImportDescriptor ) )
		return L"AAFClassID_BWFImportDescriptor";
	else if ( auidCmp( auid, &AAFClassID_MPEGVideoDescriptor ) )
		return L"AAFClassID_MPEGVideoDescriptor";
	else if ( auidCmp( auid, &AAFClassID_ClassDefinition ) )
		return L"AAFClassID_ClassDefinition";
	else if ( auidCmp( auid, &AAFClassID_PropertyDefinition ) )
		return L"AAFClassID_PropertyDefinition";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinition ) )
		return L"AAFClassID_TypeDefinition";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionInteger ) )
		return L"AAFClassID_TypeDefinitionInteger";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionStrongObjectReference ) )
		return L"AAFClassID_TypeDefinitionStrongObjectReference";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionWeakObjectReference ) )
		return L"AAFClassID_TypeDefinitionWeakObjectReference";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionEnumeration ) )
		return L"AAFClassID_TypeDefinitionEnumeration";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionFixedArray ) )
		return L"AAFClassID_TypeDefinitionFixedArray";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionVariableArray ) )
		return L"AAFClassID_TypeDefinitionVariableArray";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionSet ) )
		return L"AAFClassID_TypeDefinitionSet";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionString ) )
		return L"AAFClassID_TypeDefinitionString";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionStream ) )
		return L"AAFClassID_TypeDefinitionStream";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionRecord ) )
		return L"AAFClassID_TypeDefinitionRecord";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionRename ) )
		return L"AAFClassID_TypeDefinitionRename";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionExtendibleEnumeration ) )
		return L"AAFClassID_TypeDefinitionExtendibleEnumeration";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionIndirect ) )
		return L"AAFClassID_TypeDefinitionIndirect";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionOpaque ) )
		return L"AAFClassID_TypeDefinitionOpaque";
	else if ( auidCmp( auid, &AAFClassID_TypeDefinitionCharacter ) )
		return L"AAFClassID_TypeDefinitionCharacter";
	else if ( auidCmp( auid, &AAFClassID_MetaDefinition ) )
		return L"AAFClassID_MetaDefinition";
	else if ( auidCmp( auid, &AAFClassID_MetaDictionary ) )
		return L"AAFClassID_MetaDictionary";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveObject ) )
		return L"AAFClassID_DescriptiveObject";
	else if ( auidCmp( auid, &AAFClassID_DescriptiveFramework ) )
		return L"AAFClassID_DescriptiveFramework";


	if ( auidCmp( auid, &AUID_NULL ) )
		return L"AUID_NULL";



	static wchar_t ClassIDText[1024];

	ClassIDText[0] = '\0';

	aafClass *Class = NULL;

	foreachClass( Class, aafd->Classes )
	{

		if ( auidCmp( Class->ID, auid ) )
		{
			swprintf( ClassIDText, 1024, L"%s%ls%s",
		 		(Class->meta) ? ANSI_COLOR_YELLOW : "",
			 	 Class->name,
			 	(Class->meta) ? ANSI_COLOR_RESET : "" );

			return ClassIDText;
		}

	}

	return L"Unknown value";
}


const wchar_t * ContainerToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFContainerDef_External ) )
		return L"AAFContainerDef_External";

	if ( auidCmp( auid, &AAFContainerDef_OMF ) )
		return L"AAFContainerDef_OMF";

	if ( auidCmp( auid, &AAFContainerDef_AAF ) )
		return L"AAFContainerDef_AAF";

	if ( auidCmp( auid, &AAFContainerDef_AAFMSS ) )
		return L"AAFContainerDef_AAFMSS";

	if ( auidCmp( auid, &AAFContainerDef_AAFKLV ) )
		return L"AAFContainerDef_AAFKLV";

	if ( auidCmp( auid, &AAFContainerDef_AAFXML ) )
		return L"AAFContainerDef_AAFXML";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_DefinedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_ExtendedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_PictureOnly ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_DefinedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_ExtendedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_PictureOnly ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_DefinedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_ExtendedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_PictureOnly ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_DefinedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_ExtendedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_PictureOnly ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_DefinedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_ExtendedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_PictureOnly ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_DefinedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_DefinedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_ExtendedTemplate ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_ExtendedTemplate";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_PictureOnly ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_PictureOnly";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps_SMPTE322M ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps_SMPTE322M ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps_SMPTE322M ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps_SMPTE322M ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps_SMPTE322M";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_IECDV_UndefinedSource_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_IECDV_UndefinedSource_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_IECDV_UndefinedSource_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_IECDV_UndefinedSource_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_25Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_25Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_50Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_50Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_50Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_50Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_50Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x5994I_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x5994I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x5994I_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x5994I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x50I_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_1080x50I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x50I_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x50I_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_720x5994P_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_720x5994P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x5994P_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x5994P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_720x50P_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_720x50P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x50P_100Mbps ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_720x50P_100Mbps";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_DVbased_UndefinedSource ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_DVbased_UndefinedSource";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_DVbased_UndefinedSource ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_DVbased_UndefinedSource";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_MPEGES_VideoStream0_SID ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_MPEGES_VideoStream0_SID";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_CustomClosedGOPwrapped_MPEGES_VideoStream1_SID ) )
		return L"AAFContainerDef_MXFGC_CustomClosedGOPwrapped_MPEGES_VideoStream1_SID";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994I_720_422 ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994I_720_422 ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994I_720_422 ) )
		return L"AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50I_720_422 ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50I_720_422 ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50I_720_422 ) )
		return L"AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50I_720_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994P_960_422 ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994P_960_422 ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994P_960_422 ) )
		return L"AAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50P_960_422 ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50P_960_422 ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50P_960_422 ) )
		return L"AAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50P_960_422";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Broadcast_Wave_audio_data ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_Broadcast_Wave_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Broadcast_Wave_audio_data ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_Broadcast_Wave_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_AES3_audio_data ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_AES3_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_AES3_audio_data ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_AES3_audio_data";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_Alaw_Audio ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_Alaw_Audio";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_Alaw_Audio ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_Alaw_Audio";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Customwrapped_Alaw_Audio ) )
		return L"AAFContainerDef_MXFGC_Customwrapped_Alaw_Audio";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_AVCbytestream_VideoStream0_SID ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_AVCbytestream_VideoStream0_SID";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_VC3 ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_VC3";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_VC3 ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_VC3";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Framewrapped_VC1 ) )
		return L"AAFContainerDef_MXFGC_Framewrapped_VC1";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Clipwrapped_VC1 ) )
		return L"AAFContainerDef_MXFGC_Clipwrapped_VC1";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Generic_Essence_Multiple_Mappings ) )
		return L"AAFContainerDef_MXFGC_Generic_Essence_Multiple_Mappings";

	if ( auidCmp( auid, &AAFContainerDef_RIFFWAVE ) )
		return L"AAFContainerDef_RIFFWAVE";

	if ( auidCmp( auid, &AAFContainerDef_JFIF ) )
		return L"AAFContainerDef_JFIF";

	if ( auidCmp( auid, &AAFContainerDef_AIFFAIFC ) )
		return L"AAFContainerDef_AIFFAIFC";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220X_1080p ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_220X_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_1080p ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_145_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220_1080p ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_220_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_36_1080p ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_36_1080p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220X_1080i ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_220X_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_1080i ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_145_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220_1080i ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_220_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_1440_1080i ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_145_1440_1080i";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220X_720p ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_220X_720p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_220_720p ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_220_720p";

	if ( auidCmp( auid, &AAFContainerDef_MXFGC_Avid_DNX_145_720p ) )
		return L"AAFContainerDef_MXFGC_Avid_DNX_145_720p";


	return L"Unknown value";
}


const wchar_t * CompressionToText( const aafUID_t *auid )
{
	if ( auid == NULL )
		return L"n/a";

	if ( auidCmp( auid, &AAFCompressionDef_AAF_CMPR_FULL_JPEG ) )
		return L"AAFCompressionDef_AAF_CMPR_FULL_JPEG";

	if ( auidCmp( auid, &AAFCompressionDef_AAF_CMPR_AUNC422 ) )
		return L"AAFCompressionDef_AAF_CMPR_AUNC422";

	if ( auidCmp( auid, &AAFCompressionDef_LegacyDV ) )
		return L"AAFCompressionDef_LegacyDV";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_50Mbps_625x50I ) )
		return L"AAFCompressionDef_SMPTE_D10_50Mbps_625x50I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_50Mbps_525x5994I ) )
		return L"AAFCompressionDef_SMPTE_D10_50Mbps_525x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_40Mbps_625x50I ) )
		return L"AAFCompressionDef_SMPTE_D10_40Mbps_625x50I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_40Mbps_525x5994I ) )
		return L"AAFCompressionDef_SMPTE_D10_40Mbps_525x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_30Mbps_625x50I ) )
		return L"AAFCompressionDef_SMPTE_D10_30Mbps_625x50I";

	if ( auidCmp( auid, &AAFCompressionDef_SMPTE_D10_30Mbps_525x5994I ) )
		return L"AAFCompressionDef_SMPTE_D10_30Mbps_525x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_IEC_DV_525_60 ) )
		return L"AAFCompressionDef_IEC_DV_525_60";

	if ( auidCmp( auid, &AAFCompressionDef_IEC_DV_625_50 ) )
		return L"AAFCompressionDef_IEC_DV_625_50";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_25Mbps_525_60 ) )
		return L"AAFCompressionDef_DV_Based_25Mbps_525_60";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_25Mbps_625_50 ) )
		return L"AAFCompressionDef_DV_Based_25Mbps_625_50";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_50Mbps_525_60 ) )
		return L"AAFCompressionDef_DV_Based_50Mbps_525_60";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_50Mbps_625_50 ) )
		return L"AAFCompressionDef_DV_Based_50Mbps_625_50";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_1080x5994I ) )
		return L"AAFCompressionDef_DV_Based_100Mbps_1080x5994I";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_1080x50I ) )
		return L"AAFCompressionDef_DV_Based_100Mbps_1080x50I";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_720x5994P ) )
		return L"AAFCompressionDef_DV_Based_100Mbps_720x5994P";

	if ( auidCmp( auid, &AAFCompressionDef_DV_Based_100Mbps_720x50P ) )
		return L"AAFCompressionDef_DV_Based_100Mbps_720x50P";

	if ( auidCmp( auid, &AAFCompressionDef_VC3_1 ) )
		return L"AAFCompressionDef_VC3_1";

	if ( auidCmp( auid, &AAFCompressionDef_Avid_DNxHD_Legacy ) )
		return L"AAFCompressionDef_Avid_DNxHD_Legacy";


	if ( auidCmp( auid, &AUID_NULL ) )
		return L"";

	return L"Unknown value";
}
