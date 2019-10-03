#ifndef __AAFIface_h__
#define __AAFIface_h__

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
 *	@file LibAAF/AAFIface/AAFIface.h
 *	@brief AAF processing
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 *
 *	@ingroup AAFIface
 *	@addtogroup AAFIface
 *	@{
 *	@brief Abstraction layer to interpret the Objects/Class and retrieve data.
 */


#include "../AAFCore/AAFTypes.h"
#include "../AAFCore/AAFClass.h"


#define AAFI_TYPE_PCM		0x01
#define AAFI_TYPE_WAVE		0x02
#define AAFI_TYPE_AIFC		0x03
#define AAFI_TYPE_BWAV		0x04
















/**
 *	Flags for aafiTransition.flags and aafiAudioGain.flags
 */

typedef enum aafiInterpolation_e
{
	AAFI_INTERPOL_NONE     = 0x0400,
	AAFI_INTERPOL_LINEAR   = 0x0800,
	AAFI_INTERPOL_LOG      = 0x1000,
	AAFI_INTERPOL_CONSTANT = 0x2000,
	AAFI_INTERPOL_POWER    = 0x4000,
	AAFI_INTERPOL_BSPLINE  = 0x8000,

} aafiInterpolation_e;


/**
 *	Flags for aafiAudioGain.flags.
 */

typedef enum aafiAudioGain_e
{

	AAFI_AUDIO_GAIN_CONSTANT = 0x0001,
	AAFI_AUDIO_GAIN_VARIABLE = 0x0002,

} aafiAudioGain_e;



/**
 *	Flags for aafiTransition.flags.
 */

typedef enum aafiTransition_e
{
	AAFI_TRANS_SINGLE_CURVE = 0x0010,
	AAFI_TRANS_TWO_CURVE    = 0x0020,

	AAFI_TRANS_FADE_IN      = 0x0040,
	AAFI_TRANS_FADE_OUT     = 0x0080,
	AAFI_TRANS_XFADE        = 0x0100,

} aafiTransition_e;








/**
 *	Specifies a Transition that can be a fade in, a fade out or a Cross fade, and that can
 *	have one or two curves.
 *
 *	With a single curve (AAFI_TRANS_SINGLE_CURVE), the same curve is mirrored and applied
 *	as fade in and fade out to obtain a cross fade.
 *
 *	Having two curves (AAFI_TRANS_TWO_CURVE) allows a cross fade to have one curve per fade.
 *
 *	A transition should have at least two points, one at time zero and one at time 1.
 *	TODO To finish
 */

typedef struct aafiTransition
{
	/**
	 *	Should hold the transition type (either single param or two param),
	 *	the transition fade type (in, out, x) and the interpolation used.
	 */

	int             flags;

	/**
	 *	Length of the transition, in edit units.
	 */

	aafPosition_t   len;

	/**
	 *	The cut point. In the case the transition is removed or cannot be played, the
	 *	cut point specifies where in the transition, the preceding segment should end
	 *	and where the following segment should start.
	 */

	aafPosition_t   cut_pt;





	/**
	 *	Points count for the single curve, or the first one of the two. This specifies
	 *	both the number of points (time/value) in the transition curve, and consequently
	 *	the size of time_a[] and value_a[] arrays.
	 */

	int             pts_cnt_a;

	/**
	 *	Array of time points, where the corresponding level value should apply either to
	 *	the single curve, or to the first one of the two.
	 */

	aafRational_t *time_a;

	/**
	 *	Multiplier level values, each one applying at the corresponding indexed time for
	 *	either the single curve, or the first one of the two.
	 *	The interval between two points shall be calculated using the specified
	 *	interpolation.
	 */

	aafRational_t *value_a;





	/**
	 *	Points count for the second curve, only when Transition has the AAFI_TRANS_TWO_CURVE
	 *	flag. This specifies both the number of points (time/value) in the transition curve,
	 *	and consequently the size of time_b[] and value_b[] arrays.
	 */

	int             pts_cnt_b;

	/**
	 *	Array of time points, where the corresponding level value should apply to the
	 *	second curve. Used only if Transition has the AAFI_TRANS_TWO_CURVE flag.
	 */

	aafRational_t **time_b;

	/**
	 *	Multiplier level values, each one applying at the corresponding indexed time.
	 *	The interval between two points shall be calculated using the specified
	 *	interpolation. Used only if Transitions has the AAFI_TRANS_TWO_CURVE flag.
	 */

	aafRational_t **value_b;

} aafiTransition;




/**
 *	Specifies a Gain to apply either to a Clip (aafiAudioClip.gain) or to an entire Track
 *	(aafiAudioTrack.gain), that is to all the Clips contained by that Track.
 *
 *	A Gain can be of to types :
 *
 *		* Constant (AAFI_AUDIO_GAIN_CONSTANT) : A Constant gain specifies a single value
 *		  as a multiplier to be applied to the Clip or Track.
 *
 *		* Variable (AAFI_AUDIO_GAIN_VARIABLE) : A Variable gain specifies multiple points
 *		  ( time / value ) that form all together the automation curve. The values between
 *		  two points are calculated by interpolating between the two values.
 *
 *	Both the Gain type and the interpolation mode are specified in the aafiAudioGain.flags
 *	with the values from aafiAudioGain_e and aafiInterpolation_e.
 *
 *	In the case of a Constant Gain, the single multiplier value should be retrieved from
 *	aafiAudioGain.value[0].
 */

typedef struct aafiAudioGain
{
	/**
	 *	Should hold the gain type (either Constant or Variable), and if it Variable,
	 *	the interpolation used to calculate the values between two time points.
	 */

	uint16_t        flags;	// Type : Static (single multiplier for entire clip) or
					        //		  Variable (automation)
					        // Interpolation : Linear, Log, Constant, Power, BSpline


	/**
	 *	Points count. This specifies both the number of points (time/value) in the
	 *	gain automation, and is consequently the size of time[] and value[] arrays.
	 */

	int64_t         pts_cnt;

	/**
	 *	Array of time points, where the corresponding level value should apply.
	 */

	aafRational_t *time;

	/**
	 *	Multiplier level values, each one applying at the corresponding indexed time.
	 *	The interval between two points shall be calculated using the specified
	 *	interpolation.
	 */

	aafRational_t *value;

} aafiAudioGain;

typedef struct aafiAudioGain aafiAudioPan;











typedef struct aafiAudioEssence
{

	wchar_t       *original_file;	// NetworkLocator::URLString should point to original essence file if external (and in some cases, points to the AAF itself if internal..)
	wchar_t       *exported_file_path;    // TODO, not that used.. to be tweaked.  ----  Holds the file path, once the essence has been exported, copied or linked.
	wchar_t       *file_name;			// MasterMob::Name -> file name
	wchar_t		  *unique_file_name;	// unique name generated from file_name. Sometimes, multiple files share the same names so this unique name should be used on export.

	uint64_t       length; 		// Length of Essence Data

	cfbNode       *node;			// The node holding the audio stream if embedded

	aafMobID_t    *sourceMobID;	// Holds the SourceMob Mob::ID references this EssenceData
	aafMobID_t    *masterMobID;	// Holds the MasterMob Mob::ID (used by CompoMob's Sequence SourceClips)

	aafObject     *SourceMob;

	uint16_t       type;	// depends on PCMDescriptor WAVEDescriptor AIFCDescriptor

	uint8_t        is_embedded;

	aafProperty   *summary;

	uint32_t       format;
	uint32_t       samplerate;
	int16_t        samplesize;
	int16_t        channels;

	// BWF BEXT chunk data
	char           description[256];
	char           originator[32];			// could be set with header::ProductName
	char           originatorReference[32];
	uint64_t       timeReference;			// SourceMob TimelineMobSlot::Origin
	unsigned char  umid[64];				// SourceMob::MobID (32 Bytes, basic form)
	char           originationDate[10+1];		// SourceMob::CreationDate
	char           originationTime[8+1];		// SourceMob::CreationTime

	// TODO peakEnveloppe
	struct aafiAudioEssence *next;

} aafiAudioEssence;
















// forward declaration
struct aafiAudioTrack;

typedef struct aafiAudioClip
{

	struct aafiAudioTrack *track;

	aafiAudioEssence      *Essence;

	aafiAudioGain         *gain;


	// uint16_t               inner_track_channel; // for multichannel track only.

	aafPosition_t          pos;

	aafPosition_t          len;

	aafPosition_t          essence_offset; // start position in the source file


	aafMobID_t            *masterMobID; // MobID of the associated MasterMob (PID_SourceReference_SourceID)


} aafiAudioClip;








typedef enum aafiTimelineItem_type_e
{
	AAFI_CLIP  = 0x0001,
	AAFI_TRANS = 0x0002,

} aafiTimelineItem_type_e;


/**
 *	This structure can old either an aafiAudioClip or an aafiTransition struct.
 */

typedef struct aafiTimelineItem
{
	int                      type;

	struct aafiTimelineItem *next;
	struct aafiTimelineItem *prev;

	// is to be casted as aafiTransition or aafiAudioClip struct.
	unsigned char data[];

} aafiTimelineItem;


/**
 *	Used by aafiAudio.tc and aafiAudioTrack.tc.
 */

typedef struct aafiTimecode
{
	/**
	 *	Timecode start in EditUnit. (session start)
	 */

	aafPosition_t  start;

	/**
	 *	Timecode end in EditUnit. (session end)
	 */

	aafPosition_t  end;

	/**
	 *	Frame per second.
	 */

	uint16_t       fps;

	/**
	 *	Indicates whether the timecode is drop (True value) or nondrop (False value)
	 */

	uint8_t        drop;

	/**
	 *	Keeps track of the TimelineMobSlot EditRate.
	 *	TODO do we care ?
	 */

	aafRational_t *edit_rate;

} aafiTimecode;





/**
 *	Values for aafiAudioTrack.format.
 */

typedef enum aafiTrackFormat_e
{
	AAFI_TRACK_FORMAT_MONO   = 1,
	AAFI_TRACK_FORMAT_STEREO = 2,
	AAFI_TRACK_FORMAT_5_1    = 6,
	AAFI_TRACK_FORMAT_7_1    = 8

} aafiTrackFormat_e;

// forward declaration
struct aafiAudio;

typedef struct aafiAudioTrack
{
	/**
	 *	Track number
	 *	TODO Should it start at one ?
	 *	TODO Optional, should have a guess (i++) option.
	 */

	uint32_t                 number;

	uint16_t                 format;

	/**
	 *	Track name
	 */

	wchar_t                 *name;

	/**
	 *	Holds the Gain to apply on that track, that is the track volume Fader.
	 */

	aafiAudioGain           *gain;

	aafiAudioPan            *pan;

	/**
	 *	Holds the timeline items of that track, that is aafiAudioClip and aafiTransition
	 *	structures.
	 */

	struct aafiTimelineItem *Items;

	/**
	 *	The edit rate of all the contained Clips and Transitions.
	 */

	aafRational_t           *edit_rate;

	/**
	 *	Pointer to the aafiAudio for convenient access.
	 */

	struct aafiAudio        *Audio;

	/**
	 *	Pointer to the next aafiAudioTrack structure in the aafiAudio.Tracks list.
	 */

	struct aafiAudioTrack   *next;

} aafiAudioTrack;







typedef struct aafiUserComment
{
	wchar_t *name;

	wchar_t *text;

	struct aafiUserComment *next;

} aafiUserComment;







typedef struct aafiAudio
{
	/**
	 *	Holds the sequence start timecode.
	 */

	aafiTimecode     *tc;

	/**
	 *	Holds the Essence list.
	 */

	aafiAudioEssence *Essences;

	/**
	 *	Holds the Track list.
	 */

	aafiAudioTrack   *Tracks;

} aafiAudio;





typedef struct aafiContext
{
	/* Current Mob */

	aafObject *Mob;

	/* Current Mob::Slots */

	aafObject *MobSlot;

	/*
	 *	Current MobSlot Segment's DataDefinition
	 *	Mob::Slots > MobSlot::Segment > Component::DataDefinition
	 */

	// aafUID_t  *DataDef;


	uint8_t is_inside_sequence;	// takes 1 or 0. Usefull whent parsing MonoAudioGain or MonoAudioPan to know if it should be applied to clips or track.



	/* Clip */

	aafiAudioTrack * current_track;

	aafBoolean_t     current_track_is_multichannel;

	uint16_t         current_multichannel_track_channel;

	aafPosition_t    current_multichannel_track_clip_length;

	aafPosition_t    current_pos;



	/* Transition */

	aafiTransition   *current_transition;



	/* Gain */

	aafiAudioGain    *current_gain;



	/* Essence */

	aafiAudioEssence *current_audioEssence;

} aafiContext;



typedef struct AAF_Iface
{
	aafiContext ctx;

	/**
	 *	Keeps track of the AAF_Data structure.
	 */

	AAF_Data   *aafd;

	aafiAudio  *Audio;

//	AAFIface_Video Video;

	wchar_t    *compositionName;


	aafiUserComment  *Comments;

} AAF_Iface;







#define foreach_audioTrack( audioTrack, aafi ) \
	for ( audioTrack  = aafi->Audio->Tracks;   \
	      audioTrack != NULL;                  \
	      audioTrack  = audioTrack->next )     \



#define foreach_audioItem( audioItem, audioTrack ) \
	for ( audioItem  = audioTrack->Items;          \
	      audioItem != NULL;                       \
	      audioItem  = audioItem->next )           \



#define foreachAudioEssence( ae, aeList ) \
	for ( ae = aeList; ae != NULL; ae = ae->next )



#define aeDuration_h( audioEssence ) \
	(( audioEssence->samplerate == 0 ) ? 0 : ((uint16_t)(audioEssence->length / audioEssence->samplerate / (audioEssence->samplesize / 8)) / 3600))

#define aeDuration_m( audioEssence ) \
	(( audioEssence->samplerate == 0 ) ? 0 : ((uint16_t)(audioEssence->length / audioEssence->samplerate / (audioEssence->samplesize / 8)) % 3600 / 60))

#define aeDuration_s( audioEssence ) \
	(( audioEssence->samplerate == 0 ) ? 0 : ((uint16_t)(audioEssence->length / audioEssence->samplerate / (audioEssence->samplesize / 8)) % 3600 % 60))

#define aeDuration_ms( audioEssence ) \
	(( audioEssence->samplerate == 0 ) ? 0 : ((uint16_t)(audioEssence->length / (audioEssence->samplerate / 1000) / (audioEssence->samplesize / 8)) % 3600000 % 60000 % 1000))



#define eu2sample( audioClip, val ) \
	(int64_t)(val * (audioClip->Essence->samplerate * (1 / rationalToFloat(audioClip->track->edit_rate))))

/*
#define eu2tc_h( edit_rate, val ) \
	(uint16_t)((val * (1 / rationalToFloat(edit_rate))) / 3600)

#define eu2tc_m( edit_rate, val ) \
	(uint16_t)((int64_t)(val * (1 / rationalToFloat(edit_rate))) % 3600 / 60)

#define eu2tc_s( edit_rate, val ) \
	(uint16_t)((int64_t)(val * (1 / rationalToFloat(edit_rate))) % 3600 % 60)

#define eu2tc_f( edit_rate, tc, val ) \
	(uint16_t)(((int64_t)(val * (1 / rationalToFloat(edit_rate))) % tc->fps) * tc->fps)
*/




/*
#define foreachAudioClip( ac, acList ) \
	for ( ac = acList; ac != NULL; ac = ac->next )
*/








AAF_Iface * aafi_alloc( AAF_Data *aafd );

void aafi_release( AAF_Iface **aafi );

int aafi_load_file( AAF_Iface *aafi, const char * file );


aafiTransition * get_fadein( aafiTimelineItem *audioItem );

aafiTransition * get_fadeout( aafiTimelineItem *audioItem );


aafiAudioTrack * aafi_newAudioTrack( AAF_Iface *aafi );
void   aafi_freeAudioTracks( aafiAudioTrack **tracks );


aafiTimelineItem * aafi_newTimelineItem( aafiAudioTrack *track, int itemType );
void   aafi_freeAudioClip( aafiAudioClip *audioClip );
void   aafi_freeTimelineItem( aafiTimelineItem **item );
void   aafi_freeTimelineItems( aafiTimelineItem **items );

aafiUserComment * aafi_newUserComment( aafiUserComment **CommentList );
void aafi_freeUserComments( aafiUserComment **CommentList );


void   aafi_freeTransition( aafiTransition *trans );


aafiAudioEssence * aafi_newAudioEssence( AAF_Iface *aafi );
void   aafi_freeAudioEssences( aafiAudioEssence **essences );



/**
 *	@}
 */

#endif // __AAFIface_h__
