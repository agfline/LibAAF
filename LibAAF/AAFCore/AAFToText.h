#include <stdio.h>

#include "AAFTypes.h"
#include "AAFClass.h"
#include "../common/utils.h"



const char * TimestampToText( aafTimeStamp_t *ts );

const char * VersionToText( aafVersionType_t *vers );

const char * ProductVersionToText( aafProductVersion_t *vers );

const char * AUIDToText( aafUID_t *auid );



const char * FileKindToText( const aafUID_t *auid );

const char * TapeCaseTypeToText( aafTapeCaseType_t t );

const char * VideoSignalTypeToText( aafVideoSignalType_t v );

const char * TapeFormatTypeToText( aafTapeFormatType_t t );

const char * FilmTypeToText( aafFilmType_t f );

const char * SignalStandardToText( aafSignalStandard_t s );

const char * FieldNumberToText( aafFieldNumber_t f );

const char * AlphaTransparencyToText( aafAlphaTransparency_t a );

const char * FrameLayoutToText( aafFrameLayout_t f );

const char * ColorSitingToText( aafColorSiting_t c );

const char * ProductReleaseTypeToText( aafProductReleaseType_t t );

const char * FadeTypeToText( aafFadeType_t f );

const char * BoolToText( aafBoolean_t b );

const char * OperationCategoryToText( const aafUID_t *auid );

const char * PluginCategoryToText( const aafUID_t *auid );

const char * ScanningDirectionToText( aafScanningDirection_t s );

const char * ByteOrderToText( int16_t e );

const char * ElectroSpatialToText( aafElectroSpatialFormulation_t e );

const char * TypeIDToText( const aafUID_t *auid );

const char * OPDefToText( const aafUID_t *auid );

const char * DataDefToText( AAF_Data *aafd, const aafUID_t *auid );

const char * OperationDefToText( AAF_Data *aafd, const aafUID_t *auid );

const char * InterpolationToText( const aafUID_t *auid );

const char * ParameterToText( AAF_Data *aafd, const aafUID_t *auid );

const char * TransferCharacteristicToText( const aafUID_t *auid );

const char * CodingEquationsToText( const aafUID_t *auid );

const char * ColorPrimariesToText( const aafUID_t *auid );

const char * UsageCodeToText( const aafUID_t *auid );

const char * StoredFormToText( uint16_t sf );

const char * PIDToText( AAF_Data *aafd, aafPID_t pid );

const char * ClassIDToText( AAF_Data *aafd, const aafUID_t *auid );

const char * ContainerToText( const aafUID_t *auid );

const char * CompressionToText( const aafUID_t *auid );
