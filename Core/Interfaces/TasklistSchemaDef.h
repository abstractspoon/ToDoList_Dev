#if !defined(AFX_TDLSCHEMADEF_5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDLSCHEMADEF_5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_


static const LPCWSTR TDL_ROOT(L"TODOLIST");
static const LPCWSTR TDL_CHARSET(L"CHARSET");
static const LPCWSTR TDL_APPVER(L"APPVER");

static const LPCWSTR TDL_PROJECTNAME(L"PROJECTNAME");
static const LPCWSTR TDL_LASTSORTBY(L"LASTSORTBY");
static const LPCWSTR TDL_LASTSORTDIR(L"LASTSORTDIR");
static const LPCWSTR TDL_NEXTUNIQUEID(L"NEXTUNIQUEID");
static const LPCWSTR TDL_ARCHIVE(L"ARCHIVE");
static const LPCWSTR TDL_FILEFORMAT(L"FILEFORMAT");
static const LPCWSTR TDL_FILEVERSION(L"FILEVERSION");
static const LPCWSTR TDL_CHECKEDOUTTO(L"CHECKEDOUTTO");
static const LPCWSTR TDL_COMMENTSTYPE(L"CUSTOMCOMMENTSTYPE");		
static const LPCWSTR TDL_REPORTTITLE(L"REPORTTITLE");		
static const LPCWSTR TDL_REPORTDATE(L"REPORTDATE");		
static const LPCWSTR TDL_REPORTATTRIB(L"REPORTATTRIB");
static const LPCWSTR TDL_REPORTDATEOLE(L"REPORTDATEOLE");		
static const LPCWSTR TDL_FILENAME(L"FILENAME");	
static const LPCWSTR TDL_METADATA(L"METADATA");	
static const LPCWSTR TDL_EARLIESTDUEDATE(L"EARLIESTDUEDATE");		
static const LPCWSTR TDL_LASTMOD(L"LASTMOD");
static const LPCWSTR TDL_LASTMODSTRING(L"LASTMODSTRING");
static const LPCWSTR TDL_DISABLEPASSWORDPROMPTING(L"DISABLEPASSWORDPROMPTING");
static const LPCWSTR TDL_AVAILABLEATTRIB(L"TDL_AVAILABLEATTRIB");

static const LPCWSTR TDL_CUSTOMATTRIBDEFS(L"CUSTOMATTRIBDEF");	
static const LPCWSTR TDL_CUSTOMATTRIBID(L"ID");	
static const LPCWSTR TDL_CUSTOMATTRIBTYPE(L"TYPE");	
static const LPCWSTR TDL_CUSTOMATTRIBLABEL(L"LABEL");	
static const LPCWSTR TDL_CUSTOMATTRIBCOLTITLE(L"COLTITLE");	
static const LPCWSTR TDL_CUSTOMATTRIBCOLALIGN(L"COLALIGN");	
static const LPCWSTR TDL_CUSTOMATTRIBFEATURES(L"FEATURES");	
static const LPCWSTR TDL_CUSTOMATTRIBLISTDATA(L"LISTDATA");	
static const LPCWSTR TDL_CUSTOMATTRIBENABLED(L"ENABLED");	
static const LPCWSTR TDL_CUSTOMATTRIBCALCFIRSTOPERAND(L"FIRSTOPERAND");
static const LPCWSTR TDL_CUSTOMATTRIBCALCSECONDOPERANDATTRIB(L"SECONDOPERANDATTRIB");
static const LPCWSTR TDL_CUSTOMATTRIBCALCSECONDOPERANDVALUE(L"SECONDOPERANDVALUE");
static const LPCWSTR TDL_CUSTOMATTRIBCALCOPERATOR(L"OPERATOR");

static const LPCWSTR TDL_ATTRIBVIS(L"ATTRIBVIS");	
static const LPCWSTR TDL_ATTRIBVISCOL(L"COL");	
static const LPCWSTR TDL_ATTRIBVISEDIT(L"EDIT");	
static const LPCWSTR TDL_ATTRIBVISFILTER(L"FILTER");	
static const LPCWSTR TDL_ATTRIBVISSHOWFIELDS(L"SHOWEDITFILTER");	

// DEPRECATED
static const LPCWSTR TDL_CUSTOMATTRIBSORTABLE_DEP(L"SORTABLE"); // => 'FEATURES'
static const LPCWSTR TDL_CUSTOMATTRIBDEFS_DEP(L"CUSTOMATTRIB"); // => 'CUSTOMATTRIBDEF'	
	
// task attributes
static const LPCWSTR TDL_TASK(L"TASK");

