#include "HexTool.h"

using namespace std;

/***********************************
function: IsHexChar
desc:   �ж��ַ��Ƿ���0-9��a-f֮��
input�� char-�ַ�
Output��
return:  �ַ���������8-8��a-10�����
���ǣ�����-1
***********************************/
int IsHex(char hc)
{
	if ('0' <= hc && hc <= '9')
		return (int(hc) - int('0'));
	else if ('a' <= hc && hc <= 'f')
		return (int(hc) - int('a') + 10);
	else if ('A' <= hc && hc <= 'F')
		return (int(hc) - int('A') + 10);
	return -1;
}

/**************************************
function: Hex2Ascii
desc:     16�����ַ���ת��ascii��
Input:    hex-16���ƣ��ַ�����ʽ��2�ַ�
Output:
return:    ascii��
note: �����ascii���ֱ��ת��Ϊint����ֵ
**************************************/
unsigned char Hex2Ascii(const string &hex)
{
	assert(hex.length() == 2);
	int high = IsHex(hex[0]);
	int low = IsHex(hex[1]);
	if (high == -1 || low == -1)
		return '\0';
	int asc = high * 16 + low;
	//char b = toascii(asc);
	return asc;
}

/****************************************
function��HexString2Int
desc:  16�����ַ���ת��Ϊ��ֵ
Input:    16�����ַ���
Output:
return:  ��ֵ
****************************************/
int HexString2Int(string hexstr)
{
	assert(hexstr.length() % 2 == 0);
	int idx = 0;
	int high, low = -1;
	int value = 0;
	while (idx < hexstr.length()) {
		high = IsHex(hexstr[idx + 0]);
		low = IsHex(hexstr[idx + 1]);
		value = (value << 8) + (high << 4) + low;
		idx += 2;
	}
	if (high == -1 || low == -1)
		return -1;
	return value;
}


/****************************
function: HexString2AsciiString
desc:     16�����ַ���ת��Ϊascii�봮
Input:    hex-16�����ַ���
Output:
return:  ascii�봮
*****************************/
string HexString2AsciiString(const string &hex)
{
	assert(hex.length() % 2 == 0);
	string hstr;
	for (int i = 0; i<hex.length(); i += 2)
	{
		string tmp = hex.substr(i, 2);
		hstr.append(1, Hex2Ascii(tmp));
	}
	return hstr;
}

/****************************************
function��AsciiString2HexString
desc:	ascii�봮ת��Ϊ16�����ַ���
Input:	ascii�봮
Output:
return:	hex-16�����ַ���
****************************************/
string AsciiString2HexString(char *data, int len)
{
	const string hex = "0123456789abcdef";
	stringstream ss;

	for (string::size_type i = 0; i < len; ++i)
		ss << hex[(unsigned char)data[i] >> 4] << hex[(unsigned char)data[i] & 0xf];	//�Ⱥ�д���4λ�͵�4λ
	return ss.str();
}


/****************************************
function��Dec2HexString
desc:	ʮ������ת��Ϊ16�����ַ���
Input:	ʮ������dec, ���ɵ�16�����ַ�������
Output:
return:	16�����ַ���
****************************************/
string Dec2HexString(int dec, int length)
{
	string hex_output;
	for (int i = 0; i < length; i++) {
		hex_output += '0';
	}
	const char * hex = "0123456789abcdef";
	for (int i = 0; i<length; i++)
	{
		hex_output[length - i - 1] = hex[(dec >> i * 4) & 0x0F];
	}
	return hex_output;
}
/*
sprintf(s, "%08x", num) ���ʵ�������0��8λ16������
*/

/****************************************
function��PrepareAsciiString
desc:  ȥ���ַ�����Ŀո�0x������ת��Ϊasciiָ��
Input:    str��ͨ�ַ���
Output:
return:  ȥ���ո��0x��ת��Ϊasciiָ��
****************************************/
string PrepareAsciiString(string str)
{
	//��ɾ���ո�
	int i = 0;
	while (i != str.length())
	{
		i = str.find(' ', i);
		if (i == -1)
			break;
		str = str.erase(i, 1);
	}

	//ɾ��0x
	i = 0;
	string tmp("0x");
	while (i != str.length())
	{
		i = str.find(tmp, i);
		if (i == -1)
			break;
		str = str.erase(i, 2);
	}

	//ɾ��0X
	i = 0;
	tmp = "0X";
	while (i != str.length())
	{
		i = str.find(tmp, i);
		if (i == -1)
			break;
		str = str.erase(i, 2);
	}
	if (str.length() % 2 != 0)
		str.append(1, '0');

	str = HexString2AsciiString(str);
	return str;
}