#include "stdafx.h"

void GetImageParament(CImage *pImg, struct IMAGEPARAMENT *ppImgParam)
{
	if (pImg->IsNull()) return;

	ppImgParam->nWidth = pImg->GetWidth();
	ppImgParam->nHeight = pImg->GetHeight();
	ppImgParam->nBitCount = pImg->GetBPP();
	ppImgParam->nBytesPerLine = (pImg->GetWidth()*pImg->GetBPP() + 31) / 32 * 4;
	ppImgParam->nBytesPerPixel = pImg->GetBPP() / 8;
	if (pImg->GetBPP() <= 8)
		ppImgParam->nNumColors = 1 << pImg->GetBPP();
	else
		ppImgParam->nNumColors = 0;
	ppImgParam->nSize = ppImgParam->nBytesPerLine*ppImgParam->nHeight;
}


void ImageCopy(CImage *pImgn, CImage *pImgm) //ImageCopy(貼上,被複制)
{
	struct	IMAGEPARAMENT P;
	RGBQUAD	ColorTab[256];
	CDC		*pOrigDC, *pDC;

	GetImageParament(pImgm, &P);

	if (!pImgn->IsNull())
		pImgn->Destroy();
	pImgn->Create(P.nWidth, P.nHeight, P.nBitCount, 0);

	if (P.nNumColors > 0)
	{
		pImgm->GetColorTable(0, P.nNumColors, ColorTab);
		pImgn->SetColorTable(0, P.nNumColors, ColorTab);
	}

	pOrigDC = CDC::FromHandle(pImgm->GetDC());
	pDC = CDC::FromHandle(pImgn->GetDC());
	//Returns a pointer to a CDC object when given a handle to a device context. 
	//If a CDC object is not attached to the handle, a temporary CDC object is created and attached.
	pDC->BitBlt(0, 0, P.nWidth, P.nHeight, pOrigDC, 0, 0, SRCCOPY); //Copies a bitmap from a specified device context.
	pImgm->ReleaseDC();
	pImgn->ReleaseDC();
	//CImage::ReleaseDC
	//Releases the device context that was retrieved with CImage::GetDC.
}

bool LoadImg(CImage *img, CString *title)
{
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	if (FAILED(img->GetExporterFilterString(strFilter, aguidFileTypes))) {
		return false;
	}
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);

	if (dlg.DoModal() == IDOK) {
		img->Destroy();
		
		CString path01;		path01 = "/";
		if (FAILED(img->Load(dlg.GetFolderPath() + path01 + dlg.GetFileName()))) {
			CString fmt;
			fmt = "Load image failed!";
			::AfxMessageBox(fmt);
			return false;
		}
	}
	else
		return false;

	if (title != NULL) {
		*title = dlg.GetFileName();
	}
	return true;
}

bool SaveImg(CImage *img)
{
	if (img->IsNull()) {
		CString fmt;
		fmt = "CImage file not found!";
		::AfxMessageBox(fmt);
		return false;
	}
	CString strFilter;
	strFilter = "BMP文件|*.bmp|JPEG圖像文件|*.jpg|GIF圖像文件|*.gif|PNG圖像文件|*.png||";
	CFileDialog dlg(FALSE, NULL, NULL, NULL, strFilter);
	if (IDOK != dlg.DoModal()) {
		return false;
	}
	// 如果用戶沒有制定文件副檔名，則為其添加一個 
	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	//	if (dlg.m_ofn.nFileExtension == 0) { 
	switch (dlg.m_ofn.nFilterIndex)
	{
	case 1:
		strExtension = "bmp"; break;
	case 2:
		strExtension = "jpg"; break;
	case 3:
		strExtension = "gif"; break;
	case 4:
		strExtension = "png"; break;
	default:
		break;
	}
	strFileName = strFileName + '.' + strExtension;
	//	} 
		// 保存圖像 	
	HRESULT hResult = img->Save(strFileName);
	if (FAILED(hResult)) {
		CString fmt;
		fmt = "Save image failed!";
		::AfxMessageBox(fmt);
		return false;
	}
	return true;
}


BYTE** Create2DList(CImage *pImg)//對灰階有用，但rgb影像的話可能會有問題
{
	struct		IMAGEPARAMENT P;
	int			i;
	BYTE		**list;

	GetImageParament(pImg, &P);
	list = (BYTE**)malloc(P.nHeight * sizeof(BYTE*));
	for (i = 0; i < P.nHeight; i++)
		list[i] = (BYTE*)pImg->GetPixelAddress(0, i);

	return(list);
}
void Release2DList(BYTE** list)
{
	free(list);
}


