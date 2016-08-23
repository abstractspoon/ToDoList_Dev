<?xml version="1.0" ?>
<!--
**************** TDL to Project Estimate with comparison to Actual Time Spent and Status **************** 
Stylesheet for use with Tasklist Transform functionality of ToDoList by Dan Godson:
http://www.codeproject.com/Articles/5371/ToDoList

Stylesheet by Tony Gravagno (CodeProject ID: iamstarbuck)
Cloned and significantly modified from Project Summary.
All prior comments removed but credit to original authors:
- BOSC Laurent
- zajchapp
- Originally inspired from : ToDoListStyler by Manual Reyes and Hoppfrosch@gmx.de
*******************************************

Features:
- Dynamic indentation with subtask level
- Dynamic styling using variables set as preferences
- Text labels set using variables rather than hard-coded into XSL
- When HTML is rendered, container/row width changes with browser width
- Generated HTML source is very well formatted and extremely easy to read (at cost of verbosity and increased size)
- With preference variable, shifts easily between internal styles and use of external CSS file
- Extremely well commented and organized for easy maintenance (and for learning XSL)
- Actual time has different styles for underEstimate, overEstimate, and equalsEstimate.
*******************************************

Please post questions and comments about this stylesheet to the ToDoList project forum
on CodeProject. The latest version of this file will be in the ToDoList package.

Updates:
150212a : TG First version derived from Estimates-only version
150218a : TG Use REPORTTITLE rather than PROJECTNAME in report header/title

Possible enhancements:
- Include cost, maybe due date, risk, priority
- Custom styling down to 5 tiers rather than just 1,2, and N.
- Add option to compress HTML source
- Add collapsing sections via JavaScript to show/hide levels 2-3
- Eliminate multiple TD tags, replace with single tags with colspan
*******************************************

Usage:
- Change parameter values below.
- In ToDoList, select a group of tier-1 or tier-2 tasks and any sub-tasks.
- If a task does not have an estimate, it should have a comment. An example would be a
task to purchase an item which has a cost.
Only the first line of a comment block is displayed.
- Invoke as a transform from ToDoList.
- - Use Selected tasks.
- - The output columns must include the task ID, title, comments, estimates and actual hours, and status.

For Estimates only, another sheet should be used to provide prospects and clients with
pre-project estimates. See tgEstimates_eng.xsl.

*******************************************

Optional parameter values (see xsl:param nodes below) :

pHoursStyle:
= 0 to have the style the same as the Title
= 1 for all styles at level N (3+) to be consistent style "numeric"
= 2 for tier 1 the style matches the parent but for all others it's style "numeric"

pExternalStyles:
= 1 to use link to external style sheet
= 0 to use internal style definition
This was done because VS2013 doesn't toggle editing functions for styles inside xsl/xml file.
So during development it's easier to modify styles externally, and then for production to copy
the final version back inline here. This flag just makes it easy to decide which to use.

pCompanyName1, pCompanyName2:
= Company producing the time report.
'1' is generally a short name for an H1 tag.
'2' is generally a longer name for the bottom of the report.
These can be the same.

pEmail:
= Contact email for inquiries

pVersion:
This is the version ID for this XSL which is embedded in the HTML. Should not be changed.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <!--<xsl:strip-space elements="*" />-->
  <xsl:output method="html" indent="yes"/>

  <xsl:param name="pEstimateStyle" select="2" />
  <xsl:param name="pExternalStyles" select="0" />
  <xsl:param name="pCompanyName1" select="'Foo &amp; Bar'" />
  <xsl:param name="pCompanyName2" select="'Your Company'" />
  <xsl:param name="pEmail" select="'YourEmail@YourDomain.tld'" />
  <xsl:param name="pVersion" select="'150218a'" />

  <!-- Primary page generation -->
  <xsl:template match="TODOLIST">
    <xsl:element name="html">
      <xsl:variable name="hdr">
        <xsl:choose>
          <xsl:when test="string-length(@REPORTTITLE) &gt; 0">
            <xsl:value-of select="@REPORTTITLE"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="substring-before(@FILENAME,'.')"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <!-- HTML Header-->
      <xsl:element name="head">
        <xsl:element name="title">
          <xsl:value-of select="concat($hdr,' Project Estimate')" />
        </xsl:element>
        <xsl:text disable-output-escaping="yes">
