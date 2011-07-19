

//#ifdef Linux
#if defined(Linux) && !defined(MISA_APPLICATION)
#include <iostream>

#include <Synth.h>
#include <ControlScreen.h>

#include <string>

#include "GUI.h"
#include "BUTTON.h"

#include "MisaAppInc.h"
#include "MisaWidget.h"

#include "SynthScreen.h"
#include "CompressionScreen.h"
#include "DistortionScreen.h"
#include "ModulationScreen.h"
#include "DelayScreen.h"
#include "ReverbScreen.h"
#include "MixerScreen.h"
#include "EqScreen.h"

#include "MisamenuConfig.h"

extern std::string working_directory;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa synthesizer
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern Synth* pSynth;
extern ControlScreen* pcs;

//static char buf[64];

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth reset all
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ResetAllEffect()
{
	pSynth->resetSettings();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth preset operating
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthLoadPreset(std::string filepath)
{
	pSynth->loadPresetFromFile(filepath);
}

void SynthSavePreset(PSYNTH_SETTING pSetting,std::string filepath)
{
	pSynth->savePresetToFile(pSetting, filepath);
}

bool GetCurrentSetting(PSYNTH_SETTING pSetting)
{
	pSynth->GetCurrentSetting(pSetting);
	return true;
}

// OR : Drag & Drop
PSYNTH_SETTING GetCurrentSettingPtr()
{
	return pSynth->GetCurrentSettingPtr();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa distortion
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DistortionOn(int fxb, bool state)
{
	pSynth->setDistortionOn(fxb, state);
}

void DistortionSetType(int fxb, int val)
{
	pSynth->setDistortionType(fxb, val);
}

void DistortionSetDrive(int fxb, int val)
{
	pSynth->setDistortionDrive(fxb, val);
}

void DistortionSetTone(int fxb, int val)
{
	pSynth->setDistortionTone(fxb, val);
}

void DistortionSetLevel(int fxb, int val)
{
	pSynth->setDistortionLevel(fxb, val);
}

void DistortionNoiseReduction(int fxb, int val)
{
	pSynth->setDistortionNoiseReduction(fxb, val);
}

void DistortionSetBooster(int fxb, int val)
{
	pSynth->setDistortionBooster(fxb, val);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa compressor
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CompressorSetOn(int fxb, bool state)
{
	pSynth->setCompressorOn(fxb, state);
}

void CompressorSetKnee(int fxb, int val)
{
	pSynth->setCompressorKnee(fxb, val);
}

void CompressorSetAttack(int fxb, int val)
{
	pSynth->setCompressorAttack(fxb, val);
}

void CompressorSetRelease(int fxb, int val)
{
	pSynth->setCompressorRelease(fxb, val);
}

void CompressorSetThreshold(int fxb, int val)
{
	pSynth->setCompressorThreshold(fxb, val);
}

void CompressorSetRatio(int fxb, int val)
{
	pSynth->setCompressorRatio(fxb, val);
}

void CompressorSetBoost(int fxb, int val)
{
	pSynth->setCompressorBoost(fxb, val);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa delay
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DelaySetOn(int fxb, bool state)
{
	pSynth->setDelayOn(fxb, state);
}

void DelaySetMode(int fxb, int val)
{
	pSynth->setDelayMode(fxb, val);
}

void DelaySetPreLp(int fxb, int val)
{
	pSynth->setDelayPreLp(fxb, val);
}

void DelaySetLevel(int fxb, int val)
{
	pSynth->setDelayLevel(fxb, val);
}

void DelaySetTime(int fxb, int val)
{
	pSynth->setDelayTime(fxb, val);
}

void DelaySetFeedback(int fxb, int val)
{
	pSynth->setDelayFeedback(fxb, val);
}

void DelaySetFeedbackFilter(int fxb, int val)
{
	pSynth->setDelayFeedbackFilter(fxb, val);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa modulation
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ModulationSetOn(int fxb, int val)
{
	pSynth->setModulationOn(fxb, val?true:false);
}

void ModulationSetPreset(int fxb, int val)
{
	pSynth->setModulationType(fxb, val);
}

void ModulationSetLevel(int fxb, int val)
{
	pSynth->setModulationLevel(fxb, val);
}

void ModulationSetChorusDelayTime(int fxb, int val)
{
	pSynth->setModulationChorusDelayTime(fxb, val);
}

void ModulationSetChorusFeedback(int fxb, int val)
{
	pSynth->setModulationChorusFeedback(fxb, val);
}

void ModulationSetChorusHpf(int fxb, int val)
{
	pSynth->setModulationChorusHpf(fxb, val);
}

void ModulationSetDelayFeedbackFilter(int fxb, int val)
{
	pSynth->setModulationDelayFeedbackFilter(fxb, val);
}

void ModulationSetRate(int fxb, int val)
{
	pSynth->setModulationRate(fxb, val);
}

void ModulationSetDepth(int fxb, int val)
{
	pSynth->setModulationDepth(fxb, val);
}

void ModulationSetTremolo(int fxb, int val)
{
	pSynth->setModulationTremolo(fxb, val);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa reverberation
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ReverbSetType(int val)
{
	pSynth->setReverbType(val);
	//sprintf(buf,"ReverbSetType val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
}

void ReverbSetCharacter(int val)
{
	pSynth->setReverbCharacter(val);
	//sprintf(buf,"ReverbSetCharacter val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
}

void ReverbSetPreLpf(int val)
{
	pSynth->setReverbPreLpf(val);
	//sprintf(buf,"ReverbSetPreLpf val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
}

void ReverbSetLevel(int val)
{
	pSynth->setReverbLevel(val);
	//sprintf(buf,"ReverbSetLevel val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
}

void ReverbSetTime(int val)
{
	pSynth->setReverbTime(val);
	//sprintf(buf,"ReverbSetTime val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
}

void ReverbSetDelayFeedback(int val)
{
	pSynth->setReverbDelayFeedback(val);
	//sprintf(buf,"ReverbSetDelayFeedback val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
}

void ReverbSetPreDelay(int val)
{
	pSynth->setReverbPreDelay(val);
	//sprintf(buf,"ReverbSetPreDelay val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa mixer
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MixerSetLowcut(int fxb, int val)
{
	pSynth->setMixerLowCutFilterFrequency(fxb, val);
}

void MixerSetHighcut(int fxb, int val)
{
	pSynth->setMixerHighCutFilterFrequency(fxb, val);
}

void MixerSetInputgain(int fxb, int val)
{
	pSynth->setMixerInputGain(fxb, val);
}

void MixerSetLevel(int fxb, int val)
{
	pSynth->setMixerOutputLevel(fxb, val);
}

void MixerSetReverbsend(int fxb, int val)
{
	pSynth->setMixerReverbSend(fxb, val);
}

void MixerSetPan(int fxb, int val)
{
	pSynth->setMixerPan(fxb, val);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa EQ
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void EqOn(int val)
{
	pSynth->setEQOn(val?true:false);
}

void EqSetLowestBandGain(int val)
{
	pSynth->setEQLowestBandGain(val);
}

void EqSetLowestBandFrequency(int val)
{
	pSynth->setEQLowestBandFrequency(val);
}

void EqSetLowMidQ(int val)
{
	pSynth->setEQLowMidQ(val);
}

void EqSetLowerBandGain(int val)
{
	pSynth->setEQLowerBandGain(val);
}

void EqSetLowerBandFrequency(int val)
{
	pSynth->setEQLowerBandFrequency(val);
}

void EqSetHigherBandGain(int val)
{
	pSynth->setEQHigherBandGain(val);
}

void EqSetHigherBandFrequency(int val)
{
	pSynth->setEQHigherBandFrequency(val);
}

void EqSetHighMidQ(int val)
{
	pSynth->setEQHighMidQ(val);
}

void EqSetHighestBandGain(int val)
{
	pSynth->setEQHighestBandGain(val);
}

void EqSetHighestBandFrequency(int val)
{
	pSynth->setEQHighestBandFrequency(val);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa assign effect
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int AssigneMentMenuTranslate(int assignment)
{
	int ret;
	switch(assignment)
	{
	case TOUCH_X:
		ret = CTL_TOUCH_X;
		break;
	case TOUCH_Y:
		ret = CTL_TOUCH_Y;
		break;
	case DRAG_X:
		ret = CTL_DRAG_X;
		break;
	case DRAG_Y:
		ret = CTL_DRAG_Y;
		break;
	case BALL_X:
		ret = CTL_BALL_X;
		break;
	case BALL_Y:
		ret = CTL_BALL_Y;
		break;
	case VARIATION:
		ret = CTL_VARIATION;
		break;
	default:
		ret = -1;
	}
	return ret;
}

int SynthTranslateCtrl(int ctrl)
{
	int ret;
	switch(ctrl)
	{
	case CTL_NONE_ID:
		ret = -1;
		break;
	case CTL_TOUCH_X_ID:
		ret = TOUCH_X;
		break;
	case CTL_TOUCH_Y_ID:
		ret = TOUCH_Y;
		break;
	case CTL_DRAG_X_ID:
		ret = DRAG_X;
		break;
	case CTL_DRAG_Y_ID:
		ret = DRAG_Y;
		break;
	case CTL_BALL_X_ID:
		ret = BALL_X;
		break;
	case CTL_BALL_Y_ID:
		ret = BALL_Y;
		break;
	case CTL_VARIATION_ID:
		ret = VARIATION;
		break;
	default:
		ret = -1;
	}
	return ret;
}

int SynthTranslateEffect(int effect)
{
	int ret;
	switch(effect)
	{
	case COMPRESSION_ID_PROGATTACK:
		ret = PARAM_COMPRESSION_ATTACK;
		break;
	case COMPRESSION_ID_PROGRELEASE:
		ret = PARAM_COMPRESSION_RELEASE;
		break;
	case COMPRESSION_ID_PROGBOOST:
		ret = PARAM_COMPRESSION_BOOST;
		break;
	case COMPRESSION_ID_PROGTRES:
		ret = PARAM_COMPRESSION_THRESHOLD;
		break;
	case COMPRESSION_ID_PROGRATIO:
		ret = PARAM_COMPRESSION_RATIO;
		break;
	case DISTORTION_ID_PROGDRIVE:
		ret = PARAM_DISTORTION_DRIVE;
		break;
	case DISTORTION_ID_PROGTONE:
		ret = PARAM_DISTORTION_TONE;
		break;
	case DISTORTION_ID_PROGLEVEL:
		ret = PARAM_DISTORTION_LEVEL;
		break;
	case MODULATION_ID_PROGMODRATE:
		ret = PARAM_MODULATION_RATE;
		break;
	case MODULATION_ID_PROGMODDEPTH:
		ret = PARAM_MODULATION_DEPTH;
		break;
	case MODULATION_ID_PROGLEVEL:
		ret = PARAM_MODULATION_LEVEL;
		break;
	case MODULATION_ID_PROGFDBK:
		ret = PARAM_MODULATION_CHORUS_FBK;
		break;
	case MODULATION_ID_PROGFDBKFILTER:
		ret = PARAM_MODULATION_DELAY_FBKFILT;
		break;
	case MODULATION_ID_PROGSHAPE:
		ret = PARAM_MODULATION_TREMOLO;
		break;
	case MODULATION_ID_PROGTIME:
		ret = PARAM_MODULATION_CHORUS_DT;
		break;
	case DELAY_ID_PROGPRELPF:
		ret = PARAM_DELAY_PRELPF;
		break;
	case DELAY_ID_PROGTIME:
		ret = PARAM_DELAY_TIME;
		break;
	case DELAY_ID_PROGLEVEL:
		ret = PARAM_DELAY_LEVEL;
		break;
	case DELAY_ID_PROGFDBK:
		ret = PARAM_DELAY_FBK;
		break;
	case DELAY_ID_PROGFDBKFILTER:
		ret = PARAM_DELAY_FBKFILT;
		break;
	case REVERB_ID_PROGPRELPF:
		ret = PARAM_REVERB_PRELPF;
		break;
	case REVERB_ID_PROGCHAR:
		ret = PARAM_REVERB_CHARACTER;
		break;
	case REVERB_ID_PROGLEVEL:
		ret = PARAM_REVERB_LEVEL;
		break;
	case REVERB_ID_PROGTIME:
		ret = PARAM_REVERB_TIME;
		break;
	case REVERB_ID_PROGDELAYFDBK:
		ret = PARAM_REVERB_DELAY_FBK;
		break;
	case REVERB_ID_PROGDELAYTIME:
		ret = PARAM_REVERB_PREDELAY;
		break;
	case MIXER_PROGLOWCUT_ID:
		ret = PARAM_MIXER_LOWCUT_FILTER_FREQ;
		break;
	case MIXER_PROGHIGHCUT_ID:
		ret = PARAM_MIXER_HIGHCUT_FILTER_FREQ;
		break;
	case MIXER_PROGINPUTGAIN_ID:
		ret = PARAM_MIXER_INPUT_GAIN;
		break;
	case MIXER_PROGLEVEL_ID:
		ret = PARAM_MIXER_OUTPUT_LEVEL;
		break;
	case MIXER_PROGREVERBSEND_ID:
		ret = PARAM_MIXER_REVERB_SEND;
		break;
	case MIXER_PROGPAN_ID:
		ret = PARAM_MIXER_PAN;
		break;
	case EQ_LOWBANDGAIN_ID:
		ret = PARAM_EQ_LOWEST_GAIN;
		break;
	case EQ_LOWBANDFREQ_ID:
		ret = PARAM_EQ_LOWEST_FREQ;
		break;
	case EQ_LOWMIDBANDQ_ID:
		ret = PARAM_EQ_LOWMID_Q;
		break;
	case EQ_LOWMIDBANDGAIN_ID:
		ret = PARAM_EQ_LOWER_GAIN;
		break;
	case EQ_LOWMIDBANDFREQ_ID:
		ret = PARAM_EQ_LOWER_FREQ;
		break;
	case EQ_HIGHMIDBANDGAIN_ID:
		ret = PARAM_EQ_HIGHER_GAIN;
		break;
	case EQ_HIGHMIDBANDFREQ_ID:
		ret = PARAM_EQ_HIGHER_FREQ;
		break;
	case EQ_HIGHMIDBANDQ_ID:
		ret = PARAM_EQ_HIGHMID_Q;
		break;
	case EQ_HIGHBANDGAIN_ID:
		ret = PARAM_EQ_HIGHEST_GAIN;
		break;
	case EQ_HIGHBANDFREQ_ID:
		ret = PARAM_EQ_HIGHEST_FREQ;
		break;
	case VOICE_ID_AMPATTACK:
		ret = PARAM_VOICE_AMP_ENV_ATTACK;
		break;
	case VOICE_ID_AMPDECAY:
		ret = PARAM_VOICE_AMP_ENV_DECAY;
		break;
	case VOICE_ID_AMPRELEASE:
		ret = PARAM_VOICE_AMP_ENV_RELEASE;
		break;
	case VOICE_ID_FILTFREQ:
		ret = PARAM_VOICE_FILTER_FREQUENCY;
		break;
	case VOICE_ID_FILTRES:
		ret = PARAM_VOICE_FILTER_RESONANCE;
		break;
	case VOICE_ID_DETUNECOURSE:
		ret = PARAM_VOICE_DETUNE_COURSE;
		break;
	case VOICE_ID_DETUNEFINE:
		ret = PARAM_VOICE_DETUNE_FINE;
		break;
	case VOICE_ID_VIBRATERATE:
		ret = PARAM_VOICE_VIBRATE_RATE;
		break;
	case VOICE_ID_VIBRATEDEPTH:
		ret = PARAM_VOICE_VIBRATE_DEPTH;
		break;
	case VOICE_ID_VIBRATEDELAY:
		ret = PARAM_VOICE_VIBRATE_DELAY;
		break;
	case VOICE_ID_LEVEL:
		ret = PARAM_VOICE_CHANNEL_VOLUME;
		break;
	case VOICE_ID_PORTMTIME:
		ret = PARAM_VOICE_PORTAMENTO_TIME;
		break;
	case VOICE_ID_PAN:
		ret = PARAM_VOICE_PAN;
		break;
	case VOICE_ID_PITCHWHEEL:
		ret = PARAM_VOICE_PITCH_WHEEL;
		break;
	case VOICE_ID_VELOCITY:
		ret = PARAM_VOICE_VELOCITY;
		break;
	case VOICE_ID_PITCHWSEMITONES:
		ret = PARAM_VOICE_PITCH_BEND_ST;
		break;
	case VOICE_ID_REVERBSEND:
		ret = PARAM_VOICE_REVERB_SEND;
		break;
	case VOICE_ID_FILTER_TYPE:
		ret = PARAM_VOICE_FILTER_TYPE;
		break;
	case VOICE_ID_FILTER_ATTACK:
		ret = PARAM_VOICE_FILTER_ATTACK;
		break;
	case VOICE_ID_FILTER_DECAY:
		ret = PARAM_VOICE_FILTER_DECAY;
		break;
	case VOICE_ID_FILTER_RELEASE:
		ret = PARAM_VOICE_FILTER_RELEASE;
		break;
	default:
		ret = PARAM_NONE;
	}
	return ret;
}

void SynthAssignEffect(unsigned char touch_control, unsigned char synth_param, unsigned char fxb,bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	if(touch_control <= VARIATION && synth_param)
	{
		char buf[64];
		pSynth->assignSynthEffect(touch_control, synth_param, fxb, inverse, variation_start, variation_end);
		//sprintf(buf,"assignSynthEffect(%d,%d,%d,%d,%d,%d);",touch_control, synth_param, fxb, inverse, variation_start, variation_end);
		//DEBUG_STDOUT(buf);
	}
}

void SynthRemoveEffect(unsigned char synth_param, unsigned char fxb)
{
	if(synth_param)
	{
		pSynth->unAssignSynthEffect(synth_param, fxb);
	}
}

std::vector<int> SynthFindEffect(unsigned char synth_param, unsigned char fxb)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;
	if(synth_param)
	{
		vectInt = pSynth->findAssignSynthEffect(synth_param, fxb, &vectAe);
		size = vectInt.size();
		for(i=0;i<size;i++)
		{
			if(VARIATION == vectInt[i])
			{
				vectInt[i] |= vectAe[i]->variation_start<<16;
				vectInt[i] |= vectAe[i]->variation_end<<24;
			}
			vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
		}
	}
	return vectInt;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa synth string
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthTuning(unsigned char str, unsigned char note)
{
	pSynth->setTuning(str, note);
}

void SynthSetMidiOutChannel(unsigned char str, signed char channel)
{
	pcs->turnCurrentNotesOff(0);
	pSynth->setMidiOutChannel(str, channel);
}

signed char SynthGetMidiOutChannel(unsigned char str)
{
	return pSynth->getMidiOutChannel(str);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa voice effect
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void VoiceAssignEffect(unsigned char touch_control, unsigned char synth_param, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	if(touch_control <= VARIATION && synth_param)
	{
		pSynth->assignSynthVoiceParam(touch_control, synth_param, str, voice_index, inverse, variation_start, variation_end);
	}
}

void VoiceRemoveEffect(unsigned char synth_param, unsigned char str, unsigned char voice_index)
{
	if(synth_param)
	{
		pSynth->unAssignSynthVoiceParam(synth_param, str, voice_index);
	}
}

std::vector<int> VoiceFindEffect(unsigned char synth_param, unsigned char str, unsigned char voice_index)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;
	if(synth_param)
	{
		vectInt = pSynth->findAssignSynthVoiceParam(synth_param, str, voice_index, &vectAe);
		size = vectInt.size();
		for(i=0;i<size;i++)
		{
			if(VARIATION == vectInt[i])
			{
				vectInt[i] |= vectAe[i]->variation_start<<16;
				vectInt[i] |= vectAe[i]->variation_end<<24;
			}
			vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
		}
	}
	return vectInt;
}

void SynthInsertVoice(int str, int wave)
{
	pSynth->insertNewVoice(str, wave);
}

void SynthRemoveVoice(int str, int voice_index)
{
	pSynth->removeVoice(str, voice_index);
}

void SynthVoiceSetFX(int str, int voice_index, int fxb, bool state)
{
	pSynth->setFxBlockOn(str, voice_index, fxb, state);
}

void SynthVoiceSetWave(int str, int voice_index, int program)
{
	pSynth->setWave(str, voice_index, program);
}

void SynthVoiceAmpEnvAttack(int str, int voice_index, int val)
{
	pSynth->setAmpEnvAttack(str, voice_index, val);
}

void SynthVoiceAmpEnvDecay(int str, int voice_index, int val)
{
	pSynth->setAmpEnvDecay(str, voice_index, val);
}

void SynthVoiceAmpEnvRelease(int str, int voice_index, int val)
{
	pSynth->setAmpEnvRelease(str, voice_index, val);
}

void SynthVoiceFilterFrequency(int str, int voice_index, int val)
{
	pSynth->setFilterFrequency(str, voice_index, val);
}

void SynthVoiceFilterResonance(int str, int voice_index, int val)
{
	pSynth->setFilterResonance(str, voice_index, val);
}

void SynthVoiceDetuneCourse(int str, int voice_index, int val)
{
	pSynth->setDetuneCourse(str, voice_index, val);
}

void SynthVoiceDetuneFine(int str, int voice_index, int val)
{
	pSynth->setDetuneFine(str, voice_index, val);
}

void SynthVoiceVibrateRate(int str, int voice_index, int val)
{
	pSynth->setVibrateRate(str, voice_index, val);
}

void SynthVoiceVibrateDepth(int str, int voice_index, int val)
{
	pSynth->setVibrateDepth(str, voice_index, val);
}

void SynthVoiceVibrateDelay(int str, int voice_index, int val)
{
	pSynth->setVibrateDelay(str, voice_index, val);
}

void SynthVoiceChannelVolume(int str, int voice_index, int val)
{
	pSynth->setChannelVolume(str, voice_index, val);
}

void SynthVoicePortamentoTime(int str, int voice_index, int val)
{
	pSynth->setPortamentoTime(str, voice_index, val);
}

void SynthVoiceReverbSend(int str, int voice_index, int val)
{
	pSynth->setReverbSend(str, voice_index, val);
}

void SynthVoicePan(int str, int voice_index, int val)
{
	pSynth->setPan(str, voice_index, val);
}

void SynthVoicePitchBendSemitones(int str, int voice_index, int val)
{
	pSynth->setPitchBendSemitones(str, voice_index, val);
}

void SynthVoicePitchWheel(int str, int voice_index, int val)
{
	pSynth->setPitchWheel(str, voice_index, val);
}

void SynthVoiceVoiceVelocity(int str, int voice_index, int val)
{
	pSynth->setVoiceVelocity(str, voice_index, val);
}

void SynthVoiceFilterType(int str, int voice_index, int val)
{
	pSynth->setFilterType(str, voice_index, val);
}

void SynthVoiceFilterAttack(int str, int voice_index, int val)
{
	pSynth->setFilterAttack(str, voice_index, val);
}

void SynthVoiceFilterDecay(int str, int voice_index, int val)
{
	pSynth->setFilterDecay(str, voice_index, val);
}

void SynthVoiceFilterRelease(int str, int voice_index, int val)
{
	pSynth->setFilterRelease(str, voice_index, val);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa MIDI control
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthAssignMidiPitch(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	pSynth->assignMidiPitch(touch_control, channel, inverse, variation_start, variation_end);
}

void SynthUnassignMidiPitch(unsigned char touch_control, unsigned char channel)
{
	pSynth->unAssignMidiPitch(touch_control, channel);
}

void SynthAssignMidiVelocity(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	pSynth->assignMidiVelocity(touch_control, channel, inverse, variation_start, variation_end);
}

void SynthUnassignMidiVelocity(unsigned char touch_control, unsigned char str)
{
	pSynth->unAssignMidiVelocity(touch_control, str);
}

void SynthAssignCC(unsigned char touch_control, unsigned char channel, unsigned int cc, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	pSynth->assignCC(touch_control, 1, channel, cc,inverse, variation_start, variation_end);
}

void SynthUnassignCC(unsigned char touch_control, unsigned char channel, unsigned int cc)
{
	pSynth->unAssignCC(MIDI_OUT, touch_control, channel, cc);
}

void SynthAssignStopSound(unsigned char str, unsigned char cc, unsigned char val)
{
	pSynth->assignMidiStopSound(str, cc, val);
}

void SynthUnassignStopSound(unsigned char str, unsigned char cc)
{
	pSynth->unAssignMidiStopSound(str, cc);
}

std::vector<int> MidiPitchFindEffect(unsigned char chan)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;
	vectInt = pSynth->findAssignMidiPitch(chan, &vectAe);
	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		if(VARIATION == vectInt[i])
		{
			vectInt[i] |= vectAe[i]->variation_start<<16;
			vectInt[i] |= vectAe[i]->variation_end<<24;
		}
		vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
	}
	return vectInt;
}

std::vector<int> MidiVelocityFindEffect(unsigned char str)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;
	vectInt = pSynth->findAssignMidiVelocity(str, &vectAe);
	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		if(VARIATION == vectInt[i])
		{
			vectInt[i] |= vectAe[i]->variation_start<<16;
			vectInt[i] |= vectAe[i]->variation_end<<24;
		}
		vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
	}
	return vectInt;
}

std::vector<ASSIGNED_EFFECT> MidiCCFindEffect(unsigned char channel)
{
	int i,size;
	ASSIGNED_EFFECT tmp;
	std::vector<int> vectInt;
	std::vector<ASSIGNED_EFFECT> vectAS;
	std::vector<struct assignable_effect*> vectAe;
	vectInt = pSynth->findAssignAllCC(MIDI_OUT, channel, &vectAe);
	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		tmp.type = vectInt[i];
		tmp.inv = vectAe[i]->inverse?1:0;
		tmp.varmin = vectAe[i]->variation_start;
		tmp.varmax = vectAe[i]->variation_end;
		tmp.cc = vectAe[i]->cc;
		vectAS.push_back(tmp);
	}
	return vectAS;
}

std::vector<int> MidiStopSoundFind(unsigned char str)
{
	std::vector<int> vectInt;
	vectInt = pSynth->findAssignStopSound(str);
	return vectInt;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa global
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthTurnNotesOff()
{
	pcs->turnCurrentNotesOff(0);
}

void SynthSetDragCentre(unsigned char synth_param, unsigned char fxb, unsigned int drag_center)
{
	pSynth->setDragCenterSynthEffect(synth_param, fxb, drag_center);
}

void VoiceSetDragCentre(unsigned char synth_param, unsigned char str, unsigned char voice_index, unsigned int drag_center)
{
	pSynth->setDragCenterSynthVoiceParam(synth_param, str, voice_index, drag_center);
}

void MidiPitchSetDragCentre(unsigned char channel, unsigned int drag_center)
{
	pSynth->setDragCenterMidiPitch(channel, drag_center);
}

void MidiVelocitySetDragCentre(int str, unsigned int drag_center)
{
	pSynth->setDragCenterMidiVelocity(str, drag_center);
}

void MidiCCSetDragCentre(unsigned char channel, unsigned int cc, unsigned int drag_center)
{
	pSynth->setDragCenterCC(MIDI_OUT, channel, cc, drag_center);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Linux system
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int UpdateSoundbank()
{
	int ret;
	//ret = system("./test");
	pSynth->closeMidiPorts();
	usleep(100);
	std::string cmd = "nice -n -20 samprom " + working_directory + "/wavetable.bin";
	ret = system(cmd.c_str());
//	ret = system("nice -n -20 samprom wavetable.bin");
	//ret = system("samprom wave.bin");
	pSynth->openMidiPorts();
	std::cout<<"Return value="<<ret<<std::endl<<std::flush;
	return ret;
}

int RebootSystem()
{
	return system("reboot");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Quick set menu
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int SynthGetMastervolume(void)
{
	return pSynth->getMasterVolume();
}

void SynthSetMastervolume(int val)
{
	pSynth->setMasterVolume(val);
}

int SynthGetTapmode()
{
	return pcs->GetTapmode()?1:0;
}

void SynthSetTapmode(int val)
{
	pcs->SetTapmode(val?true:false);
}

int SynthGetStringmode()
{
	return pcs->GetStringmode()?1:0;
}

void SynthSetStringmode(int val)
{
	pcs->SetStringmode(val?true:false);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Graphic system
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static int handmode = MISA_RIGHTHAND_MODE;
void SetLcdFlip(unsigned char flip);
unsigned char GetLcdFlip();

int MisaGetHandMode()
{
	return handmode;//GetLcdFlip();
}

int MisaSetHandMode(int mode)
{
	handmode = mode;
	SetLcdFlip(mode);
	pcs->setLeftHandMode(MISA_LEFTHAND_MODE==handmode?true:false);
	return mode;
}

int MisaGetBallTravel()
{
	return pcs->getBallTravelOn()?1:0;
}

int MisaSetBallTravel(int mode)
{
	pcs->setBallTravelOn(mode?true:false);
	return 0;
}

// OR : Drag & Drop
void SendVoiceParamToSynth(int string_index, int voice_index)
{
	pSynth->SendParamToSynth(string_index, voice_index);

}
#else

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

#include <Windows.h>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaAppInc.h"
#include "MisaWidget.h"

#include "SynthScreen.h"
#include "Compressionscreen.h"
#include "DistortionScreen.h"
#include "ModulationScreen.h"
#include "DelayScreen.h"
#include "ReverbScreen.h"
#include "MixerScreen.h"
#include "EqScreen.h"

#include "MisamenuConfig.h"

using namespace std;

const int effect_compressor::KNEE_HARD = 0;
const int effect_compressor::KNEE_SOFT = 1;
//const int effect_wahwah::TYPE_LPF = 0;
//const int effect_wahwah::TYPE_BPF = 1;
const int effect_delay::MODE_MONO = 0;
const int effect_delay::MODE_STEREO = 1;

// OR 28-06-11 : make it compile with Visual Studio 2008
std::string working_directory;		
std::vector<int> MidiStopSoundFind(unsigned char str)
{
	std::vector<int> vectInt;
	return vectInt;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa distortion stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DistortionOn(int fxb, bool state)
{
	current_setting.fx_block[fxb].distortion.on = state;
}

void DistortionSetType(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.type = val;
}

void DistortionSetDrive(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.drive = val;
}

void DistortionSetTone(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.tone = val;
}

void DistortionSetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.level = val;
}

void DistortionNoiseReduction(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.noise_reduction = val;
}

void DistortionSetBooster(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.booster = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa compressor stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CompressorSetOn(int fxb, bool state)
{
	current_setting.fx_block[fxb].compressor.on = state;
}

void CompressorSetKnee(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.knee = val;
}

void CompressorSetAttack(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.attack = val;
}

void CompressorSetRelease(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.release = val;
}

void CompressorSetThreshold(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.threshold = val;
}

void CompressorSetRatio(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.ratio = val;
}

void CompressorSetBoost(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.boost = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa delay stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DelaySetOn(int fxb, bool state)
{
	current_setting.fx_block[fxb].delay.on = state;
}

void DelaySetMode(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.mode = val;
}

void DelaySetPreLp(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.pre_lp = val;
}

void DelaySetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.level = val;
}

void DelaySetTime(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.time = val;
}

void DelaySetFeedback(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.feedback = val;
}

void DelaySetFeedbackFilter(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.feedback_filter = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa modulation stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ModulationSetOn(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.on = val?true:false;
}

void ModulationSetPreset(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.type = val;
}

void ModulationSetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.level = val;
}

void ModulationSetChorusDelayTime(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.chorus_delay_time = val;
}

void ModulationSetChorusFeedback(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.chorus_feedback = val;
}

void ModulationSetChorusHpf(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.chorus_hpf = val;
}

void ModulationSetDelayFeedbackFilter(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.delay_feedback_filter = val;
}

void ModulationSetRate(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.rate = val;
}

void ModulationSetDepth(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.depth = val;
}

void ModulationSetTremolo(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.tremolo = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa reverberation stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ReverbSetType(int val)
{
	current_setting.reverb.type = val;
}

void ReverbSetCharacter(int val)
{
	current_setting.reverb.character = val;
}

void ReverbSetPreLpf(int val)
{
	current_setting.reverb.pre_lpf = val;
}

void ReverbSetLevel(int val)
{
	current_setting.reverb.level = val;
}

void ReverbSetTime(int val)
{
	current_setting.reverb.time = val;
}

void ReverbSetDelayFeedback(int val)
{
	current_setting.reverb.delay_feedback = val;
}

void ReverbSetPreDelay(int val)
{
	current_setting.reverb.pre_delay_time = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa mixer stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MixerSetLowcut(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.low_cut_filter_frequency = val;
}

void MixerSetHighcut(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.high_cut_filter_frequency = val;
}

void MixerSetInputgain(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.input_gain = val;
}

void MixerSetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.output_level = val;
}

void MixerSetReverbsend(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.reverb_send = val;
}

void MixerSetPan(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.pan = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa EQ stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void EqOn(int val)
{
	current_setting.equalizer.on = val?true:false;
}

void EqSetLowestBandGain(int val)
{
	current_setting.equalizer.lowest.gain = val;
}

void EqSetLowestBandFrequency(int val)
{
	current_setting.equalizer.lowest.frequency = val;
}

void EqSetLowMidQ(int val)
{
	current_setting.equalizer.low_mid_q = val;
}

void EqSetLowerBandGain(int val)
{
	current_setting.equalizer.lower.gain = val;
}

void EqSetLowerBandFrequency(int val)
{
	current_setting.equalizer.lower.frequency = val;
}

void EqSetHigherBandGain(int val)
{
	current_setting.equalizer.higher.gain = val;
}

void EqSetHigherBandFrequency(int val)
{
	current_setting.equalizer.higher.frequency = val;
}

void EqSetHighMidQ(int val)
{
	current_setting.equalizer.high_mid_q = val;
}

void EqSetHighestBandGain(int val)
{
	current_setting.equalizer.highest.gain = val;
}

void EqSetHighestBandFrequency(int val)
{
	current_setting.equalizer.highest.frequency = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa assign effect stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int AssigneMentMenuTranslate(int assignment)
{
	int ret;
	switch(assignment)
	{
	case TOUCH_X:
		ret = CTL_TOUCH_X;
		break;
	case TOUCH_Y:
		ret = CTL_TOUCH_Y;
		break;
	case DRAG_X:
		ret = CTL_DRAG_X;
		break;
	case DRAG_Y:
		ret = CTL_DRAG_Y;
		break;
	case BALL_X:
		ret = CTL_BALL_X;
		break;
	case BALL_Y:
		ret = CTL_BALL_Y;
		break;
	case VARIATION:
		ret = CTL_VARIATION;
		break;
	default:
		ret = -1;
	}
	return ret;
}

int SynthTranslateCtrl(int ctrl)
{
	int ret;
	switch(ctrl)
	{
	case CTL_NONE_ID:
		ret = -1;
		break;
	case CTL_TOUCH_X_ID:
		ret = TOUCH_X;
		break;
	case CTL_TOUCH_Y_ID:
		ret = TOUCH_Y;
		break;
	case CTL_DRAG_X_ID:
		ret = DRAG_X;
		break;
	case CTL_DRAG_Y_ID:
		ret = DRAG_Y;
		break;
	case CTL_BALL_X_ID:
		ret = BALL_X;
		break;
	case CTL_BALL_Y_ID:
		ret = BALL_Y;
		break;
	case CTL_VARIATION_ID:
		ret = VARIATION;
		break;
	default:
		ret = -1;
	}
	return ret;
}

int SynthTranslateEffect(int effect)
{
	int ret;
	switch(effect)
	{
	case COMPRESSION_ID_PROGATTACK:
		ret = PARAM_COMPRESSION_ATTACK;
		break;
	case COMPRESSION_ID_PROGRELEASE:
		ret = PARAM_COMPRESSION_RELEASE;
		break;
	case COMPRESSION_ID_PROGBOOST:
		ret = PARAM_COMPRESSION_BOOST;
		break;
	case COMPRESSION_ID_PROGTRES:
		ret = PARAM_COMPRESSION_THRESHOLD;
		break;
	case COMPRESSION_ID_PROGRATIO:
		ret = PARAM_COMPRESSION_RATIO;
		break;
	case DISTORTION_ID_PROGDRIVE:
		ret = PARAM_DISTORTION_DRIVE;
		break;
	case DISTORTION_ID_PROGTONE:
		ret = PARAM_DISTORTION_TONE;
		break;
	case DISTORTION_ID_PROGLEVEL:
		ret = PARAM_DISTORTION_LEVEL;
		break;
	case MODULATION_ID_PROGMODRATE:
		ret = PARAM_MODULATION_RATE;
		break;
	case MODULATION_ID_PROGMODDEPTH:
		ret = PARAM_MODULATION_DEPTH;
		break;
	case MODULATION_ID_PROGLEVEL:
		ret = PARAM_MODULATION_LEVEL;
		break;
	case MODULATION_ID_PROGFDBK:
		ret = PARAM_MODULATION_CHORUS_FBK;
		break;
	case MODULATION_ID_PROGFDBKFILTER:
		ret = PARAM_MODULATION_DELAY_FBKFILT;
		break;
	case MODULATION_ID_PROGSHAPE:
		ret = PARAM_MODULATION_TREMOLO;
		break;
	case MODULATION_ID_PROGTIME:
		ret = PARAM_MODULATION_CHORUS_DT;
		break;
	case DELAY_ID_PROGPRELPF:
		ret = PARAM_DELAY_PRELPF;
		break;
	case DELAY_ID_PROGTIME:
		ret = PARAM_DELAY_TIME;
		break;
	case DELAY_ID_PROGLEVEL:
		ret = PARAM_DELAY_LEVEL;
		break;
	case DELAY_ID_PROGFDBK:
		ret = PARAM_DELAY_FBK;
		break;
	case DELAY_ID_PROGFDBKFILTER:
		ret = PARAM_DELAY_FBKFILT;
		break;
	case REVERB_ID_PROGPRELPF:
		ret = PARAM_REVERB_PRELPF;
		break;
	case REVERB_ID_PROGCHAR:
		ret = PARAM_REVERB_CHARACTER;
		break;
	case REVERB_ID_PROGLEVEL:
		ret = PARAM_REVERB_LEVEL;
		break;
	case REVERB_ID_PROGTIME:
		ret = PARAM_REVERB_TIME;
		break;
	case REVERB_ID_PROGDELAYFDBK:
		ret = PARAM_REVERB_DELAY_FBK;
		break;
	case REVERB_ID_PROGDELAYTIME:
		ret = PARAM_REVERB_PREDELAY;
		break;
	case MIXER_PROGLOWCUT_ID:
		ret = PARAM_MIXER_LOWCUT_FILTER_FREQ;
		break;
	case MIXER_PROGHIGHCUT_ID:
		ret = PARAM_MIXER_HIGHCUT_FILTER_FREQ;
		break;
	case MIXER_PROGINPUTGAIN_ID:
		ret = PARAM_MIXER_INPUT_GAIN;
		break;
	case MIXER_PROGLEVEL_ID:
		ret = PARAM_MIXER_OUTPUT_LEVEL;
		break;
	case MIXER_PROGREVERBSEND_ID:
		ret = PARAM_MIXER_REVERB_SEND;
		break;
	case MIXER_PROGPAN_ID:
		ret = PARAM_MIXER_PAN;
		break;
	case EQ_LOWBANDGAIN_ID:
		ret = PARAM_EQ_LOWEST_GAIN;
		break;
	case EQ_LOWBANDFREQ_ID:
		ret = PARAM_EQ_LOWEST_FREQ;
		break;
	case EQ_LOWMIDBANDQ_ID:
		ret = PARAM_EQ_LOWMID_Q;
		break;
	case EQ_LOWMIDBANDGAIN_ID:
		ret = PARAM_EQ_LOWER_GAIN;
		break;
	case EQ_LOWMIDBANDFREQ_ID:
		ret = PARAM_EQ_LOWER_FREQ;
		break;
	case EQ_HIGHMIDBANDGAIN_ID:
		ret = PARAM_EQ_HIGHER_GAIN;
		break;
	case EQ_HIGHMIDBANDFREQ_ID:
		ret = PARAM_EQ_HIGHER_FREQ;
		break;
	case EQ_HIGHMIDBANDQ_ID:
		ret = PARAM_EQ_HIGHMID_Q;
		break;
	case EQ_HIGHBANDGAIN_ID:
		ret = PARAM_EQ_HIGHEST_GAIN;
		break;
	case EQ_HIGHBANDFREQ_ID:
		ret = PARAM_EQ_HIGHEST_FREQ;
		break;
	case VOICE_ID_AMPATTACK:
		ret = PARAM_VOICE_AMP_ENV_ATTACK;
		break;
	case VOICE_ID_AMPDECAY:
		ret = PARAM_VOICE_AMP_ENV_DECAY;
		break;
	case VOICE_ID_AMPRELEASE:
		ret = PARAM_VOICE_AMP_ENV_RELEASE;
		break;
	case VOICE_ID_FILTFREQ:
		ret = PARAM_VOICE_FILTER_FREQUENCY;
		break;
	case VOICE_ID_FILTRES:
		ret = PARAM_VOICE_FILTER_RESONANCE;
		break;
	case VOICE_ID_DETUNECOURSE:
		ret = PARAM_VOICE_DETUNE_COURSE;
		break;
	case VOICE_ID_DETUNEFINE:
		ret = PARAM_VOICE_DETUNE_FINE;
		break;
	case VOICE_ID_VIBRATERATE:
		ret = PARAM_VOICE_VIBRATE_RATE;
		break;
	case VOICE_ID_VIBRATEDEPTH:
		ret = PARAM_VOICE_VIBRATE_DEPTH;
		break;
	case VOICE_ID_VIBRATEDELAY:
		ret = PARAM_VOICE_VIBRATE_DELAY;
		break;
	case VOICE_ID_LEVEL:
		ret = PARAM_VOICE_CHANNEL_VOLUME;
		break;
	case VOICE_ID_PORTMTIME:
		ret = PARAM_VOICE_PORTAMENTO_TIME;
		break;
	case VOICE_ID_PAN:
		ret = PARAM_VOICE_PAN;
		break;
	case VOICE_ID_PITCHWSEMITONES:
		ret = PARAM_VOICE_PITCH_BEND_ST;
		break;
	case VOICE_ID_REVERBSEND:
		ret = PARAM_VOICE_REVERB_SEND;
		break;
	case VOICE_ID_FILTER_TYPE:
		ret = PARAM_VOICE_FILTER_TYPE;
		break;
	case VOICE_ID_FILTER_ATTACK:
		ret = PARAM_VOICE_FILTER_ATTACK;
		break;
	case VOICE_ID_FILTER_DECAY:
		ret = PARAM_VOICE_FILTER_DECAY;
		break;
	case VOICE_ID_FILTER_RELEASE:
		ret = PARAM_VOICE_FILTER_RELEASE;
		break;
	default:
		ret = PARAM_NONE;
	}
	return ret;
}

static std::vector<struct assignable_effect> *SynthGetControlList(unsigned char touch_control)
{
	switch(touch_control)
	{
		case TOUCH_X: return &current_setting.AE_touch_x; break;
		case TOUCH_Y: return &current_setting.AE_touch_y; break;
		case DRAG_X: return &current_setting.AE_drag_x; break;
		case DRAG_Y: return &current_setting.AE_drag_y; break;
		case BALL_X: return &current_setting.AE_ball_x; break;
		case BALL_Y: return &current_setting.AE_ball_y; break;
		case STRING_DRAG_X_0: return &current_setting.AE_string_drag_x_0; break;
		case STRING_DRAG_X_1: return &current_setting.AE_string_drag_x_1; break;
		case STRING_DRAG_X_2: return &current_setting.AE_string_drag_x_2; break;
		case STRING_DRAG_X_3: return &current_setting.AE_string_drag_x_3; break;
		case STRING_DRAG_X_4: return &current_setting.AE_string_drag_x_4; break;
		case STRING_DRAG_X_5: return &current_setting.AE_string_drag_x_5; break;
		case STRING_TOUCH_X_0: return &current_setting.AE_string_touch_x_0; break;
		case STRING_TOUCH_X_1: return &current_setting.AE_string_touch_x_1; break;
		case STRING_TOUCH_X_2: return &current_setting.AE_string_touch_x_2; break;
		case STRING_TOUCH_X_3: return &current_setting.AE_string_touch_x_3; break;
		case STRING_TOUCH_X_4: return &current_setting.AE_string_touch_x_4; break;
		case STRING_TOUCH_X_5: return &current_setting.AE_string_touch_x_5; break;
		case VARIATION: return &current_setting.AE_variation; break;
	}

	return NULL;
}


void SynthAssignEffect(unsigned char touch_control, unsigned char synth_param, unsigned char fxb,bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = synth_param;
	ae.fxb = fxb;
	ae.inverse = inverse;
	ae.output = 0;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;
	SynthGetControlList(touch_control)->push_back(ae);
}

void SynthRemoveEffect(unsigned char synth_param, unsigned char fxb)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);
		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).fxb == fxb))
			control_list->erase(control_list->begin() + i);
		}
	}
}

std::vector<int> SynthFindEffect(unsigned char synth_param, unsigned char fxb)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);
		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).fxb == fxb))
			{
				controls.push_back(touch_control);
				vectAe.push_back(&control_list->at(i));
			}
		}
	}

	vectInt = controls;

	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		if(VARIATION == vectInt[i])
		{
			vectInt[i] |= vectAe[i]->variation_start<<16;
			vectInt[i] |= vectAe[i]->variation_end<<24;
		}
		vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
	}
	return vectInt;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa synth string
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthTuning(unsigned char str, unsigned char note)
{
	if(6>str)
	{
		current_setting.tuning[str]=note;
	}
}

