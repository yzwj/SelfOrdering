#include "stdafx.h"
#pragma once

class CQrcodeBitBuffer
{
public:
	void put(BOOL bit);
	void put(int num, int length);
	BOOL get(int index);
	CString toString();
	int getLengthInBits();
	BYTE* getBuffer();
	CQrcodeBitBuffer();
	virtual ~CQrcodeBitBuffer();
private:
	BYTE *buffer;
	int buffer_len;
	int length;
	int inclements;
};

class CQrcodePolynomial
{
public:
	CQrcodePolynomial mod(CQrcodePolynomial &e);
	CQrcodePolynomial multiply(CQrcodePolynomial &e);
	CString toLogString();
	CString toString();
	int getLength();
	int get(int idx);
	//
	CQrcodePolynomial(CUIntArray& nums, int shift);
	CQrcodePolynomial(const CQrcodePolynomial &p);
	CQrcodePolynomial& operator=(const CQrcodePolynomial &p);

	virtual ~CQrcodePolynomial();

	CUIntArray m_nums;
};

class CQrcodeUtil
{
public:
	static int unsignedRightShift(int v, int n);
	static int getBCHTypeNumber(int data);
	static int getBCHTypeInfo(int data);
	static int getBCHDigit(int data);
	static BOOL isHanZi(CString s);
	static BOOL isAlphaNum(CString s);
	static BOOL isNumber(CString s);
	static int getMode(CString s);
	static BOOL getMask(int maskPattern, int i, int j);
	static CQrcodePolynomial getErrorCorrectPolynomial(int errorCorrectLength);
	static void getPatternPosition(int typeNumber, CUIntArray &p);
	static int getMaxLength(int typeNumber, int mode, int errorCorrectLevel);

	CQrcodeUtil();
	virtual ~CQrcodeUtil();

};



class CQrcodeMath
{
public:
	int gexp(int n);
	int glog(int n);

	static CQrcodeMath * getInstance();
	
	CQrcodeMath();
	virtual ~CQrcodeMath();


private:
	CUIntArray EXP_TABLE;
	CUIntArray LOG_TABLE;
};


class CQrcodeData : public CObject
{
public:
	int getLengthInBits(int type);
	virtual void write(CQrcodeBitBuffer &buffer);
	virtual int getLength();

	int getMode();
	CQrcodeData(int mode, CString data);
	virtual ~CQrcodeData();

	CString data;

private:
	int mode;
};

class CQrcodeNumber : public CQrcodeData
{
public:
	virtual void write(CQrcodeBitBuffer &buffer);
	int parseInt(CString s);
	CQrcodeNumber(CString data);
	virtual ~CQrcodeNumber();

};




class CQrcodeAlphaNum : public CQrcodeData
{
public:
	int getCode(char c);
	virtual void write(CQrcodeBitBuffer &buffer);
	CQrcodeAlphaNum(CString data);
	virtual ~CQrcodeAlphaNum();

};

class CQrcodeHanzi : public CQrcodeData
{
public:
	virtual void write(CQrcodeBitBuffer &buffer);
	virtual int getLength();
	CQrcodeHanzi(CString data);
	virtual ~CQrcodeHanzi();

};



class CQrcode8BitByte : public CQrcodeData
{
public:
	virtual void write(CQrcodeBitBuffer &buffer);
	CQrcode8BitByte(CString data);
	virtual ~CQrcode8BitByte();

};



class CQrcode2DIntArray
{
public:
	CUIntArray * GetIntArray(int r);
	int GetAt(int r, int c);
	void SetAt(int r, int c, int v);
	CQrcode2DIntArray();
	virtual ~CQrcode2DIntArray();
private:
	CObArray a;
};



class CQrcodeRSBlock : public CObject
{
public:
	CQrcodeRSBlock(int totalCount, int dataCount);
	virtual ~CQrcodeRSBlock();

	int getDataCount();
	int getTotalCount();
	static void getRsBlockTable(int typeNumber, int errorCorrectLevel, CUIntArray &a);
	static void getRSBlocks(int typeNumber, int errorCorrectLevel, CObArray &RBSlocks);

private:
	int totalCount;
	int dataCount;

};


class CQrcode
{
public:
	void SaveToBmp(CString filename, int cellSize, int margin);
	void Draw(CDC *pdc, int cellSize, int margin);
	void CQrcode::Draw(CDC *pdc, int cellSize, int margin, int offsetX, int offsetY);
	static CQrcode* getMinimumQRCode(CString data, int errorCorrectLevel);
	void mapData(BYTE* bytes, int bytes_size, int maskPattern);
	static BYTE* createData(int typeNumber, int errorCorrectLevel, CObArray &dataArray, int* bytesSize);
	static BYTE* createBytes(CQrcodeBitBuffer &buffer, CObArray &rsBlocks, int* bytesSize);
	void setupTypeNumber(BOOL test);
	void setupTypeInfo(BOOL test, int maskPattern);
	void setupTimingPattern();
	void setupPositionAdjustPattern();
	void setupPositionProbePattern(int row, int col);
	static int getLostPoint(CQrcode *qrcode);
	int getBestMaskPattern();
	void make();
	void make(BOOL test, int maskPattern);
	int getModuleCount();
	BOOL isDark(int row, int col);
	CQrcodeData * getData(int index);
	int getDataCount();
	void clearData();
	void addData(CString data, int mode);
	void addData(CString data);
	void setErrorCorrectLevel(int errorCorrectLevel);
	void setTypeNumber(int typeNumber);
	CQrcode();
	virtual ~CQrcode();

private:
	BYTE *modules;
	int moduleCount;
	int typeNumber;
	int errorCorrectLevel;
	CObArray qrDataList;
	CObArray forMemoryLeak;
};

#define ErrorCorrectLevel_L  1
#define ErrorCorrectLevel_M  0
#define ErrorCorrectLevel_Q  3
#define ErrorCorrectLevel_H  2
