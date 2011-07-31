
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string> // OR 28-06-11 : make it compile with Visual Studio 2008

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "MisaDialogueBox.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "VoiceScreen.h"
#include "SynthScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define VOICE_TYPE_XPOS 80
#define VOICE_TYPE_YPOS 120

#define VOICE_XOFFSET 2
#define VOICE_YOFFSET 2

#define VOICE_PAGE_POSITIVE_FACTOR 175
#define VOICE_PAGE_NEGATIVE_FACTOR -175

///////////////////////////////////////////////////////////////////////////////
//

extern std::string working_directory;

typedef enum __VOICEITEMS
{
	
	VOICE_INDICATOR,
	VOICE_SLIDEWINDOW,
	VOICE_DELETE,
	VOICE_LEFTARROW,
	VOICE_RIGHTARROW,
	VOICE_AMPATTACK,
	VOICE_AMPDECAY,
	VOICE_AMPRELEASE,
	VOICE_FILTFREQ,
	VOICE_FILTRES,
	VOICE_DETUNECOURSE,
	VOICE_DETUNEFINE,
	VOICE_VIBRATERATE,
	VOICE_VIBRATEDEPTH,
	VOICE_VIBRATEDELAY,
	VOICE_PORTMTIME,
	VOICE_PAN,
	VOICE_PITCHWSEMITONES,
	VOICE_PITCHWHEEL,
	VOICE_VELOCITY,
	VOICE_REVERBSEND,
	VOICE_LEVEL,
	VOICE_FILTER_TYPE,
	VOICE_FILTER_ATTACK,
	VOICE_FILTER_DECAY,
	VOICE_FILTER_RELEASE,
	VOICE_MAX
} VOICEITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hVoice;
static void VoiceProc(WM_MESSAGE* pMsg);
static WM_HWIN hVoiceItems[VOICE_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
void SlideGoNextPage();
void SlideGoPreviousPage();

static U8 SlidingBorder;

static int VoiceGetFX();
static void VoiceControlMenuProc(int menuId);
static const GUI_RECT leftsidebutton=
{
	0,132,39,498
};
static const GUI_RECT rightsidebutton=
{
	760,132,799,498
};

static const GUI_RECT rectModType=
{
	250,
	62,
	550,
	130
};
static int voiceIndex;

std::vector<std::string> strVoicetype;

void ReadVoiceTypesFromFile(void);

/*static const char* strVoicetype[]=
{
	"saw",
	"sin",
	"square",
	"triangle",
	"xenoxpad",
	"riverpad",
	"loungepad",
	"chinapad",
	"spiritchoir",
	"strings",
	"systrings",
	"dronepad",
	"tjampad",
	"southpad",
	"jazzistik",
	"sekentar",
	"leadman",
	"beatporte",
	"moglead",
	"windlead",
	"patlead",
	"sybrass1",
	"sybrass2",
	"sitara"
};

*/

void VoiceSubWindowProc(WM_MESSAGE* pMsg);
OptionSubWin *OptionSubWin::pOptionSubWin  = NULL;


///////////////////////////////////////////////////////////////////////////////
//

U8 CreateVoiceScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	voiceIndex = 0;

	ReadVoiceTypesFromFile();

	hVoice = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, VoiceProc, 0);
	return 0;
}

U8 DeleteVoiceScreen()
{
	WM_DeleteWindow(hVoice);
	return 0;
}

U8 TopVoiceScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hVoice);
	//WM_BringToTop(hVoice);
	SetWindowToUpdateIfPresetModified(hVoice);
	return 0;
}

