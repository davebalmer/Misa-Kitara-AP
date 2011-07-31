#include "ControlScreen.h"
#include <iostream>
#include "Touchpanel.h"
#include <vector>
#include <algorithm>
#include "Synth.h"
#include <sys/types.h>
#include <dirent.h>

#include "GUI.h"
#include "ucGuiMain.h"

extern std::string working_directory;

ControlScreen::ControlScreen(Graphics *g)
{
	graphics = g;

	tap_mode = false;
	string_mode = false;
	left_handed = false;

	show_ball = false;
	show_strings = false;
	graphics->showBall(show_ball);
	graphics->showStrings(show_strings);
	
	changeScreen(UI_NORMAL);

	graphics->clearScreen(0x000000);
	graphics->hideFloatingWindows();
	graphics->setLCDScaled();

	for(int i = 0; i < 6; i++)
	{
		int new_y = 50 + (100 * (5-i)) - 1;
		graphics->setStringPosition(i, new_y);
		string_box[i].y_start = new_y - 49;
		string_box[i].y_end = new_y + 50;
		current_note[i] = -1;
		ringing_note[i] = -1;
//		sustained_note[i] = false;
		old_pitch[i] = 0x2000;
	}

	xy_control.x = graphics->getWindow1Width()/2;
	xy_control.y = 300;
	graphics->setXYControlPosition(xy_control.x, xy_control.y);

	synth.sendControl(TOUCH_X, graphics->getWindow1Width()/2, graphics->getWindow1Width());
	synth.sendControl(TOUCH_Y, graphics->getScreenHeight()/2, graphics->getScreenHeight());
	graphics->setDragOriginIndicator(graphics->getWindow1Width()/2, graphics->getScreenHeight()/2);

	ball_travel_on = false;
	ball_vector_index = 0;
	ball_vector.x = 0; ball_vector.y = 0;
	for(int i = 0; i < 10; i++)
	{
		ball_vector_samples[i].x = 0;
		ball_vector_samples[i].y = 0;
	}

	quickset_start = false;
	tap_mode_toggle = false;
	for(int i = 0; i < 4; i++)
		corner_pressed[i] = false;

	updatePresetList();

	if(preset_filenames.size() == 0)
	{
		std::cerr << "No presets found. Exiting." << std::endl << std::flush;
		exit(1);
	}
	else
	{
		synth.loadPresetFromFile(preset_filenames.at(0));
	}
}

ControlScreen::~ControlScreen()
{
}

void ControlScreen::draw(Neck *neck, Touchpanel *ts)
{
	switch(screens.top())
	{
		default:
			graphics->draw();
			break;
	}
}

unsigned char ControlScreen::midiScale(int number, int scaler)
{
	if(scaler == 0) return 0;

	int val = (number * 127) / scaler;

	return (unsigned char) val;
}

unsigned int ControlScreen::scaleVelocity(unsigned int pos, unsigned int scaler)
{
	int vel = pos;

	//split velocity control in half
	if(vel >= scaler / 2)
		vel = scaler - vel;
	vel = midiScale(vel, scaler / 2);

	return vel;
}

unsigned int ControlScreen::centerTouch(unsigned int pos, unsigned int scaler)
{
	int vel = pos;

	//split control in half
	if(vel >= scaler / 2)
		vel = scaler - vel;

	return vel;
}

bool ControlScreen::isDragXChanged(unsigned char str, unsigned int p)
{
	if(old_pitch[str] != p)
	{
		old_pitch[str] = p;
		return true;
	}
	else
		return false;
}

unsigned char ControlScreen::update(Neck *neck, Touchpanel *ts)
{
	ts->getTouchState(&touch_state);
	neck->getNeckState(&neck_state, left_handed);

	static int old_button[6] = {0, 0, 0, 0, 0, 0};

	//identify new button presses
	for(unsigned int s = 0; s < 6; s++)
	{
		if(neck_state.string_button[s] != old_button[s])
		{
			if(neck_state.string_button[s] == 0)
				event_queue.push_back(newButtonEvent(EVENT_BUTTON_RELEASED, s, neck_state.string_button[s]));
			else
			if(neck_state.string_button[s] > old_button[s])
				event_queue.push_back(newButtonEvent(EVENT_BUTTON_PRESSED, s, neck_state.string_button[s]));
			else
			if(neck_state.string_button[s] < old_button[s])
				event_queue.push_back(newButtonEvent(EVENT_BUTTON_RELEASED, s, neck_state.string_button[s]));

			old_button[s] = neck_state.string_button[s];
		}
	}

	if(show_strings)
	{
		graphics->setWindow1Width(550);
		graphics->setWindow2Width(graphics->getScreenWidth() - 550);
	}
	else
	{
		graphics->setWindow1Width(800);
		graphics->setWindow2Width(0);
	}

	for(int i = 0; i < 5; i++)
	{
		if(touch_state.x[i] < graphics->getWindow1Width())
		{
			touch_state_w1.x[i] = touch_state.x[i];
			touch_state_w1.y[i] = touch_state.y[i];
			touch_state_w2.x[i] = 0;
			touch_state_w2.y[i] = 0;
		}
		else
		{
			touch_state_w2.x[i] = touch_state.x[i] - graphics->getWindow1Width();
			touch_state_w2.y[i] = touch_state.y[i];
			touch_state_w1.x[i] = 0;
			touch_state_w1.y[i] = 0;
		}
	}

	switch(screens.top())
	{
		case UI_NORMAL:
			updateBallWindow();

			if(show_strings)
				updateStringWindow();

			testForCornerSwitches();
			break;
	}

	processEventQueue();

	return 0;
}

