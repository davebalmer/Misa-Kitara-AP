/*


PRESET BUILDER CODE - see Synth::run()


*/
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "Synth.h"
#include <string>
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

Synth::Synth()
{
	resetSettings();
	srand(time(NULL));
}

Synth::~Synth()
{
//	delete presets and voices here
	resetSettings();
}

void Synth::resetSettings(void)
{
	current_setting.tuning[0] = 64-12;
	current_setting.tuning[1] = 59-12;
	current_setting.tuning[2] = 55-12;
	current_setting.tuning[3] = 50-12;
	current_setting.tuning[4] = 45-12;
	current_setting.tuning[5] = 40-12;

	//delete any previously loaded voices
	for(int s = 0; s < 6; s++)
	{
		current_setting.voices[s].clear();
		current_setting.string_midi_out_channel[s] = -1;
		current_setting.stop_sound_cmds[s].clear();
	}

	memset(&current_setting.equalizer,0,sizeof(struct eq));
	memset(&current_setting.reverb,0,sizeof(struct effect_reverb));
	memset(&current_setting.fx_block[0],0,sizeof(struct fxb));
	memset(&current_setting.fx_block[1],0,sizeof(struct fxb));

	current_setting.AE_touch_x.clear();
	current_setting.AE_touch_y.clear();
	current_setting.AE_ball_x.clear();
	current_setting.AE_ball_y.clear();
	current_setting.AE_drag_x.clear();
	current_setting.AE_drag_y.clear();
	current_setting.AE_string_touch_x_0.clear();
	current_setting.AE_string_touch_x_1.clear();
	current_setting.AE_string_touch_x_2.clear();
	current_setting.AE_string_touch_x_3.clear();
	current_setting.AE_string_touch_x_4.clear();
	current_setting.AE_string_touch_x_5.clear();
	current_setting.AE_string_drag_x_0.clear();
	current_setting.AE_string_drag_x_1.clear();
	current_setting.AE_string_drag_x_2.clear();
	current_setting.AE_string_drag_x_3.clear();
	current_setting.AE_string_drag_x_4.clear();
	current_setting.AE_string_drag_x_5.clear();
	current_setting.AE_variation.clear();
	
	//reset all effects
//	midi.sendNRPN(SYNTH, 0, 0x3A, 0x7F, 0); //fx block 1 //fix: should val be 0x7F?
//	midi.sendNRPN(SYNTH, 0, 0x3B, 0x7F, 0); //fx block 2
//	midi.sendGSReset();

	//HD algorithms need to be reset manually
	setDistortionOn(0, false);
	setDistortionOn(1, false);

	for(int i = 0; i < 32; i++)
	{
//		midi.sendCC(SYNTH, i, 121, 0);	//reset all controllers
//		midi.sendCC(SYNTH, i, 126, 0);	//channel mono on = 0
//		midi.sendCC(SYNTH, i, 65, 0x7F);	//channel portamento on = 0x7F

		//unassign effects inserts
//		int chan = midi.setMidiPort(i);
//		unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x40+chan, 0x22, 0x00, 0x00, 0xF7};
//		midi.sendSysex(SYNTH, sysex, 11);
	}

	for(int i = 0; i < 6; i++)
		velocity[i] = 127;

	//set all the default values internally
	setMasterVolume(0x7F);
//	setLfoRate(0xFF);
//	setLfoFrequencyDepth(0xFF);
//	setLfoAmpDepth(0xFF);
//	setLfoPitchDepth(0xFF);
	setEQOn(false);
	setEQLowestBandGain(0x60);
	setEQLowerBandGain(0x38);
	setEQHigherBandGain(0x38);
	setEQHighestBandGain(0x60);
	setEQLowestBandFrequency(0x0A);
	setEQLowerBandFrequency(0x18);
	setEQHigherBandFrequency(0x68);
	setEQHighestBandFrequency(0x7F);
	setEQLowMidQ(0x40);
	setEQHighMidQ(0x40);
	setReverbType(0x04);
	setReverbCharacter(0x04);
	setReverbPreLpf(0);
	setReverbLevel(0x40);
	setReverbTime(0);
	setReverbDelayFeedback(0);
	setReverbPreDelay(0);
	for(int i = 0; i < 2; i++)
	{
		setDistortionOn(i, false);
		setDistortionType(i, 0);
		setDistortionLevel(i, 0);
		setDistortionDrive(i, 0);
		setDistortionTone(i, 0);

		setCompressorOn(i, false);
		setCompressorAttack(i, 0);
		setCompressorRelease(i, 0);
		setCompressorThreshold(i, 0);
		setCompressorRatio(i, 0);
		setCompressorBoost(i, 0);
		setCompressorKnee(i, 0);

		setModulationOn(i, 0);
		setModulationType(i, 1);
		setModulationLevel(i, 56);
		setModulationChorusDelayTime(i, 20);
		setModulationChorusFeedback(i, 0);
		setModulationChorusHpf(i, 0);
		setModulationDelayFeedbackFilter(i, 0);
		setModulationRate(i, 40);
		setModulationDepth(i, 30);
		setModulationTremolo(i, 0);

		setDelayOn(i, false);
		setDelayMode(i, 0);
		setDelayPreLp(i, 0);
		setDelayLevel(i, 0);
		setDelayTime(i, 0);
		setDelayFeedback(i, 0);
		setDelayFeedbackFilter(i, 0);

		setMixerLowCutFilterFrequency(i, 0x7F); //fix: datasheet says opposite
		setMixerHighCutFilterFrequency(i, 0x7F);
		setMixerInputGain(i, 0x7F);
		setMixerOutputLevel(i, 0x7F);
		setMixerPan(i, 0x40);
		setMixerReverbSend(i, 0);
	}
}

void Synth::run(void)
{
		loadMeltGuitar();
		savePresetToFile(&current_setting, "./presets/meltguitar.mz");
//		loadTremstar();
//		savePresetToFile(&current_setting, "./presets/tremstar.mz");
//		loadRemstar();
//		savePresetToFile(&current_setting, "./presets/remstar.mz");
		loadSouthpad();
		savePresetToFile(&current_setting, "./presets/southpad.mz");
//		loadPhaseGuitar();
//		savePresetToFile(&current_setting, "./presets/phaseguitar.mz");
//		loadSquareLead();
//		savePresetToFile(&current_setting, "./presets/squarelead.mz");
		loadWarmBass();
		savePresetToFile(&current_setting, "./presets/warmbass.mz");
//		loadWarmBassStd();
//		savePresetToFile(&current_setting, "./presets/warmbassstd.mz");
//		loadSparklevar();
//		savePresetToFile(&current_setting, "./presets/sparklevar.mz");
//		loadExplode();
//		savePresetToFile(&current_setting, "./presets/explode.mz");
//		loadPadSustain();
//		savePresetToFile(&current_setting, "./presets/padsustain.mz");
		loadSineGuitar();
		savePresetToFile(&current_setting, "./presets/sineguitar.mz");
//		loadSineWave();
//		savePresetToFile(&current_setting, "./presets/sinewave.mz");
//		loadSineGuitarStd();
//		savePresetToFile(&current_setting, "./presets/sineguitarstd.mz");
//		loadEightyFour();
//		savePresetToFile(&current_setting, "./presets/eightyfour.mz");
		loadXenoxJeff();
		savePresetToFile(&current_setting, "./presets/xenoxjeff.mz");
		loadSineDefault();
		savePresetToFile(&current_setting, "./presets/default.mz");
/*		savePresetToFile(&current_setting, "./presets/user01.mz");
		savePresetToFile(&current_setting, "./presets/user02.mz");
		savePresetToFile(&current_setting, "./presets/user03.mz");
		savePresetToFile(&current_setting, "./presets/user04.mz");
		savePresetToFile(&current_setting, "./presets/user05.mz");
		savePresetToFile(&current_setting, "./presets/user06.mz");
		savePresetToFile(&current_setting, "./presets/user07.mz");
		savePresetToFile(&current_setting, "./presets/user08.mz");
		savePresetToFile(&current_setting, "./presets/user09.mz");
		savePresetToFile(&current_setting, "./presets/user10.mz");
*/
		system("rm ../presets.tar.gz");
		system("tar zcvf ../presets.tar.gz presets");
		std::cout << "Done." << std::endl << std::flush;
}

void Synth::loadSouthpad(void)
{
	resetSettings();
//	setMasterVolume(100);
	current_setting.tuning[0] = 64+12;
	current_setting.tuning[1] = 59+12;
	current_setting.tuning[2] = 55+12;
	current_setting.tuning[3] = 50+12;
	current_setting.tuning[4] = 45+12;
	current_setting.tuning[5] = 40+12;
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 13);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 59);
		setPitchBendSemitones(s, 0, 12);
		setPortamentoTime(s, 0, 30);
//		setReverbSend(s, 0, 127);
		setFilterType(s, 0, 5);
		//assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(DRAG_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 50);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_PAN, s, 0, false, 64-10, 64+10);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 0);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

//	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
	assignSynthEffect(BALL_X, PARAM_DELAY_LEVEL, 0, true, 0, 0);

	setDelayOn(0, true);
	setDelayMode(0, 1);
	setDelayLevel(0, 82);
	setDelayPreLp(0, 55);
	setDelayTime(0, 66);
	setDelayFeedback(0, 8);
	setDelayFeedbackFilter(0, 68);

//	setDistortionOn(0, true);
//	setDistortionType(0, 4);
//	setDistortionDrive(0, 120);
//	setDistortionLevel(0, 44);
//	setDistortionTone(0, 70);

	//notice this is incomplete
//	setModulationType(0, 8);
//	setModulationLevel(0, 64);
}

void Synth::loadMeltGuitar(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 59);
		setPitchBendSemitones(s, 0, 2);
//		setReverbSend(s, 0, 127);
		setAmpEnvDecay(s, 0, 127);
		setAmpEnvRelease(s, 0, 33);
		setFilterType(s, 0, 5);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_VELOCITY, s, 0, false, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 55);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 55);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	setMixerOutputLevel(0, 75);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 4);
	setDistortionDrive(0, 116);
	setDistortionLevel(0, 38);
	setDistortionTone(0, 70);
	setDistortionNoiseReduction(0, 0);
	setDistortionBooster(0, 0);

	//notice this is incomplete
	setModulationOn(0, true);
	setModulationType(0, 9);
	setModulationLevel(0, 64);
}

/*
void Synth::loadMeltGuitarOriginal(void) //control decay and release from TOUCH_Y
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 59);
		setPitchBendSemitones(s, 0, 2);
//		setReverbSend(s, 0, 127);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 55);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 55);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	setMixerOutputLevel(0, 75);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 4);
	setDistortionDrive(0, 116);
	setDistortionLevel(0, 38);
	setDistortionTone(0, 70);
	setDistortionNoiseReduction(0, 0);
	setDistortionBooster(0, 0);

	//notice this is incomplete
	setModulationOn(0, true);
	setModulationType(0, 9);
	setModulationLevel(0, 64);
}
*/

