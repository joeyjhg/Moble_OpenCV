#pragma once

class Process;

class MainUI {


public:
	struct RGBScale {
		int r = 0;
		int g = 0;
		int b = 0;
		int rgb = 0;

		RGBScale(int r, int g, int b) {
			this->r = r;
			this->g = g;
			this->b = b;

			this->rgb = RGB(r, g, b);
		}
	};

public:
	void Mainmenu();
	void Showdata(INFO input);
	void CPputText(cv::Mat& O_image, cv::String text, cv::Point org, int ori, const char* fontName, int fontWeight, double fontScale, RGBScale textColor, RGBScale bkColor);
	void Show_sizedatas(Mat window, double value1, double value2, double value3);
	void Show_data(Mat window, String title, String value, int x, int y);

	// data보여주는 창
	void ShowData(Mat& mat);
	void Showtrackbardata();



public:

	Process* pProcess = new Process();



public:
	cv::Mat cam0;
	cv::Mat cam1;
};