//***************************************************************************
// Smt_XML.cpp : implementation file
//***************************************************************************

#include "Smt_XML.h"

/////////////////////////////////////////////////////////////////////////////
// Smt_XMLParser code

Smt_XMLParser::Smt_XMLParser( Smt_String filename )
{
	m_FileName = filename;

	m_XMLDoc = NULL;	                   
	m_CurrNode = NULL;		              
	m_XPathObj = NULL;             
	m_NewChildNode = NULL; 

	xmlKeepBlanksDefault( 1 );
}

Smt_XMLParser::~Smt_XMLParser()
{
	if( m_XMLDoc != NULL )
		CloseTable();
}

/****************************************************************************
�� �� ��: OpenTable
��    ��:
������ֵ: 
��������: ��һ��XML�ı�ڵ�
*****************************************************************************/
Smt_Uint Smt_XMLParser::OpenTable(Smt_String tlbname)
{
	Smt_Uint nRetVal = Smt_Success;

	try
	{
		m_XMLDoc = xmlReadFile(m_FileName.c_str(),"GB2312",XML_PARSE_RECOVER);   //�����ļ� 
		
		if (NULL == m_XMLDoc) return Smt_Fail;  
		
		xmlChar szXpath[256];
		::sprintf((char*)szXpath, "/Configer/Tables/Table[@TableName='%s']", tlbname.c_str());
		
		m_XPathObj = GetNodeSet(m_XMLDoc,szXpath);                                //��ѯ���õ����
		
		if ( m_XPathObj == NULL ) 
		{
			xmlFreeDoc(m_XMLDoc);
			m_XMLDoc = NULL;
			return Smt_Fail;
		}
		
		char *szValue = NULL;
		xmlNodeSetPtr nodeset = m_XPathObj->nodesetval;
		
		if ( nodeset->nodeNr != 1) 
		{
			xmlXPathFreeObject ( m_XPathObj );
			m_XPathObj = NULL;

			xmlFreeDoc(m_XMLDoc);
			m_XMLDoc = NULL;

			return Smt_Fail;
		}
		
		m_CurrNode = nodeset->nodeTab[0];
	}
	catch (...)
	{
		nRetVal = Smt_Fail;
	}
	
	return nRetVal;
}

/****************************************************************************
�� �� ��: CloseTable
��    ��:
������ֵ: 
��������: �رձ�ڵ�
*****************************************************************************/
Smt_Uint Smt_XMLParser::CloseTable()
{
	Smt_Uint nRetVal = Smt_Success;

	try
	{
		if( m_XPathObj != NULL)
			xmlXPathFreeObject ( m_XPathObj );
		
		if( m_XMLDoc != NULL )
			xmlFreeDoc(m_XMLDoc);
		
		m_XPathObj = NULL;
		m_XMLDoc = NULL;
	}
	catch (...)
	{
		nRetVal = Smt_Fail;		
	}

	return nRetVal;
}

/****************************************************************************
�� �� ��: GetString
��    ��:
������ֵ: 
��������: �����ֶ����ƻ�ȡ�ֶ�ֵ���ַ����ͣ�
*****************************************************************************/
Smt_String Smt_XMLParser::GetPropString(Smt_String fieldname)
{	
	Smt_String strRet = "";
	
	try
	{
		char *szValue = NULL;
		
		if (xmlHasProp(m_CurrNode, BAD_CAST fieldname.c_str()) )
		{
			szValue = U2G((char*)xmlGetProp(m_CurrNode,BAD_CAST  fieldname.c_str() ));
			if (szValue != NULL) 
			{
				strRet = szValue;
				ACE_OS::free(szValue);
			}
		}	
	}
	catch (...)
	{
		strRet = "";
	}
	
	return strRet;
}

/****************************************************************************
�� �� ��: GetUint
��    ��:
������ֵ: 
��������: �����ֶ����ƻ�ȡ�ֶ�ֵ����ֵ�ͣ�
*****************************************************************************/
Smt_Uint Smt_XMLParser::GetPropUint(Smt_String fieldname)
{	
	Smt_Uint nRetVal = 0;
	
	try
	{
		char *szValue = NULL;
		
		if (xmlHasProp(m_CurrNode, BAD_CAST fieldname.c_str()) )
		{
			szValue = U2G((char*)xmlGetProp(m_CurrNode,BAD_CAST fieldname.c_str() ));
			if (szValue != NULL) 
			{
				nRetVal = ACE_OS::atoi(szValue);
				ACE_OS::free(szValue);
			}
		}
	}
	catch (...)
	{
		nRetVal = 0;	
	}
	
	return nRetVal;
}

