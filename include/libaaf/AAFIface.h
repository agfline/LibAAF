/*
 * Copyright (C) 2017-2024 Adrien Gesta-Fline
 *
 * This file is part of libAAF.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __AAFIface_h__
#define __AAFIface_h__

/**
 * @file LibAAF/AAFIface/AAFIface.h
 * @brief AAF processing
 */


#include <libaaf/AAFTypes.h>
#include <libaaf/AAFCore.h>


/**
 * @defgroup AAFIface AAFIface
 * @ingroup private_api
 *
 * @{
 */

/**
 * @brief Transition type flag for aafiTransition.flags
 */

typedef enum aafiTransition_e
{
	AAFI_TRANS_SINGLE_CURVE = 1<<4,
	AAFI_TRANS_TWO_CURVE    = 1<<5,

	AAFI_TRANS_FADE_IN      = 1<<6,
	AAFI_TRANS_FADE_OUT     = 1<<7,
	AAFI_TRANS_XFADE        = 1<<8,

} aafiTransition_e;

#define AAFI_TRANS_CURVE_COUNT_MASK ( \
    AAFI_TRANS_SINGLE_CURVE           \
  | AAFI_TRANS_TWO_CURVE              \
)

#define AAFI_TRANS_FADE_MASK ( \
    AAFI_TRANS_FADE_IN         \
  | AAFI_TRANS_FADE_OUT        \
  | AAFI_TRANS_XFADE           \
)

/**
 * @}
 */


/**
 * @ingroup public_api Public API
 * @addtogroup public_api Public API
 * @{
 * @brief Library public API
 */



/**
 * @brief Audio gain flag for aafiAudioGain.flags and aafiTransition.flags
 */

typedef enum aafiAudioGain_e
{
	AAFI_AUDIO_GAIN_CONSTANT = 1<<0,
	AAFI_AUDIO_GAIN_VARIABLE = 1<<1,

} aafiAudioGain_e;

#define AAFI_AUDIO_GAIN_MASK ( \
    AAFI_AUDIO_GAIN_CONSTANT   \
  | AAFI_AUDIO_GAIN_VARIABLE   \
)


/**
 * @brief Flags for aafiTransition.flags and aafiAudioGain.flags
 */

typedef enum aafiInterpolation_e
{
	AAFI_INTERPOL_NONE     = 1<<10,
	AAFI_INTERPOL_LINEAR   = 1<<11,
	AAFI_INTERPOL_LOG      = 1<<12,
	AAFI_INTERPOL_CONSTANT = 1<<13,
	AAFI_INTERPOL_POWER    = 1<<14,
	AAFI_INTERPOL_BSPLINE  = 1<<15,

} aafiInterpolation_e;

#define AAFI_INTERPOL_MASK (unsigned)( \
    AAFI_INTERPOL_NONE                 \
  | AAFI_INTERPOL_LINEAR               \
  | AAFI_INTERPOL_LOG                  \
  | AAFI_INTERPOL_CONSTANT             \
  | AAFI_INTERPOL_POWER                \
  | AAFI_INTERPOL_BSPLINE              \
)







/**
 * Specifies a Transition which can be a fade in, a fade out or a Cross fade, and
 * which can have one or two curves.
 *
 * With a single curve (AAFI_TRANS_SINGLE_CURVE), the same curve is mirrored and applied
 * as fade in and fade out to obtain a cross fade.
 *
 * Having two curves (AAFI_TRANS_TWO_CURVE) allows a cross fade to have one curve per fade.
 *
 * A transition should have at least two points, one at time zero and one at time 1.
 */

