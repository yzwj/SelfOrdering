/*########################################################################
	���ļ�����: 	ado.h
	����  �ơ�:		ADO ��װ��.
	����  ����:		0.20
	����  �ߡ�:		����
	��E-mail��:		anyou@sina.com
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADORECORDSET_H)
#define _ANYOU_COOL_ADORECORDSET_H
#include "Ado.h"

/*########################################################################
			 ------------------------------------------------
						  CAdoRecordSet class
			 ------------------------------------------------
  ########################################################################*/

class CAdoRecordSet 
{
// ����/�۹� --------------------------------------------
public:
	CAdoRecordSet();
	CAdoRecordSet(CAdoConnection *pConnection);

	virtual ~CAdoRecordSet();
	
protected:
	void Release();
	
// ���� ------------------------------------------------	
public:
	// ��ǰ�༭״̬ ----------------------------
	EditModeEnum GetEditMode();
	
	// ��ǰ״̬ --------------------------------
	BOOL IsEOF();
	BOOL IsBOF();
	BOOL IsOpen();
	long GetState();
	long GetStatus();

	// �����ص�����¼�� --------------------
	long GetMaxRecordCount();
	BOOL SetMaxRecordCount(long count);
	
	// ���λ�� --------------------------------
	CursorLocationEnum GetCursorLocation();
	BOOL SetCursorLocation(CursorLocationEnum CursorLocation = adUseClient);
	
	// ������� --------------------------------
	CursorTypeEnum GetCursorType();
	BOOL SetCursorType(CursorTypeEnum CursorType = adOpenStatic);
	
	// ��ǩ --------------------------------
	_variant_t GetBookmark();
	BOOL SetBookmark(_variant_t varBookMark = _variant_t((long)adBookmarkFirst));
	
	// ��ǰ��¼λ�� ------------------------
	long GetAbsolutePosition();
	BOOL SetAbsolutePosition(int nPosition);

	long GetAbsolutePage();
	BOOL SetAbsolutePage(int nPage);

	// ÿҳ�ļ�¼�� ------------------------
	long GetPageSize();
	BOOL SetCacheSize(const long& lCacheSize);	

	// ҳ�� --------------------------------
	long GetPageCount();

	// ��¼�����ֶ��� ----------------------
	long GetRecordCount();
	long GetFieldsCount();
	
	// ��ѯ�ַ��� --------------------------
	CString GetSQLText() {return m_strSQL;}
	void SetSQLText(LPSTR strSQL) {m_strSQL = strSQL;}
	
	// ���Ӷ��� ----------------------------
	CAdoConnection* GetConnection() {return m_pConnection;}
	void SetAdoConnection(CAdoConnection *pConnection);

	// ��¼������ --------------------------
	_RecordsetPtr& GetRecordset();

	CString GetLastError();

// �ֶ����� ----------------------------------------------
public:
	// �ֶμ� -------------------------------
	FieldsPtr GetFields();

	// �ֶζ��� -----------------------------
	FieldPtr  GetField(long lIndex);
	FieldPtr  GetField(LPSTR lpszFieldName);
	
	// �ֶ��� -------------------------------
	CString GetFieldName(long lIndex);
	
	// �ֶ��������� -------------------------
	DataTypeEnum GetFieldType(long lIndex);
	DataTypeEnum GetFieldType(LPSTR lpszFieldName);

	// �ֶ����� -----------------------------
	long  GetFieldAttributes(long lIndex);
	long  GetFieldAttributes(LPSTR lpszFieldName);

	// �ֶζ��峤�� -------------------------
	long  GetFieldDefineSize(long lIndex);
	long  GetFieldDefineSize(LPSTR lpszFieldName);

	// �ֶ�ʵ�ʳ��� -------------------------
	long  GetFieldActualSize(long lIndex);
	long  GetFieldActualSize(LPSTR lpszFieldName);