/****************************************************************************
�� �� ��: AddNewRecord
��    ��:
������ֵ: 
��������: ����һ����¼
*****************************************************************************/
Smt_Uint Smt_XMLParser::AddNewRecord()
{
	Smt_Uint nRetVal = Smt_Success;

	try
	{
		m_NewChildNode = xmlNewNode(NULL, BAD_CAST "RecRow");
		
		xmlAddChild( m_CurrNode, m_NewChildNode );
	}
	catch (...)
	{
		nRetVal = Smt_Fail;
	}
	
	return nRetVal;
}

/****************************************************************************
�� �� ��: PutPropString
��    ��:
������ֵ: 
��������: 
*****************************************************************************/
Smt_Uint Smt_XMLParser::PutPropString(Smt_String fieldname, Smt_String value )
{
	Smt_Uint nRetVal = Smt_Success;
	try
	{
		char* szOut = G2U( (char*)value.c_str() );
		
		xmlNewProp(m_NewChildNode, BAD_CAST fieldname.c_str(), BAD_CAST szOut);
		
		ACE_OS::free(szOut);
		szOut = NULL;
	}
	catch (...)
	{
		nRetVal = Smt_Fail;	
	}

	return nRetVal;
}

/****************************************************************************
�� �� ��: PutPropUint
��    ��:
������ֵ: 
��������: 
*****************************************************************************/
Smt_Uint Smt_XMLParser::PutPropUint(Smt_String fieldname, Smt_Uint value )
{
	Smt_Uint nRetVal = Smt_Success;
	try
	{
		Smt_String strValue = HLFormatStr( "%d", value );

		char* szOut = G2U( (char*)strValue.c_str() );
		
		xmlNewProp(m_NewChildNode, BAD_CAST fieldname.c_str(), BAD_CAST szOut);
		
		ACE_OS::free(szOut);
		szOut = NULL;
	}
	catch (...)
	{
		nRetVal = Smt_Fail;	
	}
	
	return nRetVal;
}

/****************************************************************************
�� �� ��: PutContentString
��    ��:
������ֵ: 
��������: 
*****************************************************************************/
Smt_Uint Smt_XMLParser::PutContentString( Smt_String value )
{
	Smt_Uint nRetVal = Smt_Success;
	try
	{
		char* szOut = G2U( (char*)value.c_str() );

		xmlNodePtr ndContent = xmlNewText( BAD_CAST value.c_str() );
		xmlAddChild( m_NewChildNode, ndContent );
		
		ACE_OS::free(szOut);
		szOut = NULL;
	}
	catch (...)
	{
		nRetVal = Smt_Fail;	
	}
	
	return nRetVal;
}

/****************************************************************************
�� �� ��: ClearTable
��    ��:
������ֵ: 
��������: 
*****************************************************************************/
Smt_Uint Smt_XMLParser::ClearTable()
{
	Smt_Uint nRetVal = Smt_Success;
	try
	{
		xmlNodePtr ndTemp = m_CurrNode->children;
		xmlNodePtr ndTemp2;
		while (NULL != ndTemp )
		{
			if (!xmlStrcmp(ndTemp->name, BAD_CAST "RecRow"))
			{
				ndTemp2 = ndTemp->next;
				xmlUnlinkNode(ndTemp);
				xmlFreeNode(ndTemp);
				ndTemp = ndTemp2;
				continue;
			}
		}
	}
	catch (...)
	{
		nRetVal = Smt_Fail;	
	}
	
	return nRetVal;
}

/****************************************************************************
�� �� ��: UpdateTable
��    ��:
������ֵ: 
��������: �������ݵ� XML �ļ�
*****************************************************************************/

Smt_Uint Smt_XMLParser::UpdateTable()
{		
	Smt_Uint nRetVal = Smt_Success;
	try
	{
		xmlSaveCtxtPtr cTxt;
		cTxt = xmlSaveToFilename( m_FileName.c_str(), "GB2312", XML_SAVE_FORMAT );
		if (cTxt != NULL) 
		{ 
			if (xmlSaveDoc(cTxt, m_XMLDoc) < 0) 
			{
				nRetVal = Smt_Fail;
			} 
			xmlSaveClose(cTxt); 
		}
	}
	catch (...)
	{
		nRetVal = Smt_Fail;	
	}
	
	return nRetVal;
}

/****************************************************************************
�� �� ��: MoveFirst
��    ��:
������ֵ: 
��������: �Ƶ�ĳ����ĵ�һ����¼
*****************************************************************************/
Smt_Uint Smt_XMLParser::MoveFirst()
{
	if(m_CurrNode != NULL)
		m_CurrNode = m_CurrNode->xmlChildrenNode;

	return Smt_Success;
}

