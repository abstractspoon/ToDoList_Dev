#if !defined(AFX_CALENDARENUM_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_CALENDARENUM_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

/////////////////////////////////////////////////////////////////////////////

enum // options
{
	TCCO_CALCMISSINGSTARTASCREATION				= 0x0001,
	TCCO_CALCMISSINGSTARTASDUE					= 0x0002,
	TCCO_CALCMISSINGSTARTASEARLIESTDUEANDTODAY	= 0x0004,
	TCCO_CALCMISSINGDUEASSTART					= 0x0008,
	TCCO_CALCMISSINGDUEASLATESTSTARTANDTODAY	= 0x0010,
	TCCO_DISPLAYCONTINUOUS						= 0x0020,
	TCCO_DISPLAYSTART							= 0x0040,
	TCCO_DISPLAYDUE								= 0x0080,
	TCCO_DISPLAYCALCSTART						= 0x0100,
	TCCO_DISPLAYCALCDUE							= 0x0200,
	TCCO_ADJUSTTASKHEIGHTS						= 0x0400,
	TCCO_DISPLAYDONE							= 0x0800,
	TCCO_TASKTEXTCOLORISBKGND					= 0x1000,
	TCCO_STRIKETHRUDONETASKS					= 0x2000,
};

/////////////////////////////////////////////////////////////////////////////

enum TCC_HITTEST
{
	TCCHT_NOWHERE = -1,
	TCCHT_BEGIN,
	TCCHT_MIDDLE,
	TCCHT_END,
};

/////////////////////////////////////////////////////////////////////////////

enum TCC_SNAPMODE
{
	TCCSM_NONE = -1,
	TCCSM_NEARESTDAY,
	TCCSM_NEARESTHALFDAY,
	TCCSM_NEARESTHOUR,
	TCCSM_FREE,
};

/////////////////////////////////////////////////////////////////////////////

const double	ONE_HOUR		= (1.0 / 24.0);
const int		DEF_TASK_HEIGHT = 18;
const int		MIN_TASK_HEIGHT = (DEF_TASK_HEIGHT - 6);
const int		DAY_IN_SECONDS	= (24 * 60 * 60);
const int		WEEK_IN_SECONDS	= (DAY_IN_SECONDS * 7);

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CALENDARENUM_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
