#include "stdafx.h"

#include "tdcstruct.h"
#include "filteredtodoctrl.h"
#include "preferencesdlg.h"
#include "TDCToDoCtrlPreferenceHelper.h"

void CTDCToDoCtrlPreferenceHelper::UpdateToDoCtrl(const CPreferencesDlg& prefs, CFilteredToDoCtrl& tdc, 
													BOOL bShowProjectName, BOOL bShowTreeListBar)
{
	// simplified version
	tdc.NotifyBeginPreferencesUpdate();

	UpdateToDoCtrlPrefs(prefs, bShowProjectName, bShowTreeListBar, tdc);

	// we're done
	tdc.NotifyEndPreferencesUpdate();
}

void CTDCToDoCtrlPreferenceHelper::UpdateToDoCtrl(const CPreferencesDlg& prefs, const TODOITEM& tdiDefault, 
													BOOL bShowProjectName, BOOL bShowTreeListBar,
													CFont& fontTree, CFont& fontComments, CFilteredToDoCtrl& tdc)
{
	tdc.NotifyBeginPreferencesUpdate();

	UpdateToDoCtrlPrefs(prefs, bShowProjectName, bShowTreeListBar, tdc);
	
	// App-controlled settings
	tdc.SetDefaultTaskAttributes(tdiDefault);

	// fonts
	if (!fontTree.GetSafeHandle() || !fontComments.GetSafeHandle())
	{
		CString sFaceName;
		int nFontSize;

		if (!fontTree.GetSafeHandle() && prefs.GetTreeFont(sFaceName, nFontSize))
			fontTree.Attach(GraphicsMisc::CreateFont(sFaceName, nFontSize));

		if (!fontComments.GetSafeHandle() && prefs.GetCommentsFont(sFaceName, nFontSize))
			fontComments.Attach(GraphicsMisc::CreateFont(sFaceName, nFontSize));
	}

	tdc.SetTreeFont(fontTree);
	tdc.SetCommentsFont(fontComments);

	// we're done
	tdc.NotifyEndPreferencesUpdate();
}

