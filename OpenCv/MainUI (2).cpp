#include "Include.h"
#include "Process.h"
#include "MainUI.h"

#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define __TEST__


void MainUI::Mainmenu()
{
	cv::Mat main = cv::Mat(400, 500, CV_8UC3);
	cv::Mat ProcessMat = cv::Mat(900, 1600, CV_8UC3);
	
	cvui::init("Moble Delivery");

	

	while (true)
	{
		cvui::text(main, 60, 120, "Moble Delivery Service", 1, 0xffffff);
		cvui::imshow("Moble Delivery", main);

		waitKey(20);
		if (cvui::button(main, 200, 200, "Start"))
			break;
		else if (cvui::button(main, 270, 200, "Quit"))
			break;
	}

	pProcess = new Process();
	pProcess->Initialize();

	while (true)
	{
		//색 검출 버튼
		
		cvui::init("Delivery Process");

		waitKey(20);

		if (cvui::button(ProcessMat, 250, 700, 220,80 ,"Color Detection"))
			pProcess->Set_DETECT(Process::DETECT::COLOR);
		//외곽선검출 버튼
		if (cvui::button(ProcessMat, 550, 700, 220, 80, "Contour Detection"))
			pProcess->Set_DETECT(Process::DETECT::CONTOUR);
		//조회버튼
		if (cvui::button(ProcessMat, 850, 700, 220, 80, "Data Check"))
			return;
		//종료버튼 
		if (cvui::button(ProcessMat, 1150, 700, 220, 80, "Quit"))
			return;


		bool res = pProcess->Update();
		if (res == false)
			break;
		pProcess->LateUpdate();


#ifdef __TEST__
		pProcess->Render();
#endif

		//이미지렌더링 2개 캠 
		if (pProcess->Get_DETECT() != Process::DETECTEND)
		{
			cam0 = pProcess->Get_Cam0();
			cam1 = pProcess->Get_Cam1();
			int pix = cam0.rows;
			cvui::image(ProcessMat, 80, 50, cam0);
			cvui::image(ProcessMat, 400+ pix, 50, cam1);
		}
		else
		{
			cam0 = imread("sig.jpg");
			resize(cam0,cam0,Size(400, 300));
			cam1 = imread("sig.jpg");
			resize(cam1, cam1, Size(400, 300));
			cvui::image(ProcessMat, 100, 50, cam0);
			cvui::image(ProcessMat, 900, 50, cam1);
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

