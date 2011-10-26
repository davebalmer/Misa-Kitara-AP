#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "Synth.h"
#include <string>
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

extern std::string working_directory;
extern char *VERSION_STRING;

Synth::Synth()
{
	resetSettings();
	mode_ringing_notes = false;

	srand(time(NULL));
	setMasterVolume(64);
	setSynthesizerVolume(100); //stop clipping
	for(int s = 0; s < 6; s++)
		string_note[s] = 0;

	readWavetable();

	midi.sendAllNotesOff(); //in case AP incorrectly shut down and the notes are not turned off inside the chip
}

Synth::~Synth()
{
//	delete presets and voices here
	resetSettings();
}

void Synth::readWavetable(void)
{
	std::ifstream ifs(std::string(working_directory+"/wavetable.txt").c_str());
	wavetable.clear();
	if(ifs.is_open())
	{
		std::string s;
		while(getline(ifs, s))
		{
			if(s.length() > 0)
			{
				std::string str;
				if(s[s.size()-1] == '\r')
					str = s.substr(0,s.size()-1);
				else
					str = s;

				wavetable.push_back(str);
			}
		}
	}

	ifs.close();
}

void Synth::resetSettings(void)
{
	int temp_volume = master_volume;
	setMasterVolume(0);

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

	memset(&current_setting.equalizer, 0, sizeof(struct eq));
	memset(&current_setting.reverb, 0, sizeof(struct effect_reverb));
	memset(&current_setting.fx_block[0], 0, sizeof(struct fxb));
	memset(&current_setting.fx_block[1], 0, sizeof(struct fxb));

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
	midi.sendNRPN(SYNTH, 0, 0x3A, 0x7F, 0); //fx block 1
	midi.sendNRPN(SYNTH, 0, 0x3B, 0x7F, 0); //fx block 2
	midi.sendGSReset();

	//HQ distortion algorithms need to be reset manually
	setDistortionOn(0, false);
	setDistortionOn(1, false);

	for(int i = 0; i < 32; i++)
	{
		midi.sendCC(SYNTH, i, 121, 0);	//reset all controllers
		midi.sendCC(SYNTH, i, 126, 0);	//channel mono on = 0
		midi.sendCC(SYNTH, i, 65, 0x7F);	//channel portamento on = 0x7F
		midi.sendCC(SYNTH, i, 120, 0);	//turn all sound off
		midi.sendCC(SYNTH, i, 123, 0);	//turn all notes off

		//unassign effects inserts
		int chan = midi.setMidiPort(i);
		unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x40+chan, 0x22, 0x00, 0x00, 0xF7};
		midi.sendSysex(SYNTH, sysex, 11);
	}

	for(int i = 0; i < 6; i++)
		velocity[i] = 127;

	//set all the default values internally
//	setMasterVolume(0x7F);
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
	//setReverbCharacter(0x04);
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
		setDistortionNoiseReduction(i, 0);
		setDistortionBooster(i, 0);

		setCompressorOn(i, false);
		setCompressorAttack(i, 0);
		setCompressorRelease(i, 0);
		setCompressorThreshold(i, 0);
		setCompressorRatio(i, 0);
		setCompressorBoost(i, 0);
		setCompressorKnee(i, 0);

		setModulationType(i, 1);
		setModulationOn(i, 0);
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

		setMixerLowCutFilterFrequency(i, 0); //fix: datasheet says opposite
		setMixerHighCutFilterFrequency(i, 0x7F);
		setMixerInputGain(i, 0x40);
		setMixerOutputLevel(i, 0x7F);
		setMixerPan(i, 0x40);
		setMixerReverbSend(i, 0);
	}

	//this sets all the internal "parts" as normal sound (not drumset)
	unsigned char set_sound_cmd_p1[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x10, 0x15, 0x00, 0x00, 0xF7};
	unsigned char set_sound_cmd_p2[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x50, 0x10, 0x15, 0x00, 0x00, 0xF7};
	for(int p = 0; p < 16; p++)
	{
		set_sound_cmd_p1[6] = 0x10+p;
		set_sound_cmd_p2[6] = 0x10+p;
		midi.sendSysex(SYNTH, set_sound_cmd_p1, 11);
		midi.sendSysex(SYNTH, set_sound_cmd_p2, 11);
	}

	setMasterVolume(temp_volume);

	currentPresetName = "";
}

/*void Synth::loadPresetFromFile(std::string filename)
{
	std::ifstream ifs((working_directory + "/presets/" + filename).c_str(), std::ios::binary);

	if(!ifs.is_open())
	{
		std::cout << "Preset file could not be opened." << std::endl << std::flush;
		return;
	}

	resetSettings();
	int temp_volume = master_volume;
	setMasterVolume(0);

	try
	{

	ifs.read((char *) &current_setting.tuning, 6);
	ifs.read((char *) &current_setting.string_midi_out_channel, 6);

	ifs.read((char *) &current_setting.master_volume, sizeof(int));
	//setMasterVolume(current_setting.master_volume); //overridden by preset-independent volume
	ifs.read((char *) &current_setting.lfo_rate, sizeof(int));
	setLfoRate(current_setting.lfo_rate);
	ifs.read((char *) &current_setting.lfo_frequency_depth, sizeof(int));
	setLfoFrequencyDepth(current_setting.lfo_frequency_depth);
	ifs.read((char *) &current_setting.lfo_amp_depth, sizeof(int));
	setLfoAmpDepth(current_setting.lfo_amp_depth);
	ifs.read((char *) &current_setting.lfo_pitch_depth, sizeof(int));
	setLfoPitchDepth(current_setting.lfo_pitch_depth);

	ifs.read((char *) &current_setting.equalizer, sizeof(struct eq));
	setEQOn(current_setting.equalizer.on);
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
		setMixerPan(i, current_setting.fx_block[i].mixer.pan);
		setMixerReverbSend(i, current_setting.fx_block[i].mixer.reverb_send);
	}

	unsigned int vector_size;

	//read voice vectors
	for(int i = 0; i < 6; i++)
	{
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
			setPitchWheel(i, j, current_setting.voices[i].at(j).pitch_wheel);
			setVoiceVelocity(i, j, current_setting.voices[i].at(j).velocity);
			setReverbSend(i, j, current_setting.voices[i].at(j).reverb_send);
			setFilterType(i, j, current_setting.voices[i].at(j).filter_type);
			setFilterAttack(i, j, current_setting.voices[i].at(j).filter_attack);
			setFilterDecay(i, j, current_setting.voices[i].at(j).filter_decay);
			setFilterRelease(i, j, current_setting.voices[i].at(j).filter_release);

			if(current_setting.voices[i].at(j).insert_fx_block_on[0])
				setFxBlockOn(i, j, 0, true);
			else
			if(current_setting.voices[i].at(j).insert_fx_block_on[1])
				setFxBlockOn(i, j, 1, true);
		}
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

	for(int i = 0; i < 6; i++)
	{
		ifs.read((char *) &vector_size, sizeof(unsigned int));
		current_setting.stop_sound_cmds[i].resize(vector_size);
		ifs.read((char *) &current_setting.stop_sound_cmds[i][0], sizeof(struct cc) * vector_size);
	}

	ifs.close();
	}
	catch(const std::ifstream::failure& e)
	{
		std::cout << "loadPresetFromFile() failed: " << e.what() << std::endl << std::flush;
		setMasterVolume(temp_volume);
	}

	setMasterVolume(temp_volume);
	std::cout << "Preset file " << working_directory << "/presets/" << filename << " loaded." << std::endl << std::flush;
}
*/

