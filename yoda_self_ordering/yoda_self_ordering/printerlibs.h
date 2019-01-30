#ifndef PRINTERLIBS_H
#define PRINTERLIBS_H

#ifdef PRINTERLIBS_EXPORTS
#define PRINTERLIBS_API __declspec(dllexport)
#else
#define PRINTERLIBS_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define HORIZONTALALIGNMENT_LEFT		-1
#define HORIZONTALALIGNMENT_CENTER		-2
#define HORIZONTALALIGNMENT_RIGHT		-3
#define ALIGN_HLEFT						HORIZONTALALIGNMENT_LEFT
#define ALIGN_HCENTER					HORIZONTALALIGNMENT_CENTER
#define ALIGN_HRIGHT					HORIZONTALALIGNMENT_RIGHT

#define POS_BARCODE_TYPE_UPCA		0x41
#define POS_BARCODE_TYPE_UPCE		0x42
#define POS_BARCODE_TYPE_EAN13		0x43
#define POS_BARCODE_TYPE_EAN8		0x44
#define POS_BARCODE_TYPE_CODE39		0x45
#define POS_BARCODE_TYPE_ITF		0x46
#define POS_BARCODE_TYPE_CODABAR	0x47
#define POS_BARCODE_TYPE_CODE93		0x48
#define POS_BARCODE_TYPE_CODE128	0x49

#define ENCODING_GBK		0
#define ENCODING_UTF8		1
#define ENCODING_BIG5		3
#define ENCODING_SHIFTJIS	4
#define ENCODING_EUCKR		5

#define BINARYALGORITHM_DITHER		0
#define BINARYALGORITHM_THRESHOLD	1

#define COMPRESSMETHOD_NONE			0
#define COMPRESSMETHOD_COMPRESS		1

/**端口函数****************************************************************************/
// 返回端口句柄。非零表示打开成功，零表示打开失败。
// 波特率
// 校验 /* 0-4=None,Odd,Even,Mark,Space    */
PRINTERLIBS_API int Port_OpenCom(char * pName, int dwBaudrate, int dwParity);
PRINTERLIBS_API int Port_OpenTcp(char * szIp, unsigned short nPort);
PRINTERLIBS_API int Port_OpenUsb(char * pName);
PRINTERLIBS_API int Port_OpenLpt(char * pName);
PRINTERLIBS_API int Port_OpenPrn(char * pName);
// 返回写入的字节数，返回-1表示写入失败
PRINTERLIBS_API int Port_Write(int handle, const void *buffer, unsigned int count, unsigned int timeout);
// 返回读取的字节数，返回-1表示读取失败
PRINTERLIBS_API int Port_Read(int handle, void *buffer, unsigned int count, unsigned int timeout);
PRINTERLIBS_API void Port_Close(int handle);
PRINTERLIBS_API void Port_EnumCom(char * pBuf, int cbBuf, int * pcbNeeded, int * pcnReturned);
PRINTERLIBS_API void Port_EnumLpt(char * pBuf, int cbBuf, int * pcbNeeded, int * pcnReturned);
PRINTERLIBS_API void Port_EnumUSB(char * pBuf, int cbBuf, int * pcbNeeded, int * pcnReturned);
PRINTERLIBS_API void Port_EnumPrn(char * pBuf, int cbBuf, int * pcbNeeded, int * pcnReturned);
PRINTERLIBS_API void Port_EnumNet(char * pBuf, int cbBuf, int * pcbNeeded, int * pcnReturned);

/**页模式打印函数****************************************************************************/
PRINTERLIBS_API bool PAGE_PageEnter(int handle);
PRINTERLIBS_API bool PAGE_PagePrint(int handle);
PRINTERLIBS_API bool PAGE_PageExit(int handle);
PRINTERLIBS_API bool PAGE_SetPrintArea(int handle, int left, int top, int right, int bottom, int direction);
PRINTERLIBS_API bool PAGE_DrawString(int handle, char *pszString, int x, int y, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle);
PRINTERLIBS_API bool PAGE_DrawSString(int handle, char *pszString, int x, int y, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle, int nCharacterSet, int nCharacterCodepage);
PRINTERLIBS_API bool PAGE_DrawWString(int handle, wchar_t *pszString, int x, int y, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle, int nEncoding);
PRINTERLIBS_API bool PAGE_DrawUTF8String(int handle, wchar_t *pszString, int x, int y, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle);
PRINTERLIBS_API bool PAGE_DrawRect(int handle, int x, int y, int nWidth, int nHeight, int nColor);
PRINTERLIBS_API bool PAGE_DrawBarcode(int handle, char *pszBarcodeContent, int x, int y, int nBarcodeUnitWidth, int nBarcodeHeight, int nHriFontType, int nHriFontPosition, int nBarcodeType);
PRINTERLIBS_API bool PAGE_DrawQRCode(int handle, char *pszContent, int x, int y, int nQRCodeUnitWidth, int nVersion, int nEcLevel);
PRINTERLIBS_API bool PAGE_DrawImage(int handle, bool *img, int x, int y, int imgw, int imgh);
PRINTERLIBS_API bool PAGE_DrawBitmap(int handle, char *szFileName, int x, int y, int dstWidth, int dstHeight, int nBinaryAlgorithm);