typedef struct aafiTransition
{
	struct aafiTimelineItem *timelineItem;

	/**
	 * Should hold the transition type (either single param or two param),
	 * the transition fade type (in, out, x) and the interpolation used.
	 */

	uint32_t        flags;


	/**
	 * Length of the transition, in edit units.
	 */

	aafPosition_t   len;


	/**
	 * The cut point. In the case the transition is removed or cannot be played, the
	 * cut point specifies where in the transition, the preceding segment should end
	 * and where the following segment should start.
	 */

	aafPosition_t   cut_pt;


	/**
	 * Points count for the single curve, or the first one of the two. This specifies
	 * both the number of points (time/value) in the transition curve, and consequently
	 * the size of time_a[] and value_a[] arrays.
	 */

	int             pts_cnt_a;


	/**
	 * Array of time points, where the corresponding level value should apply either to
	 * the single curve, or to the first one of the two.
	 */

	aafRational_t  *time_a;


	/**
	 * Multiplier level values, each one applying at the corresponding indexed time for
	 * either the single curve, or the first one of the two.
	 * The interval between two points shall be calculated using the specified
	 * interpolation.
	 */

	aafRational_t  *value_a;


	/**
	 * Points count for the second curve, only when Transition has the AAFI_TRANS_TWO_CURVE
	 * flag. This specifies both the number of points (time/value) in the transition curve,
	 * and consequently the size of time_b[] and value_b[] arrays.
	 */

	int             pts_cnt_b;


	/**
	 * Array of time points, where the corresponding level value should apply to the
	 * second curve. Used only if Transition has the AAFI_TRANS_TWO_CURVE flag.
	 */

	aafRational_t **time_b;


	/**
	 * Multiplier level values, each one applying at the corresponding indexed time.
	 * The interval between two points shall be calculated using the specified
	 * interpolation. Used only if Transitions has the AAFI_TRANS_TWO_CURVE flag.
	 */

	aafRational_t **value_b;

} aafiTransition;




/**
 * Specifies a Gain to apply either to a Clip (aafiAudioClip.gain) or to an entire Track
 * (aafiAudioTrack.gain), that is to all the Clips contained by that Track.
 *
 * A Gain can be of to types :
 *
 * 	* Constant (AAFI_AUDIO_GAIN_CONSTANT) : A Constant gain specifies a single value
 * 	  as a multiplier to be applied to the Clip or Track.
 *
 * 	* Variable (AAFI_AUDIO_GAIN_VARIABLE) : A Variable gain specifies multiple points
 * 	  ( time / value ) that form all together the automation curve. The values between
 * 	  two points are calculated by interpolating between the two values.
 *
 * Both the Gain type and the interpolation mode are specified in the aafiAudioGain.flags
 * with the values from aafiAudioGain_e and aafiInterpolation_e.
 *
 * In the case of a Constant Gain, the single multiplier value should be retrieved from
 * aafiAudioGain.value[0].
 */

typedef struct aafiAudioGain
{
	/**
	 * Should hold the gain type (either Constant or Variable), and if it is Variable,
	 * the interpolation used to calculate the values between two time points.
	 */

	uint32_t        flags;	// Type : Constant (single multiplier for entire clip) or
	                        //        Variable (automation)
	                        // Interpolation : Linear, Log, Constant, Power, BSpline


	/**
	 * Points count. This specifies both the number of points (time/value) in the
	 * gain automation, and is consequently the size of time[] and value[] arrays.
	 */

	unsigned int    pts_cnt;


	/**
	 * Array of time points, where the corresponding level value should apply.
	 */

	aafRational_t  *time;


	/**
	 * Multiplier level values, each one applying at the corresponding indexed time.
	 * The interval between two points shall be calculated using the specified
	 * interpolation.
	 */

	aafRational_t  *value;

} aafiAudioGain;


typedef struct aafiAudioGain aafiAudioPan;


typedef struct aafiMetaData
{
	char    *name;
	char    *text;

	struct aafiMetaData *next;

} aafiMetaData;




/* forward declaration */
struct aafiAudioTrack;
struct aafiVideoTrack;
struct aafiAudioEssenceFile;
struct aafiVideoEssenceFile;
struct aafiAudioEssencePointer;