void SynthSetMidiOutChannel(unsigned char str, signed char channel)
{
	if(6>str)
	{
		current_setting.string_midi_out_channel[str]=channel;
	}
}

signed char SynthGetMidiOutChannel(unsigned char str)
{
	if(6>str)
	{
		return current_setting.string_midi_out_channel[str];
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa voice effect stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void VoiceAssignEffect(unsigned char touch_control, unsigned char synth_param, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = synth_param;
	ae.str = str;
	ae.output = 0;
	ae.voice_index = voice_index;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;
	SynthGetControlList(touch_control)->push_back(ae);
}

void VoiceRemoveEffect(unsigned char synth_param, unsigned char str, unsigned char voice_index)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).str == str) && (control_list->at(i).voice_index == voice_index))
				control_list->erase(control_list->begin() + i);
		}
	}
}

std::vector<int> VoiceFindEffect(unsigned char synth_param, unsigned char str, unsigned char voice_index)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).str == str) && (control_list->at(i).voice_index == voice_index))
			{
				controls.push_back(touch_control);
				vectAe.push_back(&control_list->at(i));
			}
		}
	}

	vectInt = controls;

	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		if(VARIATION == vectInt[i])
		{
			vectInt[i] |= vectAe[i]->variation_start<<16;
			vectInt[i] |= vectAe[i]->variation_end<<24;
		}
		vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
	}
	return vectInt;
}

