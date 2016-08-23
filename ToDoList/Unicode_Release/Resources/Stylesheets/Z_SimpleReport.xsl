<?xml version="1.0" encoding="UTF-8"?>
<!-- Based on ToDoListTableStylesheet_v1.0 as Created by xabatcha@seznam.cz, 2006-10-17 -->
<!-- Modified by JAZ, April 12 2010, 
August&December 2012, (added variety of parameters to guide formatting of output
June 2103 (added coloured tasks, HTML comments, ability to handle CATEGORY, TAG, PERSON and COMMENTS as task elements rather than task attributes)-->
<!-- Transform ToDoList to html, using table layout -->

<!--### USER ADJUSTABLE through GLOBAL PARAMETERS below these comments ###-->

<!-- General-->
<!-- This stylesheet gives a single column, listing tasks in a simple manner.  
           Have included the possibility to add Flagged status, Priority, Progress, Assigned to, Category, & Status next to the task title.
           Can add Comments in a box below the task title
               This is set at the top of the code using global parameters; showcat, showstatus, showflagged, showcomments etc...
               The length of the comment can be limited to a defined number of characters
             Have included the possibility to add more than just the first person assigned to a task (up to 3)
             Have included up to 5 categories-->
 <!-- Finished and unfinished tasks are able to be shown. 
            Select from the print preview dialog 
            Finished tasks are greyed and struck out-->
<!-- Added the possibility of a bullet to indented tasks for clarity-->
<!-- Where Calcpercentdone = 100% but task has not been checked off (percentdone < 100%), progess is given as 100%, but the task not deemed to be finished and hence not 'struck through' -->

<!-- Formatting:--> 
<!--    Table borders are mostly switched off (except bottom border).  
            To turn on other borders around the table, uncomment table.finished.  table.finished does the outside of the table.
            Rows may have a line below them, adjust td.tborder to change
            To adjust or turn off the borders, goto td.tborder, td.headborder and comment out as needed.  -->
<!--    Text size and colour is changed depending on value and task status.  Feel free to modify. As supplied:
            A task that is done is greyed and crossed out
			Tasks may be coloured.
            Additional information after a task is grey and in a smaller font, to keep the task title visually clearer
            Comments are greyed and italicised to keep the task title visually clearer
            Lead / root tasks may be bolded (user settable global parameter: boldroottasks)
            Lead / root tasks can have extra spacing before it for visual clarity
               To turn this off, set the global parameter at the top of the code, leadspacing
			Rich text comments are enabled, however this can be switched off, so richtext comments appear as simple text (can save space)-->

<!--  Intended improvements not achieved
        Proper tabbing of task names in treeview (indent when task name wraps)
          - note original doesn't do this properly either.
	Dont have listview bolded, but still have top task bolded in treeview (I think they are bolded as in that view, tasks have no descendents
	Repeat the heading on each page.
	Colouring due or overdue tasks.-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
	<xsl:strip-space elements="*"/>
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" encoding="utf-8"/>
	
	<!-- ################## Selectable values to customise the report ######################
	                          To apply setting, enter 1, to turn off setting, enter 0.-->
	
	<!-- To put spacing (extra line for visual separation) in front of a lead task, enter 1.  To remove this, enter 0. 
	       Note, this can be useul when reporting from List View-->  
	<xsl:param name="leadspacing" select="1" />
	<!--  To separate tasks with lines (for visual clarity)-->
	<xsl:param name="lineforTask" select="1" /> <!-- effectively underlines each task-->
	<xsl:param name="lineforRoot" select="1" /> <!-- puts a line before each root task-->
	<!-- To bold and make bigger all root tasks, enter 1 (default).  To remove this, enter 0.-->  
	<xsl:param name="boldroottask" select="1" />
<!-- To show task colouring, enter 1 (default).  To remove this, enter 0.-->  
	<xsl:param name="showtaskcolouring" select="1" />	
	<!-- To show any of additional information with the task title,  -->  
	<xsl:param name="showdot" select="1" />
	<xsl:param name="showcat" select="1" />
	<xsl:param name="showstatus" select="0" />
	<xsl:param name="showtag" select="0" />
	<xsl:param name="showflagged" select="1" />
	<xsl:param name="showpeople" select="0" />
	<xsl:param name="showpriority" select="0" />
	<xsl:param name="showprogress" select="0" />
	<xsl:param name="showstartdate" select="0" />
	<xsl:param name="showduedate" select="0" />
	<!-- To show the dates simply dd/mm/yy - select 1, or as dd mmm yy - select 0  -->  
	<xsl:param name="showsimpledates" select="1" />
	<!-- To show comments (1)-->
	<xsl:param name="showcomments" select="1" />
	<!-- To show formatted (Richtext) comments (1)-->
	<xsl:param name="showformattedcomments" select="1" />	
	<!--To limit the length of the comment, enter the number of characters to show.  
	    Note, it is about 130 characters per line.  However, if there are lots of line breaks, it will still be tall.-->
	<xsl:param name="commentlength" select="0" />
	
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
					<!--  Note:  Colors: red FF0000, blue 0000FF, green 008000-->
					<!-- Note:  padding in a table cell: 10px 20px 15px 5px  -  top, right, bottom, left respectively
						    (Easy reminder - Clockwise starting with top).  Same for margin.-->
					table.finished{
					<!--	This does the outside of the main table - i.e. all tasks
						border-top: 2px solid Black;
						border-right: 1px solid Black; 	   
						border-left: 2px solid Black;  -->
						border-bottom: 1px solid Black;
						width: 100%;
						vertical-align: center;
					}
					tr.titlef{
						font-weight: bold;
						font-size: 18px;
					}
					tr.blankline{
						font-size: 9px;
						border-bottom: 2px solid white;
						}  -->
					td.headleft{
						text-align:left;
					}
					tr.leadbold{
						vertical-align: center;
						font-weight: bold;   
					}
					tr.taskdone{
						color: #D3D3D3;
						text-decoration: line-through;
					}					
					td.tasktitle{
						text-align:left;
						vertical-align: center;
						font-size: 12px;
					}

					span.tinfo{
						font-size: 10px;
						color: #808080
					}
					span.tinfodone{
						font-size: 10px;
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
					td.headborder{
						border-bottom: 2px solid #CDCDCD;
					}
					td.tborder{
					<!--	border-top: 1px solid #CDCDCD;	-->
						border-bottom: 1px solid #CDCDCD;
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
                                <!-- Project name -->				
				<xsl:element name="tr">
					<xsl:attribute name="class">titlef</xsl:attribute>
					<xsl:element name="td">
						<xsl:attribute name="colspan">1</xsl:attribute>
						<xsl:attribute name="class">headleft headborder</xsl:attribute>
						<xsl:text>Project: </xsl:text>
						<xsl:value-of select="@PROJECTNAME"/>
						<!--<BR/><BR/>-->
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
	
	<!-- 30 - lead task - child of TODOLIST tag -->
	<xsl:template name="get_LeadTask">
		<xsl:element name="tr">
				<xsl:if test="($boldroottask=1)">
					<xsl:if test="(@DONEDATESTRING)">
						<xsl:attribute name="class">leadbold taskdone</xsl:attribute>
					</xsl:if>				
					<xsl:if test="not(@DONEDATESTRING)">
						<xsl:attribute name="class">leadbold</xsl:attribute>
					</xsl:if>				
				</xsl:if>  
				<xsl:if test="($boldroottask=0)">
					<xsl:if test="(@DONEDATESTRING)">
						<xsl:attribute name="class">taskdone</xsl:attribute>
					</xsl:if>							
				</xsl:if>  
			<xsl:call-template name="get_Task_Details"/>
		</xsl:element>
	</xsl:template>
	
	<!-- 31- single task -->
	<xsl:template name="get_Task">
		<xsl:element name="tr">
			<xsl:if test="(@DONEDATESTRING)">
				<xsl:attribute name="class">taskdone</xsl:attribute>
			</xsl:if>
			<xsl:call-template name="get_Task_Details"/>
		</xsl:element>
	</xsl:template>
	
	<!--32 - detail of single task -->	
	<xsl:template name="get_Task_Details">
		<xsl:call-template name="get_Task_title"/>
	</xsl:template>
	
	<!-- 33 - blank row to insert before a lead task - as spacing -->	
	<xsl:template name="insert_line">
		<xsl:element name="tr">	
			<xsl:attribute name="class">blankline</xsl:attribute>
			<xsl:element name="td">
				<BR/>
			</xsl:element>
		</xsl:element>
	</xsl:template>	

	<!-- 46 - @TITLE as title-->
		<!-- Note:  To add additional information (e.g. category, etc or comments) to the task title in the report, set
			set the appropriate switches at the top of the code (e.g. category = showcat, comments = showcomments-->
	<xsl:template name="get_Task_title">
		<xsl:element name="td">

                       <!--Insertion of lines for visual clarity-->
            <xsl:if test="count(ancestor::TASK)=0">		<!--Is a root task-->
			<!--	<xsl:if test="position()>1">-->
					<xsl:if test="($lineforRoot=1) or ($lineforTask=1)">
						<xsl:attribute name="class">tasktitle tborder</xsl:attribute>
					</xsl:if>
					<xsl:if test="($lineforRoot=0) and ($lineforTask=0)">
						<xsl:attribute name="class">tasktitle</xsl:attribute>
					</xsl:if>
			<!--	</xsl:if>-->
 			</xsl:if>
			<xsl:if test="count(ancestor::TASK)>0">		<!--Is a normal task-->	
				<xsl:if test="($lineforTask=1)">
					<xsl:attribute name="class">tasktitle tborder</xsl:attribute>
				</xsl:if> 
				<xsl:if test="($lineforTask=0)">
					<xsl:attribute name="class">tasktitle</xsl:attribute>
				</xsl:if> 
			</xsl:if> 
			<xsl:if test="$showtaskcolouring=1">
				<xsl:attribute name="style">color:<xsl:value-of select="@TEXTWEBCOLOR"/></xsl:attribute>	<!--PRIORITYWEBCOLOR??-->
			</xsl:if>
			<xsl:if test="$showtaskcolouring=0">
				<xsl:attribute name="style">color:black</xsl:attribute>	<!--PRIORITYWEBCOLOR??-->
			</xsl:if>
                                     
		   <!--Addition of bullet / marker non-root tasks for visual clarity-->
			<xsl:if test="count(ancestor::TASK)>0">
				<xsl:call-template name="tab"/>
				<xsl:if test="($showdot=1)">
					<xsl:text>&#183; </xsl:text><!--&middot; middot.  alternatives &#149=bullet; &#187=»-->
				</xsl:if>
			</xsl:if>
			
            <!--Task title-->
            <xsl:value-of select="@TITLE"/>
			<xsl:text>&#160; &#160;</xsl:text> <!-- &nbsp;-->

            <!--Additional information to the task title-->
			<xsl:element name="span">
				<xsl:if test="not(@DONEDATESTRING)">
					<xsl:attribute name="class">tinfo</xsl:attribute>
				</xsl:if>
				<xsl:if test="(@DONEDATESTRING)">
					<xsl:attribute name="class">tinfodone</xsl:attribute>
				</xsl:if>
	
				<xsl:if test="@REFID&gt;0">
					<xsl:element name="span">
						<xsl:attribute name="style">font-style: italic</xsl:attribute>
						<xsl:text> [Ref] </xsl:text>
		<!--				<xsl:if test="//TASK/@ID=@REFID">
							<xsl:text> [foundit] </xsl:text>
						</xsl:if>		-->				
					</xsl:element>
				</xsl:if>
				<!--Are we able to test the referred task for completeness, and if so, any other useful info? -->

				<xsl:if test="($showflagged=1 and @FLAG)">
					<xsl:element name="span">
						<xsl:attribute name="class">tinfobold</xsl:attribute>
						<xsl:text>[Flagged] </xsl:text>
					</xsl:element>
				</xsl:if>
			    <xsl:if test="($showpriority=1) and @PRIORITY">
					<xsl:text>    [Priority: </xsl:text>
					<xsl:value-of select="@PRIORITY"/>
					<xsl:text>]</xsl:text>
				</xsl:if>
				<xsl:if test="($showprogress=1) ">
					<xsl:text>    [Progress: </xsl:text>
					<xsl:choose>
						<xsl:when test="(@PERCENTDONE)">
							<xsl:choose>
								<xsl:when test="(@PERCENTDONE=0)">
									<xsl:choose>
										<xsl:when test="(@CALCPERCENTDONE)">
											<xsl:value-of select="@CALCPERCENTDONE"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="@PERCENTDONE"/>
										</xsl:otherwise>
									</xsl:choose>			            			            
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="@PERCENTDONE"/>			            
								</xsl:otherwise>
							</xsl:choose>		            			            
						</xsl:when>
						<xsl:otherwise>
							<xsl:if test="(@CALCPERCENTDONE)">
								<xsl:value-of select="@CALCPERCENTDONE"/>
							</xsl:if>
						</xsl:otherwise>
					</xsl:choose>			            			            
					<xsl:text>%]</xsl:text>
				</xsl:if>
				<xsl:if test="($showstartdate=1) and @STARTDATESTRING">
					<xsl:text>    [Start: </xsl:text>
					<xsl:if test="($showsimpledates=1)">
						 <xsl:value-of select="substring(@STARTDATESTRING,1,10)"/>
					</xsl:if>
					<xsl:if test="$showsimpledates=0">
						<xsl:call-template name="FormatDate">
						    <xsl:with-param name="thisdate" select="@STARTDATESTRING"/>
						</xsl:call-template>  
					</xsl:if>				
					<xsl:text>]</xsl:text>
				</xsl:if>
				<xsl:if test="($showduedate=1) and @DUEDATESTRING">
					<xsl:if test="not(@DONEDATESTRING)">
						<xsl:text>    [Due: </xsl:text>
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
						<xsl:text>    [Done: </xsl:text>
						<xsl:if test="($showsimpledates=1)">
							 <xsl:value-of select="substring(@DONEDATESTRING,1,10)"/>
						</xsl:if>
						<xsl:if test="$showsimpledates=0">
							<xsl:call-template name="FormatDate">
							    <xsl:with-param name="thisdate" select="@DONEDATESTRING"/>
							</xsl:call-template>  
						</xsl:if>
					</xsl:if>				
					<xsl:text>]</xsl:text>
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
					<!--	<xsl:call-template name="tab"/>  -->
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
<!-- Ideally this following code should be used instead of the string manipulation code below 
However, these are xsl v2.0 functions, and TDL only supports xsl v1.0 functions.-->
<!--<xsl:param name="year" select="year-from-date(xs:date("$cleandate"))" />
<xsl:param name="mnth" select="month-from-date(xs:date("$cleandate"))" />
<xsl:param name="day" select="day-from-date(xs:date("$cleandate"))" />-->
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

