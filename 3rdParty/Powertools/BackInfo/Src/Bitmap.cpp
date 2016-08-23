#include <windows.h>

///////////////////////////////////////////////////////////

// helper function prototypes
static BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, 
                                        WORD wHeight);

static void ShrinkDataInt(BYTE *pInBuff, 
                          WORD wWidth, 
                          WORD wHeight,
                          BYTE *pOutBuff, 
                          WORD wNewWidth, 
                          WORD wNewHeight);

static void EnlargeDataInt(BYTE *pInBuff, 
                           WORD wWidth, 
                           WORD wHeight,
                           BYTE *pOutBuff, 
                           WORD wNewWidth, 
                           WORD wNewHeight);

///////////////////////////////////////////////////////////
// Main resize function

HBITMAP ScaleBitmapInt(
    HBITMAP hBmp, 
    WORD wNewWidth, 
    WORD wNewHeight
)
{
    BITMAP bmp;
    ::GetObject(hBmp, sizeof(BITMAP), &bmp);
    //
    //  check for valid size
    //
    if((bmp.bmWidth > wNewWidth && bmp.bmHeight < wNewHeight) ||
       (bmp.bmWidth < wNewWidth && bmp.bmHeight > wNewHeight))
    {
        return NULL;
    }

    HDC hDC = ::GetDC(NULL);
    BITMAPINFO *pbi = PrepareRGBBitmapInfo((WORD)bmp.bmWidth, (WORD)bmp.bmHeight);
    BYTE *pData = new BYTE[pbi->bmiHeader.biSizeImage];

    ::GetDIBits(hDC, hBmp, 0, bmp.bmHeight, pData, pbi, DIB_RGB_COLORS);

    delete pbi;
    pbi = PrepareRGBBitmapInfo(wNewWidth, wNewHeight);
    BYTE *pData2 = new BYTE[pbi->bmiHeader.biSizeImage];

    if(bmp.bmWidth >= wNewWidth && bmp.bmHeight >= wNewHeight)
    {
        ShrinkDataInt(  pData, 
                        (WORD)bmp.bmWidth, 
                        (WORD)bmp.bmHeight,
                        pData2, 
                        wNewWidth, 
                        wNewHeight);
    }
    else
    {
        EnlargeDataInt( pData, 
                        (WORD)bmp.bmWidth, 
                        (WORD)bmp.bmHeight,
                        pData2, 
                        wNewWidth, 
                        wNewHeight);
    }
    delete pData;

    HBITMAP hResBmp = ::CreateCompatibleBitmap(hDC, wNewWidth, wNewHeight);

    ::SetDIBits(hDC, 
                hResBmp, 
                0, 
                wNewHeight, 
                pData2, 
                pbi, 
                DIB_RGB_COLORS);

    ::ReleaseDC(NULL, hDC);

    delete pbi;
    delete pData2;

    return hResBmp;
}   // ScaleBitmapInt

///////////////////////////////////////////////////////////

BITMAPINFO *
PrepareRGBBitmapInfo(
    WORD wWidth, 
    WORD wHeight
)
{
    BITMAPINFO *pRes = new BITMAPINFO;
    ::ZeroMemory(pRes, sizeof(BITMAPINFO));
    pRes->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pRes->bmiHeader.biWidth = wWidth;
    pRes->bmiHeader.biHeight = wHeight;
    pRes->bmiHeader.biPlanes = 1;
    pRes->bmiHeader.biBitCount = 24;

    pRes->bmiHeader.biSizeImage = ((3 * wWidth + 3) & ~3) * wHeight;

    return pRes;
}   // PrepareRGBBitmapInfo

///////////////////////////////////////////////////////////

static int *
CreateCoeffInt(
    int nLen, 
    int nNewLen, 
    BOOL bShrink
)
{
    int nSum = 0, nSum2;
    int *pRes = new int[2 * nLen];
    int *pCoeff = pRes;
    int nNorm = (bShrink) ? (nNewLen << 12) / nLen : 0x1000;
    int nDenom = (bShrink)? nLen : nNewLen;

    ::ZeroMemory(pRes, 2 * nLen * sizeof(int));
    for(int i = 0; i < nLen; i++, pCoeff += 2)
    {
        nSum2 = nSum + nNewLen;
        if(nSum2 > nLen)
        {
            *pCoeff = ((nLen - nSum) << 12) / nDenom;
            pCoeff[1] = ((nSum2 - nLen) << 12) / nDenom;
            nSum2 -= nLen;
        }
        else
        {
            *pCoeff = nNorm;
            if(nSum2 == nLen)
            {
                pCoeff[1] = -1;
                nSum2 = 0;
            }
        }
        nSum = nSum2;
    }

    return pRes;
}   // CreateCoeffInt

///////////////////////////////////////////////////////////