void ControlScreen::testForCornerSwitches(void)
{
	corner_pressed[0] = false;
	corner_pressed[1] = false;
	corner_pressed[2] = false;
	corner_pressed[3] = false;

	for(int i = 0; i < 5; i++)
	{
		if((touch_state.x[i] != 0) || (touch_state.y[i] != 0))
		{
			if((touch_state.x[i] >= graphics->getScreenWidth()-80) && (touch_state.y[i] >= graphics->getScreenHeight()-80))
				corner_pressed[0] = true;

			if((touch_state.x[i] >= graphics->getScreenWidth()-80) && (touch_state.y[i] <= 80))
				corner_pressed[1] = true;

			if((touch_state.x[i] <= 80) && (touch_state.y[i] >= graphics->getScreenHeight()-80))
				corner_pressed[2] = true;

			if((touch_state.x[i] <= 80) && (touch_state.y[i] <= 80))
				corner_pressed[3] = true;
		}
	}

	if(corner_pressed[2] && corner_pressed[3])
		quickset_start = true;

	if(quickset_start)
	{
		bool flag = false;
		for(int i = 0; i < 5; i++)
			if((touch_state.x[i] != 0) || (touch_state.y[i] != 0))
				flag = true;

		if(!flag)
		{
			graphics->setPaused(true);
			usleep(1000*60);
			synth.sendControl(TOUCH_X, graphics->getWindow1Width()/2, graphics->getWindow1Width());
			synth.sendControl(TOUCH_Y, graphics->getScreenHeight()/2, graphics->getScreenHeight());
			synth.sendControl(DRAG_X, graphics->getWindow1Width()/2, graphics->getWindow1Width());
			synth.sendControl(DRAG_Y, graphics->getScreenHeight()/2, graphics->getScreenHeight());
			if(show_ball)
			{
				synth.sendControl(BALL_X, graphics->getWindow1Width()/2, graphics->getWindow1Width());
				synth.sendControl(BALL_Y, graphics->getScreenHeight()/2, graphics->getScreenHeight());
				xy_control.x = graphics->getWindow1Width()/2;
				xy_control.y = graphics->getScreenHeight()/2;
				graphics->setXYControlPosition(xy_control.x, xy_control.y);
			}

			graphics->hideFloatingWindows();
			graphics->setLCDUnScaled();

			//trigger to start test mode
			if((neck_state.string_button[0] == 24) && (neck_state.string_button[5] == 24))
			{
				system("/usr/test_mode/testmode; reboot");
				exit(1);
			}

			synth.sendCurrentSynthNotesOff();

			KitaraMenu();
			graphics->setLCDScaled();

			graphics->setDragOriginIndicator(graphics->getWindow1Width()/2, graphics->getScreenHeight()/2);
			graphics->setPaused(false);
			corner_pressed[2] = false;
			corner_pressed[3] = false;
			quickset_start = false;
		}
	}
}