U8 UpdateVoiceInfo(PVOICE_INFO pInfo)
{
	std::vector<int> effect;
	switch(pInfo->Fx)
	{
	case 0:
	default:
		MisaCheckbox_SetStatus(hCOMPcomm.hFX1,0);
		MisaCheckbox_SetStatus(hCOMPcomm.hFX2,0);
		break;
	case 1:
		MisaCheckbox_SetStatus(hCOMPcomm.hFX1,1);
		MisaCheckbox_SetStatus(hCOMPcomm.hFX2,0);
		break;
	case 2:
		
		MisaCheckbox_SetStatus(hCOMPcomm.hFX1,0);
		MisaCheckbox_SetStatus(hCOMPcomm.hFX2,1);
	}
	voiceIndex = pInfo->wave;
	//if(0>voiceIndex || sizeof(strVoicetype)/4<=voiceIndex)
	if(0>voiceIndex || strVoicetype.size()<=voiceIndex)
	{
		voiceIndex = 0;
	}
	WM_InvalidateRect(hVoice, &rectModType);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_AMPATTACK],pInfo->ampattack,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_AMPDECAY],pInfo->ampdecay,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_AMPRELEASE],pInfo->amprelease,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_FILTFREQ],pInfo->filtfreq,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_FILTRES],pInfo->filtres,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_DETUNECOURSE],pInfo->detunecource,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_DETUNEFINE],pInfo->detunefine,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_VIBRATERATE],pInfo->vibrate,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_VIBRATEDEPTH],pInfo->vibdepth,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_VIBRATEDELAY],pInfo->vibdelay,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_PORTMTIME],pInfo->portmtime,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_PAN],pInfo->pan,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_PITCHWSEMITONES],pInfo->pitchWsemitones,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_PITCHWHEEL],pInfo->pitchwheel,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_VELOCITY],pInfo->volecity,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_REVERBSEND],pInfo->reverbsend,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_LEVEL],pInfo->level,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_FILTER_TYPE],pInfo->filter_type,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_FILTER_ATTACK],pInfo->filter_attack,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_FILTER_DECAY],pInfo->filter_decay,0);
	MisaSmallProgressCombo_SetPercentEx(hVoiceItems[VOICE_FILTER_RELEASE],pInfo->filter_release,0);

	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_AMPATTACK),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_AMPATTACK], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_AMPDECAY),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_AMPDECAY], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_AMPRELEASE),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_AMPRELEASE], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_FILTFREQ),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_FILTFREQ], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_FILTRES),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_FILTRES], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_DETUNECOURSE),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_DETUNECOURSE], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_DETUNEFINE),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_DETUNEFINE], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_VIBRATERATE),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_VIBRATERATE], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_VIBRATEDEPTH),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_VIBRATEDEPTH], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_VIBRATEDELAY),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_VIBRATEDELAY], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_PORTMTIME),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_PORTMTIME], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_PAN),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_PAN], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_PITCHWSEMITONES),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_PITCHWSEMITONES], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_PITCHWHEEL),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_PITCHWHEEL], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_VELOCITY),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_VELOCITY], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_REVERBSEND),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_REVERBSEND], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_LEVEL),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_LEVEL], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_FILTER_TYPE),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_FILTER_TYPE], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_FILTER_ATTACK),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_FILTER_ATTACK], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_FILTER_DECAY),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_FILTER_DECAY], effect.size()?1:0);
	effect = VoiceFindEffect(SynthTranslateEffect(VOICE_ID_FILTER_RELEASE),SynthGetStringIndex(),SynthGetVoiceIndex());
	MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_FILTER_RELEASE], effect.size()?1:0);

	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_AMPATTACK),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->ampattack);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_AMPDECAY),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->ampdecay);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_AMPRELEASE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->amprelease);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTFREQ),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->filtfreq);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTRES),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->filtres);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_DETUNECOURSE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->detunecource);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_DETUNEFINE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->detunefine);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VIBRATERATE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->vibrate);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VIBRATEDEPTH),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->vibdepth);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VIBRATEDELAY),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->vibdelay);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_PAN),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->pan);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_PITCHWSEMITONES),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->pitchWsemitones);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_PITCHWHEEL),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->pitchwheel);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VELOCITY),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->volecity);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_REVERBSEND),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->reverbsend);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_LEVEL),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->level);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_TYPE),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_type);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_ATTACK),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_attack);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_DECAY),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_decay);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_RELEASE),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_release);
	return 0;
}

U8 GetVoiceInfo(PVOICE_INFO pInfo)
{
	pInfo->Fx = VoiceGetFX();
	pInfo->wave = voiceIndex;
	WM_InvalidateRect(hVoice, &rectModType);
	pInfo->ampattack = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_AMPATTACK]);
	pInfo->ampdecay = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_AMPDECAY]);
	pInfo->amprelease = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_AMPRELEASE]);
	pInfo->filtfreq = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_FILTFREQ]);
	pInfo->filtres = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_FILTRES]);
	pInfo->detunecource = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_DETUNECOURSE]);
	pInfo->detunefine = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_DETUNEFINE]);
	pInfo->vibrate = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_VIBRATERATE]);
	pInfo->vibdepth = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_VIBRATEDEPTH]);
	pInfo->vibdelay = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_VIBRATEDELAY]);
	pInfo->portmtime = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_PORTMTIME]);
	pInfo->pan = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_PAN]);
	pInfo->pitchWsemitones = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_PITCHWSEMITONES]);
	pInfo->pitchwheel = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_PITCHWHEEL]);
	pInfo->volecity = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_VELOCITY]);
	pInfo->reverbsend = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_REVERBSEND]);
	pInfo->level = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_LEVEL]);
	pInfo->filter_type = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_FILTER_TYPE]);
	pInfo->filter_attack = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_FILTER_ATTACK]);
	pInfo->filter_decay = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_FILTER_DECAY]);
	pInfo->filter_release = MisaSmallProgressCombo_GetPercent(hVoiceItems[VOICE_FILTER_RELEASE]);
	return 0;
}

static void CenterVoiceControls()
{
	PVOICE_INFO pInfo;
	VOICE_INFO vc_info;
	pInfo = &vc_info;
	GetVoiceInfo(pInfo);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_AMPATTACK),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->ampattack);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_AMPDECAY),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->ampdecay);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_AMPRELEASE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->amprelease);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTFREQ),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->filtfreq);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTRES),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->filtres);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_DETUNECOURSE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->detunecource);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_DETUNEFINE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->detunefine);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VIBRATERATE),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->vibrate);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VIBRATEDEPTH),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->vibdepth);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VIBRATEDELAY),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->vibdelay);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_PAN),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->pan);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_PITCHWSEMITONES),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->pitchWsemitones);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_PITCHWHEEL),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->pitchwheel);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_VELOCITY),SynthGetStringIndex(),SynthGetVoiceIndex(),pInfo->volecity);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_REVERBSEND),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->reverbsend);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_LEVEL),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->level);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_TYPE),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_type);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_ATTACK),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_attack);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_DECAY),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_decay);
	VoiceSetDragCentre(SynthTranslateEffect(VOICE_ID_FILTER_RELEASE),SynthGetStringIndex(),SynthGetVoiceIndex(), pInfo->filter_release);
}

