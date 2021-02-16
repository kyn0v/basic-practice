#include "HexTool.h"

using namespace std;

/***********************************
function: IsHexChar
desc:   判断字符是否在0-9，a-f之间
input： char-字符
Output：
return:  字符索引，如8-8，a-10，如果不是，返回-1
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
desc:     短字符串转化ascii码（如"FF"->255, "01"->1）
Input:    hex-16进制，字符串形式，2字符
Output:
return:    ascii码
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
function：HexString2Int
desc:  长字符串转化为数值(如"0100"->256)
Input:    16进制字符串
Output:
return:  数值
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
desc:     16进制字符串转化为ascii码串(如"FF01"-> {255, 1} ，注:此处共两个字节,即用string保存16进制数)
Input:    hex-16进制字符串
Output:
return:  ascii码串
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
function：AsciiString2HexString
desc:	ascii码串转化为16进制字符串(如：{11, 1}->"0A01",注：此处为11和1共两个字节 )
Input:	ascii码串
Output:
return:	hex-16进制字符串
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
function：Dec2HexString
desc:	十进制数转化为16进制字符串（如：128->"80"）
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
function：PrepareAsciiCmd
desc:  去掉字符串里的空格，0x。将其转换为ascii指令(如: "0xFF 0x01"-> {255, 1})
Input:    str普通字符串
Output:
return:  去掉空格和0x，并转换为ascii指令码
****************************************/
string PrepareAsciiCmd(string str)
{
	//删除空格
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
	assert(str.length() % 2 == 0);
	str = HexString2AsciiString(str);
	return str;
}