void Synth::loadSineDefault(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
	}
}

void Synth::loadRemstar(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 59);
		setPitchBendSemitones(s, 0, 2);
//		setReverbSend(s, 0, 127);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 75);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 75);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 0);
		setFxBlockOn(s, 1, 1, true);
//		setDetuneCourse(s, 1, 59);
		setPitchBendSemitones(s, 1, 2);
//		setReverbSend(s, 1, 127);
		setAmpEnvAttack(s, 1, 64);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 1, true, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 1, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 1, true, 0, 0);
	}

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 1, true, 0, 0);
	assignSynthEffect(TOUCH_Y, PARAM_MODULATION_RATE, 1, true, 0, 0);

	setMixerOutputLevel(0, 50);
	setMixerLowCutFilterFrequency(0, 0);
	setMixerOutputLevel(1, 50);
	setMixerLowCutFilterFrequency(1, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 4);
	setDistortionDrive(0, 120);
	setDistortionLevel(0, 44);
	setDistortionTone(0, 70);

	setDistortionOn(1, true);
	setDistortionType(1, 5);
	setDistortionDrive(1, 120);
	setDistortionLevel(1, 44);
	setDistortionTone(1, 70);

	//notice this is incomplete
	setModulationType(1, 22);
	setModulationOn(1, true);
	setModulationLevel(1, 84);
}

void Synth::loadTremstar(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
		setDetuneCourse(s, 0, 59);
//		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 50);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 50);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 0);
		setFxBlockOn(s, 1, 1, true);
		setDetuneCourse(s, 1, 59);
//		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
		setAmpEnvAttack(s, 1, 64);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 1, true, 0, 0);

	setMixerOutputLevel(0, 50);
	setMixerLowCutFilterFrequency(0, 0);
	setMixerOutputLevel(1, 50);
	setMixerLowCutFilterFrequency(1, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 4);
	setDistortionDrive(0, 120);
	setDistortionLevel(0, 44);
	setDistortionTone(0, 70);

	setDistortionOn(1, true);
	setDistortionType(1, 5);
	setDistortionDrive(1, 120);
	setDistortionLevel(1, 44);
	setDistortionTone(1, 70);

	//notice this is incomplete
	setModulationType(1, 22);
	setModulationOn(1, true);
	setModulationLevel(1, 84);
}

void Synth::loadWarmBass(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 59);
		setPitchBendSemitones(s, 0, 2);
//		setReverbSend(s, 0, 127);
		setAmpEnvDecay(s, 0, 127);
		setAmpEnvRelease(s, 0, 82);
		setFilterType(s, 0, 5);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_VELOCITY, s, 0, false, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
//		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 75);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 75);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 18);
		setFxBlockOn(s, 1, 0, true);
//		setDetuneCourse(s, 1, 59);
		setPitchBendSemitones(s, 1, 2);
//		setReverbSend(s, 1, 127);
		setAmpEnvAttack(s, 1, 64);
		setAmpEnvDecay(s, 1, 127);
		setAmpEnvRelease(s, 1, 82);
		setFilterType(s, 1, 5);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 1, true, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 1, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 1, true, 0, 0);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 0);
	setDistortionDrive(0, 127);
	setDistortionLevel(0, 100);
	setDistortionTone(0, 0);

	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);

	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);
}

void Synth::loadWarmBassStd(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
		setDetuneCourse(s, 0, 64+12);
//		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 75);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 75);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 18);
		setFxBlockOn(s, 1, 0, true);
		setDetuneCourse(s, 1, 64+12);
//		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
		setAmpEnvAttack(s, 1, 64);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 0);
	setDistortionDrive(0, 127);
	setDistortionLevel(0, 100);
	setDistortionTone(0, 0);

	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);

	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);
}

void Synth::loadEightyFour(void)
{
	resetSettings();
//	setMasterVolume(127);

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 2);
		setFxBlockOn(s, 0, 0, true);
		setDetuneCourse(s, 0, 64-12);
//		setDetuneFine(s, 0, 67);
//		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		setPortamentoTime(s, 0, 26);
		setAmpEnvAttack(s, 0, 0);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 50);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 2);
		setFxBlockOn(s, 1, 0, true);
		setDetuneCourse(s, 1, 64);
//		setDetuneFine(s, 1, 61);
//		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
		setPortamentoTime(s, 1, 24);
		setAmpEnvAttack(s, 1, 0);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 2);
		setFxBlockOn(s, 2, 0, true);
		setDetuneCourse(s, 2, 64+24);
//		setDetuneFine(s, 2, 60);
//		setPitchBendSemitones(s, 2, 12);
//		setReverbSend(s, 2, 127);
		setPortamentoTime(s, 2, 25);
		setAmpEnvAttack(s, 2, 0);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 2, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 2, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 2, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 2, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 2, false, 0, 50);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	setMixerOutputLevel(0, 85);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 3);
	setDistortionDrive(0, 73);
	setDistortionLevel(0, 45);
	setDistortionTone(0, 0);

/*	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);
*/
	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);
/*
	setDelayOn(0, true);
	setDelayMode(0, 0);
	setDelayLevel(0, 28);
	setDelayTime(0, 60);
	setDelayFeedback(0, 50);
	setDelayFeedbackFilter(0, 30);
*/
}

void Synth::loadSineGuitar(void)
{
	resetSettings();
//	setMasterVolume(127);

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 64);
		setDetuneFine(s, 0, 67);
		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		setPortamentoTime(s, 0, 6);
		setAmpEnvAttack(s, 0, 0);
		setAmpEnvDecay(s, 0, 127);
		setAmpEnvRelease(s, 0, 33);
		setPitchWheel(s, 0, 0x2000);
		setFilterType(s, 0, 5);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_VELOCITY, s, 0, false, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);

		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 50);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 1, 0, true);
//		setDetuneCourse(s, 1, 64);
		setDetuneFine(s, 1, 61);
		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
		setPortamentoTime(s, 1, 4);
		setAmpEnvAttack(s, 1, 0);
		setAmpEnvDecay(s, 1, 127);
		setAmpEnvRelease(s, 1, 33);
		setPitchWheel(s, 1, 0x2000);
		setFilterType(s, 1, 5);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 1, true, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
//		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthPitch(control, s, 1, false, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 1, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 1, true, 0, 0);

		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 2, 0, true);
//		setDetuneCourse(s, 2, 64+12);
//		setDetuneFine(s, 2, 60);
		setPitchBendSemitones(s, 2, 12);
//		setReverbSend(s, 2, 127);
		setPortamentoTime(s, 2, 5);
		setAmpEnvAttack(s, 2, 0);
		setAmpEnvDecay(s, 2, 127);
		setAmpEnvRelease(s, 2, 33);
		setFilterType(s, 2, 5);
		setPitchWheel(s, 2, 0x2000);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 2, true, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 2, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 2, false, 0, 0);
//		assignSynthPitch(DRAG_X, s, 2, false, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 2, false, 0, 0);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthPitch(control, s, 2, false, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 2, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 2, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 2, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 2, true, 0, 0);

		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 2, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 2, false, 0, 50);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
	assignSynthEffect(TOUCH_X, PARAM_DISTORTION_LEVEL, 0, true, 0, 0);

	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 5);
	setDistortionDrive(0, 127);
	setDistortionLevel(0, 95);
	setDistortionTone(0, 111);
	setDistortionNoiseReduction(0, 0);
	setDistortionBooster(0, 0);

/*	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);
*/
	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);

	setDelayOn(0, true);
	setDelayMode(0, 0);
	setDelayLevel(0, 28);
	setDelayTime(0, 60);
	setDelayFeedback(0, 50);
	setDelayFeedbackFilter(0, 30);
}

void Synth::loadSineWave(void)
{
	resetSettings();
//	setMasterVolume(127);

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
//		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 64);
//		setDetuneFine(s, 0, 67);
//		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
//		setPortamentoTime(s, 0, 6);
		setAmpEnvAttack(s, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_X, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);

//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 50);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 50);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

//	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
//	assignSynthEffect(TOUCH_X, PARAM_DISTORTION_LEVEL, 0, true, 0, 0);

//	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
//	setDistortionOn(0, true);
//	setDistortionType(0, 5);
//	setDistortionDrive(0, 127);
//	setDistortionLevel(0, 95);
//	setDistortionTone(0, 111);
//	setDistortionNoiseReduction(0, 0);
//	setDistortionBooster(0, 0);

/*	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);
*/
	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);

//	setDelayOn(0, true);
//	setDelayMode(0, 0);
//	setDelayLevel(0, 28);
//	setDelayTime(0, 60);
//	setDelayFeedback(0, 50);
//	setDelayFeedbackFilter(0, 30);
}

void Synth::loadSineGuitarStd(void)
{
	resetSettings();
//	setMasterVolume(127);

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 0, 0, true);
		setDetuneCourse(s, 0, 64+12);
		setDetuneFine(s, 0, 67);
		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		setPortamentoTime(s, 0, 6);
		setAmpEnvAttack(s, 0, 0);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 50);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 1, 0, true);
		setDetuneCourse(s, 1, 64+12);
		setDetuneFine(s, 1, 61);
		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
		setPortamentoTime(s, 1, 4);
		setAmpEnvAttack(s, 1, 0);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 2, 0, true);
		setDetuneCourse(s, 2, 64+24);
//		setDetuneFine(s, 2, 60);
		setPitchBendSemitones(s, 2, 12);
//		setReverbSend(s, 2, 127);
		setPortamentoTime(s, 2, 5);
		setAmpEnvAttack(s, 2, 0);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 2, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 2, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 2, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 2, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 2, false, 0, 50);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
	assignSynthEffect(TOUCH_X, PARAM_DISTORTION_LEVEL, 0, true, 0, 0);

	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 5);
	setDistortionDrive(0, 127);
	setDistortionLevel(0, 95);
	setDistortionTone(0, 111);
	setDistortionNoiseReduction(0, 0);
	setDistortionBooster(0, 0);

/*	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);
*/
	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);

	setDelayOn(0, true);
	setDelayMode(0, 0);
	setDelayLevel(0, 28);
	setDelayTime(0, 60);
	setDelayFeedback(0, 50);
	setDelayFeedbackFilter(0, 30);
}