<![CDATA[<!--
Generated from ToDoList task management software http://www.codeproject.com/Articles/5371/ToDoList
using tg_EstimatesAndActuals XSL v]]></xsl:text>
        <xsl:value-of select="$pVersion" />
        <xsl:text disable-output-escaping="yes">
<![CDATA[-->]]>
		</xsl:text>
        <xsl:choose>
          <xsl:when test="$pExternalStyles='1'">
            <xsl:element name="link">
              <xsl:attribute name="href">myTDLstyles1.css</xsl:attribute>
              <xsl:attribute name="rel">stylesheet</xsl:attribute>
            </xsl:element>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="buildInternalStyles" />
          </xsl:otherwise>
        </xsl:choose>
      </xsl:element>
      <!-- HTML Body-->
      <xsl:element name="body">
        <!-- Page Header -->
        <xsl:element name="div">
          <xsl:attribute name="class">headerSpan</xsl:attribute>
          <xsl:element name="h2">
            <xsl:value-of select="concat($pCompanyName1,' Project Status Summary for ',$hdr)" />
          </xsl:element>
        </xsl:element>

        <!-- Main content - tasks -->
        <xsl:apply-templates select ="TASK"/>

        <!-- Page Footer -->
        <xsl:element name="div">
          <xsl:attribute name="class">footerSpan</xsl:attribute>
          <xsl:element name="h5">
            <xsl:value-of select="$pCompanyName2"/>
            <!-- Text tags here and below are constructed to put BR tag on a separate line in the HTML source -->
            <xsl:text disable-output-escaping="yes">
            <![CDATA[<br/>]]>
            </xsl:text>
            <xsl:value-of select="$pEmail"/>
          </xsl:element>
        </xsl:element>

        <!-- Legend / final notes -->
        <xsl:element name="div">
          <xsl:text disable-output-escaping="yes">
