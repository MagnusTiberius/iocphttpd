#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
using namespace std;

class LogCtrl
{
public:

	static void LogIt(const char* msg)
	{
		ofstream myfile;
		myfile.open("application.log");
		myfile << msg;
		myfile.close();
	}

};