
#include <cstdlib>
#include <iostream>


#ifdef Linux
#include <dirent.h>
#endif

using namespace std;


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
#include "BitcrusherScreen.h"
#include "DistortionScreen.h"
#include "ModulationScreen.h"
#include "DelayScreen.h"
#include "ReverbScreen.h"
#include "MixerScreen.h"
#include "EqScreen.h"

#include "PresetsScreen.h"
#include "Keyboard.h"
#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define PRESETS_TYPE_XPOS 80
#define PRESETS_TYPE_YPOS 120

#define PRESETS_XOFFSET 0
#define PRESETS_YOFFSET 2

#define PRESETS_ITEM_POS 50
#define PRESETS_ITEMBLANK 80

///////////////////////////////////////////////////////////////////////////////
//

extern std::string working_directory;

typedef enum __PRESETSITEMS
{
	PRESETS_LOAD,
	PRESETS_SAVE,
	PRESETS_SAVE_AS,
	//PRESETS_NEW,
	PRESETS_DELETE,
	PRESETS_CLOSE,
	PRESETS_MAX
} PRESETSITEMS;

typedef enum __PRESETS_ID
{
	PRESETS_ID_LOAD=GUI_ID_PRESETS_BASE,
	PRESETS_ID_SAVE,
	PRESETS_ID_SAVE_AS,
	PRESETS_ID_DELETE,
	//PRESETS_ID_NEW,
	PRESETS_ID_ITEMSBASE,

} PRESETS_ID;

///////////////////////////////////////////////////////////////////////////////
//
static PresetSlideList presetSlideList;

static WM_HWIN hPresets;
static void PresetsProc(WM_MESSAGE* pMsg);
static WM_HWIN hPresetsItems[PRESETS_MAX];
static U8 PresetsDeleteItems();
bool DoesPresetExists(string fileName);

// static U8 SlidingBorder;


static std::vector<std::string> preset_filenames;
static BUTTON_Handle* pPresetsItems = 0;

static SYNTH_SETTING synthSetting;

static int SaveEffect();
static bool SavePresetAs(WM_HWIN hWinCaller);
static bool NewPreset();
static bool DeletePreset();

bool GetCurrentSetting(PSYNTH_SETTING pSetting);

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
	presetSlideList.CreateSlideItem();

	//WM_BringToTop(hPresets);
	return 0;
}

bool ReadPresetsDir(std::vector<std::string> &preset_filenames)
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

	hFind = FindFirstFile((working_directory + "/presets\\*.mz").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
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
	return true;
}

