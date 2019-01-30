#include "stdafx.h"

#include "qrcode.h"

#define MODE_NUMBER  (1<<0)
 #define MODE_ALPHA_NUM (1<<1)
 #define MODE_8BIT_BYTE (1<<2)
 #define MODE_HANZI (1<<3)
 #define PATTERN000 0
 #define PATTERN001 1
 #define PATTERN010 2
 #define PATTERN011 3
 #define PATTERN100 4
 #define PATTERN101 5
 #define PATTERN110 6
 #define PATTERN111 7
 #define PAD0   0xEC
 #define PAD1   0x11
 #define G15 ((1<<10)|(1<<8)|(1<<5)|(1<<4)|(1<<2)|(1<<1)|(1<<0))
 #define G18 ((1<<12)|(1<<11)|(1<<10)|(1<<9)|(1<<8)|(1<<5)|(1<<2)|(1<<0))
 #define G15_MASK ((1<<14)|(1<<12)|(1<<10)|(1<<4)|(1<<1))

int RS_BLOCK_TABLE[40][6]  = {

  // L =1
   // M =0
   // Q =3
   // H =2

  // 1
   {1, 26, 19,0,0,0},
   {1, 26, 16,0,0,0},
   {1, 26, 13,0,0,0},
   {1, 26, 9,0,0,0},
   
   // 2
   {1, 44, 34,0,0,0},
   {1, 44, 28,0,0,0},
   {1, 44, 22,0,0,0},
   {1, 44, 16,0,0,0},

  // 3
   {1, 70, 55,0,0,0},
   {1, 70, 44,0,0,0},
   {2, 35, 17,0,0,0},
   {2, 35, 13,0,0,0},

  // 4  
   {1, 100, 80,0,0,0},
   {2, 50, 32,0,0,0},
   {2, 50, 24,0,0,0},
   {4, 25, 9,0,0,0},
   
   // 5
   {1, 134, 108,0,0,0},
   {2, 67, 43,0,0,0},
   {2, 33, 15, 2, 34, 16},
   {2, 33, 11, 2, 34, 12},
   
   // 6
   {2, 86, 68,0,0,0},
   {4, 43, 27,0,0,0},
   {4, 43, 19,0,0,0},
   {4, 43, 15,0,0,0},
   
   // 7  
   {2, 98, 78,0,0,0},
   {4, 49, 31,0,0,0},
   {2, 32, 14, 4, 33, 15},
   {4, 39, 13, 1, 40, 14},
   
   // 8
   {2, 121, 97,0,0,0},
   {2, 60, 38, 2, 61, 39},
   {4, 40, 18, 2, 41, 19},
   {4, 40, 14, 2, 41, 15},
   
   // 9
   {2, 146, 116,0,0,0},
   {3, 58, 36, 2, 59, 37},
   {4, 36, 16, 4, 37, 17},
   {4, 36, 12, 4, 37, 13},
   
   // 10  
   {2, 86, 68, 2, 87, 69},
   {4, 69, 43, 1, 70, 44},
   {6, 43, 19, 2, 44, 20},
   {6, 43, 15, 2, 44, 16}

 };

 

 int PATTERN_POSITION_TABLE[40][7] ={
   {0,0,0,0,0,0,0},
   {6, 18,0,0,0,0,0},
   {6, 22,0,0,0,0,0},
   {6, 26,0,0,0,0,0},
   {6, 30,0,0,0,0,0},
   {6, 34,0,0,0,0,0},
   {6, 22, 38,0,0,0,0},
   {6, 24, 42,0,0,0,0},
   {6, 26, 46,0,0,0,0},
   {6, 28, 50,0,0,0,0},
   {6, 30, 54,0,0,0,0},  
   {6, 32, 58,0,0,0,0},
   {6, 34, 62,0,0,0,0},
   {6, 26, 46, 66,0,0,0},
   {6, 26, 48, 70,0,0,0},
   {6, 26, 50, 74,0,0,0},
   {6, 30, 54, 78,0,0,0},
   {6, 30, 56, 82,0,0,0},
   {6, 30, 58, 86,0,0,0},
   {6, 34, 62, 90,0,0,0},
   {6, 28, 50, 72, 94,0,0},
   {6, 26, 50, 74, 98,0,0},
   {6, 30, 54, 78, 102,0,0},
   {6, 28, 54, 80, 106,0,0},
   {6, 32, 58, 84, 110,0,0},
   {6, 30, 58, 86, 114,0,0},
   {6, 34, 62, 90, 118,0,0},
   {6, 26, 50, 74, 98, 122,0},
   {6, 30, 54, 78, 102, 126,0},
   {6, 26, 52, 78, 104, 130,0},
   {6, 30, 56, 82, 108, 134,0},
   {6, 34, 60, 86, 112, 138,0},
   {6, 30, 58, 86, 114, 142,0},
   {6, 34, 62, 90, 118, 146,0},
   {6, 30, 54, 78, 102, 126, 150},
   {6, 24, 50, 76, 102, 128, 154},
   {6, 28, 54, 80, 106, 132, 158},
   {6, 32, 58, 84, 110, 136, 162},
   {6, 26, 54, 82, 110, 138, 166},
   {6, 30, 58, 86, 114, 142, 170}
  };
  int MAX_LENGTH[10][4][4] = {
       //L N   A    8B  Han           M                         Q                     H
         { {41,  25,  17,  10},  {34,  20,  14,  8},   {27,  16,  11,  7},  {17,  10,  7,   4} },
         { {77,  47,  32,  20},  {63,  38,  26,  16},  {48,  29,  20,  12}, {34,  20,  14,  8} },
         { {127, 77,  53,  32},  {101, 61,  42,  26},  {77,  47,  32,  20}, {58,  35,  24,  15} },
         { {187, 114, 78,  48},  {149, 90,  62,  38},  {111, 67,  46,  28}, {82,  50,  34,  21} },
         { {255, 154, 106, 65},  {202, 122, 84,  52},  {144, 87,  60,  37}, {106, 64,  44,  27} },
         { {322, 195, 134, 82},  {255, 154, 106, 65},  {178, 108, 74,  45}, {139, 84,  58,  36} },
         { {370, 224, 154, 95},  {293, 178, 122, 75},  {207, 125, 86,  53}, {154, 93,  64,  39} },
         { {461, 279, 192, 118}, {365, 221, 152, 93},  {259, 157, 108, 66}, {202, 122, 84,  52} },
         { {552, 335, 230, 141}, {432, 262, 180, 111}, {312, 189, 130, 80}, {235, 143, 98,  60} },
         { {652, 395, 271, 167}, {513, 311, 213, 131}, {364, 221, 151, 93}, {288, 174, 119, 74} }
     };


  