void SynthInsertVoice(int str, int wave)
{
	int v = current_setting.voices[str].size();

	struct voice new_voice;
	current_setting.voices[str].push_back(new_voice);

	//find an empty channel
	int new_channel = 0;
	std::vector<int> channel_list;

	for(int i = 0; i < 6; i++)
		for(int j = 0; j < current_setting.voices[i].size(); j++)
			channel_list.push_back(current_setting.voices[i].at(j).channel);

	for(new_channel = 0; new_channel < 32; new_channel++)
	{
//		if((new_channel == 9) || (new_channel == 25)) continue; //ignore these...?
		if(new_channel == 16) continue; //chip bug?
		if(find(channel_list.begin(), channel_list.end(), new_channel) == channel_list.end())
			break;
	}

	current_setting.voices[str].at(v).channel = new_channel;
	current_setting.voices[str].at(v).wavetable_index = wave;
	current_setting.voices[str].at(v).amp_env_attack = 64;
	current_setting.voices[str].at(v).amp_env_decay = 64;
	current_setting.voices[str].at(v).amp_env_release = 64;
	current_setting.voices[str].at(v).filter_frequency = 64;
	current_setting.voices[str].at(v).filter_resonance = 64;
	current_setting.voices[str].at(v).detune_course = 64;
	current_setting.voices[str].at(v).detune_fine = 64;
	current_setting.voices[str].at(v).vibrate_rate = 64;
	current_setting.voices[str].at(v).vibrate_depth = 64;
	current_setting.voices[str].at(v).vibrate_delay = 64;
	current_setting.voices[str].at(v).channel_volume = 64;
	current_setting.voices[str].at(v).portamento_time = 64;
	current_setting.voices[str].at(v).pan = 64;
	current_setting.voices[str].at(v).pitch_bend_semitones = 64;
	current_setting.voices[str].at(v).reverb_send = 0;
	current_setting.voices[str].at(v).filter_type = 4; //off
	current_setting.voices[str].at(v).filter_attack = 64;
	current_setting.voices[str].at(v).filter_decay = 64;
	current_setting.voices[str].at(v).filter_release = 64;
	current_setting.voices[str].at(v).insert_fx_block_on[0] = false;
	current_setting.voices[str].at(v).insert_fx_block_on[1] = false;
}

