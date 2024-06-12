#include "Include.h"
#include "Process.h"
#include "MainUI.h"

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define __TEST__

void MainUI::Mainmenu()
{
	cv::Mat main = cv::Mat(400, 500, CV_8UC3);
	cv::Mat ProcessMat = cv::Mat(560, 1280, CV_8UC3);

	cvui::init("Moble Delivery");


	while (true)
	{
		Mat back = imread("back.png");
		resize(back, back, Size(500, 400));
		cvui::image(main, 0, 0, back);
		cvui::text(main, 80, 100, "Moble Delivery Service", 1, 0xffffff);

		waitKey(20);
		if (cvui::button(main, 100, 300, 100, 50, "Start"))
			break;
		else if (cvui::button(main, 300, 300, 100, 50, "Quit"))
			break;

		cvui::imshow("Moble Delivery", main);
	}

	pProcess = new Process();
	pProcess->Initialize();

	while (true)
	{
		//색 검출 버튼

		cvui::init("Delivery Process");

		waitKey(20);

		if (cvui::button(ProcessMat, 0, 480, 320, 80, "Color Detection"))
			pProcess->Set_DETECT(Process::DETECT::COLOR);
		//외곽선검출 버튼
		if (cvui::button(ProcessMat, 320, 480, 320, 80, "Contour Detection"))
			pProcess->Set_DETECT(Process::DETECT::CONTOUR);
		//조회버튼
		if (cvui::button(ProcessMat, 320 * 2, 480, 320, 80, "Data Check"))
		{

			//이 함수 내에서 서치해서 위치 적당히 찾아서 넣어주면될듯.
			Showtrackbardata();
		}
		//종료버튼 
		if (cvui::button(ProcessMat, 320 * 3, 480, 320, 80, "Quit"))
			return;


		bool res = pProcess->Update();
		if (res == false)
			break;
		pProcess->LateUpdate();


#ifdef __TEST__
		pProcess->Render(this);
#endif

		//이미지렌더링 2개 캠 
		if (pProcess->Get_DETECT() != Process::DETECTEND)
		{
			cam0 = pProcess->Get_Cam0();
			cam1 = pProcess->Get_Cam1();
			cvui::image(ProcessMat, 0, 0, cam0);
			cvui::image(ProcessMat, 640, 0, cam1);
		}
		else
		{
			cam0 = imread("sig.jpg");
			resize(cam0, cam0, Size(640, 480));
			cam1 = imread("sig.jpg");
			resize(cam1, cam1, Size(640, 480));
			cvui::image(ProcessMat, 0, 0, cam0);
			cvui::image(ProcessMat, 640, 0, cam1);
		}

		//조회버튼 
		//종료 
		cvui::imshow("Delivery Process", ProcessMat);

	}



	Safe_Delete(pProcess);

}


void MainUI::Showdata(INFO input) {
	cv::Mat datawindow = cv::Mat(400, 500, CV_8UC3);
	cvui::init("Moble Delivery");
	while (1) {

		cvui::text(datawindow, 60, 70, input.strdo, 0.7, 0xffffff);
		cvui::text(datawindow, 60, 110, input.strsi, 0.7, 0xffffff);
		cvui::text(datawindow, 60, 150, input.strroad, 0.7, 0xffffff);
		cvui::text(datawindow, 60, 190, input.strnum, 0.7, 0xffffff);
	}
}
void MainUI::Show_sizedatas(Mat window, double value1, double value2, double value3) {
	//cv::Mat Window = cv::Mat(300, 400, CV_8UC3);

	waitKey(20);
	int inital_x = 30, inital_y = 180;

	Show_data(window, "Area", to_string(floor(value1 * 10.f + 0.5) / 10.f) + "cm", inital_x, inital_y);
	Show_data(window, "Width ", to_string(floor(value2 * 10.f + 0.5) / 10.f) + "cm", inital_x, inital_y + 40);
	Show_data(window, "Height", to_string(floor(value3 * 10.f + 0.5) / 10.f) + "cm", inital_x, inital_y + 80);
}