typedef struct aafiAudioClip
{
	/**
	 * The track that audio clip is attached to.
	 */
	struct aafiAudioTrack    *track;

	/**
	 * Audio channel count of audio clip. Might be different of #aafiAudioEssenceFile.channels
	 */
	int                       channels;

	/**
	 * List of essence pointer that compose an audio clip.
	 *
	 * An audio clip can hold a single aafiAudioEssencePointer, if clip is mono or
	 * Each aafiAudioEssencePointer points to a single aafiAudioEssenceFile.
	 *
	 * If clip is mono, it has a single aafiAudioEssencePointer which points to a
	 * single aafiAudioEssenceFile.
	 */
	struct aafiAudioEssencePointer  *essencePointerList;

	/**
	 * SubCLip name is set in rare cases, when a SourceClip targets a SubClip
	 * CompositionMob with a name (see AAFUsage_SubClip)
	 */
	char                     *subClipName;

	/*
	 * Some editors (like Resolve) support automation attached to a clip AND a fixed value clip gain
	 */
	aafiAudioGain            *gain;
	aafiAudioGain            *automation;

	int                       mute;

	aafPosition_t             pos; /* in edit unit, edit rate definition is aafiAudioTrack->edit_rate */

	aafPosition_t             len; /* in edit unit, edit rate definition is aafiAudioTrack->edit_rate */

	/*
	 * Start position in source file, set from SourceClip::StartTime
	 *
	 * « Specifies the offset from the origin of the referenced Mob MobSlot in edit units
	 * determined by the SourceClip object’s context. »
	 *
	 * « A SourceClip’s StartTime and Length values are in edit units determined by the slot
	 * owning the SourceClip. »

	 * « Informative note: If the SourceClip references a MobSlot that specifies a different
	 * edit rate than the MobSlot owning the SourceClip, the StartTime and Length are in
	 * edit units of the slot owning the SourceClip, and not edit units of the referenced slot.»
	 */

	 /*
	  * set with CompoMob's SourceClip::StartTime. In the case of an OperationGroup(AudioChannelCombiner),
		* There is one SourceClip per audio channel. So even though it's very unlikely, there could possibly
		* be one essence_offset per channel.
	  * Value is in edit unit, edit rate definition is aafiAudioTrack->edit_rate
	  */
	aafPosition_t             essence_offset;

	aafiMetaData             *metadata;


	struct aafiTimelineItem  *timelineItem; // Corresponding timeline item

} aafiAudioClip;



typedef struct aafiVideoClip
{
	struct aafiVideoTrack *track;

	struct aafiVideoEssenceFile *Essence;

	aafPosition_t          pos;

	aafPosition_t          len;

	aafPosition_t          essence_offset; // start position in the source file

	aafMobID_t            *masterMobID; // MobID of the associated MasterMob (PID_SourceReference_SourceID)

	struct aafiTimelineItem  *timelineItem; // Corresponding timeline item

} aafiVideoClip;



typedef enum aafiTimelineItem_type_e
{
	AAFI_AUDIO_CLIP  = 0x0001,
	AAFI_VIDEO_CLIP  = 0x0002,
	AAFI_TRANS       = 0x0003,

} aafiTimelineItem_type_e;



/**
 * This structure can old either an aafiAudioClip, aafiVideoClip or an aafiTransition struct.
 */

typedef struct aafiTimelineItem
{
	/**
	 * Identifies timelineItem type.
	 */
	aafiTimelineItem_type_e  type;

	/**
	 * Item position on the timeline. Value is in edit unit, specified by #aafiAudioTrack.edit_rate.
	 */
	aafPosition_t            pos;

	/**
	 * Item length on the timeline. Value is in edit unit, specified by #aafiAudioTrack.edit_rate.
	 */
	aafPosition_t            len;

	/**
	 * The actual timelineItem object data. Can hold an aafiTransition, aafiAudioClip
	 * or aafiVideoClip structure, depending on #aafiTimelineItem.type value.
	 */
	void                    *data;

	struct aafiTimelineItem *next;
	struct aafiTimelineItem *prev;

} aafiTimelineItem;



/**
 *
 */

typedef struct aafiTimecode
{
	/**
	 * Timecode start in EditUnit. (session start)
	 */

	aafPosition_t  start;


	/**
	 * Frame per second.
	 */

	uint16_t       fps;


	/**
	 * Indicates whether the timecode is drop (True value) or nondrop (False value)
	 */

	uint8_t        drop;


	/**
	 * Keeps track of the TimelineMobSlot EditRate.
	 * TODO do we care ?
	 */

	aafRational_t *edit_rate;

} aafiTimecode;



/**
 * Values for aafiAudioTrack.format.
 */

typedef enum aafiTrackFormat_e
{
	AAFI_TRACK_FORMAT_NOT_SET = 0,
	AAFI_TRACK_FORMAT_MONO    = 1,
	AAFI_TRACK_FORMAT_STEREO  = 2,
	AAFI_TRACK_FORMAT_5_1     = 6,
	AAFI_TRACK_FORMAT_7_1     = 8,
	AAFI_TRACK_FORMAT_UNKNOWN = 99

} aafiTrackFormat_e;



