<?xml version="1.0" encoding="UTF-8"?>
<!-- Based on ToDoListTableStylesheet_v1.0 as Created by xabatcha@seznam.cz, 2006-10-17 -->
<!-- Modified by JAZ, April 12 2010, 
then again AprilMay 2012 -->
<!--  V3 Dec 2012
V4 June 2103 (added coloured tasks, HTML comments, ability to handle CATEGORY, TAG, PERSON and COMMENTS as task elements rather than task attributes)-->
<!-- Transform ToDoList to html, using table layout -->

<!--### USER ADJUSTABLE through GLOBAL PARAMETERS below these comments ###
									Best run from Task tree view-->

<!-- General-->
<!-- This stylesheet includes: Priority, Taskname, Start date, Due date, Progress, Assigned to as a default
            Haven't included Category or Status as a column.  Code is there though for Category - Section 45.
                If you want it, you could swap one of the other attributes.  Column definitions are in section 20.  Attrubute calls - section 32.
            Have included the possibility to add flagged status, category, status & comments next to the task title.
               This is set at the top of the code using global parameters; showcat, showstatus, showflagged, showcomments
               The length of the comment can be limited to a defined number of characters
             Have included the possibility to add more than just the first person assigned to a task (up to 3) - or to limit it
               This is set at the top of the code using global parameter; numpeople
             Have included up to 5 categories
 
                        ### These options are set at the top of the code using global parameters; showcat, showstatus etc  ###
       Selecting particular tasks to report on can be handled by selection in the printing dialog or by pre-filtering-->
             
 <!-- Finished and unfinished tasks are able to be shown. 
            Select from the print preview dialog 
            Finished tasks are greyed and struck out
            If a task is complete, the due date is replaced by the 'done date'-->
<!-- Empty attributes have a '-' added-->
<!-- Added a bullet to indented tasks for clarity-->
<!-- Where Calcpercentdone = 100% but task has not been checked off (percentdone < 100%), progess is given as 100%, but the task not deemed to be finished and hence not 'struck through' -->

<!-- Formatting:--> 
<!--    Table borders are mostly switched off.  
            To turn on borders around the table, uncomment table.finished.  table.finished does the outside of the table.
            Rows are underlined, adjust td.tborder to change
            To adjust or turn off the borders, goto td.tborder, td.headborder, td.underline and comment out as needed.  -->
<!--    Text size and colour is changed depending on value and task status
            A task that is done is greyed and crossed out
			Tasks may be coloured.  Also can choose whether only the task title is coloured, or all attributes
            Percentage done, 0% is greyed and smaller font to highlight the tasks that are started
            Priority below a value is in a smaller font and greyed
               The cut off value is set at the top of the code using a global parameter; prioritycutoff
            Lead / root tasks are bolded by default, but this can be switched off (useful if printing from Listview only) - see tr.lead
            Lead / root tasks have extra spacing before it for visual clarity
               To turn this off, set the global parameter at the top of the code, leadspacing
			Rich text comments are enabled, however this can be switched off, so richtext comments appear as simple text (can save space)-->
<!--    Date can be reported in the standard way (dd/mm/yy), or in the form dd mmm yy  
                 If dd mmm yy doesn't work properly (is a bit dependent on regional settings of the operating system):
                 Firstly check section 70 if regional settings are mm/dd/yy (can modify the code to deal with this)
                 Or set the global parameter to 1 at the top of the code;  showsimpledates -->

