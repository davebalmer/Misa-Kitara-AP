/*


PRESET BUILDER CODE


*/

#ifndef __SYNTH_H__
#define __SYNTH_H__

#include <string>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <vector>

#define SYNTH	0
#define MIDI_OUT	1

enum {
	TOUCH_X	= 0,
	TOUCH_Y,
	BALL_X,
	BALL_Y,
	DRAG_X,
	DRAG_Y,
	STRING_TOUCH_X_0,
	STRING_TOUCH_X_1,
	STRING_TOUCH_X_2,
	STRING_TOUCH_X_3,
	STRING_TOUCH_X_4,
	STRING_TOUCH_X_5,
	STRING_DRAG_X_0,
	STRING_DRAG_X_1,
	STRING_DRAG_X_2,
	STRING_DRAG_X_3,
	STRING_DRAG_X_4,
	STRING_DRAG_X_5,
	VARIATION,
	CONTROL_END
};

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
#define PARAM_MIXER_PAN		50
#define PARAM_MIXER_REVERB_SEND	51

#define PARAM_VOICE_AMP_ENV_ATTACK		60
#define PARAM_VOICE_AMP_ENV_DECAY		61
#define PARAM_VOICE_AMP_ENV_RELEASE		62
#define PARAM_VOICE_FILTER_FREQUENCY	63
#define PARAM_VOICE_FILTER_RESONANCE	64
#define PARAM_VOICE_DETUNE_COURSE		65
#define PARAM_VOICE_DETUNE_FINE			66
#define PARAM_VOICE_VIBRATE_RATE		67
#define PARAM_VOICE_VIBRATE_DEPTH		68
#define PARAM_VOICE_VIBRATE_DELAY		69
#define PARAM_VOICE_CHANNEL_VOLUME		70
#define PARAM_VOICE_PORTAMENTO_TIME		71
#define PARAM_VOICE_PAN					72
#define PARAM_VOICE_PITCH_BEND_ST		73
#define PARAM_VOICE_PITCH_WHEEL			74
#define PARAM_VOICE_VELOCITY			75
#define PARAM_VOICE_REVERB_SEND			76
#define PARAM_VOICE_FILTER_TYPE			77
#define PARAM_VOICE_FILTER_ATTACK		78
#define PARAM_VOICE_FILTER_DECAY		79
#define PARAM_VOICE_FILTER_RELEASE		80

#define PARAM_VELOCITY					81
#define PARAM_PITCH						82
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
	static const int KNEE_HARD = 0;
	static const int KNEE_SOFT = 1;

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
	static const int MODE_MONO = 0;
	static const int MODE_STEREO = 1;

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

class Synth
{
	private:
//		MIDI midi;

		unsigned char velocity[6];

		struct synth_setting current_setting;
		unsigned char midiScale(int val, int scaler);

	public:
		void resetSettings(void);
		std::vector<std::string> wavetable;
		void readWavetable(void);
		int getWavetableIndex(std::string wave);

	private:
		void setMasterVolume(int val);
		void setLfoRate(int val);
		void setLfoFrequencyDepth(int val);
		void setLfoAmpDepth(int val);
		void setLfoPitchDepth(int val);

		void setChannel(int str, int voice_index, int val);
		void setWave(int str, int voice_index, int program);
		void setAmpEnvAttack(int str, int voice_index, int val);
		void setAmpEnvDecay(int str, int voice_index, int val);
		void setAmpEnvRelease(int str, int voice_index, int val);
		void setFilterFrequency(int str, int voice_index, int val);
		void setFilterResonance(int str, int voice_index, int val);
		void setDetuneCourse(int str, int voice_index, int val);
		void setDetuneFine(int str, int voice_index, int val);
		void setVibrateRate(int str, int voice_index, int val);
		void setVibrateDepth(int str, int voice_index, int val);
		void setVibrateDelay(int str, int voice_index, int val);
		void setChannelVolume(int str, int voice_index, int val);
		void setPortamentoTime(int str, int voice_index, int val);
		void setPan(int str, int voice_index, int val);
		void setPitchBendSemitones(int str, int voice_index, int val);
		void setPitchWheel(int str, int voice_index, int val);
		void setVoiceVelocity(int str, int voice_index, int val);
		void setReverbSend(int str, int voice_index, int val);
		void setFilterType(int str, int voice_index, int val);
		void setFilterAttack(int str, int voice_index, int val);
		void setFilterDecay(int str, int voice_index, int val);
		void setFilterRelease(int str, int voice_index, int val);

