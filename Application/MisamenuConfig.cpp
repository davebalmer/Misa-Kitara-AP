
#include "GUI.h"
#include "WM.h"

static bool PresetModified = false;
static bool lockModified = false;
static WM_HWIN hWinToInvalidate = 0;
GUI_RECT RectToInvalidate ={0, 0, 800, 62};
static void SetModified();
static bool IsModified();

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
#include "BitcrusherScreen.h"
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
	ClearModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth preset operating
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthLoadPreset(std::string filename)
{
	pSynth->loadPresetFromFile(filename);
	ClearModified();
}

void SynthDeletePreset(std::string filename)
{
	pSynth->deletePresetFile(filename);
	ClearModified();
}

void SynthSavePreset(PSYNTH_SETTING pSetting,std::string filename)
{
	pSynth->savePresetToFile(pSetting, filename);
	ClearModified();
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
// Misa configuration file
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SaveToConfigFile(void)
{
	pcs->saveConfigFile();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa bitcrusher stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BitCrusherOn(int fxb, bool state)
{
	pSynth->setBitCrusherOn(fxb, state);
	SetModified();
}

void BitCrusherSetBitResolution(int fxb, int val)
{
	pSynth->setBitcrusherBitResolution(fxb, val);
	SetModified();
}

//void BitCrusherSetBrightness(int fxb, int val)
//{
//	pSynth->setBitcrusherBrightness(fxb, val);
//	SetModified();
//}

void BitCrusherSetDownsampling(int fxb, int val)
{
	pSynth->setBitcrusherDownsampling(fxb, val);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa distortion
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DistortionOn(int fxb, bool state)
{
	pSynth->setDistortionOn(fxb, state);
	SetModified();
}

void DistortionSetType(int fxb, int val)
{
	pSynth->setDistortionType(fxb, val);
	SetModified();
}

void DistortionSetDrive(int fxb, int val)
{
	pSynth->setDistortionDrive(fxb, val);
	SetModified();
}

void DistortionSetTone(int fxb, int val)
{
	pSynth->setDistortionTone(fxb, val);
	SetModified();
}

void DistortionSetLevel(int fxb, int val)
{
	pSynth->setDistortionLevel(fxb, val);
	SetModified();
}

void DistortionNoiseReduction(int fxb, int val)
{
	pSynth->setDistortionNoiseReduction(fxb, val);
	SetModified();
}

void DistortionSetBooster(int fxb, int val)
{
	pSynth->setDistortionBooster(fxb, val);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa compressor
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CompressorSetOn(int fxb, bool state)
{
	pSynth->setCompressorOn(fxb, state);
	SetModified();
}

void CompressorSetKnee(int fxb, int val)
{
	pSynth->setCompressorKnee(fxb, val);
	SetModified();
}

void CompressorSetAttack(int fxb, int val)
{
	pSynth->setCompressorAttack(fxb, val);
	SetModified();
}

void CompressorSetRelease(int fxb, int val)
{
	pSynth->setCompressorRelease(fxb, val);
	SetModified();
}

void CompressorSetThreshold(int fxb, int val)
{
	pSynth->setCompressorThreshold(fxb, val);
	SetModified();
}

void CompressorSetRatio(int fxb, int val)
{
	pSynth->setCompressorRatio(fxb, val);
	SetModified();
}

void CompressorSetBoost(int fxb, int val)
{
	pSynth->setCompressorBoost(fxb, val);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa delay
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DelaySetOn(int fxb, bool state)
{
	pSynth->setDelayOn(fxb, state);
	SetModified();
}

void DelaySetMode(int fxb, int val)
{
	pSynth->setDelayMode(fxb, val);
	SetModified();
}

void DelaySetPreLp(int fxb, int val)
{
	pSynth->setDelayPreLp(fxb, val);
	SetModified();
}

void DelaySetLevel(int fxb, int val)
{
	pSynth->setDelayLevel(fxb, val);
	SetModified();
}

void DelaySetTime(int fxb, int val)
{
	pSynth->setDelayTime(fxb, val);
	SetModified();
}

void DelaySetFeedback(int fxb, int val)
{
	pSynth->setDelayFeedback(fxb, val);
	SetModified();
}

void DelaySetFeedbackFilter(int fxb, int val)
{
	pSynth->setDelayFeedbackFilter(fxb, val);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa modulation
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ModulationSetOn(int fxb, int val)
{
	pSynth->setModulationOn(fxb, val?true:false);
	SetModified();
}

void ModulationSetPreset(int fxb, int val)
{
	pSynth->setModulationType(fxb, val);
	SetModified();
}

void ModulationSetLevel(int fxb, int val)
{
	pSynth->setModulationLevel(fxb, val);
	SetModified();
}

void ModulationSetChorusDelayTime(int fxb, int val)
{
	pSynth->setModulationChorusDelayTime(fxb, val);
	SetModified();
}

void ModulationSetChorusFeedback(int fxb, int val)
{
	pSynth->setModulationChorusFeedback(fxb, val);
	SetModified();
}

void ModulationSetChorusHpf(int fxb, int val)
{
	pSynth->setModulationChorusHpf(fxb, val);
	SetModified();
}

void ModulationSetDelayFeedbackFilter(int fxb, int val)
{
	pSynth->setModulationDelayFeedbackFilter(fxb, val);
	SetModified();
}

void ModulationSetRate(int fxb, int val)
{
	pSynth->setModulationRate(fxb, val);
	SetModified();
}

void ModulationSetDepth(int fxb, int val)
{
	pSynth->setModulationDepth(fxb, val);
	SetModified();
}

void ModulationSetTremolo(int fxb, int val)
{
	pSynth->setModulationTremolo(fxb, val);
	SetModified();
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
	SetModified();
}

void ReverbSetCharacter(int val)
{
	pSynth->setReverbCharacter(val);
	//sprintf(buf,"ReverbSetCharacter val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
	SetModified();
}

void ReverbSetPreLpf(int val)
{
	pSynth->setReverbPreLpf(val);
	//sprintf(buf,"ReverbSetPreLpf val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
	SetModified();
}

void ReverbSetLevel(int val)
{
	pSynth->setReverbLevel(val);
	//sprintf(buf,"ReverbSetLevel val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
	SetModified();
}

void ReverbSetTime(int val)
{
	pSynth->setReverbTime(val);
	//sprintf(buf,"ReverbSetTime val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
	SetModified();
}

void ReverbSetDelayFeedback(int val)
{
	pSynth->setReverbDelayFeedback(val);
	//sprintf(buf,"ReverbSetDelayFeedback val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
	SetModified();
}

void ReverbSetPreDelay(int val)
{
	pSynth->setReverbPreDelay(val);
	//sprintf(buf,"ReverbSetPreDelay val=%d,FX=%d",val,0);
	//DEBUG_STDOUT(buf);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa mixer
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MixerSetLowcut(int fxb, int val)
{
	pSynth->setMixerLowCutFilterFrequency(fxb, val);
	SetModified();
}

void MixerSetHighcut(int fxb, int val)
{
	pSynth->setMixerHighCutFilterFrequency(fxb, val);
	SetModified();
}

void MixerSetInputgain(int fxb, int val)
{
	pSynth->setMixerInputGain(fxb, val);
	SetModified();
}

void MixerSetLevel(int fxb, int val)
{
	pSynth->setMixerOutputLevel(fxb, val);
	SetModified();
}

void MixerSetReverbsend(int fxb, int val)
{
	pSynth->setMixerReverbSend(fxb, val);
	SetModified();
}

void MixerSetPan(int fxb, int val)
{
	pSynth->setMixerPan(fxb, val);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa EQ
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void EqOn(int val)
{
	pSynth->setEQOn(val?true:false);
	SetModified();
}

void EqSetLowestBandGain(int val)
{
	pSynth->setEQLowestBandGain(val);
	SetModified();
}

void EqSetLowestBandFrequency(int val)
{
	pSynth->setEQLowestBandFrequency(val);
	SetModified();
}

void EqSetLowMidQ(int val)
{
	pSynth->setEQLowMidQ(val);
	SetModified();
}

void EqSetLowerBandGain(int val)
{
	pSynth->setEQLowerBandGain(val);
	SetModified();
}

void EqSetLowerBandFrequency(int val)
{
	pSynth->setEQLowerBandFrequency(val);
	SetModified();
}

void EqSetHigherBandGain(int val)
{
	pSynth->setEQHigherBandGain(val);
	SetModified();
}

void EqSetHigherBandFrequency(int val)
{
	pSynth->setEQHigherBandFrequency(val);
	SetModified();
}

void EqSetHighMidQ(int val)
{
	pSynth->setEQHighMidQ(val);
	SetModified();
}

void EqSetHighestBandGain(int val)
{
	pSynth->setEQHighestBandGain(val);
	SetModified();
}

void EqSetHighestBandFrequency(int val)
{
	pSynth->setEQHighestBandFrequency(val);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa assign effect
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthAssignEffect(unsigned char touch_control, unsigned char synth_param, unsigned char fxb,bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	if(touch_control <= VARIATION && synth_param)
	{
		char buf[64];
		pSynth->assignSynthEffect(touch_control, synth_param, fxb, inverse, variation_start, variation_end);
		//sprintf(buf,"assignSynthEffect(%d,%d,%d,%d,%d,%d);",touch_control, synth_param, fxb, inverse, variation_start, variation_end);
		//DEBUG_STDOUT(buf);
		SetModified();
	}
}

void SynthRemoveEffect(unsigned char synth_param, unsigned char fxb)
{
	if(synth_param)
	{
		pSynth->unAssignSynthEffect(synth_param, fxb);
		SetModified();
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
	SetModified();
}

void SynthSetMidiOutChannel(unsigned char str, signed char channel)
{
	pcs->turnCurrentNotesOff(0);
	pSynth->setMidiOutChannel(str, channel);
	SetModified();
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
		SetModified();
	}
}

void VoiceRemoveEffect(unsigned char synth_param, unsigned char str, unsigned char voice_index)
{
	if(synth_param)
	{
		pSynth->unAssignSynthVoiceParam(synth_param, str, voice_index);
		SetModified();
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
	SetModified();
}

void SynthRemoveVoice(int str, int voice_index)
{
	pSynth->removeVoice(str, voice_index);
	SetModified();
}

void SynthVoiceSetFX(int str, int voice_index, int fxb, bool state)
{
	pSynth->setFxBlockOn(str, voice_index, fxb, state);
	SetModified();
}

void SynthVoiceSetWave(int str, int voice_index, int program)
{
	pSynth->setWave(str, voice_index, program);
	SetModified();
}

void SynthVoiceAmpEnvAttack(int str, int voice_index, int val)
{
	pSynth->setAmpEnvAttack(str, voice_index, val);
	SetModified();
}

void SynthVoiceAmpEnvDecay(int str, int voice_index, int val)
{
	pSynth->setAmpEnvDecay(str, voice_index, val);
	SetModified();
}

void SynthVoiceAmpEnvRelease(int str, int voice_index, int val)
{
	pSynth->setAmpEnvRelease(str, voice_index, val);
	SetModified();
}

void SynthVoiceFilterFrequency(int str, int voice_index, int val)
{
	pSynth->setFilterFrequency(str, voice_index, val);
	SetModified();
}

void SynthVoiceFilterResonance(int str, int voice_index, int val)
{
	pSynth->setFilterResonance(str, voice_index, val);
	SetModified();
}

void SynthVoiceDetuneCourse(int str, int voice_index, int val)
{
	pSynth->setDetuneCourse(str, voice_index, val);
	SetModified();
}

void SynthVoiceDetuneFine(int str, int voice_index, int val)
{
	pSynth->setDetuneFine(str, voice_index, val);
	SetModified();
}

void SynthVoiceVibrateRate(int str, int voice_index, int val)
{
	pSynth->setVibrateRate(str, voice_index, val);
	SetModified();
}

void SynthVoiceVibrateDepth(int str, int voice_index, int val)
{
	pSynth->setVibrateDepth(str, voice_index, val);
	SetModified();
}

void SynthVoiceVibrateDelay(int str, int voice_index, int val)
{
	pSynth->setVibrateDelay(str, voice_index, val);
	SetModified();
}

void SynthVoiceChannelVolume(int str, int voice_index, int val)
{
	pSynth->setChannelVolume(str, voice_index, val);
	SetModified();
}

void SynthVoicePortamentoTime(int str, int voice_index, int val)
{
	pSynth->setPortamentoTime(str, voice_index, val);
	SetModified();
}

void SynthVoiceReverbSend(int str, int voice_index, int val)
{
	pSynth->setReverbSend(str, voice_index, val);
	SetModified();
}

void SynthVoicePan(int str, int voice_index, int val)
{
	pSynth->setPan(str, voice_index, val);
	SetModified();
}

void SynthVoicePitchBendSemitones(int str, int voice_index, int val)
{
	pSynth->setPitchBendSemitones(str, voice_index, val);
	SetModified();
}

void SynthVoicePitchWheel(int str, int voice_index, int val)
{
	pSynth->setPitchWheel(str, voice_index, val);
	SetModified();
}

void SynthVoiceVoiceVelocity(int str, int voice_index, int val)
{
	pSynth->setVoiceVelocity(str, voice_index, val);
	SetModified();
}

void SynthVoiceFilterType(int str, int voice_index, int val)
{
	pSynth->setFilterType(str, voice_index, val);
	SetModified();
}

void SynthVoiceFilterAttack(int str, int voice_index, int val)
{
	pSynth->setFilterAttack(str, voice_index, val);
	SetModified();
}

void SynthVoiceFilterDecay(int str, int voice_index, int val)
{
	pSynth->setFilterDecay(str, voice_index, val);
	SetModified();
}

void SynthVoiceFilterRelease(int str, int voice_index, int val)
{
	pSynth->setFilterRelease(str, voice_index, val);
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa MIDI control
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SynthAssignMidiPitch(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	pSynth->assignMidiPitch(touch_control, channel, inverse, variation_start, variation_end);
	SetModified();
}

void SynthUnassignMidiPitch(unsigned char touch_control, unsigned char channel)
{
	pSynth->unAssignMidiPitch(touch_control, channel);
	SetModified();
}

void SynthAssignMidiVelocity(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	pSynth->assignMidiVelocity(touch_control, channel, inverse, variation_start, variation_end);
	SetModified();
}

void SynthUnassignMidiVelocity(unsigned char touch_control, unsigned char str)
{
	pSynth->unAssignMidiVelocity(touch_control, str);
	SetModified();
}

void SynthAssignCC(unsigned char touch_control, unsigned char channel, unsigned int cc, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	pSynth->assignCC(touch_control, 1, channel, cc,inverse, variation_start, variation_end);
	SetModified();
}

void SynthUnassignCC(unsigned char touch_control, unsigned char channel, unsigned int cc)
{
	pSynth->unAssignCC(MIDI_OUT, touch_control, channel, cc);
	SetModified();
}

void SynthSendLearnCC(unsigned char channel, unsigned int cc)
{
	pSynth->sendLearnCC(channel, cc);
//	SetModified();
}

void SynthAssignStopSound(unsigned char str, unsigned char cc, unsigned char val)
{
	pSynth->assignMidiStopSound(str, cc, val);
	SetModified();
}

void SynthUnassignStopSound(unsigned char str, unsigned char cc)
{
	pSynth->unAssignMidiStopSound(str, cc);
	SetModified();
}

void SynthSendLearnStopSound(unsigned char str, unsigned char cc)
{
	pSynth->sendLearnStopSound(str, cc);
//	SetModified();
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
	SetModified();
}

void VoiceSetDragCentre(unsigned char synth_param, unsigned char str, unsigned char voice_index, unsigned int drag_center)
{
	pSynth->setDragCenterSynthVoiceParam(synth_param, str, voice_index, drag_center);
	SetModified();
}

void MidiPitchSetDragCentre(unsigned char channel, unsigned int drag_center)
{
	pSynth->setDragCenterMidiPitch(channel, drag_center);
	SetModified();
}

void MidiVelocitySetDragCentre(int str, unsigned int drag_center)
{
	pSynth->setDragCenterMidiVelocity(str, drag_center);
	SetModified();
}

void MidiCCSetDragCentre(unsigned char channel, unsigned int cc, unsigned int drag_center)
{
	pSynth->setDragCenterCC(MIDI_OUT, channel, cc, drag_center);
	SetModified();
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

int SynthGetBallmode()
{
	return pcs->isShowBall()?1:0;
}

void SynthSetBallmode(int val)
{
	pcs->showBall(val?true:false);
}

int SynthGetStringmode()
{
	return pcs->isShowStrings()?1:0;
}

void SynthSetStringmode(int val)
{
	pcs->showStrings(val?true:false);
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
	handmode = pcs->getLeftHandMode()?0:1;
	return handmode;//GetLcdFlip();
}

int MisaSetHandMode(int mode)
{
	handmode = mode;
	SetLcdFlip(mode);
	pcs->setLeftHandMode(MISA_LEFTHAND_MODE==handmode?true:false);
	return mode;
}

int MisaGetBallTravel(void)
{
	return pcs->getBallTravelOn()?1:0;
}

int MisaSetBallTravel(int mode)
{
	pcs->setBallTravelOn(mode?true:false);
	return 0;
}

int MisaGetEnableSustain()
{
	return pcs->isSustainEnabled()?1:0;
}

int MisaSetEnableSustain(int mode)
{
	pcs->setSustainEnabled(mode?true:false);
	return 0;
}

int MisaGetRingingNotes()
{
	return pcs->isRingingNotes()?1:0;
}

int MisaSetRingingNotes(int mode)
{
	pcs->setRingingNotes(mode?true:false);
	return 0;
}

const std::string &GetCurrentPresetName()
{
	return pSynth->getCurrentPresetName();
}

// OR : Drag & Drop
void SendVoiceParamToSynth(int string_index, int voice_index)
{
	pSynth->SendParamToSynth(string_index, voice_index);
	SetModified();
}

void SynthMuteVoice(int string_index, int voice_index, bool Mute)
{
	SynthTurnNotesOff();
	pSynth->SetMuteChannelForString(string_index, voice_index, Mute);
}

void SynthSoloVoice(int string_index, int voice_index, bool Solo)
{
	SynthTurnNotesOff();
	pSynth->SetSoloChannelForString(string_index, voice_index, Solo);
}

#else		// !Linux || MISA_APPLICATION

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
#include "BitcrusherScreen.h"
#include "DistortionScreen.h"
#include "ModulationScreen.h"
#include "DelayScreen.h"
#include "ReverbScreen.h"
#include "MixerScreen.h"
#include "EqScreen.h"

#include "MisamenuConfig.h"

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

using namespace std;

const int effect_compressor::KNEE_HARD = 0;
const int effect_compressor::KNEE_SOFT = 1;
//const int effect_wahwah::TYPE_LPF = 0;
//const int effect_wahwah::TYPE_BPF = 1;
const int effect_delay::MODE_MONO = 0;
const int effect_delay::MODE_STEREO = 1;

string CurrentPresetName;

// OR 28-06-11 : make it compile with Visual Studio 2008
std::string working_directory(".");		
std::vector<int> MidiStopSoundFind(unsigned char str)
{
	std::vector<int> vectInt;
	return vectInt;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa bitcrusher stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BitCrusherOn(int fxb, bool state)
{
	current_setting.fx_block[fxb].bitcrusher.on = state;
	SetModified();
}

void BitCrusherSetBitResolution(int fxb, int val)
{
	current_setting.fx_block[fxb].bitcrusher.bitresolution = val;
	SetModified();
}

//void BitCrusherSetBrightness(int fxb, int val)
//{
//	current_setting.fx_block[fxb].bitcrusher.brightness = val;
//	SetModified();
//}

void BitCrusherSetDownsampling(int fxb, int val)
{
	current_setting.fx_block[fxb].bitcrusher.downsampling = val;
	SetModified();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa distortion stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DistortionOn(int fxb, bool state)
{
	current_setting.fx_block[fxb].distortion.on = state;
	SetModified();
}

void DistortionSetType(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.type = val;
	SetModified();
}

void DistortionSetDrive(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.drive = val;
	SetModified();
}

void DistortionSetTone(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.tone = val;
	SetModified();
}

void DistortionSetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.level = val;
	SetModified();
}

void DistortionNoiseReduction(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.noise_reduction = val;
	SetModified();
}

void DistortionSetBooster(int fxb, int val)
{
	current_setting.fx_block[fxb].distortion.booster = val;
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa compressor stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CompressorSetOn(int fxb, bool state)
{
	current_setting.fx_block[fxb].compressor.on = state;
	SetModified();
}

void CompressorSetKnee(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.knee = val;
	SetModified();
}

void CompressorSetAttack(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.attack = val;
	SetModified();
}

void CompressorSetRelease(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.release = val;
	SetModified();
}

void CompressorSetThreshold(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.threshold = val;
	SetModified();
}

void CompressorSetRatio(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.ratio = val;
	SetModified();
}

void CompressorSetBoost(int fxb, int val)
{
	current_setting.fx_block[fxb].compressor.boost = val;
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa delay stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void DelaySetOn(int fxb, bool state)
{
	current_setting.fx_block[fxb].delay.on = state;
	SetModified();
}

void DelaySetMode(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.mode = val;
	SetModified();
}

void DelaySetPreLp(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.pre_lp = val;
	SetModified();
}

void DelaySetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.level = val;
	SetModified();
}

void DelaySetTime(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.time = val;
	SetModified();
}

void DelaySetFeedback(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.feedback = val;
	SetModified();
}

void DelaySetFeedbackFilter(int fxb, int val)
{
	current_setting.fx_block[fxb].delay.feedback_filter = val;
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa modulation stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ModulationSetOn(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.on = val?true:false;
	SetModified();
}

void ModulationSetPreset(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.type = val;
	SetModified();
}

void ModulationSetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.level = val;
	SetModified();
}

void ModulationSetChorusDelayTime(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.chorus_delay_time = val;
	SetModified();
}

void ModulationSetChorusFeedback(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.chorus_feedback = val;
	SetModified();
}

void ModulationSetChorusHpf(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.chorus_hpf = val;
	SetModified();
}

void ModulationSetDelayFeedbackFilter(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.delay_feedback_filter = val;
	SetModified();
}

void ModulationSetRate(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.rate = val;
	SetModified();
}

void ModulationSetDepth(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.depth = val;
	SetModified();
}

void ModulationSetTremolo(int fxb, int val)
{
	current_setting.fx_block[fxb].modulation.tremolo = val;
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa reverberation stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ReverbSetType(int val)
{
	current_setting.reverb.type = val;
	SetModified();
}

void ReverbSetCharacter(int val)
{
	current_setting.reverb.character = val;
	SetModified();
}

void ReverbSetPreLpf(int val)
{
	current_setting.reverb.pre_lpf = val;
	SetModified();
}

void ReverbSetLevel(int val)
{
	current_setting.reverb.level = val;
	SetModified();
}

void ReverbSetTime(int val)
{
	current_setting.reverb.time = val;
	SetModified();
}

void ReverbSetDelayFeedback(int val)
{
	current_setting.reverb.delay_feedback = val;
	SetModified();
}

void ReverbSetPreDelay(int val)
{
	current_setting.reverb.pre_delay_time = val;
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa mixer stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MixerSetLowcut(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.low_cut_filter_frequency = val;
	SetModified();
}

void MixerSetHighcut(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.high_cut_filter_frequency = val;
	SetModified();
}

void MixerSetInputgain(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.input_gain = val;
	SetModified();
}

void MixerSetLevel(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.output_level = val;
	SetModified();
}

void MixerSetReverbsend(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.reverb_send = val;
	SetModified();
}

void MixerSetPan(int fxb, int val)
{
	current_setting.fx_block[fxb].mixer.pan = val;
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa EQ stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void EqOn(int val)
{
	current_setting.equalizer.on = val?true:false;
	SetModified();
}

void EqSetLowestBandGain(int val)
{
	current_setting.equalizer.lowest.gain = val;
	SetModified();
}

void EqSetLowestBandFrequency(int val)
{
	current_setting.equalizer.lowest.frequency = val;
	SetModified();
}

void EqSetLowMidQ(int val)
{
	current_setting.equalizer.low_mid_q = val;
	SetModified();
}

void EqSetLowerBandGain(int val)
{
	current_setting.equalizer.lower.gain = val;
	SetModified();
}

void EqSetLowerBandFrequency(int val)
{
	current_setting.equalizer.lower.frequency = val;
	SetModified();
}

void EqSetHigherBandGain(int val)
{
	current_setting.equalizer.higher.gain = val;
	SetModified();
}

void EqSetHigherBandFrequency(int val)
{
	current_setting.equalizer.higher.frequency = val;
	SetModified();
}

void EqSetHighMidQ(int val)
{
	current_setting.equalizer.high_mid_q = val;
	SetModified();
}

void EqSetHighestBandGain(int val)
{
	current_setting.equalizer.highest.gain = val;
	SetModified();
}

void EqSetHighestBandFrequency(int val)
{
	current_setting.equalizer.highest.frequency = val;
	SetModified();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa assign effect stub
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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
	SetModified();
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
	SetModified();
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
	SetModified();
}

void SynthSetMidiOutChannel(unsigned char str, signed char channel)
{
	if(6>str)
	{
		current_setting.string_midi_out_channel[str]=channel;
	}
	SetModified();
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

	SetModified();
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
	SetModified();
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

	SetModified();
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

	SetModified();
}

void SynthVoiceSetFX(int str, int voice_index, int fxb, bool state)
{
	current_setting.voices[str].at(voice_index).insert_fx_block_on[fxb] = state;
	SetModified();
}

void SynthVoiceSetWave(int str, int voice_index, int program)
{
	current_setting.voices[str].at(voice_index).wavetable_index = program;
	SetModified();
}

void SynthVoiceAmpEnvAttack(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).amp_env_attack = val;
	SetModified();
}

void SynthVoiceAmpEnvDecay(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).amp_env_decay = val;
	SetModified();
}

void SynthVoiceAmpEnvRelease(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).amp_env_release = val;	
	SetModified();
}

void SynthVoiceFilterFrequency(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_frequency = val;
	SetModified();
}

void SynthVoiceFilterResonance(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_resonance = val;
	SetModified();
}

void SynthVoiceDetuneCourse(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).detune_course = val;
	SetModified();
}

void SynthVoiceDetuneFine(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).detune_fine = val;
	SetModified();
}

void SynthVoiceVibrateRate(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).vibrate_rate = val;
	SetModified();
}

void SynthVoiceVibrateDepth(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).vibrate_depth = val;
	SetModified();
}

void SynthVoiceVibrateDelay(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).vibrate_delay = val;
	SetModified();
}

void SynthVoiceChannelVolume(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).channel_volume = val;
	SetModified();
}

void SynthVoicePortamentoTime(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).portamento_time = val;
	SetModified();
}

void SynthVoiceReverbSend(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).reverb_send = val;
	SetModified();
}

void SynthVoicePan(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).pan = val;
	SetModified();
}

void SynthVoicePitchBendSemitones(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).pitch_bend_semitones = val;
	SetModified();
}

void SynthVoicePitchWheel(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).pitch_wheel = val;
	SetModified();
}

void SynthVoiceVoiceVelocity(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).velocity = val;
	SetModified();
}

void SynthVoiceFilterType(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_type = val;
	SetModified();
}

void SynthVoiceFilterAttack(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_attack = val;
	SetModified();
}

void SynthVoiceFilterDecay(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_decay = val;
	SetModified();
}

void SynthVoiceFilterRelease(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_release = val;
	SetModified();
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

	SetModified();
}

void SynthUnassignMidiPitch(unsigned char touch_control, unsigned char channel)
{
	//pSynth->unAssignMidiPitch(touch_control, channel);

	SetModified();
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

	SetModified();
}

void SynthUnassignMidiVelocity(unsigned char touch_control, unsigned char str)
{
	//pSynth->unAssignMidiVelocity(touch_control, str);
	SetModified();
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
	SetModified();
}

void SynthUnassignCC(unsigned char touch_control, unsigned char channel, unsigned int cc)
{
	//pSynth->unAssignCC(MIDI_OUT, touch_control, channel, cc);
	SetModified();
}

void SynthSendLearnCC(unsigned char channel, unsigned int cc)
{
}

void SynthAssignStopSound(unsigned char str, unsigned char cc, unsigned char val)
{
	SetModified();
}

void SynthUnassignStopSound(unsigned char str, unsigned char cc)
{
	SetModified();
}

void SynthSendLearnStopSound(unsigned char str, unsigned char cc)
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
	SetModified();
}

void VoiceSetDragCentre(unsigned char synth_param, unsigned char str, unsigned char voice_index, unsigned int drag_center)
{
	SetModified();
}

void MidiPitchSetDragCentre(unsigned char channel, unsigned int drag_center)
{
	SetModified();
}

void MidiVelocitySetDragCentre(int str, unsigned int drag_center)
{
	SetModified();
}

void MidiCCSetDragCentre(unsigned char channel, unsigned int cc, unsigned int drag_center)
{
	SetModified();
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
static int synthballmode;
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

int SynthGetBallmode()
{
	return synthballmode;
}

void SynthSetBallmode(int val)
{
	synthballmode = val;
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



int MisaGetGetEnableSustain()
{
	return 0;
}


int MisaGetEnableSustain()
{
	return 0;
}

int MisaSetEnableSustain(int mode)
{
	return 0;
}

int MisaGetRingingNotes()
{
	return 0;
}

int MisaSetRingingNotes(int mode)
{
	return 0;
}

int MisaGetShowStrings()
{
	return 0;
}

int MisaSetShowStrings(int mode)
{
	return 0;
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
		BitCrusherOn(i, false);
		BitCrusherSetBitResolution(i, 0);
		// BitCrusherSetBrightness(i, 0);
		BitCrusherSetDownsampling(i, 0);

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
	CurrentPresetName = "";
	ClearModified();
}

// [mz] we must try to avoid code duplication
void SynthLoadPreset(std::string filename)
{
	TiXmlDocument doc((working_directory + "/presets/" + filename +".mz").c_str());
	if(!doc.LoadFile())
	{
		std::cout << "Preset file could not be opened." << std::endl << std::flush;
		return;
	}

	ResetAllEffect();
	CurrentPresetName = filename;

	TiXmlElement *root = doc.RootElement();

	for(TiXmlElement *e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		std::string e_str = e->Value();

		if(e_str == "master")
		{
			if(e->Attribute("volume") != NULL)
				current_setting.master_volume = atoi(e->Attribute("volume"));
		}
		else
		if(e_str == "tuning")
		{
			if(e->Attribute("value"))
				current_setting.tuning[atoi(e->Attribute("string"))] = atoi(e->Attribute("value"));
		}
		else
		if(e_str == "midi_out_channel")
		{
			if(e->Attribute("value"))
				current_setting.string_midi_out_channel[atoi(e->Attribute("string"))] = atoi(e->Attribute("value"));
		}
		else
		if(e_str == "equalizer")
		{
			if(e->Attribute("on")) EqOn(atoi(e->Attribute("on")));
			if(e->Attribute("low_mid_q")) EqSetLowMidQ(atoi(e->Attribute("low_mid_q")));
			if(e->Attribute("high_mid_q")) EqSetHighMidQ(atoi(e->Attribute("high_mid_q")));

			for(TiXmlElement *e2 = e->FirstChildElement(); e2 != NULL; e2 = e2->NextSiblingElement())
			{
				if(std::string(e2->Value()) != "eq_band") continue;
				if(!e2->Attribute("type")) continue;

				if(std::string(e2->Attribute("type")) == "lowest")
				{
					if(e2->Attribute("gain")) EqSetLowestBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) EqSetLowestBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(std::string(e2->Attribute("type")) == "lower")
				{
					if(e2->Attribute("gain")) EqSetLowerBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) EqSetLowerBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(std::string(e2->Attribute("type")) == "higher")
				{
					if(e2->Attribute("gain")) EqSetHigherBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) EqSetHigherBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(std::string(e2->Attribute("type")) == "highest")
				{
					if(e2->Attribute("gain")) EqSetHighestBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) EqSetHighestBandFrequency(atoi(e2->Attribute("frequency")));
				}
			}
		}
		else
		if(e_str == "reverb")
		{
			if(e->Attribute("type")) ReverbSetType(atoi(e->Attribute("type")));
			if(e->Attribute("character")) ReverbSetCharacter(atoi(e->Attribute("character")));
			if(e->Attribute("pre_lpf")) ReverbSetPreLpf(atoi(e->Attribute("pre_lpf")));
			if(e->Attribute("level")) ReverbSetLevel(atoi(e->Attribute("level")));
			if(e->Attribute("time")) ReverbSetTime(atoi(e->Attribute("time")));
			if(e->Attribute("delay_feedback")) ReverbSetDelayFeedback(atoi(e->Attribute("delay_feedback")));
			if(e->Attribute("pre_delay_time")) ReverbSetPreDelay(atoi(e->Attribute("pre_delay_time")));
		}
		else
		if(e_str == "bitcrusher")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) BitCrusherOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("bitresolution")) BitCrusherSetBitResolution(fxb, atoi(e->Attribute("bitresolution")));
				// if(e->Attribute("brightness")) BitCrusherSetBrightness(fxb, atoi(e->Attribute("brightness")));
				if(e->Attribute("downsampling")) BitCrusherSetDownsampling(fxb, atoi(e->Attribute("downsampling")));
			}
		}
		else
		if(e_str == "distortion")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) DistortionOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("type")) DistortionSetType(fxb, atoi(e->Attribute("type")));
				if(e->Attribute("level")) DistortionSetLevel(fxb, atoi(e->Attribute("level")));
				if(e->Attribute("drive")) DistortionSetDrive(fxb, atoi(e->Attribute("drive")));
				if(e->Attribute("tone")) DistortionSetTone(fxb, atoi(e->Attribute("tone")));
				if(e->Attribute("booster")) DistortionSetBooster(fxb, atoi(e->Attribute("booster")));
			}
		}
		else
		if(e_str == "compression")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) CompressorSetOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("attack")) CompressorSetAttack(fxb, atoi(e->Attribute("attack")));
				if(e->Attribute("release")) CompressorSetRelease(fxb, atoi(e->Attribute("release")));
				if(e->Attribute("threshold")) CompressorSetThreshold(fxb, atoi(e->Attribute("threshold")));
				if(e->Attribute("ratio")) CompressorSetRatio(fxb, atoi(e->Attribute("ratio")));
				if(e->Attribute("boost")) CompressorSetBoost(fxb, atoi(e->Attribute("boost")));
				if(e->Attribute("knee")) CompressorSetKnee(fxb, atoi(e->Attribute("knee")));
			}
		}
		else
		if(e_str == "modulation")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("type")) ModulationSetPreset(fxb, atoi(e->Attribute("type")));
				if(e->Attribute("on")) ModulationSetOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("level")) ModulationSetLevel(fxb, atoi(e->Attribute("level")));
				if(e->Attribute("chorus_delay_time")) ModulationSetChorusDelayTime(fxb, atoi(e->Attribute("chorus_delay_time")));
				if(e->Attribute("chorus_feedback")) ModulationSetChorusFeedback(fxb, atoi(e->Attribute("chorus_feedback")));
				if(e->Attribute("chorus_hpf")) ModulationSetChorusHpf(fxb, atoi(e->Attribute("chorus_hpf")));
				if(e->Attribute("delay_feedback_filter")) ModulationSetDelayFeedbackFilter(fxb, atoi(e->Attribute("delay_feedback_filter")));
				if(e->Attribute("rate")) ModulationSetRate(fxb, atoi(e->Attribute("rate")));
				if(e->Attribute("depth")) ModulationSetDepth(fxb, atoi(e->Attribute("depth")));
				if(e->Attribute("tremolo")) ModulationSetTremolo(fxb, atoi(e->Attribute("tremolo")));
			}
		}
		else
		if(e_str == "delay")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) DelaySetOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("mode")) DelaySetMode(fxb, atoi(e->Attribute("mode")));
				if(e->Attribute("pre_lp")) DelaySetPreLp(fxb, atoi(e->Attribute("pre_lp")));
				if(e->Attribute("level")) DelaySetLevel(fxb, atoi(e->Attribute("level")));
				if(e->Attribute("time")) DelaySetTime(fxb, atoi(e->Attribute("time")));
				if(e->Attribute("feedback")) DelaySetFeedback(fxb, atoi(e->Attribute("feedback")));
				if(e->Attribute("feedback_filter")) DelaySetFeedbackFilter(fxb, atoi(e->Attribute("feedback_filter")));
			}
		}
		else
		if(e_str == "mixer")
		{
			int fxb = 0;
			if(e->Attribute("fxblock")) 
			{
				int fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("low_cut_filter_frequency")) MixerSetLowcut(fxb, atoi(e->Attribute("low_cut_filter_frequency")));
				if(e->Attribute("high_cut_filter_frequency")) MixerSetHighcut(fxb, atoi(e->Attribute("high_cut_filter_frequency")));
				if(e->Attribute("input_gain")) MixerSetInputgain(fxb, atoi(e->Attribute("input_gain")));
				if(e->Attribute("output_level")) MixerSetLevel(fxb, atoi(e->Attribute("output_level")));
				if(e->Attribute("pan")) MixerSetPan(fxb, atoi(e->Attribute("pan")));
				if(e->Attribute("reverb_send")) MixerSetReverbsend(fxb, atoi(e->Attribute("reverb_send")));
			}
		}
		else
		if(e_str == "control")
		{
			if(atoi(e->Attribute("type")) == 100) //fix: this nasty hack immediately!
			{
				if((e->Attribute("string")) && (e->Attribute("cc")) && (e->Attribute("value")))
					SynthAssignStopSound(atoi(e->Attribute("string")), atoi(e->Attribute("cc")), atoi(e->Attribute("value")));
			}
			else
			{
				std::vector<struct assignable_effect> *control = SynthGetControlList(atoi(e->Attribute("type")));
				struct assignable_effect ae;
				if(e->Attribute("name")) ae.name = atoi(e->Attribute("name"));
				if(e->Attribute("string")) ae.str = atoi(e->Attribute("string"));
				if(e->Attribute("index")) ae.voice_index = atoi(e->Attribute("index"));
				if(e->Attribute("output")) ae.output = atoi(e->Attribute("output"));
				if(e->Attribute("channel")) ae.channel = atoi(e->Attribute("channel"));
				if(e->Attribute("cc")) ae.cc = atoi(e->Attribute("cc"));
				if(e->Attribute("inverse")) ae.inverse = atoi(e->Attribute("inverse"));
				if(e->Attribute("variation_start")) ae.variation_start = atoi(e->Attribute("variation_start"));
				if(e->Attribute("variation_end")) ae.variation_end = atoi(e->Attribute("variation_end"));
				if(e->Attribute("fxb")) ae.fxb = atoi(e->Attribute("fxb"));
				if(e->Attribute("drag_center")) ae.drag_center = atoi(e->Attribute("drag_center"));
				control->push_back(ae);
			}
		}
		else
		if(e_str == "voice")
		{
			if((e->Attribute("string")) && (e->Attribute("channel")) && (e->Attribute("wavetable_index")))
			{
				int i = atoi(e->Attribute("string"));
				int j = current_setting.voices[i].size();
				SynthInsertVoice(i, j);

				
				current_setting.voices[i].at(j).channel = atoi(e->Attribute("channel"));	// setChannel(i, j, atoi(e->Attribute("channel")));

				SynthVoiceSetWave(i, j, atoi(e->Attribute("wavetable_index")));
				if(e->Attribute("amplitude_attack")) SynthVoiceAmpEnvAttack(i, j, atoi(e->Attribute("amplitude_attack")));
				if(e->Attribute("amplitude_decay")) SynthVoiceAmpEnvDecay(i, j, atoi(e->Attribute("amplitude_decay")));
				if(e->Attribute("amplitude_release")) SynthVoiceAmpEnvRelease(i, j, atoi(e->Attribute("amplitude_release")));
				if(e->Attribute("filter_frequency")) SynthVoiceFilterFrequency(i, j, atoi(e->Attribute("filter_frequency")));
				if(e->Attribute("filter_resonance")) SynthVoiceFilterResonance(i, j, atoi(e->Attribute("filter_resonance")));
				if(e->Attribute("detune_course")) SynthVoiceDetuneCourse(i, j, atoi(e->Attribute("detune_course")));
				if(e->Attribute("detune_fine")) SynthVoiceDetuneFine(i, j, atoi(e->Attribute("detune_fine")));
				if(e->Attribute("vibrate_rate")) SynthVoiceVibrateRate(i, j, atoi(e->Attribute("vibrate_rate")));
				if(e->Attribute("vibrate_depth")) SynthVoiceVibrateDepth(i, j, atoi(e->Attribute("vibrate_depth")));
				if(e->Attribute("vibrate_delay")) SynthVoiceVibrateDelay(i, j, atoi(e->Attribute("vibrate_delay")));
				if(e->Attribute("channel_volume")) SynthVoiceChannelVolume(i, j, atoi(e->Attribute("channel_volume")));
				if(e->Attribute("portamento_time")) SynthVoicePortamentoTime(i, j, atoi(e->Attribute("portamento_time")));
				if(e->Attribute("pan")) SynthVoicePan(i, j, atoi(e->Attribute("pan")));
				if(e->Attribute("pitch_bend_semitones")) SynthVoicePitchBendSemitones(i, j, atoi(e->Attribute("pitch_bend_semitones")));
				if(e->Attribute("pitch_wheel")) SynthVoicePitchWheel(i, j, atoi(e->Attribute("pitch_wheel")));
				if(e->Attribute("velocity")) SynthVoiceVoiceVelocity(i, j, atoi(e->Attribute("velocity")));
				if(e->Attribute("reverb_send")) SynthVoiceReverbSend(i, j, atoi(e->Attribute("reverb_send")));
				if(e->Attribute("filter_type")) SynthVoiceFilterType(i, j, atoi(e->Attribute("filter_type")));
				if(e->Attribute("filter_attack")) SynthVoiceFilterAttack(i, j, atoi(e->Attribute("filter_attack")));
				if(e->Attribute("filter_decay")) SynthVoiceFilterDecay(i, j, atoi(e->Attribute("filter_decay")));
				if(e->Attribute("filter_release")) SynthVoiceFilterRelease(i, j, atoi(e->Attribute("filter_release")));

				if((e->Attribute("fxb0_on")) && (atoi(e->Attribute("fxb0_on"))))
					SynthVoiceSetFX(i, j, 0, true);
				else
				if((e->Attribute("fxb1_on")) && (atoi(e->Attribute("fxb1_on"))))
					SynthVoiceSetFX(i, j, 1, true);
			}
		}

	}
	ClearModified();
}