CQrcodeBitBuffer::CQrcodeBitBuffer()
 {
  inclements = 32;
  buffer = (BYTE*)malloc(inclements);
  memset(buffer, 0, inclements);
  buffer_len = inclements;
  length = 0;
 }

CQrcodeBitBuffer::~CQrcodeBitBuffer()
 {
  free(buffer);
 }

BYTE* CQrcodeBitBuffer::getBuffer()
 {
  return buffer;
 }

int CQrcodeBitBuffer::getLengthInBits()
 {
  return length;
 }

CString CQrcodeBitBuffer::toString()
 {
  CString s;
  for (int i = 0; i < getLengthInBits(); i++) {
   s += get(i) ? "1" : "0";
  }
  return s;
 }

BOOL CQrcodeBitBuffer::get(int index)
 {
  int n = CQrcodeUtil::unsignedRightShift(buffer[index / 8], 7 - index % 8);
  return (n&1) == 1;
 }

void CQrcodeBitBuffer::put(int num, int length)
 {
  int t;
  for (int i = 0; i < length; i++) {
   t = CQrcodeUtil::unsignedRightShift(num, length-i-1);
   put(  (t & 1) == 1);
  }
 }

void CQrcodeBitBuffer::put(BOOL bit)
 {
  if (length == buffer_len * 8) {
   BYTE* newBuffer = (BYTE*)malloc(buffer_len + inclements);
   memset(newBuffer, 0, buffer_len + inclements);
   memcpy(newBuffer, buffer, buffer_len);
   free(buffer);
   buffer = newBuffer; 
   buffer_len += inclements;
  }

 if (bit) {
   buffer[length / 8] |= CQrcodeUtil::unsignedRightShift(0x80, length % 8);
  }

 length++;
 }


  

CQrcodeUtil::CQrcodeUtil()
 {

}

CQrcodeUtil::~CQrcodeUtil()
 {

}


 void CQrcodeUtil::getPatternPosition(int typeNumber, CUIntArray &p)
 {
  int *pp = PATTERN_POSITION_TABLE[typeNumber - 1];
  for(int i=0;i<7;i++)
  {
   if (pp[i] == 0) break;
   p.Add(pp[i]);
  }
 }
 int CQrcodeUtil::getMaxLength(int typeNumber, int mode, int errorCorrectLevel)
 {
     int t = typeNumber - 1;
     int e = 0;
     int m = 0;

    switch(errorCorrectLevel) {
     case ErrorCorrectLevel_L : e = 0; break;
     case ErrorCorrectLevel_M : e = 1; break;
     case ErrorCorrectLevel_Q : e = 2; break;
     case ErrorCorrectLevel_H : e = 3; break;
     default :
         return 0;
     }

    switch(mode) {
     case MODE_NUMBER    : m = 0; break;
     case MODE_ALPHA_NUM : m = 1; break;
     case MODE_8BIT_BYTE : m = 2; break;
     case MODE_HANZI    : m = 3; break;
     default :
         return 0;
     }

    return MAX_LENGTH[t][e][m];
 }

CQrcodePolynomial CQrcodeUtil::getErrorCorrectPolynomial(int errorCorrectLength)
 {
  CUIntArray tnums;
  tnums.Add(1);
  CQrcodePolynomial a(tnums,0);
  CQrcodeMath *qrmath = CQrcodeMath::getInstance();
  for (int i = 0; i < errorCorrectLength; i++) {
   tnums.RemoveAll();
   tnums.Add(1);
   tnums.Add(qrmath->gexp(i));
   CQrcodePolynomial b(tnums,0);
   a = a.multiply(b);
  }

 return a;
 }

BOOL CQrcodeUtil::getMask(int maskPattern, int i, int j)
 {
  switch (maskPattern) {
  case PATTERN000 : return (i + j) % 2 == 0;
  case PATTERN001 : return i % 2 == 0;
  case PATTERN010 : return j % 3 == 0;
  case PATTERN011 : return (i + j) % 3 == 0;
  case PATTERN100 : return (i / 2 + j / 3) % 2 == 0;
  case PATTERN101 : return (i * j) % 2 + (i * j) % 3 == 0;
  case PATTERN110 : return ( (i * j) % 2 + (i * j) % 3) % 2 == 0;
  case PATTERN111 : return ( (i * j) % 3 + (i + j) % 2) % 2 == 0;
  default :
   return FALSE;
  }
  return FALSE;
 }

int CQrcodeUtil::getMode(CString s)
 {
  if (isAlphaNum(s) ) {
   if (isNumber(s) ) {
    return MODE_NUMBER;
   }
   return MODE_ALPHA_NUM;
  } else if (isHanZi(s) ) {
   return MODE_HANZI;
  } else {
   return MODE_8BIT_BYTE;
  }
 }

BOOL CQrcodeUtil::isNumber(CString s)
 {
  for (int i = 0; i < s.GetLength(); i++) {
   char c = s.GetAt(i);
   if (!('0' <= c && c <= '9') ) {
    return FALSE;
   }
  }
  return TRUE;
 }

BOOL CQrcodeUtil::isAlphaNum(CString s)
 {
  CString othersAlpha = " $%*+-./:";
  for (int i = 0; i < s.GetLength(); i++) {
   char c = s.GetAt(i);
   if (!('0' <= c && c <= '9') && !('A' <= c && c <= 'Z') && othersAlpha.Find(c) == -1) {
    return FALSE;
   }
  }
  return TRUE;
 }

BOOL CQrcodeUtil::isHanZi(CString s)
 {
  //byte[] data = s.getBytes(QRUtil.getJISEncoding() );

 int i = 0;
  int dLen = s.GetLength();

 while (i + 1 < dLen) {
   
   int c = ( (0xff & s[i]) << 8) | (0xff & s[i + 1]);

  if (!(0x8140 <= c && c <= 0x9FFC) && !(0xE040 <= c && c <= 0xEBBF) ) {
    return FALSE;
   }
   
   i += 2;
  }

 if (i < dLen) {
   return FALSE;
  }
  
  return TRUE;
 }

