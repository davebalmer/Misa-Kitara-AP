#include "MyException.h"
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
}

void Synth::resetSettings(void)
{
	//delete any previously loaded voices
	for(int s = 0; s < 6; s++)
	{
		for(int v = 0; v < current_setting.voices[s].size(); v++)
			delete(current_setting.voices[s].at(v));

		current_setting.voices[s].clear();

		current_setting.string_midi_out_channel[s] = -1;
	}

	//fix: delete all sysex strings

	AE_touch_x.clear();
	AE_touch_y.clear();
	AE_ball_x.clear();
	AE_ball_y.clear();
	AE_drag_x.clear();
	AE_drag_y.clear();
	AE_string_touch_x_0.clear();
	AE_string_touch_x_1.clear();
	AE_string_touch_x_2.clear();
	AE_string_touch_x_3.clear();
	AE_string_touch_x_4.clear();
	AE_string_touch_x_5.clear();
	AE_string_drag_x_0.clear();
	AE_string_drag_x_1.clear();
	AE_string_drag_x_2.clear();
	AE_string_drag_x_3.clear();
	AE_string_drag_x_4.clear();
	AE_string_drag_x_5.clear();
	AE_variation.clear();

	//reset all effects
	midi.sendNRPN(SYNTH, 0, 0x3A, 0x7F, 0); //fx block 1 //fix: should val be 0x7F?
	midi.sendNRPN(SYNTH, 0, 0x3B, 0x7F, 0); //fx block 2
	midi.sendGSReset();

	//HD algorithms need to be reset manually
	setDistortionOn(0, false);
	setDistortionOn(1, false);

	setMasterVolume(100);

	//i need to research what value to send
	for(int i = 0; i < 32; i++)
	{
		midi.sendCC(SYNTH, i, 121, 0);	//reset all controllers
		midi.sendCC(SYNTH, i, 126, 0);	//channel mono on = 0
		midi.sendCC(SYNTH, i, 65, 0x7F);	//channel portamento on = 0x7F

		//unassign effects inserts
		int chan = midi.setMidiPort(i);
		unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x40+chan, 0x22, 0x00, 0x00, 0xF7};
		midi.sendSysex(SYNTH, sysex, 11);
	}

	for(int i = 0; i < 6; i++)
		velocity[i] = 127;
}

/* .mz preset file rules:
	1.	"wave" will load a soundbank instrument. this should come first because every other 
		parameter will override the settings loaded direct from the soundbank instrument.
		The reason you can override the instrument settings is so that you can create
		complex sounds using multiple instruments as individual voices.
	2.	effects that are not completely defined in the file will show undefined parameters
		as greyed-out in the configuration screen (it does not know the default).
*/