void SynthRemoveVoice(int str, int voice_index)
{
	VoiceRemoveEffect(PARAM_VOICE_AMP_ENV_ATTACK, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_AMP_ENV_DECAY, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_AMP_ENV_RELEASE, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_FILTER_FREQUENCY, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_FILTER_RESONANCE, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_DETUNE_COURSE, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_DETUNE_FINE, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_VIBRATE_RATE, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_VIBRATE_DEPTH, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_VIBRATE_DELAY, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_CHANNEL_VOLUME, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_PORTAMENTO_TIME, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_PAN, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_PITCH_BEND_ST, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_REVERB_SEND, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_FILTER_TYPE, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_FILTER_ATTACK, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_FILTER_DECAY, str, voice_index);
	VoiceRemoveEffect(PARAM_VOICE_FILTER_RELEASE, str, voice_index);
	//unAssignSynthPitch(str, voice_index);
	current_setting.voices[str].erase(current_setting.voices[str].begin() + voice_index);

	//we must update all references to voice parameter controls
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if(((control_list->at(i).name >= PARAM_VOICE_AMP_ENV_ATTACK) && (control_list->at(i).name <= PARAM_VOICE_REVERB_SEND)) || (control_list->at(i).name == PARAM_MIDI_PITCH))
			{
				if((control_list->at(i).str == str) && (control_list->at(i).voice_index > voice_index))
				{
					control_list->at(i).voice_index--;
				}
				//fix: when velocity is per voice i will need to add code to re-adjust here
			}
		}
	}
}