	// �ֶ��Ƿ�ΪNULL -----------------------
	BOOL  IsFieldNull(long index);
	BOOL  IsFieldNull(LPSTR lpFieldName);

// ��¼���� --------------------------------------------
public:
	BOOL AddNew();
	BOOL Update();
	BOOL UpdateBatch(AffectEnum AffectRecords = adAffectAll); 
	BOOL CancelUpdate();
	BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
	BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);
	
	// ˢ�¼�¼���е����� ------------------
	BOOL Requery(long Options = adConnectUnspecified);
	BOOL Resync(AffectEnum AffectRecords = adAffectAll, ResyncEnum ResyncValues = adResyncAllValues);   

	BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
	BOOL AddNew(CADORecordBinding &pAdoRecordBinding);
	
// ��¼���������� --------------------------------------
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
	BOOL Move(long lRecords, _variant_t Start = _variant_t((long)adBookmarkFirst));
	
	// ����ָ���ļ�¼ ----------------------
	BOOL Find(LPSTR lpszFind, SearchDirectionEnum SearchDirection = adSearchForward);
	BOOL FindNext();

// ��ѯ ------------------------------------------------
public:
	BOOL Open(CString strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	BOOL Cancel();
	void Close();

	// ����/����־����ļ� -----------------
	BOOL Save(LPSTR strFileName = "", PersistFormatEnum PersistFormat = adPersistXML);
	BOOL Load(LPSTR strFileName);
	
// �ֶδ�ȡ --------------------------------------------
public:
	BOOL PutCollect(long index, const _variant_t &value);
	BOOL PutCollect(long index, const CString &value);
	BOOL PutCollect(long index, const double &value);
	BOOL PutCollect(long index, const float  &value);
	BOOL PutCollect(long index, const long   &value);
	BOOL PutCollect(long index, const DWORD  &value);
	BOOL PutCollect(long index, const int    &value);
	BOOL PutCollect(long index, const short  &value);
	BOOL PutCollect(long index, const BYTE   &value);
	BOOL PutCollect(long index, const bool   &value);
	BOOL PutCollect(long index, const COleDateTime &value);
	BOOL PutCollect(long index, const COleCurrency &value);

	

	BOOL PutCollect(LPSTR strFieldName, const _variant_t &value);
	BOOL PutCollect(LPSTR strFieldName, const CString &value);
	BOOL PutCollect(LPSTR strFieldName, const double &value);
	BOOL PutCollect(LPSTR strFieldName, const float  &value);
	BOOL PutCollect(LPSTR strFieldName, const long   &value);
	BOOL PutCollect(LPSTR strFieldName, const DWORD  &value);
	BOOL PutCollect(LPSTR strFieldName, const int    &value);
	BOOL PutCollect(LPSTR strFieldName, const short  &value);
	BOOL PutCollect(LPSTR strFieldName, const BYTE   &value);
	BOOL PutCollect(LPSTR strFieldName, const bool   &value);
	BOOL PutCollect(LPSTR strFieldName, const COleDateTime &value);
	BOOL PutCollect(LPSTR strFieldName, const COleCurrency &value);

	
	// ---------------------------------------------------------

	BOOL GetCollect(long index, CString &value);
	BOOL GetCollect(long index, double  &value);
	BOOL GetCollect(long index, float   &value);
	BOOL GetCollect(long index, long    &value);
	BOOL GetCollect(long index, DWORD   &value);
	BOOL GetCollect(long index, int     &value);
	BOOL GetCollect(long index, short   &value);
	BOOL GetCollect(long index, BYTE    &value);
	BOOL GetCollect(long index, bool   &value);
	BOOL GetCollect(long index, COleDateTime &value);
	BOOL GetCollect(long index, COleCurrency &value);

	BOOL GetFieldValue(long index, CString &value);
	BOOL GetFieldValue(long index, double  &value);
	BOOL GetFieldValue(long index, float   &value);
	BOOL GetFieldValue(long index, long    &value);
	BOOL GetFieldValue(long index, DWORD   &value);
	BOOL GetFieldValue(long index, int     &value);
	BOOL GetFieldValue(long index, short   &value);
	BOOL GetFieldValue(long index, BYTE    &value);
	BOOL GetFieldValue(long index, bool   &value);
	BOOL GetFieldValue(long index, COleDateTime &value);
	BOOL GetFieldValue(long index, COleCurrency &value);

	
	

	BOOL GetCollect(LPSTR strFieldName, CString &strValue);
	BOOL GetCollect(LPSTR strFieldName, double &value);
	BOOL GetCollect(LPSTR strFieldName, float  &value);
	BOOL GetCollect(LPSTR strFieldName, long   &value);
	BOOL GetCollect(LPSTR strFieldName, DWORD  &value);
	BOOL GetCollect(LPSTR strFieldName, int    &value);
	BOOL GetCollect(LPSTR strFieldName, short  &value);
	BOOL GetCollect(LPSTR strFieldName, BYTE   &value);
	BOOL GetCollect(LPSTR strFieldName, bool   &value);
	BOOL GetCollect(LPSTR strFieldName, COleDateTime &value);
	BOOL GetCollect(LPSTR strFieldName, COleCurrency &value);

	BOOL GetFieldValue(LPSTR strFieldName, CString &strValue);
	BOOL GetFieldValue(LPSTR strFieldName, double &value);
	BOOL GetFieldValue(LPSTR strFieldName, float  &value);
	BOOL GetFieldValue(LPSTR strFieldName, long   &value);
	BOOL GetFieldValue(LPSTR strFieldName, DWORD  &value);
	BOOL GetFieldValue(LPSTR strFieldName, int    &value);
	BOOL GetFieldValue(LPSTR strFieldName, short  &value);
	BOOL GetFieldValue(LPSTR strFieldName, BYTE   &value);
	BOOL GetFieldValue(LPSTR strFieldName, bool   &value);
	BOOL GetFieldValue(LPSTR strFieldName, COleDateTime &value);
	BOOL GetFieldValue(LPSTR strFieldName, COleCurrency &value);


	// BLOB ���ݴ�ȡ ------------------------------------------
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(long index, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(LPSTR strFieldName, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(long index, LPCTSTR lpszFileName);
	BOOL AppendChunk(LPSTR strFieldName, LPCTSTR lpszFileName);

	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	BOOL GetChunk(long index, LPVOID lpData);
	BOOL GetChunk(LPSTR strFieldName, LPVOID lpData);
	BOOL GetChunk(long index, CBitmap &bitmap);
	BOOL GetChunk(LPSTR strFieldName, CBitmap &bitmap);

// �������� --------------------------------------------
public:
	// ���˼�¼ ---------------------------------
	BOOL SetFilter(LPSTR lpszFilter);

	// ���� -------------------------------------
	BOOL SetSort(LPSTR lpszCriteria);

	// �����Ƿ�֧��ĳ���� -----------------------
	BOOL Supports(CursorOptionEnum CursorOptions = adAddNew);

	// ��¡ -------------------------------------
	BOOL Clone(CAdoRecordSet &pRecordSet);
	_RecordsetPtr operator = (_RecordsetPtr &pRecordSet);
	
	// ��ʽ�� _variant_t ����ֵ -----------------
	
//��Ա���� --------------------------------------------
protected:
	CAdoConnection     *m_pConnection;
	_RecordsetPtr		m_pRecordset;
	CString				m_strSQL;
	CString				m_strFind;
	CString				m_strFileName;
	IADORecordBinding	*m_pAdoRecordBinding;
	SearchDirectionEnum m_SearchDirection;
public:
	BOOL ClearMemory();
	_variant_t			m_varBookmark;
};
//________________________________________________________________________

#endif //_ANYOU_COOL_ADORECORDSET_H