void Synth::loadXenoxJeff(void)
{
	resetSettings();
//	setMasterVolume(127);

	current_setting.tuning[0] = 64;
	current_setting.tuning[1] = 59;
	current_setting.tuning[2] = 55;
	current_setting.tuning[3] = 50;
	current_setting.tuning[4] = 45-12;
	current_setting.tuning[5] = 40-12;

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 4);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 64+12);
//		setDetuneFine(s, 0, 67);
		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		setPortamentoTime(s, 0, 20);
		setAmpEnvAttack(s, 0, 0);
		setAmpEnvDecay(s, 0, 75);
		setAmpEnvRelease(s, 0, 62);
		setFilterType(s, 0, 1);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_VELOCITY, s, 0, false, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthVoiceParam(BALL_X, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 0);
		assignSynthVoiceParam(BALL_Y, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 0);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 0, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 0, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 0, true, 0, 0);
	}

	for(int s = 4; s < 6; s++)
	{
		insertNewVoice(s, 14);
		setFxBlockOn(s, 1, 0, true);
//		setDetuneCourse(s, 1, 64+12);
//		setDetuneFine(s, 1, 61);
		setPitchBendSemitones(s, 1, 0);
//		setReverbSend(s, 1, 127);
		setPortamentoTime(s, 1, 20);
		setAmpEnvAttack(s, 1, 0);
		setAmpEnvDecay(s, 1, 75);
		setAmpEnvRelease(s, 1, 62);
		setChannelVolume(s, 1, 127);
		setFilterType(s, 1, 1);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_VELOCITY, s, 1, false, 0, 0);
		assignSynthVoiceParam(DRAG_X, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthVoiceParam(BALL_X, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 0);
		assignSynthVoiceParam(BALL_Y, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 0);

		int control;
		switch(s)
		{
			case 0: control = STRING_DRAG_X_0; break;
			case 1: control = STRING_DRAG_X_1; break;
			case 2: control = STRING_DRAG_X_2; break;
			case 3: control = STRING_DRAG_X_3; break;
			case 4: control = STRING_DRAG_X_4; break;
			case 5: control = STRING_DRAG_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_PITCH_WHEEL, s, 1, false, 0, 0);

		switch(s)
		{
			case 0: control = STRING_TOUCH_X_0; break;
			case 1: control = STRING_TOUCH_X_1; break;
			case 2: control = STRING_TOUCH_X_2; break;
			case 3: control = STRING_TOUCH_X_3; break;
			case 4: control = STRING_TOUCH_X_4; break;
			case 5: control = STRING_TOUCH_X_5; break;
		}
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_DECAY, s, 1, true, 0, 0);
//		assignSynthVoiceParam(control, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, true, 0, 0);
		assignSynthVoiceParam(control, PARAM_VOICE_VELOCITY, s, 1, true, 0, 0);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

//	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
	assignSynthEffect(TOUCH_X, PARAM_DISTORTION_LEVEL, 0, true, 0, 0);

	setMixerOutputLevel(0, 85);
	setMixerLowCutFilterFrequency(0, 0);
	
/*	setDistortionOn(0, true);
	setDistortionType(0, 5);
	setDistortionDrive(0, 127);
	setDistortionLevel(0, 95);
	setDistortionTone(0, 111);
*/
/*	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);
*/
	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);

/*	setDelayOn(0, true);
	setDelayMode(0, 0);
	setDelayLevel(0, 50);
	setDelayTime(0, 64);
	setDelayFeedback(0, 100);
//	setDelayFeedbackFilter(0, 30);
*/
}

void Synth::loadPadSustain(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
		setDetuneCourse(s, 0, 64);
//		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
//		setChannelVolume(s, 0, 100);
		setAmpEnvAttack(s, 0, 0);
		setAmpEnvRelease(s, 0, 40);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 40, 75);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 40, 75);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_CHANNEL_VOLUME, s, 0, false, 80, 110);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 12);
		setFxBlockOn(s, 1, 1, true);
		setDetuneCourse(s, 1, 64+12);
//		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
		setAmpEnvAttack(s, 1, 0);
		setChannelVolume(s, 1, 120);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(BALL_X, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 0);
		assignSynthVoiceParam(BALL_Y, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_DETUNE_FINE, s, 1, false, 54, 74);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

//	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	assignSynthEffect(VARIATION, PARAM_MIXER_HIGHCUT_FILTER_FREQ, 1, false, 0, 80);

	setMixerOutputLevel(0, 58);
	setMixerLowCutFilterFrequency(0, 0);
	setMixerOutputLevel(1, 66);
	setMixerLowCutFilterFrequency(1, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 3);
	setDistortionDrive(0, 94);
	setDistortionLevel(0, 64);
	setDistortionTone(0, 0);
/*
	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);
*/
	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);
}

void Synth::loadExplode(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 59);
//		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		setChannelVolume(s, 0, 90);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 75);
//		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 75);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 0);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 20);
		setFxBlockOn(s, 1, 0, true);
//		setDetuneCourse(s, 1, 59);
//		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
//		setAmpEnvAttack(s, 1, 64);
		setChannelVolume(s, 1, 127);
//		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 0);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(TOUCH_X, PARAM_DISTORTION_LEVEL, 0, true, 0, 0);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);
	assignSynthEffect(TOUCH_Y, PARAM_MODULATION_RATE, 0, false, 0, 0);

	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 4);
	setDistortionDrive(0, 118);
	setDistortionLevel(0, 100);
	setDistortionTone(0, 64);

	setCompressorOn(0, true);
	setCompressorAttack(0, 96);
	setCompressorRelease(0, 101);
	setCompressorBoost(0, 31);
	setCompressorThreshold(0, 102);
	setCompressorRatio(0, 49);

	//notice this is incomplete
	setModulationType(0, 22);
	setModulationOn(0, true);
	setModulationDepth(0, 127);
	setModulationLevel(0, 113);
	setModulationChorusDelayTime(0, 11);
	setModulationTremolo(0, 0);
}

void Synth::loadSparklevar(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 59);
//		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 64);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 50, 127);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 50, 127);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_CHANNEL_VOLUME, s, 0, false, 70, 110);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_PAN, s, 0, false, 54, 74);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 21);
		setFxBlockOn(s, 1, 0, true);
//		setDetuneCourse(s, 1, 59);
//		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 64);
//		setAmpEnvAttack(s, 1, 64);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 50);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 50);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	setMixerOutputLevel(0, 100);
	setMixerReverbSend(0, 80);
	setMixerLowCutFilterFrequency(0, 0);
	setMixerHighCutFilterFrequency(0, 50);
	
	setDistortionOn(0, true);
	setDistortionType(0, 0);
	setDistortionDrive(0, 93);
	setDistortionLevel(0, 15);
	setDistortionTone(0, 74);

	setCompressorOn(0, true);
	setCompressorAttack(0, 88);
	setCompressorRelease(0, 97);
	setCompressorBoost(0, 69);
	setCompressorThreshold(0, 59);
	setCompressorRatio(0, 85);

	setReverbType(5);
	setReverbLevel(62);
	setReverbCharacter(55);
	setReverbTime(28);
	setReverbPreDelay(97);
	setReverbDelayFeedback(90);

	//notice this is incomplete
//	setModulationType(0, 22);
//	setModulationOn(0, true);
//	setModulationLevel(0, 84);
}

void Synth::loadPhaseGuitar(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 64);
		setPitchBendSemitones(s, 0, 2);
//		setReverbSend(s, 0, 127);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 0, 60);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 0, 60);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 1);
		setFxBlockOn(s, 1, 0, true);
		setDetuneCourse(s, 1, 64+7);
		setPitchBendSemitones(s, 1, 2);
//		setReverbSend(s, 1, 127);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 0, 60);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 0, 60);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 23);
		setFxBlockOn(s, 2, 0, true);
		setDetuneCourse(s, 2, 64);
		setPitchBendSemitones(s, 2, 2);
//		setReverbSend(s, 2, 127);
		assignVelocity(TOUCH_X, s, true, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 2, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 2, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 2, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 2, false, 0, 60);//80110
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 2, false, 0, 60);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
	setDistortionOn(0, true);
	setDistortionType(0, 1);
	setDistortionDrive(0, 105);
	setDistortionLevel(0, 76);
	setDistortionTone(0, 95);

	//notice this is incomplete
	setModulationOn(0, true);
	setModulationType(0, 3);
	setModulationLevel(0, 64);
	setModulationRate(0, 45);
	setModulationDepth(0, 40);
	setModulationChorusFeedback(0, 0);
	setModulationDelayFeedbackFilter(0, 0);
	setModulationChorusDelayTime(0, 26);
}

void Synth::loadSquareLead(void)
{
	resetSettings();
//	setMasterVolume(127);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 2);
		setFxBlockOn(s, 0, 0, true);
//		setDetuneCourse(s, 0, 64);
		setPitchBendSemitones(s, 0, 12);
//		setReverbSend(s, 0, 127);
		assignVelocity(TOUCH_X, s, true, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 0, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 0, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 0, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 0, false, 70, 127);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 0, false, 70, 127);
	}

	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, 2);
		setFxBlockOn(s, 1, 0, true);
		setDetuneCourse(s, 1, 64);
		setPitchBendSemitones(s, 1, 12);
//		setReverbSend(s, 1, 127);
		assignVelocity(TOUCH_X, s, true, 0, 0);
//		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_DECAY, s, 1, false, 0, 0);
		assignSynthVoiceParam(TOUCH_Y, PARAM_VOICE_AMP_ENV_RELEASE, s, 1, false, 0, 0);
		assignSynthPitch(DRAG_X, s, 1, false, 0, 0);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_FREQUENCY, s, 1, false, 70, 127);
		assignSynthVoiceParam(VARIATION, PARAM_VOICE_FILTER_RESONANCE, s, 1, false, 70, 127);
	}

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

	assignSynthEffect(BALL_X, PARAM_DELAY_LEVEL, 0, true, 0, 0);
	assignSynthEffect(BALL_Y, PARAM_DELAY_TIME, 0, true, 0, 0);

//	setMixerOutputLevel(0, 35);
	setMixerLowCutFilterFrequency(0, 0);
	
//	setDistortionOn(0, true);
//	setDistortionType(0, 4);
//	setDistortionDrive(0, 105);
//	setDistortionLevel(0, 100);
//	setDistortionTone(0, 95);

	//notice this is incomplete
//	setModulationOn(0, true);
//	setModulationType(0, 16);
//	setModulationLevel(0, 100);
//	setModulationRate(0, 32);
//	setModulationDepth(0, 109);

	setDelayOn(0, true);
	setDelayMode(0, 1);
	setDelayLevel(0, 70);
	setDelayFeedback(0, 80);
	setDelayFeedbackFilter(0, 44);
	setDelayTime(0, 81);
}