void Synth::loadPresetFromFile(std::string filename)
{
	std::string file = working_directory + "/presets/" + filename + ".mz";
	TiXmlDocument doc(file.c_str());
	if(!doc.LoadFile())
	{
		std::cout << "Preset file " + file + " could not be opened." << std::endl << std::flush;
		return;
	}

	resetSettings();

	currentPresetName = filename;

	//temp mute while loading
	int temp_volume = master_volume;
	setMasterVolume(0);

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
			if(e->Attribute("on")) setEQOn(atoi(e->Attribute("on")));
			if(e->Attribute("low_mid_q")) setEQLowMidQ(atoi(e->Attribute("low_mid_q")));
			if(e->Attribute("high_mid_q")) setEQHighMidQ(atoi(e->Attribute("high_mid_q")));

			for(TiXmlElement *e2 = e->FirstChildElement(); e2 != NULL; e2 = e2->NextSiblingElement())
			{
				if(std::string(e2->Value()) != "eq_band") continue;
				if(!e2->Attribute("type")) continue;
				if(std::string(e2->Attribute("type")) == "lowest")
				{
					if(e2->Attribute("gain")) setEQLowestBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) setEQLowestBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(std::string(e2->Attribute("type")) == "lower")
				{
					if(e2->Attribute("gain")) setEQLowerBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) setEQLowerBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(std::string(e2->Attribute("type")) == "higher")
				{
					if(e2->Attribute("gain")) setEQHigherBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) setEQHigherBandFrequency(atoi(e2->Attribute("frequency")));
				}
				else
				if(std::string(e2->Attribute("type")) == "highest")
				{
					if(e2->Attribute("gain")) setEQHighestBandGain(atoi(e2->Attribute("gain")));
					if(e2->Attribute("frequency")) setEQHighestBandFrequency(atoi(e2->Attribute("frequency")));
				}
			}
		}
		else
		if(e_str == "reverb")
		{
			if(e->Attribute("type")) setReverbType(atoi(e->Attribute("type")));
			//if(e->Attribute("character")) setReverbCharacter(atoi(e->Attribute("character")));
			if(e->Attribute("pre_lpf")) setReverbPreLpf(atoi(e->Attribute("pre_lpf")));
			if(e->Attribute("level")) setReverbLevel(atoi(e->Attribute("level")));
			if(e->Attribute("time")) setReverbTime(atoi(e->Attribute("time")));
			if(e->Attribute("delay_feedback")) setReverbDelayFeedback(atoi(e->Attribute("delay_feedback")));
			if(e->Attribute("pre_delay_time")) setReverbPreDelay(atoi(e->Attribute("pre_delay_time")));
		}
		else
		if(e_str == "bitcrusher")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) setBitCrusherOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("bitresolution")) setBitcrusherBitResolution(fxb, atoi(e->Attribute("bitresolution")));
				//if(e->Attribute("brightness")) setBitcrusherBrightness(fxb, atoi(e->Attribute("brightness")));
				if(e->Attribute("downsampling")) setBitcrusherDownsampling(fxb, atoi(e->Attribute("downsampling")));
			}
		}
		else
		if(e_str == "distortion")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) setDistortionOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("type")) setDistortionType(fxb, atoi(e->Attribute("type")));
				if(e->Attribute("level")) setDistortionLevel(fxb, atoi(e->Attribute("level")));
				if(e->Attribute("drive")) setDistortionDrive(fxb, atoi(e->Attribute("drive")));
				if(e->Attribute("tone")) setDistortionTone(fxb, atoi(e->Attribute("tone")));
				if(e->Attribute("booster")) setDistortionBooster(fxb, atoi(e->Attribute("booster")));
			}
		}
		else
		if(e_str == "compression")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) setCompressorOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("attack")) setCompressorAttack(fxb, atoi(e->Attribute("attack")));
				if(e->Attribute("release")) setCompressorRelease(fxb, atoi(e->Attribute("release")));
				if(e->Attribute("threshold")) setCompressorThreshold(fxb, atoi(e->Attribute("threshold")));
				if(e->Attribute("ratio")) setCompressorRatio(fxb, atoi(e->Attribute("ratio")));
				if(e->Attribute("boost")) setCompressorBoost(fxb, atoi(e->Attribute("boost")));
				if(e->Attribute("knee")) setCompressorKnee(fxb, atoi(e->Attribute("knee")));
			}
		}
		else
		if(e_str == "modulation")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("type")) setModulationType(fxb, atoi(e->Attribute("type")));
				if(e->Attribute("on")) setModulationOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("level")) setModulationLevel(fxb, atoi(e->Attribute("level")));
				if(e->Attribute("chorus_delay_time")) setModulationChorusDelayTime(fxb, atoi(e->Attribute("chorus_delay_time")));
				if(e->Attribute("chorus_feedback")) setModulationChorusFeedback(fxb, atoi(e->Attribute("chorus_feedback")));
				if(e->Attribute("chorus_hpf")) setModulationChorusHpf(fxb, atoi(e->Attribute("chorus_hpf")));
				if(e->Attribute("delay_feedback_filter")) setModulationDelayFeedbackFilter(fxb, atoi(e->Attribute("delay_feedback_filter")));
				if(e->Attribute("rate")) setModulationRate(fxb, atoi(e->Attribute("rate")));
				if(e->Attribute("depth")) setModulationDepth(fxb, atoi(e->Attribute("depth")));
				if(e->Attribute("tremolo")) setModulationTremolo(fxb, atoi(e->Attribute("tremolo")));
			}
		}
		else
		if(e_str == "delay")
		{
			int fxb = 0;
			if(e->Attribute("fxblock"))
			{
				fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("on")) setDelayOn(fxb, atoi(e->Attribute("on")));
				if(e->Attribute("mode")) setDelayMode(fxb, atoi(e->Attribute("mode")));
				if(e->Attribute("pre_lp")) setDelayPreLp(fxb, atoi(e->Attribute("pre_lp")));
				if(e->Attribute("level")) setDelayLevel(fxb, atoi(e->Attribute("level")));
				if(e->Attribute("time")) setDelayTime(fxb, atoi(e->Attribute("time")));
				if(e->Attribute("feedback")) setDelayFeedback(fxb, atoi(e->Attribute("feedback")));
				if(e->Attribute("feedback_filter")) setDelayFeedbackFilter(fxb, atoi(e->Attribute("feedback_filter")));
			}
		}
		else
		if(e_str == "mixer")
		{
			int fxb = 0;
			if(e->Attribute("fxblock")) 
			{
				int fxb = atoi(e->Attribute("fxblock"));
				if(e->Attribute("low_cut_filter_frequency")) setMixerLowCutFilterFrequency(fxb, atoi(e->Attribute("low_cut_filter_frequency")));
				if(e->Attribute("high_cut_filter_frequency")) setMixerHighCutFilterFrequency(fxb, atoi(e->Attribute("high_cut_filter_frequency")));
				if(e->Attribute("input_gain")) setMixerInputGain(fxb, atoi(e->Attribute("input_gain")));
				if(e->Attribute("output_level")) setMixerOutputLevel(fxb, atoi(e->Attribute("output_level")));
				if(e->Attribute("pan")) setMixerPan(fxb, atoi(e->Attribute("pan")));
				if(e->Attribute("reverb_send")) setMixerReverbSend(fxb, atoi(e->Attribute("reverb_send")));
			}
		}
		else
		if(e_str == "control")
		{
			if(atoi(e->Attribute("type")) == 100) //fix: this nasty hack immediately!
			{
				if((e->Attribute("string")) && (e->Attribute("cc")) && (e->Attribute("value")))
					assignMidiStopSound(atoi(e->Attribute("string")), atoi(e->Attribute("cc")), atoi(e->Attribute("value")));
			}
			else
			{
				std::vector<struct assignable_effect> *control = getControlList(atoi(e->Attribute("type")));
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
			if((e->Attribute("string")) && (e->Attribute("wave_id")))
			{
				int wi = getWavetableIndex(e->Attribute("wave_id"));
				if(wi != -1)
				{
					int i = atoi(e->Attribute("string"));
					int j = current_setting.voices[i].size();

					//insertNewVoice(i, atoi(e->Attribute("wavetable_index")));
					insertNewVoice(i, wi);
					//setChannel(i, j, atoi(e->Attribute("channel")));
					//setWave(i, j, atoi(e->Attribute("wavetable_index")));
					if(e->Attribute("amplitude_attack")) setAmpEnvAttack(i, j, atoi(e->Attribute("amplitude_attack")));
					if(e->Attribute("amplitude_decay")) setAmpEnvDecay(i, j, atoi(e->Attribute("amplitude_decay")));
					if(e->Attribute("amplitude_release")) setAmpEnvRelease(i, j, atoi(e->Attribute("amplitude_release")));
					if(e->Attribute("filter_frequency")) setFilterFrequency(i, j, atoi(e->Attribute("filter_frequency")));
					if(e->Attribute("filter_resonance")) setFilterResonance(i, j, atoi(e->Attribute("filter_resonance")));
					if(e->Attribute("detune_course")) setDetuneCourse(i, j, atoi(e->Attribute("detune_course")));
					if(e->Attribute("detune_fine")) setDetuneFine(i, j, atoi(e->Attribute("detune_fine")));
					if(e->Attribute("vibrate_rate")) setVibrateRate(i, j, atoi(e->Attribute("vibrate_rate")));
					if(e->Attribute("vibrate_depth")) setVibrateDepth(i, j, atoi(e->Attribute("vibrate_depth")));
					if(e->Attribute("vibrate_delay")) setVibrateDelay(i, j, atoi(e->Attribute("vibrate_delay")));
					if(e->Attribute("channel_volume")) setChannelVolume(i, j, atoi(e->Attribute("channel_volume")));
					if(e->Attribute("portamento_time")) setPortamentoTime(i, j, atoi(e->Attribute("portamento_time")));
					if(e->Attribute("pan")) setPan(i, j, atoi(e->Attribute("pan")));
					if(e->Attribute("pitch_bend_semitones")) setPitchBendSemitones(i, j, atoi(e->Attribute("pitch_bend_semitones")));
					if(e->Attribute("pitch_wheel")) setPitchWheel(i, j, atoi(e->Attribute("pitch_wheel")));
					if(e->Attribute("velocity")) setVoiceVelocity(i, j, atoi(e->Attribute("velocity")));
					if(e->Attribute("reverb_send")) setReverbSend(i, j, atoi(e->Attribute("reverb_send")));
					if(e->Attribute("filter_type")) setFilterType(i, j, atoi(e->Attribute("filter_type")));
					if(e->Attribute("filter_attack")) setFilterAttack(i, j, atoi(e->Attribute("filter_attack")));
					if(e->Attribute("filter_decay")) setFilterDecay(i, j, atoi(e->Attribute("filter_decay")));
					if(e->Attribute("filter_release")) setFilterRelease(i, j, atoi(e->Attribute("filter_release")));

					if((e->Attribute("fxb0_on")) && (atoi(e->Attribute("fxb0_on"))))
						setFxBlockOn(i, j, 0, true);
					else
					if((e->Attribute("fxb1_on")) && (atoi(e->Attribute("fxb1_on"))))
						setFxBlockOn(i, j, 1, true);
				}
			}
		}

	}

	setMasterVolume(temp_volume);

	std::cout << "Preset file " << working_directory << "/presets/" << filename << ".mz loaded." << std::endl << std::flush;
}