void MainUI::CPputText(cv::Mat& O_image, cv::String text, cv::Point org, int ori, const char* fontName, int fontWeight, double fontScale, RGBScale textColor, RGBScale bkColor) {
	int fontSize = (int)(7 * fontScale);
	int width = O_image.cols;
	int height = fontSize * 3 / 2;

	HDC hdc = CreateCompatibleDC(NULL); //텍스트 이미지를 만들어두는 곳 같은거

	HBRUSH hBrush = CreateSolidBrush(bkColor.rgb); //채우는 방식인데 bkColor로 단색으로 채우는거

	//텍스트 이미지 크기 정하는거
	RECT rect;
	rect.left = rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	//비트맵의 구조를 사전에 정의하는 것 크기나 색
	BITMAPINFOHEADER header;
	ZeroMemory(&header, sizeof(BITMAPINFOHEADER));
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = width;
	header.biHeight = height;
	header.biPlanes = 1;
	header.biBitCount = 24;
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader = header;
	HBITMAP hbmp = CreateDIBSection(NULL, (LPBITMAPINFO)&bitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0);
	SelectObject(hdc, hbmp); //hdc에 적용? 하는 거

	FillRect(hdc, &rect, hBrush); //지정한 크기만큼 완전하게 채우는거 (다 채움)

	BITMAP bitmap;
	GetObject(hbmp, sizeof(BITMAP), &bitmap);

	//텍스트 이미지 만들 때 사용할 수 있는 폰트를 생성? 하는 그런거
	HFONT hFont = CreateFontA(
		fontSize,
		0,
		0,
		0,
		fontWeight,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET, //한국어나 일본어나 해주게 하는거 (아마)
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, //안티 에일리어싱을 켜주는거
		VARIABLE_PITCH | FF_ROMAN,
		fontName);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, textColor.rgb);
	SetBkColor(hdc, bkColor.rgb);

	//계산을 위해 미리 텍스트의 사이즈 구하는거
	SIZE size;
	GetTextExtentPoint32A(hdc, text.c_str(), (int)text.length(), &size);

	TextOutA(hdc, 0, height / 3 * 1, text.c_str(), (int)text.length()); //이미지에 텍스트 적는거
	int posX = (ori == 0 ? org.x : (ori == 1 ? org.x - (size.cx / 2) : org.x - size.cx)); //기준 정하는거 0은 텍스트의 왼쪽 1은 텍스트의 중간 2는 텍스트의 오른쪽
	int posY = org.y - (size.cy / 2 + 5);

	//비트맵 사진을 OpenCV이미지에 삽입해주는거
	unsigned char* _tmp;
	unsigned char* _img;
	for (int y = 0; y < bitmap.bmHeight; y++) {
		if (posY + y >= 0 && posY + y < O_image.rows) {
			_img = O_image.data + (int)(3 * posX + (posY + y) * (((bitmap.bmBitsPixel / 8) * O_image.cols) & ~3));
			_tmp = (unsigned char*)(bitmap.bmBits) + (int)((bitmap.bmHeight - y - 1) * (((bitmap.bmBitsPixel / 8) * bitmap.bmWidth) & ~3));
			for (int x = 0; x < bitmap.bmWidth; x++) {
				if (x + posX >= O_image.cols)
					break;

				if (_tmp[0] != bkColor.b || _tmp[0] != bkColor.g || _tmp[0] != bkColor.r) { //텍스트 이미지의 배경 컬러는 없애기 위한 것, bgr 순서로 하는 이유는 Mat 이미지를 처음에 만들 때 BGR 순이여서
					_img[0] = (unsigned char)_tmp[0]; //B
					_img[1] = (unsigned char)_tmp[1]; //G
					_img[2] = (unsigned char)_tmp[2]; //R
				}
				_img += 3;
				_tmp += 3;
			}
		}
	}

	//메모리에서 삭제해주는거 이거 안하면 메모리 계속 사용함
	DeleteObject(hBrush);
	DeleteObject(hFont);
	DeleteObject(hbmp);
	DeleteObject(hdc);
}





void  MainUI::ShowData(Mat& mat)
{
	mat = cv::Mat(620, 900, CV_8UC3);
	cvui::window(mat, 40, 30, 60, 410, "Sender");
	cvui::window(mat, 100, 30, 60, 410, "receiver");
	cvui::window(mat, 160, 30, 350, 410, "Address");
	cvui::window(mat, 510, 30, 170, 410, "contents.");
	cvui::window(mat, 680, 30, 170, 410, "etc");
	if (cvui::button(mat, 50, 480, 100, 80, "Load"))
		int a = 0;
	if (cvui::button(mat, 200, 450, 100, 20, "previous"))
		int a = 0;
	if (cvui::button(mat, 420, 450, 100, 20, "next"))
		int a = 0;




}