/****************************************************************************
�� �� ��: MoveNext
��    ��:
������ֵ: 
��������: ��һ����¼
*****************************************************************************/
Smt_Uint Smt_XMLParser::MoveNext()
{
	if(m_CurrNode != NULL)
		m_CurrNode = m_CurrNode->next; 
	return Smt_Success;
}

/****************************************************************************
�� �� ��: CodeConvert
��    ��:
������ֵ: int
��������: ����ת��,��һ�ֱ���תΪ��һ�ֱ���
*****************************************************************************/
int Smt_XMLParser::CodeConvert(char* from_charset, char* to_charset, char* inbuf,
				 int inlen, char* outbuf, int outlen)
{
	iconv_t cd;
	char** pin = &inbuf;   
	char** pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);   
	if(cd == 0)
		return -1;
	ACE_OS::memset(outbuf,0,outlen);   
	if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen) == -1)
		return -1;   
	iconv_close(cd);
	return 0;   
}

/****************************************************************************
�� �� ��: U2G
��    ��:
������ֵ: int
��������: UNICODE��תΪGB2312��,�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL
          nOutLen �� 1 ���ֽڷ�ֹ���� 0 �ֽڵ�ת��
*****************************************************************************/
char* Smt_XMLParser::U2G(char *inbuf)   
{
	int nOutLen = 2 * ACE_OS::strlen(inbuf) + 1;       
	char* szOut = (char*)ACE_OS::malloc(nOutLen);

	ACE_OS::memset(szOut, 0x0, nOutLen);
	
	if (-1 == CodeConvert("utf-8","gb2312",inbuf,ACE_OS::strlen(inbuf),szOut,nOutLen))
	{
		ACE_OS::free(szOut);
		szOut = NULL;
	}
	return szOut;
}   

/****************************************************************************
�� �� ��: G2U
��    ��:
������ֵ: char
��������: GB2312��תΪUNICODE��,�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL
          nOutLen �� 1 ���ֽڷ�ֹ���� 0 �ֽڵ�ת��
*****************************************************************************/
char* Smt_XMLParser::G2U(char *inbuf)   
{
	int nOutLen = 2 * ACE_OS::strlen(inbuf) + 1;         
	char* szOut = (char*)ACE_OS::malloc(nOutLen);

	ACE_OS::memset(szOut, 0x0, nOutLen);

	if (-1 == CodeConvert("gb2312","utf-8",inbuf,ACE_OS::strlen(inbuf),szOut,nOutLen))
	{
		ACE_OS::free(szOut);
		szOut = NULL;
	}
	return szOut;
}   

/****************************************************************************
�� �� ��: GetNodeSet
��    ��:
������ֵ: xmlXPathObjectPtr
��������: ��ȡDoc��NodeSet,�ɹ��򷵻�һ��xmlXPathObjectPtr��������Ҫ��ʹ����Ϻ��ֶ�xmlXPathFreeObject��ʧ�ܷ���NULL
*****************************************************************************/
xmlXPathObjectPtr Smt_XMLParser::GetNodeSet(xmlDocPtr doc, const xmlChar *szXpath) 
{
	xmlXPathContextPtr context;							//XPATH������ָ��
	xmlXPathObjectPtr result;							//XPATH����ָ�룬�����洢��ѯ���

	try
	{
		context = xmlXPathNewContext(doc);					//����һ��XPath������ָ��
		if (context == NULL) 
		{	
			//printf("context is NULL\n");
			return NULL; 
		}

		result = xmlXPathEvalExpression(szXpath, context);	//��ѯXPath���ʽ���õ�һ����ѯ���
		xmlXPathFreeContext(context);						//�ͷ�������ָ��
		if (result == NULL) 
		{
			//printf("xmlXPathEvalExpression return NULL\n"); 
			return NULL;  
		}

		if (xmlXPathNodeSetIsEmpty(result->nodesetval))		//����ѯ����Ƿ�Ϊ��
		{
			xmlXPathFreeObject(result);
			//printf("nodeset is empty\n");
			return NULL;
		}
	}
	catch(...)
	{
	}

	return result;	
}

/****************************************************************************
�� �� ��: IsEmpty
��    ��:
������ֵ: 
��������: ��¼�Ƿ�Ϊ��
*****************************************************************************/
Smt_Bool Smt_XMLParser::IsEmpty()
{
	if( m_CurrNode == NULL )
		return Smt_BoolTRUE;
	else
		return Smt_BoolFALSE;
}
