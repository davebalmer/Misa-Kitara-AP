#ifndef __SYNTH_H__
#define __SYNTH_H__

#include <string>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <vector>
#include "Touchpanel.h"
#include "Neck.h"
#include "Midi.h"

#define AE_CC		0
#define AE_NRPN		1
#define AE_SYSEX	2
#define AE_PITCH	3
#define AE_VELOCITY	4

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
	bool insert_fx_block_on[2];
	int reverb_send;
};

struct eq_band
{
	int gain;
	int frequency;
	int q;
};

struct eq
{
	struct eq_band lowest, lower, higher, highest;
};

struct effect_distortion
{
	bool on;
	int pre_gain;
	int type;
	int lpf_frequency;
	int lpf_resonance;
	int post_gain;
	int drive;
	int tone;
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

struct effect_wahwah
{
	static const int TYPE_LPF = 0;
	static const int TYPE_BPF = 1;

	int type;
	int frequency;
	int resonance;
	int sensitivity;
};

struct effect_modulation
{
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

struct fxb
{
	struct effect_distortion distortion;
	struct effect_compressor compressor;
	struct effect_wahwah wahwah;
	struct effect_modulation modulation;
	struct effect_delay delay;
};

struct synth_setting
{
	std::string preset_name;
	int lfo_rate;
	int lfo_frequency_depth;
	int lfo_amp_depth;
	int lfo_pitch_depth;

	int master_volume;

	struct eq equalizer;
	struct effect_reverb reverb;

	struct fxb fx_block[2];

	std::vector<struct voice *> voices[6];

	signed char string_midi_out_channel[6];
};

struct assignable_effect
{
	unsigned char output; //AE_SYNTH or AE_MIDI_OUT
	unsigned char type; //AE_CC, AE_NRPN, AE_SYSEX, AE_PITCH, AE_VELOCITY
	unsigned char channel;
	unsigned char cc;
	unsigned char nrpn_hi;
	unsigned char nrpn_lo;
	std::string sysex;
	unsigned char string;
	bool inverse;
	unsigned int variation_start;
	unsigned int variation_end;
};

class Synth
{
	private:
		MIDI midi;

		unsigned char velocity[6];

		struct synth_setting current_setting;
//		std::vector<struct synth_setting *> presets;

		std::vector<struct assignable_effect *> AE_touch_x, AE_touch_y, AE_drag_x, AE_drag_y, AE_ball_x, AE_ball_y, AE_string_touch_x_0, AE_string_touch_x_1, AE_string_touch_x_2, AE_string_touch_x_3, AE_string_touch_x_4, AE_string_touch_x_5, AE_string_drag_x_0, AE_string_drag_x_1, AE_string_drag_x_2, AE_string_drag_x_3, AE_string_drag_x_4, AE_string_drag_x_5, AE_variation;

		unsigned char midiScale(int val, int scaler);

		void resetSettings(void);

		void setMasterVolume(int val);
		void setLfoRate(int val);
		void setLfoFrequencyDepth(int val);
		void setLfoAmpDepth(int val);
		void setLfoPitchDepth(int val);

		void setChannel(int str, int voice_index, int val);
		void setWave(int str, int voice_index, int bank, int program);
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
		void setReverbSend(int str, int voice_index, int val);

		void setFxBlockOn(int str, int voice_index, int fxb, bool state);
public:
		void setDistortionOn(int fxb, bool state);
		void setDistortionPreGain(int fxb, int val);
		void setDistortionType(int fxb, int val);
		void setDistortionLpfFrequency(int fxb, int val);
		void setDistortionLpfResonance(int fxb, int val);
		void setDistortionPostGain(int fxb, int val);
		void setDistortionDrive(int fxb, int val);
		void setDistortionTone(int fxb, int val);
		void setCompressorOn(int fxb, bool state);
		void setCompressorAttack(int fxb, int val);
		void setCompressorRelease(int fxb, int val);
		void setCompressorThreshold(int fxb, int val);
		void setCompressorRatio(int fxb, int val);
		void setCompressorBoost(int fxb, int val);
		void setCompressorKnee(int fxb, int val);
		void setWahwahType(int fxb, int val);
		void setWahwahFrequency(int fxb, int val);
		void setWahwahResonance(int fxb, int val);
		void setWahwahSensitivity(int fxb, int val);
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
		void loadPresetFromFile(std::string filepath);
//		void loadPreset(unsigned int i);
		void sendNoteOn(unsigned char str, unsigned char note);
		void sendNoteOff(unsigned char str, unsigned char note);
		void sendCC(unsigned char str, unsigned char control, unsigned char val);
		void sendControl(unsigned char control_type, int val, int scaler);
		void sendToEffect(struct assignable_effect *ae, int val, int scaler);
		void sendVariation(int channel);
		void printCurrentPreset(void);
		void setVelocity(unsigned char str, unsigned char v);
		void setPitch(unsigned char output, unsigned char str, unsigned int p);
		void setSynthCC(unsigned char output, unsigned char str, unsigned char cc, unsigned char v);
		void assignEffect(unsigned char touch_control, unsigned char fxb, std::string *effect_control, std::string *extra_field, std::string *extra_field_2, std::string *extra_field_3, std::string *extra_field_4);
		bool lookupEffect(std::string s, unsigned char fxb, struct assignable_effect *ae);
};

#endif
