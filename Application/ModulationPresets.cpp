
#include "GUI.h"

#include "ModulationPresets.h"

static const MODPRESET modulation_preset_off=
{
	"OFF", 				// name
	0, 					// level
	0, 					// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	0, 					// rate
	0, 					// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_light_1=
{
	"chorus light 1", 	// name
	56, 				// level
	20, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	40, 				// rate
	30, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_light_2=
{
	"chorus light 2", 	// name
	56, 				// level
	40, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	30, 				// rate
	30, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_medium_1=
{
	"chorus medium 1", 	// name
	64, 				// level
	26, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	45, 				// rate
	40, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_medium_2=
{
	"chorus medium 2", 	// name
	64, 				// level
	60, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	35, 				// rate
	50, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_deep_1=
{
	"chorus deep 1", 	// name
	64, 				// level
	64, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	40, 				// rate
	70, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_deep_2=
{
	"chorus deep 2", 	// name
	64, 				// level
	80, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	45, 				// rate
	90, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_fast_1=
{
	"chorus fast 1", 	// name
	64, 				// level
	30, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	70, 				// rate
	10, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET chorus_fast_2=
{
	"chorus fast 2", 	// name
	64, 				// level
	60, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	60, 				// rate
	20, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET resonant_chorus=
{
	"resonant chorus", 	// name
	64, 				// level
	30, 				// delay
	80,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	40, 				// rate
	20, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET long_time_chorus=
{
	"long time chorus", // name
	64, 				// level
	100, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	20, 				// rate
	20, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET flange_light=
{
	"flange light", 	// name
	64, 				// level
	8, 					// delay
	32,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	40, 				// rate
	30, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET flange_medium=
{
	"flange medium", 	// name
	72, 				// level
	13, 				// delay
	72,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	40, 				// rate
	80, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET flange_slow_deep=
{
	"flange slow deep", // name
	72, 				// level
	2, 					// delay
	50,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	20, 				// rate
	90, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET flange_deep=
{
	"flange deep", 		// name
	72, 				// level
	6, 					// delay
	100,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	50, 				// rate
	100, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET phaser_light=
{
	"phaser light", 	// name
	64, 				// level
	0, 					// delay
	80,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	50, 				// rate
	50, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET phaser_medium=
{
	"phaser medium", 	// name
	64, 				// level
	0, 					// delay
	90,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	60, 				// rate
	80, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET phaser_slow_deep=
{
	"phaser slow deep", // name
	64, 				// level
	0, 					// delay
	100,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	20, 				// rate
	110, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET phaser_fast=
{
	"phaser fast", 		// name
	64, 				// level
	0, 					// delay
	40,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	120, 				// rate
	60, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET tremolo_slow=
{
	"tremolo slow", 	// name
	64, 				// level
	0, 					// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	20, 				// rate
	60, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET tremolo_medium_1=
{
	"tremolo medium 1", // name
	64, 				// level
	0, 					// delay
	0,  				// Feedback
	0, 					// hpf
	20, 				// HDAMP
	40, 				// rate
	70, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET tremolo_medium_2=
{
	"tremolo medium 2", 	// name
	64, 				// level
	0, 					// delay
	0,  				// Feedback
	0, 					// hpf
	40, 				// HDAMP
	60, 				// rate
	90, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET tremolo_fast=
{
	"tremolo fast", 	// name
	64, 				// level
	0, 					// delay
	0,  				// Feedback
	0, 					// hpf
	60, 				// HDAMP
	100, 				// rate
	110, 				// depth
	0, 					// shape
	0, 					// rotary
};

static const MODPRESET rotary_slow=
{
	"rotary slow", 		// name
	90, 				// level
	30, 				// delay
	0,  				// Feedback
	0, 					// hpf
	0, 					// HDAMP
	50, 				// rate
	20, 				// depth
	0, 					// shape
	0, 					// rotary
};

MODPRESET const *pModpresets[MODULATIONPRESETS_NUM]=
{
	&modulation_preset_off,
	&chorus_light_1,
	&chorus_light_2,
	&chorus_medium_1,
	&chorus_medium_2,
	&chorus_deep_1,
	&chorus_deep_2,
	&chorus_fast_1,
	&chorus_fast_2,
	&resonant_chorus,
	&long_time_chorus,
	&flange_light,
	&flange_medium,
	&flange_slow_deep,
	&flange_deep,
	&phaser_light,
	&phaser_medium,
	&phaser_slow_deep,
	&phaser_fast,
	&tremolo_slow,
	&tremolo_medium_1,
	&tremolo_medium_2,
	&tremolo_fast,
	&rotary_slow
};
