#include "HexTool.h"

using namespace std;

/***********************************
function: IsHexChar
desc:   �ж��ַ��Ƿ���0-9��a-f֮��
input�� char-�ַ�
Output��
return:  �ַ���������8-8��a-10��������ǣ�����-1
***********************************/
int IsHexChar(char ch)
{
	if ('0' <= ch && ch <= '9')
		return (int(ch) - int('0'));
	else if ('a' <= ch && ch <= 'f')
		return (int(ch) - int('a') + 10);
	else if ('A' <= ch && ch <= 'F')
		return (int(ch) - int('A') + 10);
	return -1;
}

/**************************************
function: Hex2Ascii
desc:     ���ַ���ת��ascii�루��"FF"->255, "01"->1��
Input:    hex-16���ƣ��ַ�����ʽ��2�ַ�
Output:
return:    ascii��
note:
**************************************/
unsigned char Hex2Ascii(const string &hex)
{
	assert(hex.length() == 2);
	int high = IsHexChar(hex[0]);
	int low = IsHexChar(hex[1]);
	if (high == -1 || low == -1)
		return '\0';
	// int ascii_val = high * 16 + low; // 0~255
	// cout << ascii_val << endl;
	unsigned char ascii_val = high * 16 + low; // 0~255
	return ascii_val;
}

/****************************************
function��HexString2Int
desc:  ���ַ���ת��Ϊ��ֵ(��"0100"->256)
Input:    16�����ַ���
Output:
return:  ��ֵ
****************************************/
int HexString2Int(string hex_str)
{
	if (hex_str.length() % 2 != 0)
		hex_str.insert(0, 1, '0');
	int idx = 0;
	int high, low = -1;
	int value = 0;
	while (idx < hex_str.length()) {
		high = IsHexChar(hex_str[idx + 0]);
		low = IsHexChar(hex_str[idx + 1]);
		if (high == -1 || low == -1) {
			value = -1;
			break;
		}
		value = (value << 8) + (high << 4) + low;
		idx += 2;
	}
	return value;
}

/****************************
function: HexString2AsciiString
desc:     16�����ַ���ת��Ϊascii�봮(��"FF01"-> {255, 1} ��ע:�˴��������ֽ�,����string����16������)
Input:    hex-16�����ַ���
Output:
return:  ascii�봮
*****************************/
string HexString2AsciiString(string hex_str)
{
	if (hex_str.length() % 2 != 0)
		hex_str.insert(0, 1, '0');
	string ascii_str;
	for (int i = 0; i<hex_str.length(); i += 2)
	{
		string tmp = hex_str.substr(i, 2);
		ascii_str.append(1, Hex2Ascii(tmp));
	}
	return ascii_str;
}

/****************************************
function��AsciiString2HexString
desc:	ascii�봮ת��Ϊ16�����ַ���(�磺{11, 1}->"0A01",ע���˴�Ϊ11��1�������ֽ� )
Input:	ascii�봮
Output:
return:	hex-16�����ַ���
****************************************/
string AsciiString2HexString(char *ascii_ptr, int len)
{
	const string hex_str = "0123456789abcdef";
	stringstream ss;

	for (string::size_type i = 0; i < len; ++i)
		ss << hex_str[(unsigned char)ascii_ptr[i] >> 4] << hex_str[(unsigned char)ascii_ptr[i] & 0xf];	//�Ⱥ�д���4λ�͵�4λ
	return ss.str();
}

/****************************************
function��Dec2HexString
desc:	ʮ������ת��Ϊ16�����ַ������磺128->"80"��
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
function��PrepareAsciiCmd
desc:  ȥ���ַ�����Ŀո�0x������ת��Ϊasciiָ��(��: "0xFF 0x01"-> {255, 1})
Input:    str��ͨ�ַ���
Output:
return:  ȥ���ո��0x����ת��Ϊasciiָ����
****************************************/
string PrepareAsciiCmd(string str)
{
	//ɾ���ո�
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
	assert(str.length() % 2 == 0);
	str = HexString2AsciiString(str);
	return str;
}