void SynthVoiceSetFX(int str, int voice_index, int fxb, bool state)
{
	current_setting.voices[str].at(voice_index).insert_fx_block_on[fxb] = state;
}

void SynthVoiceSetWave(int str, int voice_index, int program)
{
	current_setting.voices[str].at(voice_index).wavetable_index = program;
}

void SynthVoiceAmpEnvAttack(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).amp_env_attack = val;
}

void SynthVoiceAmpEnvDecay(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).amp_env_decay = val;
}

void SynthVoiceAmpEnvRelease(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).amp_env_release = val;	
}

void SynthVoiceFilterFrequency(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_frequency = val;
}

void SynthVoiceFilterResonance(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_resonance = val;
}

void SynthVoiceDetuneCourse(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).detune_course = val;
}

void SynthVoiceDetuneFine(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).detune_fine = val;
}

void SynthVoiceVibrateRate(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).vibrate_rate = val;
}

void SynthVoiceVibrateDepth(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).vibrate_depth = val;
}

void SynthVoiceVibrateDelay(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).vibrate_delay = val;
}

void SynthVoiceChannelVolume(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).channel_volume = val;
}

void SynthVoicePortamentoTime(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).portamento_time = val;
}

void SynthVoiceReverbSend(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).reverb_send = val;
}

