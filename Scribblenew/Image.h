#ifndef IMAHE_H
#define IMAGE_H


struct IMAGEPARAMENT {
	int		nWidth;
	int		nHeight;
	int		nBitCount;
	int		nBytesPerLine;
	int		nBytesPerPixel;
	int		nNumColors;
	int		nSize;
};

void  ImageCopy(CImage *pImgn, CImage *pImgm);//ImageCopy
void  GetImageParament(CImage *pImg, struct IMAGEPARAMENT *ppImgParam);

bool  LoadImg(CImage *img, CString *title = NULL);
bool  SaveImg(CImage *img);
BYTE** Create2DList(CImage *pImg);
void  Release2DList(BYTE** list);

void  ColorToGray(CImage *pImgn, CImage *pImgm);
void  CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);
void Zoom(CImage *Img, int SetH, int SetW);


#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

#endif

