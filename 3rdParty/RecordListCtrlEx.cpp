// ==========================================================================
// CRecordListCtrl.cpp
// 
// Author : Vitali Halershtein
//          vitali2001by@yahoo.co.uk
//
// Last Modified : 7.09.2003
// by            : Vitali Halershtein
// Copyright  :
// This software is released into the public domain. You are free to use it in any way you like.
// You should save Author text in the header files. If you modify it or extend it, please consider 
// posting new code here for everyone to share. This software is provided "as is" with no 
// expressed or implied warranty. I accept no liability for any damage or loss of business
// that this software may cause. 
// ==========================================================================

#include "stdafx.h"
//#include "resource.h"
#include "RecordListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRecordListCtrl, CListCtrl/*CListCtrlEx*/)
BEGIN_MESSAGE_MAP(CRecordListCtrl, CListCtrl/*CListCtrlEx*/)
	//{{AFX_MSG_MAP(CRecordListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CRecordListCtrl::CRecordListCtrl() : CListCtrl/*CListCtrlEx*/()
{
}

CRecordListCtrl::~CRecordListCtrl()
{
}

int CRecordListCtrl::LoadListCtrl(CString aDSN, CString aSQL)
{
	if ( !IsWindow(m_hWnd) ) return -1;
	
	DeleteAllItems ();
	while (GetHeaderCtrl()->GetItemCount() > 0) DeleteColumn(0);
	
	TRY
    {
		CDatabase db;

		db.OpenEx(aDSN, CDatabase::openReadOnly | CDatabase::noOdbcDialog );
		
		CRecordset rs(&db);
		
// 		aSQL = ParseSQL(aSQL);

		if (aSQL.IsEmpty())
			aSQL = rs.GetDefaultSQL();
		
		if ( rs.Open(CRecordset::snapshot, aSQL) )
		{
			
			int iFieldCount = rs.GetODBCFieldCount ();
			
			if (iFieldCount == 0)
			{
				rs.Close();
				db.Close();
				return -1;
			}
			
			CODBCFieldInfo fInfo;
			
			for(int i=0; i < iFieldCount;i++)
			{
				rs.GetODBCFieldInfo (i, fInfo);
				InsertColumn(i, fInfo.m_strName , LVCFMT_LEFT, 100, i );
			}
			
			if ((!rs.IsEOF()) || (!rs.IsBOF())) rs.MoveFirst();
			
			CString m_fValue;
			while ( !rs.IsEOF() )
			{
				rs.GetFieldValue((short)0, m_fValue);
				int item = InsertItem(GetItemCount(), m_fValue );
				
				int width = GetStringWidth( m_fValue );
				if(width > GetColumnWidth(0)) SetColumnWidth(0, width+30);
				
				for(int i=1; i < iFieldCount; i++)
				{
					rs.GetFieldValue(i, m_fValue);
					int pos = m_fValue.Find("00:00:00", 0);
					if( pos != -1 )
					{
						m_fValue.Delete(pos, m_fValue.GetLength() - pos);
						
						CString tmp, tmp1;
						pos = m_fValue.Find("-", 0);
						if( pos != -1 )
						{
							tmp = m_fValue.Mid(0, 4);
							m_fValue.Delete(0, pos+1);
						}
						pos = m_fValue.Find("-", 0);
						if( pos != -1 )
						{
							tmp1 = m_fValue.Mid(0, 2);
							m_fValue.Delete(0, pos+1);
						}
						m_fValue.TrimRight();
						m_fValue = m_fValue + "-" + tmp1 + "-" + tmp;
					}
					SetItemText(item, i, (char*)(LPCTSTR)m_fValue);
					
					width = GetStringWidth( m_fValue );
					if(width > GetColumnWidth(i))
						SetColumnWidth(i, width+30);
				}
				rs.MoveNext();
			}
			
			rs.Close();
		}
		db.Close();
		
		Invalidate();
    }
	CATCH(CDBException, pEx)
    {
		AfxMessageBox(pEx->m_strError);
		return -1;
    }
	END_CATCH

	return GetItemCount();
}


BOOL CRecordListCtrl::Create(CRect& rc, CWnd *pParentWnd, UINT nID)
{
	return CListCtrl::Create (WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, rc, pParentWnd, nID);
}

/*
CString CRecordListCtrl::ParseSQL(CString aSQL)
{
	CString lSQL = aSQL;
	CString alias;
	CString sField;
	CString sRes;
	// lSQL.MakeUpper ();
	// 
	int iPos = lSQL.Find(" from ", 0);
	if (iPos == -1) 
		iPos = lSQL.Find(" FROM ", 0);
	lSQL = lSQL.Left(iPos);
	lSQL = lSQL.Right(lSQL.GetLength()-sizeof(char)*6);
	
	lSQL.TrimRight ();
	lSQL.TrimLeft ();
	// 
	int iPosComma = lSQL.Find(",", 0);
	int iPosAs = -1;
	
	if ((lSQL.GetLength()>0) && (iPosComma == -1))
		iPosComma = lSQL.GetLength ();
	// 
	while ( iPosComma > -1 )
    {
		//
		sField = lSQL.Mid(0,iPosComma);
		//
		iPosAs = sField.Find(" as ", 0);
		if (iPosAs == -1)
			iPosAs = sField.Find(" AS ", 0);
		//
		if (iPosAs > -1)
		{
			//
			alias = sField.Mid(iPosAs+sizeof(char)*4, sField.GetLength());
			//
			alias.TrimRight(']');
			alias.TrimLeft('[');
			//
			sField.Delete(iPosAs,sField.GetLength()-iPosAs);
		}
		// 
		else alias = sField;
		//
		m_arAlias.Add (alias); 
		//
		sRes = sRes + sField + ", ";
		//
		lSQL.Delete(0, iPosComma+1);
		lSQL.TrimLeft ();
		iPosComma = lSQL.Find(",", 0);
		//
		if ((lSQL.GetLength()>0) && (iPosComma == -1))
			iPosComma = lSQL.GetLength ();
    }
	
	sRes.TrimRight(", ");
	sRes = "SELECT " + sRes;
	lSQL = aSQL;
	lSQL.Delete(0, iPos);
	lSQL = sRes + lSQL;
	return lSQL;
}

*/