unsigned char ControlScreen::updateStringWindow(void)
{
	for(int s = 0; s < 6; s++)
	{
		for(int i = 0; i < 5; i++)
		{
			if((touch_state_w2.x[i] != 0) || (touch_state_w2.y[i] != 0))
			{
				if((touch_state_w2.y[i] > string_box[s].y_start) 
				&& (touch_state_w2.y[i] < string_box[s].y_end))
				{
					std::vector<unsigned int>::iterator t;
					t = find(st[s].begin(), st[s].end(), i);
					if(t == st[s].end())
					{
						struct coord touch_location;
						touch_location.x = touch_state_w2.x[i];
						touch_location.y = touch_state_w2.y[i];
						event_queue.push_back(newStringEvent(EVENT_STRING_PRESSED, i, left_handed?5-s:s, touch_location));
						setDragPointOrigin(i, &touch_location);
						st[s].push_back(i);
					}
					else
					{
						if(getDistance(drag_origin[i].x, touch_state_w2.x[i]) >= 3)
							if(getDistance(drag_position[i].x, touch_state_w2.x[i]) >= 3)
							{
								struct coord touch_location;
								touch_location.x = touch_state_w2.x[i];
								touch_location.y = touch_state_w2.y[i];
								event_queue.push_back(newStringEvent(EVENT_STRING_DRAGGED, i, left_handed?5-s:s, touch_location));
								drag_position[i] = touch_location;
							}
					}
				}
				else
				{
					std::vector<unsigned int>::iterator t;
					t = find(st[s].begin(), st[s].end(), i);
					if(t != st[s].end())
					{
						struct coord temp = {0, 0};
						event_queue.push_back(newStringEvent(EVENT_STRING_RELEASED, i, left_handed?5-s:s, temp));
						st[s].erase(t);
					}
				}
			}
			else
			if((touch_state_w2.x[i] == 0) && (touch_state_w2.y[i] == 0))
			{
				std::vector<unsigned int>::iterator t;

				t = find(st[s].begin(), st[s].end(), i);
				if(t != st[s].end())
				{
					struct coord temp = {0, 0};
					event_queue.push_back(newStringEvent(EVENT_STRING_RELEASED, i, left_handed?5-s:s, temp));
					st[s].erase(t);
				}
			}
		}
	}

	return 0;
}

void ControlScreen::identifyTouches(struct touchstate *state)
{
	for(int i = 0; i < touch_history.size(); i++)
	{
		if(isInXYControlBounds(state->x[touch_history.at(i)], state->y[touch_history.at(i)]))
			bt.push_back(touch_history.at(i));
		else
			nt.push_back(touch_history.at(i));
	}
}

void ControlScreen::playNotes(struct neckstate *state)
{
	for(unsigned int s = 0; s < 6; s++)
	{
		if(state->string_button[s] != current_note[s])
		{
			if(state->string_button[s] != 0)
			{
				synth.sendNoteOn(s, state->string_button[s], true);

				if(current_note[s] != -1)
					synth.sendNoteOffRinging(s, current_note[s]);

				current_note[s] = state->string_button[s];
			}
			else //not pressed any more
			{
				if(current_note[s] != -1)
					synth.sendNoteOffRinging(s, current_note[s]);

				current_note[s] = -1;
			}
		}
	}
}

void ControlScreen::turnCurrentNotesOff(struct neckstate *state)
{
	for(unsigned int s = 0; s < 6; s++)
	{
		if(current_note[s] != -1)
			synth.sendNoteOff(s, current_note[s]);

		current_note[s] = -1;
	}
}

unsigned int ControlScreen::getDistance(unsigned int v1, unsigned int v2)
{
	if(v1 <= v2)
		return v2 - v1;

	if(v2 < v1)
		return v1 - v2;
}

struct control_message_t *ControlScreen::newButtonEvent(int type, int string_id, int button_id)
{
	struct control_message_t *m = new struct control_message_t;

	m->type = type;
	m->string_id = string_id;
	m->button_id = button_id;

	return m;
}

struct control_message_t *ControlScreen::newTouchEvent(int type, int id, struct coord location)
{
	struct control_message_t *m = new struct control_message_t;

	m->type = type;
	m->touch_id = id;
	m->current_location = location;

	return m;
}

struct control_message_t *ControlScreen::newAutoEvent(int type, struct coord location)
{
	struct control_message_t *m = new struct control_message_t;

	m->type = type;
	m->current_location = location;

	return m;
}

struct control_message_t *ControlScreen::newStringEvent(int type, int id, int str, struct coord location)
{
	struct control_message_t *m = new struct control_message_t;

	m->type = type;
	m->touch_id = id;
	m->string_id = str;
	m->current_location = location;

	return m;
}

void ControlScreen::processEventButtonPressed(struct control_message_t *msg)
{
//	std::cout << "Button pressed." << std::flush;

	//if all buttons were unpressed and the drag is not reset, then reset it
	bool button_pressed = false;
	for(int s = 0; s < 6; s++)
	{
		if(s == msg->string_id)
			continue;

		if(current_note[s] != -1)
			button_pressed = true;
	}

	if(((screens.top() == UI_NORMAL) && ((nt.size() != 0) || (bt.size() != 0))) ||
	  ((st[left_handed?5-msg->string_id:msg->string_id].size() != 0)) ||
	  (tap_mode))
	{
		synth.sendNoteOn(msg->string_id, msg->button_id, false);

//		if(current_note[msg->string_id] != -1)
//			synth.sendNoteOff(msg->string_id, current_note[msg->string_id]);

		current_note[msg->string_id] = msg->button_id;
		ringing_note[msg->string_id] = current_note[msg->string_id];
	}
}