		void setFxBlockOn(int str, int voice_index, int fxb, bool state);
public:
		void setDistortionOn(int fxb, bool state);
		void setDistortionType(int fxb, int val);
		void setDistortionLevel(int fxb, int val);
		void setDistortionDrive(int fxb, int val);
		void setDistortionTone(int fxb, int val);
		void setDistortionNoiseReduction(int fxb, int val);
		void setDistortionBooster(int fxb, int val);
		void setCompressorOn(int fxb, bool state);
		void setCompressorAttack(int fxb, int val);
		void setCompressorRelease(int fxb, int val);
		void setCompressorThreshold(int fxb, int val);
		void setCompressorRatio(int fxb, int val);
		void setCompressorBoost(int fxb, int val);
		void setCompressorKnee(int fxb, int val);
/*		void setWahwahType(int fxb, int val);
		void setWahwahFrequency(int fxb, int val);
		void setWahwahResonance(int fxb, int val);
		void setWahwahSensitivity(int fxb, int val);
*/		void setModulationOn(int fxb, bool state);
		void setModulationType(int fxb, int val);
		void setModulationLevel(int fxb, int val);
		void setModulationChorusDelayTime(int fxb, int val);
		void setModulationChorusFeedback(int fxb, int val);
		void setModulationChorusHpf(int fxb, int val);
		void setModulationDelayFeedbackFilter(int fxb, int val);
		void setModulationRate(int fxb, int val);
		void setModulationDepth(int fxb, int val);
		void setModulationTremolo(int fxb, int val);
		void setDelayOn(int fxb, bool state);
		void setDelayMode(int fxb, int val);
		void setDelayPreLp(int fxb, int val);
		void setDelayLevel(int fxb, int val);
		void setDelayTime(int fxb, int val);
		void setDelayFeedback(int fxb, int val);
		void setDelayFeedbackFilter(int fxb, int val);
		void setEQOn(bool state);
		void setEQLowestBandGain(int val);
		void setEQLowerBandGain(int val);
		void setEQHigherBandGain(int val);
		void setEQHighestBandGain(int val);
		void setEQLowestBandFrequency(int val);
		void setEQLowerBandFrequency(int val);
		void setEQHigherBandFrequency(int val);
		void setEQHighestBandFrequency(int val);
		void setEQLowMidQ(int val);
		void setEQHighMidQ(int val);
		void setMixerLowCutFilterFrequency(int fxb, int val);
		void setMixerHighCutFilterFrequency(int fxb, int val);
		void setMixerInputGain(int fxb, int val);
		void setMixerOutputLevel(int fxb, int val);
		void setMixerPan(int fxb, int val);
		void setMixerReverbSend(int fxb, int val);

		void setReverbType(int val);
		void setReverbCharacter(int val);
		void setReverbPreLpf(int val);
		void setReverbLevel(int val);
		void setReverbTime(int val);
		void setReverbDelayFeedback(int val);
		void setReverbPreDelay(int val);

	public:
		Synth(void);
		~Synth(void);
		void loadPresetFromFile(std::string filename);
		void loadSouthpad(void);
		void loadMeltGuitar(void);
		void loadDigitar(void);
		void loadRemstar(void);
		void loadTremstar(void);
		void loadPhaseGuitar(void);
		void loadSquareLead(void);
		void loadWarmBass(void);
		void loadWarmBassStd(void);
		void loadEightyFour(void);
		void loadSineGuitar(void);
		void loadSineDefault(void);
		void loadSineWave(void);
		void loadSineGuitarStd(void);
		void loadXenoxJeff(void);
		void loadExplode(void);
		void loadPadSustain(void);
		void loadSparklevar(void);
		void savePresetToFile(struct synth_setting *s, std::string filepath);
		void printCurrentSettings(void);
		void sendNoteOn(unsigned char str, unsigned char note, bool harmonic, bool attack);
		void sendNoteOff(unsigned char str, unsigned char note, bool harmonic);
		void sendStopSound(unsigned char str, unsigned char note, bool harmonic);
		void sendCC(unsigned char str, unsigned char control, unsigned char val);
		void sendControl(unsigned char control_type, int val, int scaler);
		void sendToEffect(struct assignable_effect *ae, int val, int scaler);
		void sendVariation(int str, int channel);
		void setVelocity(unsigned char str, unsigned char v);
		void setPitch(unsigned char output, unsigned char str, unsigned int p);
		void assignSynthEffect(unsigned char touch_control, unsigned char synth_param, unsigned char fxb, bool inverse, unsigned int variation_start, unsigned int variation_end);
		void unAssignSynthEffect(unsigned char synth_param, unsigned char fxb);
		void assignSynthVoiceParam(unsigned char touch_control, unsigned char synth_param, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end);
		void unAssignSynthVoiceParam(unsigned char synth_param, unsigned char str, unsigned char voice_index);
		void assignVelocity(unsigned char touch_control, int str, bool inverse, unsigned int variation_start, unsigned int variation_end);
		void unAssignVelocity(int str);
		void assignSynthPitch(unsigned char touch_control, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end);
		void assignMidiPitch(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end);
		void unAssignSynthPitch(unsigned char str, unsigned char voice_index);
		void unAssignMidiPitch(unsigned char channel);
		void assignCC(unsigned char touch_control, unsigned char output, unsigned char channel, unsigned int cc, bool inverse, unsigned int variation_start, unsigned int variation_end);
		void unAssignCC(unsigned char output, unsigned char channel, unsigned int cc);
		void addToControl(unsigned char touch_control, struct assignable_effect ae);
		std::vector<struct assignable_effect> *getControlList(unsigned char touch_control);
		void insertNewVoice(int str, int wave);
		void removeVoice(int str, int voice_index);
//		bool GetCurrentSetting(PSYNTH_SETTING pSynthsetting);
		void run(void);
		void setDragCenterSynthEffect(unsigned char synth_param, unsigned char fxb, unsigned int drag_center);
		void setDragCenterSynthVoiceParam(unsigned char synth_param, unsigned char str, unsigned char voice_index, unsigned int drag_center);
		void setDragCenterVelocity(int str, unsigned int drag_center);
		void setDragCenterSynthPitch(unsigned char str, unsigned char voice_index, unsigned int drag_center);
		void setDragCenterMidiPitch(unsigned char channel, unsigned int drag_center);
		void setDragCenterCC(unsigned char output, unsigned char channel, unsigned int cc, unsigned int drag_center);
		void assignMidiStopSound(unsigned char str, unsigned char cc, unsigned char val);
};

#endif