void Synth::loadPresetFromFile(std::string filepath)
{
	std::ifstream preset_file(("./presets/" + filepath).c_str());

	if(!preset_file.is_open())
	{
		std::cerr << "Could not open preset file " << filepath << "." << std::endl;
		return;
	}

	resetSettings();
	
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
			current_setting.preset_name = field2;
		else
		if(field1 == "master_volume")
			setMasterVolume(atoi(field2.c_str()));
		else
		if(field1 == "lfo_rate")
			setLfoRate(atoi(field2.c_str()));
		else
		if(field1 == "lfo_frequency_depth")
			setLfoFrequencyDepth(atoi(field2.c_str()));
		else
		if(field1 == "lfo_amp_depth")
			setLfoAmpDepth(atoi(field2.c_str()));
		else
		if(field1 == "lfo_pitch_depth")
			setLfoPitchDepth(atoi(field2.c_str()));
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
			setChannel(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "wave")
			setWave(current_string, voice_index, atoi(field2.c_str()), atoi(field3.c_str()));
		else
		if(field1 == "amp_env_attack")
			setAmpEnvAttack(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "amp_env_decay")
			setAmpEnvDecay(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "amp_env_release")
			setAmpEnvRelease(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "filter_frequency")
			setFilterFrequency(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "filter_resonance")
			setFilterResonance(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "detune_course")
			setDetuneCourse(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "detune_fine")
			setDetuneFine(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "vibrate_rate")
			setVibrateRate(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "vibrate_depth")
			setVibrateDepth(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "vibrate_delay")
			setVibrateDelay(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "channel_volume")
			setChannelVolume(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "portamento_time")
			setPortamentoTime(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "reverb_send")
			setReverbSend(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "pan")
			setPan(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "pitch_bend_semitones")
			setPitchBendSemitones(current_string, voice_index, atoi(field2.c_str()));
		else
		if(field1 == "fx_block_on")
			setFxBlockOn(current_string, voice_index, atoi(field2.c_str()), true);
		else
		if(field1 == "fx_block")
			current_fx_block = atoi(field2.c_str());
		else
		if(field1 == "distortion_on")
			setDistortionOn(current_fx_block, true);
		else
		if(field1 == "distortion_type")
			setDistortionType(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "distortion_post_gain")
			setDistortionPostGain(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "distortion_drive")
			setDistortionDrive(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "distortion_tone")
			setDistortionTone(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_level")
			setModulationLevel(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_chorus_delay_time")
			setModulationChorusDelayTime(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_chorus_feedback")
			setModulationChorusFeedback(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_chorus_hpf")
			setModulationChorusHpf(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_delay_feedback_filter")
			setModulationDelayFeedbackFilter(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_rate")
			setModulationRate(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_depth")
			setModulationDepth(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "modulation_tremolo")
			setModulationTremolo(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "delay_on")
			setDelayOn(current_fx_block, true);
		else
		if(field1 == "delay_mode")
			setDelayMode(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "delay_pre_lp")
			setDelayPreLp(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "delay_level")
			setDelayLevel(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "delay_time")
			setDelayTime(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "delay_feedback")
			setDelayFeedback(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "delay_feedback_filter")
			setDelayFeedbackFilter(current_fx_block, atoi(field2.c_str()));
		else
		if(field1 == "reverb_type")
			setReverbType(atoi(field2.c_str()));
		else
		if(field1 == "reverb_character")
			setReverbCharacter(atoi(field2.c_str()));
		else
		if(field1 == "reverb_pre_lpf")
			setReverbPreLpf(atoi(field2.c_str()));
		else
		if(field1 == "reverb_level")
			setReverbLevel(atoi(field2.c_str()));
		else
		if(field1 == "reverb_time")
			setReverbTime(atoi(field2.c_str()));
		else
		if(field1 == "reverb_delay_feedback")
			setReverbDelayFeedback(atoi(field2.c_str()));
		else
		if(field1 == "reverb_pre_delay")
			setReverbPreDelay(atoi(field2.c_str()));
		else
		if(field1 == "variation")
			assignEffect(VARIATION, current_fx_block, &field2, &field3, &field4, &field5, &field6);
		else
		if(field1 == "touch_x_control")
			assignEffect(TOUCH_X, current_fx_block, &field2, &field3, &field4, &field5, &field6);
		else
		if(field1 == "touch_y_control")
			assignEffect(TOUCH_Y, current_fx_block, &field2, &field3, &field4, &field5, &field6);
		else
		if(field1 == "ball_x_control")
			assignEffect(BALL_X, current_fx_block, &field2, &field3, &field4, &field5, &field6);
		else
		if(field1 == "ball_y_control")
			assignEffect(BALL_Y, current_fx_block, &field2, &field3, &field4, &field5, &field6);
		else
		if(field1 == "drag_x_control")
			assignEffect(DRAG_X, current_fx_block, &field2, &field3, &field4, &field5, &field6);
		else
		if(field1 == "drag_y_control")
			assignEffect(DRAG_Y, current_fx_block, &field2, &field3, &field4, &field5, &field6);
		else
		if(field1 == "string_touch_x_control")
		{
			switch(atoi(field2.c_str()))
			{
				case 0: assignEffect(STRING_TOUCH_X_0, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 1: assignEffect(STRING_TOUCH_X_1, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 2: assignEffect(STRING_TOUCH_X_2, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 3: assignEffect(STRING_TOUCH_X_3, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 4: assignEffect(STRING_TOUCH_X_4, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 5: assignEffect(STRING_TOUCH_X_5, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
			}
		}
		else
		if(field1 == "string_drag_x_control")
		{
			switch(atoi(field2.c_str()))
			{
				case 0: assignEffect(STRING_DRAG_X_0, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 1: assignEffect(STRING_DRAG_X_1, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 2: assignEffect(STRING_DRAG_X_2, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 3: assignEffect(STRING_DRAG_X_3, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 4: assignEffect(STRING_DRAG_X_4, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
				case 5: assignEffect(STRING_DRAG_X_5, current_fx_block, &field3, &field4, &field5, &field6, &field7); break;
			}
		}
		else
		if(field1 == "midi_channel")
			current_setting.string_midi_out_channel[current_string] = atoi(field2.c_str());
	}

	preset_file.close();

	std::cout << "Preset file " << filepath << " loaded." << std::endl << std::flush;
}

void Synth::printCurrentPreset(void)
{
	std::cout << "Preset name: " << current_setting.preset_name << " ";
	std::cout << "LFO rate: " << current_setting.lfo_rate << std::endl;
	std::cout << "LFO frequency depth: " << current_setting.lfo_frequency_depth << " ";
	std::cout << "LFO amp depth: " << current_setting.lfo_amp_depth << " ";
	std::cout << "LFO pitch depth: " << current_setting.lfo_pitch_depth << " ";
	
	for(int s = 0; s < 6; s++)
	{
		for(int v = 0; v < current_setting.voices[s].size(); v++)
		{
			struct voice *source_voice = current_setting.voices[s].at(v);

			std::cout << "String: " << s << " Voice: " << v << std::endl;
			std::cout << "Amp env attack: " << source_voice->amp_env_attack << " ";
			std::cout << "Amp env decay: " << source_voice->amp_env_decay << " ";
			std::cout << "Amp env release: " << source_voice->amp_env_release << std::endl;
			std::cout << "Filter frequency: " << source_voice->filter_frequency << " ";
			std::cout << "Filter resonance: " << source_voice->filter_resonance << std::endl;
			std::cout << "Detune course: " << source_voice->detune_course << " ";
			std::cout << "Detune fine: " << source_voice->detune_fine << std::endl;
			std::cout << "Vibrate rate: " << source_voice->vibrate_rate << " ";
			std::cout << "Vibrate depth: " << source_voice->vibrate_depth << " ";
			std::cout << "Vibrate delay: " << source_voice->vibrate_delay << std::endl;
		}
	}
}

bool Synth::lookupEffect(std::string s, unsigned char fxb, struct assignable_effect *ae)
{
	ae->channel = 0;
	ae->output = SYNTH;

	if(s == "distortion_type")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x60; }
	else if(s == "distortion_on")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x61; }
	else if(s == "distortion_drive")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x62; }
	else if(s == "distortion_tone")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x64; }
	else if(s == "distortion_post_gain")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x66; }
	else if(s == "compressor_on")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x41; }
	else if(s == "compressor_attack")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x42; }
	else if(s == "compressor_release")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x43; }
	else if(s == "compressor_threshold")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x44; }
	else if(s == "compressor_ratio")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x45; }
	else if(s == "compressor_boost")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x46; }
	else if(s == "compressor_knee")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x47; }
	else if(s == "wahwah_type")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x51; }
	else if(s == "wahwah_frequency")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x52; }
	else if(s == "wahwah_resonance")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x53; }
	else if(s == "wahwah_sensitivity")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x54; }
	else if(s == "modulation_level")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x31; }
	else if(s == "modulation_chorus_delay_time")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x32; }
	else if(s == "modulation_chorus_feedback")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x33; }
	else if(s == "modulation_chorus_hpf")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x34; }
	else if(s == "modulation_delay_feedback_filter")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x35; }
	else if(s == "modulation_rate")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x36; }
	else if(s == "modulation_depth")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x37; }
	else if(s == "modulation_tremolo")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x38; }
	else if(s == "delay_on")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x59; }
	else if(s == "delay_mode")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x5A; }
	else if(s == "delay_pre_lp")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x5B; }
	else if(s == "delay_level")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x5C; }
	else if(s == "delay_time")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x5D; }
	else if(s == "delay_feedback")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x5E; }
	else if(s == "delay_feedback_filter")
	{ ae->type = AE_NRPN; ae->nrpn_hi = 0x3A + fxb; ae->nrpn_lo = 0x5F; }