int CQrcodeUtil::getBCHDigit(int data)
 {
  int digit = 0;

 while (data != 0) {
   digit++;
   data = unsignedRightShift(data, 1);  //data >>>= 1;
  }

 return digit;
 }

int CQrcodeUtil::getBCHTypeInfo(int data)
 {
  int d = data << 10;
  while (getBCHDigit(d) - getBCHDigit(G15) >= 0) {
   d ^= (G15 << (getBCHDigit(d) - getBCHDigit(G15) ) );  
  }
  return ( (data << 10) | d) ^ G15_MASK;
 }

int CQrcodeUtil::getBCHTypeNumber(int data)
 {
  int d = data << 12;
  while (getBCHDigit(d) - getBCHDigit(G18) >= 0) {
   d ^= (G18 << (getBCHDigit(d) - getBCHDigit(G18) ) );  
  }
  return (data << 12) | d;
 }
 //
 //无符号右移运算符>>> 针对8byte应该没问题
int CQrcodeUtil::unsignedRightShift(int v, int n)
 {
  int t = v >> n;
  return t & 0x7FFFFFF;
 }


  

 

CQrcodePolynomial::CQrcodePolynomial(const CQrcodePolynomial &p)
 {
  if(this == &p)  return;
  m_nums.RemoveAll();
  for(int i=0;i<p.m_nums.GetSize();i++) m_nums.Add(p.m_nums.GetAt(i));
 }
 CQrcodePolynomial& CQrcodePolynomial::operator=(const CQrcodePolynomial &p) 
 {     
  if(this == &p)  return *this;
  m_nums.RemoveAll();
  for(int i=0;i<p.m_nums.GetSize();i++) m_nums.Add(p.m_nums.GetAt(i));
  return *this;
 } 
 //
 CQrcodePolynomial::CQrcodePolynomial(CUIntArray& nums, int shift)
 {
  int length = nums.GetSize();
  int offset = 0;

 while (offset < length && nums[offset] == 0) {
   offset++;
  }

 int i;
  //this.num = new int[length - offset + shift];
  m_nums.RemoveAll();
  for ( i=0;i<length - offset + shift;i++) m_nums.Add(0);
  //System.arraycopy(num, offset, this.num, 0, num.length - offset);
  for ( i=offset;i<length;i++) m_nums[i-offset] = nums[i];
 }

CQrcodePolynomial::~CQrcodePolynomial()
 {

}

int CQrcodePolynomial::get(int idx)
 {
  return m_nums[idx];
 }

int CQrcodePolynomial::getLength()
 {
  return m_nums.GetSize();
 }

CString CQrcodePolynomial::toString()
 {
  CString s,t;
  for (int i = 0; i < getLength(); i++) {
   if (i > 0) {
    s += ",";
   }
   t.Format("%d", get(i) );
   s += t;
  }
  return s;
 }

CString CQrcodePolynomial::toLogString()
 {
  CString s,t;
  CQrcodeMath *qrmath = CQrcodeMath::getInstance();
  for (int i = 0; i < getLength(); i++) {
   if (i > 0) {
    s += ",";
   }
   t.Format("%d", qrmath->glog(get(i)) ); //QRMath.glog(get(i) )
   s += t;
  }
  return s;
 }

CQrcodePolynomial CQrcodePolynomial::multiply(CQrcodePolynomial &e)
 {
   CUIntArray tnums;
   int i;
   for( i=0;i<getLength() + e.getLength() - 1;i++)
   {
    tnums.Add(0);
   }
   CQrcodeMath *qrmath = CQrcodeMath::getInstance();
   for ( i = 0; i < getLength(); i++) {
    for (int j = 0; j < e.getLength(); j++) {
     tnums[i + j] ^= qrmath->gexp(qrmath->glog(get(i) ) + qrmath->glog(e.get(j) ) );
    }
   }

  return CQrcodePolynomial(tnums,0);
 }

CQrcodePolynomial CQrcodePolynomial::mod(CQrcodePolynomial &e)
 {
  if (getLength() - e.getLength() < 0) {
   return (*this);
  }

 CQrcodeMath *qrmath = CQrcodeMath::getInstance();
  // 
  int ratio = qrmath->glog(get(0));
  ratio -= qrmath->glog(e.get(0));

 //
  //int[] num = new int[getLength()];
  CUIntArray tnums;
  int i;
  for(  i=0;i<getLength();i++) tnums.Add(0);
  for ( i = 0; i < getLength(); i++) {
   tnums[i] = get(i);
  }
  
  // 
  for ( i = 0; i < e.getLength(); i++) {
   tnums[i] ^= qrmath->gexp(qrmath->glog(e.get(i)) + ratio);
  }

 //
  CQrcodePolynomial pt(tnums,0);
  return pt.mod(e);
 }


  

CQrcodeMath *pQrmath = NULL;

CQrcodeMath::CQrcodeMath()
 {
  int i;
  for ( i = 0; i < 256; i++) EXP_TABLE.Add(0);
  for ( i = 0; i < 8; i++) {
   EXP_TABLE[i] = 1 << i;
  }
  for ( i = 8; i < 256; i++) EXP_TABLE[i] =  EXP_TABLE[i - 4]^ EXP_TABLE[i - 5]^ EXP_TABLE[i - 6]^ EXP_TABLE[i - 8];
  for ( i = 0; i < 256; i++)  LOG_TABLE.Add(0);
  for ( i = 0; i < 255; i++)  LOG_TABLE[EXP_TABLE[i]] = i;
 }

CQrcodeMath::~CQrcodeMath()
 {

}

CQrcodeMath * CQrcodeMath::getInstance()
 {
  if (pQrmath!=NULL){
   return pQrmath;
  }
  pQrmath = new CQrcodeMath();
  return pQrmath;
 }

int CQrcodeMath::glog(int n)
 {
  if (n < 1) {
   return 0;
  }
  
  return LOG_TABLE[n];
 }

int CQrcodeMath::gexp(int n)
 {
  while (n < 0) {
   n += 255;
  }

 while (n >= 256) {
   n -= 255;
  }

 return EXP_TABLE[n];
 }


  

CQrcodeRSBlock::CQrcodeRSBlock(int totalCount, int dataCount)
 {
   this->totalCount = totalCount;
   this->dataCount  = dataCount;
 }