void SynthSavePreset(PSYNTH_SETTING pSetting,std::string filename)
{
	std::string filepath = working_directory + "/presets/" + filename +".mz";

	HANDLE h = CreateFile(filepath.c_str(), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (h)
		CloseHandle(h);

	CurrentPresetName = filename;
	ClearModified();
}

bool GetCurrentSetting(PSYNTH_SETTING pSetting)
{
	*pSetting = current_setting;
	return true;
}

const string &GetCurrentPresetName()
{
	return CurrentPresetName;
}

// OR : Drag & Drop
PSYNTH_SETTING GetCurrentSettingPtr()
{
	return &current_setting;
}
void SendVoiceParamToSynth(int string_index, int voice_index)
{
	// Do nothing in the Windows simulation
	SetModified();
}

void SynthMuteVoice(int string_index, int voice_index, bool Mute)
{
	// Do nothing in the Windows simulation
}

void SynthSoloVoice(int string_index, int voice_index, bool Solo)
{
	// Do nothing in the Windows simulation
}

void SaveToConfigFile(void)
{
}

#endif // !Linux || MISA_APPLICATION

//***********************************************************************
//
//	Common Code (Linux & Windows Simulation)
//
//***********************************************************************

void SetWindowToUpdateIfPresetModified(WM_HWIN hWin)
{
	hWinToInvalidate = hWin;
}

void LockModified()
{
	lockModified = true;
}

void UnlockModified()
{
	lockModified = false;
}

static void SetModified()
{
	if (lockModified)
		return;

	if (PresetModified == false && hWinToInvalidate)
		WM_InvalidateRect(hWinToInvalidate, &RectToInvalidate);

	PresetModified = true;
}

void ClearModified()
{
	//if (PresetModified == true)
	//	WM_Invalidate();

	PresetModified = false;
}

static bool IsModified()
{
	return PresetModified;
}

std::string &GetTitleWithPreset(std::string &TitleBase)
{
	if (!GetCurrentPresetName().empty())
	{
		TitleBase += " (";
		if (IsModified())
			TitleBase+= "*";
		TitleBase += GetCurrentPresetName();
		TitleBase += ")";
	}
	
	return TitleBase;
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa assign effect Tools
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

	case BITCRUSHER_ID_BITRESOLUTION:
		ret = PARAM_BITCRUSHER_RESOLUTION;
		break;

	//case BITCRUSHER_ID_BRIGHTNESS:
	//	ret = PARAM_BITCRUSHER_BRIGHTNESS;
	//	break;

	case BITCRUSHER_ID_DOWNSAMPLINGFACTOR:
		ret = PARAM_BITCRUSHER_DOWNSAMPLING;
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


// end of common code (Linux & Windows Simulation)