void  MainUI::Showtrackbardata() {

	cvui::init("hoho");
	bool checked = false;
	int i = 0;


	string str = "hi";	//보낸분  [hi] <->  sender
	const char* cstr = str.c_str();

	string astr = "ai";	// 받는 분 [ai ]<-> receiver
	const char* acstr = astr.c_str();
	string bstr = "bi";	//주소 [bi] <-> address
	const char* bcstr = bstr.c_str();
	string dstr = "ci";	//내용 [ci] <-> contents
	const char* dcstr = dstr.c_str();

	cv::Mat datawindow = cv::Mat(620, 900, CV_8UC3);
	cvui::window(datawindow, 40, 30, 60, 410, "Sender");
	cvui::window(datawindow, 100, 30, 60, 410, "receiver");
	cvui::window(datawindow, 160, 30, 350, 410, "Address");
	cvui::window(datawindow, 510, 30, 170, 410, "contents.");
	cvui::window(datawindow, 680, 30, 170, 410, "etc");

	int X = 70;
	int Y = 30;
	bool checkkey = false;
	int j = 18;
	bool checkpress = false;

	int textsize = 13;

	while (true)
	{

		checkpress = false;
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			checkpress = true;

		int key = waitKey(20);

		if (cvui::button(datawindow, 200, 450, 100, 20, "previous") && checkpress == true && checkkey == true)
		{
			MYSQL_RES* Result = pProcess->SQL_Select_All();

			int i = 0;
			ShowData(datawindow);
			if (Result != NULL) {
				MYSQL_ROW Row;
				//결과 집합에서 각 행 가져와서 처리
				while ((Row = mysql_fetch_row(Result)) != NULL) {
					checkkey = true;
					//행 처리
					//Sender
					CPputText(datawindow, pProcess->utf8toansi(Row[0]), Point(X, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//receiver
					CPputText(datawindow, pProcess->utf8toansi(Row[1]), Point(X + 60, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//Address
					string address = pProcess->utf8toansi(Row[3]) + pProcess->utf8toansi(Row[4]) + pProcess->utf8toansi(Row[5]);
					CPputText(datawindow, address, Point(X + 260, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//content
					CPputText(datawindow, pProcess->utf8toansi(Row[2]), Point(X + 525, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//etc.
					CPputText(datawindow, pProcess->utf8toansi(Row[8]), Point(X + 695, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					cv::line(datawindow, Point(40, 80 + Y * i), Point(850, 80 + Y * i), cv::Scalar(255, 255, 255), 1);
					i++;
					if (i == textsize)
						break;
				}
				if (waitKey(20) == 27)
				{
					//결과 집합 메모리 해제
					mysql_free_result(Result);
					break;
				}

			}
		}


		if (cvui::button(datawindow, 420, 450, 100, 20, "next") && checkpress == true && checkkey == true)
		{
			MYSQL_RES* Result = pProcess->SQL_Select_All();

			int i = 0;
			ShowData(datawindow);
			if (Result != NULL) {
				MYSQL_ROW Row;
				//결과 집합에서 각 행 가져와서 처리
				while ((Row = mysql_fetch_row(Result)) != NULL) {
					if (i >= textsize)
					{
						int j = i % textsize;
						//행 처리
						//Sender
						CPputText(datawindow, pProcess->utf8toansi(Row[0]), Point(X, 60 + Y * j), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//receiver
						CPputText(datawindow, pProcess->utf8toansi(Row[1]), Point(X + 60, 60 + Y * j), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//Address
						string address = pProcess->utf8toansi(Row[3]) + pProcess->utf8toansi(Row[4]) + pProcess->utf8toansi(Row[5]);
						CPputText(datawindow, address, Point(X + 260, 60 + Y * j), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//content
						CPputText(datawindow, pProcess->utf8toansi(Row[2]), Point(X + 525, 60 + Y * j), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//etc.
						CPputText(datawindow, pProcess->utf8toansi(Row[8]), Point(X + 695, 60 + Y * j), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						cv::line(datawindow, Point(40, 80 + Y * j), Point(850, 80 + Y * j), cv::Scalar(255, 255, 255), 1);
						if (i == textsize * 2)
							break;
					}
					i++;
				}
				if (waitKey(20) == 27)
				{
					//결과 집합 메모리 해제
					mysql_free_result(Result);
					break;
				}

			}
		}



		if (cvui::button(datawindow, 50, 480, 100, 80, "Load") && checkpress == true)
		{
			MYSQL_RES* Result = pProcess->SQL_Select_All();

			int i = 0;
			ShowData(datawindow);
			if (Result != NULL) {
				MYSQL_ROW Row;
				//결과 집합에서 각 행 가져와서 처리
				while ((Row = mysql_fetch_row(Result)) != NULL) {
					checkkey = true;
					//행 처리
					//Sender
					CPputText(datawindow, pProcess->utf8toansi(Row[0]), Point(X, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//receiver
					CPputText(datawindow, pProcess->utf8toansi(Row[1]), Point(X + 60, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//Address
					string address = pProcess->utf8toansi(Row[3]) + pProcess->utf8toansi(Row[4]) + pProcess->utf8toansi(Row[5]);
					CPputText(datawindow, address, Point(X + 260, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//content
					CPputText(datawindow, pProcess->utf8toansi(Row[2]), Point(X + 525, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//etc.
					CPputText(datawindow, pProcess->utf8toansi(Row[8]), Point(X + 695, 60 + Y * i), 1, "맑은 고딕", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					cv::line(datawindow, Point(40, 80 + Y * i), Point(850, 80 + Y * i), cv::Scalar(255, 255, 255), 1);
					i++;
					if (i == textsize)
						break;
				}
				if (waitKey(20) == 27)
				{
					//결과 집합 메모리 해제
					mysql_free_result(Result);
					break;
				}

			}
		}
		if (cvui::button(datawindow, 600, 500, 100, 20, "Quit") && checkpress == true)
		{
			destroyWindow("hoho");
			break;

		}

		imshow("hoho", datawindow);

	}
}



void MainUI::Show_data(Mat window, String title, String value, int x, int y) {
	cvui::window(window, x, y, 100, 40, title);
	cvui::text(window, x + 10, y + 25, value);
}


