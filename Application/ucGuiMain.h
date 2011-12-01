#ifndef __UCGUI_MAIN_H__
#define __UCGUI_MAIN_H__

U32 ucGuiMain(void* pPara);
void TerminateApplication();

U32 KitaraInit();
U32 KitaraExit();
U32 KitaraMenu();
U32 KitaraDisplaySceneName(const std::string &Name);
U32 KitaraQuitset();
U8 KitrarCreateBackPic(int flip);

// Only for test on Windows
U8 KitrarTestQuickset(int x,int y);

#endif // __UCGUI_MAIN_H__

