/*******************************************************
进制转换工具类

Created by kyn0v.
2020.08.15
********************************************************/

#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <assert.h>

using std::string;

int IsHex(char hc);

unsigned char Hex2Ascii(const string & hex);

int HexString2Int(string hexstr);

string HexString2AsciiString(const string & hex);

string AsciiString2HexString(char * data, int len);

string Dec2HexString(int dec, int length);

string PrepareAsciiString(string str);