void ControlScreen::processEventButtonReleased(struct control_message_t *msg)
{
//	std::cout << "Button released." << std::flush;

//	if(sustained_note[msg->string_id]) return;

	//pull off to another note
	if((msg->button_id != 0) || ((msg->button_id == 0) && (st[left_handed?5-msg->string_id:msg->string_id].size() > 0)))
	{
		if(((screens.top() == UI_NORMAL) && ((nt.size() > 0) || (bt.size() > 0))) ||
		  (((st[left_handed?5-msg->string_id:msg->string_id].size() > 0))) ||
		  (tap_mode))
		{
			synth.sendNoteOn(msg->string_id, msg->button_id, false);

//			if(current_note[msg->string_id] != -1)
//				synth.sendNoteOff(msg->string_id, current_note[msg->string_id]);

			current_note[msg->string_id] = msg->button_id;
			ringing_note[msg->string_id] = current_note[msg->string_id];
		}
	}
	else
	if(ringing_note[msg->string_id] != -1)
	{
//		if(current_note[msg->string_id] != -1)
//			synth.sendNoteOff(msg->string_id, current_note[msg->string_id]);

//		if(ringing_note[msg->string_id] != current_note[msg->string_id])
			synth.sendNoteOff(msg->string_id, ringing_note[msg->string_id]);

//		synth.sendStopSound(msg->string_id, ringing_note[msg->string_id]);

		current_note[msg->string_id] = -1;
		ringing_note[msg->string_id] = current_note[msg->string_id];
	}
}

void ControlScreen::processEventBallPressed(struct control_message_t *msg)
{
//	std::cout << "Ball pressed." << std::flush;
}

void ControlScreen::processEventBallDragged(struct control_message_t *msg)
{
//	std::cout << "Ball dragged." << std::flush;

	synth.sendControl(BALL_X, msg->current_location.x, graphics->getWindow1Width());
	synth.sendControl(BALL_Y, left_handed?graphics->getScreenHeight()-msg->current_location.y:msg->current_location.y, graphics->getScreenHeight());

	xy_control.x = msg->current_location.x;
	xy_control.y = msg->current_location.y;
	graphics->setXYControlPosition(xy_control.x, xy_control.y);
}

void ControlScreen::processEventBallReleased(struct control_message_t *msg)
{
//	std::cout << "Ball released." << std::flush;

	if((nt.size() == 0) && (bt.size() == 0) && !tap_mode)
	{
		for(int s = 0; s < 6; s++)
		{
			if(st[left_handed?5-s:s].size() == 0)
				if(current_note[s] != -1)
				{
//					synth.sendNoteOff(s, current_note[s]);
					synth.sendNoteOffRinging(s, current_note[s]);
					current_note[s] = -1;
				}
		}
	}
}

void ControlScreen::processEventBallPushed(struct control_message_t *msg)
{
//	std::cout << "Ball pushed." << std::flush;
}

void ControlScreen::processEventBallMoved(struct control_message_t *msg)
{
//	std::cout << "Ball moved." << std::flush;
	synth.sendControl(BALL_X, msg->current_location.x, graphics->getWindow1Width());
	synth.sendControl(BALL_Y, left_handed?graphics->getScreenHeight()-msg->current_location.y:msg->current_location.y, graphics->getScreenHeight());
	xy_control.x = msg->current_location.x;
	xy_control.y = msg->current_location.y;
	graphics->setXYControlPosition(xy_control.x, xy_control.y);
}

void ControlScreen::setDragPointOrigin(int touch_id, struct coord *c)
{
	drag_origin[touch_id].x = c->x;
	drag_origin[touch_id].y = left_handed?graphics->getScreenHeight()-c->y:c->y;
	drag_position[touch_id] = drag_origin[touch_id];
}

