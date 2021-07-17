#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <iostream>
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

/* 通用工具
*/
class Common 
{
public:
	static int min(int a, int b)
	{
		return (a < b) ? a : b;
	}
	static int max(int a, int b)
	{
		return (a < b) ? b : a;
	}
	static int min(uint32 a, uint32 b)
	{
		return (a < b) ? a : b;
	}
};