CQrcodeRSBlock::~CQrcodeRSBlock()
 {

}

void CQrcodeRSBlock::getRsBlockTable(int typeNumber, int errorCorrectLevel, CUIntArray &a)
 {
  int *p = NULL;
  switch(errorCorrectLevel) {
  case ErrorCorrectLevel_L :
   p = RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 0];
   break;
  case ErrorCorrectLevel_M :
   p =  RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 1];
   break;
  case ErrorCorrectLevel_Q :
   p =  RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 2];
   break;
  case ErrorCorrectLevel_H :
   p = RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 3];
  default :
   break;
  }
  if (p!=NULL){
   for (int i=0;i<6;i++){
    if (p[i] == 0) break;
    a.Add(p[i]);
   }
  }
 }


 void CQrcodeRSBlock::getRSBlocks(int typeNumber, int errorCorrectLevel, CObArray &RBSlocks)
 {
  CUIntArray rsBlock;
  getRsBlockTable(typeNumber, errorCorrectLevel, rsBlock);
  int length = rsBlock.GetSize() / 3;
  //  
  for (int i = 0; i < length; i++) {

  int count = rsBlock[i * 3 + 0];
   int totalCount = rsBlock[i * 3 + 1];
   int dataCount  = rsBlock[i * 3 + 2];

  for (int j = 0; j < count; j++) {
    RBSlocks.Add(new CQrcodeRSBlock(totalCount, dataCount) ); 
   }
  }
 }

int CQrcodeRSBlock::getDataCount() {
  return dataCount;
 }
  
 int CQrcodeRSBlock::getTotalCount() {
  return totalCount;
 }


 CQrcode2DIntArray::CQrcode2DIntArray()
 {

}

CQrcode2DIntArray::~CQrcode2DIntArray()
 {
	 for (int i = 0;i < a.GetCount(); i++) {
		 delete a.GetAt(i);
	 }
	 a.RemoveAll();
}

void CQrcode2DIntArray::SetAt(int r, int c, int v)
 {
  for(int i=a.GetSize()-1;i<r;i++) a.Add(new CUIntArray());
  CUIntArray *p = (CUIntArray*) a.GetAt(r);
  for(int j=p->GetSize()-1;j<c;j++) p->Add(0);
  p->SetAt(c, v);
 }

int CQrcode2DIntArray::GetAt(int r, int c)
 {
  CUIntArray *p = (CUIntArray*) a.GetAt(r);
  return p->GetAt(c);
 }

CUIntArray * CQrcode2DIntArray::GetIntArray(int r)
 {
  return (CUIntArray*) a.GetAt(r);
 }

 

CQrcode8BitByte::CQrcode8BitByte(CString data) : CQrcodeData(MODE_8BIT_BYTE, data)
 {

}

CQrcode8BitByte::~CQrcode8BitByte()
 {

}

void CQrcode8BitByte::write(CQrcodeBitBuffer &buffer)
 {
  for (int i = 0; i < data.GetLength(); i++) {
   buffer.put(data[i], 8);
  }
 }

CQrcodeAlphaNum::CQrcodeAlphaNum(CString data) : CQrcodeData(MODE_ALPHA_NUM, data)
 {

}

CQrcodeAlphaNum::~CQrcodeAlphaNum()
 {

}

void CQrcodeAlphaNum::write(CQrcodeBitBuffer &buffer)
 {
  int i = 0;
  int len = data.GetLength();

 while (i + 1 < len) {
   buffer.put(getCode(data[i]) * 45 + getCode(data[i + 1]), 11);
   i += 2;
  }
  
  if (i < len) {
   buffer.put(getCode(data[i]), 6);
  }
 }

int CQrcodeAlphaNum::getCode(char c)
 {
   if ('0' <= c && c <= '9') {
    return c - '0';
   } else if ('A' <= c && c <= 'Z') {
    return c - 'A' + 10;
   } else {
    switch (c) {
    case ' ' : return 36;
    case '$' : return 37;
    case '%' : return 38;
    case '*' : return 39;
    case '+' : return 40;
    case '-' : return 41;
    case '.' : return 42;
    case '/' : return 43;
    case ':' : return 44;
    default :
      return 36;
    }
   }
 }

CQrcodeData::CQrcodeData(int mode, CString data)
 {
  this->mode = mode;
  this->data = data;
 }

CQrcodeData::~CQrcodeData()
 {

}

int CQrcodeData::getMode()
 {
  return mode;
 }


 int CQrcodeData::getLength()
 {
  return data.GetLength();
 }

void CQrcodeData::write(CQrcodeBitBuffer &buffer)
 {

}

int CQrcodeData::getLengthInBits(int type)
 {
   if (1 <= type && type < 10) {

   // 1 - 9

   switch(mode) {
    case MODE_NUMBER   : return 10;
    case MODE_ALPHA_NUM  : return 9;
    case MODE_8BIT_BYTE : return 8;
    case MODE_HANZI    : return 8;
    default :
     return 8;
    }

  } else if (type < 27) {

   // 10 - 26

   switch(mode) {
    case MODE_NUMBER   : return 12;
    case MODE_ALPHA_NUM  : return 11;
    case MODE_8BIT_BYTE : return 16;
    case MODE_HANZI    : return 10;
    default :
     return 10;
    }

  } else if (type < 41) {

   // 27 - 40

   switch(mode) {
    case MODE_NUMBER   : return 14;
    case MODE_ALPHA_NUM : return 13;
    case MODE_8BIT_BYTE : return 16;
    case MODE_HANZI    : return 12;
    default :
     return 12;
    }

  } else {
    return 8;
   }
 }

CQrcodeHanzi::CQrcodeHanzi(CString data) : CQrcodeData(MODE_HANZI, data)
 {

}

CQrcodeHanzi::~CQrcodeHanzi()
 {

}

int CQrcodeHanzi::getLength()
 {
  return data.GetLength()/2;
 }