void ControlScreen::processEventTouchPressed(struct control_message_t *msg)
{
//	std::cout << "Touch pressed " << (unsigned int) msg->touch_id << "." << std::endl << std::flush;

	synth.sendControl(TOUCH_X, graphics->getWindow1Width() - msg->current_location.x, graphics->getWindow1Width());
	synth.sendControl(TOUCH_Y, left_handed?(graphics->getScreenHeight()-msg->current_location.y):msg->current_location.y, graphics->getScreenHeight());

	//only reset drag if it is the first touch
	//fix: doesn't work if two fingers pressed down at same time

/*	if(nt.size() == 5)
	{
		for(int i = 0; i < 6; i++)
			if(neck_state.string_button[i] != 0)
				sustained_note[i] = true;
			else
				sustained_note[i] = false;
	}
*/

	bool flag = false;	
	for(unsigned int s = 0; s < 6; s++)
	{
/*		if(sustained_note[s])
		{
			flag = true;
			continue;
		}
*/
		if(st[left_handed?5-s:s].size() == 0)
		{
			//turn off any playing notes
//			if(current_note[s] != -1)
//				synth.sendNoteOff(s, current_note[s]);
//might need to add code to handle above
			//play note
			if(neck_state.string_button[s] != 0)
			{
				flag = true;
				current_note[s] = neck_state.string_button[s];
				ringing_note[s] = current_note[s];
				synth.sendNoteOn(s, current_note[s], true);
			}
		}
	}
	
	if(!flag)
	//if no buttons pressed, silence any ringing notes (happens with open strings)
		for(int i = 0; i < 6; i++)
		{
//			sustained_note[i] = false;
//			if((ringing_note[i] != -1) && (st[left_handed?5-i:i].size() == 0))
			if(st[left_handed?5-i:i].size() == 0)
			{
//				if(ringing_note[i] != -1)
//					synth.sendNoteOff(i, ringing_note[i]);
				synth.sendStopSound(i, ringing_note[i]);
//				ringing_note[i] = -1;
			}
		}

	graphics->setDragOriginIndicator(msg->current_location.x, msg->current_location.y);
	graphics->setTouchIndicator(msg->touch_id, msg->current_location.x, msg->current_location.y);
}

void ControlScreen::processEventTouchDragged(struct control_message_t *msg)
{
//	std::cout << "Touch dragged." << std::endl << std::flush;
	if(nt.size() == 0)
	{
		std::cout << "This should not have happened." << std::endl << std::flush;
		return;
	}

	if(msg->touch_id == nt.at(nt.size() - 1))
	{
		unsigned int drag_x = processDrag(msg->current_location.x, graphics->getWindow1Width(), drag_origin[msg->touch_id].x);
		synth.sendControl(DRAG_X, drag_x, graphics->getWindow1Width());

		unsigned int drag_y;
		if(left_handed)
			drag_y = processDrag(graphics->getScreenHeight()-msg->current_location.y, graphics->getScreenHeight(), drag_origin[msg->touch_id].y);
		else
			drag_y = processDrag(msg->current_location.y, graphics->getScreenHeight(), drag_origin[msg->touch_id].y);

		synth.sendControl(DRAG_Y, drag_y, graphics->getScreenHeight());
	}

	graphics->setTouchIndicator(msg->touch_id, msg->current_location.x, msg->current_location.y);
}

void ControlScreen::processEventTouchReleased(struct control_message_t *msg)
{
//	std::cout << "Touch released " << (unsigned int) msg->touch_id << "." << std::endl << std::flush;

	if(nt.size() == 0)
	{
		synth.sendControl(DRAG_X, graphics->getWindow1Width()/2, graphics->getWindow1Width());
		synth.sendControl(DRAG_Y, graphics->getScreenHeight()/2, graphics->getScreenHeight());
	}

	if((nt.size() == 0) && (bt.size() == 0))
	{
		for(int s = 0; s < 6; s++)
		{
//			if((current_note[s] != -1) && (st[left_handed?5-s:s].size() == 0) && !sustained_note[s])
			if((current_note[s] != -1) && (st[left_handed?5-s:s].size() == 0) && !tap_mode)
			{
//				synth.sendNoteOff(s, current_note[s]);
				synth.sendNoteOffRinging(s, current_note[s]);
				current_note[s] = -1;
			}
		}
	}
	
	graphics->hideTouchIndicator(msg->touch_id);
}

void ControlScreen::processEventStringPressed(struct control_message_t *msg)
{
//	std::cout << "String pressed." << std::flush;

	int touch_control;

	switch(msg->string_id)
	{
		case 0: touch_control = STRING_TOUCH_X_0; break;
		case 1: touch_control = STRING_TOUCH_X_1; break;
		case 2: touch_control = STRING_TOUCH_X_2; break;
		case 3: touch_control = STRING_TOUCH_X_3; break;
		case 4: touch_control = STRING_TOUCH_X_4; break;
		case 5: touch_control = STRING_TOUCH_X_5; break;
	}

	synth.sendControl(touch_control, graphics->getWindow2Width() - msg->current_location.x, graphics->getWindow2Width());

	//turn off any playing notes
//	if(current_note[msg->string_id] != -1)
//		synth.sendNoteOff(msg->string_id, current_note[msg->string_id]);

	if(st[left_handed?5-msg->string_id:msg->string_id].size() == 1)
	{
		int drag_control;
		switch(msg->string_id)
		{
			case 0: drag_control = STRING_DRAG_X_0; break;
			case 1: drag_control = STRING_DRAG_X_1; break;
			case 2: drag_control = STRING_DRAG_X_2; break;
			case 3: drag_control = STRING_DRAG_X_3; break;
			case 4: drag_control = STRING_DRAG_X_4; break;
			case 5: drag_control = STRING_DRAG_X_5; break;
		}

		synth.sendControl(drag_control, graphics->getWindow2Width()/2, graphics->getWindow2Width());
	}

	//play note
	current_note[msg->string_id] = neck_state.string_button[msg->string_id];
	ringing_note[msg->string_id] = current_note[msg->string_id];
	synth.sendNoteOn(msg->string_id, current_note[msg->string_id], true);

	graphics->setStringPressed(left_handed?5-msg->string_id:msg->string_id, true);
}