int Synth::getWavetableIndex(std::string wave)
{
	for(int i = 0; i < wavetable.size(); i++)
		if(wavetable.at(i) == wave)
			return i;

	std::cout << "Wavetable index not found for: " << wave << "." << std::endl << std::flush;
	return -1;
}

void Synth::deletePresetFile(std::string filename)
{
	if(filename == "[default]")
		std::cout << "Cannot delete [default] preset." << std::endl << std::flush;
	else
	if(filename == "[new]")
		std::cout << "Cannot delete [new] preset." << std::endl << std::flush;
	else
	{
		system("mount -o remount,rw /usr");

		std::string filepath = working_directory + "/presets/" + filename + ".mz";
		std::string cmd = "rm -f " + filepath;
		system(cmd.c_str());

		system("mount -o remount,ro /usr");

		std::cout << "Preset file " << filepath << " deleted." << std::endl << std::flush;
	}
}

/*
void Synth::savePresetToFile(struct synth_setting *s, std::string filepath)
{
	std::ofstream ofs(filepath.c_str(), std::ios::binary);

	ofs.exceptions(ofs.badbit | ofs.failbit);

	try
	{

	ofs.write((char *) &s->tuning, 6);
	ofs.write((char *) &s->string_midi_out_channel, 6);

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
	ofs.write((char *) &s->AE_variation[0], sizeof(struct assignable_effect) * vector_size);

	for(int i = 0; i < 6; i++)
	{
		vector_size = s->stop_sound_cmds[i].size();
		ofs.write((char *) &vector_size, sizeof(unsigned int));
		ofs.write((char *) &s->stop_sound_cmds[i][0], sizeof(struct cc) * vector_size);
	}

	ofs.flush();
	ofs.close();
	}
	catch(const std::ofstream::failure& e)
	{
		std::cout << "savePresetToFile() failed: " << e.what() << std::endl << std::flush;
	}

	std::cout << "File " << filepath << " saved." << std::endl << std::flush;
	system("sync");
}
*/
void Synth::savePresetToFile(struct synth_setting *s, std::string filename)
{
	if(filename == "[default]")
	{
		std::cout << "Cannot overwrite [default] preset." << std::endl << std::flush;
		return;
	}
	else
	if(filename == "[new]")
	{
		std::cout << "Cannot overwrite [new] preset." << std::endl << std::flush;
		return;
	}

	TiXmlDocument doc;
	TiXmlDeclaration *decl;
	TiXmlElement *root, *element, *element2;

	decl = new TiXmlDeclaration("1.0", "", "");

	doc.LinkEndChild(decl);

	//root node
	root = new TiXmlElement("preset");
	doc.LinkEndChild(root);

	//identification
	element = new TiXmlElement("kitara_id");
	element->SetAttribute("ap", VERSION_STRING);
	root->LinkEndChild(element);

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
	//element->SetAttribute("character", s->reverb.character);
	element->SetAttribute("pre_lpf", s->reverb.pre_lpf);
	element->SetAttribute("level", s->reverb.level);
	element->SetAttribute("time", s->reverb.time);
	element->SetAttribute("delay_feedback", s->reverb.delay_feedback);
	element->SetAttribute("pre_delay_time", s->reverb.pre_delay_time);
	root->LinkEndChild(element);

	//fx blocks
	for(int fxb = 0; fxb < 2; fxb++)
	{
		element = new TiXmlElement("bitcrusher");
		element->SetAttribute("fxblock", fxb);
		element->SetAttribute("on", s->fx_block[fxb].bitcrusher.on);
		element->SetAttribute("bitresolution", s->fx_block[fxb].bitcrusher.bitresolution);
		// element->SetAttribute("brightness", s->fx_block[fxb].bitcrusher.brightness);
		element->SetAttribute("downsampling", s->fx_block[fxb].bitcrusher.downsampling);
		root->LinkEndChild(element);

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
//			element->SetAttribute("channel", s->voices[str].at(vi).channel); //not saved, auto-generated
			element->SetAttribute("wave_id", wavetable.at(s->voices[str].at(vi).wavetable_index).c_str());
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

	
		
	std::string filepath = working_directory + "/presets/" + filename + ".mz";
	system("mount -o remount,rw /usr"); //mount read-write
	sleep(1);
	doc.SaveFile(filepath.c_str());
	system("sync");
	system("mount -o remount,ro /usr"); //mount read-write

	currentPresetName = filename;

	std::cout << "File " << filepath << " saved." << std::endl << std::flush;
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
		if(new_channel == 16) continue; // an elusive chip bug that i hate fiercely
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
	setMute(str, v, false);

	setFxBlockOn(str, v, 0, false);
	setFxBlockOn(str, v, 1, false);

	int control_t = 0, control_d = 0;
	switch(str)
	{
		case 0: control_t = STRING_TOUCH_X_0; control_d = STRING_DRAG_X_0; break;
		case 1: control_t = STRING_TOUCH_X_0; control_d = STRING_DRAG_X_1; break;
		case 2: control_t = STRING_TOUCH_X_0; control_d = STRING_DRAG_X_2; break;
		case 3: control_t = STRING_TOUCH_X_0; control_d = STRING_DRAG_X_3; break;
		case 4: control_t = STRING_TOUCH_X_0; control_d = STRING_DRAG_X_4; break;
		case 5: control_t = STRING_TOUCH_X_0; control_d = STRING_DRAG_X_5; break;
	}

//	assignSynthVoiceParam(control_t, PARAM_VOICE_VELOCITY, str, v, true, 0, 0);
//	assignSynthVoiceParam(control_d, PARAM_VOICE_PITCH_WHEEL, str, v, false, 0, 0);
}

void Synth::removeVoice(int str, int voice_index)
{
//std::cout << "Remove voice - str: " << (unsigned int) str << " vi: " << (unsigned int) voice_index << std::endl << std::flush;

	unAssignSynthVoiceParam(PARAM_VOICE_AMP_ENV_ATTACK, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_AMP_ENV_DECAY, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_AMP_ENV_RELEASE, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_FILTER_FREQUENCY, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_FILTER_RESONANCE, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_DETUNE_COURSE, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_DETUNE_FINE, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_VIBRATE_RATE, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_VIBRATE_DEPTH, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_VIBRATE_DELAY, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_CHANNEL_VOLUME, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_PORTAMENTO_TIME, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_PAN, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_PITCH_BEND_ST, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_PITCH_WHEEL, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_VELOCITY, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_REVERB_SEND, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_FILTER_ATTACK, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_FILTER_DECAY, str, voice_index);
	unAssignSynthVoiceParam(PARAM_VOICE_FILTER_RELEASE, str, voice_index);

	current_setting.voices[str].erase(current_setting.voices[str].begin() + voice_index);

	//we must update all references to voice parameter controls
	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name >= PARAM_VOICE_AMP_ENV_ATTACK) && (control_list->at(i).name < PARAM_VOICE_END))
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

