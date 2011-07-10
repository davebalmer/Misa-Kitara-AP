/*


PRESET BUILDER CODE - see Synth::run()


*/
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "Synth.h"
#include <string>

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
	//delete any previously loaded voices
	for(int s = 0; s < 6; s++)
	{
		current_setting.voices[s].clear();
		current_setting.string_midi_out_channel[s] = -1;
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
		setMixerReverbSend(i, 0);
	}
}

void Synth::run(void)
{
	for(int i = 0; i < 29; i++)
	{
		std::stringstream filename;
		loadTestPreset(i);

		filename << "./presets/";

		switch(i)
		{
			case 0: filename << "saw"; break;
			case 1: filename << "sin"; break;
			case 2: filename << "square"; break;
			case 3: filename << "triangle"; break;
			case 4: filename << "xenoxpad"; break;
			case 5: filename << "riverpad"; break;
			case 6: filename << "loungepad"; break;
			case 7: filename << "chinapad"; break;
			case 8: filename << "spiritchoir"; break;
			case 9: filename << "strings"; break;
			case 10: filename << "systrings"; break;
			case 11: filename << "dronepad"; break;
			case 12: filename << "tjampad"; break;
			case 13: filename << "southpad"; break;
			case 14: filename << "jazzistik"; break;
			case 15: filename << "sekentar"; break;
			case 16: filename << "leadman"; break;
			case 17: filename << "beatporte"; break;
			case 18: filename << "moglead"; break;
			case 19: filename << "windlead"; break;
			case 20: filename << "patlead"; break;
			case 21: filename << "sybrass1"; break;
			case 22: filename << "sybrass2"; break;
			case 23: filename << "sitara"; break;
			case 24: filename << "dronepad_var"; break;
			case 25: filename << "saw_var"; break;
			case 26: filename << "sin_var"; break;
			case 27: filename << "square_var"; break;
			case 28: filename << "triangle_var"; break;
		}

		filename << ".mz";

//		filename << "./presets/" << i << ".mz";
		savePresetToFile(&current_setting, filename.str());
//std::cout << filename.str() << std::endl << std::flush;
	}
}