void ControlScreen::processEventStringDragged(struct control_message_t *msg)
{
//	std::cout << "String " << msg->string_id << " dragged." << std::flush;

	unsigned int drag_x = processDrag(msg->current_location.x, graphics->getWindow2Width(), drag_origin[msg->touch_id].x);

	int control;
	switch(msg->string_id)
	{
		case 0: control = STRING_DRAG_X_0; break;
		case 1: control = STRING_DRAG_X_1; break;
		case 2: control = STRING_DRAG_X_2; break;
		case 3: control = STRING_DRAG_X_3; break;
		case 4: control = STRING_DRAG_X_4; break;
		case 5: control = STRING_DRAG_X_5; break;
	}

	synth.sendControl(control, drag_x, graphics->getWindow2Width());
}

void ControlScreen::processEventStringReleased(struct control_message_t *msg)
{
//	std::cout << "String released." << std::flush;

	if(st[left_handed?5-msg->string_id:msg->string_id].size() == 0)
	{
//		if((nt.size() == 0) && (bt.size() == 0))
		{
			if(current_note[msg->string_id] != -1)
			{
				if(tap_mode)
				{
					if(current_note[msg->string_id] == 0)
					{
//						synth.sendNoteOff(msg->string_id, current_note[msg->string_id]);
						synth.sendNoteOffRinging(msg->string_id, current_note[msg->string_id]);
						current_note[msg->string_id] = -1;
					}
				}
				else
				if(!tap_mode)
				{
//					synth.sendNoteOff(msg->string_id, current_note[msg->string_id]);
					synth.sendNoteOffRinging(msg->string_id, current_note[msg->string_id]);
					current_note[msg->string_id] = -1;
				}
			}
		}

		graphics->setStringPressed(left_handed?5-msg->string_id:msg->string_id, false);
	}
}

void ControlScreen::processEventQueue(void)
{
	for(int i = 0; i < event_queue.size(); i++)
	{
		struct control_message_t *msg = event_queue.at(i);

		switch(msg->type)
		{
			case EVENT_BUTTON_PRESSED:
				processEventButtonPressed(msg);
				break;

			case EVENT_BUTTON_RELEASED:
				processEventButtonReleased(msg);
				break;

			case EVENT_BALL_PRESSED:
				processEventBallPressed(msg);
				break;

			case EVENT_BALL_DRAGGED:
				processEventBallDragged(msg);
				break;

			case EVENT_BALL_RELEASED:
				processEventBallReleased(msg);
				break;

			case EVENT_BALL_PUSHED:
				processEventBallPushed(msg);
				break;

			case EVENT_BALL_MOVED:
				processEventBallMoved(msg);
				break;

			case EVENT_TOUCH_PRESSED:
				processEventTouchPressed(msg);
				break;

			case EVENT_TOUCH_DRAGGED:
				processEventTouchDragged(msg);
				break;

			case EVENT_TOUCH_RELEASED:
				processEventTouchReleased(msg);
				break;

			case EVENT_STRING_PRESSED:
				processEventStringPressed(msg);
				break;

			case EVENT_STRING_DRAGGED:
				processEventStringDragged(msg);
				break;

			case EVENT_STRING_RELEASED:
				processEventStringReleased(msg);
				break;
		}

		delete(msg);
	}
	event_queue.clear();
}

