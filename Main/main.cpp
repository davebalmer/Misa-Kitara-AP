#include <cstdlib>
#include <iostream>
#include <fstream>
#include "ControlScreen.h"
#include "Graphics.h"
#include "Touchpanel.h"
#include "Neck.h"
#include "Synth.h"

#include "GUI.h"
#include "ucGuiMain.h"

char *VERSION_STRING = "1.0.2";
char *VERSION_ID = "Misa Kitara Control System v1.0.2";

Touchpanel* pts;
Neck* pnk;
ControlScreen* pcs;
Synth* pSynth;

Graphics *pGraph;

void SetLcdFrameBuffer(unsigned long* pFrame);
std::string getSystemInfo(void);
bool doesFileExist(std::string path);
bool systemCheckPassed(void);

std::string working_directory;

int main(int argc, char **argv)
{
	if(!systemCheckPassed())
		return 1;
	
	if(argc == 2)
		working_directory = argv[1];
	else
		working_directory = "/usr/ap";

	std::cout << "Working directory: " << working_directory << std::endl << std::flush;

	const int FRAMES_PER_SECOND = 30;
	const int MS_PER_FRAME = 1000 / FRAMES_PER_SECOND;

	Graphics *graphics = new Graphics(argc, argv);

	ControlScreen *cs = new ControlScreen(graphics);

	Touchpanel ts;
	Neck neck;

	struct timespec now, lastIteration;
	int timePassedMs;

	clock_gettime(CLOCK_MONOTONIC, &lastIteration);

	pts = &ts;
	pnk = &neck;
	pcs = cs;
	pSynth = cs->GetSynth();
	pGraph = graphics;

///////////////////////////////////////////////////////////////////////////////
// Initialize menusystem
	KitaraInit();
///////////////////////////////////////////////////////////////////////////////

	graphics->hideFloatingWindows();
	graphics->setLCDScaled();

	while(1)
	{
//		clock_gettime(CLOCK_MONOTONIC, &now);

//		timePassedMs = ((now.tv_sec - lastIteration.tv_sec) * 1000) + ((now.tv_nsec - lastIteration.tv_nsec) / 1000000);

		//std::cout << "TimePassed= " << timePassedMs << std::endl << std::flush;

		cs->update(&neck, &ts);

//		if(timePassedMs >= MS_PER_FRAME)
		{
//			lastIteration = now;

			cs->draw(&neck, &ts);
		}
	}

	delete(graphics);

///////////////////////////////////////////////////////////////////////////////
// Quit menusystem
	KitaraExit();
///////////////////////////////////////////////////////////////////////////////
	
	return 0;
}

std::string getSystemInfo(void)
{
	std::string info;

	std::string serialNum = "read_mac_sn | grep SN";
	std::string tpVer = "modinfo " + working_directory + "/driver/nastech_ts.ko | grep -v srcversion | grep version | sed 's/version/TP/g'";
	std::string neckVer = "modinfo " + working_directory + "/driver/keyboard144.ko | grep -v srcversion | grep version | sed 's/version/NK/g'";
	std::string synthVer = "modinfo " + working_directory + "/driver/sam3716.ko | grep -v srcversion | grep version | sed 's/version/SY/g'";
	std::string powerVer = "modinfo " + working_directory + "/driver/guitar_key.ko | grep -v srcversion | grep version | sed 's/version/PW/g'";
	std::string dspVer = "echo -ne 'DSP: v'; strings " + working_directory + "/3716-FW_MISA.bin | grep 3716-MISA | cut -d\\  -f 2";
	std::string cmd = "(" + serialNum + " ; " + tpVer + " ; " + neckVer + " ; " + synthVer + " ; " + powerVer + " ; " + dspVer + ")" + " > /tmp/sysinfo";

	system(cmd.c_str());

	std::ifstream output_file("/tmp/sysinfo");

	std::string line;
	while(std::getline(output_file, line))
		info += (line + '\n');

	if(info.size() <= 3)
		info = "No driver version information found.";

//	std::cout << info << std::endl << std::flush;

	return std::string(VERSION_ID) + "\n\n" + info;
}

bool doesFileExist(std::string path)
{
	std::fstream fin;
	bool ret = false;

	fin.open(path.c_str(), std::ios::in);

	if(fin.is_open())
		ret = true;
	else
		std::cout << path << " does not exist." << std::endl << std::flush;

	fin.close();

	return ret;
}

bool systemCheckPassed(void)
{
	//the following system files must exist:

	if(!doesFileExist("/usr/bin/modinfo")) return false;
	if(!doesFileExist("/usr/bin/samprom")) return false;
	if(!doesFileExist("/usr/bin/power_key")) return false;
	if(!doesFileExist("/usr/bin/otgd")) return false;
	if(!doesFileExist("/usr/bin/read_mac_sn")) return false;

	std::string info;

	//make sure power_key is running
	std::string power_key_check = "ps | grep power_key | grep -v ps";
	std::string cmd = "(" + power_key_check + ")" + " > /tmp/systest";
	system(cmd.c_str());

	std::ifstream output_file("/tmp/systest");

	std::string line;
	while(std::getline(output_file, line))
		info += (line + '\n');

	if(info.size() <= 3)
	{
		std::cout << "power_key is not running." << std::endl << std::flush;
		return false;
	}

	return true;
}
