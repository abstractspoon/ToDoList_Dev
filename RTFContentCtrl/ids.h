//
// Assorted defines and ids for the RulerRichEditCtrl
//

#ifndef _IDS_H_
#define _IDS_H_

// Some measures
#define TOOLBAR_HEIGHT		27
#define RULER_HEIGHT		26
#define TOP_HEIGHT			RULER_HEIGHT + TOOLBAR_HEIGHT
#define FONT_COMBO_WIDTH	128
#define SIZE_COMBO_WIDTH	48
#define COMBO_HEIGHT		256
#define COMBO_WIDTH			40
#define MIN_FONT_COMBO_WIDTH	60

#define FONT_NAME_POS		0
#define FONT_SIZE_POS		2
#define FONT_NAME_ID		1
#define FONT_SIZE_ID		2

// Measures for the ruler
#define MODE_INCH	0
#define MODE_METRIC	1

// Mouse handling
extern UINT urm_RULERACTION;
extern UINT urm_GETSCROLLPOS;
#define UP		0
#define DOWN	1
#define MOVE	2

#endif // _IDS_H_
