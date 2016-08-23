// MergeToDoList.h: interface for the CMergeToDoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MERGETODOLIST_H__7BFD737D_FF21_46B9_9378_D14BDA114022__INCLUDED_)
#define AFX_MERGETODOLIST_H__7BFD737D_FF21_46B9_9378_D14BDA114022__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

enum TDL_MERGEBY
{
	TDLM_BYTITLE,
	TDLM_BYID
};

enum TDL_MERGEHOW
{
	TDLM_LEAVE,
	TDLM_MOVE,
};

class CXmlItem;

class CTDCMergeTasklist  
{
public:
	CTDCMergeTasklist(TDL_MERGEBY nBy, TDL_MERGEHOW nHow = TDLM_MOVE);
	virtual ~CTDCMergeTasklist();

	int Merge(LPCTSTR szSrcPath, LPCTSTR szDestPath);
	int Merge(const CXmlItem* pXISrc, CXmlItem* pXIDest);
	int Merge(const CXmlItem* pXISrc, LPCTSTR szDestPath);
	int Merge(LPCTSTR szSrcPath, CXmlItem* pXIDest);

	DWORD GetNextID() { return m_dwNextID; }

protected:
	CXmlItem* m_pXIDestRoot;
	TDL_MERGEBY m_nMergeBy;
	TDL_MERGEHOW m_nMergeHow;
	DWORD m_dwNextID, m_dwOrgNextID;
	CMap<DWORD, DWORD, CXmlItem*, CXmlItem*&> m_mapID2Item;
	
protected:
	int MergeTasks(const CXmlItem* pXISrc, CXmlItem* pXIDest);
	BOOL MergeAttributes(const CXmlItem* pXISrc, CXmlItem* pXIDest);
	
	int MergeTasksByTitle(const CXmlItem* pXISrc, CXmlItem* pXIDest);
	int MergeTasksByID(const CXmlItem* pXISrc, CXmlItem* pXIDest);
	int MergeTaskByID(const CXmlItem* pXISrc, CXmlItem* pXIDestParent);

	CXmlItem* FindDestTask(DWORD dwID);
	void BuildDestLookupMap();
	void AddDestTaskToLookupMap(CXmlItem* pXIDest, BOOL bAndSiblings);
};

#endif // !defined(AFX_MERGETODOLIST_H__7BFD737D_FF21_46B9_9378_D14BDA114022__INCLUDED_)
