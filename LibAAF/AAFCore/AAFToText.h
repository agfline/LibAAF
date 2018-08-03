#include <stdio.h>
#include <wchar.h>

#include "AAFTypes.h"
#include "AAFClass.h"

#include "../LibCFB/CFBTypes.h"
#include "../LibCFB/CFBDump.h"

#include "../common/utils.h"



#define AUIDToText( auid ) \
    CLSIDToText( (cfbCLSID_t*)auid )


const wchar_t * MobIDToText( aafMobID_t *mobid );

const wchar_t * TimestampToText( aafTimeStamp_t *ts );

const wchar_t * VersionToText( aafVersionType_t *vers );

const wchar_t * ProductVersionToText( aafProductVersion_t *vers );


const wchar_t * FileKindToText( const aafUID_t *auid );

const wchar_t * TapeCaseTypeToText( aafTapeCaseType_t t );

const wchar_t * VideoSignalTypeToText( aafVideoSignalType_t v );

const wchar_t * TapeFormatTypeToText( aafTapeFormatType_t t );

const wchar_t * FilmTypeToText( aafFilmType_t f );

const wchar_t * SignalStandardToText( aafSignalStandard_t s );

const wchar_t * FieldNumberToText( aafFieldNumber_t f );

const wchar_t * AlphaTransparencyToText( aafAlphaTransparency_t a );

const wchar_t * FrameLayoutToText( aafFrameLayout_t f );

const wchar_t * ColorSitingToText( aafColorSiting_t c );

const wchar_t * ProductReleaseTypeToText( aafProductReleaseType_t t );

const wchar_t * FadeTypeToText( aafFadeType_t f );

const wchar_t * BoolToText( aafBoolean_t b );

const wchar_t * OperationCategoryToText( const aafUID_t *auid );

const wchar_t * PluginCategoryToText( const aafUID_t *auid );

const wchar_t * ScanningDirectionToText( aafScanningDirection_t s );

const wchar_t * ByteOrderToText( int16_t e );

const wchar_t * ElectroSpatialToText( aafElectroSpatialFormulation_t e );

const wchar_t * TypeIDToText( const aafUID_t *auid );

const wchar_t * OPDefToText( const aafUID_t *auid );

const wchar_t * DataDefToText( AAF_Data *aafd, const aafUID_t *auid );

const wchar_t * OperationDefToText( AAF_Data *aafd, const aafUID_t *auid );

const wchar_t * InterpolationToText( const aafUID_t *auid );

const wchar_t * ParameterToText( AAF_Data *aafd, const aafUID_t *auid );

const wchar_t * TransferCharacteristicToText( const aafUID_t *auid );

const wchar_t * CodingEquationsToText( const aafUID_t *auid );

const wchar_t * ColorPrimariesToText( const aafUID_t *auid );

const wchar_t * UsageCodeToText( const aafUID_t *auid );

const wchar_t * StoredFormToText( uint16_t sf );

const wchar_t * PIDToText( AAF_Data *aafd, aafPID_t pid );

const wchar_t * ClassIDToText( AAF_Data *aafd, const aafUID_t *auid );

const wchar_t * ContainerToText( const aafUID_t *auid );

const wchar_t * CompressionToText( const aafUID_t *auid );