unsigned char ControlScreen::updateBallWindow(void)
{
	//identify new touches
	for(int i = 0; i < 5; i++)
	{
		if((touch_state_w1.x[i] != 0) || (touch_state_w1.y[i] != 0))
		{
			if((find(nt.begin(), nt.end(), i) == nt.end()) &&
			 (find(bt.begin(), bt.end(), i) == bt.end()))
			{
				if((show_ball) && (isInXYControlBounds(touch_state_w1.x[i], touch_state_w1.y[i])))
				{
					struct coord touch_location;
					touch_location.x = touch_state_w1.x[i];
					touch_location.y = touch_state_w1.y[i];
					event_queue.push_back(newTouchEvent(EVENT_BALL_PRESSED, i, touch_location));
					xy_timer = 0;

					ball_vector.x = 0; ball_vector.y = 0;
					ball_vector_index = 0;
					for(int j = 0; j < 10; j++)
					{
						ball_vector_samples[j].x = 0;
						ball_vector_samples[j].y = 0;
					}

					bt.push_back(i);
				}
				else
				{
					struct coord touch_location;
					touch_location.x = touch_state_w1.x[i];
					touch_location.y = touch_state_w1.y[i];
					event_queue.push_back(newTouchEvent(EVENT_TOUCH_PRESSED, i, touch_location));
					setDragPointOrigin(i, &touch_location);
					nt.push_back(i);
				}
			}
		}
		else
		if((touch_state_w1.x[i] == 0) && (touch_state_w1.y[i] == 0))
		{
			std::vector<unsigned int>::iterator t;

			t = find(nt.begin(), nt.end(), i);
			if(t != nt.end())
			{
				struct coord temp = {0, 0};
				event_queue.push_back(newTouchEvent(EVENT_TOUCH_RELEASED, i, temp));
				nt.erase(t);
			}

			if(show_ball)
			{
				t = find(bt.begin(), bt.end(), i);
				if(t != bt.end())
				{
					struct coord temp = {0, 0};
					if((xy_timer < 1500) && (ball_travel_on))
					{
						event_queue.push_back(newTouchEvent(EVENT_BALL_PUSHED, i, temp));
					}
					else
					{
						ball_vector.x = 0; ball_vector.y = 0; ball_vector_index = 0;
						event_queue.push_back(newTouchEvent(EVENT_BALL_RELEASED, i, temp));
					}

					bt.erase(t);
				}
			}
		}
	}

	if((bt.size() > 0) && show_ball)
	{
		if((getDistance(xy_control.x, touch_state_w1.x[bt.at(0)]) >= 3) ||
		 (getDistance(xy_control.y, touch_state_w1.y[bt.at(0)]) >= 3))
		{
			struct coord touch_location;
			touch_location.x = touch_state_w1.x[bt.at(0)];
			touch_location.y = touch_state_w1.y[bt.at(0)];

			if(ball_travel_on)
			{
				ball_vector_samples[ball_vector_index++].x = xy_control.x - touch_location.x;
				ball_vector_samples[ball_vector_index++].y = xy_control.y - touch_location.y;
				if(ball_vector_index >= 10) ball_vector_index = 0;

				ball_vector.x = 0; ball_vector.y = 0;
				for(int i = 0; i < 10; i++)
				{
					ball_vector.x += ball_vector_samples[i].x;
					ball_vector.y += ball_vector_samples[i].y;
				}
				ball_vector.x /= 10;
				ball_vector.y /= 10;

				if((ball_vector.x > 0) && (ball_vector.x < 4)) ball_vector.x = 0;
				else if((ball_vector.x < 0) && (ball_vector.x > -4)) ball_vector.x = 0;
				if((ball_vector.y > 0) && (ball_vector.y < 4)) ball_vector.y = 0;
				else if((ball_vector.y < 0) && (ball_vector.y > -4)) ball_vector.y = 0;
			}

			event_queue.push_back(newTouchEvent(EVENT_BALL_DRAGGED, bt.at(0), touch_location));
			xy_timer = 0;
		}
	}
	else
	{	//this should run even if the ball is hidden
		if(ball_travel_on && (xy_timer > 500)) //only update ball movement every 100 iterations
		{
			xy_timer = 0;

			struct coord new_location;
			new_location.x = xy_control.x - (ball_vector.x / 2);
			new_location.y = xy_control.y - (ball_vector.y / 2);
	
			if(new_location.x >= graphics->getWindow1Width())
			{
				new_location.x = graphics->getWindow1Width() - 1;
				ball_vector.x *= -1;
			}
			else
			if(new_location.x < 0)
			{
				new_location.x = 0;
				ball_vector.x *= -1;
			}

			if(new_location.y >= graphics->getScreenHeight())
			{
				new_location.y = graphics->getScreenHeight() - 1;
				ball_vector.y *= -1;
			}
			else
			if(new_location.y < 0)
			{
				new_location.y = 0;
				ball_vector.y *= -1;
			}

			event_queue.push_back(newAutoEvent(EVENT_BALL_MOVED, new_location));
		}
	}

	xy_timer++;

	for(int i = 0; i < nt.size(); i++)
	{
		if((getDistance(drag_origin[nt.at(i)].x, touch_state_w1.x[nt.at(i)]) >= 3) ||
		 (getDistance(drag_origin[nt.at(i)].y, touch_state_w1.y[nt.at(i)]) >= 3))
		{
			if((getDistance(drag_position[nt.at(i)].x, touch_state_w1.x[nt.at(i)]) >= 3) ||
			 (getDistance(drag_position[nt.at(i)].y, touch_state_w1.y[nt.at(i)]) >= 3))
			{
				struct coord touch_location;
				touch_location.x = touch_state_w1.x[nt.at(i)];
				touch_location.y = touch_state_w1.y[nt.at(i)];
				event_queue.push_back(newTouchEvent(EVENT_TOUCH_DRAGGED, nt.at(i), touch_location));
				drag_position[nt.at(i)] = touch_location;
			}
		}
	}
	
	return 0;
}