void Synth::assignMidiStopSound(unsigned char str, unsigned char cc, unsigned char val)
{
//	std::cout << "assignMidiStopSound- str:" << (unsigned int) str << " cc: " << (unsigned int) cc << " val: " << (unsigned int) val << std::endl << std::flush;
	struct cc new_cc;
	new_cc.cc_num = cc;
	new_cc.value = val;
	current_setting.stop_sound_cmds[str].push_back(new_cc);
}

std::vector<int> Synth::findAssignStopSound(unsigned char str)
{
	std::vector<int> ret;

	for(int i = 0; i < current_setting.stop_sound_cmds[str].size(); i++)
		ret.push_back(current_setting.stop_sound_cmds[str].at(i).cc_num);

	return ret;
}

void Synth::unAssignMidiStopSound(unsigned char str, unsigned char cc)
{
//	std::cout << "unAssignMidiStopSound- str:" << (unsigned int) str << " cc: " << (unsigned int) cc << std::endl << std::flush;

	std::vector<struct cc> *control_list = &current_setting.stop_sound_cmds[str];
	std::vector<struct cc>::iterator t = control_list->begin();

	while(t != control_list->end())
	{
		if((*t).cc_num == cc)
			control_list->erase(t);
		else
			t++;
	}
}

void Synth::sendLearnStopSound(unsigned char str, unsigned char cc)
{
	for(int i = 0; i < current_setting.stop_sound_cmds[str].size(); i++)
		if(current_setting.stop_sound_cmds[str].at(i).cc_num == cc)
		{
			midi.sendCC(MIDI_OUT, current_setting.string_midi_out_channel[str], cc, current_setting.stop_sound_cmds[str].at(i).value);
			break;
		}
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
	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
		std::vector<struct assignable_effect>::iterator t = control_list->begin();

		while(t != control_list->end())
		{
			if(((*t).name == synth_param) && ((*t).fxb == fxb))
				control_list->erase(t);
			else
				t++;
		}
	}
}

std::vector<int> Synth::findAssignSynthEffect(unsigned char synth_param, unsigned char fxb, std::vector<struct assignable_effect *> *ae)
{
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).fxb == fxb))
			{
				controls.push_back(touch_control);
				ae->push_back(&control_list->at(i));
			}
		}
	}

	return controls;
}

void Synth::setDragCenterSynthEffect(unsigned char synth_param, unsigned char fxb, unsigned int drag_center)
{
//std::cout << "Set drag center synth effect. " << std::endl << std::flush;
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

void Synth::assignSynthVoiceParam(unsigned char touch_control, unsigned char synth_param, unsigned char str, unsigned char voice_index, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = synth_param;
	ae.str = str;
	ae.output = SYNTH;
	ae.voice_index = voice_index;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignSynthVoiceParam(unsigned char synth_param, unsigned char str, unsigned char voice_index)
{
	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
		std::vector<struct assignable_effect>::iterator t = control_list->begin();

		while(t != control_list->end())
		{
			if(((*t).name == synth_param) && ((*t).str == str) && ((*t).voice_index == voice_index))
				control_list->erase(t);
			else
				t++;
		}
	}
}

std::vector<int> Synth::findAssignSynthVoiceParam(unsigned char synth_param, unsigned char str, unsigned char voice_index, std::vector<struct assignable_effect *> *ae)
{
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == synth_param) && (control_list->at(i).str == str) && (control_list->at(i).voice_index == voice_index))
			{
				controls.push_back(touch_control);
				ae->push_back(&control_list->at(i));
			}
		}
	}

	return controls;
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

void Synth::assignMidiVelocity(unsigned char touch_control, int str, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_MIDI_VELOCITY;
	ae.str = str;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignMidiVelocity(unsigned char touch_control, int str)
{
	std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
	std::vector<struct assignable_effect>::iterator t = control_list->begin();

	while(t != control_list->end())
	{
		if(((*t).name == PARAM_MIDI_VELOCITY) && ((*t).str == str))
			control_list->erase(t);
		else
			t++;
	}
}

std::vector<int> Synth::findAssignMidiVelocity(int str, std::vector<struct assignable_effect *> *ae)
{
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_MIDI_VELOCITY) && (control_list->at(i).str == str))
			{
				controls.push_back(touch_control);
				ae->push_back(&control_list->at(i));
			}
		}
	}

	return controls;
}

void Synth::setDragCenterMidiVelocity(int str, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_MIDI_VELOCITY) && (control_list->at(i).str == str))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
		}
	}
}

void Synth::assignMidiPitch(unsigned char touch_control, unsigned char channel, bool inverse, unsigned int variation_start, unsigned int variation_end)
{
	struct assignable_effect ae;

	ae.name = PARAM_MIDI_PITCH;
	ae.output = MIDI_OUT;
	ae.channel = channel;
	ae.inverse = inverse;
	ae.variation_start = variation_start;
	ae.variation_end = variation_end;
	ae.drag_center = 64;

	addToControl(touch_control, ae);
}

void Synth::unAssignMidiPitch(unsigned char touch_control, unsigned char channel)
{
	std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
	std::vector<struct assignable_effect>::iterator t = control_list->begin();

	while(t != control_list->end())
	{
		if(((*t).name == PARAM_MIDI_PITCH) && ((*t).output == MIDI_OUT) && ((*t).channel == channel))
			control_list->erase(t);
		else
			t++;
	}
}

std::vector<int> Synth::findAssignMidiPitch(unsigned char channel, std::vector<struct assignable_effect *> *ae)
{
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_MIDI_PITCH) && (control_list->at(i).output == MIDI_OUT) && (control_list->at(i).channel == channel))
			{
				controls.push_back(touch_control);
				ae->push_back(&control_list->at(i));
			}
		}
	}

	return controls;
}

void Synth::setDragCenterMidiPitch(unsigned char channel, unsigned int drag_center)
{
	for(int touch_control = DRAG_X; touch_control <= DRAG_Y; touch_control++)
	{
		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_MIDI_PITCH) && (control_list->at(i).output == MIDI_OUT) && (control_list->at(i).channel == channel))
			{
				control_list->at(i).drag_center = drag_center;
				return;
			}
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

void Synth::unAssignCC(unsigned char output, unsigned char touch_control, unsigned char channel, unsigned int cc)
{
	std::vector<struct assignable_effect> *control_list = getControlList(touch_control);
	std::vector<struct assignable_effect>::iterator t = control_list->begin();

	while(t != control_list->end())
	{
		if(((*t).name == PARAM_CC) && ((*t).output == output) && ((*t).channel == channel) && ((*t).cc == cc))
			control_list->erase(t);
		else
			t++;
	}
}

void Synth::sendLearnCC(unsigned char channel, unsigned int cc)
{
	midi.sendCC(MIDI_OUT, channel, cc, 64);
}

std::vector<int> Synth::findAssignAllCC(unsigned char output, unsigned char channel, std::vector<struct assignable_effect *> *ae)
{
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_CC) && (control_list->at(i).output == output) && (control_list->at(i).channel == channel))
			{
				controls.push_back(touch_control);
				ae->push_back(&control_list->at(i));
			}
		}
	}

	return controls;
}

