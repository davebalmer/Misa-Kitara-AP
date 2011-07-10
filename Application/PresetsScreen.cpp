
#include <cstdlib>
#include <iostream>

#ifdef Linux
#include <dirent.h>
#endif

using namespace std;

#include <string>
#include <vector>

#ifdef WIN32
#include <Windows.h>
#endif

//#ifdef Linux
#if defined(Linux) && !defined(MISA_APPLICATION)
#include <Synth.h>
#endif

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "MisaDialogueBox.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "SynthScreen.h"
#include "ControlAssignments.h"
#include "CompressionScreen.h"
#include "DistortionScreen.h"
#include "ModulationScreen.h"
#include "DelayScreen.h"
#include "ReverbScreen.h"
#include "MixerScreen.h"
#include "EqScreen.h"

#include "PresetsScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define PRESETS_TYPE_XPOS 80
#define PRESETS_TYPE_YPOS 120

#define PRESETS_XOFFSET 2
#define PRESETS_YOFFSET 2

#define PRESETS_ITEM_POS 50
#define PRESETS_ITEMBLANK 80

///////////////////////////////////////////////////////////////////////////////
//

extern std::string working_directory;

typedef enum __PRESETSITEMS
{
	PRESETS_INDICATOR,
	PRESETS_SLIDEWINDOW,
	PRESETS_LOAD,
	PRESETS_SAVE,
	PRESETS_MAX
} PRESETSITEMS;

typedef enum __PRESETS_ID
{
	PRESETS_ID_LOAD=GUI_ID_PRESETS_BASE,
	PRESETS_ID_SAVE,
	PRESETS_ID_ITEMSBASE,
	PRESETS_ID_MAX
} PRESETS_ID;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hPresets;
static void PresetsProc(WM_MESSAGE* pMsg);
static WM_HWIN hPresetsItems[PRESETS_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 PresetsDeleteItems();
static U8 SlideCreateItems(WM_HWIN hParent);
static U8 SlideDeleteItems();

static U8 SlidingBorder;

static std::vector<std::string> preset_filenames;
static BUTTON_Handle* pPresetsItems = 0;

static SYNTH_SETTING synthSetting;

static int SaveEffect();
static int LoadEffect();

bool GetCurrentSetting(PSYNTH_SETTING pSetting);
void SynthSavePreset(PSYNTH_SETTING pSetting,std::string filepath);

///////////////////////////////////////////////////////////////////////////////
//

U8 CreatePresetsScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	hPresets = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, PresetsProc, 0);
	return 0;
}

U8 DeletePresetsScreen()
{
	PresetsDeleteItems();
	WM_DeleteWindow(hPresets);
	return 0;
}

U8 TopPresetsScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hPresets);
	if(hPresetsItems[PRESETS_SLIDEWINDOW])
	{
		SlideDeleteItems();
		SlideCreateItems(hPresetsItems[PRESETS_SLIDEWINDOW]);
	}
	//WM_BringToTop(hPresets);
	return 0;
}

