#include "HexTool.h"

using namespace std;

/***********************************
function: IsHexChar
desc:   判断字符是否在0-9，a-f之间
input： char-字符
Output：
return:  字符索引，如8-8，a-10，如果
不是，返回-1
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
desc:     16进制字符串转化ascii码
Input:    hex-16进制，字符串形式，2字符
Output:
return:    ascii码
note: 这里的ascii码可直接转换为int型数值
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
function：HexString2Int
desc:  16进制字符串转化为数值
Input:    16进制字符串
Output:
return:  数值
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
desc:     16进制字符串转化为ascii码串
Input:    hex-16进制字符串
Output:
return:  ascii码串
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
function：AsciiString2HexString
desc:	ascii码串转化为16进制字符串
Input:	ascii码串
Output:
return:	hex-16进制字符串
****************************************/
string AsciiString2HexString(char *data, int len)
{
	const string hex = "0123456789abcdef";
	stringstream ss;

	for (string::size_type i = 0; i < len; ++i)
		ss << hex[(unsigned char)data[i] >> 4] << hex[(unsigned char)data[i] & 0xf];	//先后写入高4位和低4位
	return ss.str();
}


/****************************************
function：Dec2HexString
desc:	十进制数转化为16进制字符串
Input:	十进制数dec, 生成的16进制字符串长度
Output:
return:	16进制字符串
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
sprintf(s, "%08x", num) 亦可实现输出补0的8位16进制数
*/

/****************************************
function：PrepareAsciiString
desc:  去掉字符串里的空格，0x。将其转换为ascii指令
Input:    str普通字符串
Output:
return:  去掉空格和0x并转换为ascii指令
****************************************/
string PrepareAsciiString(string str)
{
	//先删除空格
	int i = 0;
	while (i != str.length())
	{
		i = str.find(' ', i);
		if (i == -1)
			break;
		str = str.erase(i, 1);
	}

	//删除0x
	i = 0;
	string tmp("0x");
	while (i != str.length())
	{
		i = str.find(tmp, i);
		if (i == -1)
			break;
		str = str.erase(i, 2);
	}

	//删除0X
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