/*	else if(s == "reverb_type")
	{
		ae->sysex = "\xF0" "\x41" "\x00" "\x42" "\x12" "\x40" "\x01" "\x30" "\x0" "\x0" "\xF7";
		ae->type = AE_SYSEX;
	}
	else if(s == "reverb_character")
	{
		ae->sysex = "\xF0" "\x41" "\x00" "\x42" "\x12" "\x40" "\x01" "\x31" "\x0" "\x0" "\xF7";
		ae->type = AE_SYSEX;
	}
	else if(s == "reverb_pre_lpf")
	{
		ae->sysex = "\xF0" "\x41" "\x00" "\x42" "\x12" "\x40" "\x01" "\x32" "\x0" "\x0" "\xF7";
		ae->type = AE_SYSEX;
	}
	else if(s == "reverb_level")
	{
		ae->sysex = "\xF0" "\x41" "\x00" "\x42" "\x12" "\x40" "\x01" "\x33" "\x0" "\x0" "\xF7";
		ae->type = AE_SYSEX;
	}
	else if(s == "reverb_time")
	{
		ae->sysex = "\xF0" "\x41" "\x00" "\x42" "\x12" "\x40" "\x01" "\x34" "\x0" "\x0" "\xF7";
		ae->type = AE_SYSEX;
	}
	else if(s == "reverb_delay_feedback")
	{
		ae->sysex = "\xF0" "\x41" "\x00" "\x42" "\x12" "\x40" "\x01" "\x35" "\x0" "\x0" "\xF7";
		ae->type = AE_SYSEX;
	}
	else if(s == "reverb_pre_delay")
	{
		ae->sysex = "\xF0" "\x41" "\x00" "\x42" "\x12" "\x40" "\x01" "\x37" "\x0" "\x0" "\xF7";
		ae->type = AE_SYSEX;
	}
*/	else
		return false;

	return true;
}