static U8 ReadPresetsDir()
{
#ifdef Linux
	DIR *dp;
	struct dirent *ep;

	preset_filenames.clear();

	dp = opendir((working_directory+"/presets/").c_str());

	if(dp != NULL)
	{
		while((ep = readdir(dp)) != NULL)
		{
			std::string s = ep->d_name;
			if(s.size() > 3)
				if(s.substr(s.size() - 3, 3) == ".mz")
				{
					s.erase(s.size()-3,s.size());
					preset_filenames.push_back(s);
				}
		}
	}

	sort(preset_filenames.begin(), preset_filenames.end());

	closedir(dp);
#else
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	DWORD error;

	hFind = FindFirstFile("presets\\*.mz", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		string s;
		s = FindFileData.cFileName;
		if(s.substr(s.size() - 3, 3) == ".mz")
		{
			s.erase(s.size() - 3,s.size());
			preset_filenames.push_back(s);
		}
		while(FindNextFile(hFind,&FindFileData))
		{
			s = FindFileData.cFileName;
			if(s.substr(s.size() - 3, 3) == ".mz")
			{
				s.erase(s.size() - 3,s.size());
				preset_filenames.push_back(s);
			}
		}
		error = GetLastError();
	}
	FindClose(hFind);
#if 0
	hFind = FindFirstFile("presets\\*.mz", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	else
	{
		string s;
		s = FindFileData.cFileName;
		preset_filenames.push_back(s);
		while(FindNextFile(hFind,&FindFileData))
		{
			s = FindFileData.cFileName;
			if(s.substr(s.size() - 3, 3) == ".mz")
			{
				s.erase(s.size() - 3,s.size());
				preset_filenames.push_back(s);
			}
		}
		error = GetLastError();
	}
	FindClose(hFind);
#endif
#endif
	return preset_filenames.size();
}

static U8 PresetsCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_CLOSE);
	memset(hPresetsItems,0,sizeof(hPresetsItems));
	x=0;
	y = bmEMPTYTITLEBAR.YSize;
	hPresetsItems[PRESETS_INDICATOR] = CreateSlidePanel(x,y+INDICATORFRAME,WM_GetWindowSizeX(hParent),WM_GetWindowSizeY(hParent)-200-y,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	x = 1;
	y = 502;
	hPresetsItems[PRESETS_LOAD] = BUTTON_CreateAsChild(x,y,bmPRESET_LOAD_UN.XSize,bmPRESET_LOAD_UN.YSize,hParent,PRESETS_ID_LOAD,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hPresetsItems[PRESETS_LOAD],0);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_LOAD],BUTTON_CI_UNPRESSED,&bmPRESET_LOAD_UN);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_LOAD],BUTTON_CI_PRESSED,&bmPRESET_LOAD_SE);
	x += bmPRESET_LOAD_UN.XSize;
	x += PRESETS_XOFFSET;
	hPresetsItems[PRESETS_SAVE] = BUTTON_CreateAsChild(x,y,bmPRESET_SAVE_UN.XSize,bmPRESET_SAVE_UN.YSize,hParent,PRESETS_ID_SAVE,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hPresetsItems[PRESETS_SAVE],0);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_SAVE],BUTTON_CI_UNPRESSED,&bmPRESET_SAVE_UN);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_SAVE],BUTTON_CI_PRESSED,&bmPRESET_SAVE_SE);
	return 0;
}

static U8 PresetsDeleteItems()
{
	if(hPresetsItems[PRESETS_INDICATOR])
	{
		DeleteSlidePanel(hPresetsItems[PRESETS_INDICATOR]);
		hPresetsItems[PRESETS_INDICATOR] = 0;
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static void PresetsProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	WM_HWIN hFocus;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		PresetsCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		PresetsDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		x=1;
		y=0;
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		x = 222;
		y += bmEMPTYTITLEBAR.YSize;
		y += PRESETS_YOFFSET;
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_DispStringHCenterAt("Presets", x / 2, 5);
		break;
	case WM_NOTIFY_PARENT:
		if(DispatchCommComponent(pMsg,&hCOMPcomm))
		{
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("PRESETS button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case PRESETS_ID_LOAD:
					LoadEffect();
					break;
				case PRESETS_ID_SAVE:
					hFocus = WM_GetFocussedWindow();
					if(hFocus)
					{
						if(GUI_ID_OK == Misa_ConfirmBox("Are you sure? You will OVERWRITE the selected preset.","Delete this voice!",GUI_MESSAGEBOX_CF_MODAL))
						{
							WM_SetFocus(hFocus);
							SaveEffect();
						}
						else
						{
							WM_SetFocus(hFocus);
						}
					}
					break;
				case COMMON_ID_CLOSE:
					SlideDeleteItems();
					break;
				default:
					;
				}
				DEBUG_STDOUT("PRESETS button released!");
				break;
			}
		}
		break;
	case WM_TOUCH:
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Sliding window support
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static WM_HWIN hSlideWindow;
static void SlideWindowProc(WM_MESSAGE* pMsg);
static void IndicatorProc(WM_MESSAGE* pMsg);

static U8 IndicatorCreateItems(WM_HWIN hParent)
{
	hPresetsItems[PRESETS_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,WM_GetWindowSizeX(hParent),WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hPresetsItems[PRESETS_SLIDEWINDOW])
	{
		SlideDeleteItems();
		WM_DeleteWindow(hPresetsItems[PRESETS_SLIDEWINDOW]);
		hPresetsItems[PRESETS_SLIDEWINDOW] = 0;
	}
	return 0;
}

WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style)
{
	return WM_CreateWindowAsChild(x0,y0,width,height,hParent,Style,IndicatorProc,0);
}