/* forward declaration */
struct aafiAudio;
struct aafiVideo;

typedef struct aafiAudioTrack
{
	/**
	 * Track number
	 */

	uint32_t                 number;


	uint16_t                 format; // aafiTrackFormat_e, value = channel count

	/**
	 * Track name
	 */

	char                    *name;


	/**
	 * Holds the Gain to apply on that track, that is the track volume Fader.
	 */

	aafiAudioGain           *gain;


	aafiAudioPan            *pan;

	char                     solo;
	char                     mute;


	/**
	 * Holds the timeline items of that track, that is aafiAudioClip and aafiTransition
	 * structures.
	 */

	struct aafiTimelineItem *timelineItems;
	int                      clipCount;


	/**
	 * The edit rate of all the contained Clips, Transitions, also lengths and track->current_pos;
	 */

	aafRational_t           *edit_rate;


	/**
	 * Pointer to the aafiAudio for convenient access.
	 */

	struct aafiAudio        *Audio;


	/**
	 * Pointer to the next aafiAudioTrack structure in the aafiAudio.Tracks list.
	 */

	aafPosition_t            current_pos;


	struct aafiAudioTrack   *next;

} aafiAudioTrack;



typedef struct aafiVideoTrack
{
	/**
	 * Track number
	 * TODO Should it start at one ?
	 * TODO Optional, should have a guess (i++) option.
	 */

	uint32_t                 number;


	/**
	 * Track name
	 */

	char                    *name;


	/**
	 * Holds the timeline items of that track, that is aafiVideoClip and aafiTransition
	 * structures.
	 */

	struct aafiTimelineItem *timelineItems;


	/**
	 * The edit rate of all the contained Clips and Transitions.
	 */

	aafRational_t           *edit_rate;


	/**
	 * Pointer to the aafiVideo for convenient access.
	 */

	struct aafiVideo        *Video;


	/**
	 * Pointer to the next aafiVideoTrack structure in the aafiVideo.Tracks list.
	 */

	aafPosition_t            current_pos;


	struct aafiVideoTrack   *next;


} aafiVideoTrack;




typedef struct aafiAudio
{
	/**
	 * Holds the sequence start timecode.
	 */

	aafPosition_t            start;

	uint16_t                 samplesize;
	uint32_t                 samplerate;
	aafRational_t           *samplerateRational; // eg. { 48000, 1 }

	/**
	 * Holds the Essence list.
	 */

	int                      essenceCount;
	struct aafiAudioEssenceFile    *essenceFiles;
	struct aafiAudioEssencePointer *essencePointers; /* only used for freeing */
	/**
	 * Holds the Track list.
	 */

	aafiAudioTrack          *Tracks;
	uint32_t                 track_count;

} aafiAudio;



typedef struct aafiVideo
{
	/**
	 * Holds the sequence start timecode.
	 */

	aafPosition_t     start;


	/**
	 * Holds the Essence list.
	 */

	struct aafiVideoEssenceFile *essenceFiles;

	/**
	 * Holds the Track list.
	 */

	aafiVideoTrack   *Tracks;

} aafiVideo;



typedef struct aafiMarker
{
	/*
	 * TODO: link marker to specific track ? (optional in AAF standard, not yet seen in AAF files)
	 */

	aafPosition_t     start;
	aafPosition_t     length;
	aafRational_t    *edit_rate;

	char             *name;
	char             *comment;
	uint16_t          RGBColor[3];

	struct aafiMarker *prev;
	struct aafiMarker *next;

} aafiMarker;



typedef struct aafiContext
{
	/* Clip */
	aafObject        *TopLevelCompositionMob;

	aafiAudioTrack   *current_track;

	aafiAudioClip    *current_clip;
	aafiVideoClip    *current_video_clip;
	int               current_clip_is_muted;

	int               current_clip_is_combined; // Inside OperationGroup::AAFOperationDef_AudioChannelCombiner
	uint32_t          current_combined_clip_total_channel;
	int               current_combined_clip_channel_num; // current SourceClip represents channel num
	aafPosition_t     current_combined_clip_forced_length;
	/* Transition */

	aafiTransition   *current_transition;

	char             *subClipName;

	int               current_opgroup_affect_track;

	/* Gain */

	aafiAudioGain    *current_clip_gain;
	int               current_clip_gain_is_used; // if not then free( current_clip_gain );

	aafiAudioGain    *current_clip_variable_gain;
	int               current_clip_variable_gain_is_used;

	/* Essence */

	struct aafiAudioEssenceFile *current_audio_essence;
	struct aafiVideoEssenceFile *current_video_essence;


	aafRational_t    *current_markers_edit_rate;

	aafRational_t    *avid_warp_clip_edit_rate;


	struct options {

		int              trace;
		int              dump_meta;
		int              dump_tagged_value;
		char            *dump_class_aaf_properties;
		char            *dump_class_raw_properties;
		char            *media_location;
		int              mobid_essence_filename;

		/* vendor specific */
		int              protools;
	} options;

} aafiContext;