void Synth::loadPresetFromFile(std::string filename)
{
	TiXmlDocument doc(("presets/" + filename).c_str());
	if(!doc.LoadFile())
	{
		std::cout << "Preset file could not be opened." << std::endl << std::flush;
		return;
	}

	resetSettings();

	TiXmlElement *root = doc.RootElement();

	for(TiXmlElement *e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		std::string e_str = e->Value();

		if(e_str == "master")
			current_setting.master_volume = atoi(e->Attribute("volume"));
		else
		if(e_str == "tuning")
		{
			current_setting.tuning[atoi(e->Attribute("string"))] = atoi(e->Attribute("value"));
		}
		else
		if(e_str == "midi_out_channel")
		{
			current_setting.string_midi_out_channel[atoi(e->Attribute("string"))] = atoi(e->Attribute("value"));
		}
		else
		if(e_str == "equalizer")
		{
			setEQOn(atoi(e->Attribute("on")));
			setEQLowMidQ(atoi(e->Attribute("low_mid_q")));
			setEQHighMidQ(atoi(e->Attribute("high_mid_q")));

			for(TiXmlElement *e2 = e->FirstChildElement(); e2 != NULL; e2 = e2->NextSiblingElement())
			{
				if(e2->Value() != "eq_band") continue;
				if(e2->Attribute("type") == "lowest")
				{
					setEQLowestBandGain(atoi(e2->Attribute("gain")));
					setEQLowestBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(e2->Attribute("type") == "lower")
				{
					setEQLowerBandGain(atoi(e2->Attribute("gain")));
					setEQLowerBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(e2->Attribute("type") == "higher")
				{
					setEQHigherBandGain(atoi(e2->Attribute("gain")));
					setEQHigherBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(e2->Attribute("type") == "highest")
				{
					setEQHighestBandGain(atoi(e2->Attribute("gain")));
					setEQHighestBandFrequency(atoi(e2->Attribute("frequency")));
				}
			}
		}
		else
		if(e_str == "reverb")
		{
			setReverbType(atoi(e->Attribute("type")));
			setReverbCharacter(atoi(e->Attribute("character")));
			setReverbPreLpf(atoi(e->Attribute("pre_lpf")));
			setReverbLevel(atoi(e->Attribute("level")));
			setReverbTime(atoi(e->Attribute("time")));
			setReverbDelayFeedback(atoi(e->Attribute("delay_feedback")));
			setReverbPreDelay(atoi(e->Attribute("pre_delay_time")));
		}
		else
		if(e_str == "distortion")
		{
			int fxb = atoi(e->Attribute("fxblock"));
			setDistortionOn(fxb, atoi(e->Attribute("on")));
			setDistortionType(fxb, atoi(e->Attribute("type")));
			setDistortionLevel(fxb, atoi(e->Attribute("level")));
			setDistortionDrive(fxb, atoi(e->Attribute("drive")));
			setDistortionTone(fxb, atoi(e->Attribute("tone")));
			setDistortionBooster(fxb, atoi(e->Attribute("booster")));
		}
		else
		if(e_str == "compression")
		{
			int fxb = atoi(e->Attribute("fxblock"));
			setCompressorOn(fxb, atoi(e->Attribute("on")));
			setCompressorAttack(fxb, atoi(e->Attribute("attack")));
			setCompressorRelease(fxb, atoi(e->Attribute("release")));
			setCompressorThreshold(fxb, atoi(e->Attribute("threshold")));
			setCompressorRatio(fxb, atoi(e->Attribute("ratio")));
			setCompressorBoost(fxb, atoi(e->Attribute("boost")));
			setCompressorKnee(fxb, atoi(e->Attribute("knee")));
		}
		else
		if(e_str == "modulation")
		{
			int fxb = atoi(e->Attribute("fxblock"));
			setModulationType(fxb, atoi(e->Attribute("type")));
			setModulationOn(fxb, atoi(e->Attribute("on")));
			setModulationLevel(fxb, atoi(e->Attribute("level")));
			setModulationChorusDelayTime(fxb, atoi(e->Attribute("chorus_delay_time")));
			setModulationChorusFeedback(fxb, atoi(e->Attribute("chorus_feedback")));
			setModulationChorusHpf(fxb, atoi(e->Attribute("chorus_hpf")));
			setModulationDelayFeedbackFilter(fxb, atoi(e->Attribute("delay_feedback_filter")));
			setModulationRate(fxb, atoi(e->Attribute("rate")));
			setModulationDepth(fxb, atoi(e->Attribute("depth")));
			setModulationTremolo(fxb, atoi(e->Attribute("tremolo")));
		}
		else
		if(e_str == "delay")
		{
			int fxb = atoi(e->Attribute("fxblock"));
			setDelayOn(fxb, atoi(e->Attribute("on")));
			setDelayMode(fxb, atoi(e->Attribute("mode")));
			setDelayPreLp(fxb, atoi(e->Attribute("pre_lp")));
			setDelayLevel(fxb, atoi(e->Attribute("level")));
			setDelayTime(fxb, atoi(e->Attribute("time")));
			setDelayFeedback(fxb, atoi(e->Attribute("feedback")));
			setDelayFeedbackFilter(fxb, atoi(e->Attribute("feedback_filter")));
		}
		else
		if(e_str == "mixer")
		{
			int fxb = atoi(e->Attribute("fxblock"));
			setMixerLowCutFilterFrequency(fxb, atoi(e->Attribute("low_cut_filter_frequency")));
			setMixerHighCutFilterFrequency(fxb, atoi(e->Attribute("high_cut_filter_frequency")));
			setMixerInputGain(fxb, atoi(e->Attribute("input_gain")));
			setMixerOutputLevel(fxb, atoi(e->Attribute("output_level")));
			setMixerPan(fxb, atoi(e->Attribute("pan")));
			setMixerReverbSend(fxb, atoi(e->Attribute("reverb_send")));
		}
		else
		if(e_str == "control")
		{
			if(atoi(e->Attribute("type")) == 100) //fix: this nasty hack immediately!
			{
				assignMidiStopSound(atoi(e->Attribute("string")), atoi(e->Attribute("cc")), atoi(e->Attribute("value")));
			}
			else
			{
				std::vector<struct assignable_effect> *control = getControlList(atoi(e->Attribute("type")));
				struct assignable_effect ae;
				ae.name = atoi(e->Attribute("name"));
				ae.str = atoi(e->Attribute("string"));
				ae.voice_index = atoi(e->Attribute("index"));
				ae.output = atoi(e->Attribute("output"));
				ae.channel = atoi(e->Attribute("channel"));
				ae.cc = atoi(e->Attribute("cc"));
				ae.inverse = atoi(e->Attribute("inverse"));
				ae.variation_start = atoi(e->Attribute("variation_start"));
				ae.variation_end = atoi(e->Attribute("variation_end"));
				ae.fxb = atoi(e->Attribute("fxb"));
				ae.drag_center = atoi(e->Attribute("drag_center"));
				control->push_back(ae);
			}
		}
		else
		if(e_str == "voice")
		{
			int i = atoi(e->Attribute("string"));
			int j = current_setting.voices[i].size();
			insertNewVoice(i, j);

			setChannel(i, j, atoi(e->Attribute("channel")));
			setWave(i, j, atoi(e->Attribute("wavetable_index")));
			setAmpEnvAttack(i, j, atoi(e->Attribute("amplitude_attack")));
			setAmpEnvDecay(i, j, atoi(e->Attribute("amplitude_decay")));
			setAmpEnvRelease(i, j, atoi(e->Attribute("amplitude_release")));
			setFilterFrequency(i, j, atoi(e->Attribute("filter_frequency")));
			setFilterResonance(i, j, atoi(e->Attribute("filter_resonance")));
			setDetuneCourse(i, j, atoi(e->Attribute("detune_course")));
			setDetuneFine(i, j, atoi(e->Attribute("detune_fine")));
			setVibrateRate(i, j, atoi(e->Attribute("vibrate_rate")));
			setVibrateDepth(i, j, atoi(e->Attribute("vibrate_depth")));
			setVibrateDelay(i, j, atoi(e->Attribute("vibrate_delay")));
			setChannelVolume(i, j, atoi(e->Attribute("channel_volume")));
			setPortamentoTime(i, j, atoi(e->Attribute("portamento_time")));
			setPan(i, j, atoi(e->Attribute("pan")));
			setPitchBendSemitones(i, j, atoi(e->Attribute("pitch_bend_semitones")));
			setPitchWheel(i, j, atoi(e->Attribute("pitch_wheel")));
			setVoiceVelocity(i, j, atoi(e->Attribute("velocity")));
			setReverbSend(i, j, atoi(e->Attribute("reverb_send")));
			setFilterType(i, j, atoi(e->Attribute("filter_type")));
			setFilterAttack(i, j, atoi(e->Attribute("filter_attack")));
			setFilterDecay(i, j, atoi(e->Attribute("filter_decay")));
			setFilterRelease(i, j, atoi(e->Attribute("filter_release")));

			if(atoi(e->Attribute("fxb0_on")))
				setFxBlockOn(i, j, 0, true);
			else
			if(atoi(e->Attribute("fxb1_on")))
				setFxBlockOn(i, j, 1, true);
		}
	}
}

void Synth::savePresetToFile(struct synth_setting *s, std::string filepath)
{
	TiXmlDocument doc;
	TiXmlDeclaration *decl;
	TiXmlElement *root, *element, *element2;

	decl = new TiXmlDeclaration("1.0", "", "");

	doc.LinkEndChild(decl);

	//root node
	root = new TiXmlElement("preset");
	doc.LinkEndChild(root);

	//master settings
	element = new TiXmlElement("master");
	element->SetAttribute("volume", s->master_volume);
	root->LinkEndChild(element);

	//tuning 
	for(int i = 0; i < 6; i++)
	{
		element = new TiXmlElement("tuning");
		element->SetAttribute("string", i);
		element->SetAttribute("value", s->tuning[i]);
		root->LinkEndChild(element);
	}

	//midi out channel
	for(int i = 0; i < 6; i++)
	{
		element = new TiXmlElement("midi_out_channel");
		element->SetAttribute("string", i);
		element->SetAttribute("value", s->string_midi_out_channel[i]);
		root->LinkEndChild(element);
	}

	//equalizer
	element = new TiXmlElement("equalizer");
	element->SetAttribute("on", (int) s->equalizer.on);
	element->SetAttribute("low_mid_q", s->equalizer.low_mid_q);
	element->SetAttribute("high_mid_q", s->equalizer.high_mid_q);
	root->LinkEndChild(element);

	element2 = new TiXmlElement("eq_band");
	element2->SetAttribute("type", "lowest");
	element2->SetAttribute("gain", s->equalizer.lowest.gain);
	element2->SetAttribute("frequency", s->equalizer.lowest.frequency);
	element->LinkEndChild(element2);

	element2 = new TiXmlElement("eq_band");
	element2->SetAttribute("type", "lower");
	element2->SetAttribute("gain", s->equalizer.lower.gain);
	element2->SetAttribute("frequency", s->equalizer.lower.frequency);
	element->LinkEndChild(element2);

	element2 = new TiXmlElement("eq_band");
	element2->SetAttribute("type", "higher");
	element2->SetAttribute("gain", s->equalizer.higher.gain);
	element2->SetAttribute("frequency", s->equalizer.higher.frequency);
	element->LinkEndChild(element2);

	element2 = new TiXmlElement("eq_band");
	element2->SetAttribute("type", "highest");
	element2->SetAttribute("gain", s->equalizer.highest.gain);
	element2->SetAttribute("frequency", s->equalizer.highest.frequency);
	element->LinkEndChild(element2);

	//reverb
	element = new TiXmlElement("reverb");
	element->SetAttribute("type", s->reverb.type);
	element->SetAttribute("character", s->reverb.character);
	element->SetAttribute("pre_lpf", s->reverb.pre_lpf);
	element->SetAttribute("level", s->reverb.level);
	element->SetAttribute("time", s->reverb.time);
	element->SetAttribute("delay_feedback", s->reverb.delay_feedback);
	element->SetAttribute("pre_delay_time", s->reverb.pre_delay_time);
	root->LinkEndChild(element);

	//fx blocks
	for(int fxb = 0; fxb < 2; fxb++)
	{
		element = new TiXmlElement("distortion");
		element->SetAttribute("fxblock", fxb);
		element->SetAttribute("on", s->fx_block[fxb].distortion.on);
		element->SetAttribute("type", s->fx_block[fxb].distortion.type);
		element->SetAttribute("level", s->fx_block[fxb].distortion.level);
		element->SetAttribute("drive", s->fx_block[fxb].distortion.drive);
		element->SetAttribute("tone", s->fx_block[fxb].distortion.tone);
		element->SetAttribute("booster", s->fx_block[fxb].distortion.booster);
		root->LinkEndChild(element);

		element = new TiXmlElement("compression");
		element->SetAttribute("fxblock", fxb);
		element->SetAttribute("on", s->fx_block[fxb].compressor.on);
		element->SetAttribute("attack", s->fx_block[fxb].compressor.attack);
		element->SetAttribute("release", s->fx_block[fxb].compressor.release);
		element->SetAttribute("threshold", s->fx_block[fxb].compressor.threshold);
		element->SetAttribute("ratio", s->fx_block[fxb].compressor.ratio);
		element->SetAttribute("boost", s->fx_block[fxb].compressor.boost);
		element->SetAttribute("knee", s->fx_block[fxb].compressor.knee);
		root->LinkEndChild(element);

		element = new TiXmlElement("modulation");
		element->SetAttribute("fxblock", fxb);
		element->SetAttribute("on", s->fx_block[fxb].modulation.on);
		element->SetAttribute("type", s->fx_block[fxb].modulation.type);
		element->SetAttribute("level", s->fx_block[fxb].modulation.level);
		element->SetAttribute("chorus_delay_time", s->fx_block[fxb].modulation.chorus_delay_time);
		element->SetAttribute("chorus_feedback", s->fx_block[fxb].modulation.chorus_feedback);
		element->SetAttribute("chorus_hpf", s->fx_block[fxb].modulation.chorus_hpf);
		element->SetAttribute("delay_feedback_filter", s->fx_block[fxb].modulation.delay_feedback_filter);
		element->SetAttribute("rate", s->fx_block[fxb].modulation.rate);
		element->SetAttribute("depth", s->fx_block[fxb].modulation.depth);
		element->SetAttribute("tremolo", s->fx_block[fxb].modulation.tremolo);
		root->LinkEndChild(element);

		element = new TiXmlElement("delay");
		element->SetAttribute("fxblock", fxb);
		element->SetAttribute("on", s->fx_block[fxb].delay.on);
		element->SetAttribute("mode", s->fx_block[fxb].delay.mode);
		element->SetAttribute("pre_lp", s->fx_block[fxb].delay.pre_lp);
		element->SetAttribute("level", s->fx_block[fxb].delay.level);
		element->SetAttribute("time", s->fx_block[fxb].delay.time);
		element->SetAttribute("feedback", s->fx_block[fxb].delay.feedback);
		element->SetAttribute("feedback_filter", s->fx_block[fxb].delay.feedback_filter);
		root->LinkEndChild(element);

		element = new TiXmlElement("mixer");
		element->SetAttribute("fxblock", fxb);
		element->SetAttribute("low_cut_filter_frequency", s->fx_block[fxb].mixer.low_cut_filter_frequency);
		element->SetAttribute("high_cut_filter_frequency", s->fx_block[fxb].mixer.high_cut_filter_frequency);
		element->SetAttribute("input_gain", s->fx_block[fxb].mixer.input_gain);
		element->SetAttribute("output_level", s->fx_block[fxb].mixer.output_level);
		element->SetAttribute("pan", s->fx_block[fxb].mixer.pan);
		element->SetAttribute("reverb_send", s->fx_block[fxb].mixer.reverb_send);
		root->LinkEndChild(element);
	}

	//voices
	for(int str = 0; str < 6; str++)
		for(int vi = 0; vi < s->voices[str].size(); vi++)
		{
			element = new TiXmlElement("voice");
			element->SetAttribute("string", str);
//			element->SetAttribute("index", vi);
			element->SetAttribute("channel", s->voices[str].at(vi).channel);
			element->SetAttribute("wavetable_index", s->voices[str].at(vi).wavetable_index);
			element->SetAttribute("amplitude_attack", s->voices[str].at(vi).amp_env_attack);
			element->SetAttribute("amplitude_decay", s->voices[str].at(vi).amp_env_decay);
			element->SetAttribute("amplitude_release", s->voices[str].at(vi).amp_env_release);
			element->SetAttribute("detune_course", s->voices[str].at(vi).detune_course);
			element->SetAttribute("detune_fine", s->voices[str].at(vi).detune_fine);
			element->SetAttribute("vibrate_rate", s->voices[str].at(vi).vibrate_rate);
			element->SetAttribute("vibrate_depth", s->voices[str].at(vi).vibrate_depth);
			element->SetAttribute("vibrate_delay", s->voices[str].at(vi).vibrate_delay);
			element->SetAttribute("channel_volume", s->voices[str].at(vi).channel_volume);
			element->SetAttribute("portamento_time", s->voices[str].at(vi).portamento_time);
			element->SetAttribute("pan", s->voices[str].at(vi).pan);
			element->SetAttribute("pitch_bend_semitones", s->voices[str].at(vi).pitch_bend_semitones);
			element->SetAttribute("pitch_wheel", s->voices[str].at(vi).pitch_wheel);
			element->SetAttribute("velocity", s->voices[str].at(vi).velocity);
			element->SetAttribute("fxb0_on", s->voices[str].at(vi).insert_fx_block_on[0]);
			element->SetAttribute("fxb1_on", s->voices[str].at(vi).insert_fx_block_on[1]);
			element->SetAttribute("reverb_send", s->voices[str].at(vi).reverb_send);
			element->SetAttribute("filter_frequency", s->voices[str].at(vi).filter_frequency);
			element->SetAttribute("filter_resonance", s->voices[str].at(vi).filter_resonance);
			element->SetAttribute("filter_type", s->voices[str].at(vi).filter_type);
			element->SetAttribute("filter_attack", s->voices[str].at(vi).filter_attack);
			element->SetAttribute("filter_decay", s->voices[str].at(vi).filter_decay);
			element->SetAttribute("filter_release", s->voices[str].at(vi).filter_release);
			root->LinkEndChild(element);
		}

	for(int c = TOUCH_X; c < CONTROL_END; c++)
	{
		std::vector<struct assignable_effect> *control = getControlList(c);
		for(int i = 0; i < control->size(); i++)
		{
			element = new TiXmlElement("control");
			element->SetAttribute("type", c);
			element->SetAttribute("name", control->at(i).name);
			element->SetAttribute("string", control->at(i).str);
			element->SetAttribute("index", control->at(i).voice_index);
			element->SetAttribute("output", control->at(i).output);
			element->SetAttribute("channel", control->at(i).channel);
			element->SetAttribute("cc", control->at(i).cc);
			element->SetAttribute("inverse", control->at(i).inverse);
			element->SetAttribute("variation_start", control->at(i).variation_start);
			element->SetAttribute("variation_end", control->at(i).variation_end);
			element->SetAttribute("fxb", control->at(i).fxb);
			element->SetAttribute("drag_center", control->at(i).drag_center);
			root->LinkEndChild(element);
		}
	}

	for(int i = 0; i < 6; i++)
		for(int j = 0; j < s->stop_sound_cmds[i].size(); j++)
		{
			element = new TiXmlElement("control");
			element->SetAttribute("type", 100); //fix: this nasty hack immediately!
			element->SetAttribute("string", i);
			element->SetAttribute("cc", s->stop_sound_cmds[i][j].cc_num);
			element->SetAttribute("value", s->stop_sound_cmds[i][j].value);
			root->LinkEndChild(element);
		}

	doc.SaveFile(filepath.c_str());

	system("sync");
}

void Synth::printCurrentSettings(void)
{
	std::cout << " MVol " << current_setting.master_volume;
	std::cout << " LFO R " << current_setting.lfo_rate;
	std::cout << " LFO FD " << current_setting.lfo_frequency_depth;
	std::cout << " LFO AD " << current_setting.lfo_amp_depth;
	std::cout << " LFO PD " << current_setting.lfo_pitch_depth;

	std::cout << " EQ 1G " << (current_setting.equalizer.lowest.gain);
	std::cout << " EQ 2G " << (current_setting.equalizer.lower.gain);
	std::cout << " EQ 3G " << (current_setting.equalizer.higher.gain);
	std::cout << " EQ 4G " << (current_setting.equalizer.highest.gain);
	std::cout << " EQ 1F " << (current_setting.equalizer.lowest.frequency);
	std::cout << " EQ 2F " << (current_setting.equalizer.lower.frequency);
	std::cout << " EQ 3F " << (current_setting.equalizer.higher.frequency);
	std::cout << " EQ 4F " << (current_setting.equalizer.highest.frequency);
	std::cout << " EQ LMQ " << (current_setting.equalizer.low_mid_q);
	std::cout << " EQ HMQ " << (current_setting.equalizer.high_mid_q);

	std::cout << " Rvb Typ " << (current_setting.reverb.type);
	std::cout << " Rvb Chr " << (current_setting.reverb.character);
	std::cout << " Rvb LPF " << (current_setting.reverb.pre_lpf);
	std::cout << " Rvb Lvl " << (current_setting.reverb.level);
	std::cout << " Rvb Tim " << (current_setting.reverb.time);
	std::cout << " Rvb FBK " << (current_setting.reverb.delay_feedback);
	std::cout << " Rvb DTm " << (current_setting.reverb.pre_delay_time);

	for(int i = 0; i < 2; i++)
	{
		std::cout << " Dist On " << (unsigned int) current_setting.fx_block[i].distortion.on;
		std::cout << " Dist Typ " << current_setting.fx_block[i].distortion.type;
		std::cout << " Dist Lvl " << current_setting.fx_block[i].distortion.level;
		std::cout << " Dist Drv " << current_setting.fx_block[i].distortion.drive;
		std::cout << " Dist Ton " << current_setting.fx_block[i].distortion.tone;

		std::cout << " Comp On " << current_setting.fx_block[i].compressor.on;
		std::cout << " Comp Att " << current_setting.fx_block[i].compressor.attack;
		std::cout << " Comp Rel " << current_setting.fx_block[i].compressor.release;
		std::cout << " Comp Thr " << current_setting.fx_block[i].compressor.threshold;
		std::cout << " Comp Rat " << current_setting.fx_block[i].compressor.ratio;
		std::cout << " Comp Bst " << current_setting.fx_block[i].compressor.boost;
		std::cout << " Comp Kne " << current_setting.fx_block[i].compressor.knee;

		std::cout << " Mod On " << current_setting.fx_block[i].modulation.on;
		std::cout << " Mod Typ " << current_setting.fx_block[i].modulation.type;
		std::cout << " Mod Lvl " << current_setting.fx_block[i].modulation.level;
		std::cout << " Mod CDT " << current_setting.fx_block[i].modulation.chorus_delay_time;
		std::cout << " Mod CFb " << current_setting.fx_block[i].modulation.chorus_feedback;
		std::cout << " Mod HPF " << current_setting.fx_block[i].modulation.chorus_hpf;
		std::cout << " Mod DFF " << current_setting.fx_block[i].modulation.delay_feedback_filter;
		std::cout << " Mod Rat " << current_setting.fx_block[i].modulation.rate;
		std::cout << " Mod Dep " << current_setting.fx_block[i].modulation.depth;
		std::cout << " Mod Tre " << current_setting.fx_block[i].modulation.tremolo;

		std::cout << " Del On " << current_setting.fx_block[i].delay.on;
		std::cout << " Del Md " << current_setting.fx_block[i].delay.mode;
		std::cout << " Del LP " << current_setting.fx_block[i].delay.pre_lp;
		std::cout << " Del Lvl " << current_setting.fx_block[i].delay.level;
		std::cout << " Del Tme " << current_setting.fx_block[i].delay.time;
		std::cout << " Del FBK " << current_setting.fx_block[i].delay.feedback;
		std::cout << " Del FBF " << current_setting.fx_block[i].delay.feedback_filter;

		std::cout << " Mix LCF " << current_setting.fx_block[i].mixer.low_cut_filter_frequency;
		std::cout << " Mix HCF " << current_setting.fx_block[i].mixer.high_cut_filter_frequency;
		std::cout << " Mix IGn " << current_setting.fx_block[i].mixer.input_gain;
		std::cout << " Mix OLv " << current_setting.fx_block[i].mixer.output_level;
		std::cout << " Mix RSn " << current_setting.fx_block[i].mixer.reverb_send;
	}	
	std::cout << std::flush;
}

void Synth::insertNewVoice(int str, int wave)
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
		if(new_channel == 16) continue; //some type of chip bug?
		if(find(channel_list.begin(), channel_list.end(), new_channel) == channel_list.end())
			break;
	}

	setChannel(str, v, new_channel);
	setWave(str, v, wave); //needs to be the first thing after setChannel()
	setAmpEnvAttack(str, v, 64);
	setAmpEnvDecay(str, v, 64);
	setAmpEnvRelease(str, v, 64);
	setFilterFrequency(str, v, 64);
	setFilterResonance(str, v, 64);
	setDetuneCourse(str, v, 64);
	setDetuneFine(str, v, 64);
	setVibrateRate(str, v, 64);
	setVibrateDepth(str, v, 64);
	setVibrateDelay(str, v, 64);
	setChannelVolume(str, v, 100);
	setPortamentoTime(str, v, 0);
	setPan(str, v, 64);
	setPitchBendSemitones(str, v, 2);
	setPitchWheel(str, v, 0x2000);
	setVoiceVelocity(str, v, 127);
	setReverbSend(str, v, 0);
	setFilterType(str, v, 4); //off
	setFilterAttack(str, v, 64);
	setFilterDecay(str, v, 64);
	setFilterRelease(str, v, 64);

	setFxBlockOn(str, v, 0, false);
	setFxBlockOn(str, v, 1, false);
}

void Synth::removeVoice(int str, int voice_index)
{
	current_setting.voices[str].erase(current_setting.voices[str].begin() + voice_index);
}

void Synth::assignSynthEffect(unsigned char touch_control, unsigned char synth_param, unsigned char fxb, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = synth_param;
	ae.fxb = fxb;
	ae.inverse = inverse;
	ae.output = SYNTH;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignSynthEffect(unsigned char synth_param, unsigned char fxb)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).fxb == fxb))
			control_list->erase(control_list->begin() + i);
		}
	}
}