static const LPCWSTR TDL_TASKID(L"ID");
static const LPCWSTR TDL_TASKREFID(L"REFID");
static const LPCWSTR TDL_TASKPOSITION(L"POS");
static const LPCWSTR TDL_TASKPARENTID(L"PARENTID");
static const LPCWSTR TDL_TASKTITLE(L"TITLE");
static const LPCWSTR TDL_TASKALLOCTO(L"PERSON");
static const LPCWSTR TDL_TASKALLOCBY(L"ALLOCATEDBY");
static const LPCWSTR TDL_TASKCATEGORY(L"CATEGORY");
static const LPCWSTR TDL_TASKTAG(L"TAG");
static const LPCWSTR TDL_TASKSTATUS(L"STATUS");
static const LPCWSTR TDL_TASKFILELINKPATH(L"FILEREFPATH");
static const LPCWSTR TDL_TASKCREATEDBY(L"CREATEDBY");
static const LPCWSTR TDL_TASKCOLOR(L"COLOR");
static const LPCWSTR TDL_TASKFLAG(L"FLAG");
static const LPCWSTR TDL_TASKLOCK(L"LOCK");
static const LPCWSTR TDL_TASKPRIORITY(L"PRIORITY");
static const LPCWSTR TDL_TASKPERCENTDONE(L"PERCENTDONE");
static const LPCWSTR TDL_TASKTIMEESTIMATE(L"TIMEESTIMATE");
static const LPCWSTR TDL_TASKTIMEESTUNITS(L"TIMEESTUNITS");
static const LPCWSTR TDL_TASKTIMESPENT(L"TIMESPENT");
static const LPCWSTR TDL_TASKTIMESPENTUNITS(L"TIMESPENTUNITS");
static const LPCWSTR TDL_TASKRECURRENCE(L"RECURRENCE");
static const LPCWSTR TDL_TASKRECURRENCEFREQ(L"RECURFREQ");
static const LPCWSTR TDL_TASKRECURRENCESPEC1(L"RECURSPECIFIC1");
static const LPCWSTR TDL_TASKRECURRENCESPEC2(L"RECURSPECIFIC2");
static const LPCWSTR TDL_TASKRECURRENCEFROM(L"RECURFROM");
static const LPCWSTR TDL_TASKRECURRENCEREUSE(L"RECURREUSE");
static const LPCWSTR TDL_TASKRECURRENCENUM(L"RECURNUM");
static const LPCWSTR TDL_TASKRECURRENCEREMAINING(L"RECURREMAINING");
static const LPCWSTR TDL_TASKRECURRENCEPRESERVECOMMENTS(L"RECURPRESERVECOMMENTS");
static const LPCWSTR TDL_TASKDEPENDENCY(L"DEPENDS");
static const LPCWSTR TDL_TASKDEPENDENCYLEADIN(L"DEPENDSLEADIN");
static const LPCWSTR TDL_TASKVERSION(L"VERSION");
static const LPCWSTR TDL_TASKICON(L"ICONINDEX");
static const LPCWSTR TDL_TASKMETADATA(L"METADATA");
static const LPCWSTR TDL_TASKRISK(L"RISK");		
static const LPCWSTR TDL_TASKCOST(L"COST");		
static const LPCWSTR TDL_TASKEXTERNALID(L"EXTERNALID");
static const LPCWSTR TDL_TASKLASTMODBY(L"LASTMODBY");

// custom attributes
static const LPCWSTR TDL_TASKCUSTOMATTRIBDATA(L"CUSTOMATTRIB");	
static const LPCWSTR TDL_TASKCUSTOMATTRIBID(L"ID");	
static const LPCWSTR TDL_TASKCUSTOMATTRIBVALUE(L"VALUE");	
static const LPCWSTR TDL_TASKCUSTOMATTRIBCALCVALUE(L"CALC");
static const LPCWSTR TDL_TASKCUSTOMATTRIBDISPLAYSTRING(L"VALUEDISPLAY");	
static const LPCWSTR TDL_TASKCUSTOMATTRIBCALCDISPLAYSTRING(L"CALCDISPLAY");	

// comments		
static const LPCWSTR TDL_TASKCOMMENTS(L"COMMENTS");
static const LPCWSTR TDL_TASKCUSTOMCOMMENTS(L"CUSTOMCOMMENTS");		
static const LPCWSTR TDL_TASKCOMMENTSTYPE(L"COMMENTSTYPE");

// dates
static const LPCWSTR TDL_TASKDONEDATE(L"DONEDATE");
static const LPCWSTR TDL_TASKDUEDATE(L"DUEDATE");
static const LPCWSTR TDL_TASKSTARTDATE(L"STARTDATE");
static const LPCWSTR TDL_TASKCREATIONDATE(L"CREATIONDATE");
static const LPCWSTR TDL_TASKLASTMOD(L"LASTMOD");
		