void SynthVoicePan(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).pan = val;
}

void SynthVoicePitchBendSemitones(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).pitch_bend_semitones = val;
}

void SynthVoicePitchWheel(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).pitch_wheel = val;
}

void SynthVoiceVoiceVelocity(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).velocity = val;
}

void SynthVoiceFilterType(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_type = val;
}

void SynthVoiceFilterAttack(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_attack = val;
}

void SynthVoiceFilterDecay(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_decay = val;
}

void SynthVoiceFilterRelease(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_release = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa MIDI control stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthAssignMidiPitch(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_MIDI_PITCH;
	ae.output = 1;
	ae.channel = channel;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	SynthGetControlList(touch_control)->push_back(ae);
}

void SynthUnassignMidiPitch(unsigned char touch_control, unsigned char channel)
{
	//pSynth->unAssignMidiPitch(touch_control, channel);
}

void SynthAssignMidiVelocity(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_MIDI_VELOCITY;
	ae.str = channel;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	SynthGetControlList(touch_control)->push_back(ae);
}

void SynthUnassignMidiVelocity(unsigned char touch_control, unsigned char str)
{
	//pSynth->unAssignMidiVelocity(touch_control, str);
}

void SynthAssignCC(unsigned char touch_control, unsigned char channel, unsigned int cc, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_CC;
	ae.output = 1;
	ae.channel = channel;
	ae.cc = cc;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	SynthGetControlList(touch_control)->push_back(ae);
}

void SynthUnassignCC(unsigned char touch_control, unsigned char channel, unsigned int cc)
{
	//pSynth->unAssignCC(MIDI_OUT, touch_control, channel, cc);
}

void SynthAssignStopSound(unsigned char str, unsigned char cc, unsigned char val)
{
}

void SynthUnassignStopSound(unsigned char str, unsigned char cc)
{
}

std::vector<int> MidiPitchFindEffect(unsigned char chan)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;

	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_MIDI_PITCH) && (control_list->at(i).output == 1) && (control_list->at(i).channel == chan))
			{
				controls.push_back(touch_control);
				vectAe.push_back(&control_list->at(i));
			}
		}
	}

	vectInt = controls;
	
	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		if(VARIATION == vectInt[i])
		{
			vectInt[i] |= vectAe[i]->variation_start<<16;
			vectInt[i] |= vectAe[i]->variation_end<<24;
		}
		vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
	}
	return vectInt;
}

std::vector<int> MidiVelocityFindEffect(unsigned char str)
{
	int i,size;
	std::vector<int> vectInt;
	std::vector<struct assignable_effect*> vectAe;

	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_MIDI_VELOCITY) && (control_list->at(i).str == str))
			{
				controls.push_back(touch_control);
				vectAe.push_back(&control_list->at(i));
			}
		}
	}

	vectInt = controls;
	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		if(VARIATION == vectInt[i])
		{
			vectInt[i] |= vectAe[i]->variation_start<<16;
			vectInt[i] |= vectAe[i]->variation_end<<24;
		}
		vectInt[i] |= vectAe[i]->inverse?0x00000100:0;
	}
	return vectInt;
}

std::vector<ASSIGNED_EFFECT> MidiCCFindEffect(unsigned char channel)
{
	int i,size;
	ASSIGNED_EFFECT tmp;
	std::vector<int> vectInt;
	std::vector<ASSIGNED_EFFECT> vectAS;
	std::vector<struct assignable_effect*> vectAe;

	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = SynthGetControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_CC) && (control_list->at(i).output == 1) && (control_list->at(i).channel == channel))
			{
				controls.push_back(touch_control);
				vectAe.push_back(&control_list->at(i));
			}
		}
	}

	vectInt = controls;
	size = vectInt.size();
	for(i=0;i<size;i++)
	{
		tmp.type = vectInt[i];
		tmp.inv = vectAe[i]->inverse?1:0;
		tmp.varmin = vectAe[i]->variation_start;
		tmp.varmax = vectAe[i]->variation_end;
		tmp.cc = vectAe[i]->cc;
		vectAS.push_back(tmp);
	}
	return vectAS;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa global
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthTurnNotesOff()
{
}

void SynthSetDragCentre(unsigned char synth_param, unsigned char fxb, unsigned int drag_center)
{
}

void VoiceSetDragCentre(unsigned char synth_param, unsigned char str, unsigned char voice_index, unsigned int drag_center)
{
}

void MidiPitchSetDragCentre(unsigned char channel, unsigned int drag_center)
{
}

void MidiVelocitySetDragCentre(int str, unsigned int drag_center)
{
}

void MidiCCSetDragCentre(unsigned char channel, unsigned int cc, unsigned int drag_center)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Linux system stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int UpdateSoundbank()
{
	int ret=0;
	Sleep(10000);
	return ret;
}

int RebootSystem()
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Quick set menu
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static int synthtapmode;
static int synthstringmode;

int SynthGetMastervolume(void)
{
	return 64;
}

void SynthSetMastervolume(int val)
{
}

int SynthGetTapmode()
{
	return synthtapmode;
}

void SynthSetTapmode(int val)
{
	synthtapmode = val;
}

int SynthGetStringmode()
{
	return synthstringmode;
}