<!--  Intended improvements not achieved
        Proper tabbing of task names in treeview (indent when task name wraps)
          - note original doesn't do this properly either.
          - comments are in a sub-box with a dashed border - fixed shift (float) distance to the right
	Ideally dont have listview automatically bolded, but still have top task bolded in treeview (they are bolded as in that view, tasks have no descendents)
	Ideally repeat the heading on each page.-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
	<xsl:strip-space elements="*"/>
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" encoding="utf-8"/>
	
	<!-- ################## Selectable values to customise the report #####################
	                                         Generally, to apply setting, enter 1.  To turn off setting, enter 0.-->
	                                         
	<!-- To put spacing (extra line for visual separation) in front of a lead task, enter 1.  To remove this, enter 0. 
	Note, this can be useul when reporting from List View-->  
	<xsl:param name="leadspacing" select="1" />
	<!-- To bold and make bigger all root tasks, enter 1 (default).  To remove this, enter 0.-->  
	<xsl:param name="boldroottask" select="1" />
	<!-- To colour the task title only, or colour the task line, or no colour at all, change the 0 to a 1. -->  
	<xsl:param name="showtaskcolouring" select="1" />
	<xsl:param name="colourtasktitleonly" select="0" />
	<!-- To show any of these with the task title, change the 0 to a 1. -->  
	<xsl:param name="showcat" select="1" />
	<xsl:param name="showstatus" select="0" />
	<xsl:param name="showflagged" select="1" />
	<xsl:param name="showcomments" select="0" />
	<!-- To show formatted (Richtext) comments (1)-->
	<xsl:param name="showformattedcomments" select="1" />	
	<!--To limit the length of the comment, enter the number of characters to show.  
	    Note, it is about 70 characters per line.  However, if there are lots of line breaks, it will still be tall.-->
	<xsl:param name="commentlength" select="0" />
	<!-- To show dd mmm yy dates, select 0.  To show simple dd/mm/yy dates, enter 1.  --> 	
	<xsl:param name="showsimpledates" select="1" />
	<!-- Priorities of tasks below the value given are presented greyed and in a smaller font. -->  
	<xsl:param name="prioritycutoff" select="3" />
	<!-- There may be more than one person assigned to a task.  This allows you to set the max number of people (up to 3).
	Note Values of 0 or 1, give one person.  Values of 2 or 3 give 2 or 3 people respectively.  Values above 3 give no more than 3 people-->  
	<xsl:param name="numpeople" select="3" />
	
	<xsl:template match="/">
		<xsl:element name="html">
			<xsl:apply-templates select="TODOLIST"/>
		</xsl:element>
	</xsl:template>
	<!-- 00 - body of report -->
	<xsl:template match="TODOLIST">
		<xsl:element name="head">
			<xsl:element name="title">
				<xsl:value-of select="@PROJECTNAME"/>
			</xsl:element>
			<xsl:element name="style">
				<xsl:attribute name="type">text/css</xsl:attribute>
				<xsl:text>
					<!--  Note:  Colors: dark grey 808080, mid grey A8A8A8, light grey C0C0C0, vlight CDCDCD-->
					table.finished{
					<!--	border-bottom: 1px solid Black;
						border-top: 2px solid Black;
						border-right: 1px solid Black; 	   
						border-left: 2px solid Black;  -->
						width: 100%;
						vertical-align: center;
					<!--	table-omit-header-at-break: false;-->
					}
					tr.titlef{
						font-weight: bold;
						font-size: 18px;
					}
					tr.header{
						vertical-align: bottom;
						background-color: #33ccff;
						font-weight: bold;
						font-style: italic;
						font-size: 10px;
						color: #808080;
					}
					td.headright{
						text-align:right;
					}
					td.headleft{
						text-align:left;
					}
					td.headcenter{
						text-align:center;
					}
					tr.lead{
						vertical-align: center;
						font-weight: bold;
					}
					td.taskname{
						text-align:left;
						vertical-align: center;
						font-size: 11px;
						display: block;
					}
					td.tasknamedone{
						color: #D3D3D3;
						text-decoration: line-through;
					}
					td.taskinfo{
						text-align:center;
						vertical-align: top;
					}
					td.tinormal{
						font-size: 10px;
					}
					td.timinor{
						font-size: 8px;
						color: #C0C0C0;
					}
					td.tidone{
						font-size: 9px;
						color: #C0C0C0;
						text-decoration: line-through;
					}
					span.tinfo{
						font-size: 9px;
						color: #808080
					}
					span.tinfodone{
						font-size: 9px;
						color: #D3D3D3;
						text-decoration: line-through;
					}
					span.tinfobold{
						font-weight: bold;
					}
					span.tab{
						padding-left:20px;
						margin: 0 0 0 0;
					}
					<!--td.padding
					{
						padding-left:80px;
						margin: 0 0 0 0;
					}-->
					.noborder{
						margin: 0 5 0 0;
					}
					<!-- Note:  padding: 10px 20px 15px 5px  -  top, right, bottom, left respectively
						    (Easy reminder - Clockwise starting with top).  Same for margin.-->
					td.headborder{
						border-bottom: 2px solid #CDCDCD;
					<!--	border-right: 1px solid #CDCDCD;
						border-left: 1px solid #CDCDCD; --> 
					}
					td.tborder{
						border-bottom: 1px solid #CDCDCD;
					<!--	border-right: 1px solid #CDCDCD;  
						border-left: 1px solid #CDCDCD;  -->
					}
					div.commentbox{
						width: 80%;
						float: right;
						margin: 10 1 1 0;
						padding:5px;
						display: block;
						border: 2px dotted #CDCDCD;
					}
					div.commentfont{
						font-style: italic;
						font-weight: normal;
						font-size: 9px;
						color: #C0C0C0;
					}

					body {
						font-family: Arial;
						vertical-align: center;
					}
				</xsl:text>
			</xsl:element>
		</xsl:element>
		<xsl:element name="body">