std::vector<int> Synth::findAssignCC(unsigned char output, unsigned char channel, unsigned int cc, std::vector<struct assignable_effect *> *ae)
{
	std::vector<int> controls;

	for(int touch_control = TOUCH_X; touch_control < CONTROL_END; touch_control++)
	{
		switch(touch_control)
		{
			case STRING_TOUCH_X_0:
			case STRING_TOUCH_X_1:
			case STRING_TOUCH_X_2:
			case STRING_TOUCH_X_3:
			case STRING_TOUCH_X_4:
			case STRING_TOUCH_X_5:
			case STRING_DRAG_X_0:
			case STRING_DRAG_X_1:
			case STRING_DRAG_X_2:
			case STRING_DRAG_X_3:
			case STRING_DRAG_X_4:
			case STRING_DRAG_X_5:
				continue;
		}

		std::vector<struct assignable_effect> *control_list = getControlList(touch_control);

		for(int i = 0; i < control_list->size(); i++)
		{
			if((control_list->at(i).name == PARAM_CC) && (control_list->at(i).output == output) && (control_list->at(i).channel == channel) && (control_list->at(i).cc == cc))
			{
				controls.push_back(touch_control);
				ae->push_back(&control_list->at(i));
			}
		}
	}

	return controls;
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
//	std::cout << "setLfoRate not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x03, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_rate = val;
}

void Synth::setLfoFrequencyDepth(int val)
{
//	std::cout << "setLfoFrequencyDepth not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x05, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_frequency_depth = val;
}

void Synth::setLfoAmpDepth(int val)
{
//	std::cout << "setLfoAmpDepth not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x06, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_amp_depth = val;
}

void Synth::setLfoPitchDepth(int val)
{
//	std::cout << "setLfoPitchDepth not implemented yet.";
//	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x2p, 0x04, val, 0x00, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.lfo_pitch_depth = val;
}

int Synth::getMasterVolume(void)
{
	return master_volume;
//	return current_setting.master_volume;
}

void Synth::setMasterVolume(int val)
{
//	unsigned char sysex[8] = {0xF0, 0x7F, 0x7F, 0x04, 0x01, 0x00, val, 0xF7};
//	midi.sendSysex(SYNTH, sysex, 8);

	midi.sendNRPN(SYNTH, 0, 0x37, 0x5E, val); //anything greater than 64 is > 0dB up to +6dB
	master_volume = val;
//	current_setting.master_volume = val; //we use preset-independent volume now
}

void Synth::setSynthesizerVolume(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x07, val);
}

void Synth::setChannel(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).channel = val;
}

void Synth::setWave(int str, int voice_index, int program)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendProgramChange(channel, 0, program);
//	current_setting.voices[str].at(voice_index).bank = bank;
	current_setting.voices[str].at(voice_index).wavetable_index = program;
}

void Synth::setAmpEnvAttack(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x63, val);
	current_setting.voices[str].at(voice_index).amp_env_attack = val;
}

void Synth::setAmpEnvDecay(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x64, val);
	current_setting.voices[str].at(voice_index).amp_env_decay = val;
}

void Synth::setAmpEnvRelease(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x66, val);
	current_setting.voices[str].at(voice_index).amp_env_release = val;
}

void Synth::setFilterFrequency(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x20, val);
	current_setting.voices[str].at(voice_index).filter_frequency = val;
}

void Synth::setFilterResonance(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x21, val);
	current_setting.voices[str].at(voice_index).filter_resonance = val;
}

void Synth::setDetuneCourse(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendRPN(SYNTH, channel, 0x00, 0x02, val);
	current_setting.voices[str].at(voice_index).detune_course = val;
}

void Synth::setDetuneFine(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendRPN(SYNTH, channel, 0x00, 0x01, val);
	current_setting.voices[str].at(voice_index).detune_fine = val;
}

void Synth::setVibrateRate(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x08, val);
	current_setting.voices[str].at(voice_index).vibrate_rate = val;
}

void Synth::setVibrateDepth(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x09, val);
	current_setting.voices[str].at(voice_index).vibrate_depth = val;
}

void Synth::setVibrateDelay(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x0A, val);
	current_setting.voices[str].at(voice_index).vibrate_delay = val;
}

void Synth::setChannelVolume(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 7, val);
	current_setting.voices[str].at(voice_index).channel_volume = val;
}

void Synth::setMuteChannelVolume(int str, int voice_index)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 7, 0);
}

void Synth::setUnMuteChannelVolume(int str, int voice_index)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 7, current_setting.voices[str].at(voice_index).channel_volume);
}

void Synth::setPortamentoTime(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 5, val);
	current_setting.voices[str].at(voice_index).portamento_time = val;
}

void Synth::setReverbSend(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 91, val);
	current_setting.voices[str].at(voice_index).reverb_send = val;
}

void Synth::setPitchWheel(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendPitch(SYNTH, channel, val);
	current_setting.voices[str].at(voice_index).pitch_wheel = val;
}

void Synth::setVoiceVelocity(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index).velocity = val;
}

void Synth::setFilterType(int str, int voice_index, int val)
{
//std::cout << "Set Filter Type: " << val << std::endl << std::flush;
	int channel = current_setting.voices[str].at(voice_index).channel;
//std::cout << "Filter Type not implemented." << std::endl << std::flush;
	midi.sendCC(SYNTH, channel, 79, val);
	current_setting.voices[str].at(voice_index).filter_type = val;
}

void Synth::setFilterAttack(int str, int voice_index, int val)
{
//std::cout << "Set Filter Attack: " << val << std::endl << std::flush;
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 80, val);
	current_setting.voices[str].at(voice_index).filter_attack = val;
}

void Synth::setFilterDecay(int str, int voice_index, int val)
{
//std::cout << "Set Filter Decay: " << val << std::endl << std::flush;
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 81, val);
	current_setting.voices[str].at(voice_index).filter_decay = val;
}

void Synth::setFilterRelease(int str, int voice_index, int val)
{
//std::cout << "Set Filter Release: " << val << std::endl << std::flush;
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 82, val);
	current_setting.voices[str].at(voice_index).filter_release = val;
}

void Synth::setMute(int str, int voice_index, bool state)
{
	current_setting.voices[str].at(voice_index).mute = state;
}

void Synth::setPan(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendCC(SYNTH, channel, 10, val);
	current_setting.voices[str].at(voice_index).pan = val;
}

void Synth::setPitchBendSemitones(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	midi.sendRPN(SYNTH, channel, 0x00, 0x00, val);
	current_setting.voices[str].at(voice_index).pitch_bend_semitones = val;
}

void Synth::setFxBlockOn(int str, int voice_index, int fxb, bool state)
{
	int channel = current_setting.voices[str].at(voice_index).channel;
	int chan = channel;
	if(channel >= 16) chan = channel - 16;

	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x40+chan, 0x22, fxb+1, 0x00, 0xF7};
	if(!state)
		sysex[8] = 0;	//turn off
	midi.sendSysexToChannel(SYNTH, sysex, 11, channel);

	current_setting.voices[str].at(voice_index).insert_fx_block_on[fxb] = state;
}

void Synth::setBitCrusherOn(int fxb, bool state)
{
	std::cout << "setBitCrusherOn" << std::endl << std::flush;
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x24, state);
	current_setting.fx_block[fxb].bitcrusher.on = state;
	std::cout << "setBitCrusherOn exit" << std::endl << std::flush;
}

void Synth::setBitcrusherBitResolution(int fxb, int val)
{
	std::cout << "setBitcrusherBitResolution" << std::endl << std::flush;
	current_setting.fx_block[fxb].bitcrusher.bitresolution = val;

	if (val > 16)
		return;

	if (val > 0)
		val = 17 - val;

	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x25, val);
	std::cout << "setBitcrusherBitResolution exit" << std::endl << std::flush;
}

//void Synth::setBitcrusherBrightness(int fxb, int val)
//{
//	std::cout << "setBitcrusherBrightness" << std::endl << std::flush;
//	//midi.sendNRPN(SYNTH, 0, 0x3A+fxb, ??, val);
//	current_setting.fx_block[fxb].bitcrusher.brightness = val;
//	std::cout << "setBitcrusherBrightness exit" << std::endl << std::flush;
//}

void Synth::setBitcrusherDownsampling(int fxb, int val)
{
	std::cout << "setBitcrusherDownsampling" << std::endl << std::flush;
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x26, val);
	current_setting.fx_block[fxb].bitcrusher.downsampling = val;
	std::cout << "setBitcrusherDownsampling exit" << std::endl << std::flush;
}


void Synth::setDistortionOn(int fxb, bool state)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x61, state);
	current_setting.fx_block[fxb].distortion.on = state;
}