typedef struct AAF_Iface
{
	aafiContext       ctx;

	/**
	 * Keeps track of the AAF_Data structure.
	 */

	AAF_Data         *aafd;


	aafiAudio        *Audio;

	aafiVideo        *Video;

	aafiTimecode     *Timecode;

	aafiMarker       *Markers;


	char             *compositionName;

	aafPosition_t     compositionStart; // sets from aafi->Timecode->start
	aafRational_t    *compositionStart_editRate;

	aafPosition_t     compositionLength; // sets from the longest audio or video track->current_pos
	aafRational_t    *compositionLength_editRate; /* might be NULL if file empty ! */


	/**
	 * Metadata retrieved from main CompositionMob::UserComments
	 */
	aafiMetaData     *metadata;


	struct aafLog       *log;

} AAF_Iface;





/**
 * @name Function macros
 * @{
 */

/**
 * Loops through each audio track in AAF file.
 *
 * @param aafi        Pointer to the current AAF_Iface struct.
 * @param audioTrack  NULL pointer to an aafiAudioTrack struct.
 */
#define AAFI_foreachAudioTrack( aafi, audioTrack ) \
  for ( audioTrack  = aafi->Audio->Tracks;         \
        audioTrack != NULL;                        \
        audioTrack  = audioTrack->next )           \

/**
 * Loops through each video track in AAF file.
 *
 * @param aafi        Pointer to the current AAF_Iface struct.
 * @param videoTrack  NULL pointer to an aafiVideoTrack struct.
 */
#define AAFI_foreachVideoTrack( aafi, videoTrack ) \
  for ( videoTrack  = aafi->Video->Tracks;         \
        videoTrack != NULL;                        \
        videoTrack  = videoTrack->next )           \

/**
 * Loops through each timelineItem of a track. Audio tracks can contain
 * either clips (AAFI_AUDIO_CLIP) or crossfades (AAFI_TRANS).
 *
 * @param track         Pointer to an aafiAudioTrack struct.
 * @param timelineItem  NULL pointer to an aafiTimelineItem struct.
 */
#define AAFI_foreachTrackItem( track, timelineItem ) \
  for ( timelineItem  = track->timelineItems;        \
        timelineItem != NULL;                        \
        timelineItem  = timelineItem->next )         \



/**
 * Loops through each marker in AAF file.
 *
 * @param aafi   Pointer to the current AAF_Iface struct.
 * @param marker NULL pointer to an aafiMarker struct.
 */
#define AAFI_foreachMarker( aafi, marker ) \
  for ( marker = aafi->Markers;            \
        marker != NULL;                    \
        marker = marker->next )            \


#define AAFI_foreachMetadata( metadataList, metadata ) \
  for ( metadata = metadataList;                       \
        metadata != NULL;                              \
        metadata = metadata->next )                    \

/**
 * @}
 */


/**
 * @addtogroup libaaf_api_init Initialization
 *
 * Typical library usage begins as follow :
 *
 * @code{.c}
#include <libaaf.h>

// allocate a new AAF_Iface object
AAF_Iface *aafi = aafi_alloc( NULL );

if ( !aafi ) {
	// error
}

// set verbosity level, optional file pointer or callback function
aafi_set_debug( aafi, VERB_DEBUG, 1, stdout, NULL, NULL );

// set libaaf options
aafi_set_option_int( aafi, "trace", 1 );
aafi_set_option_int( aafi, "dump_meta", 1 );
aafi_set_option_int( aafi, "dump_tagged_value", 1 );
aafi_set_option_int( aafi, "protools", (AAFI_PROTOOLS_OPT_REPLACE_CLIP_FADES | AAFI_PROTOOLS_OPT_REMOVE_SAMPLE_ACCURATE_EDIT) );
aafi_set_option_str( aafi, "media_location", "./media/" );
aafi_set_option_str( aafi, "dump_class_aaf_properties", "AAFClassID_TimelineMobSlot" );

// load AAF file
if ( aafi_load_file( aafi, "./file.aaf" ) ) {
 	// error
}

// ...

aafi_release( &aafi );
 * @endcode
 *
 * @{
 */

