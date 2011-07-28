
#ifndef __MISA_MENUCONFIG_H__
#define __MISA_MENUCONFIG_H__

#include <vector>
#include <string>

#if defined(Linux) && !defined(MISA_APPLICATION)
#include <Synth.h>
#endif		// Linux

#if defined(WIN32) || defined(MISA_APPLICATION)

#define TOUCH_X	0
#define TOUCH_Y	1
#define BALL_X	2
#define BALL_Y	3
#define DRAG_X	4
#define DRAG_Y	5
#define	STRING_TOUCH_X_0	6
#define	STRING_TOUCH_X_1	7
#define	STRING_TOUCH_X_2	8
#define	STRING_TOUCH_X_3	9
#define	STRING_TOUCH_X_4	10
#define	STRING_TOUCH_X_5	11
#define STRING_DRAG_X_0		12
#define STRING_DRAG_X_1		13
#define STRING_DRAG_X_2		14
#define STRING_DRAG_X_3		15
#define STRING_DRAG_X_4		16
#define STRING_DRAG_X_5		17
#define VARIATION			18

#define PARAM_NONE		0
#define PARAM_DISTORTION_ON	1
#define PARAM_DISTORTION_TYPE	2
#define PARAM_DISTORTION_LEVEL	3
#define PARAM_DISTORTION_DRIVE	4
#define PARAM_DISTORTION_TONE	5
#define PARAM_COMPRESSION_ON	6
#define PARAM_COMPRESSION_ATTACK	7
#define PARAM_COMPRESSION_RELEASE	8
#define PARAM_COMPRESSION_THRESHOLD	9
#define PARAM_COMPRESSION_RATIO	10
#define PARAM_COMPRESSION_BOOST	11
#define PARAM_COMPRESSION_KNEE		12
#define PARAM_MODULATION_ON		131
#define PARAM_MODULATION_TYPE		13
#define PARAM_MODULATION_LEVEL		14
#define PARAM_MODULATION_CHORUS_DT		15
#define PARAM_MODULATION_CHORUS_FBK	16
#define PARAM_MODULATION_CHORUS_HPF	17
#define PARAM_MODULATION_DELAY_FBKFILT	18
#define PARAM_MODULATION_RATE	19
#define PARAM_MODULATION_DEPTH	20
#define PARAM_MODULATION_TREMOLO	21
#define PARAM_DELAY_ON	22
#define PARAM_DELAY_MODE	23
#define PARAM_DELAY_PRELPF	24
#define PARAM_DELAY_LEVEL	25
#define PARAM_DELAY_TIME	26
#define PARAM_DELAY_FBK	27
#define PARAM_DELAY_FBKFILT	28
#define PARAM_REVERB_TYPE	29
#define PARAM_REVERB_CHARACTER	30
#define PARAM_REVERB_PRELPF	31
#define PARAM_REVERB_LEVEL		32
#define PARAM_REVERB_TIME	33
#define PARAM_REVERB_DELAY_FBK	34
#define PARAM_REVERB_PREDELAY	35
#define PARAM_EQ_LOWEST_GAIN	36
#define PARAM_EQ_LOWER_GAIN	37
#define PARAM_EQ_HIGHER_GAIN	38
#define PARAM_EQ_HIGHEST_GAIN	39
#define PARAM_EQ_LOWEST_FREQ	40
#define PARAM_EQ_LOWER_FREQ	41
#define PARAM_EQ_HIGHER_FREQ	42
#define PARAM_EQ_HIGHEST_FREQ	43
#define PARAM_EQ_LOWMID_Q	44
#define PARAM_EQ_HIGHMID_Q	45
#define PARAM_MIXER_LOWCUT_FILTER_FREQ	46
#define PARAM_MIXER_HIGHCUT_FILTER_FREQ	47
#define PARAM_MIXER_INPUT_GAIN	48
#define PARAM_MIXER_OUTPUT_LEVEL	49
#define PARAM_MIXER_PAN			50
#define PARAM_MIXER_REVERB_SEND	51