static U8 PresetsCreateItems(WM_HWIN hParent)
{
	int x,y;
	memset(hPresetsItems,0,sizeof(hPresetsItems));
	x=0;
	y = bmEMPTYTITLEBAR.YSize;
	presetSlideList.CreateWidget(x,y + 15, WM_GetWindowSizeX(hParent),WM_GetWindowSizeY(hParent)-130-y,hParent,WM_CF_SHOW|WM_CF_MEMDEV);


	x = 1;
	y = 502;
	hPresetsItems[PRESETS_LOAD] = BUTTON_CreateAsChild(x,y,bmPRESET_LOAD_UN.XSize,bmPRESET_LOAD_UN.YSize,hParent,PRESETS_ID_LOAD,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hPresetsItems[PRESETS_LOAD],0);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_LOAD],BUTTON_CI_UNPRESSED,&bmPRESET_LOAD_UN);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_LOAD],BUTTON_CI_PRESSED,&bmPRESET_LOAD_SE);

	// save As
	x += bmPRESET_LOAD_UN.XSize + PRESETS_XOFFSET;
	hPresetsItems[PRESETS_SAVE_AS] = BUTTON_CreateAsChild(x,y,bmPRESET_SAVEAS_UN.XSize,bmPRESET_SAVEAS_UN.YSize,hParent,PRESETS_ID_SAVE_AS,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hPresetsItems[PRESETS_SAVE_AS],0);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_SAVE_AS],BUTTON_CI_UNPRESSED,&bmPRESET_SAVEAS_UN);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_SAVE_AS],BUTTON_CI_PRESSED,&bmPRESET_SAVEAS_SE);


	x += bmPRESET_SAVE_UN.XSize + PRESETS_XOFFSET;
	hPresetsItems[PRESETS_SAVE] = BUTTON_CreateAsChild(x,y,bmPRESET_SAVE_UN.XSize,bmPRESET_SAVE_UN.YSize,hParent,PRESETS_ID_SAVE,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hPresetsItems[PRESETS_SAVE],0);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_SAVE],BUTTON_CI_UNPRESSED,&bmPRESET_SAVE_UN);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_SAVE],BUTTON_CI_PRESSED,&bmPRESET_SAVE_SE);


	// Delete PRESETS_DELETE PRESETS_ID_DELETE
	x += bmPRESET_SAVEAS_UN.XSize + PRESETS_XOFFSET;
	hPresetsItems[PRESETS_DELETE] = BUTTON_CreateAsChild(x,y,bmVC_DEL_UN.XSize,bmVC_DEL_UN.YSize,hParent,PRESETS_ID_DELETE,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hPresetsItems[PRESETS_DELETE],0);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_DELETE],BUTTON_CI_UNPRESSED,&bmVC_DEL_UN);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_DELETE],BUTTON_CI_PRESSED,&bmVC_DEL_SE);

	// Close
	x += bmVC_DEL_UN.XSize + PRESETS_XOFFSET;
	hPresetsItems[PRESETS_CLOSE] = BUTTON_CreateAsChild(x,y,bmPRESET_BACK_UN.XSize,bmPRESET_BACK_UN.YSize,hParent, COMMON_ID_CLOSE,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hPresetsItems[PRESETS_CLOSE],0);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_CLOSE],BUTTON_CI_UNPRESSED,&bmPRESET_BACK_UN);
	BUTTON_SetBitmap(hPresetsItems[PRESETS_CLOSE],BUTTON_CI_PRESSED,&bmPRESET_BACK_SE);

	// New PRESETS_NEW PRESETS_ID_NEW


	return 0;
}

static U8 PresetsDeleteItems()
{
	presetSlideList.DeleteWidget();
	for(int i=0; i < PRESETS_MAX; i++)
	{
		if (hPresetsItems[i])
		{
			BUTTON_Delete(hPresetsItems[i]);
			hPresetsItems[i] = 0;
		}
	}
	return 0;
}

static void PresetsProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	WM_HWIN hFocus;
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
		{
			std::string title("Presets");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
		}
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			DEBUG_STDOUT("PRESETS button clicked!");
			break;
		case WM_NOTIFICATION_RELEASED:

			if (Keyboard::IsOpen())
				break;		// lock the buttons if saveas keyboard is open

			switch(Id)
			{
			case PRESETS_ID_LOAD:
				LoadEffect(presetSlideList);
				break;

			case PRESETS_ID_SAVE:
				SaveEffect();
				break;

			case PRESETS_ID_SAVE_AS:
				SavePresetAs(pMsg->hWin);
				break;

			case PRESETS_ID_DELETE:
				// confirmation ?
				hFocus = WM_GetFocussedWindow();
				if(hFocus)
				{
					if(GUI_ID_OK == Misa_ConfirmBox("Are you sure?","Delete this preset!",GUI_MESSAGEBOX_CF_MODAL))
					{
						WM_SetFocus(hFocus);
						DeletePreset();
						presetSlideList.CreateSlideItem();
					}
					else
					{
						WM_SetFocus(hFocus);
					}
				}
				break;

/*			case PRESETS_ID_NEW:
				NewPreset();
				presetSlideList.CreateSlideItem();
				break;*/

			case COMMON_ID_CLOSE:
				TopMainMenu(pMsg->hWin);
				PresetSlideDeleteItems();
				break;
			default:
				;
			}
			DEBUG_STDOUT("PRESETS button released!");
			break;
		}
		break;
	case WM_TOUCH:
		break;

	case WM_CUST_KEYBOARD_DONE:
		{
			string fileName((char *)pMsg->Data.p);
			bool Exists = false;
		
			if (fileName.empty())
				break;

			// Does a preset with the same name already exists ?
			if (DoesPresetExists(fileName))
			{
				std::string msg("Are you sure? You will OVERWRITE ");
				msg += (string)fileName + " !";
				if(GUI_ID_OK != Misa_ConfirmBox(msg.c_str(), "Delete this preset!",GUI_MESSAGEBOX_CF_MODAL))
					break;
			}


			SynthSavePreset(&synthSetting, fileName);
			presetSlideList.CreateSlideItem();		 //update the list
			GUI_RECT RectToInvalidate ={0, 0, 800, 62};
			WM_InvalidateRect(hPresets, &RectToInvalidate);
			Keyboard::Close();
		}
		break;

	case WM_CUST_KEYBOARD_CANCEL:
		Keyboard::Close();
		break;

	default:
		WM_DefaultProc(pMsg);
	}
}
	