void Synth::setDistortionType(int fxb, int val)
{
	//modified for new algorithms (was 0x63)
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x60, val);
	current_setting.fx_block[fxb].distortion.type = val;
}

void Synth::setDistortionLevel(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x66, val);
	current_setting.fx_block[fxb].distortion.level = val;
}

void Synth::setDistortionDrive(int fxb, int val)
{
	//modified for new algorithms (was 0x67)
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x62, val);
	current_setting.fx_block[fxb].distortion.drive = val;
}

void Synth::setDistortionTone(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x64, val);
	current_setting.fx_block[fxb].distortion.tone = val;
}

void Synth::setDistortionNoiseReduction(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x68, val);
	current_setting.fx_block[fxb].distortion.noise_reduction = val;
}

void Synth::setDistortionBooster(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x69, val);
	current_setting.fx_block[fxb].distortion.booster = val;
}

void Synth::setCompressorOn(int fxb, bool state)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x41, state);
	current_setting.fx_block[fxb].compressor.on = state;
}

void Synth::setCompressorAttack(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x42, val);
	current_setting.fx_block[fxb].compressor.attack = val;
}

void Synth::setCompressorRelease(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x43, val);
	current_setting.fx_block[fxb].compressor.release = val;
}

void Synth::setCompressorThreshold(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x44, val);
	current_setting.fx_block[fxb].compressor.threshold = val;
}

void Synth::setCompressorRatio(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x45, val);
	current_setting.fx_block[fxb].compressor.ratio = val;
}

void Synth::setCompressorBoost(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x46, val);
	current_setting.fx_block[fxb].compressor.boost = val;
}

void Synth::setCompressorKnee(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x47, val);
	current_setting.fx_block[fxb].compressor.knee = val;
}

/*
void Synth::setWahwahType(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x51, val);
	current_setting.fx_block[fxb].wahwah.type = val;
}

void Synth::setWahwahFrequency(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x53, val);
	current_setting.fx_block[fxb].wahwah.frequency = val;
}

void Synth::setWahwahResonance(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x53, val);
	current_setting.fx_block[fxb].wahwah.resonance = val;
}

void Synth::setWahwahSensitivity(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x54, val);
	current_setting.fx_block[fxb].wahwah.sensitivity = val;
}
*/

void Synth::setModulationOn(int fxb, bool state)
{
	if(state)
        setModulationType(fxb, current_setting.fx_block[fxb].modulation.type);
	else
       	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x30, 0);

	current_setting.fx_block[fxb].modulation.on = state;
}

void Synth::setModulationType(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x30, val);
	current_setting.fx_block[fxb].modulation.type = val;
}

void Synth::setModulationLevel(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x31, val);
	current_setting.fx_block[fxb].modulation.level = val;
}

void Synth::setModulationChorusDelayTime(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x32, val);
	current_setting.fx_block[fxb].modulation.chorus_delay_time = val;
}

void Synth::setModulationChorusFeedback(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x33, val);
	current_setting.fx_block[fxb].modulation.chorus_feedback = val;
}

void Synth::setModulationChorusHpf(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x34, val);
	current_setting.fx_block[fxb].modulation.chorus_hpf = val;
}

void Synth::setModulationDelayFeedbackFilter(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x35, val);
	current_setting.fx_block[fxb].modulation.delay_feedback_filter = val;
}

void Synth::setModulationRate(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x36, val);
	current_setting.fx_block[fxb].modulation.rate = val;
}

void Synth::setModulationDepth(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x37, val);
	current_setting.fx_block[fxb].modulation.depth = val;
}

void Synth::setModulationTremolo(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x38, val);
	current_setting.fx_block[fxb].modulation.tremolo = val;
}

void Synth::setDelayOn(int fxb, bool state)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x59, state);
	current_setting.fx_block[fxb].delay.on = state;
}

void Synth::setDelayMode(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5A, val);
	current_setting.fx_block[fxb].delay.mode = val;
}

void Synth::setDelayPreLp(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5B, val);
	current_setting.fx_block[fxb].delay.pre_lp = val;
}

void Synth::setDelayLevel(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5C, val);
	current_setting.fx_block[fxb].delay.level = val;
}

void Synth::setDelayTime(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5D, val);
	current_setting.fx_block[fxb].delay.time = val;
}

void Synth::setDelayFeedback(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5E, val);
	current_setting.fx_block[fxb].delay.feedback = val;
}

void Synth::setDelayFeedbackFilter(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x5F, val);
	current_setting.fx_block[fxb].delay.feedback_filter = val;
}

void Synth::setReverbType(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x30, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.type = val;
	//setReverbCharacter(current_setting.reverb.character);
	setReverbPreLpf(current_setting.reverb.pre_lpf);
	setReverbLevel(current_setting.reverb.level);
	setReverbTime(current_setting.reverb.time);
	setReverbDelayFeedback(current_setting.reverb.delay_feedback);
	setReverbPreDelay(current_setting.reverb.pre_delay_time);
}

void Synth::setReverbCharacter(int val)
{
/*	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x31, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.character = val;
*/
}

void Synth::setReverbPreLpf(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x32, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.pre_lpf = val;
}

void Synth::setReverbLevel(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x33, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.level = val;
}

void Synth::setReverbTime(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x34, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.time = val;
}

void Synth::setReverbDelayFeedback(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x35, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.delay_feedback = val;
}

void Synth::setReverbPreDelay(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x37, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.pre_delay_time = val;
}

void Synth::setEQOn(bool state)
{
	if(state)
		midi.sendNRPN(SYNTH, 0, 0x37, 0x55, 2);
	else
		midi.sendNRPN(SYNTH, 0, 0x37, 0x55, 0);

	current_setting.equalizer.on = state;
}

void Synth::setEQLowestBandGain(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x08, val);
	current_setting.equalizer.lowest.gain = val;
}

void Synth::setEQLowerBandGain(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x09, val);
	current_setting.equalizer.lower.gain = val;
}

void Synth::setEQHigherBandGain(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x0A, val);
	current_setting.equalizer.higher.gain = val;
}

void Synth::setEQHighestBandGain(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x0B, val);
	current_setting.equalizer.highest.gain = val;
}

void Synth::setEQLowestBandFrequency(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x0C, val);
	current_setting.equalizer.lowest.frequency = val;
}

void Synth::setEQLowerBandFrequency(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x0D, val);
	current_setting.equalizer.lower.frequency = val;
}

void Synth::setEQHigherBandFrequency(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x0E, val);
	current_setting.equalizer.higher.frequency = val;
}

void Synth::setEQHighestBandFrequency(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x0F, val);
	current_setting.equalizer.highest.frequency = val;
}

void Synth::setEQLowMidQ(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x01, val);
	current_setting.equalizer.low_mid_q = val;
}

void Synth::setEQHighMidQ(int val)
{
	midi.sendNRPN(SYNTH, 0, 0x37, 0x02, val);
	current_setting.equalizer.high_mid_q = val;
}

void Synth::setMixerLowCutFilterFrequency(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x22, val); //datasheet incorrect
	current_setting.fx_block[fxb].mixer.low_cut_filter_frequency = val;
}

void Synth::setMixerHighCutFilterFrequency(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x23, val); //datasheet incorrect
	current_setting.fx_block[fxb].mixer.high_cut_filter_frequency = val;
}

void Synth::setMixerInputGain(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x21, val); //datasheet incorrect
	current_setting.fx_block[fxb].mixer.input_gain = val;
}

void Synth::setMixerOutputLevel(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x2A, val);
	current_setting.fx_block[fxb].mixer.output_level = val;
}

void Synth::setMixerPan(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x2B, val);
	current_setting.fx_block[fxb].mixer.pan = val;
}

void Synth::setMixerReverbSend(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x2D, val);
	current_setting.fx_block[fxb].mixer.reverb_send = val;
}

/*	[MZ] "Sliding" - There are two problems we must avoid when sliding on the neck:
	1. The sound of one tone changing to another when pressing a new note - this is handled by portamento;
	2. The restarting of the amplitude and filter envelopes when a new note is turned on after the old note has been turned off.
	The functions below handle this. They are very important and it wouldn't be the misa kitara without them.
*/