void Synth::assignSynthVoiceParam(unsigned char touch_control, unsigned char synth_param, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = synth_param;
	ae.str = str;
	ae.voice_index = voice_index;
	ae.output = SYNTH;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignSynthVoiceParam(unsigned char synth_param, unsigned char str, unsigned char voice_index)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).str == str) && (control_list->at(i).voice_index == voice_index))
				control_list->erase(control_list->begin() + i);
		}
	}
}

void Synth::assignVelocity(unsigned char touch_control, int str, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_VELOCITY;
	ae.str = str;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignVelocity(int str)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_VELOCITY) && (control_list->at(i).str == str))
				control_list->erase(control_list->begin() + i);
		}
	}
}

void Synth::assignSynthPitch(unsigned char touch_control, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_PITCH;
	ae.output = SYNTH;
	ae.channel = current_setting.voices[str].at(voice_index).channel;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::assignMidiPitch(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_PITCH;
	ae.output = MIDI_OUT;
	ae.channel = channel;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignSynthPitch(unsigned char str, unsigned char voice_index)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		int chan = current_setting.voices[str].at(voice_index).channel;
		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_PITCH) && (control_list->at(i).output == SYNTH) && (control_list->at(i).channel == chan))
				control_list->erase(control_list->begin() + i);
		}
	}
}