bool DoesPresetExists(string fileName)
{
	vector<string>::iterator it;
	bool Exists = false;
	for (it = preset_filenames.begin(); it != preset_filenames.end(); it++)
	{
		if (*it == fileName)
		{
			Exists = true;
			break;
		}
	}
	return Exists;
}


U8 DeletePresetSlidePanel(WM_HWIN hSlidePanel)
{
	presetSlideList.DeleteSlideWin();
	WM_DeleteWindow(hSlidePanel);
	return 0;
}


U8 PresetSlideCreateItems(WM_HWIN hParent, bool singleColumn)
{
	int x,y,i,size;
	x = 0;
	y = 0;
	preset_filenames.clear();
	ReadPresetsDir(preset_filenames);
	size = preset_filenames.size();
	if(size)
		pPresetsItems = (BUTTON_Handle*)malloc(sizeof(BUTTON_Handle*)*size);

	//preset_filenames.size();
	if (singleColumn)
	{
		for(i=0;i<size;i++)
		{
			pPresetsItems[i] = MisaItem_CreateEx(10, bmSELECT.YSize * i, bmSELECT.XSize, bmSELECT.YSize,
				hParent, PRESETS_ID_ITEMSBASE+i, WM_CF_SHOW|WM_CF_MEMDEV, preset_filenames[i].c_str(),0, &bmSELECT);
			// BUTTON_SetTextAlign(pPresetsItems[i], GUI_TA_LEFT | GUI_TA_VCENTER);
			if (preset_filenames[i] == GetCurrentPresetName())
				WM_SetFocus(pPresetsItems[i]);

		}
		y = bmSELECT.YSize*size;
	}
	else
	{
		for(i=0;i<size;i++)
		{
			pPresetsItems[i] = MisaItem_CreateEx(PRESETS_ITEM_POS+(bmSELECT.XSize+PRESETS_ITEMBLANK)*(i%3), bmSELECT.YSize*(i/3), bmSELECT.XSize, bmSELECT.YSize,
				hParent, PRESETS_ID_ITEMSBASE+i, WM_CF_SHOW|WM_CF_MEMDEV, preset_filenames[i].c_str(),0, &bmSELECT);
			if (preset_filenames[i] == GetCurrentPresetName())
				WM_SetFocus(pPresetsItems[i]);
		}
		y = size%3?bmSELECT.YSize*((i/3)+1):bmSELECT.YSize*(i/3);
	}
	x = WM_GetWindowSizeX(hParent);
	WM_SetSize(hParent, x, WM_GetWindowSizeY(hParent)<y?y:WM_GetWindowSizeY(hParent));
				BUTTON_SetPressed(pPresetsItems[2], int(true));
	return 0;
}

U8 PresetSlideDeleteItems()
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

static bool SavePresetAs(WM_HWIN hWinCaller)
{
	// Create Keyboard
	string fileName;
	Keyboard::Open(hWinCaller, 0, bmEMPTYTITLEBAR.YSize);
	return true;
}

static bool DeletePreset()
{
	int size,pos;
	WM_HWIN hFocus;
	size = preset_filenames.size();
	hFocus = WM_GetFocussedWindow();
	if(!presetSlideList.IsPresetItem(hFocus))
		return false;

	if(hFocus)
	{
		pos = WM_GetId(hFocus)-PRESETS_ID_ITEMSBASE;
		if(pos < size)
		{
			GetCurrentSetting(&synthSetting);
#ifdef Linux
			SynthDeletePreset(preset_filenames[pos]);
#else	// Win
			string fileToDeletePath = working_directory+"/presets/" + preset_filenames[pos]+".mz";
			DeleteFile(fileToDeletePath.c_str());
#endif	// Win
			return true;

		}
	}
	return false;
}