// 'calculated' values
static const LPCWSTR TDL_TASKTEXTCOLOR(L"TEXTCOLOR");		
static const LPCWSTR TDL_TASKBACKCOLOR(L"BACKCOLOR");		
static const LPCWSTR TDL_TASKTEXTWEBCOLOR(L"TEXTWEBCOLOR");		
static const LPCWSTR TDL_TASKBACKWEBCOLOR(L"BACKWEBCOLOR");		
static const LPCWSTR TDL_TASKPRIORITYCOLOR(L"PRIORITYCOLOR");		
static const LPCWSTR TDL_TASKPRIORITYWEBCOLOR(L"PRIORITYWEBCOLOR");		
static const LPCWSTR TDL_TASKCALCTIMEESTIMATE(L"CALCTIMEESTIMATE");		
static const LPCWSTR TDL_TASKCALCTIMEESTUNITS(L"CALCTIMEESTUNITS");		
static const LPCWSTR TDL_TASKCALCTIMESPENT(L"CALCTIMESPENT");		
static const LPCWSTR TDL_TASKCALCTIMESPENTUNITS(L"CALCTIMESPENTUNITS");		
static const LPCWSTR TDL_TASKCALCTIMEREMAINING(L"CALCTIMEREMAINING");		
static const LPCWSTR TDL_TASKCALCTIMEREMAININGUNITS(L"CALCTIMEREMAININGUNITS");
static const LPCWSTR TDL_TASKCALCSTARTDATE(L"CALCSTARTDATE");		
static const LPCWSTR TDL_TASKCALCSTARTDATESTRING(L"CALCSTARTDATESTRING");		
static const LPCWSTR TDL_TASKCALCDUEDATE(L"CALCDUEDATE");		
static const LPCWSTR TDL_TASKCALCDUEDATESTRING(L"CALCDUEDATESTRING");		
static const LPCWSTR TDL_TASKCALCCOMPLETION(L"CALCPERCENTDONE");		
static const LPCWSTR TDL_TASKHIGHESTPRIORITY(L"HIGHESTPRIORITY");		
static const LPCWSTR TDL_TASKHIGHESTRISK(L"HIGHESTRISK");		
static const LPCWSTR TDL_TASKCALCCOST(L"CALCCOST");		
static const LPCWSTR TDL_TASKPATH(L"PATH");		
static const LPCWSTR TDL_TASKISPARENT(L"ISPARENT");		
static const LPCWSTR TDL_TASKPOSSTRING(L"POSSTRING");
static const LPCWSTR TDL_TASKSUBTASKDONE(L"SUBTASKDONE");
static const LPCWSTR TDL_TASKGOODASDONE(L"GOODASDONE");
static const LPCWSTR TDL_TASKHTMLCOMMENTS(L"HTMLCOMMENTS");		
static const LPCWSTR TDL_TASKWEBCOLOR(L"WEBCOLOR");
static const LPCWSTR TDL_TASKDONEDATESTRING(L"DONEDATESTRING");		
static const LPCWSTR TDL_TASKDUEDATESTRING(L"DUEDATESTRING"); 	
static const LPCWSTR TDL_TASKSTARTDATESTRING(L"STARTDATESTRING");
static const LPCWSTR TDL_TASKCREATIONDATESTRING(L"CREATIONDATESTRING");		
static const LPCWSTR TDL_TASKLASTMODSTRING(L"LASTMODSTRING");
static const LPCWSTR TDL_TASKCALCFLAG(L"CALCFLAG");
static const LPCWSTR TDL_TASKCALCLOCK(L"CALCLOCK");

// DEPRECATED -------------------------------------------------------------
static const LPCWSTR TDL_TASKRECURRENCEREG_DEP(L"REGULARITY");     // => 'RECURFREQ'
static const LPCWSTR TDL_TASKRECURRENCESPEC1_DEP(L"REGPECIFIC1");  // => 'RECURSPECIFIC1'
static const LPCWSTR TDL_TASKRECURRENCESPEC2_DEP(L"REGPECIFIC2");  // => 'RECURSPECIFIC2'
static const LPCWSTR TDL_TASKRECURRENCEFROMDUE_DEP(L"REGFROMDUE"); // => 'RECURFROM'
static const LPCWSTR TDL_TASKNUMALLOCTO_DEP(L"NUMPERSON");         // no longer required
static const LPCWSTR TDL_TASKNUMCATEGORY_DEP(L"NUMCATEGORY");      // no longer required
static const LPCWSTR TDL_TASKNUMTAGS_DEP(L"NUMTAGS");              // no longer required
static const LPCWSTR TDL_TASKNUMDEPENDENCY_DEP(L"NUMDEPENDS");     // no longer required

#endif // AFX_TDLSCHEMADEF_5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_