void Synth::unAssignMidiPitch(unsigned char channel)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_PITCH) && (control_list->at(i).output == MIDI_OUT) && (control_list->at(i).channel == channel))
				control_list->erase(control_list->begin() + i);
		}
	}
}

void Synth::assignCC(unsigned char touch_control, unsigned char output, unsigned char channel, unsigned int cc, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_CC;
	ae.output = output;
	ae.channel = channel;
	ae.cc = cc;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignCC(unsigned char output, unsigned char channel, unsigned int cc)
{
	for(int touch_control = TOUCH_X; touch_control <= VARIATION; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_CC) && (control_list->at(i).output == output) && (control_list->at(i).channel == channel) && (control_list->at(i).cc == cc))
				control_list->erase(control_list->begin() + i);
		}
	}
}

void Synth::addToControl(unsigned char touch_control, struct assignable_effect ae)
{
	getControlList(touch_control)->push_back(ae);
}

std::vector<struct assignable_effect> *Synth::getControlList(unsigned char touch_control)
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

void Synth::setLfoRate(int val)
{
	std::cout << "setLfoRate not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x03, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_rate = val;
}

void Synth::setLfoFrequencyDepth(int val)
{
	std::cout << "setLfoFrequencyDepth not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x05, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_frequency_depth = val;
}

void Synth::setLfoAmpDepth(int val)
{
	std::cout << "setLfoAmpDepth not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x06, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_amp_depth = val;
}

void Synth::setLfoPitchDepth(int val)
{
	std::cout << "setLfoPitchDepth not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x04, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_pitch_depth = val;
}

void Synth::setMasterVolume(int val)
{
	unsigned char sysex[8] = {0xF0, 0x7F, 0x7F, 0x04, 0x01, 0x00, val, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 8);

	current_setting.master_volume = val;
}

void Synth::setChannel(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).channel = val;
}

void Synth::setWave(int str, int voice_index, int program)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendProgramChange(channel, 0, program);
//	current_setting.voices[str].at(voice_index).bank = bank;
	current_setting.voices[str].at(voice_index).wavetable_index = program;
}

void Synth::setAmpEnvAttack(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x63, val);
	current_setting.voices[str].at(voice_index).amp_env_attack = val;
}

void Synth::setAmpEnvDecay(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x64, val);
	current_setting.voices[str].at(voice_index).amp_env_decay = val;
}

void Synth::setAmpEnvRelease(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x66, val);
	current_setting.voices[str].at(voice_index).amp_env_release = val;
}

void Synth::setFilterFrequency(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x20, val);
	current_setting.voices[str].at(voice_index).filter_frequency = val;
}

void Synth::setFilterResonance(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x21, val);
	current_setting.voices[str].at(voice_index).filter_resonance = val;
}

void Synth::setDetuneCourse(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendRPN(SYNTH, channel, 0x00, 0x02, val);
	current_setting.voices[str].at(voice_index).detune_course = val;
}

void Synth::setDetuneFine(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendRPN(SYNTH, channel, 0x00, 0x01, val);
	current_setting.voices[str].at(voice_index).detune_fine = val;
}

void Synth::setVibrateRate(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x08, val);
	current_setting.voices[str].at(voice_index).vibrate_rate = val;
}

void Synth::setVibrateDepth(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x09, val);
	current_setting.voices[str].at(voice_index).vibrate_depth = val;
}

void Synth::setVibrateDelay(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendNRPN(SYNTH, channel, 0x01, 0x0A, val);
	current_setting.voices[str].at(voice_index).vibrate_delay = val;
}

void Synth::setChannelVolume(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendCC(SYNTH, channel, 7, val);
	current_setting.voices[str].at(voice_index).channel_volume = val;
}

void Synth::setPortamentoTime(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendCC(SYNTH, channel, 5, val);
	current_setting.voices[str].at(voice_index).portamento_time = val;
}

void Synth::setReverbSend(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendCC(SYNTH, channel, 91, val);
	current_setting.voices[str].at(voice_index).reverb_send = val;
}

void Synth::setPan(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendCC(SYNTH, channel, 10, val);
	current_setting.voices[str].at(voice_index).pan = val;
}

void Synth::setPitchBendSemitones(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendRPN(SYNTH, channel, 0x00, 0x00, val);
	current_setting.voices[str].at(voice_index).pitch_bend_semitones = val;
}

void Synth::setPitchWheel(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
//	midi.sendPitch(SYNTH, channel, val);
	current_setting.voices[str].at(voice_index).pitch_wheel = val;
}

void Synth::setVoiceVelocity(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).velocity = val;
}

void Synth::setFilterType(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_type = val;
}

void Synth::setFilterAttack(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_attack = val;
}

void Synth::setFilterDecay(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_decay = val;
}

void Synth::setFilterRelease(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).filter_release = val;
}