static U8 VoiceCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_FX1|COMMON_ITEM_FX2|COMMON_ITEM_CLOSE);
	memset(hVoiceItems,0,sizeof(hVoiceItems));
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;
	hVoiceItems[VOICE_LEFTARROW] = BUTTON_CreateAsChild(x,y,bmLEFTARROW_UN.XSize,bmLEFTARROW_UN.YSize,hParent,VOICE_ID_LEFTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hVoiceItems[VOICE_LEFTARROW],0);
	BUTTON_SetBitmap(hVoiceItems[VOICE_LEFTARROW],BUTTON_CI_UNPRESSED,&bmLEFTARROW_UN);
	BUTTON_SetBitmap(hVoiceItems[VOICE_LEFTARROW],BUTTON_CI_PRESSED,&bmLEFTARROW_SE);
	x += bmLEFTARROW_UN.XSize+356;
	hVoiceItems[VOICE_RIGHTARROW] = BUTTON_CreateAsChild(x,y,bmRIGHTARROW_UN.XSize,bmRIGHTARROW_UN.YSize,hParent,VOICE_ID_RIGHTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hVoiceItems[VOICE_RIGHTARROW],0);
	BUTTON_SetBitmap(hVoiceItems[VOICE_RIGHTARROW],BUTTON_CI_UNPRESSED,&bmRIGHTARROW_UN);
	BUTTON_SetBitmap(hVoiceItems[VOICE_RIGHTARROW],BUTTON_CI_PRESSED,&bmRIGHTARROW_SE);
	y += bmDISTYPEAREA.YSize;
	y += VOICE_YOFFSET;
	x = 50;
	
	hVoiceItems[VOICE_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);

	x=VOICE_XOFFSET;
	y=500;
	hCOMPcomm.hFX1 = MisaCheckbox_Create(x,y,bmFX1_NORMAL.XSize,bmFX1_NORMAL.YSize,hParent,
			COMMON_ID_FX1,WM_CF_SHOW | WM_CF_MEMDEV,&bmFX1_NORMAL,&bmFX1_PRESSED);
	x+=bmFX1_NORMAL.XSize;
	hCOMPcomm.hFX2 = MisaCheckbox_Create(x,y,bmFX2_NORMAL.XSize,bmFX2_NORMAL.YSize,hParent,
			COMMON_ID_FX2,WM_CF_SHOW | WM_CF_MEMDEV,&bmFX2_NORMAL,&bmFX2_PRESSED);
	x+=bmFX2_NORMAL.XSize;
	x+=VOICE_XOFFSET;
	//y+=VOICE_YOFFSET;
	hVoiceItems[VOICE_DELETE] = BUTTON_CreateAsChild(x,y,bmVC_PLUS_UN.XSize,bmVC_PLUS_UN.YSize,hParent,VOICE_ID_MOREOPTIONS, WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hVoiceItems[VOICE_DELETE],0);
	BUTTON_SetBitmap(hVoiceItems[VOICE_DELETE],BUTTON_CI_UNPRESSED,&bmVC_PLUS_UN);
	BUTTON_SetBitmap(hVoiceItems[VOICE_DELETE],BUTTON_CI_PRESSED,&bmVC_PLUS_SE);
	return 0;
}