void Synth::assignEffect(unsigned char touch_control, unsigned char fxb, std::string *effect_control, std::string *extra_field, std::string *extra_field_2, std::string *extra_field_3, std::string *extra_field_4)
{
	struct assignable_effect *ae = new struct assignable_effect;

	//if the effect is not found, then it is not part of the synth
	//which means assign the control should be assigned to a midi parameter
	if(!lookupEffect(effect_control->c_str(), fxb, ae))
	{
		if(effect_control->compare("velocity") == 0)
		{
			ae->type = AE_VELOCITY;
			ae->string = atoi(extra_field->c_str());
			if(touch_control != VARIATION)
				ae->inverse = (extra_field_2->compare("inverse") == 0);
			else
			{
				ae->variation_start = atoi(extra_field_2->c_str());
				ae->variation_end = atoi(extra_field_3->c_str());
			}
		}
		else
		if(effect_control->compare("midi_pitch") == 0)
		{
			ae->output = MIDI_OUT;
			ae->type = AE_PITCH;
			ae->channel = atoi(extra_field->c_str());
			if(touch_control != VARIATION)
				ae->inverse = (extra_field_2->compare("inverse") == 0);
			else
			{
				ae->variation_start = atoi(extra_field_2->c_str());
				ae->variation_end = atoi(extra_field_3->c_str());
			}
		}
		else
		if(effect_control->compare("synth_pitch") == 0)
		{
			ae->output = SYNTH;
			ae->type = AE_PITCH; //affects all assigned channels
			ae->channel = atoi(extra_field->c_str());
			if(touch_control != VARIATION)
				ae->inverse = (extra_field_2->compare("inverse") == 0);
			else
			{
				ae->variation_start = atoi(extra_field_2->c_str());
				ae->variation_end = atoi(extra_field_3->c_str());
			}
		}
		else
		if(effect_control->compare("midi_cc") == 0)
		{
			ae->output = MIDI_OUT;
			ae->type = AE_CC;	//fix: add support for MIDI NRPN and SYSEX later
			ae->cc = atoi(extra_field->c_str());
			ae->channel = atoi(extra_field_2->c_str());
			if(touch_control != VARIATION)
				ae->inverse = (extra_field_3->compare("inverse") == 0);
			else
			{
				ae->variation_start = atoi(extra_field_3->c_str());
				ae->variation_end = atoi(extra_field_4->c_str());
			}
		}
		if(effect_control->compare("synth_cc") == 0)
		{
			ae->output = SYNTH;
			ae->type = AE_CC;	//fix: add support for MIDI NRPN and SYSEX later
			ae->cc = atoi(extra_field->c_str());
			ae->channel = atoi(extra_field_2->c_str());
			if(touch_control != VARIATION)
				ae->inverse = (extra_field_3->compare("inverse") == 0);
			else
			{
				ae->variation_start = atoi(extra_field_3->c_str());
				ae->variation_end = atoi(extra_field_4->c_str());
			}
		}
	}
	else //if synth effect setting
	{
		if(touch_control != VARIATION) //if effect setting
			ae->inverse = (extra_field->compare("inverse") == 0);
		else //detune_course etc - not tested yet! - MZ
		{
			ae->channel = atoi(extra_field->c_str());
			ae->variation_start = atoi(extra_field_2->c_str());
			ae->variation_end = atoi(extra_field_3->c_str());
		}
	}

	if(touch_control == TOUCH_X)
		AE_touch_x.push_back(ae);
	else
	if(touch_control == TOUCH_Y)
		AE_touch_y.push_back(ae);
	else
	if(touch_control == BALL_X)
		AE_ball_x.push_back(ae);
	else
	if(touch_control == BALL_Y)
		AE_ball_y.push_back(ae);
	else
	if(touch_control == DRAG_X)
		AE_drag_x.push_back(ae);
	else
	if(touch_control == DRAG_Y)
		AE_drag_y.push_back(ae);
	else
	if(touch_control == STRING_TOUCH_X_0)
		AE_string_touch_x_0.push_back(ae);
	else
	if(touch_control == STRING_TOUCH_X_1)
		AE_string_touch_x_1.push_back(ae);
	else
	if(touch_control == STRING_TOUCH_X_2)
		AE_string_touch_x_2.push_back(ae);
	else
	if(touch_control == STRING_TOUCH_X_3)
		AE_string_touch_x_3.push_back(ae);
	else
	if(touch_control == STRING_TOUCH_X_4)
		AE_string_touch_x_4.push_back(ae);
	else
	if(touch_control == STRING_TOUCH_X_5)
		AE_string_touch_x_5.push_back(ae);
	else
	if(touch_control == STRING_DRAG_X_0)
		AE_string_drag_x_0.push_back(ae);
	else
	if(touch_control == STRING_DRAG_X_1)
		AE_string_drag_x_1.push_back(ae);
	else
	if(touch_control == STRING_DRAG_X_2)
		AE_string_drag_x_2.push_back(ae);
	else
	if(touch_control == STRING_DRAG_X_3)
		AE_string_drag_x_3.push_back(ae);
	else
	if(touch_control == STRING_DRAG_X_4)
		AE_string_drag_x_4.push_back(ae);
	else
	if(touch_control == STRING_DRAG_X_5)
		AE_string_drag_x_5.push_back(ae);
	else
	if(touch_control == VARIATION)
		AE_variation.push_back(ae);
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
	midi.sendSysex(SYNTH, sysex, 8);

	current_setting.master_volume = val;
}

