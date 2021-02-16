/***********************************
进制转换工具

Written by kynov.

2020.09.17
***********************************/

#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <assert.h>

using std::string;

int IsHexChar(char hc);

unsigned char Hex2Ascii(const string & hex);

int HexString2Int(string hexstr);

string HexString2AsciiString(string hex);

string AsciiString2HexString(char * data, int len);

string Dec2HexString(int dec, int length);

string PrepareAsciiCmd(string str);