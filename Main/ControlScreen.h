#ifndef __CONTROLSCREEN_H__
#define __CONTROLSCREEN_H__

#include "Touchpanel.h"
#include <vector>
#include <stack>
#include "Graphics.h"
#include "Midi.h"
#include "Synth.h"
#include "tinyxml/tinyxml.h"

// OR  : Scene memories
#define SCENES_NUMBER		6

#define UI_NORMAL	 		0
#define UI_STRINGS			1

//fix: all #defines should be constants in the class
#define EVENT_BUTTON_PRESSED	0
#define EVENT_BUTTON_RELEASED	1
#define EVENT_BALL_PRESSED		2
#define EVENT_BALL_DRAGGED		3
#define EVENT_BALL_RELEASED		4
#define EVENT_BALL_PUSHED		5
#define EVENT_BALL_MOVED		6
#define EVENT_TOUCH_PRESSED		7
#define EVENT_TOUCH_DRAGGED		8
#define EVENT_TOUCH_RELEASED	9
#define EVENT_STRING_PRESSED	10
#define EVENT_STRING_DRAGGED	11
#define EVENT_STRING_RELEASED	12

struct bounding_box_t
{
	int x_start;
	int x_end;
	int y_start;
	int y_end;
};

struct touch_element
{
	int type;	//SELECT_BUTTON, SLIDER, BACKGROUND etc
	bool toggled;
	int x, y, width, height;
	int value;
	std::string text;
	bool visible;
};

struct coord
{
	int x;
	int y;
};

struct control_message_t
{
	int type;
	struct coord origin, current_location;
	int string_id;
	int button_id;
	int touch_id;
};

struct scene_memory_t
{
	std::string preset;

	int volume;
	int ball_mode;
	int string_mode;
	int ball_travel;
	int sustain_mode;
	int ringing_mode;
	int tap_mode;
};

class ControlScreen
{
	private:
		// OR  : Scene memories
		struct scene_memory_t scenes_memory[SCENES_NUMBER];

		std::stack<unsigned char> screens;
		unsigned int processDrag(unsigned int pos, unsigned int scaler, unsigned int origin);
		void setDragPointOriginX(unsigned char str, unsigned int o);
		void setDragPointOriginY(unsigned char str, unsigned int o);
		void setDragPointOriginXGlobal(unsigned int o);
		void setDragPointOriginYGlobal(unsigned int o);
		bool isDragXChanged(unsigned char str, unsigned int p);
		bool isDragXChangedGlobal(unsigned int p);
		bool isDragYChangedGlobal(unsigned int p);
		unsigned int scaleVelocity(unsigned int pos, unsigned int scaler);
		unsigned int centerTouch(unsigned int pos, unsigned int scaler);
		bool isInXYControlBounds(unsigned int new_x, unsigned int new_y);
		void identifyTouches(struct touchstate *state);
		// Chaniel remove the follow line 01032011
		//void playNotes(struct neckstate *state);
		unsigned int getDistance(unsigned int v1, unsigned int v2);
		bool isTouchDetected(int x, int y, int ex, int ey, int width, int height);

		std::vector<std::string> preset_filenames;

		struct bounding_box_t string_box[6];
		signed char current_note[6], ringing_note[6];
		unsigned int old_pitch[6];
		struct coord xy_control, ball_vector_samples[10], ball_vector;
		int ball_vector_index;
		bool ball_travel_on;
		bool no_buttons_pressed;

		std::vector<unsigned int> nt, bt, st[6], ost[6], touch_history;

		bool tap_mode, string_mode, left_handed, sustain_enabled, mode_ringing_notes;
		bool scene_enabled;		// OR  : Scene memories
		bool corner_pressed[4], quickset_start, tap_mode_toggle;

		Synth synth;

		struct touchstate touch_state, touch_state_w1, touch_state_w2;
		struct neckstate neck_state;
		
		void changeScreen(unsigned char screen_id);
		void popScreen(void);

		Graphics *graphics;
		
		std::vector<struct control_message_t *> event_queue;
		struct coord drag_origin[5], drag_position[5];
		int xy_timer;

		bool sustained_note[6];

		bool show_ball, show_strings;
		
		// String spacing  (OR 16-01-12)
		bool update_string_spacing; 
		int string_spacing;

	public:
		ControlScreen(Graphics *g);
		~ControlScreen(void);
		// OR  : Scene memories
		bool loadSceneFromConfig(TiXmlElement *e, int SceneNumber);
		bool saveSceneInConfig(TiXmlElement *root, int SceneNumber);
		std::string &getSceneName(int SceneNumber);

		void loadConfigFile(void);
		void saveConfigFile(void);
		void draw(Neck *neck, Touchpanel *ts);
		unsigned char update(Neck *neck, Touchpanel *ts);
		unsigned char updateBallWindow(void);
		unsigned char updateStringWindow();
		unsigned char convertButtonToNote(unsigned char button);
		unsigned int scalePitchWheel(unsigned char c);
		unsigned char midiScale(int val, int scaler);
		void updatePresetList(void);
		void playNotes(struct neckstate *state);
		void turnCurrentNotesOff(struct neckstate *state);
		Synth* GetSynth();
		struct control_message_t *newButtonEvent(int type, int string_id, int button_id);
		struct control_message_t *newTouchEvent(int type, int id, struct coord location);
		struct control_message_t *newAutoEvent(int type, struct coord location);
		struct control_message_t *newStringEvent(int type, int id, int str, struct coord location);
		void processEventQueue(void);
		void processEventButtonPressed(struct control_message_t *msg);
		void processEventButtonReleased(struct control_message_t *msg);
		void processEventBallPressed(struct control_message_t *msg);
		void processEventBallDragged(struct control_message_t *msg);
		void processEventBallReleased(void);
		void processEventBallPushed(struct control_message_t *msg);
		void processEventBallMoved(struct control_message_t *msg);
		void processEventTouchPressed(struct control_message_t *msg);
		void processEventTouchDragged(struct control_message_t *msg);
		void processEventTouchReleased(struct control_message_t *msg);
		void processEventStringPressed(struct control_message_t *msg);
		void processEventStringDragged(struct control_message_t *msg);
		void processEventStringReleased(struct control_message_t *msg);
		void setDragPointOrigin(int touch_id, struct coord *c);
		bool testForCornerSwitches(void);
		bool getBallTravelOn();
		void setBallTravelOn(bool state);
		void setLeftHandMode(bool state);
		bool getLeftHandMode(void);
		bool GetTapmode();
		void SetTapmode(bool state);
		bool isShowBall(void);
		void showBall(bool state);
		bool isShowStrings(void);
		void showStrings(bool state);
		bool isSustainEnabled(void);
		void setSustainEnabled(bool state);
		bool isRingingNotes(void);
		void setRingingNotes(bool state);
		// OR  : Scene memories
		bool isScenesEnabled(void);
		void setScenesEnabled(bool state);
		bool SwitchToScene(int SceneNumber);
		bool initScene(int SceneNumber, bool SaveIntoConfigFile = true);
		bool saveScene(int SceneNumber, const std::string &PresetName, int Volume, int Ball_mode, int String_mode,
							  int Ball_travel, int Sustain_mode, int Ringing_mode, int Tap_mode);

		// String spacing  (OR 16-01-12)
		void setStringSpacing(int space);

};

#endif