void CQrcodeHanzi::write(CQrcodeBitBuffer &buffer)
 {

 int dlen = data.GetLength();
    int i = 0;

   while (i + 1 < dlen) {
     
     int c = ( (0xff & data[i]) << 8) | (0xff & data[i + 1]);

    if (0x8140 <= c && c <= 0x9FFC) {
      c -= 0x8140;
     } else if (0xE040 <= c && c <= 0xEBBF) {
      c -= 0xC140;
     } else {
      i += 2;
      continue;
     }
     
     c = ( CQrcodeUtil::unsignedRightShift(c, 8) & 0xff) * 0xC0 + (c & 0xff);

    buffer.put(c, 13);
     
     i += 2;
    }

   if (i < dlen) {
    }
 }


 CQrcodeNumber::CQrcodeNumber(CString data) : CQrcodeData(MODE_NUMBER, data)
 {
 }

CQrcodeNumber::~CQrcodeNumber()
 {

}


 int CQrcodeNumber::parseInt(CString s)
 {
  int num = 0;
  for (int i = 0; i < s.GetLength(); i++) {
   num = num * 10 + (s.GetAt(i) - '0');
  }
  //CString ss;ss.Format("%d",num);AfxMessageBox(ss);
  return num;
 }


 void CQrcodeNumber::write(CQrcodeBitBuffer &buffer)
 {
  int i = 0;
  int dataLen = data.GetLength();
  int num;

 while (i + 2 < dataLen  ) {
   num = parseInt(data.Mid(i, 3));//数字类型的数据，每三位数 变成一个10位的二进制,如果256  变成 0010000000
   buffer.put(num, 10);
   i += 3;
  }
  
  if (i < dataLen ) {
   
   if (dataLen - i == 1) {
    num = parseInt(data.Mid(i, 1));
    buffer.put(num, 4);
   } else if (dataLen - i == 2) {
    num = parseInt(data.Mid(i, 2));
    buffer.put(num, 7);
   }

 }
 }


  

CQrcode::CQrcode()
 {
  modules = NULL;
   typeNumber = 1;
  errorCorrectLevel = ErrorCorrectLevel_H;
 }

CQrcode::~CQrcode()
 {
  clearData();
  free(modules);
  delete CQrcodeMath::getInstance();
  pQrmath = NULL;
 }

void CQrcode::setTypeNumber(int typeNumber)
 {
  this->typeNumber = typeNumber;
 }

void CQrcode::setErrorCorrectLevel(int errorCorrectLevel)
 {
  this->errorCorrectLevel = errorCorrectLevel;
 }

void CQrcode::addData(CString data)
 {
  addData(data, CQrcodeUtil::getMode(data) );
 }

void CQrcode::addData(CString data, int mode)
 {
   switch(mode) {

  case MODE_NUMBER :
    qrDataList.Add(new CQrcodeNumber(data) );
    break;

  case MODE_ALPHA_NUM :
    qrDataList.Add(new CQrcodeAlphaNum(data) );
    break;

  case MODE_8BIT_BYTE :
    qrDataList.Add(new CQrcode8BitByte(data) );
    break;

  case MODE_HANZI :
    qrDataList.Add(new CQrcodeHanzi(data) );
    break;

  default :
    ;
   }
 }

void CQrcode::clearData()
 {
  for(int i=0;i<qrDataList.GetSize();i++){
   delete qrDataList.GetAt(i);
  }
  qrDataList.RemoveAll();
 }

int CQrcode::getDataCount()
 {
  return qrDataList.GetSize();
 }

CQrcodeData * CQrcode::getData(int index)
 {
  return (CQrcodeData*) qrDataList.GetAt(index);
 }

BOOL CQrcode::isDark(int row, int col)
 {
  if (modules[row*moduleCount + col] != 0 ) {
   return (modules[row*moduleCount + col] == 1) ? TRUE : FALSE; //1==true 2==false
  } else {
   return FALSE;
  }
  return TRUE;
 }

int CQrcode::getModuleCount()
 {
  return moduleCount;
 }

void CQrcode::make(BOOL test, int maskPattern)
 {
   //二维码方框像数大小
  moduleCount = typeNumber * 4 + 17;
   //modules = new Boolean[moduleCount][moduleCount];
  if (modules != NULL)
	  free(modules);

   modules = (BYTE*)malloc(moduleCount*moduleCount);
   memset(modules, 0, moduleCount*moduleCount);

  // 
   setupPositionProbePattern(0, 0);
   setupPositionProbePattern(moduleCount - 7, 0);
   setupPositionProbePattern(0, moduleCount - 7);
   
   setupPositionAdjustPattern();
   setupTimingPattern();

  setupTypeInfo(test, maskPattern);

  if (typeNumber >= 7) {
    setupTypeNumber(test);
   }
   
   //QRData[] dataArray = (QRData[])qrDataList.toArray(new QRData[qrDataList.size()]);
   //byte[] data = createData(typeNumber, errorCorrectLevel, dataArray);
   //mapData(data, maskPattern);

  int bytes_len = 0;
   BYTE* bytes = createData(typeNumber, errorCorrectLevel, qrDataList, &bytes_len);
   mapData(bytes, bytes_len, maskPattern);
   free(bytes);
 }

void CQrcode::make()
 {
  make(FALSE, getBestMaskPattern() );
 }

int CQrcode::getBestMaskPattern()
 {
   int minLostPoint = 0;
   int pattern = 0;

  for (int i = 0; i < 8; i++) {

   make(true, i);

   int lostPoint = getLostPoint(this);

   if (i == 0 || minLostPoint >  lostPoint) {
     minLostPoint = lostPoint;
     pattern = i;
    }
   }

  return pattern;
 }