static bool NewPreset()
{
	return true;
}

static int SaveEffect()
{
	int size,pos;
	WM_HWIN hFocus;
	bool ReCreateList = true;

	std::string presetFileName;
	size = preset_filenames.size();
	hFocus = WM_GetFocussedWindow();

	if(hFocus && presetSlideList.IsPresetItem(hFocus))
	{
		pos = WM_GetId(hFocus)-PRESETS_ID_ITEMSBASE;
		if(pos >= size)
			return 0;
		presetFileName = preset_filenames[pos];
	}
	else
	{
		presetFileName = GetCurrentPresetName();		// Overwrite the current preset
	}

	if (presetFileName.empty())
		return 0;

	if (DoesPresetExists(presetFileName))
	{
		std::string msg("Are you sure? You will OVERWRITE ");
		msg += presetFileName + " !";
		if(GUI_ID_OK != Misa_ConfirmBox(msg.c_str(), "Overwrite this preset!",GUI_MESSAGEBOX_CF_MODAL))
			return 0;
		ReCreateList = false;
	}

	GetCurrentSetting(&synthSetting);
	SynthSavePreset(&synthSetting, presetFileName);
	GUI_RECT RectToInvalidate ={0, 0, 800, 62};
	WM_InvalidateRect(hPresets, &RectToInvalidate);

	if (ReCreateList)
		presetSlideList.CreateSlideItem();

	return 1;
}

int LoadEffect(PresetSlideList &thePresetSlideList)
{
	int size,pos;
	WM_HWIN hFocus;
	size = preset_filenames.size();
	hFocus = WM_GetFocussedWindow();
	if(!thePresetSlideList.IsPresetItem(hFocus))
		return 0;

	if(hFocus)
	{
		pos = WM_GetId(hFocus)-PRESETS_ID_ITEMSBASE;
		if(pos < size)
		{
			SynthTurnNotesOff();
			SynthLoadPreset(preset_filenames[pos]);
			UpdateSynthSetting();
			// ClearModified();		// requested here because UpdateSynthSetting send values to the synth (SynthSetDragCentre...)
			GUI_RECT RectToInvalidate ={0, 0, 800, 62};
			WM_InvalidateRect(hPresets, &RectToInvalidate);
		}
	}
	return 1;
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
	distortion.booster = synthSetting.fx_block[distortion.Fx].distortion.booster * 32;
	UpdateDistortionInfo(&distortion);

	// Bit Crusher
TODO : 	GetBitCrusherInfo();
		...
		UpdateBitCrusherInfo()

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
	eq.lowmidbandgain = synthSetting.equalizer.lower.gain;
	eq.lowmidbandfreq = synthSetting.equalizer.lower.frequency;
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
	BITCRUSHER_INFO bitcrusher;
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

	LockModified();		// OR : Lock modified flag setting during this function

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
		distortion.booster = synthSetting.fx_block[distortion.Fx].distortion.booster * 32;
		UpdateDistortionInfo(&distortion);
	}

	// Bit Crusher
	if(MISAMODULE_BITCRUSHER & module)
	{
		GetBitCrusherInfo(&bitcrusher);
		bitcrusher.On = synthSetting.fx_block[bitcrusher.Fx].bitcrusher.on;
		bitcrusher.bitresolution = synthSetting.fx_block[bitcrusher.Fx].bitcrusher.bitresolution * 127 / 17 + 1;
		if (bitcrusher.bitresolution == 1)
			bitcrusher.bitresolution = 0;
		// bitcrusher.brightness = synthSetting.fx_block[bitcrusher.Fx].bitcrusher.brightness;
		if (synthSetting.fx_block[bitcrusher.Fx].bitcrusher.downsampling == 0)
			bitcrusher.downsamplingFactor = 0;
		else
			bitcrusher.downsamplingFactor = (synthSetting.fx_block[bitcrusher.Fx].bitcrusher.downsampling - 1 )* 8;
		UpdateBitCrusherInfo(&bitcrusher);
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
		eq.lowmidbandgain = synthSetting.equalizer.lower.gain;
		eq.lowmidbandfreq = synthSetting.equalizer.lower.frequency;
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

	UnlockModified();	// OR : Lock modified flag setting during this function
}