void SynthSetStringmode(int val)
{
	synthstringmode = val;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Graphic system stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static int handmode = MISA_RIGHTHAND_MODE;
static int balltravel;

extern "C"
{
void SetLcdFlip(unsigned char flip);
unsigned char GetLcdFlip();
}
int MisaGetHandMode()
{
	return handmode;//GetLcdFlip();
}

int MisaSetHandMode(int mode)
{
	handmode = mode;
	SetLcdFlip(mode);
	return mode;
}

int MisaGetBallTravel()
{
	return balltravel;
}

int MisaSetBallTravel(int mode)
{
	balltravel = mode;
	return mode;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa reset all stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void ResetAllEffect()
{
	for(int s = 0; s < 6; s++)
	{
		//for(int v = 0; v < current_setting.voices[s].size(); v++)
		//	delete(current_setting.voices[s].at(v));

		current_setting.voices[s].clear();
		current_setting.string_midi_out_channel[s] = -1;
	}
	memset(&current_setting.equalizer,0,sizeof(struct eq));
	memset(&current_setting.reverb,0,sizeof(struct effect_reverb));
	memset(&current_setting.fx_block[0],0,sizeof(struct fxb));
	memset(&current_setting.fx_block[1],0,sizeof(struct fxb));
	current_setting.master_volume = 0x7F;
	EqSetLowestBandGain(0x60);
	EqSetLowerBandGain(0x38);
	EqSetHigherBandGain(0x38);
	EqSetHighestBandGain(0x60);
	EqSetLowestBandFrequency(0x0A);
	EqSetLowerBandFrequency(0x18);
	EqSetHigherBandFrequency(0x68);
	EqSetHighestBandFrequency(0x7F);
	EqSetLowMidQ(0x40);
	EqSetHighMidQ(0x40);
	ReverbSetType(0x04);
	ReverbSetCharacter(0x04);
	ReverbSetPreLpf(0);
	ReverbSetLevel(0x40);
	ReverbSetTime(0);
	ReverbSetDelayFeedback(0);
	ReverbSetPreDelay(0);
	for(int i = 0; i < 2; i++)
	{
		DistortionOn(i, false);
		DistortionSetType(i, 0);
		DistortionSetLevel(i, 0);
		DistortionSetDrive(i, 0);
		DistortionSetTone(i, 0);

		CompressorSetOn(i, false);
		CompressorSetAttack(i, 0);
		CompressorSetRelease(i, 0);
		CompressorSetThreshold(i, 0);
		CompressorSetRatio(i, 0);
		CompressorSetBoost(i, 0);
		CompressorSetKnee(i, 0);

		ModulationSetOn(i, 0);
		ModulationSetPreset(i, 1);
		ModulationSetLevel(i, 56);
		ModulationSetChorusDelayTime(i, 20);
		ModulationSetChorusFeedback(i, 0);
		ModulationSetChorusHpf(i, 0);
		ModulationSetDelayFeedbackFilter(i, 0);
		ModulationSetRate(i, 40);
		ModulationSetDepth(i, 30);
		ModulationSetTremolo(i, 0);

		DelaySetOn(i, false);
		DelaySetMode(i, 0);
		DelaySetPreLp(i, 0);
		DelaySetLevel(i, 0);
		DelaySetTime(i, 0);
		DelaySetFeedback(i, 0);
		DelaySetFeedbackFilter(i, 0);

		MixerSetLowcut(i, 0x7F); //fix: datasheet says opposite
		MixerSetHighcut(i, 0x7F);
		MixerSetInputgain(i, 0x7F);
		MixerSetLevel(i, 0x7F);
		MixerSetReverbsend(i, 0);
	}
}

void SynthLoadPreset(std::string filename)
{
#if 1
	int i;
	ResetAllEffect();
	std::ifstream ifs((working_directory + "/presets/" + filename).c_str(), std::ios::binary);

	ifs.read((char *) &current_setting.tuning, 6);
	ifs.read((char *) &current_setting.string_midi_out_channel, 6);

	ifs.read((char *) &current_setting.master_volume, sizeof(int));
	//setMasterVolume(current_setting.master_volume);
	ifs.read((char *) &current_setting.lfo_rate, sizeof(int));
	//setLfoRate(current_setting.lfo_rate);
	ifs.read((char *) &current_setting.lfo_frequency_depth, sizeof(int));
	//setLfoFrequencyDepth(current_setting.lfo_frequency_depth);
	ifs.read((char *) &current_setting.lfo_amp_depth, sizeof(int));
	//setLfoAmpDepth(current_setting.lfo_amp_depth);
	ifs.read((char *) &current_setting.lfo_pitch_depth, sizeof(int));
	//setLfoPitchDepth(current_setting.lfo_pitch_depth);

	ifs.read((char *) &current_setting.equalizer, sizeof(struct eq));
	/*setEQLowestBandGain(current_setting.equalizer.lowest.gain);
	setEQLowerBandGain(current_setting.equalizer.lower.gain);
	setEQHigherBandGain(current_setting.equalizer.higher.gain);
	setEQHighestBandGain(current_setting.equalizer.highest.gain);
	setEQLowestBandFrequency(current_setting.equalizer.lowest.frequency);
	setEQLowerBandFrequency(current_setting.equalizer.lower.frequency);
	setEQHigherBandFrequency(current_setting.equalizer.higher.frequency);
	setEQHighestBandFrequency(current_setting.equalizer.highest.frequency);
	setEQLowMidQ(current_setting.equalizer.low_mid_q);
	setEQHighMidQ(current_setting.equalizer.high_mid_q);*/

	ifs.read((char *) &current_setting.reverb, sizeof(struct effect_reverb));
	/*setReverbType(current_setting.reverb.type);
	setReverbCharacter(current_setting.reverb.character);
	setReverbPreLpf(current_setting.reverb.pre_lpf);
	setReverbLevel(current_setting.reverb.level);
	setReverbTime(current_setting.reverb.time);
	setReverbDelayFeedback(current_setting.reverb.delay_feedback);
	setReverbPreDelay(current_setting.reverb.pre_delay_time);*/

	for(i = 0; i < 2; i++)
	{
		ifs.read((char *) &current_setting.fx_block[i], sizeof(struct fxb));
		/*setDistortionOn(i, current_setting.fx_block[i].distortion.on);
		setDistortionType(i, current_setting.fx_block[i].distortion.type);
		setDistortionLevel(i, current_setting.fx_block[i].distortion.level);
		setDistortionDrive(i, current_setting.fx_block[i].distortion.drive);
		setDistortionTone(i, current_setting.fx_block[i].distortion.tone);
		setDistortionNoiseReduction(i, current_setting.fx_block[i].distortion.noise_reduction);
		setDistortionBooster(i, current_setting.fx_block[i].distortion.booster);

		setCompressorOn(i, current_setting.fx_block[i].compressor.on);
		setCompressorAttack(i, current_setting.fx_block[i].compressor.attack);
		setCompressorRelease(i, current_setting.fx_block[i].compressor.release);
		setCompressorThreshold(i, current_setting.fx_block[i].compressor.threshold);
		setCompressorRatio(i, current_setting.fx_block[i].compressor.ratio);
		setCompressorBoost(i, current_setting.fx_block[i].compressor.boost);
		setCompressorKnee(i, current_setting.fx_block[i].compressor.knee);

		setModulationType(i, current_setting.fx_block[i].modulation.type);
		setModulationOn(i, current_setting.fx_block[i].modulation.on);
		setModulationLevel(i, current_setting.fx_block[i].modulation.level);
		setModulationChorusDelayTime(i, current_setting.fx_block[i].modulation.chorus_delay_time);
		setModulationChorusFeedback(i, current_setting.fx_block[i].modulation.chorus_feedback);
		setModulationChorusHpf(i, current_setting.fx_block[i].modulation.chorus_hpf);
		setModulationDelayFeedbackFilter(i, current_setting.fx_block[i].modulation.delay_feedback_filter);
		setModulationRate(i, current_setting.fx_block[i].modulation.rate);
		setModulationDepth(i, current_setting.fx_block[i].modulation.depth);
		setModulationTremolo(i, current_setting.fx_block[i].modulation.tremolo);

		setDelayOn(i, current_setting.fx_block[i].delay.on);
		setDelayMode(i, current_setting.fx_block[i].delay.mode);
		setDelayPreLp(i, current_setting.fx_block[i].delay.pre_lp);
		setDelayLevel(i, current_setting.fx_block[i].delay.level);
		setDelayTime(i, current_setting.fx_block[i].delay.time);
		setDelayFeedback(i, current_setting.fx_block[i].delay.feedback);
		setDelayFeedbackFilter(i, current_setting.fx_block[i].delay.feedback_filter);

		setMixerLowCutFilterFrequency(i, current_setting.fx_block[i].mixer.low_cut_filter_frequency);
		setMixerHighCutFilterFrequency(i, current_setting.fx_block[i].mixer.high_cut_filter_frequency);
		setMixerInputGain(i, current_setting.fx_block[i].mixer.input_gain);
		setMixerOutputLevel(i, current_setting.fx_block[i].mixer.output_level);
		setMixerReverbSend(i, current_setting.fx_block[i].mixer.reverb_send);*/
	}

	unsigned int vector_size;

	//read voice vectors
	for(i = 0; i < 6; i++)
	{
		ifs.read((char *) &vector_size, sizeof(unsigned int));
		current_setting.voices[i].resize(vector_size);
		ifs.read((char *) &current_setting.voices[i][0], sizeof(struct voice) *vector_size);

		/*for(int j = 0; j < current_setting.voices[i].size(); j++)
		{
			setChannel(i, j, current_setting.voices[i].at(j).channel);
			setWave(i, j, current_setting.voices[i].at(j).wavetable_index);
			setAmpEnvAttack(i, j, current_setting.voices[i].at(j).amp_env_attack);
			setAmpEnvDecay(i, j, current_setting.voices[i].at(j).amp_env_decay);
			setAmpEnvRelease(i, j, current_setting.voices[i].at(j).amp_env_release);
			setFilterFrequency(i, j, current_setting.voices[i].at(j).filter_frequency);
			setFilterResonance(i, j, current_setting.voices[i].at(j).filter_resonance);
			setDetuneCourse(i, j, current_setting.voices[i].at(j).detune_course);
			setDetuneFine(i, j, current_setting.voices[i].at(j).detune_fine);
			setVibrateRate(i, j, current_setting.voices[i].at(j).vibrate_rate);
			setVibrateDepth(i, j, current_setting.voices[i].at(j).vibrate_depth);
			setVibrateDelay(i, j, current_setting.voices[i].at(j).vibrate_delay);
			setChannelVolume(i, j, current_setting.voices[i].at(j).channel_volume);
			setPortamentoTime(i, j, current_setting.voices[i].at(j).portamento_time);
			setPan(i, j, current_setting.voices[i].at(j).pan);
			setPitchBendSemitones(i, j, current_setting.voices[i].at(j).pitch_bend_semitones);
			setReverbSend(i, j, current_setting.voices[i].at(j).reverb_send);

			if(current_setting.voices[i].at(j).insert_fx_block_on[0])
				setFxBlockOn(i, j, 0, true);
			else
			if(current_setting.voices[i].at(j).insert_fx_block_on[1])
				setFxBlockOn(i, j, 1, true);
		}*/
	}

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_touch_x.resize(vector_size);
	ifs.read((char *) &current_setting.AE_touch_x[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_touch_y.resize(vector_size);
	ifs.read((char *) &current_setting.AE_touch_y[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_drag_x.resize(vector_size);
	ifs.read((char *) &current_setting.AE_drag_x[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_drag_y.resize(vector_size);
	ifs.read((char *) &current_setting.AE_drag_y[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_ball_x.resize(vector_size);
	ifs.read((char *) &current_setting.AE_ball_x[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_ball_y.resize(vector_size);
	ifs.read((char *) &current_setting.AE_ball_y[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_0.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_0[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_1.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_1[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_2.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_2[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_3.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_3[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_4.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_4[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_5.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_5[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_0.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_0[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_1.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_1[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_2.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_2[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_3.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_3[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_4.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_4[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_5.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_5[0], sizeof(struct assignable_effect) * vector_size);

	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_variation.resize(vector_size);
	ifs.read((char *) &current_setting.AE_variation[0], sizeof(struct assignable_effect) * vector_size);
	
	ifs.close();
#else
	std::ifstream preset_file((working_directory + "/presets/" + filepath).c_str());

	if(!preset_file.is_open())
	{
		return;
	}
	ResetAllEffect();
	char buffer[80];
	int current_string = 0, current_fx_block = 0;
	int voice_index = 0;
	struct voice *new_voice;

	while(!preset_file.eof())
	{
		std::string field1, field2, field3, field4, field5, field6, field7, field8;
		preset_file.getline(buffer, sizeof(buffer));
		std::istringstream istr(std::string(buffer), std::ios_base::out);
		istr >> field1 >> field2 >> field3 >> field4 >> field5 >> field6 >> field7 >> field8;

		if(field1 == "name")
		{
			current_setting.preset_name = field2;
		}
		else
		if(field1 == "master_volume")
		{
			current_setting.master_volume = atoi(field2.c_str());
		}
		else
		if(field1 == "lfo_rate")
		{
			current_setting.lfo_rate = atoi(field2.c_str());
		}
		else
		if(field1 == "lfo_frequency_depth")
		{
			current_setting.lfo_frequency_depth = atoi(field2.c_str());
		}
		else
		if(field1 == "lfo_amp_depth")
		{
			current_setting.lfo_amp_depth = atoi(field2.c_str());
		}
		else
		if(field1 == "lfo_pitch_depth")
		{
			current_setting.lfo_pitch_depth = atoi(field2.c_str());
		}
		else
		if(field1 == "string")
		{
			current_string = atoi(field2.c_str());
			new_voice = new struct voice;
			current_setting.voices[current_string].push_back(new_voice);
			voice_index = current_setting.voices[current_string].size() - 1;
		}
		else
		if(field1 == "channel")
		{
			current_setting.voices[current_string].at(voice_index)->channel = atoi(field2.c_str());
		}
		else
		if(field1 == "wave")
		{
			int channel = current_setting.voices[current_string].at(voice_index)->channel;
			current_setting.voices[current_string].at(voice_index)->bank = atoi(field2.c_str());
			current_setting.voices[current_string].at(voice_index)->wavetable_index = atoi(field3.c_str());
		}
		else
		if(field1 == "amp_env_attack")
		{
			current_setting.voices[current_string].at(voice_index)->amp_env_attack = atoi(field2.c_str());
		}
		else
		if(field1 == "amp_env_decay")
		{
			current_setting.voices[current_string].at(voice_index)->amp_env_decay = atoi(field2.c_str());
		}
		else
		if(field1 == "amp_env_release")
		{
			current_setting.voices[current_string].at(voice_index)->amp_env_release = atoi(field2.c_str());
		}
		else
		if(field1 == "filter_frequency")
		{
			current_setting.voices[current_string].at(voice_index)->filter_frequency = atoi(field2.c_str());
		}
		else
		if(field1 == "filter_resonance")
		{
			current_setting.voices[current_string].at(voice_index)->filter_resonance = atoi(field2.c_str());
		}
		else
		if(field1 == "detune_course")
		{
			current_setting.voices[current_string].at(voice_index)->detune_course = atoi(field2.c_str());
		}
		else
		if(field1 == "detune_fine")
		{
			current_setting.voices[current_string].at(voice_index)->detune_fine = atoi(field2.c_str());
		}
		else
		if(field1 == "vibrate_rate")
		{
			current_setting.voices[current_string].at(voice_index)->vibrate_rate = atoi(field2.c_str());
		}
		else
		if(field1 == "vibrate_depth")
		{
			current_setting.voices[current_string].at(voice_index)->vibrate_depth = atoi(field2.c_str());
		}
		else
		if(field1 == "vibrate_delay")
		{
			current_setting.voices[current_string].at(voice_index)->vibrate_delay = atoi(field2.c_str());
		}
		else
		if(field1 == "filter_type")
		{
			current_setting.voices[current_string].at(voice_index)->filter_type = atoi(field2.c_str());
		}
		else
		if(field1 == "filter_attack")
		{
			current_setting.voices[current_string].at(voice_index)->filter_attack = atoi(field2.c_str());
		}
		else
		if(field1 == "filter_decay")
		{
			current_setting.voices[current_string].at(voice_index)->filter_decay = atoi(field2.c_str());
		}
		else
		if(field1 == "filter_release")
		{
			current_setting.voices[current_string].at(voice_index)->filter_release = atoi(field2.c_str());
		}
		else
		if(field1 == "channel_volume")
		{
			current_setting.voices[current_string].at(voice_index)->channel_volume = atoi(field2.c_str());
		}
		else
		if(field1 == "portamento_time")
		{
			current_setting.voices[current_string].at(voice_index)->portamento_time = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_send")
		{
			current_setting.voices[current_string].at(voice_index)->reverb_send = atoi(field2.c_str());
		}
		else
		if(field1 == "pan")
		{
			current_setting.voices[current_string].at(voice_index)->pan = atoi(field2.c_str());
		}
		else
		if(field1 == "pitch_bend_semitones")
		{
			current_setting.voices[current_string].at(voice_index)->pitch_bend_semitones = atoi(field2.c_str());
		}
		else
		if(field1 == "fx_block_on")
		{
			current_setting.voices[current_string].at(voice_index)->insert_fx_block_on[atoi(field2.c_str())] = true;
		}
		else
		if(field1 == "fx_block")
		{
			current_fx_block = atoi(field2.c_str());
		}
		else
		if(field1 == "distortion_on")
		{
			current_setting.fx_block[current_fx_block].distortion.on = true;
		}
		else
		if(field1 == "distortion_type")
		{
			current_setting.fx_block[current_fx_block].distortion.type = atoi(field2.c_str());
		}
		else
		if(field1 == "distortion_post_gain")
		{
			current_setting.fx_block[current_fx_block].distortion.post_gain = atoi(field2.c_str());
		}
		else
		if(field1 == "distortion_drive")
		{
			current_setting.fx_block[current_fx_block].distortion.drive = atoi(field2.c_str());
		}
		else
		if(field1 == "distortion_tone")
		{
			current_setting.fx_block[current_fx_block].distortion.tone = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_level")
		{
			current_setting.fx_block[current_fx_block].modulation.level = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_chorus_delay_time")
		{
			current_setting.fx_block[current_fx_block].modulation.chorus_delay_time = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_chorus_feedback")
		{
			current_setting.fx_block[current_fx_block].modulation.chorus_feedback = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_chorus_hpf")
		{
			current_setting.fx_block[current_fx_block].modulation.chorus_hpf = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_delay_feedback_filter")
		{
			current_setting.fx_block[current_fx_block].modulation.delay_feedback_filter = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_rate")
		{
			current_setting.fx_block[current_fx_block].modulation.rate = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_depth")
		{
			current_setting.fx_block[current_fx_block].modulation.depth = atoi(field2.c_str());
		}
		else
		if(field1 == "modulation_tremolo")
		{
			current_setting.fx_block[current_fx_block].modulation.tremolo = atoi(field2.c_str());
		}
		else
		if(field1 == "delay_on")
		{
			current_setting.fx_block[current_fx_block].delay.on = true;
		}
		else
		if(field1 == "delay_mode")
		{
			current_setting.fx_block[current_fx_block].delay.mode = atoi(field2.c_str());
		}
		else
		if(field1 == "delay_pre_lp")
		{
			current_setting.fx_block[current_fx_block].delay.pre_lp = atoi(field2.c_str());
		}
		else
		if(field1 == "delay_level")
		{
			current_setting.fx_block[current_fx_block].delay.level = atoi(field2.c_str());
		}
		else
		if(field1 == "delay_time")
		{
			current_setting.fx_block[current_fx_block].delay.time = atoi(field2.c_str());
		}
		else
		if(field1 == "delay_feedback")
		{
			current_setting.fx_block[current_fx_block].delay.feedback = atoi(field2.c_str());
		}
		else
		if(field1 == "delay_feedback_filter")
		{
			current_setting.fx_block[current_fx_block].delay.feedback_filter = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_type")
		{
			current_setting.reverb.type = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_character")
		{
			current_setting.reverb.character = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_pre_lpf")
		{
			current_setting.reverb.pre_lpf = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_level")
		{
			current_setting.reverb.level = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_time")
		{
			current_setting.reverb.time = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_delay_feedback")
		{
			current_setting.reverb.delay_feedback = atoi(field2.c_str());
		}
		else
		if(field1 == "reverb_pre_delay")
		{
			current_setting.reverb.pre_delay_time = atoi(field2.c_str());
		}
		else
		if(field1 == "midi_channel")
		{
			current_setting.string_midi_out_channel[current_string] = atoi(field2.c_str());
		}
	}

	preset_file.close();	
#endif
}

void SynthSavePreset(PSYNTH_SETTING pSetting,std::string filepath)
{
}

bool GetCurrentSetting(PSYNTH_SETTING pSetting)
{
	*pSetting = current_setting;
	return true;
}

// OR : Drag & Drop
PSYNTH_SETTING GetCurrentSettingPtr()
{
	return &current_setting;
}
void SendVoiceParamToSynth(int string_index, int voice_index)
{
	// Do nothing in the Windows simulation
}

#endif // Linux

//***********************************************************************
//
//	Common Code (Linux & Windows Simulation)
//
//***********************************************************************

// end of common code (Linux & Windows Simulation)