void Synth::sendNoteOn(unsigned char str, unsigned char btn, bool attack, bool make_sound, bool portamento_off)
{
	unsigned char note = current_setting.tuning[str] + btn;

	if(current_setting.string_midi_out_channel[str] >= 0) //midi output
	{
		if(current_setting.stop_sound_cmds[str].size() > 0)	//misa kitara mode!
		{
			if(attack)
			{
				sendVariation(str, 0, current_setting.string_midi_out_channel[str]);
				midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], string_note[str], 0);

				if(make_sound)
				{
					for(int cmd = 0; cmd < current_setting.stop_sound_cmds[str].size(); cmd++)
						midi.sendCC(MIDI_OUT, current_setting.string_midi_out_channel[str], current_setting.stop_sound_cmds[str].at(cmd).cc_num, current_setting.stop_sound_cmds[str].at(cmd).value);
				}
				else
				{
					for(int cmd = 0; cmd < current_setting.stop_sound_cmds[str].size(); cmd++)
						midi.sendCC(MIDI_OUT, current_setting.string_midi_out_channel[str], current_setting.stop_sound_cmds[str].at(cmd).cc_num, 0);
				}

				midi.sendNoteOn(MIDI_OUT, current_setting.string_midi_out_channel[str], note, velocity[str]);
			}
			else
			{
				for(int cmd = 0; cmd < current_setting.stop_sound_cmds[str].size(); cmd++)
					midi.sendCC(MIDI_OUT, current_setting.string_midi_out_channel[str], current_setting.stop_sound_cmds[str].at(cmd).cc_num, current_setting.stop_sound_cmds[str].at(cmd).value);

				if(string_note[str] != note)
				{
					midi.sendNoteOn(MIDI_OUT, current_setting.string_midi_out_channel[str], note, velocity[str]);
					if(string_note[str] != -1)// && (string_note[str] != note))
						midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], string_note[str], 0);
				}
			}
		}
		else	//"compatibility" mode (not very good)
		{
			sendVariation(str, 0, current_setting.string_midi_out_channel[str]);
			midi.sendNoteOn(MIDI_OUT, current_setting.string_midi_out_channel[str], note, velocity[str]);
			if((string_note[str] != note) && (string_note[str] != -1))
				midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], string_note[str], 0);
		}
		string_note[str] = note;
	}
	else //synth output
	{
		if(!mode_ringing_notes)
		{
			for(int i = 0; i < current_setting.voices[str].size(); i++)
			{
				int vel = current_setting.voices[str].at(i).velocity;

				if(attack)
				{
					sendVariation(str, i, current_setting.voices[str].at(i).channel);
					midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, string_note[str], 0);
					if(make_sound)
						setUnMuteChannelVolume(str, i);
					else
						setMuteChannelVolume(str, i);

					if(!current_setting.voices[str].at(i).mute)
						midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i).channel, note, vel);
				}
				else
				{
					setUnMuteChannelVolume(str, i);
					if(string_note[str] != note)
					{
						if(!current_setting.voices[str].at(i).mute)
						{
							//the synth IC uses some timer code causing commands to not be sequential
							midi.sendCC(SYNTH, current_setting.voices[str].at(i).channel, 5, current_setting.voices[str].at(i).portamento_time);
							if(portamento_off) midi.sendCC(SYNTH, current_setting.voices[str].at(i).channel, 5, 0);
							midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i).channel, note, vel);
							//if(portamento_off) midi.sendCC(SYNTH, current_setting.voices[str].at(i).channel, 5, current_setting.voices[str].at(i).portamento_time);
						}
						if(string_note[str] != -1)
							midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, string_note[str], 0);
					}
				}
			}
		}
		else //ringing notes mode (customer request) - no envelope control / sliding
		{
			for(int i = 0; i < current_setting.voices[str].size(); i++)
			{
				sendVariation(str, 0, current_setting.string_midi_out_channel[str]);
				midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i).channel, note, current_setting.voices[str].at(i).velocity);
				if((string_note[str] != note) && (string_note[str] != -1))
					midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, string_note[str], 0);
			}
		}
		string_note[str] = note;
	}
}

void Synth::sendNoteOff(unsigned char str, unsigned char btn)
{
	unsigned char note = current_setting.tuning[str] + btn;

	if(current_setting.string_midi_out_channel[str] >= 0)	//midi mode
	{
		if(current_setting.stop_sound_cmds[str].size() > 0) //misa kitara mode
		{
			for(int cmd = 0; cmd < current_setting.stop_sound_cmds[str].size(); cmd++)
				midi.sendCC(MIDI_OUT, current_setting.string_midi_out_channel[str], current_setting.stop_sound_cmds[str].at(cmd).cc_num, 0);
		}
		else //compatibility mode (no.)
		{
			midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], note, 0);
		}
	}
	else	//synth mode
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
		{
			if(mode_ringing_notes)
				midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, note, 0);
			else
				setMuteChannelVolume(str, i);
		}
	}
}

void Synth::sendNoteOffRinging(unsigned char str, unsigned char btn)
{
	unsigned char note = current_setting.tuning[str] + btn;

	if(current_setting.string_midi_out_channel[str] >= 0)
	{
		midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], note, 0);
	}
	else
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
		{
			midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, note, 0);
		}
	}
	string_note[str] = -1;
}

void Synth::sendCurrentSynthNotesOff(void)
{
	for(int str = 0; str < 6; str++)
	{
		if(current_setting.string_midi_out_channel[str] >= 0)
		{
			if(string_note[str] != -1)
				midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], string_note[str], 0);

			midi.sendSoundOff(MIDI_OUT, current_setting.string_midi_out_channel[str]);
			if(current_setting.stop_sound_cmds[str].size() > 0) //misa kitara mode
			{
				//this may make some noise if the sendSoundOff function above is not supported by the connected synthesizer
				for(int cmd = 0; cmd < current_setting.stop_sound_cmds[str].size(); cmd++)
					midi.sendCC(MIDI_OUT, current_setting.string_midi_out_channel[str], current_setting.stop_sound_cmds[str].at(cmd).cc_num, current_setting.stop_sound_cmds[str].at(cmd).value);
			}
		}
		else
		{
			for(int i = 0; i < current_setting.voices[str].size(); i++)
			{
				if(string_note[str] != -1)
					midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, string_note[str], 0);
				midi.sendSoundOff(SYNTH, current_setting.voices[str].at(i).channel);
				setUnMuteChannelVolume(str, i);
			}
		}
		string_note[str] = -1;
	}
}

void Synth::sendStopSound(unsigned char str, unsigned char btn)
{
	unsigned char note = current_setting.tuning[str] + btn;

	if(current_setting.string_midi_out_channel[str] >= 0)
	{
		for(int i = 0; i < current_setting.stop_sound_cmds[str].size(); i++)
			midi.sendCC(MIDI_OUT, current_setting.string_midi_out_channel[str], current_setting.stop_sound_cmds[str].at(i).cc_num, 0);
	}
	else
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
		{
			if(string_note[str] != -1)
				midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i).channel, string_note[str], 0);
			midi.sendSoundOff(SYNTH, current_setting.voices[str].at(i).channel);
//			setMuteChannelVolume(str, i); //note envelope will auto restart
//			midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i).channel, string_note[str], current_setting.voices[str].at(i).velocity); //note envelope will auto restart
		}
		string_note[str] = -1;
	}
}

void Synth::startEnvelope(unsigned char str, unsigned char btn, bool attack, bool make_sound, bool portamento_off)
{
	bool env = false;
	if(current_setting.string_midi_out_channel[str] >= 0)
	{
		if(current_setting.stop_sound_cmds[str].size() > 0)
			env = true;
	}
	else
	if(!mode_ringing_notes)
		env = true;

	if(env)
	{
		sendNoteOn(str, 0, true, false, false);
		sendNoteOff(str, 0);
	}
}

unsigned char Synth::midiScale(int number, int scaler)
{
	if(scaler == 0)
		return 0;

	int val = (number * 127) / scaler;

	return (unsigned char) val;
}

