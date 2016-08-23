<?xml version="1.0" encoding="UTF-8"?>
<!-- Based on ToDoListTableStylesheet_v1.0 as Created by xabatcha@seznam.cz, 2006-10-17 -->
<!-- Modified by JAZ,   V1 Dec 2012-->
<!-- Transform ToDoList to html, using table layout. -->

<!--### USER ADJUSTABLE through GLOBAL PARAMETERS below these comments ###
                                                                     Best run from TaskTree View -->

<!-- This stylesheet includes: Taskname,  Time Estimate, Time Spent, Calculated Time Estimate, Calculated Time Spent, Time Spent Units
            Have included the possibility to add the category and people assigned next to the task title if you wish.
            The standard view is the whole task hierarchy, in its indented structure (from Task Tree view).
            Have included the possibility of viewing only the root tasks (from Task Tree view - in List View, all tasks are root tasks).
            Have included the possibility of viewing only the child tasks (tree leaves), below the root tasks (from Task Tree view).
                If this is selected, the task path (ancestors) is also shown next to the childs taskname, and the list is presented in flat format (not indented).
                
           ### These options are set at the top of the code using global parameters; showcat, showpeople, showOnlyLeaves  ###
       Selecting particular tasks to report on can be handled by selection in the printing dialog or by pre-filtering-->
       
 <!-- The number formatting highlights the likely important numbers - 
            i.e. any parent task will have a calculated time spent, and may well not have a time spent-->
<!-- Empty attributes have a '-' added-->
<!-- A dot is added to indented tasks for clarity-->
<!-- Table borders are mostly switched off.  
            To turn on borders around the table, uncomment table.finished.  table.finished does the outside of the table.
            Rows are underlined, adjust td.tborder to change
            To adjust or turn off the borders, goto td.tborder, td.headborder, td.underline and comment out as needed.  -->