void Synth::setChannel(int str, int voice_index, int val)
{
	current_setting.voices[str].at(voice_index)->channel = val;
}

void Synth::setWave(int str, int voice_index, int bank, int program)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendProgramChange(channel, bank, program);
	current_setting.voices[str].at(voice_index)->bank = bank;
	current_setting.voices[str].at(voice_index)->wavetable_index = program;
}

void Synth::setAmpEnvAttack(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x63, val);
	current_setting.voices[str].at(voice_index)->amp_env_attack = val;
}

void Synth::setAmpEnvDecay(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x64, val);
	current_setting.voices[str].at(voice_index)->amp_env_decay = val;
}

void Synth::setAmpEnvRelease(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x66, val);
	current_setting.voices[str].at(voice_index)->amp_env_release = val;
}

void Synth::setFilterFrequency(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x20, val);
	current_setting.voices[str].at(voice_index)->filter_frequency = val;
}

void Synth::setFilterResonance(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x21, val);
	current_setting.voices[str].at(voice_index)->filter_resonance = val;
}

void Synth::setDetuneCourse(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendRPN(SYNTH, channel, 0x00, 0x02, val);
	current_setting.voices[str].at(voice_index)->detune_course = val;
}

void Synth::setDetuneFine(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendRPN(SYNTH, channel, 0x00, 0x01, val);
	current_setting.voices[str].at(voice_index)->detune_fine = val;
}

void Synth::setVibrateRate(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x08, val);
	current_setting.voices[str].at(voice_index)->vibrate_rate = val;
}

