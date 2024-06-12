#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>
#include<crtdbg.h>
#include <opencv2/opencv.hpp>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <math.h>
//#include <my_global.h>
#include <mysql.h>
#include <codecvt>

#include "enum.h"
#include "struct.h"

#pragma warning (disable:6031)
#pragma warning (disable:4996)
#pragma comment(lib,"libmySQL.lib")

#define _CRT_SECURE_NO_WARNINGS


#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif


using namespace cv;
using namespace std;




template <typename T>
void Safe_Delete(T& pointer)
{
	if (NULL != pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}

template <typename T>
void Safe_Delete_Array(T& pointer)
{
	if (NULL != pointer)
	{
		delete[] pointer;
		pointer = NULL;
	}
}