int CQrcode::getLostPoint(CQrcode *qrcode)
 {
   int moduleCount = qrcode->getModuleCount();
   
   int lostPoint = 0;
   
   int row;

  // LEVEL1
   
   for ( row = 0; row < moduleCount; row++) {

   for (int col = 0; col < moduleCount; col++) {

    int sameCount = 0;
     BOOL dark = qrcode->isDark(row, col);
     
     for (int r = -1; r <= 1; r++) {

     if (row + r < 0 || moduleCount <= row + r) {
       continue;
      }

     for (int c = -1; c <= 1; c++) {

      if (col + c < 0 || moduleCount <= col + c) {
        continue;
       }

      if (r == 0 && c == 0) {
        continue;
       }

      if (dark == qrcode->isDark(row + r, col + c) ) {
        sameCount++;
       }
      }
     }

    if (sameCount > 5) {
      lostPoint += (3 + sameCount - 5);
     }
    }
   }

  // LEVEL2

  for ( row = 0; row < moduleCount - 1; row++) {
    for (int col = 0; col < moduleCount - 1; col++) {
     int count = 0;
     if (qrcode->isDark(row,     col    ) ) count++;
     if (qrcode->isDark(row + 1, col    ) ) count++;
     if (qrcode->isDark(row,     col + 1) ) count++;
     if (qrcode->isDark(row + 1, col + 1) ) count++;
     if (count == 0 || count == 4) {
      lostPoint += 3;
     }
    }
   }

  // LEVEL3

  for ( row = 0; row < moduleCount; row++) {
    for (int col = 0; col < moduleCount - 6; col++) {
     if (qrcode->isDark(row, col)
       && !qrcode->isDark(row, col + 1)
       &&  qrcode->isDark(row, col + 2)
       &&  qrcode->isDark(row, col + 3)
       &&  qrcode->isDark(row, col + 4)
       && !qrcode->isDark(row, col + 5)
       &&  qrcode->isDark(row, col + 6) ) {
      lostPoint += 40;
     }
    }
   }

  int col;
   for ( col = 0; col < moduleCount; col++) {
    for (int row = 0; row < moduleCount - 6; row++) {
     if (qrcode->isDark(row, col)
       && !qrcode->isDark(row + 1, col)
       &&  qrcode->isDark(row + 2, col)
       &&  qrcode->isDark(row + 3, col)
       &&  qrcode->isDark(row + 4, col)
       && !qrcode->isDark(row + 5, col)
       &&  qrcode->isDark(row + 6, col) ) {
      lostPoint += 40;
     }
    }
   }

  // LEVEL4
   
   int darkCount = 0;

  for ( col = 0; col < moduleCount; col++) {
    for (int row = 0; row < moduleCount; row++) {
     if (qrcode->isDark(row, col) ) {
      darkCount++;
     }
    }
   }
   
   int ratio = abs(100 * darkCount / moduleCount / moduleCount - 50) / 5;
   lostPoint += ratio * 10;
   
   return lostPoint;  
 }

void CQrcode::setupPositionProbePattern(int row, int col)
 {
  for (int r = -1; r <= 7; r++) {

  for (int c = -1; c <= 7; c++) {

   if (row + r <= -1 || moduleCount <= row + r 
      || col + c <= -1 || moduleCount <= col + c) {
     continue;
    }
     
    if ( (0 <= r && r <= 6 && (c == 0 || c == 6) )
      || (0 <= c && c <= 6 && (r == 0 || r == 6) )
      || (2 <= r && r <= 4 && 2 <= c && c <= 4) ) {
     //modules[row + r][col + c] = new Boolean(true);
     modules[(row + r) * moduleCount + (col + c)] = 1; //01
    } else {
     //modules[row + r][col + c] = new Boolean(false);
     modules[(row + r) * moduleCount + (col + c)] = 2; //10
    }
   }  
  } 
 }

void CQrcode::setupPositionAdjustPattern()
 {
  CUIntArray pos;

 // int[] pos = QRUtil.getPatternPosition(typeNumber);
  CQrcodeUtil::getPatternPosition(typeNumber, pos);
  int row,col;

 int pos_length = pos.GetSize();
  for (int i = 0; i < pos_length; i++) 
  {
   for (int j = 0; j < pos_length; j++) 
   {
    row = pos[i];
    col = pos[j];
    
    //if (modules[row][col] != null) {
    if (modules[row*moduleCount+col] != 0) {
     continue;
    }
    
    for (int r = -2; r <= 2; r++) {

    for (int c = -2; c <= 2; c++) {

     if (r == -2 || r == 2 || c == -2 || c == 2 || (r == 0 && c == 0) ) {
       //modules[row + r][col + c] = new Boolean(true);
       modules[(row+r)*moduleCount + col + c] = 1;//01
      } else {
       //modules[row + r][col + c] = new Boolean(false);
       modules[(row+r)*moduleCount + col + c] = 2;//10
      }
     }
    }

  }
  }
 }

void CQrcode::setupTimingPattern()
 {
  for (int r = 8; r < moduleCount - 8; r++) {
   //if (modules[r][6] != null) {
   if (modules[r*moduleCount + 6] != 0) {
    continue;
   }
   //modules[r][6] = new Boolean(r % 2 == 0);
   modules[r*moduleCount + 6] = (r % 2) == 0 ? 1 : 2;
  }
  for (int c = 8; c < moduleCount - 8; c++) {
   if (modules[6*moduleCount+ c] != 0) {
    continue;
   }
   //modules[6][c] = new Boolean(c % 2 == 0);
   modules[6*moduleCount+ c] = (c % 2) == 0 ? 1 : 2;
  }
 }

void CQrcode::setupTypeInfo(BOOL test, int maskPattern)
 {
   int data = (errorCorrectLevel << 3) | maskPattern;
   int bits = CQrcodeUtil::getBCHTypeInfo(data);

  BOOL mod = FALSE;
   int i;
   // 縦方向  
   for ( i = 0; i < 15; i++) {

   //Boolean mod = new Boolean(!test && ( (bits >> i) & 1) == 1);
    mod = FALSE;if (!test && ( (bits >> i) & 1) == 1) mod = TRUE;

   if (i < 6) {
     //modules[i][8] = mod;
     modules[i*moduleCount + 8] = mod ? 1:2;
    } else if (i < 8) {
     //modules[i + 1][8] = mod;
     modules[(i+1)*moduleCount + 8] = mod ? 1:2;
    } else {
     //modules[moduleCount - 15 + i][8] = mod;
     modules[(moduleCount - 15 + i)*moduleCount + 8] = mod ? 1:2;
    }
   }

  // 横方向
  for ( i = 0; i < 15; i++) {

   //Boolean mod = new Boolean(!test && ( (bits >> i) & 1) == 1);
    mod = FALSE;if (!test && ( (bits >> i) & 1) == 1) mod = TRUE;
    
    if (i < 8) {
     //modules[8][moduleCount - i - 1] = mod;
     modules[8*moduleCount + moduleCount - i - 1] = mod ? 1:2;
    } else if (i < 9) {
     //modules[8][15 - i - 1 + 1] = mod;
     modules[8*moduleCount + 15 - i - 1 + 1] = mod ? 1:2;
    } else {
     //modules[8][15 - i - 1] = mod;
     modules[8*moduleCount + 15 - i - 1] = mod ? 1:2;
    }
   }

  // 固定
  //modules[moduleCount - 8][8] = new Boolean(!test);
   modules[(moduleCount - 8)*moduleCount + 8] = !test ? 1:2;
 }