void Synth::sendControl(unsigned char control_type, int val, int scaler)
{
	std::vector<struct assignable_effect> *control_list = getControlList(control_type);
	for(int i = 0; i < control_list->size(); i++)
	{
		struct assignable_effect *ae = &control_list->at(i);
		int v = val;

		if((control_type == DRAG_X) || (control_type == DRAG_Y))
		{
			if((ae->name != PARAM_MIDI_PITCH) && (ae->name != PARAM_VOICE_PITCH_WHEEL))
			{
				if(v < scaler / 2)
				{
					int max;
					if(!ae->inverse)
						max = ((ae->drag_center+1) * scaler / 128);
					else
						max = ((127-ae->drag_center)+1) * scaler / 128;

					v *= max;
					v /= (scaler / 2);
				}
				else //if(v >= scaler / 2)
				{
					int min;
					if(!ae->inverse)
						min = ((ae->drag_center+1) * scaler / 128);
					else
						min = ((127-ae->drag_center)+1) * scaler / 128;

					v = (v - (scaler/2));
					v *= (scaler - min);
					v /= scaler / 2;
					v += min;
				}
			}
		}
		sendToEffect(ae, v, scaler);
	}
}

void Synth::sendVariation(int str, int voice_index, int channel)
{
	for(int i = 0; i < current_setting.AE_variation.size(); i++)
	{
		struct assignable_effect *ae = &current_setting.AE_variation.at(i);

		bool flag = false;
		switch(ae->name)
		{
			case PARAM_MIDI_PITCH:
			case PARAM_CC:
				if(ae->channel == channel)
					flag = true;
				break;

			case PARAM_MIDI_VELOCITY:
				if(ae->str == str)
					flag = true;
				break;

			default: //synth effect or voice parameter
				if(ae->name >= PARAM_VOICE_AMP_ENV_ATTACK)
				{
					if((ae->str == str) && (ae->voice_index == voice_index))
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
	unsigned int scaled_pitch = 0x2000;
	unsigned char final_val = mval;
	if(ae->inverse) final_val = 127 - final_val;		

	switch(ae->name)
	{
		//hopefully the compiler compiles this to a branch table
		case PARAM_BITCRUSHER_ON: setBitCrusherOn(ae->fxb, final_val); break;
		case PARAM_BITCRUSHER_RESOLUTION: setBitcrusherBitResolution(ae->fxb, final_val); break;
		//case PARAM_BITCRUSHER_BRIGHTNESS: setBitcrusherBrightness(ae->fxb, final_val); break;
		case PARAM_BITCRUSHER_DOWNSAMPLING: setBitcrusherDownsampling(ae->fxb, final_val); break;

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
//		case PARAM_VOICE_PITCH_WHEEL: setPitchWheel(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_VELOCITY: setVoiceVelocity(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_REVERB_SEND: setReverbSend(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_TYPE: setFilterType(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_ATTACK: setFilterAttack(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_DECAY: setFilterDecay(ae->str, ae->voice_index, final_val); break;
		case PARAM_VOICE_FILTER_RELEASE: setFilterRelease(ae->str, ae->voice_index, final_val); break;

		case PARAM_MIDI_VELOCITY: setVelocity(ae->str, final_val); break;
		case PARAM_MIDI_PITCH:
			scaled_pitch = (val * 0x4000) / scaler; //yes... we scale up... resolution limited
			midi.sendPitch(ae->output, ae->channel, ae->inverse?0x4000-scaled_pitch:scaled_pitch);
			break;
		case PARAM_VOICE_PITCH_WHEEL:
			scaled_pitch = (val * 0x4000) / scaler; //yes... we scale up... resolution limited
			setPitchWheel(ae->str, ae->voice_index, ae->inverse?0x4000-scaled_pitch:scaled_pitch);
			break;
		case PARAM_CC:
			midi.sendCC(ae->output, ae->channel, ae->cc, final_val);
			break;	
	}
}

void Synth::setVelocity(unsigned char str, unsigned char v)
{
	velocity[str] = v;
}

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

void Synth::setTuning(unsigned char str, unsigned char note)
{
	if(6 > str)
	{
		current_setting.tuning[str] = note;
	}
}

void Synth::setMidiOutChannel(unsigned char str, signed char channel)
{
	if(6 > str)
	{
		current_setting.string_midi_out_channel[str] = channel;
		//std::cout<<"midi_out_channel"<<(signed int)str<<"="<<(signed int)channel<<std::endl<<std::flush;
	}
}

signed char Synth::getMidiOutChannel(unsigned char str)
{
	if(6 > str)
	{
		//std::cout<<"midi_out_channel"<<(signed int)str<<"="<<(signed int)channel<<std::endl<<std::flush;
		return current_setting.string_midi_out_channel[str];
	}
	else
	{
		return -1;
	}
}

bool Synth::GetCurrentSetting(PSYNTH_SETTING pSynthsetting)
{
	*pSynthsetting = current_setting;
	return true;
}

void Synth::closeMidiPorts(void)
{
	midi.closePorts();
}

void Synth::openMidiPorts(void)
{
	midi.openPorts();
}

// OR : Drag & Drop
//synth::duplicateVoice(int src_string, int srv_voice_index, int dest_string, int dest_voice_index)
//{
//	synth::insertNewVoice(int string, int wave)
//
//		wavetable_index
//}

void Synth::SendParamToSynth(int string_index, int voice_index)
{
    setChannel(string_index, voice_index, current_setting.voices[string_index].at(voice_index).channel);
    setWave(string_index, voice_index, current_setting.voices[string_index].at(voice_index).wavetable_index);
    setAmpEnvAttack(string_index, voice_index, current_setting.voices[string_index].at(voice_index).amp_env_attack);
    setAmpEnvDecay(string_index, voice_index, current_setting.voices[string_index].at(voice_index).amp_env_decay);
    setAmpEnvRelease(string_index, voice_index, current_setting.voices[string_index].at(voice_index).amp_env_release);
    setFilterFrequency(string_index, voice_index, current_setting.voices[string_index].at(voice_index).filter_frequency);
    setFilterResonance(string_index, voice_index, current_setting.voices[string_index].at(voice_index).filter_resonance);
    setDetuneCourse(string_index, voice_index, current_setting.voices[string_index].at(voice_index).detune_course);
    setDetuneFine(string_index, voice_index, current_setting.voices[string_index].at(voice_index).detune_fine);
    setVibrateRate(string_index, voice_index, current_setting.voices[string_index].at(voice_index).vibrate_rate);
    setVibrateDepth(string_index, voice_index, current_setting.voices[string_index].at(voice_index).vibrate_depth);
    setVibrateDelay(string_index, voice_index, current_setting.voices[string_index].at(voice_index).vibrate_delay);
    setChannelVolume(string_index, voice_index, current_setting.voices[string_index].at(voice_index).channel_volume);
    setPortamentoTime(string_index, voice_index, current_setting.voices[string_index].at(voice_index).portamento_time);
    setPan(string_index, voice_index, current_setting.voices[string_index].at(voice_index).pan);
    setPitchBendSemitones(string_index, voice_index, current_setting.voices[string_index].at(voice_index).pitch_bend_semitones);
    setPitchWheel(string_index, voice_index, current_setting.voices[string_index].at(voice_index).pitch_wheel);
    setVoiceVelocity(string_index, voice_index, current_setting.voices[string_index].at(voice_index).velocity);
    setReverbSend(string_index, voice_index, current_setting.voices[string_index].at(voice_index).reverb_send);
	setFilterType(string_index, voice_index, current_setting.voices[string_index].at(voice_index).filter_type);
	setFilterAttack(string_index, voice_index, current_setting.voices[string_index].at(voice_index).filter_attack);
	setFilterDecay(string_index, voice_index, current_setting.voices[string_index].at(voice_index).filter_decay);
	setFilterRelease(string_index, voice_index, current_setting.voices[string_index].at(voice_index).filter_release);

    if(current_setting.voices[string_index].at(voice_index).insert_fx_block_on[0])
        setFxBlockOn(string_index, voice_index, 0, true);
    else
    if(current_setting.voices[string_index].at(voice_index).insert_fx_block_on[1])
        setFxBlockOn(string_index, voice_index, 1, true);
}


void Synth::SetMuteChannelForString(int string_index, int voice_index, bool Mute)
{
	current_setting.voices[string_index].at(voice_index).mute = Mute;
}

void Synth::SetSoloChannelForString(int string_index, int voice_index, bool Solo)
{
	// Mute all other channel
	for (int voiceIdx = 0; voiceIdx < 5; voiceIdx++)
	{
		if ((voiceIdx == voice_index) || (voiceIdx >= current_setting.voices[string_index].size()))
			continue;

		SetMuteChannelForString(string_index, voiceIdx, Solo);
	}
}

void Synth::setModeRingingNotes(bool mode)
{
	mode_ringing_notes = mode;
}