// OR : Drag & Drop
// enumeration of all PARAM_VOICE_XXX parameters
enum
{
	PARAM_VOICE_AMP_ENV_ATTACK	=	60,
	PARAM_VOICE_AMP_ENV_DECAY,
	PARAM_VOICE_AMP_ENV_RELEASE,	
	PARAM_VOICE_FILTER_FREQUENCY,
	PARAM_VOICE_FILTER_RESONANCE,
	PARAM_VOICE_DETUNE_COURSE,
	PARAM_VOICE_DETUNE_FINE,	
	PARAM_VOICE_VIBRATE_RATE,	
	PARAM_VOICE_VIBRATE_DEPTH,	
	PARAM_VOICE_VIBRATE_DELAY,	
	PARAM_VOICE_CHANNEL_VOLUME,	
	PARAM_VOICE_PORTAMENTO_TIME,	
	PARAM_VOICE_PAN,
	PARAM_VOICE_PITCH_BEND_ST,
	PARAM_VOICE_PITCH_WHEEL,	
	PARAM_VOICE_VELOCITY,	
	PARAM_VOICE_REVERB_SEND,
	PARAM_VOICE_FILTER_TYPE,
	PARAM_VOICE_FILTER_ATTACK,
	PARAM_VOICE_FILTER_DECAY,
	PARAM_VOICE_FILTER_RELEASE,
	PARAM_VOICE_END
};

#define PARAM_MIDI_VELOCITY				81
#define PARAM_MIDI_PITCH				82
#define PARAM_CC						83


struct voice
{
	int channel;
	int bank;
	int wavetable_index;
	int amp_env_attack;
	int amp_env_decay;
	int amp_env_release;
	int filter_frequency;
	int filter_resonance;
	int detune_course;
	int detune_fine;
	int vibrate_rate;
	int vibrate_depth;
	int vibrate_delay;
	int channel_volume;
	int portamento_time;
	int pan;
	int pitch_bend_semitones;
	int pitch_wheel;
	int velocity;
	bool insert_fx_block_on[2];
	int reverb_send;
	int filter_type;
	int filter_attack;
	int filter_decay;
	int filter_release;
};

struct eq_band
{
	int gain;
	int frequency;
};

struct eq
{
	bool on;
	struct eq_band lowest, lower, higher, highest;
	int low_mid_q, high_mid_q;
};

struct effect_distortion
{
	bool on;
	int type;
	int level;
	int drive;
	int tone;
	int noise_reduction;
	int booster;
};

struct effect_compressor
{
	static const int KNEE_HARD;
	static const int KNEE_SOFT;

	bool on;
	int attack;
	int release;
	int threshold;
	int ratio;
	int boost;
	int knee;
};

/*
struct effect_wahwah
{
	static const int TYPE_LPF = 0;
	static const int TYPE_BPF = 1;

	int type;
	int frequency;
	int resonance;
	int sensitivity;
};
*/

struct effect_modulation
{
	bool on;
	int type;
	int level;
	int chorus_delay_time;
	int chorus_feedback;
	int chorus_hpf;
	int delay_feedback_filter;
	int rate;
	int depth;
	int tremolo;
};

struct effect_delay
{
	static const int MODE_MONO;
	static const int MODE_STEREO;

	bool on;

	int mode;
	int pre_lp;
	int level;
	int time;
	int feedback;
	int feedback_filter;
};

struct effect_reverb
{
	int type;
	int character;
	int pre_lpf;
	int level;
	int time;
	int delay_feedback;
	int pre_delay_time;
};

struct mixer_controls
{
	int low_cut_filter_frequency;
	int high_cut_filter_frequency;
	int input_gain;
	int output_level;
	int pan;
	int reverb_send;
};

struct fxb
{
	struct effect_distortion distortion;
	struct effect_compressor compressor;
//	struct effect_wahwah wahwah;
	struct effect_modulation modulation;
	struct effect_delay delay;
	struct mixer_controls mixer;
};

struct assignable_effect
{
	unsigned char name;
	int str;
	int voice_index;
	unsigned char output;
	unsigned char channel;
	unsigned char cc;
	bool inverse;
	unsigned int variation_start;
	unsigned int variation_end;
	unsigned int fxb;
	unsigned int drag_center;
};

struct cc
{
	unsigned char cc_num;
	unsigned char value;
};	

typedef struct synth_setting
{
	unsigned char tuning[6];

	int lfo_rate;
	int lfo_frequency_depth;
	int lfo_amp_depth;
	int lfo_pitch_depth;

	int master_volume;

	struct eq equalizer;
	struct effect_reverb reverb;

	struct fxb fx_block[2];

	std::vector<struct voice> voices[6];

	signed char string_midi_out_channel[6];

	std::vector<struct assignable_effect> AE_touch_x, AE_touch_y, AE_drag_x, AE_drag_y, AE_ball_x, AE_ball_y, AE_string_touch_x_0, AE_string_touch_x_1, AE_string_touch_x_2, AE_string_touch_x_3, AE_string_touch_x_4, AE_string_touch_x_5, AE_string_drag_x_0, AE_string_drag_x_1, AE_string_drag_x_2, AE_string_drag_x_3, AE_string_drag_x_4, AE_string_drag_x_5, AE_variation;

	std::vector<struct cc> stop_sound_cmds[6];
} SYNTH_SETTING,*PSYNTH_SETTING;