<![CDATA[<p>Please note:<p>
<ul>
<li>Task ID is in brackets"[00]". This number can be used to reference specific items.</li>
<li>Parent tasks are broken down into sub-tasks. Parent tasks show the sum of all child tasks.</li>
<li>Tasks may have been added which were not in the original estimate.</li>
</ul>]]>
          </xsl:text>
        </xsl:element>
      </xsl:element>
    </xsl:element>
  </xsl:template>


  <!-- Main loop processes each task element recursively -->
  <xsl:template match="TASK">
    <xsl:choose>

      <xsl:when test="../../TODOLIST">
        <!--Top level task, generate a new table -->
        <xsl:element name="div">
          <xsl:attribute name="class">taskTier1</xsl:attribute>
          <xsl:element name="table">
            <xsl:attribute name="class">tabletier1</xsl:attribute>

            <!-- Table header shows available columns -->
            <xsl:element name="thead">
              <xsl:element name="tr">
                <xsl:element name="th">
                  <xsl:attribute name="colspan">8</xsl:attribute>
                  <xsl:attribute name="align">left</xsl:attribute>
                  <xsl:value-of select="'Tasks'" />
                </xsl:element>
                <xsl:element name="th">
                  <xsl:attribute name="colspan">8</xsl:attribute>
                  <xsl:attribute name="align">right</xsl:attribute>
                  <xsl:value-of select="'Estimates'" />
                </xsl:element>
                <xsl:element name="th">
                  <xsl:attribute name="align">right</xsl:attribute>
                  <xsl:value-of select="'Actuals'" />
                </xsl:element>
                <xsl:element name="th">
                  <xsl:attribute name="width">20</xsl:attribute>
                </xsl:element>
                <xsl:element name="th">
                  <xsl:attribute name="align">left</xsl:attribute>
                  <xsl:value-of select="'Status'" />
                </xsl:element>
              </xsl:element>
            </xsl:element>

            <!-- Generate the first row, then process sub-tasks -->
            <xsl:call-template name="buildRow" >
              <xsl:with-param name="level" select="1" />
            </xsl:call-template>
            <xsl:if test="child::TASK">
              <!-- Recursively call back on this template to process sub-tasks -->
              <xsl:apply-templates select ="TASK" />
            </xsl:if>

            <!-- End the table -->
          </xsl:element>
          <!-- End the Div and styling surrounding the table -->
        </xsl:element>
      </xsl:when>

      <xsl:otherwise>
        <!-- This is not a level 1 task -->
        <xsl:choose>
          <xsl:when test="../../../TODOLIST">
            <!-- This is a level 2 task, subtask under main task/project -->

            <!-- Build a single row -->
            <xsl:call-template name="buildRow" >
              <xsl:with-param name="level" select="2" />
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <!-- This is any level below level 2 ... grandchildren of top level tasks -->
            <xsl:value-of select="N" />
            <xsl:call-template name="buildRow" >
              <xsl:with-param name="level" select="'N'" />
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>

        <!-- For any level 2+ row, process nested sub-tasks -->
        <xsl:if test="child::TASK">
          <xsl:apply-templates select ="TASK" />
        </xsl:if>
        <!-- Done processing any level 2+ task-->
      </xsl:otherwise>

    </xsl:choose>

    <!-- Done processing any single task -->
  </xsl:template>

  <!-- Add a row to the table - this is a single task of a project -->
  <xsl:template name="buildRow">
    <xsl:param name="level" />
    <xsl:element name="tr">
      <!-- Start row content -->

      <!-- Add empty cells for indentation based on the level of current task/sub-task -->
      <xsl:call-template name="add_table_cells" />
      <!-- Now positioned to correct indentation, generate content -->

      <!-- Front batch of TD cells/columns has Title, Comments, maybe more info -->
      <xsl:call-template name="get_Description" >
        <xsl:with-param name="level" select="$level" />
      </xsl:call-template>

      <!-- Back batch of TD cells has Estimates with indentation corresponding to level -->
      <xsl:call-template name="get_EstimateTime" >
        <xsl:with-param name="level" select="$level" />
      </xsl:call-template>
      <!-- Now follow that cell with empty cells to balance out original indentation -->
      <xsl:call-template name="add_table_cells" />

      <!-- Final columns don't indent -->
      <xsl:call-template name="get_ActualTime" >
        <xsl:with-param name="level" select="$level" />
      </xsl:call-template>

      <!-- Add empty cell to unclutter -->
      <xsl:call-template name="add_a_cell">
        <xsl:with-param name="index" select="number('1')" />
        <xsl:with-param name="startLevel" select="number('1')" />
        <xsl:with-param name="endLevel" select="number('2')" />
      </xsl:call-template>

      <!-- Add Status -->
      <xsl:call-template name="get_Status" >
        <xsl:with-param name="level" select="$level" />
      </xsl:call-template>


      <!-- End row content -->
    </xsl:element>
  </xsl:template>

  <!-- Each row has a Description and Hours. This is the first part.-->
  <xsl:template name="get_Description">
    <xsl:param name="level" />
    <xsl:element name="td">
      <!-- With a max of 9 levels, colspan is calculated. If this is L5 then span = 4. -->
      <xsl:attribute name="colspan">
        <xsl:value-of select="9-count(ancestor::*)" />
      </xsl:attribute>
      <!-- Span for text within the TD, provides text styling versus cell styling -->
      <xsl:element name="span">
        <!-- CSS class for text is calculated based on level -->
        <xsl:call-template name="get_Title_Class">
          <xsl:with-param name="level" select="$level" />
        </xsl:call-template>
        <!-- Now add Title and task ID with same styling -->
        <xsl:value-of select="@TITLE" />
        <xsl:text> </xsl:text>
        <xsl:value-of select="concat('[',@ID,']')" />
      </xsl:element>
      <!-- Still in same TD, add some or all of the task comment -->
      <xsl:call-template name="get_Comment" />
    </xsl:element>
  </xsl:template>

  <!-- Initiate a loop which adds empty cells before the Description cell -->
  <xsl:template name="add_table_cells">
    <xsl:choose>
      <!-- Only start indent when level > 1 -->
      <xsl:when test="count(ancestor::*) &gt; 1">
        <!-- Make initial call to template for cell. It will loop on itself then return here to exit. -->
        <xsl:call-template name="add_a_cell">
          <xsl:with-param name="index" select="number('1')" />
          <xsl:with-param name="startLevel" select="number('1')" />
          <xsl:with-param name="endLevel" select="count(ancestor::*)" />
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Add a single cell before or after other row data -->
  <xsl:template name="add_a_cell">
    <!-- Parameters are passed in to allow a loop 'For index = start To finish' -->
    <xsl:param name="index" />
    <xsl:param name="startLevel" />
    <xsl:param name="endLevel" />
    <!-- Keep going as long as we're not done -->
    <xsl:if test="$index &lt; $endLevel">
      <!-- Generate empty cell, hard-coded spacing (modify as desired) -->
      <xsl:element name="td">
        <xsl:attribute name="width">20</xsl:attribute>
      </xsl:element>
      <!-- Now call back recursively on this template, incrementing the index -->
      <xsl:call-template name="add_a_cell">
        <xsl:with-param name="index" select="$index+1" />
        <xsl:with-param name="startLevel" select="$startLevel" />
        <xsl:with-param name="endLevel" select="$endLevel" />
      </xsl:call-template>
    </xsl:if>
    <!-- Eventually execution unwinds here and eventually exits back to caller -->
  </xsl:template>

  <!-- Add a cell for the estimate -->
  <xsl:template name="get_EstimateTime">
    <xsl:param name="level" />
    <!-- Estimate -->
    <!-- With or without data we need the cell -->
    <xsl:element name="td">
      <xsl:attribute name="colspan">
        <xsl:value-of select="9-count(ancestor::*)" />
      </xsl:attribute>
      <xsl:attribute name="class">numeric</xsl:attribute>
      <!-- If there is an estimate, add it, otherwise hard-code a space character -->
      <xsl:choose>
        <xsl:when test="@CALCTIMEESTIMATE">
          <xsl:element name="span">
            <!-- CSS class for number is calculated based on level -->
            <xsl:call-template name="get_Estimate_Class">
              <xsl:with-param name="level" select="$level" />
            </xsl:call-template>
            <xsl:value-of select="format-number(@CALCTIMEESTIMATE,'0.00')" />
          </xsl:element>
        </xsl:when>
        <xsl:otherwise>
          <!-- No estimate, make sure the cell has empty content for proper cell formatting -->
          <xsl:text disable-output-escaping="yes"><![CDATA[&nbsp;]]></xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:element>
  </xsl:template>

  <!-- Add a cell for the actual time -->
  <xsl:template name="get_ActualTime">
    <xsl:param name="level" />
    <!-- Actual -->
    <!-- With or without data we need the cell -->
    <xsl:element name="td">
      <xsl:attribute name="class">numeric</xsl:attribute>
      <!-- If there is time spent, add it, otherwise hard-code a space character -->
      <xsl:choose>
        <xsl:when test="@CALCTIMESPENT">
          <xsl:element name="span">
            <!-- CSS class for actuals is calculated based on comparison with estimate -->
            <xsl:call-template name="get_Actuals_Class" />
            <xsl:value-of select="format-number(@CALCTIMESPENT,'0.00')" />
          </xsl:element>
        </xsl:when>
        <xsl:otherwise>
          <!-- No time spent, make sure the cell has empty content for proper cell formatting -->
          <xsl:text disable-output-escaping="yes"><![CDATA[&nbsp;]]></xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:element>
  </xsl:template>

  <!-- Add a cell for the Status -->
  <xsl:template name="get_Status">
    <xsl:param name="level" />
    <!-- Status -->
    <!-- With or without data we need the cell -->
    <xsl:element name="td">
      <xsl:attribute name="class">prettyStatus</xsl:attribute>
      <!-- If there is a status, add it, otherwise hard-code a space character -->
      <xsl:choose>
        <xsl:when test="@STATUS">
          <xsl:element name="span">
            <xsl:value-of select="@STATUS" />
          </xsl:element>
        </xsl:when>
        <xsl:otherwise>
          <!-- No status, make sure the cell has empty content for proper cell formatting -->
          <xsl:text disable-output-escaping="yes"><![CDATA[&nbsp;]]></xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:element>
  </xsl:template>

  <!-- This sets the class of an existing tag for Estimated Time. It's assumed that the tag is created just before this is called. -->
  <xsl:template name="get_Estimate_Class">
    <xsl:param name="level" />
    <xsl:choose>
      <!-- Per global parameter, what style should be applied to the number? -->
      <xsl:when test="$pEstimateStyle='0'">
        <!-- The number is styled just like the title -->
        <xsl:call-template name="get_Title_Class">
          <xsl:with-param name="level" select="$level" />
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$pEstimateStyle='1'">
            <!-- All numbers are styled the same -->
            <xsl:attribute name="class">s1TierN numeric</xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="$pEstimateStyle='2'">
                <!-- At level 1 the number is styled with the text, but at 2+ it's styled as all other numbers -->
                <xsl:attribute name="class">
                  <xsl:choose>
                    <xsl:when test="$level='1'">
                      <xsl:value-of select="'s1Tier1 numeric'" />
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="'s1TierN numeric'" />
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                <!-- No styling for this cell, invalid option value. Add other handling above if desired. -->
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- This sets the class of an existing tag for Actual Time. It's assumed that the tag is created just before this is called. -->
  <xsl:template name="get_Actuals_Class">
    <xsl:choose>
      <!-- Individuals might prefer different logic in rounding of numbers.
      This is here because a number like 1.001 is greater than 1.00, and will this show as overEstimate,
      even though that one-thousandth might be disregarded for most purposes. -->
      <xsl:when test="round(@CALCTIMESPENT * 100) &gt; round(@CALCTIMEESTIMATE*100)">
        <xsl:attribute name="class">actual overEstimate</xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="@CALCTIMESPENT &lt; @CALCTIMEESTIMATE">
            <xsl:attribute name="class">actual underEstimate</xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="class">actual equalsEstimate</xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- This sets the class of an existing tag for the Title. It's assumed that the tag is created just before this is called. -->
  <xsl:template name="get_Title_Class">
    <xsl:param name="level" />
    <!-- Begin the attribute, calculate the value -->
    <xsl:attribute name="class">
      <xsl:choose>
        <!-- For L1 and L2, the style will be s1Tier1 or s1Tier2 -->
        <xsl:when test="$level='1' or $level='2'">
          <xsl:value-of select="concat('s1Tier',$level)" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:choose>
            <!-- For any other tier, the style depends on whether this task has sub-tasks -->
            <xsl:when test="child::TASK">
              <!-- Parent tasks should be bolded or otherwise stand out -->
              <xsl:value-of select="'s1TierN s1parent'" />
            </xsl:when>
            <xsl:otherwise>
              <!-- Child tasks that have no children of their own get plain styling -->
              <xsl:value-of select="'s1TierN s1child'" />
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <!-- Comments are in the text cell, along with Title, but with different styling -->
  <xsl:template name="get_Comment">
    <!-- Only process if there are any comments -->
    <xsl:if test="COMMENTS">
      <!-- Variable 'line' will contain the comment and get cut down to just the first line -->
      <xsl:variable name="line">
        <!-- The variable starts empty, is only assigned below -->
        <xsl:choose>
          <!-- If there is a line feed (LF,x0A), just get the text before that character -->
          <xsl:when test="contains(COMMENTS, '&#10;')">
            <xsl:value-of select="substring-before(COMMENTS, '&#10;')" />
          </xsl:when>
          <xsl:otherwise>
            <!-- There is no LF, just assign the entire comment to 'line' -->
            <xsl:value-of select="COMMENTS" />
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <!-- We have the first line, but is it empty? -->
      <xsl:if test="string-length($line) &gt; 0">
        <!-- It's not empty, jump down to next line, still within current cell. -->
        <xsl:text disable-output-escaping="yes">
        <![CDATA[<br/>]]>
        -- </xsl:text>
        <!-- Surround comment sub-string in Span tag with styling -->
        <xsl:element name="span">
          <xsl:attribute name="class">commentsText</xsl:attribute>
          <xsl:value-of select="$line"/>
        </xsl:element>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <!-- Stylesheet used for inline CSS -->
  <xsl:template name="buildInternalStyles">
    <xsl:element name="style">
      <xsl:attribute name="type">text/css</xsl:attribute>
      body {
      font-family: Tahoma, serif;
      margin: 5px 5px 5px 5px;
      background-color: #FFFFFF;
      font-size: 11px;
      }

      h5 {
      font-size: 8px;
      }

      table {
      border-collapse: collapse;
      }

      tr {
      border: solid thin;
      border-color: rgba(77, 74, 74, 0.15);
      }

      td {
      text-align: left;
      vertical-align: top;
      }

      .headerSpan {
      background-color: #C4D7FF;
      border-left: 1px solid #006699;
      border-right: 1px solid #006699;
      border-bottom: 1px solid #006699;
      border-top: 1px solid #006699;
      padding-left: 2px;
      padding-right: 2px;
      font-size: 11px;
      text-align: center;
      }

      .footerSpan {
      background-color: #C4D7FF;
      border-left: 1px solid #006699;
      border-right: 1px solid #006699;
      border-bottom: 1px solid #006699;
      border-top: 1px solid #006699;
      padding-left: 2px;
      padding-right: 2px;
      font-size: 11px;
      text-align: center;
      }

      .taskTier1 {
      background-color: #FFFFFF;
      border-left: 1px solid #006699;
      border-right: 1px solid #006699;
      border-bottom: 1px solid #006699;
      border-top: 1px solid #006699;
      padding-left: 2px;
      padding-right: 2px;
      }

      .tableTier1 {
      padding-top: 5px;
      padding-bottom: 5px;
      padding-left: 5px;
      padding-right: 5px;
      background-color: #FFFFFF;
      border-left: 1px solid #006699;
      border-right: 1px solid #006699;
      border-bottom: 1px solid #006699;
      border-top: 1px solid #006699;
      width:100%
      }

      .prettyPriority {
      width: 20px;
      text-align: center;
      color: black;
      }
      .prettyStatus {
      text-align: left;
      font-size: 12px;
      color: black;
      }

      .s1Tier1 {
      font-size: 14px;
      color: blue;
      }

      .s1Tier2 {
      font-size: 12px;
      color: blue;
      }

      .s1TierN {
      font-size: 10px;
      color: black;
      }

      .s1parent {
      font-weight: bold;
      }

      .s1child {
      /* nothing special about this child yet */
      }

      .commentsText {
      font-size: 10px;
      font-style: italic;
      color: #000000;
      }

      .numeric {
      text-align: right;
      }

      .actual {
      text-align: right;
      font-size: 12px;
      }

      .overEstimate {
      font-weight: bold;
      color: red;
      }
      .underEstimate {
      color: green;
      }
      .equalsEstimate {
      font-weight: bold;
      color: black;
      }

      .priorityNumberStyle {
      font-weight: bold;
      color: black;
      }

      .riskNumberStyle {
      font-weight: bold;
      color: black;
      }

    </xsl:element>
  </xsl:template>


  <!--
	Remove redundant CRLFs (Code not yet implemented, here in anticipation of compression enhancement)
	-->
  <!--<xsl:template match="text()[not(string-length(normalize-space()))]"/>
  <xsl:template match="text()[string-length(normalize-space()) > 0]">
    <xsl:value-of select="translate(.,'&#xA;&#xD;', '  ')"/>
  </xsl:template>-->


</xsl:stylesheet>