void ColorToGray(CImage *pImgn, CImage *pImgm)
{
	int			i, x, y;
	int			gray[256];
	BYTE		*cbuf, *gbuf;
	RGBQUAD		ColorTab[256];
	CImage		gpImg;
	CString		str1;
	struct		IMAGEPARAMENT P;

	GetImageParament(pImgm, &P);
	if (P.nBitCount < 8) return;

	gpImg.Create(P.nWidth, P.nHeight, 8, 0);
	for (i = 0; i < 256; i++)
	{
		ColorTab[i].rgbBlue = ColorTab[i].rgbGreen =
			ColorTab[i].rgbRed = i;
	}
	gpImg.SetColorTable(0, 256, ColorTab);

	if (pImgm->GetBPP() <= 8) {
		pImgm->GetColorTable(0, P.nNumColors, ColorTab);
		for (i = 0; i < P.nNumColors; i++)
		{
			gray[i] = (int)(0.11*ColorTab[i].rgbBlue +
				0.59*ColorTab[i].rgbGreen + 0.30*ColorTab[i].rgbRed);
		}
		for (y = 0; y < P.nHeight; y++) {
			cbuf = (BYTE*)pImgm->GetPixelAddress(0, y);
			gbuf = (BYTE*)gpImg.GetPixelAddress(0, y);
			for (x = 0; x < P.nWidth; x++)
				gbuf[x] = (BYTE)gray[cbuf[x]];
		}
	}
	else {
		for (y = 0; y < P.nHeight; y++) {
			cbuf = (BYTE*)pImgm->GetPixelAddress(0, y);
			gbuf = (BYTE*)gpImg.GetPixelAddress(0, y);
			for (x = 0, i = 0; x < P.nWidth; x++, i += P.nBytesPerPixel) {
				gbuf[x] = (BYTE)(0.11*cbuf[i] + 0.59*cbuf[i + 1] + 0.30*cbuf[i + 2]);
			}
		}
	}

	ImageCopy(pImgn, &gpImg);
	gpImg.Destroy();
}

void CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth) {
	if (pImage->IsDIBSection()) {
		// 取得 pImage 的 DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC()); // Image 因為有自己的 DC, 所以必須使用 FromHandle 取得對應的 DC


		CBitmap *bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);

		// 建立新的 CImage
		ResultImage->Destroy();//若沒有先destory這個的話，萬一裡面有東西就會出錯

		if (bmpInfo.bmBitsPixel == 8)
			ResultImage->Create(StretchWidth, StretchHeight, 24);//沒這樣的話灰階文件會錯
		else
			ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);

		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());

		// 當 Destination 比較小的時候, 會根據 Destination DC 上的 Stretch Blt mode 決定是否要保留被刪除點的資訊
		ResultImageDC->SetStretchBltMode(HALFTONE); // 使用最高品質的方式
		::SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL); // 調整 Brush 的起點

		// 把 pImage 畫到 ResultImage 上面
		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		// pImage->Draw(*ResultImageDC,0,0,StretchWidth,StretchHeight,0,0,pImage->GetWidth(),pImage->GetHeight());

		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}
void Zoom(CImage *Img, int SetH, int SetW)
{
	struct	IMAGEPARAMENT P;
	RGBQUAD	ColorTab[256];
	CDC		*pDC;

	GetImageParament(Img, &P);
	/*
	   if (!pImgn->IsNull())
		   pImgn->Destroy();
	   pImgn->Create(P.nWidth,P.nHeight,P.nBitCount,0);
	   */
	if (P.nNumColors > 0)
	{
		Img->GetColorTable(0, P.nNumColors, ColorTab);
		//pImgn->SetColorTable(0,P.nNumColors,ColorTab);   
	}

	pDC = CDC::FromHandle(Img->GetDC());
	//---------------------------------------------
	CImage newImg;
	newImg.Destroy();
	newImg.Create(SetW, SetH, 8, 0);
	//gpImg.Create(P.nWidth,P.nHeight,8,0);
	RGBQUAD     ColorTab2[256];
	for (int i = 0; i < 256; i++) {
		ColorTab2[i].rgbBlue = ColorTab2[i].rgbGreen =
			ColorTab2[i].rgbRed = i;
	}
	newImg.SetColorTable(0, 256, ColorTab2);
	//-----------------------------------------------------
	struct	IMAGEPARAMENT P2;
	RGBQUAD	ColorTab3[256];
	CDC		*pDC2;

	GetImageParament(&newImg, &P2);
	/*
	   if (!pImgn->IsNull())
		   pImgn->Destroy();
	   pImgn->Create(P.nWidth,P.nHeight,P.nBitCount,0);
	   */
	if (P2.nNumColors > 0)
	{
		newImg.GetColorTable(0, P2.nNumColors, ColorTab3);
		//pImgn->SetColorTable(0,P.nNumColors,ColorTab);   
	}

	pDC2 = CDC::FromHandle(newImg.GetDC());

	//-----------------------------------------------------
	pDC2->StretchBlt(0, 0, SetW, SetH
		, pDC, 0, 0, Img->GetWidth(), Img->GetHeight(), SRCCOPY);

	Img->ReleaseDC();
	newImg.ReleaseDC();

	ImageCopy(Img, &newImg);

}