void CTDCToDoCtrlPreferenceHelper::UpdateToDoCtrlPrefs(const CPreferencesDlg& prefs, BOOL bShowProjectName, BOOL bShowTreeListBar, CFilteredToDoCtrl& tdc)
{
	// Assumes CtoDoCtrl::NotifyBeginPreferencesUpdate has already been called
	CTDCStylesMap styles;

	styles[TDCS_ALLOWCOMMENTSSTACKING] = prefs.GetStackEditFieldsAndComments();
	styles[TDCS_ALLOWPARENTTIMETRACKING] = prefs.GetAllowParentTimeTracking();
	styles[TDCS_ALLOWTREEITEMCHECKBOX] = prefs.GetAllowCheckboxAgainstTreeItem();
	styles[TDCS_ALWAYSHIDELISTPARENTS] = prefs.GetAlwaysHideListParents();
	styles[TDCS_AUTOADJUSTDEPENDENCYDATES] = prefs.GetAutoAdjustDependentsDates();
	styles[TDCS_AUTOCALCPERCENTDONE] = prefs.GetAutoCalcPercentDone();
	styles[TDCS_SYNCTIMEESTIMATESANDDATES] = prefs.GetSyncTimeEstimatesAndDates();
	styles[TDCS_AUTOREPOSCTRLS] = prefs.GetAutoReposCtrls();
	styles[TDCS_AVERAGEPERCENTSUBCOMPLETION] = prefs.GetAveragePercentSubCompletion();
	styles[TDCS_CALCREMAININGTIMEBYDUEDATE] = (prefs.GetTimeRemainingCalculation() == PTCP_REMAININGTTIMEISDUEDATE);
	styles[TDCS_CALCREMAININGTIMEBYPERCENT] = (prefs.GetTimeRemainingCalculation() == PTCP_REMAININGTTIMEISPERCENTAGE);
	styles[TDCS_CALCREMAININGTIMEBYSPENT] = (prefs.GetTimeRemainingCalculation() == PTCP_REMAININGTTIMEISSPENT);
	styles[TDCS_CHECKOUTONLOAD] = prefs.GetAutoCheckOut();
	styles[TDCS_COLORTEXTBYATTRIBUTE] = (prefs.GetTextColorOption() == COLOROPT_ATTRIB);
	styles[TDCS_COLORTEXTBYNONE] = (prefs.GetTextColorOption() == COLOROPT_NONE);
	styles[TDCS_COLORTEXTBYPRIORITY] = (prefs.GetTextColorOption() == COLOROPT_PRIORITY);
	styles[TDCS_COLUMNHEADERSORTING] = prefs.GetEnableColumnHeaderSorting();
	styles[TDCS_COMMENTSUSETREEFONT] = prefs.GetCommentsUseTreeFont();
	styles[TDCS_CONFIRMDELETE] = prefs.GetConfirmDelete();
	styles[TDCS_DISPLAYHMSTIMEFORMAT] = prefs.GetUseHMSTimeFormat();
	styles[TDCS_DISPLAYLOGCONFIRM] = prefs.GetDisplayLogConfirm();
	styles[TDCS_DONEHAVELOWESTPRIORITY] = prefs.GetDoneTasksHaveLowestPriority();
	styles[TDCS_DONEHAVELOWESTRISK] = prefs.GetDoneTasksHaveLowestRisk();
	styles[TDCS_DUEHAVEHIGHESTPRIORITY] = prefs.GetDueTasksHaveHighestPriority();
	styles[TDCS_FOCUSTREEONENTER] = prefs.GetFocusTreeOnEnter();
	styles[TDCS_HIDEPANESPLITBAR] = prefs.GetHidePaneSplitBar();
	styles[TDCS_HIDEPERCENTFORDONETASKS] = prefs.GetHidePercentForDoneTasks();
	styles[TDCS_HIDEPRIORITYNUMBER] = prefs.GetHidePriorityNumber();
	styles[TDCS_HIDESTARTDUEFORDONETASKS] = prefs.GetHideStartDueForDoneTasks();
	styles[TDCS_HIDEZEROPERCENTDONE] = prefs.GetHideZeroPercentDone();
	styles[TDCS_HIDEZEROTIMECOST] = prefs.GetHideZeroTimeCost();
	styles[TDCS_INCLUDEDONEINAVERAGECALC] = prefs.GetIncludeDoneInAverageCalc();
	styles[TDCS_INCLUDEDONEINPRIORITYCALC] = prefs.GetIncludeDoneInPriorityRiskCalc();
	styles[TDCS_INCLUDEDONEINRISKCALC] = prefs.GetIncludeDoneInPriorityRiskCalc();
	styles[TDCS_INCLUDEUSERINCHECKOUT] = prefs.GetIncludeUserNameInCheckout();
	styles[TDCS_LOGTASKTIMESEPARATELY] = prefs.GetLogTaskTimeSeparately();
	styles[TDCS_LOGTIMETRACKING] = prefs.GetLogTimeTracking();
	styles[TDCS_NODUEDATEISDUETODAYORSTART] = prefs.GetNoDueDateIsDueTodayOrStart();
	styles[TDCS_REFILTERONMODIFY] = prefs.GetReFilterOnModify();
	styles[TDCS_RESORTONMODIFY] = prefs.GetReSortOnModify();
	styles[TDCS_RESTOREFILTERS] = prefs.GetRestoreTasklistFilters();
	styles[TDCS_RIGHTSIDECOLUMNS] = prefs.GetShowColumnsOnRight();
	styles[TDCS_ROUNDTIMEFRACTIONS] = prefs.GetRoundTimeFractions();
	styles[TDCS_SHAREDCOMMENTSHEIGHT] = prefs.GetSharedCommentsHeight();
	styles[TDCS_SHOWCOMMENTSALWAYS] = prefs.GetShowCommentsAlways();
	styles[TDCS_SHOWCOMMENTSINLIST] = prefs.GetShowComments();
	styles[TDCS_SHOWDATESINISO] = prefs.GetDisplayDatesInISO();
	styles[TDCS_SHOWDEFAULTTASKICONS] = prefs.GetShowDefaultTaskIcons();
	styles[TDCS_SHOWFIRSTCOMMENTLINEINLIST] = prefs.GetDisplayFirstCommentLine();
	styles[TDCS_SHOWINFOTIPS] = prefs.GetShowInfoTips();
	styles[TDCS_SHOWNONFILEREFSASTEXT] = prefs.GetShowNonFilesAsText();
	styles[TDCS_SHOWPARENTSASFOLDERS] = prefs.GetShowParentsAsFolders();
	styles[TDCS_SHOWPATHINHEADER] = prefs.GetShowPathInHeader();
	styles[TDCS_SHOWPERCENTASPROGRESSBAR] = prefs.GetShowPercentAsProgressbar();
	styles[TDCS_SHOWPROJECTNAME] = bShowProjectName;
	styles[TDCS_SHOWTREELISTBAR] = bShowTreeListBar;
	styles[TDCS_SHOWWEEKDAYINDATES] = prefs.GetShowWeekdayInDates();
	styles[TDCS_SORTDONETASKSATBOTTOM] = prefs.GetSortDoneTasksAtBottom();
	styles[TDCS_SORTVISIBLETASKSONLY] = FALSE;//prefsNew.GetSortVisibleOnly();
	styles[TDCS_STRIKETHOUGHDONETASKS] = prefs.GetStrikethroughDone();
	styles[TDCS_TASKCOLORISBACKGROUND] = prefs.GetColorTaskBackground();
	styles[TDCS_TRACKSELECTEDTASKONLY] = !prefs.GetTrackNonSelectedTasks();
	styles[TDCS_TREATSUBCOMPLETEDASDONE] = prefs.GetTreatSubCompletedAsDone();
	styles[TDCS_USEEARLIESTDUEDATE] = (prefs.GetDueDateCalculation() == PTCP_EARLIESTDUEDATE);
	styles[TDCS_USEEARLIESTSTARTDATE] = (prefs.GetStartDateCalculation() == PTCP_EARLIESTSTARTDATE);
	styles[TDCS_USEHIGHESTPRIORITY] = prefs.GetUseHighestPriority();
	styles[TDCS_USEHIGHESTRISK] = prefs.GetUseHighestRisk();
	styles[TDCS_USELATESTDUEDATE] = (prefs.GetDueDateCalculation() == PTCP_LATESTDUEDATE);
	styles[TDCS_USELATESTSTARTDATE] = (prefs.GetStartDateCalculation() == PTCP_LATESTSTARTDATE);
	styles[TDCS_USEPERCENTDONEINTIMEEST] = prefs.GetUsePercentDoneInTimeEst();
	styles[TDCS_USES3RDPARTYSOURCECONTROL] = prefs.GetUsing3rdPartySourceControl();
	styles[TDCS_WARNADDDELETEARCHIVE] = prefs.GetWarnAddDeleteArchive();
	styles[TDCS_WEIGHTPERCENTCALCBYNUMSUB] = prefs.GetWeightPercentCompletionByNumSubtasks();
	styles[TDCS_SHOWREMINDERSASDATEANDTIME] = prefs.GetShowRemindersAsDateAndTime();
	styles[TDCS_SYNCCOMPLETIONTOSTATUS] = prefs.GetSyncCompletionToStatus();
	
	tdc.SetStyles(styles);

	// layout
	tdc.SetLayoutPositions((TDC_UILOCATION)prefs.GetControlsPos(), 
							(TDC_UILOCATION)prefs.GetCommentsPos(), 
							TRUE);

	// info tips
	tdc.SetMaxInfotipCommentsLength(prefs.GetMaxInfoTipCommentsLength());

	// default string lists
	TDCAUTOLISTDATA tld;
	prefs.GetDefaultListItems(tld);

	tdc.SetDefaultAutoListData(tld);

	// set list content readonly
	tdc.SetAutoListContentReadOnly(TDCA_CATEGORY,	prefs.GetDefaultListIsReadonly(TDCA_CATEGORY));
	tdc.SetAutoListContentReadOnly(TDCA_ALLOCTO,	prefs.GetDefaultListIsReadonly(TDCA_ALLOCTO));
	tdc.SetAutoListContentReadOnly(TDCA_ALLOCBY,	prefs.GetDefaultListIsReadonly(TDCA_ALLOCBY));
	tdc.SetAutoListContentReadOnly(TDCA_STATUS,		prefs.GetDefaultListIsReadonly(TDCA_STATUS));
	tdc.SetAutoListContentReadOnly(TDCA_VERSION,	prefs.GetDefaultListIsReadonly(TDCA_VERSION));
	tdc.SetAutoListContentReadOnly(TDCA_TAGS,		prefs.GetDefaultListIsReadonly(TDCA_TAGS));

	// colours
	tdc.SetGridlineColor(prefs.GetGridlineColor());
	tdc.SetCompletedTaskColor(prefs.GetDoneTaskColor());
	tdc.SetAlternateLineColor(prefs.GetAlternateLineColor());
	tdc.SetFlaggedTaskColor(prefs.GetFlaggedTaskColor());
	tdc.SetReferenceTaskColor(prefs.GetReferenceTaskColor());

	CDWordArray aPriorityColors;
	prefs.GetPriorityColors(aPriorityColors);
	tdc.SetPriorityColors(aPriorityColors);

	COLORREF color, crToday;
	prefs.GetStartedTaskColors(color, crToday);
	tdc.SetStartedTaskColors(color, crToday);

	prefs.GetDueTaskColors(color, crToday);
	tdc.SetDueTaskColors(color, crToday);

	CTDCColorMap colors;
	TDC_ATTRIBUTE nAttrib = prefs.GetAttributeColors(colors);

	tdc.SetAttributeColors(nAttrib, colors);

	// misc
	tdc.SetSubtaskDragDropPos(prefs.GetNewSubtaskPos() == PUIP_TOP);
	tdc.SetPercentDoneIncrement(prefs.GetPercentDoneIncrement());

	CString sStatus;
	prefs.GetCompletionStatus(sStatus);
	tdc.SetCompletionStatus(sStatus);
}
