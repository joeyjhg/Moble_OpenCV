#pragma once

class Process;

class MainUI {

public:
	void Mainmenu();
	void Showdata(INFO input);

public:

	Process* pProcess = new Process();



public:
	cv::Mat cam0;
	cv::Mat cam1;
};