void ShrinkDataInt(BYTE *pInBuff, 
                   WORD wWidth, 
                   WORD wHeight,
                   BYTE *pOutBuff, 
                   WORD wNewWidth, 
                   WORD wNewHeight)
{
    BYTE  *pLine = pInBuff, *pPix;
    BYTE  *pOutLine = pOutBuff;
    DWORD dwInLn = (3 * wWidth + 3) & ~3;
    DWORD dwOutLn = (3 * wNewWidth + 3) & ~3;
    int   x, y, i, ii;
    BOOL  bCrossRow, bCrossCol;
    int   *pRowCoeff = CreateCoeffInt(wWidth, wNewWidth, TRUE);
    int   *pColCoeff = CreateCoeffInt(wHeight, wNewHeight, TRUE);
    int   *pXCoeff, *pYCoeff = pColCoeff;
    DWORD dwBuffLn = 3 * wNewWidth * sizeof(DWORD);
    DWORD *pdwBuff = new DWORD[6 * wNewWidth];
    DWORD *pdwCurrLn = pdwBuff, 
          *pdwCurrPix, 
          *pdwNextLn = pdwBuff + 3 * wNewWidth;
    DWORD dwTmp, *pdwNextPix;

    ::ZeroMemory(pdwBuff, 2 * dwBuffLn);

    y = 0;
    while(y < wNewHeight)
    {
        pPix = pLine;
        pLine += dwInLn;

        pdwCurrPix = pdwCurrLn;
        pdwNextPix = pdwNextLn;

        x = 0;
        pXCoeff = pRowCoeff;
        bCrossRow = pYCoeff[1] > 0;
        while(x < wNewWidth)
        {
            dwTmp = *pXCoeff * *pYCoeff;
            for(i = 0; i < 3; i++)
            {
                pdwCurrPix[i] += dwTmp * pPix[i];
            }
            bCrossCol = pXCoeff[1] > 0;
            if(bCrossCol)
            {
                dwTmp = pXCoeff[1] * *pYCoeff;
                for(i = 0, ii = 3; i < 3; i++, ii++)
                {
                    pdwCurrPix[ii] += dwTmp * pPix[i];
                }
            }
            if(bCrossRow)
            {
                dwTmp = *pXCoeff * pYCoeff[1];
                for(i = 0; i < 3; i++)
                {
                    pdwNextPix[i] += dwTmp * pPix[i];
                }
                if(bCrossCol)
                {
                    dwTmp = pXCoeff[1] * pYCoeff[1];
                    for(i = 0, ii = 3; i < 3; i++, ii++)
                    {
                        pdwNextPix[ii] += dwTmp * pPix[i];
                    }
                }
            }
            if(pXCoeff[1])
            {
                x++;
                pdwCurrPix += 3;
                pdwNextPix += 3;
            }
            pXCoeff += 2;
            pPix += 3;
        }
        if(pYCoeff[1])
        {
            // set result line
            pdwCurrPix = pdwCurrLn;
            pPix = pOutLine;
            for(i = 3 * wNewWidth; i > 0; i--, pdwCurrPix++, pPix++)
            {
                *pPix = ((LPBYTE)pdwCurrPix)[3];
            }

            // prepare line buffers
            pdwCurrPix = pdwNextLn;
            pdwNextLn = pdwCurrLn;
            pdwCurrLn = pdwCurrPix;
            ::ZeroMemory(pdwNextLn, dwBuffLn);

            y++;
            pOutLine += dwOutLn;
        }
        pYCoeff += 2;
    }

    delete [] pRowCoeff;
    delete [] pColCoeff;
    delete [] pdwBuff;
}   // ShrinkDataInt

///////////////////////////////////////////////////////////

void EnlargeDataInt(BYTE *pInBuff, 
                    WORD wWidth, 
                    WORD wHeight,
                    BYTE *pOutBuff, 
                    WORD wNewWidth, 
                    WORD wNewHeight)
{
    BYTE  *pLine = pInBuff, 
          *pPix = pLine, 
          *pPixOld, 
          *pUpPix, 
          *pUpPixOld;
    BYTE  *pOutLine = pOutBuff, *pOutPix;
    DWORD dwInLn = (3 * wWidth + 3) & ~3;
    DWORD dwOutLn = (3 * wNewWidth + 3) & ~3;
    int   x, y, i;
    BOOL  bCrossRow, bCrossCol;
    int   *pRowCoeff = CreateCoeffInt(wNewWidth, wWidth, FALSE);
    int   *pColCoeff = CreateCoeffInt(wNewHeight, wHeight, FALSE);
    int   *pXCoeff, *pYCoeff = pColCoeff;
    DWORD dwTmp, dwPtTmp[3];

    y = 0;
    while(y < wHeight)
    {
        bCrossRow = pYCoeff[1] > 0;
        x = 0;
        pXCoeff = pRowCoeff;
        pOutPix = pOutLine;
        pOutLine += dwOutLn;
        pUpPix = pLine;
        if(pYCoeff[1])
        {
            y++;
            pLine += dwInLn;
            pPix = pLine;
        }

        while(x < wWidth)
        {
            bCrossCol = pXCoeff[1] > 0;
            pUpPixOld = pUpPix;
            pPixOld = pPix;
            if(pXCoeff[1])
            {
                x++;
                pUpPix += 3;
                pPix += 3;
            }

            dwTmp = *pXCoeff * *pYCoeff;

            for(i = 0; i < 3; i++)
            {
                dwPtTmp[i] = dwTmp * pUpPixOld[i];
            }

            if(bCrossCol)
            {
                dwTmp = pXCoeff[1] * *pYCoeff;
                for(i = 0; i < 3; i++)
                {
                    dwPtTmp[i] += dwTmp * pUpPix[i];
                }
            }

            if(bCrossRow)
            {
                dwTmp = *pXCoeff * pYCoeff[1];
                for(i = 0; i < 3; i++)
                {
                    dwPtTmp[i] += dwTmp * pPixOld[i];
                }
                if(bCrossCol)
                {
                    dwTmp = pXCoeff[1] * pYCoeff[1];
                    for(i = 0; i < 3; i++)
                    {
                        dwPtTmp[i] += dwTmp * pPix[i];
                    }
                }
            }

            for(i = 0; i < 3; i++, pOutPix++)
            {
                *pOutPix = ((LPBYTE)(dwPtTmp + i))[3];
            }
            pXCoeff += 2;
        }
        pYCoeff += 2;
    }

    delete [] pRowCoeff;
    delete [] pColCoeff;
}   // EnlargeDataInt