void CQrcode::setupTypeNumber(BOOL test)
 {
  int bits = CQrcodeUtil::getBCHTypeNumber(typeNumber);
  BOOL mod = FALSE;
  int i;
  for ( i = 0; i < 18; i++) {
   //Boolean mod = new Boolean(!test && ( (bits >> i) & 1) == 1);
   mod = FALSE;if (!test && ( (bits >> i) & 1) == 1) mod = TRUE;
   //modules[i / 3][i % 3 + moduleCount - 8 - 3] = mod;
   modules[(i/3)*moduleCount + i % 3 + moduleCount - 8 - 3] = mod ? 1:2;
  }

 for ( i = 0; i < 18; i++) {
   //Boolean mod = new Boolean(!test && ( (bits >> i) & 1) == 1);
   mod = FALSE;if (!test && ( (bits >> i) & 1) == 1) mod = TRUE;
   //modules[i % 3 + moduleCount - 8 - 3][i / 3] = mod;
   modules[(i % 3 + moduleCount - 8 - 3)*moduleCount + (i / 3)] = mod ? 1:2;
  }
 }

 

BYTE* CQrcode::createData(int typeNumber, int errorCorrectLevel, CObArray &dataArray, int* bytesSize)
 {
  //RSBlock[] rsBlocks = RSBlock.getRSBlocks(typeNumber, errorCorrectLevel);
  CObArray rsBlocks;
  CQrcodeRSBlock::getRSBlocks(typeNumber, errorCorrectLevel, rsBlocks);
  int i;
   
  CQrcodeBitBuffer buffer;
  CQrcodeData *pqrdata = NULL;
  
  for ( i = 0; i < dataArray.GetSize(); i++) {
   pqrdata = (CQrcodeData *)dataArray.GetAt(i); 
   buffer.put(pqrdata->getMode(), 4);
   buffer.put(pqrdata->getLength(), pqrdata->getLengthInBits(typeNumber) );
   pqrdata->write(buffer);
  }
  
  // 最大データ数を計算
 int totalDataCount = 0;
  CQrcodeRSBlock *pblock = NULL;
  for ( i = 0; i < rsBlocks.GetSize(); i++) {
   pblock = (CQrcodeRSBlock *)rsBlocks.GetAt(i);
   totalDataCount += pblock->getDataCount();
  }

 if (buffer.getLengthInBits() > totalDataCount * 8) {
   /*throw new IllegalArgumentException("code length overflow. ("
    + buffer.getLengthInBits()
    + ">"
    +  totalDataCount * 8
    + ")");
    */
   return NULL;
  }

 // 終端コード
 if (buffer.getLengthInBits() + 4 <= totalDataCount * 8) {
   buffer.put(0, 4);
  }

 // padding
  while (buffer.getLengthInBits() % 8 != 0) {
   buffer.put(FALSE);
  }

 // padding
  while (TRUE) {
   
   if (buffer.getLengthInBits() >= totalDataCount * 8) {
    break;
   }
   buffer.put(PAD0, 8);
   
   if (buffer.getLengthInBits() >= totalDataCount * 8) {
    break;
   }
   buffer.put(PAD1, 8);
  }

 BYTE* pRet =  createBytes(buffer, rsBlocks, bytesSize);
 for (int i = 0;i < rsBlocks.GetCount();i++) {
	 delete rsBlocks.GetAt(i);
 }
 rsBlocks.RemoveAll();
 return pRet;
 }

BYTE* CQrcode::createBytes(CQrcodeBitBuffer &buffer, CObArray &rsBlocks, int* bytesSize)
 {
   int offset = 0;
   int i,r;
   int maxDcCount = 0;
   int maxEcCount = 0;
   int dcCount;
   int ecCount;
   CUIntArray *pint = NULL;
   
   CQrcode2DIntArray dcdata;
   CQrcode2DIntArray ecdata;
   int rsBlocks_length = rsBlocks.GetSize();
   CQrcodeRSBlock *pblock = NULL;
   for ( r = 0; r < rsBlocks_length; r++) {
    pblock = (CQrcodeRSBlock *)rsBlocks.GetAt(r);
    dcCount = pblock->getDataCount();
    ecCount = pblock->getTotalCount() - dcCount;

   maxDcCount = maxDcCount > dcCount ? maxDcCount:dcCount;
    maxEcCount = maxEcCount > ecCount ? maxEcCount:ecCount;
    
    for (i = 0; i < dcCount; i++) {
     dcdata.SetAt(r,i, 0xff & buffer.getBuffer()[i + offset]);
    }
    offset += dcCount;
    
    CQrcodePolynomial rsPoly = CQrcodeUtil::getErrorCorrectPolynomial(ecCount);
    pint = dcdata.GetIntArray(r);
    CQrcodePolynomial rawPoly((*pint), rsPoly.getLength() - 1);
    CQrcodePolynomial modPoly = rawPoly.mod(rsPoly);
    //
    for ( i = 0; i < (rsPoly.getLength() - 1); i++) {
     int modIndex = i + modPoly.getLength() - (rsPoly.getLength() - 1);
     ecdata.SetAt(r,i, (modIndex >= 0)? modPoly.get(modIndex) : 0);
    }

  }
   
   
   int totalCodeCount = 0;
   for ( i = 0; i < rsBlocks_length; i++) {
    pblock = (CQrcodeRSBlock *)rsBlocks.GetAt(i);
    totalCodeCount += pblock->getTotalCount();
   }

  BYTE* data = (BYTE*)malloc(totalCodeCount);
   memset(data, 0 , totalCodeCount);
   (*bytesSize) = totalCodeCount;

  int index = 0;
   
   for ( i = 0; i < maxDcCount; i++) {
    for ( r = 0; r < rsBlocks_length; r++) {
     pint = dcdata.GetIntArray(r);
     if (i < pint->GetSize()) {
      data[index++] = (BYTE)dcdata.GetAt(r,i);
     }
    }
   }

  for ( i = 0; i < maxEcCount; i++) {
    for ( r = 0; r < rsBlocks_length; r++) {
     pint = ecdata.GetIntArray(r);
     if (i < pint->GetSize()) {
      data[index++] = (BYTE)ecdata.GetAt(r,i);
     }
    }
   }
   return data;

}