void Synth::loadTestPreset(int wave)
{
	resetSettings();
	setMasterVolume(100);
	for(int s = 0; s < 6; s++)
	{
		insertNewVoice(s, wave);
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

//	assignSynthEffect(VARIATION, PARAM_DISTORTION_DRIVE, 0, false, 1, 120);

//	assignSynthEffect(BALL_Y, PARAM_DISTORTION_DRIVE, 0, true, 0, 0);

//	setDistortionOn(0, true);
//	setDistortionType(0, 4);
//	setDistortionDrive(0, 120);
//	setDistortionLevel(0, 44);
//	setDistortionTone(0, 70);

	//notice this is incomplete
//	setModulationType(0, 8);
//	setModulationLevel(0, 64);
}

void Synth::loadPresetFromFile(std::string filename)
{
	resetSettings();

	std::ifstream ifs(("./" + filename).c_str(), std::ios::binary);

	ifs.read((char *) &current_setting.master_volume, sizeof(int));
	setMasterVolume(current_setting.master_volume);
	ifs.read((char *) &current_setting.lfo_rate, sizeof(int));
	setLfoRate(current_setting.lfo_rate);
	ifs.read((char *) &current_setting.lfo_frequency_depth, sizeof(int));
	setLfoFrequencyDepth(current_setting.lfo_frequency_depth);
	ifs.read((char *) &current_setting.lfo_amp_depth, sizeof(int));
	setLfoAmpDepth(current_setting.lfo_amp_depth);
	ifs.read((char *) &current_setting.lfo_pitch_depth, sizeof(int));
	setLfoPitchDepth(current_setting.lfo_pitch_depth);

	ifs.read((char *) &current_setting.equalizer, sizeof(struct eq));
	setEQLowestBandGain(current_setting.equalizer.lowest.gain);
	setEQLowerBandGain(current_setting.equalizer.lower.gain);
	setEQHigherBandGain(current_setting.equalizer.higher.gain);
	setEQHighestBandGain(current_setting.equalizer.highest.gain);
	setEQLowestBandFrequency(current_setting.equalizer.lowest.frequency);
	setEQLowerBandFrequency(current_setting.equalizer.lower.frequency);
	setEQHigherBandFrequency(current_setting.equalizer.higher.frequency);
	setEQHighestBandFrequency(current_setting.equalizer.highest.frequency);
	setEQLowMidQ(current_setting.equalizer.low_mid_q);
	setEQHighMidQ(current_setting.equalizer.high_mid_q);

	ifs.read((char *) &current_setting.reverb, sizeof(struct effect_reverb));
	setReverbType(current_setting.reverb.type);
	setReverbCharacter(current_setting.reverb.character);
	setReverbPreLpf(current_setting.reverb.pre_lpf);
	setReverbLevel(current_setting.reverb.level);
	setReverbTime(current_setting.reverb.time);
	setReverbDelayFeedback(current_setting.reverb.delay_feedback);
	setReverbPreDelay(current_setting.reverb.pre_delay_time);

	for(int i = 0; i < 2; i++)
	{
		ifs.read((char *) &current_setting.fx_block[i], sizeof(struct fxb));

		setDistortionOn(i, current_setting.fx_block[i].distortion.on);
		setDistortionType(i, current_setting.fx_block[i].distortion.type);
		setDistortionLevel(i, current_setting.fx_block[i].distortion.level);
		setDistortionDrive(i, current_setting.fx_block[i].distortion.drive);
		setDistortionTone(i, current_setting.fx_block[i].distortion.tone);

		setCompressorOn(i, current_setting.fx_block[i].compressor.on);
		setCompressorAttack(i, current_setting.fx_block[i].compressor.attack);
		setCompressorRelease(i, current_setting.fx_block[i].compressor.release);
		setCompressorThreshold(i, current_setting.fx_block[i].compressor.threshold);
		setCompressorRatio(i, current_setting.fx_block[i].compressor.ratio);
		setCompressorBoost(i, current_setting.fx_block[i].compressor.boost);
		setCompressorKnee(i, current_setting.fx_block[i].compressor.knee);

		setModulationOn(i, current_setting.fx_block[i].modulation.on);
		setModulationType(i, current_setting.fx_block[i].modulation.type);
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
		setMixerReverbSend(i, current_setting.fx_block[i].mixer.reverb_send);
	}

	unsigned int vector_size;

	//read voice vectors
	for(int i = 0; i < 6; i++)
	{
		vector_size = current_setting.voices[i].size();
		ifs.read((char *) &vector_size, sizeof(unsigned int));
		current_setting.voices[i].resize(vector_size);
		ifs.read((char *) &current_setting.voices[i][0], sizeof(struct voice) *vector_size);

		for(int j = 0; j < current_setting.voices[i].size(); j++)
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
		}
	}

	vector_size = current_setting.AE_touch_x.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_touch_x.resize(vector_size);
	ifs.read((char *) &current_setting.AE_touch_x[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_touch_y.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_touch_y.resize(vector_size);
	ifs.read((char *) &current_setting.AE_touch_y[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_drag_x.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_drag_x.resize(vector_size);
	ifs.read((char *) &current_setting.AE_drag_x[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_drag_y.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_drag_y.resize(vector_size);
	ifs.read((char *) &current_setting.AE_drag_y[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_ball_x.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_ball_x.resize(vector_size);
	ifs.read((char *) &current_setting.AE_ball_x[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_ball_y.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_ball_y.resize(vector_size);
	ifs.read((char *) &current_setting.AE_ball_y[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_touch_x_0.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_0.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_0[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_touch_x_1.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_1.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_1[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_touch_x_2.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_2.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_2[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_touch_x_3.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_3.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_3[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_touch_x_4.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_4.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_4[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_touch_x_5.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_touch_x_5.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_touch_x_5[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_drag_x_0.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_0.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_0[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_drag_x_1.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_1.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_1[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_drag_x_2.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_2.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_2[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_drag_x_3.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_3.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_3[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_drag_x_4.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_4.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_4[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_string_drag_x_5.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_string_drag_x_5.resize(vector_size);
	ifs.read((char *) &current_setting.AE_string_drag_x_5[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = current_setting.AE_variation.size();
	ifs.read((char *) &vector_size, sizeof(unsigned int));
	current_setting.AE_variation.resize(vector_size);
	ifs.read((char *) &current_setting.AE_variation, sizeof(struct assignable_effect) * vector_size);

	ifs.close();

	std::cout << "Preset file " << filename << " loaded." << std::endl << std::flush;

//	printCurrentSettings();
}

void Synth::savePresetToFile(struct synth_setting *s, std::string filepath)
{
	std::ofstream ofs(filepath.c_str(), std::ios::binary);

	ofs.write((char *) &s->master_volume, sizeof(int));
	ofs.write((char *) &s->lfo_rate, sizeof(int));
	ofs.write((char *) &s->lfo_frequency_depth, sizeof(int));
	ofs.write((char *) &s->lfo_amp_depth, sizeof(int));
	ofs.write((char *) &s->lfo_pitch_depth, sizeof(int));

	ofs.write((char *) &s->equalizer, sizeof(struct eq));
	ofs.write((char *) &s->reverb, sizeof(struct effect_reverb));
	ofs.write((char *) &s->fx_block[0], sizeof(struct fxb));
	ofs.write((char *) &s->fx_block[1], sizeof(struct fxb));

	unsigned int vector_size;

	//write voice vectors
	for(int i = 0; i < 6; i++)
	{
		vector_size = s->voices[i].size();
		ofs.write((char *) &vector_size, sizeof(unsigned int));
		ofs.write((char *) &s->voices[i][0], sizeof(struct voice) * vector_size);
	}

	vector_size = s->AE_touch_x.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_touch_x[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_touch_y.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_touch_y[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_drag_x.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_drag_x[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_drag_y.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_drag_y[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_ball_x.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_ball_x[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_ball_y.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_ball_y[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_touch_x_0.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_touch_x_0[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_touch_x_1.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_touch_x_1[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_touch_x_2.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_touch_x_2[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_touch_x_3.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_touch_x_3[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_touch_x_4.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_touch_x_4[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_touch_x_5.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_touch_x_5[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_drag_x_0.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_drag_x_0[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_drag_x_1.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_drag_x_1[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_drag_x_2.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_drag_x_2[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_drag_x_3.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_drag_x_3[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_drag_x_4.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_drag_x_4[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_string_drag_x_5.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_string_drag_x_5[0], sizeof(struct assignable_effect) * vector_size);

	vector_size = s->AE_variation.size();
	ofs.write((char *) &vector_size, sizeof(unsigned int));
	ofs.write((char *) &s->AE_variation, sizeof(struct assignable_effect) * vector_size);

	ofs.close();
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

	for(new_channel = 0; new_channel < 30; new_channel++)
		if(find(channel_list.begin(), channel_list.end(), new_channel) == channel_list.end())
			break;

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
	setReverbSend(str, v, 0);

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
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;

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
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;

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