static U8 DeleteSlidePanel(WM_HWIN hSlidePanel)
{
	IndicatorDeleteItems();
	WM_DeleteWindow(hSlidePanel);
	return 0;
}

static void IndicatorProc(WM_MESSAGE* pMsg)
{
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		IndicatorCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		IndicatorDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static U8 SlideCreateItems(WM_HWIN hParent)
{
	int x,y,i,size;
	x = 0;
	y = 0;
	preset_filenames.clear();
	size = ReadPresetsDir();
	if(size)
	{
		pPresetsItems = (BUTTON_Handle*)malloc(sizeof(BUTTON_Handle*)*size);
	}
	//preset_filenames.size();
	for(i=0;i<size;i++)
	{
		pPresetsItems[i] = MisaItem_CreateEx(PRESETS_ITEM_POS+(bmSELECT.XSize+PRESETS_ITEMBLANK)*(i%3), bmSELECT.YSize*(i/3), bmSELECT.XSize, bmSELECT.YSize,
			hParent, PRESETS_ID_ITEMSBASE+i, WM_CF_SHOW|WM_CF_MEMDEV, preset_filenames[i].c_str(),0, &bmSELECT);
	}
	x = WM_GetWindowSizeX(hParent);
	y = size%3?bmSELECT.YSize*((i/3)+1):bmSELECT.YSize*(i/3);
	WM_SetSize(hParent, x, WM_GetWindowSizeY(hParent)<y?y:WM_GetWindowSizeY(hParent));
	return 0;
}

static U8 SlideDeleteItems()
{
	int i,size;
	size = preset_filenames.size();
	for(i=0;i<size;i++)
	{
		MisaItem_Delete(pPresetsItems[i]);
	}
	if(pPresetsItems)
	{
		free(pPresetsItems);
		pPresetsItems = 0;
	}
	preset_filenames.clear();
	return 0;
}

static int SaveEffect()
{
	int size,pos;
	WM_HWIN hFocus;
	size = preset_filenames.size();
	hFocus = WM_GetFocussedWindow();
	if(WM_GetParent(hFocus) != hPresetsItems[PRESETS_SLIDEWINDOW])
	{
		return 0;
	}
	if(hFocus)
	{
		pos = WM_GetId(hFocus)-PRESETS_ID_ITEMSBASE;
		if(pos < size)
		{
			GetCurrentSetting(&synthSetting);
			SynthSavePreset(&synthSetting, working_directory+"/presets/" + preset_filenames[pos]+".mz");
		}
	}
	return 1;
}

static int LoadEffect()
{
	int size,pos;
	WM_HWIN hFocus;
	size = preset_filenames.size();
	hFocus = WM_GetFocussedWindow();
	if(WM_GetParent(hFocus) != hPresetsItems[PRESETS_SLIDEWINDOW])
	{
		return 0;
	}
	if(hFocus)
	{
		pos = WM_GetId(hFocus)-PRESETS_ID_ITEMSBASE;
		if(pos < size)
		{
			SynthTurnNotesOff();
			SynthLoadPreset(preset_filenames[pos]+".mz");
			UpdateSynthSetting();
		}
	}
	return 1;
}

static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int x,y;
	int NCode,Id;
	GUI_PID_STATE* pPID_State;
	static GUI_PID_STATE PID_LastState;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		//SlideCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		//SlideDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			break;
		case WM_NOTIFICATION_RELEASED:
			break;
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				int dy;
				if(!WM_HasCaptured(pMsg->hWin))
				{
					WM_SetCapture(pMsg->hWin,0);
					PID_LastState = *pPID_State;
				}
				else
				{
					y = WM_GetWindowOrgY(pMsg->hWin);
					x = WM_GetWindowSizeY(pMsg->hWin);
					dy = pPID_State->y-PID_LastState.y;
					if(dy > 0)
					{
						WM_MoveWindow(pMsg->hWin,0,dy>bmEMPTYTITLEBAR.YSize+INDICATORFRAME-y?bmEMPTYTITLEBAR.YSize+INDICATORFRAME-y:dy);
					}
					else if(dy < 0)
					{
						WM_MoveWindow(pMsg->hWin,0,dy<GUI_GetScreenSizeY()-INDICATORFRAME-100-y-x?GUI_GetScreenSizeY()-100-INDICATORFRAME-y-x:dy);
					}
					//WM_MoveWindow(pMsg->hWin,0,pPID_State->y-PID_LastState.y);
				}
			}
			else
			{
				WM_ReleaseCapture();
				y = WM_GetWindowOrgY(pMsg->hWin);
				x = WM_GetWindowSizeY(pMsg->hWin);
				if(bmEMPTYTITLEBAR.YSize+INDICATORFRAME < y)
				{
					WM_MoveWindow(pMsg->hWin,0,bmEMPTYTITLEBAR.YSize+INDICATORFRAME-y);
				}
				else if(GUI_GetScreenSizeY()-INDICATORFRAME-100 > y+x)
				{
					WM_MoveWindow(pMsg->hWin,0,GUI_GetScreenSizeY()-100-INDICATORFRAME-y-x);
				}
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hPresets);
			}
		}
		else
		{
			WM_ReleaseCapture();
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void UpdateSynthSetting()
{
#if 1
	UpdateSynthSettingEx(MISAMODULE_ALL);
#else
	SYNTH_INFO synth;
	DISTORTION_INFO distortion;
	COMPRESSION_INFO compression;
	DELAY_INFO delay;
	MODULATION_INFO modulation;
	REVERB_INFO reverb;
	MIXER_INFO mixer;
	EQ_INFO eq;
	GetCurrentSetting(&synthSetting);
	// Synth
	memcpy(synth.tune,synthSetting.tuning,6);
	UpdateSynthInfo(&synth);
	// Distortion
	GetDistortionInfo(&distortion);
	distortion.On = synthSetting.fx_block[distortion.Fx].distortion.on;
	distortion.type = synthSetting.fx_block[distortion.Fx].distortion.type;
	distortion.drive = synthSetting.fx_block[distortion.Fx].distortion.drive;
	distortion.tone = synthSetting.fx_block[distortion.Fx].distortion.tone;
	distortion.level = synthSetting.fx_block[distortion.Fx].distortion.level;
	distortion.noisereduction = synthSetting.fx_block[distortion.Fx].distortion.noise_reduction;
	distortion.booster = synthSetting.fx_block[distortion.Fx].distortion.booster;
	UpdateDistortionInfo(&distortion);
	// Compression
	GetCompressionInfo(&compression);
	compression.softhard = synthSetting.fx_block[compression.Fx].compressor.knee;
	compression.On = synthSetting.fx_block[compression.Fx].compressor.on;
	compression.attack = synthSetting.fx_block[compression.Fx].compressor.attack;
	compression.release = synthSetting.fx_block[compression.Fx].compressor.release;
	compression.boost = synthSetting.fx_block[compression.Fx].compressor.boost;
	compression.thres = synthSetting.fx_block[compression.Fx].compressor.threshold;
	compression.ratio = synthSetting.fx_block[compression.Fx].compressor.ratio;
	UpdateCompressionInfo(&compression);
	// Delay
	GetDelayInfo(&delay);
	delay.On = synthSetting.fx_block[delay.Fx].delay.on;
	delay.monostereo = synthSetting.fx_block[delay.Fx].delay.mode;
	delay.pre_lpf = synthSetting.fx_block[delay.Fx].delay.pre_lp;
	delay.time = synthSetting.fx_block[delay.Fx].delay.time;
	delay.level = synthSetting.fx_block[delay.Fx].delay.level;
	delay.fdbk = synthSetting.fx_block[delay.Fx].delay.feedback;
	delay.fdbkfilter = synthSetting.fx_block[delay.Fx].delay.feedback_filter;
	UpdateDelayInfo(&delay);
	// Modulation
	GetModulationInfo(&modulation);
	// Temporary load parameter from default setting
	#if 0// Begin
	ModulationLoadParameter(synthSetting.fx_block[modulation.Fx].modulation.type);
	GetModulationInfo(&modulation);
	synthSetting.fx_block[modulation.Fx].modulation.rate = modulation.mod_rate;
	synthSetting.fx_block[modulation.Fx].modulation.depth = modulation.mod_depth;
	synthSetting.fx_block[modulation.Fx].modulation.level = modulation.level;
	synthSetting.fx_block[modulation.Fx].modulation.chorus_feedback = modulation.fdbk;
	synthSetting.fx_block[modulation.Fx].modulation.delay_feedback_filter = modulation.fdbkfilter;
	synthSetting.fx_block[modulation.Fx].modulation.tremolo = modulation.shape;
	synthSetting.fx_block[modulation.Fx].modulation.chorus_delay_time = modulation.time;
	#endif // End
	modulation.On = synthSetting.fx_block[modulation.Fx].modulation.on?1:0;
	modulation.type = synthSetting.fx_block[modulation.Fx].modulation.type;
	modulation.mod_rate = synthSetting.fx_block[modulation.Fx].modulation.rate;
	modulation.mod_depth = synthSetting.fx_block[modulation.Fx].modulation.depth;
	modulation.level = synthSetting.fx_block[modulation.Fx].modulation.level;
	modulation.fdbk = synthSetting.fx_block[modulation.Fx].modulation.chorus_feedback;
	modulation.fdbkfilter = synthSetting.fx_block[modulation.Fx].modulation.delay_feedback_filter;
	modulation.shape = synthSetting.fx_block[modulation.Fx].modulation.tremolo;
	modulation.time = synthSetting.fx_block[modulation.Fx].modulation.chorus_delay_time;
	UpdateModulationInfo(&modulation);
	// Reverberation
	GetReverbInfo(&reverb);
	//reverb.On = synthSetting.reverb
	reverb.type = synthSetting.reverb.type;
	reverb.pre_lpf = synthSetting.reverb.pre_lpf;
	reverb.character = synthSetting.reverb.character;
	reverb.level = synthSetting.reverb.level;
	reverb.time = synthSetting.reverb.time;
	reverb.delayfdbk = synthSetting.reverb.delay_feedback;
	reverb.delaytime = synthSetting.reverb.pre_delay_time;
	UpdateReverbInfo(&reverb);
	// Mixer
	GetMixerInfo(&mixer);
	mixer.lowcut = synthSetting.fx_block[mixer.Fx].mixer.low_cut_filter_frequency;
	mixer.highcut = synthSetting.fx_block[mixer.Fx].mixer.high_cut_filter_frequency;
	mixer.inputgain = synthSetting.fx_block[mixer.Fx].mixer.input_gain;
	mixer.level = synthSetting.fx_block[mixer.Fx].mixer.output_level;
	mixer.reverbsend = synthSetting.fx_block[mixer.Fx].mixer.reverb_send;
	UpdateMixerInfo(&mixer);
	// Eq
	eq.lowbandgain = synthSetting.equalizer.lowest.gain;
	eq.lowbandfreq = synthSetting.equalizer.lowest.frequency;
	eq.lowmidbandQ = synthSetting.equalizer.low_mid_q;
	eq.highmidbandgain = synthSetting.equalizer.higher.gain;
	eq.highmidbandfreq = synthSetting.equalizer.higher.frequency;
	eq.highmidbandQ = synthSetting.equalizer.high_mid_q;
	eq.highbandgain = synthSetting.equalizer.highest.gain;
	eq.highbandfreq = synthSetting.equalizer.highest.frequency;
	UpdateEqInfo(&eq);
#endif
}

void UpdateSynthSettingEx(U32 module, U8 reloadFromSystem)
{
	SYNTH_INFO synth;
	DISTORTION_INFO distortion;
	COMPRESSION_INFO compression;
	DELAY_INFO delay;
	MODULATION_INFO modulation;
	REVERB_INFO reverb;
	MIXER_INFO mixer;
	EQ_INFO eq;
	if(reloadFromSystem)
	{
		GetCurrentSetting(&synthSetting);
	}
	// Synth
	if(MISAMODULE_SYNTH & module)
	{
		int i,j,size;
		PVOICE_INFO pVoice;
		struct voice voiceitem;
		memset(&synth,0,sizeof(SYNTH_INFO));
		memcpy(synth.tune,synthSetting.tuning,6);
		GetSynthInfo(&synth);
		for(j=0;j<6;j++)
		{
			size = synthSetting.voices[j].size();
			if(size>5)
			{
				size = 5;
			}
			for(i=0;i<size;i++)
			{
				voiceitem = synthSetting.voices[j][i];
				switch(j)
				{
				case 0:
					pVoice = synth.aVoicept1[i];
					break;
				case 1:
					pVoice = synth.aVoicept2[i];
					break;
				case 2:
					pVoice = synth.aVoicept3[i];
					break;
				case 3:
					pVoice = synth.aVoicept4[i];
					break;
				case 4:
					pVoice = synth.aVoicept5[i];
					break;
				case 5:
				default:
					pVoice = synth.aVoicept6[i];
				}
				pVoice->available = 1;
				pVoice->wave = voiceitem.wavetable_index;
				pVoice->ampattack = voiceitem.amp_env_attack;
				pVoice->ampdecay = voiceitem.amp_env_decay;
				pVoice->amprelease = voiceitem.amp_env_release;
				pVoice->filtfreq = voiceitem.filter_frequency;
				pVoice->filtres = voiceitem.filter_resonance;
				pVoice->detunecource = voiceitem.detune_course;
				pVoice->detunefine = voiceitem.detune_fine;
				pVoice->vibrate = voiceitem.vibrate_rate;
				pVoice->vibdepth = voiceitem.vibrate_depth;
				pVoice->vibdelay = voiceitem.vibrate_delay;
				pVoice->portmtime = voiceitem.portamento_time;
				pVoice->pan = voiceitem.pan;
				pVoice->pitchWsemitones = voiceitem.pitch_bend_semitones;
				pVoice->pitchwheel = voiceitem.pitch_wheel/128; //MZ
				pVoice->volecity = voiceitem.velocity;
				pVoice->reverbsend = voiceitem.reverb_send;
				pVoice->level = voiceitem.channel_volume;
				pVoice->filter_type = (voiceitem.filter_type - 1) * 18;
				pVoice->filter_attack = voiceitem.filter_attack;
				pVoice->filter_decay = voiceitem.filter_decay;
				pVoice->filter_release = voiceitem.filter_release;
				if(voiceitem.insert_fx_block_on[0])
				{
					pVoice->Fx = 1;
				}
				else if(voiceitem.insert_fx_block_on[1])
				{
					pVoice->Fx = 2;
				}
				else
				{
					pVoice->Fx = 0;
				}
			}
		}
		UpdateSynthInfo(&synth);
	}
	// Compression
	if(MISAMODULE_COMPRESSION & module)
	{
		GetCompressionInfo(&compression);
		compression.softhard = synthSetting.fx_block[compression.Fx].compressor.knee;
		compression.On = synthSetting.fx_block[compression.Fx].compressor.on;
		compression.attack = synthSetting.fx_block[compression.Fx].compressor.attack;
		compression.release = synthSetting.fx_block[compression.Fx].compressor.release;
		compression.boost = synthSetting.fx_block[compression.Fx].compressor.boost;
		compression.thres = synthSetting.fx_block[compression.Fx].compressor.threshold;
		compression.ratio = synthSetting.fx_block[compression.Fx].compressor.ratio;
		UpdateCompressionInfo(&compression);
	}
	// Distortion
	if(MISAMODULE_DISTORTION & module)
	{
		GetDistortionInfo(&distortion);
		distortion.On = synthSetting.fx_block[distortion.Fx].distortion.on;
		distortion.type = synthSetting.fx_block[distortion.Fx].distortion.type;
		distortion.drive = synthSetting.fx_block[distortion.Fx].distortion.drive;
		distortion.tone = synthSetting.fx_block[distortion.Fx].distortion.tone;
		distortion.level = synthSetting.fx_block[distortion.Fx].distortion.level;
		distortion.noisereduction = synthSetting.fx_block[distortion.Fx].distortion.noise_reduction;
		distortion.booster = synthSetting.fx_block[distortion.Fx].distortion.booster;
		UpdateDistortionInfo(&distortion);
	}
	// Modulation
	if(MISAMODULE_MODULATION & module)
	{
		GetModulationInfo(&modulation);
		modulation.On = synthSetting.fx_block[modulation.Fx].modulation.on?1:0;
		modulation.type = synthSetting.fx_block[modulation.Fx].modulation.type;
		modulation.mod_rate = synthSetting.fx_block[modulation.Fx].modulation.rate;
		modulation.mod_depth = synthSetting.fx_block[modulation.Fx].modulation.depth;
		modulation.level = synthSetting.fx_block[modulation.Fx].modulation.level;
		modulation.fdbk = synthSetting.fx_block[modulation.Fx].modulation.chorus_feedback;
		modulation.fdbkfilter = synthSetting.fx_block[modulation.Fx].modulation.delay_feedback_filter;
		modulation.shape = synthSetting.fx_block[modulation.Fx].modulation.tremolo;
		modulation.time = synthSetting.fx_block[modulation.Fx].modulation.chorus_delay_time;
		UpdateModulationInfo(&modulation);
	}
	// Delay
	if(MISAMODULE_DELAY & module)
	{
		GetDelayInfo(&delay);
		delay.On = synthSetting.fx_block[delay.Fx].delay.on;
		delay.monostereo = synthSetting.fx_block[delay.Fx].delay.mode;
		delay.pre_lpf = synthSetting.fx_block[delay.Fx].delay.pre_lp;
		delay.time = synthSetting.fx_block[delay.Fx].delay.time;
		delay.level = synthSetting.fx_block[delay.Fx].delay.level;
		delay.fdbk = synthSetting.fx_block[delay.Fx].delay.feedback;
		delay.fdbkfilter = synthSetting.fx_block[delay.Fx].delay.feedback_filter;
		UpdateDelayInfo(&delay);
	}
	// Reverberation
	if(MISAMODULE_REVERB & module)
	{
		GetReverbInfo(&reverb);
		//reverb.On = synthSetting.reverb
		reverb.type = synthSetting.reverb.type;
		reverb.pre_lpf = synthSetting.reverb.pre_lpf;
		reverb.character = synthSetting.reverb.character;
		reverb.level = synthSetting.reverb.level;
		reverb.time = synthSetting.reverb.time;
		reverb.delayfdbk = synthSetting.reverb.delay_feedback;
		reverb.delaytime = synthSetting.reverb.pre_delay_time;
		UpdateReverbInfo(&reverb);
	}
	// Mixer
	if(MISAMODULE_MIXER & module)
	{
		GetMixerInfo(&mixer);
		mixer.lowcut = synthSetting.fx_block[mixer.Fx].mixer.low_cut_filter_frequency;
		mixer.highcut = synthSetting.fx_block[mixer.Fx].mixer.high_cut_filter_frequency;
		mixer.inputgain = synthSetting.fx_block[mixer.Fx].mixer.input_gain;
		mixer.level = synthSetting.fx_block[mixer.Fx].mixer.output_level;
		mixer.reverbsend = synthSetting.fx_block[mixer.Fx].mixer.reverb_send;
		mixer.pan = synthSetting.fx_block[mixer.Fx].mixer.pan;
		UpdateMixerInfo(&mixer);
	}
	// Eq
	if(MISAMODULE_EQ & module)
	{
		eq.lowbandgain = synthSetting.equalizer.lowest.gain;
		eq.lowbandfreq = synthSetting.equalizer.lowest.frequency;
		eq.lowmidbandQ = synthSetting.equalizer.low_mid_q;
		eq.highmidbandgain = synthSetting.equalizer.higher.gain;
		eq.highmidbandfreq = synthSetting.equalizer.higher.frequency;
		eq.highmidbandQ = synthSetting.equalizer.high_mid_q;
		eq.highbandgain = synthSetting.equalizer.highest.gain;
		eq.highbandfreq = synthSetting.equalizer.highest.frequency;
		eq.On = synthSetting.equalizer.on;
		UpdateEqInfo(&eq);
	}
	if(MISAMODULE_CTRLASSIGN & module)
	{
		UpdateControlAssignmentsInfo(0);
	}
}