static U8 VoiceDeleteItems()
{
	if(hVoiceItems[VOICE_LEFTARROW])
	{
		BUTTON_Delete(hVoiceItems[VOICE_LEFTARROW]);
	}
	if(hVoiceItems[VOICE_RIGHTARROW])
	{
		BUTTON_Delete(hVoiceItems[VOICE_RIGHTARROW]);
	}
	if(hVoiceItems[VOICE_INDICATOR])
	{
		DeleteSlidePanel(hVoiceItems[VOICE_INDICATOR]);
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int VoiceGetFX()
{
	int stFx;
	stFx = 0;
	if(MisaCheckbox_GetStatus(hCOMPcomm.hFX2))
	{
		stFx = 2;
	}
	if(MisaCheckbox_GetStatus(hCOMPcomm.hFX1))
	{
		stFx = 1;
	}
	return stFx;
}

static void VoiceProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	GUI_PID_STATE* pPID_State;
	static U8 sidebutton = 0;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		VoiceCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		VoiceDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		x = 222;
		y = bmEMPTYTITLEBAR.YSize;
		GUI_DrawBitmap(&bmDISTYPEAREA, x, y);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		{
			std::string title("VOICE");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
		}
		y += 20;
		GUI_DispStringHCenterAt(strVoicetype[voiceIndex].c_str(), x / 2, y);
		// Draw button shape
		if(WM_HasCaptured(pMsg->hWin))
		{
			const GUI_RECT* prect;
			switch(sidebutton)
			{
			case 1:
				prect = &leftsidebutton;
				break;
			case 2:
				prect = &rightsidebutton;
				break;
			case 0:
			default:
				prect = 0;
			}
			if(prect)
			{
				GUI_SetColor(GUI_LIGHTBLUE);
				GUI_SetPenSize(5);
				GUI_DrawRectEx(prect);
			}
		}
		// Draw slide indicator
		y = WM_GetWindowOrgY(hVoiceItems[VOICE_SLIDEWINDOW]);
		y += 170;
		if(0x02 &SlidingBorder)
		{
			x = 4;
			GUI_DrawBitmap(&bmLARROW,x,y);
		}
		if(0x01 & SlidingBorder)
		{
			x = 750 + 15;
			GUI_DrawBitmap(&bmRARROW,x,y);
		}
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		if(WM_NOTIFICATION_RELEASED == NCode)
		{
			switch(Id)
			{
			case COMMON_ID_FX1:
				if(hCOMPcomm.hFX2)
				{
					MisaCheckbox_SetStatus(hCOMPcomm.hFX2,0);
				}
				break;
			case COMMON_ID_FX2:
				if(hCOMPcomm.hFX1)
				{
					MisaCheckbox_SetStatus(hCOMPcomm.hFX1,0);
				}
				break;
			}
			if(Id == COMMON_ID_FX1 || Id == COMMON_ID_FX2)
			{
				x = SynthGetStringIndex();
				y = SynthGetVoiceIndex();
				if(-1 != x && -1 != y)
				{
					switch(VoiceGetFX())
					{
					case 0:
					default:
						SynthVoiceSetFX(x,y,0,false);
						SynthVoiceSetFX(x,y,1,false);
						break;
					case 1:
						SynthVoiceSetFX(x,y,1,false);
						SynthVoiceSetFX(x,y,0,true);
						break;
					case 2:
						SynthVoiceSetFX(x,y,0,false);
						SynthVoiceSetFX(x,y,1,true);
					}
				}
				break;
			}
		}
		if(DispatchCommComponent(pMsg,&hCOMPcomm))
		{
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("VOICE button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case VOICE_ID_LEFTARROW:
					--voiceIndex;
					if(0>voiceIndex)
					{
						//voiceIndex = sizeof(strVoicetype)/4-1;
						voiceIndex = strVoicetype.size()-1;
					}
					WM_InvalidateRect(pMsg->hWin, &rectModType);
					SynthTurnNotesOff();
					SynthVoiceSetWave(SynthGetStringIndex(), SynthGetVoiceIndex(), voiceIndex);
					break;
				case VOICE_ID_RIGHTARROW:
					++voiceIndex;
					//if(sizeof(strVoicetype)/4 <= voiceIndex)
					if(strVoicetype.size() <= voiceIndex)
					{
						voiceIndex = 0;
					}
					WM_InvalidateRect(pMsg->hWin, &rectModType);
					SynthTurnNotesOff();
					SynthVoiceSetWave(SynthGetStringIndex(), SynthGetVoiceIndex(), voiceIndex);
					break;
				case COMMON_ID_CLOSE:

					if (OptionSubWin::pOptionSubWin != NULL)
					{
						delete OptionSubWin::pOptionSubWin;
						OptionSubWin::pOptionSubWin = NULL;
					}

					CenterVoiceControls();
					SynthEditVoice();
					TopSynthScreen(pMsg->hWin);
					UpdateSynthSettingEx(MISAMODULE_SYNTH);
					break;

				case VOICE_ID_MOREOPTIONS:
					if (OptionSubWin::pOptionSubWin == NULL)
						new OptionSubWin(pMsg->hWin);
					else
						OptionSubWin::pOptionSubWin->Show();

					break;

				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					VoiceControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("VOICE button released!");
				break;
			}
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				if(!WM_HasCaptured(pMsg->hWin))
				{
					GUI_RECT ptRect;
					WM_SetCapture(pMsg->hWin,1);
					ptRect.x0 = pPID_State->x;
					ptRect.y0 = pPID_State->y;
					ptRect.x1 = ptRect.x0+1;
					ptRect.y1 = ptRect.y0+1;
					if(GUI_RectsIntersect(&leftsidebutton,&ptRect))
					{
						sidebutton = 1;
						WM_InvalidateRect(pMsg->hWin, &leftsidebutton);
						SlideGoPreviousPage();
					}
					else if(GUI_RectsIntersect(&rightsidebutton,&ptRect))
					{
						sidebutton = 2;
						WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
						SlideGoNextPage();
					}
				}
			}
			else
			{
				//WM_ReleaseCapture();
				sidebutton = 0;
				if(pPID_State->x<50)
				{
					WM_InvalidateRect(pMsg->hWin, &leftsidebutton);
				}
				else
				{
					WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
				}
			}
		}
		else
		{
		}
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

WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style)
{
	return WM_CreateWindowAsChild(x0,y0,width,height,hParent,Style,IndicatorProc,0);
}

static U8 DeleteSlidePanel(WM_HWIN hSlidePanel)
{
	WM_DeleteWindow(hSlidePanel);
	return 0;
}