<!--			<xsl:element name="h3">Project:   <xsl:value-of select="@PROJECTNAME"/>  -  <xsl:value-of select="@REPORTDATE"/>
			</xsl:element> -->
			<xsl:element name="table">
				<xsl:attribute name="class">finished</xsl:attribute>
				<xsl:attribute name="cellspacing">0</xsl:attribute>
				<xsl:attribute name="cellpadding">1</xsl:attribute>
                                <!-- Project name and printing date -->				
				<xsl:element name="tr">
					<xsl:attribute name="class">titlef</xsl:attribute>
					<xsl:element name="td">
						<xsl:attribute name="colspan">4</xsl:attribute>
						<xsl:attribute name="class">headleft underline</xsl:attribute>
						<xsl:value-of select="@PROJECTNAME"/>
					</xsl:element>
					<xsl:element name="td">
						<xsl:attribute name="colspan">2</xsl:attribute>
						<xsl:attribute name="class">headright underline</xsl:attribute>
						<xsl:value-of select="@REPORTDATE"/>
					</xsl:element>
				</xsl:element>
                                <!-- Main body of the table - the tasks -->				
				<xsl:apply-templates select="TASK"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	
	<!-- 10 - table of tasks context -->
	<xsl:template match="TASK">
		<xsl:choose>
			<xsl:when test="parent::TODOLIST">
				<!-- 11 - create header only for first row-->
				<xsl:if test="position()=1">
					<xsl:call-template name="get_Header"/>
				</xsl:if>
				<!-- 12 - lead task, child of todolist element -->
				<xsl:if test="$leadspacing=1">
					<xsl:call-template name="insert_line"/>
				</xsl:if>
				<xsl:call-template name="get_LeadTask"/>
				<xsl:apply-templates select="TASK"/>
			</xsl:when>
			<xsl:otherwise>
				<!-- 13 - sub tasks / finished and unfinished-->
				<xsl:call-template name="get_Task"/>
				<xsl:apply-templates select="TASK"/>
 			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	<!-- 20 - header of table - titles of columns -->
	<xsl:template name="get_Header">
		<xsl:element name="tr">
			<xsl:attribute name="class">header</xsl:attribute>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">5%</xsl:attribute>
				<BR/> <BR/>
				<xsl:text>!!</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headleft headborder</xsl:attribute>
				<xsl:attribute name="width">55%</xsl:attribute>
				<xsl:text>Task</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>%</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>Start</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>Due</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>Assigned To</xsl:text>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	
	<!-- 30 - lead task - child of TODOLIST tag -->
	<xsl:template name="get_LeadTask">
		<xsl:element name="tr">
			<xsl:if test="($boldroottask=1)">
				<xsl:attribute name="class">lead</xsl:attribute>
			</xsl:if>  
			<xsl:call-template name="get_Task_Details"/>
		</xsl:element>
	</xsl:template>
	
	<!-- 31- single task -->
	<xsl:template name="get_Task">
		<xsl:element name="tr">
			<xsl:call-template name="get_Task_Details"/>
		</xsl:element>
	</xsl:template>
	
	<!--32 - detail of single task -->	
	<xsl:template name="get_Task_Details">
		<xsl:call-template name="get_Priority"/>
		<xsl:call-template name="get_Task_title"/>
		<xsl:call-template name="get_Progress"/>