<!--Text size and colour is changed depending on value and task status
            A task that is done is greyed and crossed out
            Lead / root tasks may be bolded - see tr.lead and the global parameter: boldroottasks
            Lead / root tasks have extra spacing before it for visual clarity - set the global parameter: leadspacing
               To turn this off, set the global parameter at the top of the code, leadspacing-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
	<xsl:strip-space elements="*"/>
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" encoding="utf-8"/>
	
	<!-- ################## Selectable values to customise the report ######################
	                         To apply setting, enter 1, to turn off setting, enter 0.-->
	                         
	<!-- To put spacing (extra line for visual separation) in front of a lead task, enter 1.  To remove this, enter 0.-->  
	<xsl:param name="leadspacing" select="1" />
	<!-- To bold all root tasks, enter 1 (default).  To remove this, enter 0.-->  
	<xsl:param name="boldroottask" select="1" />
	<!-- To show any of these with the task title, change the 0 to a 1. -->  
	<xsl:param name="showcat" select="0" />
	<xsl:param name="showstatus" select="0" />
	<xsl:param name="showtag" select="0" />
	<xsl:param name="showpeople" select="1" />
	<!--To show only the root tasks, and tasks with no children (leaves of the tree), select 1.  
	This results in a semi-flat list of tasks under the root task, and brings up the tasks path (ancestors)-->
	<xsl:param name="showOnlyLeaves" select="1" /> 
	<!--To show only the root tasks, select 1.  
	This results in a flat list of root tasks.  NOTE you don't want both OnlyRoots and OnlyLeaves selected-->
	<xsl:param name="showOnlyRoots" select="0" />
	
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
						font-weight: bold;	
						vertical-align: center;
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
						vertical-align: centre;
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
					body {
						font-family: Arial;
						vertical-align: center;
					}
				</xsl:text>
			</xsl:element>
		</xsl:element>
		<xsl:element name="body">
			<xsl:element name="table">
				<xsl:attribute name="class">finished</xsl:attribute>
				<xsl:attribute name="cellspacing">0</xsl:attribute>
				<xsl:attribute name="cellpadding">1</xsl:attribute>
                                <!-- Project name and printing date -->				
				<xsl:element name="tr">
					<xsl:attribute name="class">titlef</xsl:attribute>
					<xsl:element name="td">
						<xsl:attribute name="colspan">3</xsl:attribute>
						<xsl:attribute name="class">headleft underline</xsl:attribute>
						<xsl:value-of select="@PROJECTNAME"/>
						<xsl:choose>
							<xsl:when test="$showOnlyLeaves=1">
								<xsl:text>:   Time Spent - Children Only </xsl:text>		
							</xsl:when>
							<xsl:when test="$showOnlyRoots=1">
								<xsl:text>:   Time Spent - Root Tasks Only </xsl:text>		
							</xsl:when>
							<xsl:otherwise><!--process all tasks-->
								<xsl:text>:   Time Spent - All Tasks </xsl:text>		
							</xsl:otherwise>
						</xsl:choose>
						<BR/><BR/>
					</xsl:element>
					<xsl:element name="td">
						<xsl:attribute name="colspan">3</xsl:attribute>
						<xsl:attribute name="class">headright underline</xsl:attribute>
						<xsl:value-of select="@REPORTDATE"/>
						<BR/><BR/>
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
				<xsl:if test="$showOnlyRoots=0">
					<xsl:choose>
						<xsl:when test="$showOnlyLeaves=1">
							 <xsl:if test="count(child::TASK)=0">
								<xsl:call-template name="get_Task"/>
								<xsl:apply-templates select="TASK"/>
							</xsl:if>
							 <xsl:if test="count(child::TASK)>0">
								<xsl:apply-templates select="TASK"/>
							</xsl:if>
						</xsl:when>
						<xsl:otherwise><!--process all tasks-->
							<xsl:call-template name="get_Task"/>
							<xsl:apply-templates select="TASK"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
 			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	<!-- 20 - header of table - titles of columns -->
	<xsl:template name="get_Header">
		<xsl:element name="tr">
			<xsl:attribute name="class">header</xsl:attribute>
			<xsl:element name="td">
				<xsl:attribute name="class">headleft headborder</xsl:attribute>
				<xsl:attribute name="width">55%</xsl:attribute>
				<xsl:text>Task</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>Time Estimate</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>Actual Time Spent</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>Calc Time Estimate</xsl:text>
			</xsl:element>
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">10%</xsl:attribute>
				<xsl:text>Calc Time Spent</xsl:text>
			</xsl:element> 
			<xsl:element name="td">
				<xsl:attribute name="class">headcenter headborder</xsl:attribute>
				<xsl:attribute name="width">5%</xsl:attribute>
				<xsl:text>Units</xsl:text>
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
		<xsl:call-template name="get_Task_title"/>
		<xsl:call-template name="get_TimeEst"/>
		<xsl:call-template name="get_TimeSpent"/>
		<xsl:call-template name="get_EstTimeCalc"/>
		<xsl:call-template name="get_TimeCalc"/>
		<xsl:call-template name="get_TimeUnits"/>
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

	<!-- 41 - @TIMEESTIMATE as the time estimated that would take -->	
	<xsl:template name="get_TimeEst">
		<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="(@TIMEESTIMATE)">
					<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
					<xsl:value-of select='format-number((@TIMEESTIMATE), "#.00")'/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					<xsl:text>-</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			
		</xsl:element>
	</xsl:template>
	
	<!-- 42 - @TIMESPENT as the time spent on that task -->	
	<xsl:template name="get_TimeSpent">
		<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="(@TIMESPENT)">
					<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
					<xsl:value-of select='format-number((@TIMESPENT), "#.00")'/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					<xsl:text>-</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	
	<!-- 43 - @CALCTIMEESTIMATE as the time spent on that task and all subtasks-->	
	<xsl:template name="get_EstTimeCalc">
		<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="(@CALCTIMEESTIMATE)">
					<xsl:choose>
					 	<xsl:when test="count(child::TASK)>0">
					 		<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
					 		<xsl:value-of select='format-number((@CALCTIMEESTIMATE), "#.00")'/>
					 	</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					 		<xsl:value-of select='format-number((@CALCTIMEESTIMATE), "#.00")'/>
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
	
	<!-- 44 - @CALCTIMESPENT as the time spent on that task and all subtasks-->	
	<xsl:template name="get_TimeCalc">
		<xsl:element name="td">
			<xsl:choose>
				<xsl:when test="(@CALCTIMESPENT)">
					<xsl:choose>
					 	<xsl:when test="count(child::TASK)>0">
					 		<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
					 		<xsl:value-of select='format-number((@CALCTIMESPENT), "#.00")'/>
					 	</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="class">taskinfo timinor tborder</xsl:attribute>
					 		<xsl:value-of select='format-number((@CALCTIMESPENT), "#.00")'/>
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
	
	<!-- 45 - @TIMESPENTUNITS as the units used for the time spent.  Assumes the same units used for the Estimates-->	
	<xsl:template name="get_TimeUnits">
		<xsl:element name="td">
			<xsl:attribute name="class">taskinfo tinormal tborder</xsl:attribute>
			<xsl:if test="(@TIMESPENTUNITS)">
				<xsl:value-of select="@TIMESPENTUNITS"/>
			</xsl:if>
			<xsl:if test="not(@TIMESPENTUNITS)">
				<xsl:text>-</xsl:text>
			</xsl:if>
		</xsl:element>
	</xsl:template>
	
	<!-- 46 - @TITLE as title-->
		<!-- Note:  To add extra information to th task title, e.g. category, to the task title in the report, set
			the param declarations at the top of the code e.g. showcat-->
	<xsl:template name="get_Task_title">
		<xsl:element name="td">
			<xsl:if test="not(@DONEDATESTRING)">
				<xsl:attribute name="class">taskname tborder</xsl:attribute>
			</xsl:if>
			<xsl:if test="(@DONEDATESTRING)">
				<xsl:attribute name="class">taskname tasknamedone tborder</xsl:attribute>
			</xsl:if>

                        <xsl:if test="count(ancestor::TASK)>0">
				<xsl:if test="$showOnlyLeaves=1"><!--single indent if leaves only-->
					<xsl:element name="span">
						<xsl:attribute name="class">tab</xsl:attribute>
					</xsl:element>
				</xsl:if>
				<xsl:if test="$showOnlyLeaves=0"><!--proper indenting if all tasks-->
					<xsl:call-template name="tab"/>
				</xsl:if>
				<xsl:text>&#183; </xsl:text><!--&middot; middot.  alternatives &#187=»-->
			</xsl:if>
			
			<xsl:value-of select="@TITLE"/>
			<xsl:text>&#160; &#160;</xsl:text> <!-- &nbsp;-->

			<xsl:element name="span">  <!--Additional information next to task title-->
				<xsl:if test="not(@DONEDATESTRING)">
					<xsl:attribute name="class">tinfo</xsl:attribute>
				</xsl:if>
				<xsl:if test="(@DONEDATESTRING)">
					<xsl:attribute name="class">tinfodone</xsl:attribute>
				</xsl:if>
				<xsl:if test="$showOnlyLeaves=1"><!--Need to see the task ancestors-->
					<xsl:if test="count(ancestor::TASK)>0">
					    <xsl:text> [Path: </xsl:text>
					    <xsl:for-each select="(ancestor::TASK)">
						<xsl:value-of select="@TITLE"/>
						<xsl:text>/</xsl:text>
					    </xsl:for-each>
					    <xsl:text>]</xsl:text>
					</xsl:if>
				</xsl:if>
				<xsl:if test="($showcat=1) and CATEGORY">
					<xsl:text>    [Cat: </xsl:text>
					<xsl:value-of select="CATEGORY"/>
					<xsl:if test="CATEGORY[2]">; <xsl:value-of select="CATEGORY[2]" /></xsl:if>
					<xsl:if test="CATEGORY[3]">; <xsl:value-of select="CATEGORY[3]" /></xsl:if>
					<xsl:if test="CATEGORY[4]">; <xsl:value-of select="CATEGORY[4]" /></xsl:if>
					<xsl:if test="CATEGORY[5]">; <xsl:value-of select="CATEGORY[5]" /></xsl:if>
					<xsl:text>]</xsl:text>
				</xsl:if>
				<xsl:if test="($showstatus=1) and @STATUS">
					<xsl:text>    [Status: </xsl:text>
					<xsl:value-of select="@STATUS"/>
					<xsl:text>]</xsl:text>
				</xsl:if>
				<xsl:if test="($showtag=1) and TAG">
					<xsl:text>    [Tag: </xsl:text>
					<xsl:value-of select="TAG"/>
					<xsl:if test="TAG[2]">; <xsl:value-of select="TAG[2]" /></xsl:if>
					<xsl:if test="TAG[3]">; <xsl:value-of select="TAG[3]" /></xsl:if>
					<xsl:if test="TAG[4]">; <xsl:value-of select="TAG[4]" /></xsl:if>
					<xsl:if test="TAG[5]">; <xsl:value-of select="TAG[5]" /></xsl:if>
					<xsl:text>]</xsl:text>
				</xsl:if>
				<xsl:if test="($showpeople=1) and PERSON">
					<xsl:text>    [Assigned to: </xsl:text>
					<xsl:value-of select="PERSON"/>
					<xsl:if test="PERSON[2]">; <xsl:value-of select="PERSON[2]" /></xsl:if>
					<xsl:if test="PERSON[3]">; <xsl:value-of select="PERSON[3]" /></xsl:if>
					<xsl:if test="PERSON[4]">; <xsl:value-of select="PERSON[4]" /></xsl:if>
					<xsl:if test="PERSON[5]">; <xsl:value-of select="PERSON[5]" /></xsl:if>
					<xsl:text>]</xsl:text>
				</xsl:if>
			</xsl:element>
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
    
</xsl:stylesheet>