static U8 IndicatorCreateItems(WM_HWIN hParent)
{
	hVoiceItems[VOICE_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,2100,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hVoiceItems[VOICE_SLIDEWINDOW])
	{
		WM_DeleteWindow(hVoiceItems[VOICE_SLIDEWINDOW]);
	}
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

static void FilterTypeInfo(int value, char *buf)
{
	if(value / (128/7) + 1 == 1)
		strcpy(buf, "FILT TYPE: LP 12dB");
	else
	if(value / (128/7) + 1 == 2)
		strcpy(buf, "FILT TYPE: BP 12dB");
	else
	if(value / (128/7) + 1 == 3)
		strcpy(buf, "FILT TYPE: HP 12dB");
	else
	if(value / (128/7) + 1 == 4)
		strcpy(buf, "FILT TYPE: OFF");
	else
	if(value / (128/7) + 1 == 5)
		strcpy(buf, "FILT TYPE: LP 24dB");
	else
	if(value / (128/7) + 1 == 6)
		strcpy(buf, "FILT TYPE: BP 24dB");
	else
	if(value / (128/7) + 1 >= 7)
		strcpy(buf, "FILT TYPE: HP 24dB");
}

static U8 SlideCreateItems(WM_HWIN hParent)
{
	int x,y;
	x = 0;
	y = 15;
	hVoiceItems[VOICE_LEVEL] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_LEVEL,"LEVEL");
	y += 86;
	hVoiceItems[VOICE_AMPATTACK] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_AMPATTACK,"AMP ATTACK");
	y += 86;
	hVoiceItems[VOICE_AMPDECAY] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_AMPDECAY,"AMP DECAY");
	y += 86;
	hVoiceItems[VOICE_AMPRELEASE] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_AMPRELEASE,"AMP RELEASE");
	x = 350;
	y = 15;
	hVoiceItems[VOICE_VELOCITY] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_VELOCITY,"VELOCITY");
	y += 86;
	hVoiceItems[VOICE_REVERBSEND] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_REVERBSEND,"REVERB SEND");
	y += 86;
	hVoiceItems[VOICE_DETUNECOURSE] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_DETUNECOURSE,"DETUNE COURSE");
	y += 86;
	hVoiceItems[VOICE_DETUNEFINE] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_DETUNEFINE,"DETUNE FINE");

	x = 700;
	y = 15;
	hVoiceItems[VOICE_FILTER_TYPE] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_FILTER_TYPE,"FILT TYPE", FilterTypeInfo);
	//MZ MisaSmallProgressCombo_SetCheck(hVoiceItems[VOICE_FILTER_TYPE], 2);
	y += 86;
	hVoiceItems[VOICE_FILTER_ATTACK] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_FILTER_ATTACK,"FILT ATTACK");
	y += 86;
	hVoiceItems[VOICE_FILTER_DECAY] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_FILTER_DECAY,"FILT DECAY");
	y += 86;
	hVoiceItems[VOICE_FILTER_RELEASE] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_FILTER_RELEASE,"FILT RELEASE");
	x = 1050;
	y = 15;
	hVoiceItems[VOICE_FILTFREQ] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_FILTFREQ,"FILT FREQUENCY");
	y += 86;
	hVoiceItems[VOICE_FILTRES] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_FILTRES,"FILT RESONANCE");

	x = 1400;
	y = 15;
	hVoiceItems[VOICE_VIBRATERATE] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_VIBRATERATE,"VIBRATE RATE");
	y += 86;
	hVoiceItems[VOICE_VIBRATEDEPTH] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_VIBRATEDEPTH,"VIBRATE DEPTH");
	y += 86;
	hVoiceItems[VOICE_VIBRATEDELAY] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_VIBRATEDELAY,"VIBRATE DELAY");

	x += 350;
	y = 15;
	hVoiceItems[VOICE_PAN] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_PAN,"PAN");
	y += 86;
	hVoiceItems[VOICE_PITCHWHEEL] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_PITCHWHEEL,"PITCH WHEEL");
	y += 86;
	hVoiceItems[VOICE_PITCHWSEMITONES] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_PITCHWSEMITONES,"PITCH BEND AMT.");
	y += 86;
	hVoiceItems[VOICE_PORTMTIME] = MisaSmallProgressCombo_Create(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,VOICE_ID_PORTMTIME,"PORTM TIME");

#if 0	
	hVoiceItems[VOICE_AMPATTACK] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_AMPATTACK,"AMP","ATTACK",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_AMPDECAY] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_AMPDECAY,"AMP","DECAY",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_AMPRELEASE] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_AMPRELEASE,"AMP","RELEASE",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_FILTFREQ] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_FILTFREQ,"FILT","FREQ",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_FILTRES] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_FILTRES,"FILT","RES",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_DETUNECOURSE] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_DETUNECOURSE,"DETUNE","COURSE",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_DETUNEFINE] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_DETUNEFINE,"DETUNE","FINE",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_VIBRATERATE] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_VIBRATERATE,"VIBRATE","RATE",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_VIBRATEDEPTH] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_VIBRATEDEPTH,"VIBRATE","DEPTH",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_VIBRATEDELAY] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_VIBRATEDELAY,"VIBRATE","DELAY",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_PORTMTIME] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_PORTMTIME,"PORTM","TIME",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_PAN] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_PAN,"PAN",0,MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_PITCHWSEMITONES] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_PITCHWSEMITONES,"PITCH.W","SEMI TONES",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_PITCHWHEEL] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_PITCHWHEEL,"PITCH","WHEEL",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_VELOCITY] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_VELOCITY,"VELOCITY",0,MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_REVERBSEND] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_REVERBSEND,"REVERB","SEND",MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
	hVoiceItems[VOICE_LEVEL] = MisaProgressbar_CreateEx(x,y,0,0,hParent,VOICE_ID_LEVEL,"LEVEL",0,MISAPROG_FONT16,0);
	x += bmSLIDER.XSize;
	x += 100;
#endif
	return 0;
}