<!--		<xsl:call-template name="get_cat"/> -->
		<xsl:call-template name="get_start"/>
		<xsl:call-template name="get_due"/>
		<xsl:call-template name="get_to"/>
	</xsl:template>
	
	<!-- 33 - blank row to insert before a lead task - as spacing -->	
	<xsl:template name="insert_line">
		<xsl:element name="tr">
			<xsl:attribute name="class">header</xsl:attribute>
			<xsl:element name="td">
				<BR/>
			</xsl:element>
		</xsl:element>
	</xsl:template>	

	<!-- 40 - @PERCENTDONE as progress -->	
		<!--  Note:  Shows % complete for all tasks, including ancestor tasks -->
		<!-- Note:  The logic assumes either there is a percent done, or it is calc'd - not both -->
	<xsl:template name="get_Progress">
		<xsl:element name="td">
			<xsl:choose>
			    <xsl:when test="(@PERCENTDONE)">
			        <xsl:choose>
			            <xsl:when test="(@PERCENTDONE=100)">
							<xsl:attribute name="class">taskinfo tidone tborder</xsl:attribute>
							<xsl:value-of select="@PERCENTDONE"/>			            
			            </xsl:when>
			            <xsl:when test="(@PERCENTDONE=0)">
							<xsl:choose>
								<xsl:when test="(@CALCPERCENTDONE)">
									<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
									<xsl:value-of select="@CALCPERCENTDONE"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
									<xsl:value-of select="@PERCENTDONE"/>
								</xsl:otherwise>
							</xsl:choose>			            			            
			            </xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
							<xsl:value-of select="@PERCENTDONE"/>			            
						</xsl:otherwise>
   	                </xsl:choose>
					<xsl:text>%</xsl:text>
   	            </xsl:when>
   	            <xsl:otherwise>
			        <xsl:choose>
						<xsl:when test="(@CALCPERCENTDONE)">
							<xsl:choose>
								<xsl:when test="(@CALCPERCENTDONE=0)">
									<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>					    
								</xsl:when>
								<xsl:otherwise>
									<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:value-of select="@CALCPERCENTDONE"/>
							<xsl:text>%</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
							<xsl:text>-</xsl:text>
						</xsl:otherwise>			        
			        </xsl:choose>
   	            </xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	
	<!-- 41 - @STARTDATESTRING if exists -->
		<!-- Note:  If the dates aren't represented properly in dd mmm yy format, set showsimpledates to 1 at the top of the code-->	
	<xsl:template name="get_start">
		<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="(@STARTDATESTRING)">
					<xsl:if test="not(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
						<xsl:if test="$showtaskcolouring=1 and $colourtasktitleonly=0">
							<xsl:attribute name="style">color:<xsl:value-of select="@TEXTWEBCOLOR"/></xsl:attribute>
						</xsl:if>
						<xsl:if test="$showtaskcolouring=0">
							<xsl:attribute name="style">color:black</xsl:attribute>	
						</xsl:if>
					</xsl:if>
					<xsl:if test="(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tidone tborder</xsl:attribute>
					</xsl:if>
					<xsl:if test="($showsimpledates=1)">
						 <xsl:value-of select="substring(@STARTDATESTRING,1,10)"/>
					</xsl:if>
					<xsl:if test="$showsimpledates=0">
						<xsl:call-template name="FormatDate">
						    <xsl:with-param name="thisdate" select="@STARTDATESTRING"/>
						</xsl:call-template>  
					</xsl:if>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					<xsl:text>-</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	
	<!-- 42 - @DUEDATESTRING if exists -->
		<!-- Note:  If the dates aren't represented properly in dd mmm yy format, set showsimpledates to 1 at the top of the code-->	
	<xsl:template name="get_due">
	<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="(@DUEDATESTRING)">
					<xsl:if test="not(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
						<xsl:if test="$showtaskcolouring=1 and $colourtasktitleonly=0">
							<xsl:attribute name="style">color:<xsl:value-of select="@TEXTWEBCOLOR"/></xsl:attribute>
						</xsl:if>
						<xsl:if test="$showtaskcolouring=0">
							<xsl:attribute name="style">color:black</xsl:attribute>	
						</xsl:if>
						<xsl:if test="($showsimpledates=1)">
							 <xsl:value-of select="@DUEDATESTRING"/>
						</xsl:if>
						<xsl:if test="$showsimpledates=0">
							<xsl:call-template name="FormatDate">
							    <xsl:with-param name="thisdate" select="@DUEDATESTRING"/>
							</xsl:call-template>  
						</xsl:if>
					</xsl:if>
					<xsl:if test="(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tidone tborder</xsl:attribute>
						<xsl:if test="($showsimpledates=1)">
							 <xsl:value-of select="substring(@DONEDATESTRING,1,10)"/>
						</xsl:if>
						<xsl:if test="$showsimpledates=0">
							<xsl:call-template name="FormatDate">
							    <xsl:with-param name="thisdate" select="@DONEDATESTRING"/>
							</xsl:call-template>  
						</xsl:if>
					</xsl:if>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					<xsl:text>-</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	
	<!-- 43 - @PRIORITY as colored priority -->
		<!--	Note:  This code puts in priority number, but greys and shrinks the font of any priority less than a desired value  
			To change the cut-off priority, change the value of prioritycutoff at the top of the code-->	
               <xsl:template name="get_Priority">
		<xsl:element name="td">
			<xsl:choose>
                <xsl:when test="(@PRIORITY)">
                    <xsl:choose>
						<xsl:when test="(@PRIORITY&gt;$prioritycutoff)">
							<xsl:if test="not(@DONEDATESTRING)">
								<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
								<xsl:if test="$showtaskcolouring=1 and $colourtasktitleonly=0">
									<xsl:attribute name="style">color:<xsl:value-of select="@TEXTWEBCOLOR"/></xsl:attribute>
								</xsl:if>
								<xsl:if test="$showtaskcolouring=0">
									<xsl:attribute name="style">color:black</xsl:attribute>
								</xsl:if>								
							</xsl:if>
							<xsl:if test="(@DONEDATESTRING)">
								<xsl:attribute name="class">taskinfo tidone tborder</xsl:attribute>			
							</xsl:if>
							<xsl:value-of select="@PRIORITY"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>	
		<!--					<xsl:if test="$showtaskcolouring=1 and $colourtasktitleonly=0">
								<xsl:attribute name="style">color:<xsl:value-of select="@TEXTWEBCOLOR"/></xsl:attribute>
							</xsl:if>
							<xsl:if test="$showtaskcolouring=0">
								<xsl:attribute name="style">color:black</xsl:attribute>
							</xsl:if>			-->				
							<xsl:value-of select="@PRIORITY"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>			
					<xsl:text>-</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	
	<!-- 44 - PERSON as person who should do the work -->	
	<xsl:template name="get_to">
		<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="PERSON">
					<xsl:if test="not(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
						<xsl:if test="$showtaskcolouring=1 and $colourtasktitleonly=0">
							<xsl:attribute name="style">color:<xsl:value-of select="@TEXTWEBCOLOR"/></xsl:attribute>
						</xsl:if>
						<xsl:if test="$showtaskcolouring=0">
							<xsl:attribute name="style">color:black</xsl:attribute>	
						</xsl:if>
					</xsl:if>
					<xsl:if test="(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tidone tborder</xsl:attribute>
					</xsl:if>
					
					<xsl:value-of select="PERSON"/>
		
					<xsl:if test="$numpeople=2">
						<xsl:if test="PERSON[2]">; <xsl:value-of select="PERSON[2]" /></xsl:if>
					</xsl:if>
					<xsl:if test="$numpeople&gt;=3">
						<xsl:if test="PERSON[2]">; <xsl:value-of select="PERSON[2]" /></xsl:if>
						<xsl:if test="PERSON[3]">; <xsl:value-of select="PERSON[3]" /></xsl:if>
					</xsl:if>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					<xsl:text>-</xsl:text>
				</xsl:otherwise>
			</xsl:choose> 
		</xsl:element>
	</xsl:template>
	
	<!-- 45 - Category as category of the task .  Can easily change this to work for TAG-->	
	<!-- To use this code, one of the other columns on the report will need to be replaced, or another column needs to be created-->