AAF_Iface * aafi_alloc( AAF_Data *aafd );

void aafi_set_debug( AAF_Iface *aafi, verbosityLevel_e v, int ansicolor, FILE *fp, void (*callback)(struct aafLog *log, void *ctxdata, int lib, int type, const char *srcfile, const char *srcfunc, int lineno, const char *msg, void *user), void *user );

int aafi_set_option_int( AAF_Iface *aafi, const char *optname, int val );

int aafi_set_option_str( AAF_Iface *aafi, const char *optname, const char *val );

int aafi_load_file( AAF_Iface *aafi, const char *file );

void aafi_release( AAF_Iface **aafi );

/**
 * @}
 */

aafiAudioClip * aafi_timelineItemToAudioClip( aafiTimelineItem *audioItem );

aafiTransition * aafi_timelineItemToCrossFade( aafiTimelineItem *audioItem );

aafiTransition * aafi_getFadeIn( aafiAudioClip *audioClip );

aafiTransition * aafi_getFadeOut( aafiAudioClip *audioClip );

int aafi_get_clipIndex( aafiAudioClip *audioClip );


aafPosition_t aafi_convertUnit( aafPosition_t value, aafRational_t *valueEditRate, aafRational_t *destEditRate );

uint64_t aafi_convertUnitUint64( aafPosition_t value, aafRational_t *valueEditRate, aafRational_t *destEditRate );




/*
 * @ingroup AAFIface
 * @addtogroup AAFIface
 * @{
 * @brief Abstraction layer to interpret the Objects/Class and retrieve data.
 */

int aafi_removeTimelineItem( AAF_Iface *aafi, aafiTimelineItem *timelineItem );

int aafi_applyGainOffset( AAF_Iface *aafi, aafiAudioGain **gain, aafiAudioGain *offset );


aafiAudioTrack * aafi_newAudioTrack( AAF_Iface *aafi );

aafiVideoTrack * aafi_newVideoTrack( AAF_Iface *aafi );


aafiTimelineItem * aafi_newTimelineItem( AAF_Iface *aafi, void *track, int itemType, void *data );

aafiAudioClip * aafi_newAudioClip( AAF_Iface *aafi, aafiAudioTrack *track );

aafiVideoClip * aafi_newVideoClip( AAF_Iface *aafi, aafiVideoTrack *track );

aafiTransition * aafi_newTransition( AAF_Iface *aafi, aafiAudioTrack *track );


aafiMarker * aafi_newMarker( AAF_Iface *aafi, aafRational_t *editRate, aafPosition_t start, aafPosition_t length, char *name, char *comment, uint16_t *RGBColor[] );

aafiMetaData * aafi_newMetadata( AAF_Iface *aafi, aafiMetaData **CommentList );


aafiAudioGain * aafi_newAudioGain( AAF_Iface *aafi, enum aafiAudioGain_e type, enum aafiInterpolation_e interpol, aafRational_t *singleValue );

aafiAudioGain * aafi_newAudioPan( AAF_Iface *aafi, enum aafiAudioGain_e type, enum aafiInterpolation_e interpol, aafRational_t *singleValue );


void aafi_freeAudioTracks( aafiAudioTrack **tracks );

void aafi_freeVideoTracks( aafiVideoTrack **tracks );


void aafi_freeTimelineItems( aafiTimelineItem **timelineItems );

void aafi_freeTimelineItem( aafiTimelineItem *timelineItem );

void aafi_freeAudioClip( aafiAudioClip *audioClip );

void aafi_freeTransition( aafiTransition *trans );


void aafi_freeMarkers( aafiMarker **aafi );

void aafi_freeMetadata( aafiMetaData **CommentList );


void aafi_freeAudioGain( aafiAudioGain *gain );

void aafi_freeAudioPan( aafiAudioPan *pan );

/**
 * @}
 */

#endif // !__AAFIface_h__