static U8 SlideDeleteItems()
{
	if(hVoiceItems[VOICE_AMPATTACK])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_AMPATTACK]);
	}
	if(hVoiceItems[VOICE_AMPDECAY])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_AMPDECAY]);
	}
	if(hVoiceItems[VOICE_AMPRELEASE])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_AMPRELEASE]);
	}
	if(hVoiceItems[VOICE_FILTFREQ])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_FILTFREQ]);
	}
	if(hVoiceItems[VOICE_FILTRES])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_FILTRES]);
	}
	if(hVoiceItems[VOICE_DETUNECOURSE])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_DETUNECOURSE]);
	}
	if(hVoiceItems[VOICE_DETUNEFINE])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_DETUNEFINE]);
	}
	if(hVoiceItems[VOICE_VIBRATERATE])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_VIBRATERATE]);
	}
	if(hVoiceItems[VOICE_VIBRATEDEPTH])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_VIBRATEDEPTH]);
	}
	if(hVoiceItems[VOICE_VIBRATEDELAY])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_VIBRATEDELAY]);
	}
	if(hVoiceItems[VOICE_PORTMTIME])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_PORTMTIME]);
	}
	if(hVoiceItems[VOICE_PAN])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_PAN]);
	}
	if(hVoiceItems[VOICE_PITCHWSEMITONES])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_PITCHWSEMITONES]);
	}
	if(hVoiceItems[VOICE_PITCHWHEEL])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_PITCHWHEEL]);
	}
	if(hVoiceItems[VOICE_VELOCITY])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_VELOCITY]);
	}
	if(hVoiceItems[VOICE_REVERBSEND])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_REVERBSEND]);
	}
	if(hVoiceItems[VOICE_LEVEL])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_LEVEL]);
	}
	if(hVoiceItems[VOICE_FILTER_TYPE])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_FILTER_TYPE]);
	}
	if(hVoiceItems[VOICE_FILTER_ATTACK])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_FILTER_ATTACK]);
	}
	if(hVoiceItems[VOICE_FILTER_DECAY])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_FILTER_DECAY]);
	}
	if(hVoiceItems[VOICE_FILTER_RELEASE])
	{
		MisaSmallProgressCombo_Delete(hVoiceItems[VOICE_FILTER_RELEASE]);
	}
#if 0
	if(hVoiceItems[VOICE_AMPATTACK])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_AMPATTACK]);
	}
	if(hVoiceItems[VOICE_AMPDECAY])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_AMPDECAY]);
	}
	if(hVoiceItems[VOICE_AMPRELEASE])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_AMPRELEASE]);
	}
	if(hVoiceItems[VOICE_FILTFREQ])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_FILTFREQ]);
	}
	if(hVoiceItems[VOICE_FILTRES])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_FILTRES]);
	}
	if(hVoiceItems[VOICE_DETUNECOURSE])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_DETUNECOURSE]);
	}
	if(hVoiceItems[VOICE_DETUNEFINE])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_DETUNEFINE]);
	}
	if(hVoiceItems[VOICE_VIBRATERATE])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_VIBRATERATE]);
	}
	if(hVoiceItems[VOICE_VIBRATEDEPTH])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_VIBRATEDEPTH]);
	}
	if(hVoiceItems[VOICE_VIBRATEDELAY])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_VIBRATEDELAY]);
	}
	if(hVoiceItems[VOICE_PORTMTIME])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_PORTMTIME]);
	}
	if(hVoiceItems[VOICE_PAN])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_PAN]);
	}
	if(hVoiceItems[VOICE_PITCHWSEMITONES])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_PITCHWSEMITONES]);
	}
	if(hVoiceItems[VOICE_PITCHWHEEL])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_PITCHWHEEL]);
	}
	if(hVoiceItems[VOICE_VELOCITY])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_VELOCITY]);
	}
	if(hVoiceItems[VOICE_REVERBSEND])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_REVERBSEND]);
	}
	if(hVoiceItems[VOICE_LEVEL])
	{
		MisaProgressbar_Delete(hVoiceItems[VOICE_LEVEL]);
	}
#endif
	return 0;
}

