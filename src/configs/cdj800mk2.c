const CDJPanelButtonControl panelButtonControls[] = {
	{"PLAY", 0x0f, 0x01},
	{"CUE", 0x0e, 0x80},
	{"TRACKPREV", 0x10, 0x01},
	{"TRACKNEXT", 0x10, 0x02},
	{"SEARCHFORWARD", 0x10, 0x04},
	{"SEARCHBACKWARD", 0x10, 0x08},
	{"REVERSE", 0x0e, 0x20},
	{"LOOPIN", 0x0e, 0x10},
	{"LOOPOUT", 0x0e, 0x40},
	{"LOOPEXIT", 0x0f, 0x02},
	{"TEXTMODE", 0x12, 0x10},
	{"TIMEMODE", 0x12, 0x20},
	{"DELETE", 0x12, 0x08},
	{"MEMORY", 0x12, 0x04},
	{"CALLPREVCUE", 0x12, 0x02},
	{"CALLNEXTCUE", 0x12, 0x01},
	{"EJECT", 0x11, 0x08},
	{"JOGMODE", 0x11, 0x10},
	{"TEMPOWIDE", 0x11, 0x20},
	{"MASTERTEMPO", 0x11, 0x40},
	{"FOLDERPREV", 0x11, 0x01},
	{"FOLDERNEXT", 0x11, 0x02},
	{"TOUCH", 0x0c, 0x30},
};

const CDJJogControl jogControl = {0x08, 0x09, 0x0a, 0x0b};

const CDJPitchControl pitchControl = {0x04, 0x05};
const CDJReleaseControl releaseControl = {0x03};

// @todo add faders and jog here