void Synth::setFxBlockOn(int str, int voice_index, int fxb, bool state)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x40+channel, 0x22, fxb+1, 0x00, 0xF7};
	if(!state)
		sysex[8] = 0;	//turn off
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.voices[str].at(voice_index).insert_fx_block_on[fxb] = state;
}

void Synth::setDistortionOn(int fxb, bool state)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x61, state);
	current_setting.fx_block[fxb].distortion.on = state;
}

void Synth::setDistortionType(int fxb, int val)
{
	//modified for new algorithms (was 0x63)
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x60, val);
	current_setting.fx_block[fxb].distortion.type = val;
}

void Synth::setDistortionLevel(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x66, val);
	current_setting.fx_block[fxb].distortion.level = val;
}

void Synth::setDistortionDrive(int fxb, int val)
{
	//modified for new algorithms (was 0x67)
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x62, val);
	current_setting.fx_block[fxb].distortion.drive = val;
}

void Synth::setDistortionTone(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x64, val);
	current_setting.fx_block[fxb].distortion.tone = val;
}

void Synth::setDistortionNoiseReduction(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x68, val);
	current_setting.fx_block[fxb].distortion.noise_reduction = val;
}

void Synth::setDistortionBooster(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x69, val);
	current_setting.fx_block[fxb].distortion.booster = val;
}

void Synth::setCompressorOn(int fxb, bool state)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x41, state);
	current_setting.fx_block[fxb].compressor.on = state;
}

void Synth::setCompressorAttack(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x42, val);
	current_setting.fx_block[fxb].compressor.attack = val;
}

void Synth::setCompressorRelease(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x43, val);
	current_setting.fx_block[fxb].compressor.release = val;
}

void Synth::setCompressorThreshold(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x44, val);
	current_setting.fx_block[fxb].compressor.threshold = val;
}

void Synth::setCompressorRatio(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x45, val);
	current_setting.fx_block[fxb].compressor.ratio = val;
}

void Synth::setCompressorBoost(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x46, val);
	current_setting.fx_block[fxb].compressor.boost = val;
}

void Synth::setCompressorKnee(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x47, val);
	current_setting.fx_block[fxb].compressor.knee = val;
}

/*
void Synth::setWahwahType(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x51, val);
	current_setting.fx_block[fxb].wahwah.type = val;
}

void Synth::setWahwahFrequency(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x53, val);
	current_setting.fx_block[fxb].wahwah.frequency = val;
}

void Synth::setWahwahResonance(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x53, val);
	current_setting.fx_block[fxb].wahwah.resonance = val;
}

void Synth::setWahwahSensitivity(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x54, val);
	current_setting.fx_block[fxb].wahwah.sensitivity = val;
}
*/

void Synth::setModulationOn(int fxb, bool state)
{
/*	if(state)
		midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x31, current_setting.fx_block[fxb].modulation.level);
	else
		midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x31, 0);
*/
	current_setting.fx_block[fxb].modulation.on = state;
}

void Synth::setModulationType(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x30, val);
	current_setting.fx_block[fxb].modulation.type = val;
}

void Synth::setModulationLevel(int fxb, int val)
{
//	if(current_setting.fx_block[fxb].modulation.on)
//		midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x31, val);
	current_setting.fx_block[fxb].modulation.level = val;
}

void Synth::setModulationChorusDelayTime(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x32, val);
	current_setting.fx_block[fxb].modulation.chorus_delay_time = val;
}

void Synth::setModulationChorusFeedback(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x33, val);
	current_setting.fx_block[fxb].modulation.chorus_feedback = val;
}

void Synth::setModulationChorusHpf(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x34, val);
	current_setting.fx_block[fxb].modulation.chorus_hpf = val;
}

void Synth::setModulationDelayFeedbackFilter(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x35, val);
	current_setting.fx_block[fxb].modulation.delay_feedback_filter = val;
}

void Synth::setModulationRate(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x36, val);
	current_setting.fx_block[fxb].modulation.rate = val;
}

void Synth::setModulationDepth(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x37, val);
	current_setting.fx_block[fxb].modulation.depth = val;
}

void Synth::setModulationTremolo(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x38, val);
	current_setting.fx_block[fxb].modulation.tremolo = val;
}

void Synth::setDelayOn(int fxb, bool state)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x59, state);
	current_setting.fx_block[fxb].delay.on = state;
}

void Synth::setDelayMode(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5A, val);
	current_setting.fx_block[fxb].delay.mode = val;
}

void Synth::setDelayPreLp(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5B, val);
	current_setting.fx_block[fxb].delay.pre_lp = val;
}

void Synth::setDelayLevel(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5C, val);
	current_setting.fx_block[fxb].delay.level = val;
}

void Synth::setDelayTime(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5D, val);
	current_setting.fx_block[fxb].delay.time = val;
}

void Synth::setDelayFeedback(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5E, val);
	current_setting.fx_block[fxb].delay.feedback = val;
}

void Synth::setDelayFeedbackFilter(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5F, val);
	current_setting.fx_block[fxb].delay.feedback_filter = val;
}

void Synth::setReverbType(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x30, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.type = val;
}

void Synth::setReverbCharacter(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x31, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.character = val;
}

void Synth::setReverbPreLpf(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x32, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.pre_lpf = val;
}

void Synth::setReverbLevel(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x33, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.level = val;
}

void Synth::setReverbTime(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x34, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.time = val;
}

void Synth::setReverbDelayFeedback(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x35, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.delay_feedback = val;
}

void Synth::setReverbPreDelay(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x37, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.pre_delay_time = val;
}

void Synth::setEQOn(bool state)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x55, val);
	current_setting.equalizer.on = state;
}

void Synth::setEQLowestBandGain(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x08, val);
	current_setting.equalizer.lowest.gain = val;
}

void Synth::setEQLowerBandGain(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x09, val);
	current_setting.equalizer.lower.gain = val;
}

void Synth::setEQHigherBandGain(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x0A, val);
	current_setting.equalizer.higher.gain = val;
}

void Synth::setEQHighestBandGain(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x0B, val);
	current_setting.equalizer.highest.gain = val;
}

void Synth::setEQLowestBandFrequency(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x0C, val);
	current_setting.equalizer.lowest.frequency = val;
}

void Synth::setEQLowerBandFrequency(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x0D, val);
	current_setting.equalizer.lower.frequency = val;
}

void Synth::setEQHigherBandFrequency(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x0E, val);
	current_setting.equalizer.higher.frequency = val;
}

void Synth::setEQHighestBandFrequency(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x0F, val);
	current_setting.equalizer.highest.frequency = val;
}

void Synth::setEQLowMidQ(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x01, val);
	current_setting.equalizer.low_mid_q = val;
}

void Synth::setEQHighMidQ(int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x37, 0x02, val);
	current_setting.equalizer.high_mid_q = val;
}

void Synth::setMixerLowCutFilterFrequency(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x22, val); //datasheet incorrect
	current_setting.fx_block[fxb].mixer.low_cut_filter_frequency = val;
}

void Synth::setMixerHighCutFilterFrequency(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x23, val); //datasheet incorrect
	current_setting.fx_block[fxb].mixer.high_cut_filter_frequency = val;
}

void Synth::setMixerInputGain(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x21, val); //datasheet incorrect
	current_setting.fx_block[fxb].mixer.input_gain = val;
}

void Synth::setMixerOutputLevel(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x2A, val);
	current_setting.fx_block[fxb].mixer.output_level = val;
}

void Synth::setMixerPan(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x2B, val);
	current_setting.fx_block[fxb].mixer.pan = val;
}

void Synth::setMixerReverbSend(int fxb, int val)
{
//	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x2D, val);
	current_setting.fx_block[fxb].mixer.reverb_send = val;
}

