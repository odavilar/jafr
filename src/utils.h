#ifndef UTILS_H
#define UTILs_H

#include <iostream>
#include <iomanip>
extern "C" {
#include "serial.h"
}
#if (defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || (defined(__APPLE__) & defined(__MACH__)))
#include <cv.h>
#include <highgui.h>
#else
#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif
#include <cvblob.h>

using namespace std;
using namespace cvb;

int usage()
{
	cout<<"Usage: \n main.bin /dev/ttyPORT \n";
	return 0;
}

#endif