void Synth::setVibrateDepth(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x09, val);
	current_setting.voices[str].at(voice_index)->vibrate_depth = val;
}

void Synth::setVibrateDelay(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendNRPN(SYNTH, channel, 0x01, 0x0A, val);
	current_setting.voices[str].at(voice_index)->vibrate_delay = val;
}

void Synth::setChannelVolume(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendCC(SYNTH, channel, 7, val);
	current_setting.voices[str].at(voice_index)->channel_volume = val;
}

void Synth::setPortamentoTime(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendCC(SYNTH, channel, 5, val);
	current_setting.voices[str].at(voice_index)->portamento_time = val;
}

void Synth::setReverbSend(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendCC(SYNTH, channel, 91, val);
	current_setting.voices[str].at(voice_index)->reverb_send = val;
}

void Synth::setPan(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendCC(SYNTH, channel, 10, val);
	current_setting.voices[str].at(voice_index)->pan = val;
}

void Synth::setPitchBendSemitones(int str, int voice_index, int val)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	midi.sendRPN(SYNTH, channel, 0x00, 0x00, val);
	current_setting.voices[str].at(voice_index)->pitch_bend_semitones = val;
}

void Synth::setFxBlockOn(int str, int voice_index, int fxb, bool state)
{
	int channel = current_setting.voices[str].at(voice_index)->channel;
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x40+channel, 0x22, fxb+1, 0x00, 0xF7};
	if(!state)
		sysex[8] = 0;	//turn off
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.voices[str].at(voice_index)->insert_fx_block_on[fxb] = state;
}

void Synth::setDistortionOn(int fxb, bool state)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x61, state);
	current_setting.fx_block[fxb].distortion.on = state;
}

void Synth::setDistortionPreGain(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x62, val);
	current_setting.fx_block[fxb].distortion.pre_gain = val;
}

void Synth::setDistortionType(int fxb, int val)
{
	//modified for new algorithms (was 0x63)
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x60, val);
	current_setting.fx_block[fxb].distortion.type = val;
}

void Synth::setDistortionLpfFrequency(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x64, val);
	current_setting.fx_block[fxb].distortion.lpf_frequency = val;
}

void Synth::setDistortionLpfResonance(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x65, val);
	current_setting.fx_block[fxb].distortion.lpf_resonance = val;
}

void Synth::setDistortionPostGain(int fxb, int val)
{
	midi.sendNRPN(SYNTH, 0, 0x3A+fxb, 0x66, val);
	current_setting.fx_block[fxb].distortion.post_gain = val;
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
}

void Synth::setReverbCharacter(int val)
{
	unsigned char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x31, val, 0x00, 0xF7};
	midi.sendSysex(SYNTH, sysex, 11);
	current_setting.reverb.character = val;
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

void Synth::sendNoteOn(unsigned char str, unsigned char note)
{
	if(current_setting.string_midi_out_channel[str] >= 0)
	{
		sendVariation(current_setting.string_midi_out_channel[str]);
		midi.sendNoteOn(MIDI_OUT, current_setting.string_midi_out_channel[str], note, velocity[str]);
	}
	else
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
		{
			sendVariation(current_setting.voices[str].at(i)->channel);
			midi.sendNoteOn(SYNTH, current_setting.voices[str].at(i)->channel, note, velocity[str]);
		}
	}
}

void Synth::sendNoteOff(unsigned char str, unsigned char note)
{
	if(current_setting.string_midi_out_channel[str] >= 0)
		midi.sendNoteOff(MIDI_OUT, current_setting.string_midi_out_channel[str], note, 0);
	else
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
			midi.sendNoteOff(SYNTH, current_setting.voices[str].at(i)->channel, note, 0);
	}
}

void Synth::sendCC(unsigned char str, unsigned char control, unsigned char val)
{
	if(str == 6)
	{
		for(int s = 0; s < 6; s++)
			for(int i = 0; i < current_setting.voices[str].size(); i++)
				midi.sendCC(SYNTH, current_setting.voices[s].at(i)->channel, control, val);
	}
	else
		for(int i = 0; i < current_setting.voices[str].size(); i++)
			midi.sendCC(SYNTH, current_setting.voices[str].at(i)->channel, control, val);
}

unsigned char Synth::midiScale(int number, int scaler)
{
	int val = (number * 127) / scaler;

	return (unsigned char) val;
}