/**热敏打印函数****************************************************************************/
// 打印
PRINTERLIBS_API bool POS_PrintString(int handle, char *pszString, int x, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle); // 打印文本
PRINTERLIBS_API bool POS_PrintSString(int handle, char *pszString, int x, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle, int nCharacterSet, int nCharacterCodepage); // 打印文本
PRINTERLIBS_API bool POS_PrintWString(int handle, wchar_t * pszString, int x, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle, int nEncoding); // 打印文本
PRINTERLIBS_API bool POS_PrintUTF8String(int handle, wchar_t * pszString, int x, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle); // 打印文本
PRINTERLIBS_API bool POS_TextOut(int handle, wchar_t * pszString, int x, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle, int nEncoding); // 和POS_PrintWString相同
PRINTERLIBS_API bool POS_SetBarcode(int handle, char * pszBarcodeContent, int x, int nBarcodeUnitWidth, int nBarcodeHeight, int nHriFontType, int nHriFontPosition, int nBarcodeType); // 打印条码
PRINTERLIBS_API bool POS_SetQRCode(int handle, char * pszContent, int x, int nQRCodeUnitWidth, int nVersion, int nEcLevel);	//打印QR码
PRINTERLIBS_API bool POS_PrintRasterImage(int handle, const unsigned char * img, int x, int img_w_stride, int imgw, int imgh, int nCompressMethod);
PRINTERLIBS_API bool POS_PrintImage(int handle, bool * img, int x, int imgw, int imgh, int nCompressMethod);	// 打印图片
PRINTERLIBS_API bool POS_PrintPicture(int handle, char * szFileName, int x, int dstw, int dsth, int nBinaryAlgorithm, int nCompressMethod); // 打印bmp图片
PRINTERLIBS_API bool POS_PrintImageInMemory(int handle, unsigned char *buffer, int length, int x, int dstw, int dsth, int nBinaryAlgorithm, int nCompressMethod);
PRINTERLIBS_API bool POS_SelfTest(int handle); // 打印自测页

// 进纸
PRINTERLIBS_API bool POS_FeedLine(int handle);
PRINTERLIBS_API bool POS_FeedNLine(int handle, int nLine);
PRINTERLIBS_API bool POS_FeedNDot(int handle, int nDot);

// 查询
PRINTERLIBS_API bool POS_QueryStatus(int handle, int type, char * status, unsigned int timeout); // 查询打印机状态
PRINTERLIBS_API bool POS_RTQueryStatus(int handle, int type, char * status, unsigned int timeout);	// 实时查询打印机状态
PRINTERLIBS_API bool POS_TicketSucceed(int handle, int dwSendIndex, unsigned int timeout); // 单据打印结果查询

// 设置
PRINTERLIBS_API bool POS_SetMotionUnit(int handle, int nHorizontal, int nVertical);
PRINTERLIBS_API bool POS_SetLineHeight(int handle, int nDistance);	// 设置行高
PRINTERLIBS_API bool POS_SetRightSpacing(int handle, int nDistance); // 设置字符右间距

                                               // 其他
PRINTERLIBS_API bool POS_Reset(int handle);	// 软件复位
PRINTERLIBS_API bool POS_KickOutDrawer(int handle, int nID, int nHighLevelTime, int nLowLevelTime); // 开钱箱
PRINTERLIBS_API bool POS_FeedAndCutPaper(int handle); // 进纸到切刀位置并切纸
PRINTERLIBS_API bool POS_FullCutPaper(int handle); // 直接全切
PRINTERLIBS_API bool POS_HalfCutPaper(int handle); // 直接半切
PRINTERLIBS_API bool POS_Beep(int handle, int nBeepCount, int nBeepMillis); // 蜂鸣器鸣叫 nBeepCount 鸣叫次数 nBeepMillis 每次鸣叫的时间 = 100 * nBeemMillis ms


/**标签打印函数****************************************************************************/
PRINTERLIBS_API bool LABEL_PageBegin(int handle, int startx, int starty, int width, int height, int rotate);
PRINTERLIBS_API bool LABEL_PageEnd(int handle);
PRINTERLIBS_API bool LABEL_PagePrint(int handle, int num);

PRINTERLIBS_API bool LABEL_PageFeed(int handle);
PRINTERLIBS_API bool LABEL_PageCalibrate(int handle);

PRINTERLIBS_API bool LABEL_DrawLine(int handle, int startx, int starty, int endx, int endy, int width, int color);
PRINTERLIBS_API bool LABEL_DrawBox(int handle, int left, int top, int right, int bottom, int borderwidth, int bordercolor);
PRINTERLIBS_API bool LABEL_DrawPlainText(int handle, int startx, int starty, int font, int style, const char *str);
PRINTERLIBS_API bool LABEL_DrawRectangel(int handle, int left, int top, int right, int bottom, int color);
PRINTERLIBS_API bool LABEL_DrawBarcode(int handle, int startx, int starty, int type, int height, int unitwidth, int rotate, const char *str);
PRINTERLIBS_API bool LABEL_DrawQRCode(int handle, int startx, int starty, int version, int ecc, int unitwidth, int rotate, const char *str);
PRINTERLIBS_API bool LABEL_DrawPDF417(int handle, int startx, int starty, int colnum, int lwratio, int ecc, int unitwidth, int rotate, const char *str);
PRINTERLIBS_API bool LABEL_DrawImage(int handle, int startx, int starty, int width, int height, int style, const bool *img);
PRINTERLIBS_API bool LABEL_DrawBitmap(int handle, int startx, int starty, int dstw, int dsth, int style, const char *pszFile, int nBinaryAlgorithm);

#ifdef __cplusplus
}
#endif

#endif // PRINTERLIBS_H
