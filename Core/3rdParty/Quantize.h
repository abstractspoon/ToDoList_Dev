// Quantize.h
//
// CQuantizer Class : MSJ : Wicked Code 1997 : By Jeff Prosise
//
#ifndef __QUANTIZE_H__
#define __QUANTIZE_H__

typedef struct _NODE
{
    BOOL bIsLeaf;               // TRUE if node has no children
    UINT nPixelCount;           // Number of pixels represented by this leaf
    UINT nRedSum;               // Sum of red components
    UINT nGreenSum;             // Sum of green components
    UINT nBlueSum;              // Sum of blue components
    struct _NODE* pChild[8];    // Pointers to child nodes
    struct _NODE* pNext;        // Pointer to next reducible node
} NODE;

typedef NODE* PNODE;

class CQuantizer
{
protected:
    PNODE m_pTree;
    UINT  m_nLeafCount;
    PNODE m_pReducibleNodes[9];
    UINT  m_nMaxColors;
    UINT  m_nColorBits;

public:
    CQuantizer(UINT nMaxColors, UINT nColorBits);
    virtual ~CQuantizer();
	BOOL ProcessImage(HANDLE hImage);
	UINT GetColorCount();
    void GetColorTable(RGBQUAD* prgb);

protected:
    int GetLeftShiftCount(DWORD dwVal);
    int GetRightShiftCount(DWORD dwVal);
    void AddColor(PNODE* ppNode, BYTE r, BYTE g, BYTE b, UINT nColorBits,
        UINT nLevel, UINT* pLeafCount, PNODE* pReducibleNodes);
    PNODE CreateNode(UINT nLevel, UINT nColorBits, UINT* pLeafCount,
        PNODE* pReducibleNodes);
    void ReduceTree(UINT nColorBits, UINT* pLeafCount,
        PNODE* pReducibleNodes);
    void DeleteTree(PNODE* ppNode);
    void GetPaletteColors (PNODE pTree, RGBQUAD* prgb, UINT* pIndex);
};

#endif // __QUANTIZE_H__