void Synth::sendControl(unsigned char control_type, int val, int scaler)
{
	std::vector<struct assignable_effect *> *control_list;

	switch(control_type)
	{
		case TOUCH_X: control_list = &AE_touch_x; break;
		case TOUCH_Y: control_list = &AE_touch_y; break;
		case BALL_X: control_list = &AE_ball_x; break;
		case BALL_Y: control_list = &AE_ball_y; break;
		case DRAG_X: control_list = &AE_drag_x; break;
		case DRAG_Y: control_list = &AE_drag_y; break;
		case STRING_TOUCH_X_0: control_list = &AE_string_touch_x_0; break;
		case STRING_TOUCH_X_1: control_list = &AE_string_touch_x_1; break;
		case STRING_TOUCH_X_2: control_list = &AE_string_touch_x_2; break;
		case STRING_TOUCH_X_3: control_list = &AE_string_touch_x_3; break;
		case STRING_TOUCH_X_4: control_list = &AE_string_touch_x_4; break;
		case STRING_TOUCH_X_5: control_list = &AE_string_touch_x_5; break;
		case STRING_DRAG_X_0: control_list = &AE_string_drag_x_0; break;
		case STRING_DRAG_X_1: control_list = &AE_string_drag_x_1; break;
		case STRING_DRAG_X_2: control_list = &AE_string_drag_x_2; break;
		case STRING_DRAG_X_3: control_list = &AE_string_drag_x_3; break;
		case STRING_DRAG_X_4: control_list = &AE_string_drag_x_4; break;
		case STRING_DRAG_X_5: control_list = &AE_string_drag_x_5; break;
		case VARIATION: control_list = &AE_variation; break;
	}

	for(int i = 0; i < control_list->size(); i++)
	{
		struct assignable_effect *ae = control_list->at(i);

		sendToEffect(ae, val, scaler);
	}
}

void Synth::sendVariation(int channel)
{
	for(int i = 0; i < AE_variation.size(); i++)
	{
		struct assignable_effect *ae = AE_variation.at(i);
		if(ae->channel == channel)
		{
			int r = (rand() % (ae->variation_end + 1 - ae->variation_start)) + ae->variation_start;
			sendToEffect(ae, r, 127);
		}
	}
}

void Synth::sendToEffect(struct assignable_effect *ae, int val, int scaler)
{
	unsigned char mval = midiScale(val, scaler);

	unsigned char final_val = mval;
	if(ae->inverse) final_val = 127 - final_val;

	if(ae->type == AE_NRPN)
		midi.sendNRPN(ae->output, ae->channel, ae->nrpn_hi, ae->nrpn_lo, final_val);
	else
	if(ae->type == AE_SYSEX)
	{
//		if(ae->output == SYNTH)
//			ae->sysex.at(8) = final_val;

//		midi.sendSysex(ae->output, &ae->sysex, ae->sysex_size);
	}
	else
	if(ae->type == AE_CC)
	{
		midi.sendCC(ae->output, ae->channel, ae->cc, final_val);
	}
	else
	if(ae->type == AE_PITCH)
	{
		//fix: add inverse option for pitch
		unsigned int scaled_pitch = (val * 0x4000) / scaler; //yes... we scale up... resolution limited
		midi.sendPitch(ae->output, ae->channel, scaled_pitch);
	}
	else
	if(ae->type == AE_VELOCITY)
		setVelocity(ae->string, final_val);
}

void Synth::setVelocity(unsigned char str, unsigned char v) {
	//GET RID OF THIS
	if(str == 6)
	{
		for(int i = 0; i < 6; i++)
			velocity[i] = v;
	}
	else
		velocity[str] = v;
}

void Synth::setPitch(unsigned char output, unsigned char str, unsigned int p)
{
	if(output == SYNTH)
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
			midi.sendPitch(SYNTH, current_setting.voices[str].at(i)->channel, p);
	}
	else
		midi.sendPitch(MIDI_OUT, current_setting.string_midi_out_channel[str], p);
}

void Synth::setSynthCC(unsigned char output, unsigned char str, unsigned char cc, unsigned char v)
{
	if(output == SYNTH)
	{
		for(int i = 0; i < current_setting.voices[str].size(); i++)
			midi.sendCC(SYNTH, current_setting.voices[str].at(i)->channel, cc, v);
	}
}
