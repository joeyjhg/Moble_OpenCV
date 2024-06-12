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
		//�� ���� ��ư

		cvui::init("Delivery Process");

		waitKey(20);

		if (cvui::button(ProcessMat, 0, 480, 320, 80, "Color Detection"))
			pProcess->Set_DETECT(Process::DETECT::COLOR);
		//�ܰ������� ��ư
		if (cvui::button(ProcessMat, 320, 480, 320, 80, "Contour Detection"))
			pProcess->Set_DETECT(Process::DETECT::CONTOUR);
		//��ȸ��ư
		if (cvui::button(ProcessMat, 320 * 2, 480, 320, 80, "Data Check"))
		{

			//�� �Լ� ������ ��ġ�ؼ� ��ġ ������ ã�Ƽ� �־��ָ�ɵ�.
			Showtrackbardata();
		}
		//�����ư 
		if (cvui::button(ProcessMat, 320 * 3, 480, 320, 80, "Quit"))
			return;


		bool res = pProcess->Update();
		if (res == false)
			break;
		pProcess->LateUpdate();


#ifdef __TEST__
		pProcess->Render(this);
#endif

		//�̹��������� 2�� ķ 
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

		//��ȸ��ư 
		//���� 
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

	HDC hdc = CreateCompatibleDC(NULL); //�ؽ�Ʈ �̹����� �����δ� �� ������

	HBRUSH hBrush = CreateSolidBrush(bkColor.rgb); //ä��� ����ε� bkColor�� �ܻ����� ä��°�

	//�ؽ�Ʈ �̹��� ũ�� ���ϴ°�
	RECT rect;
	rect.left = rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	//��Ʈ���� ������ ������ �����ϴ� �� ũ�⳪ ��
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
	SelectObject(hdc, hbmp); //hdc�� ����? �ϴ� ��

	FillRect(hdc, &rect, hBrush); //������ ũ�⸸ŭ �����ϰ� ä��°� (�� ä��)

	BITMAP bitmap;
	GetObject(hbmp, sizeof(BITMAP), &bitmap);

	//�ؽ�Ʈ �̹��� ���� �� ����� �� �ִ� ��Ʈ�� ����? �ϴ� �׷���
	HFONT hFont = CreateFontA(
		fontSize,
		0,
		0,
		0,
		fontWeight,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET, //�ѱ�� �Ϻ�� ���ְ� �ϴ°� (�Ƹ�)
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, //��Ƽ ���ϸ������ ���ִ°�
		VARIABLE_PITCH | FF_ROMAN,
		fontName);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, textColor.rgb);
	SetBkColor(hdc, bkColor.rgb);

	//����� ���� �̸� �ؽ�Ʈ�� ������ ���ϴ°�
	SIZE size;
	GetTextExtentPoint32A(hdc, text.c_str(), (int)text.length(), &size);

	TextOutA(hdc, 0, height / 3 * 1, text.c_str(), (int)text.length()); //�̹����� �ؽ�Ʈ ���°�
	int posX = (ori == 0 ? org.x : (ori == 1 ? org.x - (size.cx / 2) : org.x - size.cx)); //���� ���ϴ°� 0�� �ؽ�Ʈ�� ���� 1�� �ؽ�Ʈ�� �߰� 2�� �ؽ�Ʈ�� ������
	int posY = org.y - (size.cy / 2 + 5);

	//��Ʈ�� ������ OpenCV�̹����� �������ִ°�
	unsigned char* _tmp;
	unsigned char* _img;
	for (int y = 0; y < bitmap.bmHeight; y++) {
		if (posY + y >= 0 && posY + y < O_image.rows) {
			_img = O_image.data + (int)(3 * posX + (posY + y) * (((bitmap.bmBitsPixel / 8) * O_image.cols) & ~3));
			_tmp = (unsigned char*)(bitmap.bmBits) + (int)((bitmap.bmHeight - y - 1) * (((bitmap.bmBitsPixel / 8) * bitmap.bmWidth) & ~3));
			for (int x = 0; x < bitmap.bmWidth; x++) {
				if (x + posX >= O_image.cols)
					break;

				if (_tmp[0] != bkColor.b || _tmp[0] != bkColor.g || _tmp[0] != bkColor.r) { //�ؽ�Ʈ �̹����� ��� �÷��� ���ֱ� ���� ��, bgr ������ �ϴ� ������ Mat �̹����� ó���� ���� �� BGR ���̿���
					_img[0] = (unsigned char)_tmp[0]; //B
					_img[1] = (unsigned char)_tmp[1]; //G
					_img[2] = (unsigned char)_tmp[2]; //R
				}
				_img += 3;
				_tmp += 3;
			}
		}
	}

	//�޸𸮿��� �������ִ°� �̰� ���ϸ� �޸� ��� �����
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


	string str = "hi";	//������  [hi] <->  sender
	const char* cstr = str.c_str();

	string astr = "ai";	// �޴� �� [ai ]<-> receiver
	const char* acstr = astr.c_str();
	string bstr = "bi";	//�ּ� [bi] <-> address
	const char* bcstr = bstr.c_str();
	string dstr = "ci";	//���� [ci] <-> contents
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
				//��� ���տ��� �� �� �����ͼ� ó��
				while ((Row = mysql_fetch_row(Result)) != NULL) {
					checkkey = true;
					//�� ó��
					//Sender
					CPputText(datawindow, pProcess->utf8toansi(Row[0]), Point(X, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//receiver
					CPputText(datawindow, pProcess->utf8toansi(Row[1]), Point(X + 60, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//Address
					string address = pProcess->utf8toansi(Row[3]) + pProcess->utf8toansi(Row[4]) + pProcess->utf8toansi(Row[5]);
					CPputText(datawindow, address, Point(X + 260, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//content
					CPputText(datawindow, pProcess->utf8toansi(Row[2]), Point(X + 525, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//etc.
					CPputText(datawindow, pProcess->utf8toansi(Row[8]), Point(X + 695, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					cv::line(datawindow, Point(40, 80 + Y * i), Point(850, 80 + Y * i), cv::Scalar(255, 255, 255), 1);
					i++;
					if (i == textsize)
						break;
				}
				if (waitKey(20) == 27)
				{
					//��� ���� �޸� ����
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
				//��� ���տ��� �� �� �����ͼ� ó��
				while ((Row = mysql_fetch_row(Result)) != NULL) {
					if (i >= textsize)
					{
						int j = i % textsize;
						//�� ó��
						//Sender
						CPputText(datawindow, pProcess->utf8toansi(Row[0]), Point(X, 60 + Y * j), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//receiver
						CPputText(datawindow, pProcess->utf8toansi(Row[1]), Point(X + 60, 60 + Y * j), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//Address
						string address = pProcess->utf8toansi(Row[3]) + pProcess->utf8toansi(Row[4]) + pProcess->utf8toansi(Row[5]);
						CPputText(datawindow, address, Point(X + 260, 60 + Y * j), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//content
						CPputText(datawindow, pProcess->utf8toansi(Row[2]), Point(X + 525, 60 + Y * j), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						//etc.
						CPputText(datawindow, pProcess->utf8toansi(Row[8]), Point(X + 695, 60 + Y * j), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
						cv::line(datawindow, Point(40, 80 + Y * j), Point(850, 80 + Y * j), cv::Scalar(255, 255, 255), 1);
						if (i == textsize * 2)
							break;
					}
					i++;
				}
				if (waitKey(20) == 27)
				{
					//��� ���� �޸� ����
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
				//��� ���տ��� �� �� �����ͼ� ó��
				while ((Row = mysql_fetch_row(Result)) != NULL) {
					checkkey = true;
					//�� ó��
					//Sender
					CPputText(datawindow, pProcess->utf8toansi(Row[0]), Point(X, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//receiver
					CPputText(datawindow, pProcess->utf8toansi(Row[1]), Point(X + 60, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//Address
					string address = pProcess->utf8toansi(Row[3]) + pProcess->utf8toansi(Row[4]) + pProcess->utf8toansi(Row[5]);
					CPputText(datawindow, address, Point(X + 260, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//content
					CPputText(datawindow, pProcess->utf8toansi(Row[2]), Point(X + 525, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					//etc.
					CPputText(datawindow, pProcess->utf8toansi(Row[8]), Point(X + 695, 60 + Y * i), 1, "���� ���", FW_BOLD, 3, RGBScale(0xFF, 0xFF, 0xFF), RGBScale(100, 100, 100));
					cv::line(datawindow, Point(40, 80 + Y * i), Point(850, 80 + Y * i), cv::Scalar(255, 255, 255), 1);
					i++;
					if (i == textsize)
						break;
				}
				if (waitKey(20) == 27)
				{
					//��� ���� �޸� ����
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