/*
void Synth::sendNoteOn(unsigned char str, unsigned char note, bool harmonic, bool attack)
{
	if(!harmonic)
	{
		if(current_setting.string_midi_out_channel[str] >= 0)
		{
			sendVariation(str, current_setting.string_midi_out_channel[str]);
			midi.sendNoteOn(MIDI_OUT, current_setting.string_midi_out_channel[str], note, velocity[str]);
		}
		else
		{
			for(int i = 0; i < current_setting.voices[str].size(); i++)
			{
				sendVariation(str, current_setting.voices[str].at(i).channel);
				if(!attack) 
					midi.sendProgramChange(current_setting.voices[str].at(i).channel, 1, current_setting.voices[str].at(i).wavetable_index);
				midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i).channel, note, velocity[str]);
				if(!attack) 
					midi.sendProgramChange(current_setting.voices[str].at(i).channel, 0, current_setting.voices[str].at(i).wavetable_index);
			}
		}
	}
	else
	{
		if(current_setting.string_midi_out_channel[str] >= 0)
		{
			sendVariation(str, current_setting.string_midi_out_channel[str]);
			midi.sendNoteOn(MIDI_OUT, current_setting.string_midi_out_channel[str], note+24, velocity[str]);
		}
		else
		{
			for(int i = 0; i < current_setting.voices[str].size(); i++)
			{
				sendVariation(str, current_setting.voices[str].at(i).channel);
				midi.sendCC(SYNTH, current_setting.voices[str].at(i).channel, 127, 0); //set polyphonic mode
				if(!attack)
					midi.sendProgramChange(current_setting.voices[str].at(i).channel, 1, current_setting.voices[str].at(i).wavetable_index);
				midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i).channel, note+36, velocity[str]);
				midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i).channel, note+24, velocity[str]);
				if(!attack)
					midi.sendProgramChange(current_setting.voices[str].at(i).channel, 0, current_setting.voices[str].at(i).wavetable_index);
			}
		}
	}
}

void Synth::sendNoteOff(unsigned char str, unsigned char note, bool harmonic)
{
	if(current_setting.string_midi_out_channel[str] >= 0)
		midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], note+24, 0);
	else
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
		{
			if(harmonic)
			{
				midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, note+36, 0);
				midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, note+24, 0);
				midi.sendCC(SYNTH, current_setting.voices[str].at(i).channel, 126, 0); //set monophonic mode
			}
			else
			{
				midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, note, 0);
				midi.sendCC(SYNTH, current_setting.voices[str].at(i).channel, 126, 0); //set monophonic mode
			}
		}
	}
}

void Synth::sendStopSound(unsigned char str, unsigned char note, bool harmonic)
{
	if(!(current_setting.string_midi_out_channel[str] >= 0))
//		midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], note, 0);
//	else
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
		{
			//fix: make sure this does not stop audio that is being processed by the FX
			midi.sendSoundOff(SYNTH, current_setting.voices[str].at(i).channel);
		}
	}
}

void Synth::sendCC(unsigned char str, unsigned char control, unsigned char val)
{
	if(str == 6)
	{
		for(int s = 0; s < 6; s++)
			for(int i = 0; i < current_setting.voices[str].size(); i++)
				midi.sendCC(SYNTH, current_setting.voices[s].at(i).channel, control, val);
	}
	else
		for(int i = 0; i < current_setting.voices[str].size(); i++)
			midi.sendCC(SYNTH, current_setting.voices[str].at(i).channel, control, val);
}

unsigned char Synth::midiScale(int number, int scaler)
{
	int val = (number * 127) / scaler;

	return (unsigned char) val;
}

void Synth::sendControl(unsigned char control_type, int val, int scaler)
{
	std::vector<struct assignable_effect> *control_list = getControlList(control_type);

	for(int i = 0; i < control_list->size(); i++)
	{
		struct assignable_effect *ae = &control_list->at(i);
		sendToEffect(ae, val, scaler);
	}
}

void Synth::sendVariation(int str, int channel)
{
	for(int i = 0; i < current_setting.AE_variation.size(); i++)
	{
		struct assignable_effect *ae = &current_setting.AE_variation.at(i);

		bool flag = false;
		switch(ae->name)
		{
			case PARAM_PITCH:
			case PARAM_CC:
				if(ae->channel == channel)
					flag = true;
				break;

			case PARAM_VELOCITY:
				if(ae->str == str)
					flag = true;
				break;

			default: //synth effect or voice parameter
				if(ae->name >= PARAM_VOICE_AMP_ENV_ATTACK)
				{
					if(ae->str == str)
						flag = true;
				}
				else
					flag = true;
				break;
		}

		if(flag)
		{
			int r = (rand() % (ae->variation_end + 1 - ae->variation_start)) + ae->variation_start;
			sendToEffect(ae, r, 127);
		}

		flag = false;
	}
}

void Synth::sendToEffect(struct assignable_effect *ae, int val, int scaler)
{
	unsigned char mval = midiScale(val, scaler);

	unsigned char final_val = mval;
	if(ae->inverse) final_val = 127 - final_val;

	switch(ae->name)
	{
		//hopefully the compiler compiles this to a branch table
		case PARAM_DISTORTION_ON: setDistortionOn(ae->fxb, final_val); break;
		case PARAM_DISTORTION_TYPE: setDistortionType(ae->fxb, final_val); break;
		case PARAM_DISTORTION_LEVEL: setDistortionLevel(ae->fxb, final_val); break;
		case PARAM_DISTORTION_DRIVE: setDistortionDrive(ae->fxb, final_val); break;
		case PARAM_DISTORTION_TONE: setDistortionTone(ae->fxb, final_val); break;
		case PARAM_COMPRESSION_ON: setCompressorOn(ae->fxb, final_val); break;
		case PARAM_COMPRESSION_ATTACK: setCompressorAttack(ae->fxb, final_val); break;
		case PARAM_COMPRESSION_RELEASE: setCompressorRelease(ae->fxb, final_val); break;
		case PARAM_COMPRESSION_THRESHOLD: setCompressorThreshold(ae->fxb, final_val); break;
		case PARAM_COMPRESSION_RATIO: setCompressorRatio(ae->fxb, final_val); break;
		case PARAM_COMPRESSION_BOOST: setCompressorBoost(ae->fxb, final_val); break;
		case PARAM_COMPRESSION_KNEE: setCompressorKnee(ae->fxb, final_val); break;
		case PARAM_MODULATION_ON: setModulationOn(ae->fxb, final_val); break;
		case PARAM_MODULATION_TYPE: setModulationType(ae->fxb, final_val); break;
		case PARAM_MODULATION_LEVEL: setModulationLevel(ae->fxb, final_val); break;
		case PARAM_MODULATION_CHORUS_DT: setModulationChorusDelayTime(ae->fxb, final_val); break;
		case PARAM_MODULATION_CHORUS_FBK: setModulationChorusFeedback(ae->fxb, final_val); break;
		case PARAM_MODULATION_CHORUS_HPF: setModulationChorusHpf(ae->fxb, final_val); break;
		case PARAM_MODULATION_DELAY_FBKFILT: setModulationDelayFeedbackFilter(ae->fxb, final_val); break;
		case PARAM_MODULATION_RATE: setModulationRate(ae->fxb, final_val); break;
		case PARAM_MODULATION_DEPTH: setModulationDepth(ae->fxb, final_val); break;
		case PARAM_MODULATION_TREMOLO: setModulationTremolo(ae->fxb, final_val); break;
		case PARAM_DELAY_ON: setDelayOn(ae->fxb, final_val); break;
		case PARAM_DELAY_MODE: setDelayMode(ae->fxb, final_val); break;
		case PARAM_DELAY_PRELPF: setDelayPreLp(ae->fxb, final_val); break;
		case PARAM_DELAY_LEVEL: setDelayLevel(ae->fxb, final_val); break;
		case PARAM_DELAY_TIME: setDelayTime(ae->fxb, final_val); break;
		case PARAM_DELAY_FBK: setDelayFeedback(ae->fxb, final_val); break;
		case PARAM_DELAY_FBKFILT: setDelayFeedbackFilter(ae->fxb, final_val); break;
		case PARAM_REVERB_TYPE: setReverbType(final_val); break;
		case PARAM_REVERB_CHARACTER: setReverbCharacter(final_val); break;
		case PARAM_REVERB_PRELPF: setReverbPreLpf(final_val); break;
		case PARAM_REVERB_LEVEL: setReverbLevel(final_val); break;
		case PARAM_REVERB_TIME: setReverbTime(final_val); break;
		case PARAM_REVERB_DELAY_FBK: setReverbDelayFeedback(final_val); break;
		case PARAM_REVERB_PREDELAY: setReverbPreDelay(final_val); break;
		case PARAM_EQ_LOWEST_GAIN: setEQLowestBandGain(final_val); break;
		case PARAM_EQ_LOWER_GAIN: setEQLowerBandGain(final_val); break;
		case PARAM_EQ_HIGHER_GAIN: setEQHigherBandGain(final_val); break;
		case PARAM_EQ_HIGHEST_GAIN: setEQHighestBandGain(final_val); break;
		case PARAM_EQ_LOWEST_FREQ: setEQLowestBandFrequency(final_val); break;
		case PARAM_EQ_LOWER_FREQ: setEQLowerBandFrequency(final_val); break;
		case PARAM_EQ_HIGHER_FREQ: setEQHigherBandFrequency(final_val); break;
		case PARAM_EQ_HIGHEST_FREQ: setEQHighestBandFrequency(final_val); break;
		case PARAM_EQ_LOWMID_Q: setEQLowMidQ(final_val); break;
		case PARAM_EQ_HIGHMID_Q: setEQHighMidQ(final_val); break;
		case PARAM_MIXER_LOWCUT_FILTER_FREQ: setMixerLowCutFilterFrequency(ae->fxb, final_val); break;
		case PARAM_MIXER_HIGHCUT_FILTER_FREQ: setMixerHighCutFilterFrequency(ae->fxb, final_val); break;
		case PARAM_MIXER_INPUT_GAIN: setMixerInputGain(ae->fxb, final_val); break;
		case PARAM_MIXER_OUTPUT_LEVEL: setMixerOutputLevel(ae->fxb, final_val); break;
		case PARAM_MIXER_PAN: setMixerPan(ae->fxb, final_val); break;
		case PARAM_MIXER_REVERB_SEND: setMixerReverbSend(ae->fxb, final_val); break;

		case PARAM_VOICE_AMP_ENV_ATTACK: setAmpEnvAttack(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_AMP_ENV_DECAY: setAmpEnvDecay(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_AMP_ENV_RELEASE: setAmpEnvRelease(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_FREQUENCY: setFilterFrequency(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_RESONANCE: setFilterResonance(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_DETUNE_COURSE: setDetuneCourse(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_DETUNE_FINE: setDetuneFine(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_VIBRATE_RATE: setVibrateRate(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_VIBRATE_DEPTH: setVibrateDepth(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_VIBRATE_DELAY: setVibrateDelay(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_CHANNEL_VOLUME: setChannelVolume(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_PORTAMENTO_TIME: setPortamentoTime(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_PAN: setPan(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_PITCH_BEND_ST: setPitchBendSemitones(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_REVERB_SEND: setReverbSend(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_TYPE: setFilterType(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_ATTACK: setFilterAttack(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_DECAY: setFilterDecay(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_RELEASE: setFilterRelease(ae->str, ae->voice_index, final_val); break;

		case PARAM_VELOCITY: setVelocity(ae->str, final_val); break;
		case PARAM_PITCH:
			//fix: add inverse option for pitch
			unsigned int scaled_pitch = (val * 0x4000) / scaler; //yes... we scale up... resolution limited
			midi.sendPitch(ae->output, ae->channel, scaled_pitch);
			break;
		case PARAM_CC:
			midi.sendCC(ae->output, ae->channel, ae->cc, final_val);
			break;
	}
}
*/

void Synth::setVelocity(unsigned char str, unsigned char v)
{
	velocity[str] = v;
}
/*
void Synth::setPitch(unsigned char output, unsigned char str, unsigned int p)
{
	if(output == SYNTH)
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
			midi.sendPitch(SYNTH, current_setting.voices[str].at(i).channel, p);
	}
	else
		midi.sendPitch(MIDI_OUT, current_setting.string_midi_out_channel[str], p);
}

bool Synth::GetCurrentSetting(PSYNTH_SETTING pSynthsetting)
{
	*pSynthsetting = current_setting;
	return true;
}
*/

void Synth::setDragCenterSynthEffect(unsigned char synth_param, unsigned char fxb, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).fxb == fxb))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
		}
	}
}

void Synth::setDragCenterSynthVoiceParam(unsigned char synth_param, unsigned char str, unsigned char voice_index, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).str == str) && (control_list->at(i).voice_index == voice_index))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
		}
	}
}

void Synth::setDragCenterVelocity(int str, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_VELOCITY) && (control_list->at(i).str == str))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
		}
	}
}

void Synth::setDragCenterSynthPitch(unsigned char str, unsigned char voice_index, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		int chan = current_setting.voices[str].at(voice_index).channel;
		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_PITCH) && (control_list->at(i).output == SYNTH) && (control_list->at(i).channel == chan))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
		}
	}
}

void Synth::setDragCenterMidiPitch(unsigned char channel, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_PITCH) && (control_list->at(i).output == MIDI_OUT) && (control_list->at(i).channel == channel))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
		}
	}
}

void Synth::setDragCenterCC(unsigned char output, unsigned char channel, unsigned int cc, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_CC) && (control_list->at(i).output == output) && (control_list->at(i).channel == channel) && (control_list->at(i).cc == cc))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
		}
	}
}

void Synth::assignMidiStopSound(unsigned char str, unsigned char cc, unsigned char val)
{
	struct cc new_cc;
	new_cc.cc_num = cc;
	new_cc.value = val;
	current_setting.stop_sound_cmds[str].push_back(new_cc);
}