static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int x,y;
	int NCode,Id;
	std::vector<int> effect;
	//GUI_PID_STATE* pPID_State;
	//static GUI_PID_STATE PID_LastState;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		SlideCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		SlideDeleteItems();
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
		case WM_NOTIFICATION_VALUE_CHANGED:
			x = MisaSmallProgressCombo_GetPercent(pMsg->hWinSrc);
			switch(Id)
			{
			case VOICE_ID_AMPATTACK:
				SynthVoiceAmpEnvAttack(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_AMPDECAY:
				SynthVoiceAmpEnvDecay(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_AMPRELEASE:
				SynthVoiceAmpEnvRelease(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_FILTFREQ:
				SynthVoiceFilterFrequency(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_FILTRES:
				SynthVoiceFilterResonance(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_DETUNECOURSE:
				SynthVoiceDetuneCourse(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_DETUNEFINE:
				SynthVoiceDetuneFine(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_VIBRATERATE:
				SynthVoiceVibrateRate(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_VIBRATEDEPTH:
				SynthVoiceVibrateDepth(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_VIBRATEDELAY:
				SynthVoiceVibrateDelay(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_PORTMTIME:
				SynthVoicePortamentoTime(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_PAN:
				SynthVoicePan(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_PITCHWSEMITONES:
				SynthVoicePitchBendSemitones(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_PITCHWHEEL:
				SynthVoicePitchWheel(SynthGetStringIndex(), SynthGetVoiceIndex(), (x*0x4000)/128);
				break;
			case VOICE_ID_VELOCITY:
				SynthVoiceVoiceVelocity(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_REVERBSEND:
				SynthVoiceReverbSend(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_LEVEL:
				SynthVoiceChannelVolume(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_FILTER_TYPE:
				SynthVoiceFilterType(SynthGetStringIndex(), SynthGetVoiceIndex(), x<126?x/18+1:7);
				break;
			case VOICE_ID_FILTER_ATTACK:
				SynthVoiceFilterAttack(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_FILTER_DECAY:
				SynthVoiceFilterDecay(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			case VOICE_ID_FILTER_RELEASE:
				SynthVoiceFilterRelease(SynthGetStringIndex(), SynthGetVoiceIndex(), x<127?x:127);
				break;
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
			if(pMsg->hWinSrc != hVoiceItems[VOICE_FILTER_TYPE])
			{
				x = WM_GetWindowOrgX(pMsg->hWinSrc);
				y = WM_GetWindowOrgY(pMsg->hWinSrc);
				MisaSetcontrolmenu_Popup(hVoice,pMsg->hWinSrc,x,y);
				effect = VoiceFindEffect(SynthTranslateEffect(Id),SynthGetStringIndex(),SynthGetVoiceIndex());
				y = effect.size();
				for(x=0;x<y;x++)
				{
					MisaMenu_SetStatus(AssigneMentMenuTranslate(effect[x]&0x000000FF),2,0,(effect[x]>>8)&0x000000FF);
					MisaMenu_SetStatus(CTL_VARMIN,0,(effect[x]>>16)&0x000000FF,(effect[x]>>8)&0x000000FF);
					MisaMenu_SetStatus(CTL_VARMAX,0,(effect[x]>>24)&0x000000FF,(effect[x]>>8)&0x000000FF);
				}
			}
			break;
		}
		break;
#if 0
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				int dx;
				if(0 == PID_LastState.Pressed)
				{
					WM_SetCapture(pMsg->hWin,0);
					PID_LastState = *pPID_State;
				}
				else
				{
					x = WM_GetWindowOrgX(pMsg->hWin);
					y = WM_GetWindowSizeX(pMsg->hWin);
					dx = pPID_State->x-PID_LastState.x;
					if(dx>0)
					{
						WM_MoveWindow(pMsg->hWin,dx>INDICATORFRAME-x?INDICATORFRAME-x:dx,0);
					}
					else if(dx<0)
					{
						WM_MoveWindow(pMsg->hWin,dx<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:dx,0);
					}
				}
			}
			else
			{
				PID_LastState = *pPID_State;
				WM_ReleaseCapture();
				x = WM_GetWindowOrgX(pMsg->hWin);
				y = WM_GetWindowSizeX(pMsg->hWin);
				if(INDICATORFRAME < x)
				{
					WM_MoveWindow(pMsg->hWin,INDICATORFRAME-x,0);
				}
				else if(GUI_GetScreenSizeX()-INDICATORFRAME-y > x)
				{
					WM_MoveWindow(pMsg->hWin,GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
				}
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hVoice);
			}
		}
		else
		{
			PID_LastState.Pressed = 0;
			PID_LastState.x = 0;
			PID_LastState.y = 0;
			WM_ReleaseCapture();
		}
		break;
#endif
	default:
		WM_DefaultProc(pMsg);
	}
}

void SlideGoNextPage()
{
	int x,y;
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:VOICE_PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:VOICE_PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:VOICE_PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:VOICE_PAGE_NEGATIVE_FACTOR,0);
	SlidingBorder = GetSlidingBordercheck(hVoiceItems[VOICE_SLIDEWINDOW],hVoice);
}

void SlideGoPreviousPage()
{
	int x;
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:VOICE_PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:VOICE_PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:VOICE_PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hVoiceItems[VOICE_SLIDEWINDOW]);
	WM_MoveWindow(hVoiceItems[VOICE_SLIDEWINDOW],VOICE_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:VOICE_PAGE_POSITIVE_FACTOR,0);
	SlidingBorder = GetSlidingBordercheck(hVoiceItems[VOICE_SLIDEWINDOW],hVoice);
}

static void VoiceControlMenuProc(int menuId)
{
	AssginVoiceControlMenuProc(menuId,SynthGetStringIndex(),SynthGetVoiceIndex());
}

void ReadVoiceTypesFromFile(void)
{
	std::ifstream ifs(std::string(working_directory+"/wavetable.txt").c_str());

	strVoicetype.clear();

	if(!ifs.is_open())
	{
		std::cout << "wavetable.txt could not be opened." << std::endl << std::flush;
		strVoicetype.push_back("error");
		return;
	}

	std::string s;
	while(getline(ifs, s))
		strVoicetype.push_back(s);

	ifs.close();
}


// OptionSubWin implementation (OptionSubWin is an object that manages an additional window with 3 buttons -  Del, Solo, Mute)
OptionSubWin::OptionSubWin(WM_HWIN parent)
{
	pOptionSubWin  = this;

	hWin = WM_CreateWindowAsChild(0, 500, GUI_GetScreenSizeX(), GUI_GetScreenSizeY() / 3, parent, WM_CF_SHOW|WM_CF_MEMDEV, VoiceSubWindowProc, 0);
	// WM_SetUserData(hWin, this, sizeof(void *));
	//WM_SetCapture(hWin, false); // to close the window if the user press outside of it.

}

OptionSubWin::~OptionSubWin()
{
	SynthMuteVoice(SynthGetStringIndex(), SynthGetVoiceIndex(), false);
	SynthSoloVoice(SynthGetStringIndex(), SynthGetVoiceIndex(), false);

	WM_DetachWindow(hWin); 
	WM_DeleteWindow(hWin);
	hWin = 0;

	pOptionSubWin = NULL;
}

void OptionSubWin::Show()
{
	WM_ShowWindow(hWin);
}

void OptionSubWin::Hide()
{
	WM_HideWindow(hWin);
}

U8 OptionSubWin::CreateItems(WM_HWIN hParent)
{
	int x,y;
	
	x = (WM_GetWindowSizeX(hParent) - 4 * bmVC_DEL_UN.XSize - 6) / 2 ;
	y = 0;

	hDelButton = BUTTON_CreateAsChild(x, y, bmVC_DEL_UN.XSize, bmVC_DEL_UN.YSize, hParent, OPTION_VOICE_ID_DELETE, WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hDelButton, 0);
	BUTTON_SetBitmap(hDelButton, BUTTON_CI_UNPRESSED, &bmVC_DEL_UN);
	BUTTON_SetBitmap(hDelButton, BUTTON_CI_PRESSED, &bmVC_DEL_SE);

	x += bmVC_SOLO_UN.XSize + VOICE_XOFFSET;
	hBackButton = BUTTON_CreateAsChild(x, y, bmVC_MINUS_UN.XSize, bmVC_MINUS_UN.YSize, hParent, COMMON_ID_CLOSE, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hBackButton, 0);
	BUTTON_SetBitmap(hBackButton,BUTTON_CI_UNPRESSED,&bmVC_MINUS_UN);
	BUTTON_SetBitmap(hBackButton, BUTTON_CI_PRESSED,&bmVC_MINUS_SE);

	x += bmVC_DEL_UN.XSize + VOICE_XOFFSET;
	hSoloButton = MisaCheckbox_Create(x, y, bmVC_MUTE_UN.XSize, bmVC_SOLO_UN.YSize, hParent, OPTION_VOICE_ID_SOLO, WM_CF_SHOW|WM_CF_MEMDEV, &bmVC_SOLO_UN, &bmVC_SOLO_SE);
	//hSoloButton = BUTTON_CreateAsChild(x, y, bmVC_SOLO_UN.XSize, bmVC_SOLO_UN.YSize, hParent, OPTION_VOICE_ID_SOLO, WM_CF_SHOW|WM_CF_MEMDEV);
	//BUTTON_SetFocussable(hSoloButton, 0);
	//BUTTON_SetBitmap(hSoloButton, BUTTON_CI_UNPRESSED, &bmVC_SOLO_UN);
	//BUTTON_SetBitmap(hSoloButton, BUTTON_CI_PRESSED, &bmVC_SOLO_SE);

	x += bmVC_SOLO_UN.XSize + VOICE_XOFFSET;
	hMuteButton = MisaCheckbox_Create(x, y, bmVC_MUTE_UN.XSize, bmVC_MUTE_UN.YSize, hParent, OPTION_VOICE_ID_MUTE, WM_CF_SHOW|WM_CF_MEMDEV, &bmVC_MUTE_UN, &bmVC_MUTE_SE);



	return 0;
}

U8 OptionSubWin::DeleteItems()
{
	BUTTON_Delete(hDelButton);
	BUTTON_Delete(hSoloButton);
	BUTTON_Delete(hMuteButton);
	BUTTON_Delete(hBackButton);

	return 0;
}


void VoiceSubWindowProc(WM_MESSAGE* pMsg)
{
	GUI_PID_STATE* pState = (GUI_PID_STATE*)pMsg->Data.p;
	WM_HWIN hWinString = 0;
	int NCode,Id;

	OptionSubWin *pOptionSubWin = OptionSubWin::pOptionSubWin;
	//WM_GetUserData(pMsg->hWin, &pOptionSubWin, sizeof(void *));

	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		pOptionSubWin->CreateItems(pMsg->hWin);
		break;

	case WM_DELETE:
		pOptionSubWin->DeleteItems();
		break;

	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		break;

	//case WM_TOUCH:
	//	delete pOptionSubWin;
	//	break;

	case WM_CAPTURE_RELEASED:
		delete pOptionSubWin;
		break;


	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(NCode)
		{
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("OptionSubWin button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case OPTION_VOICE_ID_DELETE:
					if(GUI_ID_OK == Misa_ConfirmBox("Are you sure?","Delete this voice!",GUI_MESSAGEBOX_CF_MODAL))
					{
						SynthTurnNotesOff();
						SynthDeleteVoice();
						TopSynthScreen(WM_GetParent(pMsg->hWin));
						pOptionSubWin->Hide();
					}
					break;

				case OPTION_VOICE_ID_SOLO:
					// Unselect Mute
					if (MisaCheckbox_GetStatus(pOptionSubWin->hMuteButton))
					{
						MisaCheckbox_SetStatus(pOptionSubWin->hMuteButton, 0);
						SynthMuteVoice(SynthGetStringIndex(), SynthGetVoiceIndex(), false);
					}
					SynthSoloVoice(SynthGetStringIndex(), SynthGetVoiceIndex(), (bool) MisaCheckbox_GetStatus(pMsg->hWinSrc));
					break;

				case OPTION_VOICE_ID_MUTE:
					if (MisaCheckbox_GetStatus(pOptionSubWin->hSoloButton))
					{
						MisaCheckbox_SetStatus(pOptionSubWin->hSoloButton, 0);
						SynthSoloVoice(SynthGetStringIndex(), SynthGetVoiceIndex(), false);
					}

					SynthMuteVoice(SynthGetStringIndex(), SynthGetVoiceIndex(), (bool) MisaCheckbox_GetStatus(pMsg->hWinSrc));
					break;

				case COMMON_ID_CLOSE:
					pOptionSubWin->Hide();
					break;

				}
				break;
		}

	default:
		WM_DefaultProc(pMsg);
	};

}