<!--	<xsl:template name="get_cat">
		<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="(CATEGORY)">
					<xsl:if test="not(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
					</xsl:if>
					<xsl:if test="(@DONEDATESTRING)">
						<xsl:attribute name="class">taskinfo tidone tborder</xsl:attribute>
					</xsl:if>
					<xsl:value-of select="CATEGORY"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					<xsl:text>-</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template> -->
	
	<!-- 46 - @TITLE as title-->
		<!-- Note:  To add flag information, category, or comments to the task title in the report, set
			showcat, showflagged, or showcomments at the top of the code-->
	<xsl:template name="get_Task_title">
		<xsl:element name="td">
			<xsl:if test="not(@DONEDATESTRING)">
				<xsl:attribute name="class">taskname tborder</xsl:attribute>
				<xsl:if test="$showtaskcolouring=1">
					<xsl:attribute name="style">color:<xsl:value-of select="@TEXTWEBCOLOR"/></xsl:attribute>	<!--PRIORITYWEBCOLOR??-->
				</xsl:if>
				<xsl:if test="$showtaskcolouring=0">
					<xsl:attribute name="style">color:black</xsl:attribute>	<!--PRIORITYWEBCOLOR??-->
				</xsl:if>
			</xsl:if>
			<xsl:if test="(@DONEDATESTRING)">
				<xsl:attribute name="class">taskname tasknamedone tborder</xsl:attribute>
			</xsl:if>

            <xsl:if test="count(ancestor::TASK)>0">
				<xsl:call-template name="tab"/>
				<xsl:text>&#183; </xsl:text><!--&middot; middot.  alternatives &#149=bullet; &#187=»-->
			</xsl:if>
			
			<xsl:value-of select="@TITLE"/>
			<xsl:text>&#160; &#160;</xsl:text> <!-- &nbsp;-->

			<xsl:choose>
				<xsl:when test="@REFID=0">
					<xsl:element name="span">
						<xsl:if test="not(@DONEDATESTRING)">
							<xsl:attribute name="class">tinfo</xsl:attribute>
						</xsl:if>
						<xsl:if test="(@DONEDATESTRING)">
							<xsl:attribute name="class">tinfodone</xsl:attribute>
						</xsl:if>
						<xsl:if test="($showflagged=1 and @FLAG)">
							<xsl:element name="span">
								<xsl:attribute name="class">tinfobold</xsl:attribute>
								<xsl:text>    [Flag] </xsl:text>
							</xsl:element>
						</xsl:if>
						<xsl:if test="($showcat=1) and CATEGORY">
							<xsl:text>    [Cat: </xsl:text>
							<xsl:value-of select="CATEGORY"/>
							<xsl:if test="CATEGORY[2]">;<xsl:value-of select="CATEGORY[2]" /></xsl:if>
							<xsl:if test="CATEGORY[3]">;<xsl:value-of select="CATEGORY[3]" /></xsl:if>
							<xsl:if test="CATEGORY[4]">;<xsl:value-of select="CATEGORY[4]" /></xsl:if>
							<xsl:if test="CATEGORY[5]">;<xsl:value-of select="CATEGORY[5]" /></xsl:if>
							<xsl:text>]</xsl:text>
						</xsl:if>
						<xsl:if test="($showstatus=1) and @STATUS">
							<xsl:text>    [Status: </xsl:text>
							<xsl:value-of select="@STATUS"/>
							<xsl:text>]</xsl:text>
						</xsl:if>
					</xsl:element>
					
					<xsl:if test="($showcomments=1) and COMMENTS">
						<xsl:choose>
							<xsl:when test="($showformattedcomments=1) and HTMLCOMMENTS">
								<xsl:element name="div">
									<xsl:attribute name="class">commentbox</xsl:attribute>
									<xsl:attribute name="style">color:black</xsl:attribute>
									<xsl:value-of select="HTMLCOMMENTS" disable-output-escaping="yes"/>
								</xsl:element>
							</xsl:when>
							<xsl:otherwise>
						<!--		<xsl:call-template name="tab"/>  -->
								<xsl:element name="div">
									<xsl:attribute name="class">commentbox commentfont</xsl:attribute>
									<xsl:if test="($commentlength=0)">
										<xsl:call-template name="fix-breaks">
											<xsl:with-param name="text">
												<xsl:value-of select="COMMENTS"/>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:if>
									<xsl:if test="($commentlength&gt;0)">
										<xsl:call-template name="fix-breaks">
											<xsl:with-param name="text">
												<xsl:value-of select="substring(COMMENTS,1,$commentlength)"/>
											</xsl:with-param>
										</xsl:call-template>
										<xsl:if test="(string-length(COMMENTS)&gt;=$commentlength)">
											<xsl:text>  ... </xsl:text>
										</xsl:if>
									</xsl:if>
								</xsl:element>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:if>
				</xsl:when>
				<xsl:otherwise>
					<xsl:element name="span">
						<xsl:attribute name="class">tinfo</xsl:attribute>
						<xsl:attribute name="style">font-style: italic</xsl:attribute>
						<xsl:text> [Ref] </xsl:text>
					</xsl:element>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	
	<!-- 50 - It puts spaces in (indents)-->
	<xsl:template name="tab">
		<xsl:if test="count(ancestor::TASK)>0">
			<xsl:for-each select="(ancestor::TASK)">
				<xsl:element name="span">
					<xsl:attribute name="class">tab</xsl:attribute>
				</xsl:element>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>

	<!-- 60 - retain CRLF within comments by adding BR elements ()-->
	<xsl:template name="fix-breaks">
		<xsl:param name="text" />
		<xsl:choose>
			<xsl:when test="contains($text,'&#13;&#10;')">
				<xsl:value-of select="substring-before($text,'&#13;&#10;')" />
				<xsl:element name="br"/>
				<xsl:call-template name="fix-breaks">
					<xsl:with-param name="text">
						<xsl:value-of select="substring-after($text,'&#13;&#10;')" />
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$text" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	<!-- 70 - reformat the date into dd mmm yy format-->
		<!-- Converts dates from dd/mm/yy format to dd mmm yy format-->
         <xsl:template name="FormatDate">
	    <xsl:param name="thisdate" />
	    <!--Clean the date string of any time information (e.g. from @DONEDATESTRING)-->
	    	<xsl:param name="holder" select="substring($thisdate,1,10)" />
		<xsl:param name="cleandate" select="normalize-space($holder)" />
		
	    <!--if stringlength is 10, then is a double digit date: 21/03/2012.  if stringlength is 9, then is a single digit date: 1/03/2012-->
	    <!--The date format depends on the users regional settings-->
	    <!--If the date format is mm/dd/yy, then swap 'mnth' and 'day' 9 and 17 rows below-->
	    <!--the 'year-from-date()' functions would be great but TDL can only run xsl v 1.0 functions :-(-->

		<xsl:param name="year">
			<xsl:if test="(string-length($cleandate) = 10)">
			      <xsl:value-of select="substring($cleandate,9,2)" />
			</xsl:if>
			<xsl:if test="(string-length($cleandate) = 9)">
			      <xsl:value-of select="substring($cleandate,8,2)" />
			</xsl:if>
		</xsl:param>
		<xsl:param name="mnth">
			<xsl:if test="(string-length($cleandate) = 10)">
			      <xsl:value-of select="substring($cleandate,4,2)" />
			</xsl:if>
			<xsl:if test="(string-length($cleandate) = 9)">
			      <xsl:value-of select="substring($cleandate,3,2)" />
			</xsl:if>
		</xsl:param>
		<xsl:param name="day">
			<xsl:if test="(string-length($cleandate) = 10)">
			      <xsl:value-of select="substring($cleandate,1,2)" />
			</xsl:if>
			<xsl:if test="(string-length($cleandate) = 9)">
			      <xsl:value-of select="substring($cleandate,1,1)" />
			</xsl:if>
		</xsl:param>
<!-- Ideally the code below would be used instead of the string manipulation code below.  But these are xsl v2.0 functions -->
<!--<xsl:param name="year" select="year-from-date(xs:date("$cleandate"))" />
<xsl:param name="mnth" select="month-from-date(xs:date("$cleandate"))" />
<xsl:param name="day" select="day-from-date(xs:date("$cleandate"))" /> -->
<!--<xsl:value-of year-from-date(xs:date("2005-04-23"))-->

	    <xsl:value-of select="$day"/>
	    <xsl:choose>
	      <xsl:when test="$mnth = '01'"> Jan </xsl:when>
	      <xsl:when test="$mnth = '02'"> Feb </xsl:when>
	      <xsl:when test="$mnth = '03'"> Mar </xsl:when>
	      <xsl:when test="$mnth = '04'"> Apr </xsl:when>
	      <xsl:when test="$mnth = '05'"> May </xsl:when>
	      <xsl:when test="$mnth = '06'"> Jun </xsl:when>
	      <xsl:when test="$mnth = '07'"> Jul </xsl:when>
	      <xsl:when test="$mnth = '08'"> Aug </xsl:when>
	      <xsl:when test="$mnth = '09'"> Sep </xsl:when>
	      <xsl:when test="$mnth = '10'"> Oct </xsl:when>
	      <xsl:when test="$mnth = '11'"> Nov </xsl:when>
	      <xsl:when test="$mnth = '12'"> Dec </xsl:when>
	      <xsl:otherwise>-?</xsl:otherwise>
	    </xsl:choose>
	    <xsl:value-of select="$year"/>
	  </xsl:template> 
	
</xsl:stylesheet>