bool ControlScreen::isTouchDetected(int x, int y, int ex, int ey, int width, int height)
{
	if((x >= ex) && (x <= ex + width) && (y >= ey) && (y <= ey + height))
		return true;
	else
		return false;
}

void ControlScreen::updatePresetList(void)
{
	DIR *dp;
	struct dirent *ep;

	preset_filenames.clear();

	dp = opendir(std::string(working_directory + "/presets/").c_str());
	if(dp != NULL)
	{
		while((ep = readdir(dp)) != NULL)
		{
			std::string s = ep->d_name;
			if(s.size() > 3)
				if(s.substr(s.size() - 3, 3) == ".mz")
					preset_filenames.push_back(s.substr(0, s.size() - 3));
		}
	}

	sort(preset_filenames.begin(), preset_filenames.end());

	closedir(dp);
}

bool ControlScreen::isInXYControlBounds(unsigned int new_x, unsigned int new_y)
{
	int nx = new_x;
	int ny = new_y;

	//fix: change constant to  xy_control_width
	if((nx > xy_control.x - 100) && (nx < xy_control.x + 100)
	&& (ny > xy_control.y - 100) && (ny < xy_control.y + 100))
		return true;
	else
		return false;
}

unsigned int ControlScreen::processDrag(unsigned int pos, unsigned int scaler, unsigned int origin)
{
	int val = scaler / 2; //reset value in middle
	if(pos > origin)
	{
		int drag = pos - origin;
		if(drag >= 20)
		{
			int new_origin = origin + 20;
			int total = scaler - new_origin;
			if(total <= 0) total = 1;
			assert(total!=0); //FPE
			val = (scaler / 2) - (((pos - new_origin) * (scaler / 2)) / total);
			if(val >= scaler) val = scaler - 1;
		}
	}
	else
	{
		int drag = origin - pos;
		if(drag >= 20)
		{
			int new_origin = origin - 20;
			if(new_origin <= 0) new_origin = 1;
			int total = new_origin;			
			assert(total != 0); //FPE
			val = (scaler / 2) + ((new_origin - pos) * (scaler / 2) / total);
			if(val < 0) val = 0;
		}
	}
	return val;
}

unsigned int ControlScreen::scalePitchWheel(unsigned char c)
{
	unsigned int pitch = c;

	c <<= 7;	//same as pitch *= 128;
	c <<= 1;	//get in to right format for midi message

	return pitch;
}

void ControlScreen::changeScreen(unsigned char screen_id)
{
	switch(screen_id)
	{
		case UI_NORMAL:
			if(screens.size() > 0) screens.pop();
			graphics->setGraphicsModeNormal(tap_mode);
			break;
	}
	screens.push(screen_id);
}


void ControlScreen::popScreen(void)
{
	screens.pop();
	changeScreen(screens.top());
}

bool ControlScreen::getBallTravelOn()
{
	return ball_travel_on;
}

void ControlScreen::setBallTravelOn(bool state)
{
	ball_travel_on = state;
}

void ControlScreen::setLeftHandMode(bool state)
{
	if(left_handed != state)
	{
		turnCurrentNotesOff(&neck_state);
		left_handed = state;

		for(int i = 0; i < 3; i++)
		{
			unsigned char temp = neck_state.string_button[i];
			neck_state.string_button[i] = neck_state.string_button[5-i];
			neck_state.string_button[5-i] = temp;
		}
	}
}

Synth* ControlScreen::GetSynth()
{
	return &synth;
}

bool ControlScreen::GetTapmode()
{
	
	return tap_mode;
}

void ControlScreen::SetTapmode(bool state)
{
	tap_mode = state;
	graphics->setGraphicsModeNormal(state);
}

bool ControlScreen::isShowBall(void)
{
	return show_ball;
}

void ControlScreen::showBall(bool state)
{
	show_ball = state;
	graphics->showBall(state);
}

bool ControlScreen::isShowStrings(void)
{
	return show_strings;
}

void ControlScreen::showStrings(bool state)
{
	show_strings = state;
	graphics->showStrings(state);
}