void CQrcode::mapData(BYTE* bytes, int bytes_size, int maskPattern)
 {
   int inc = -1;
   int row = moduleCount - 1;
   int bitIndex = 7;
   int byteIndex = 0;
   
   for (int col = moduleCount - 1; col > 0; col -= 2) {

   if (col == 6) col--;

   while (true) {
     
     for (int c = 0; c < 2; c++) {
      
      if (modules[row*moduleCount + col - c] == 0) {
       
       BOOL dark = FALSE;

      if (byteIndex < bytes_size) {
        dark = ( ( (bytes[byteIndex] >> bitIndex) & 1) == 1);
       }

      BOOL mask = CQrcodeUtil::getMask(maskPattern, row, col - c);

      if (mask) {
        dark = !dark;
       }
       
       modules[row*moduleCount + col - c] = dark ? 1:2;
       bitIndex--;

      if (bitIndex == -1) {
        byteIndex++;
        bitIndex = 7;
       }
      }
     }
         
     row += inc;

    if (row < 0 || moduleCount <= row) {
      row -= inc;
      inc = -inc;
      break;
     }
    }
   }
 }

CQrcode* CQrcode::getMinimumQRCode(CString data, int errorCorrectLevel)
 {
  int mode = CQrcodeUtil::getMode(data);

    CQrcode *qr = new CQrcode();
     qr->setErrorCorrectLevel(errorCorrectLevel);
     qr->addData(data, mode);

 CQrcodeData *pdata = (CQrcodeData *)qr->getData(0);
     int length = pdata->getLength();

 for (int typeNumber = 1; typeNumber <= 10; typeNumber++) {
         if (length <= CQrcodeUtil::getMaxLength(typeNumber, mode, errorCorrectLevel) ) {
             qr->setTypeNumber(typeNumber);
             break;
         }
   if (typeNumber==10) AfxMessageBox("数据太大了");
  }

    qr->make();

    return qr;
 }
void CQrcode::Draw(CDC *pdc, int cellSize, int margin) {
	Draw(pdc, cellSize, margin, 0, 0);
}

void CQrcode::Draw(CDC *pdc, int cellSize, int margin, int offsetX, int offsetY)
 {
  int imageSize = getModuleCount() * cellSize + margin * 2;
  int row,col;
     for (int y = 0; y < imageSize; y++) {
         for (int x = 0; x < imageSize; x++) {
             if (margin <= x && x < imageSize - margin
                     && margin <= y && y < imageSize - margin) {
                         
                 col = (x - margin) / cellSize;
                 row = (y - margin) / cellSize;

                if (isDark(row, col) ) {
                     pdc->SetPixel(offsetX + x,offsetY + y, RGB(0,0,0));
                 } else {
                     pdc->SetPixel(offsetX + x, offsetY + y, RGB(255,255,255));
                 }

            } else {
                 pdc->SetPixel(offsetX + x, offsetY + y, RGB(255,255,255));
             }
         }
     }

}

typedef struct _T_PIXEL
  {
      BYTE b; //代表blue
      BYTE g; //代表green
      BYTE r; //代表red
  }T_PIXEL;

void CQrcode::SaveToBmp(CString filename, int cellSize, int margin)
 {
  BITMAPFILEHEADER BMPHeader; //BMP文件头
 BITMAPINFO BMPInfo; //BMP信息块
 BITMAPINFOHEADER BMPInfoHeader; //BMP信息头（即包含在BMP信息块的 信息头）
 //RGBQUAD BMPRgbQuad; //BMP色彩表（即包含在BMP信息块的色彩表）
 CFile BMPFile;
  if (!BMPFile.Open(filename,CFile::modeCreate|CFile::modeWrite)) //创建BMP文件
 {
   AfxMessageBox("无法创建文件"+filename);
   return;
  }

 //SetBMPFileHeader
  int imageSize = getModuleCount() * cellSize + margin * 2;
  BMPHeader.bfType=0x4D42;
  BMPHeader.bfSize=3*imageSize*imageSize+0x36; //指示 整个BMP文件字节数，其中0x36是文件头本身的长度
 BMPHeader.bfReserved1=0x0;
  BMPHeader.bfReserved2=0x0;
  BMPHeader.bfOffBits=0x36; //x36是文件头本身的长度
 //以上共占据14个字节
 BMPInfoHeader.biSize=sizeof(BITMAPINFOHEADER); //指示 文件信息头大小
 BMPInfoHeader.biWidth=imageSize; //图片宽度
 BMPInfoHeader.biHeight=imageSize; //图片高度
 BMPInfoHeader.biPlanes=1; 
  BMPInfoHeader.biBitCount=24; //图片位数，位24位图
 //以上共占据14+16个字节
 BMPInfoHeader.biCompression=0; //表示没有压缩
 BMPInfoHeader.biSizeImage=0x30; //因为没有压缩，所以可以设置为0
  BMPInfoHeader.biXPelsPerMeter=0x0;
  BMPInfoHeader.biYPelsPerMeter=0x0;
  BMPInfoHeader.biClrUsed=0; //表明使用所有索引色
 BMPInfoHeader.biClrImportant=0; //说明对图象显示有重要影响的颜色索引的数目，0表示都重要。
 //以上共占据14+16+24个字节

 BMPInfo.bmiHeader=BMPInfoHeader;

 BMPFile.Write(&(BMPHeader),sizeof(BMPHeader));
  BMPFile.Write(&BMPInfoHeader,sizeof(BMPInfo)-sizeof(RGBQUAD));
  //

 T_PIXEL p;
  int row,col;
  for (int y = imageSize; y >= 0 ; y--) {
   for (int x = 0; x < imageSize; x++) {
    if (margin <= x && x < imageSize - margin
      && margin <= y && y < imageSize - margin) {
                     
     col = (x - margin) / cellSize;
     row = (y - margin) / cellSize;

    if (isDark(row, col) ) {
      p.b = 0; p.g = 0; p.r = 0;
     } else {
      p.b = 255; p.g = 255; p.r = 255;
     }

   } else {
     p.b = 255; p.g = 255; p.r = 255;
    }
    BMPFile.Write(&p, sizeof(T_PIXEL));
   }
  }
  //
  BMPFile.Flush();
  BMPFile.Close();
 }