static struct synth_setting current_setting;

#endif

// OR : Drag & Drop
PSYNTH_SETTING GetCurrentSettingPtr();
void SendVoiceParamToSynth(int string_index, int voice_index);
const std::string &GetCurrentPresetName();

void SynthMuteVoice(int string_index, int voice_index, bool Mute);
void SynthSoloVoice(int string_index, int voice_index, bool Solo);

std::string &GetTitleWithPreset(std::string &TitleBase);
void ClearModified();
void SetWindowToUpdateIfPresetModified(WM_HWIN hWin);
void LockModified();
void UnlockModified();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth reset all
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ResetAllEffect();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth preset operating
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthDeletePreset(std::string filepath);
void SynthLoadPreset(std::string filename);
void SynthSavePreset(PSYNTH_SETTING pSetting,std::string filename);
//bool GetCurrentSetting(PSYNTH_SETTING pSetting);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa distortion
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DistortionOn(int fxb, bool state);
void DistortionSetType(int fxb, int val);
void DistortionSetDrive(int fxb, int val);
void DistortionSetTone(int fxb, int val);
void DistortionSetLevel(int fxb, int val);
void DistortionNoiseReduction(int fxb, int val);
void DistortionSetBooster(int fxb, int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa compressor
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CompressorSetOn(int fxb, bool state);
void CompressorSetKnee(int fxb, int val);
void CompressorSetAttack(int fxb, int val);
void CompressorSetRelease(int fxb, int val);
void CompressorSetThreshold(int fxb, int val);
void CompressorSetRatio(int fxb, int val);
void CompressorSetBoost(int fxb, int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa delay
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DelaySetOn(int fxb, bool state);
void DelaySetMode(int fxb, int val);
void DelaySetPreLp(int fxb, int val);
void DelaySetLevel(int fxb, int val);
void DelaySetTime(int fxb, int val);
void DelaySetFeedback(int fxb, int val);
void DelaySetFeedbackFilter(int fxb, int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa modulation
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ModulationSetOn(int fxb, int val);
void ModulationSetPreset(int fxb, int val);
void ModulationSetLevel(int fxb, int val);
void ModulationSetChorusDelayTime(int fxb, int val);
void ModulationSetChorusFeedback(int fxb, int val);
void ModulationSetChorusHpf(int fxb, int val);
void ModulationSetDelayFeedbackFilter(int fxb, int val);
void ModulationSetRate(int fxb, int val);
void ModulationSetDepth(int fxb, int val);
void ModulationSetTremolo(int fxb, int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa reverberation
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ReverbSetType(int val);
void ReverbSetCharacter(int val);
void ReverbSetPreLpf(int val);
void ReverbSetLevel(int val);
void ReverbSetTime(int val);
void ReverbSetDelayFeedback(int val);
void ReverbSetPreDelay(int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa mixer
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MixerSetLowcut(int fxb, int val);
void MixerSetHighcut(int fxb, int val);
void MixerSetInputgain(int fxb, int val);
void MixerSetLevel(int fxb, int val);
void MixerSetReverbsend(int fxb, int val);
void MixerSetPan(int fxb, int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa EQ stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void EqOn(int val);
void EqSetLowestBandGain(int val);
void EqSetLowestBandFrequency(int val);
void EqSetLowMidQ(int val);
void EqSetLowerBandGain(int val);
void EqSetLowerBandFrequency(int val);
void EqSetHigherBandGain(int val);
void EqSetHigherBandFrequency(int val);
void EqSetHighMidQ(int val);
void EqSetHighestBandGain(int val);
void EqSetHighestBandFrequency(int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa assign effect
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int AssigneMentMenuTranslate(int assignment);
int SynthTranslateCtrl(int ctrl);
int SynthTranslateEffect(int effect);
void SynthAssignEffect(unsigned char touch_control, unsigned char synth_param, unsigned char fxb,bool inverse, unsigned int variation_start, unsigned int variation_end);
void SynthRemoveEffect(unsigned char synth_param, unsigned char fxb);
std::vector<int> SynthFindEffect(unsigned char synth_param, unsigned char fxb);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa synth string
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthTuning(unsigned char str, unsigned char note);
void SynthSetMidiOutChannel(unsigned char str, signed char channel);
signed char SynthGetMidiOutChannel(unsigned char str);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa voice effect
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void VoiceAssignEffect(unsigned char touch_control, unsigned char synth_param, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end);
void VoiceRemoveEffect(unsigned char synth_param, unsigned char str, unsigned char voice_index);
std::vector<int> VoiceFindEffect(unsigned char synth_param, unsigned char str, unsigned char voice_index);

void SynthInsertVoice(int str, int wave);
void SynthRemoveVoice(int str, int voice_index);
void SynthVoiceSetFX(int str, int voice_index, int fxb, bool state);
void SynthVoiceSetWave(int str, int voice_index, int program);
void SynthVoiceAmpEnvAttack(int str, int voice_index, int val);
void SynthVoiceAmpEnvDecay(int str, int voice_index, int val);
void SynthVoiceAmpEnvRelease(int str, int voice_index, int val);
void SynthVoiceFilterFrequency(int str, int voice_index, int val);
void SynthVoiceFilterResonance(int str, int voice_index, int val);
void SynthVoiceDetuneCourse(int str, int voice_index, int val);
void SynthVoiceDetuneFine(int str, int voice_index, int val);
void SynthVoiceVibrateRate(int str, int voice_index, int val);
void SynthVoiceVibrateDepth(int str, int voice_index, int val);
void SynthVoiceVibrateDelay(int str, int voice_index, int val);
void SynthVoiceChannelVolume(int str, int voice_index, int val);
void SynthVoicePortamentoTime(int str, int voice_index, int val);
void SynthVoiceReverbSend(int str, int voice_index, int val);
void SynthVoicePan(int str, int voice_index, int val);
void SynthVoicePitchBendSemitones(int str, int voice_index, int val);
void SynthVoicePitchWheel(int str, int voice_index, int val);
void SynthVoiceVoiceVelocity(int str, int voice_index, int val);
void SynthVoiceFilterType(int str, int voice_index, int val);
void SynthVoiceFilterAttack(int str, int voice_index, int val);
void SynthVoiceFilterDecay(int str, int voice_index, int val);
void SynthVoiceFilterRelease(int str, int voice_index, int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa MIDI control
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct __ASSIGNED_EFFECT
{
	U8 type;
	U8 inv;
	U8 varmin;
	U8 varmax;
	U32 cc;
} ASSIGNED_EFFECT,*LPASSIGNED_EFFECT;

void SynthAssignMidiPitch(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end);
void SynthUnassignMidiPitch(unsigned char touch_control, unsigned char channel);
void SynthAssignMidiVelocity(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end);
void SynthUnassignMidiVelocity(unsigned char touch_control, unsigned char str);
void SynthAssignCC(unsigned char touch_control, unsigned char channel, unsigned int cc, bool inverse, unsigned int variation_start, unsigned int variation_end);
void SynthUnassignCC(unsigned char touch_control, unsigned char channel, unsigned int cc);
void SynthAssignStopSound(unsigned char str, unsigned char cc, unsigned char val);
void SynthUnassignStopSound(unsigned char str, unsigned char cc);

std::vector<int> MidiPitchFindEffect(unsigned char chan);
std::vector<int> MidiVelocityFindEffect(unsigned char str);
std::vector<ASSIGNED_EFFECT> MidiCCFindEffect(unsigned char channel);
std::vector<int> MidiStopSoundFind(unsigned char str);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa global
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthTurnNotesOff();
void SynthSetDragCentre(unsigned char synth_param, unsigned char fxb, unsigned int drag_center);
void VoiceSetDragCentre(unsigned char synth_param, unsigned char str, unsigned char voice_index, unsigned int drag_center);
void MidiPitchSetDragCentre(unsigned char channel, unsigned int drag_center);
void MidiVelocitySetDragCentre(int str, unsigned int drag_center);
void MidiCCSetDragCentre(unsigned char channel, unsigned int cc, unsigned int drag_center);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Linux system
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int UpdateSoundbank();
int RebootSystem();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Quick set menu
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int SynthGetMastervolume(void);
void SynthSetMastervolume(int val);
int SynthGetTapmode();
void SynthSetTapmode(int val);
int SynthGetBallmode();
void SynthSetBallmode(int val);
int SynthGetStringmode();
void SynthSetStringmode(int val);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Graphic system
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define MISA_LEFTHAND_MODE 0
#define MISA_RIGHTHAND_MODE 1

int MisaGetHandMode();
int MisaSetHandMode(int mode);
int MisaSetBallTravel(int mode);
int MisaGetShowBall();
int MisaSetShowBall(int mode);
int MisaGetShowStrings();
int MisaSetShowStrings(int mode);

#endif // __MISA_MENUCONFIG_H__
