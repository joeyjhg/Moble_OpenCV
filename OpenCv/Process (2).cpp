#include "Include.h"
#include "Process.h"

Process::Process()
{
}

Process::~Process()
{
	vecInfo.clear();
	cap.release();
	ResetContour();
	//mysql_close(ConnPtr);
}

void Process::Initialize()
{
	oldCP = 949;
	Minval = 10000;
	cap.open(0);
	cap2.open(1);
	bqr = false;
	detectmethod = DETECT::DETECTEND;
	//SQL_Connection();
}

//주기적으로 돌아갈것들
bool Process::Update()
{
	if (Key == 27)
		return false;

	if (Key == 0x250000)
		testvaluemin -= 10;
	if (Key == 0x270000)
		testvaluemax += 10;
	if (Key == 0x260000)
		testvaluemin += 10;
	if (Key == 0x280000)
		testvaluemax -= 10;

	if (bqr == true && cqr == true)
	{
		cqr = false;
		bqr = false;
		ResetContour();
	}

	cap >> img;
	cap2 >> qrimg;

	switch (detectmethod)
	{
	case DETECT::COLOR:
		if (cqr == false && bqr == true)
			cqr = MakeMask(img);
		break;
	case DETECT::CONTOUR:
		if (cqr == false && bqr == true)
			cqr = CheckBoxContour(img);
		break;
	}

	if (bqr == false)
		ReadQR();



	return true;
}

//업데이트 이후로 돌아갈것들 
void Process::LateUpdate()
{
	if (cqr == true && bqr == true)
		MakeInfo(qrinfo, strdo, strsi, strroad, strnum, sender, receiver, contents, bsize);

	++rndframe;
}

//이미지 출력 
void Process::Show()
{
	imshow("test", img);
	imshow("qr", qrimg);

	Key = waitKeyEx(1);
}

//가장 큰 컨투어 찾기 
int Process::findMaxArea(vector<vector<cv::Point>> contours)
{
	int max_area = -1;
	int max_index = -1;

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);

		if (area < Minval)
			continue;

		if (area > max_area) {
			max_area = area;
			max_index = i;
		}
	}
	return max_index;
}

void  Process::getRectangleDimensions(const std::vector<cv::Point>& pts) {
	RotatedRect rect = minAreaRect(pts);
	Point2f box[4];
	rect.points(box);
	curwidth = norm(box[0] - box[1]);
	curheight = norm(box[1] - box[2]);
}

//컨투어 찾기 
bool Process::CheckBoxContour(Mat img_input)
{
	fullheight = img_input.rows;
	fullwidth = img_input.cols;


	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	Mat img_canny;
	Canny(img_gray, img_canny, testvaluemin, testvaluemax);

	testimg = img_canny.clone();

	Mat kernel = getStructuringElement(MORPH_ELLIPSE,
		Size(5, 5));
	morphologyEx(img_canny, img_canny, MORPH_CLOSE, kernel);


	findContours(img_canny, contours, RETR_LIST,
		CHAIN_APPROX_SIMPLE);

	int max_index = findMaxArea(contours);
	if (max_index < 0)
		return false;

	vector <Point> max_contour = contours[max_index];
	curcontour = max_contour;

	approxPolyDP(Mat(curcontour), curcontour,
		arcLength(Mat(curcontour), true) * 0.02, true);

	int vtc = curcontour.size();

	if (vtc == 4)
	{
		getRectangleDimensions(curcontour);

		curArea = contourArea(curcontour);
		if (curArea > 25400 && curArea < 33400)
			bsize = BOXSIZE::SMALL;
		else if (curArea > 38140 && curArea < 46140)
			bsize = BOXSIZE::MIDIUM;
		else if (curArea > 52800 && curArea < 62800)
			bsize = BOXSIZE::LARGE;
		else
			return false;

		resultcontours.push_back(curcontour);
		return true;
	}
	else
		return false;


}

//컨투어 컨테이너 초기화 
void Process::ResetContour()
{
	contours.clear();
	resultcontours.clear();
	resultcontours2.clear();
}

void Process::RealSize()
{

}


//qr읽기
void Process::ReadQR()
{
	qrinfo = detector.detectAndDecode(qrimg, points);

	if (!qrinfo.empty() && qrinfo != laststr)
	{
		bqr = true;

		size_t pos = 0;

		string tempstr = qrinfo;

		pos = tempstr.find(">>");
		sender = tempstr.substr(0, pos);
		tempstr.erase(0, pos + 3);

		pos = tempstr.find("<<");
		receiver = tempstr.substr(0, pos);
		tempstr.erase(0, pos + 3);

		pos = tempstr.find("[]");
		contents = tempstr.substr(0, pos);
		tempstr.erase(0, pos + 3);

		pos = tempstr.find("!");
		strdo = tempstr.substr(0, pos);
		tempstr.erase(0, pos + 1);

		pos = tempstr.find("@");
		strsi = tempstr.substr(0, pos);
		tempstr.erase(0, pos + 1);

		pos = tempstr.find("#");
		strroad = tempstr.substr(0, pos);
		tempstr.erase(0, pos + 1);

		strnum = tempstr;


		laststr = qrinfo;

	}
}

//큐알기반 데이터 생성 
void Process::MakeInfo(string& all, string strdo, string strsi, string strroad, string strnum, string send, string recieve, string content, BOXSIZE sz)
{
	INFO test;
	test.all = all;
	test.strdo = strdo;
	test.strsi = strsi;
	test.strroad = strroad;
	test.strnum = strnum;
	test.reciever = recieve;
	test.sender = send;
	test.contents = content;
	test.size = sz;
	AllotPrice(test);
	vecInfo.emplace_back(test);
}


//void Process::SQL_Insert(string sender, string receiver, string contents, string strdo, string strsi, string strroad, string strnum)
//{
//	MYSQL Conn;
//	MYSQL* ConnPtr = NULL;
//	MYSQL_RES* Result;
//	MYSQL_ROW Row;
//	int Stat;
//
//	mysql_init(&Conn);
//
//	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "0000", "moble_box_information", 3306, (char*)NULL, 0);
//	mysql_set_character_set(ConnPtr, "euckr");
//	if (ConnPtr == NULL)
//	{
//		fprintf(stderr, "error : %s", mysql_error(&Conn));
//		return;
//	}
//	mysql_query(ConnPtr, "SET NAMES utf8mb4");
//	mysql_query(ConnPtr, "SET CHARACTER SET utf8mb4");
//	mysql_query(ConnPtr, "SET SESSION collation_connection = 'utf8mb4_unicode_ci'");
//
//	// 새로운 레코드를 추가하는 INSERT 문
//	string InsertQuery = "INSERT INTO qr_information (sender, receiver, contents, strdo, strsi, strroad, strnum) VALUES ('" + sender + "', '" + receiver + "', '" + contents + "', '" + strdo + "', '" + strsi + "', '" + strroad + "', '" + strnum + "')";
//	Stat = mysql_query(ConnPtr, InsertQuery.c_str());
//	if (Stat != 0) {
//		fprintf(stderr, "error : %s", mysql_error(&Conn));
//		return;
//	}
//	else {
//		printf("new data sucsess\n");
//	}
//
//	mysql_close(ConnPtr);
//}



Process::DETECT Process::Get_DETECT()
{
	return detectmethod;
}

Mat Process::Get_Cam0()
{
	return img;
}

Mat Process::Get_Cam1()
{
	return qrimg;
}

void Process::Set_DETECT(DETECT d)
{
	detectmethod = d;
}

void Process::AllotPrice(INFO& info)
{
	int baseprice = 4000;
	switch (info.size)
	{
	case LARGE:
		info.price = AllotPricereach(info.strdo, baseprice) * 1.5;
		break;
	case MIDIUM:
		info.price = AllotPricereach(info.strdo, baseprice) * 1.2;
		break;
	case SMALL:
		info.price = AllotPricereach(info.strdo, baseprice) * 1;
		break;
	default:
		break;
	}
}


string Process::ansi_to_utf8(string& ansi)
{
	WCHAR unicode[1500];
	char utf8[1500];

	memset(unicode, 0, sizeof(unicode));
	memset(utf8, 0, sizeof(utf8));

	::MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, unicode, sizeof(unicode));
	::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, sizeof(utf8), NULL, NULL);

	return string(utf8);
}

int Process::AllotPricereach(string si, int sizeprice)
{
	string test = "서울특별시";
	string test1 = "강원도";
	string test2 = "경기도";
	string test13 = "충청도";
	string test3 = "경상도";
	string test4 = "전라도";
	string test5 = "제주도";
	string test6 = "인천광역시";
	string test7 = "대전광역시";
	string test8 = "대구광역시";
	string test9 = "울산광역시";
	string test10 = "광주광역시";
	string test11 = "부산광역시";
	string test12 = "세종시";

	if (si == ansi_to_utf8(test))
		return sizeprice;
	else if (si == ansi_to_utf8(test1))
		return sizeprice + 2000;
	else if (si == ansi_to_utf8(test2))
		return sizeprice + 1000;
	else if (si == ansi_to_utf8(test3))
		return sizeprice + 3000;
	else if (si == ansi_to_utf8(test13))
		return sizeprice + 3000;
	else if (si == ansi_to_utf8(test4))
		return sizeprice + 5000;
	else if (si == ansi_to_utf8(test5))
		return sizeprice + 10000;
	else if (si == ansi_to_utf8(test6))
		return sizeprice + 1000;
	else if (si == ansi_to_utf8(test7))
		return sizeprice + 3000;
	else if (si == ansi_to_utf8(test8))
		return sizeprice + 4000;
	else if (si == ansi_to_utf8(test9))
		return sizeprice + 5000;
	else if (si == ansi_to_utf8(test10))
		return sizeprice + 4000;
	else if (si == ansi_to_utf8(test11))
		return sizeprice + 5000;
	else if (si == ansi_to_utf8(test12))
		return sizeprice + 3000;

}


//전체 렌더 
void Process::Render()
{
	//컨투어

	if (!resultcontours.empty())
		drawContours(img, resultcontours, -1, Scalar(255, 0, 0), 3);



	/*if (!resultcontours2.empty())
		drawContours(testimg, resultcontours2, -1, Scalar(0, 255, 0), 3);*/

	if (detectmethod == DETECT::CONTOUR)
	{
		putText(img, "Area: " + to_string(curArea), Point(20, 100), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
		putText(img, "width: " + to_string(curwidth), Point(20, 130), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
		putText(img, "height: " + to_string(curheight), Point(20, 160), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
		putText(img, "testmin" + to_string(testvaluemin), Point(20, 300), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
		putText(img, "testmax: " + to_string(testvaluemax), Point(20, 330), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
	}
	putText(img, "cqr: " + to_string(cqr), Point(20, 200), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
	putText(img, "bqr: " + to_string(bqr), Point(20, 230), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));


	//QR 코드
	if (!qrinfo.empty()) {
		polylines(qrimg, points, true, Scalar(0, 0, 255), 2);
	}



	if (rndframe > 100)
	{
		system("cls");
		for (auto& iter : vecInfo)
		{
			iter.Render();
		}
		rndframe = 0;
	}

}



bool Process::detectAndDrawBoxes(Mat& frame, const Mat& mask)
{
	Mat kernel = getStructuringElement(MORPH_RECT, Size(13, 13));
	morphologyEx(mask, mask, MORPH_OPEN, kernel);

	// 택배 상자 윤곽선 검출
	vector<vector<Point>> contours;
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Point center(frame.cols / 2, frame.rows / 2);

	//상자색 화면 중심에서 검출하면 작동
	if (mask.at<uchar>(center) > 0)
	{

		// 윤곽선을 감싸는 사각형 그리기
		for (size_t i = 0; i < contours.size(); i++) {

			// 윤곽선을 감싸는 최소한의 사각형 구하기
			RotatedRect rotatedRect = minAreaRect(contours[i]);
			vector<Point2f> vertices(4);
			rotatedRect.points(vertices.data());
			// 택배물건의 크기 계산

			// 화면에 택배물건의 크기 표시
			int width = rotatedRect.size.width;
			int height = rotatedRect.size.height;
			int Area = width * height;

			if (Area > 25400) {

				// 윤곽선 주위에 사각형 대신 점 그리기
				for (int j = 0; j < 4; j++) {
					line(frame, vertices[j], vertices[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}



				if (Area > 25400 && Area < 33400)
					bsize = BOXSIZE::SMALL;
				else if (Area > 38140 && Area < 46140)
					bsize = BOXSIZE::MIDIUM;
				else if (Area > 52800 && Area < 62800)
					bsize = BOXSIZE::LARGE;
				else
					bsize = BOXSIZE::SIZEEND;

				if (bsize != BOXSIZE::SIZEEND) {
					cout << fixed;		// 소수점 고정
					cout.precision(2);	// 고정시킬 자리 설정
					putText(frame, "Width: " + to_string(width / 25.0) + " pixels", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
					cout << fixed;		// 소수점 고정
					cout.precision(2);	// 고정시킬 자리 설정
					putText(frame, "Height: " + to_string(height / 25.0) + " pixels", Point(10, 70), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);

					return true;
				}
			}

		}
	}


	return false;
}

bool Process::MakeMask(Mat frame)
{
	Mat hsv;
	cvtColor(frame, hsv, COLOR_BGR2HSV);

	// 갈색 택배 상자의 색상 범위 지정
	Scalar lower_beige = Scalar(10, 33, 45); // 갈색의 하한값(색상, 채도, 명도에 대해 적절한 범위 조정 가능)
	Scalar upper_beige = Scalar(50, 255, 255);// 갈색의 상한값(색상, 채도, 명도에 대해 적절한 범위 조정 가능)
	// 갈색 택배 상자를 위한 마스크 생성
	Mat mask;
	inRange(hsv, lower_beige, upper_beige, mask);
	// 마스크를 사용하여 택배 상자 추출 및 상자 구분
	return detectAndDrawBoxes(frame, mask);
}



//void Process::SQL_Connection()
//{
//	mysql_init(&Conn);
//
//	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "0000", "moble_box_information", 3306, (char*)NULL, 0);
//	mysql_set_character_set(ConnPtr, "euckr");
//	if (ConnPtr == NULL)
//	{
//		fprintf(stderr, "error : %s", mysql_error(&Conn));
//		return;
//	}
//	mysql_query(ConnPtr, "SET NAMES utf8mb4");
//	mysql_query(ConnPtr, "SET CHARACTER SET utf8mb4");
//	mysql_query(ConnPtr, "SET SESSION collation_connection = 'utf8mb4_unicode_ci'");
//
//}
//
//void Process::SQL_Insert(string sender, string receiver, string contents, string strdo, string strsi, string strroad, string strnum, const Mat& img)
//{
//	// 이미지 파일을 바이너리로 읽기
//	std::vector<unsigned char> imageData = matToBinary(img);
//	if (imageData.empty()) {
//		std::cerr << "Failed to read image data" << std::endl;
//		return;
//	}
//
//	// 새로운 레코드를 추가하는 INSERT 문
//	string InsertQuery = "INSERT INTO qr_information (sender, receiver, contents, strdo, strsi, strroad, strnum, qr_image) VALUES ('" + sender + "', '" + receiver + "', '" + contents + "', '" + strdo + "', '" + strsi + "', '" + strroad + "', '" + strnum + "', ?)";
//
//	MYSQL_STMT* stmt = mysql_stmt_init(ConnPtr);
//	if (!stmt) {
//		std::cerr << "Failed to initialize MySQL statement" << std::endl;
//		return;
//	}
//
//	if (mysql_stmt_prepare(stmt, InsertQuery.c_str(), InsertQuery.size()) != 0) {
//		std::cerr << "Failed to prepare MySQL statement: " << mysql_error(ConnPtr) << std::endl;
//		mysql_stmt_close(stmt);
//		return;
//	}
//
//	MYSQL_BIND bind;
//	memset(&bind, 0, sizeof(bind));
//	bind.buffer_type = MYSQL_TYPE_LONG_BLOB;
//	bind.buffer = const_cast<unsigned char*>(imageData.data());
//	bind.buffer_length = imageData.size();
//
//	if (mysql_stmt_bind_param(stmt, &bind) != 0) {
//		std::cerr << "Failed to bind parameter: " << mysql_error(ConnPtr) << std::endl;
//		mysql_stmt_close(stmt);
//		return;
//	}
//
//	if (mysql_stmt_execute(stmt) != 0) {
//		std::cerr << "Failed to execute MySQL statement: " << mysql_error(ConnPtr) << std::endl;
//		mysql_stmt_close(stmt);
//		return;
//	}
//
//}
//
//void Process::SQL_Select_sender(string& sender) {
//	string query = "SELECT * FROM qr_information WHERE sender = '" + sender + "'";
//
//	// 쿼리 실행
//	if (mysql_query(ConnPtr, query.c_str()) != 0) {
//		fprintf(stderr, "Failed to execute MySQL statement: %s\n", mysql_error(ConnPtr));
//		return;
//	}
//
//	// 결과 저장
//	Result = mysql_store_result(ConnPtr);
//	if (!Result) {
//		fprintf(stderr, "Failed to retrieve MySQL result: %s\n", mysql_error(ConnPtr));
//		return;
//	}
//
//	// 결과 출력
//	while ((Row = mysql_fetch_row(Result)) != NULL) {
//		printf("sender: %s, receiver: %s, contents: %s, strdo: %s, strsi: %s, strroad: %s, strnum: %s\n",
//			Row[0], Row[1], Row[2], Row[3], Row[4], Row[5], Row[6]);
//	}
//
//	// 결과 메모리 해제
//	mysql_free_result(Result);
//}
//
//
//Mat Process::SQL_ReadImage(MYSQL* ConnPtr, int index)
//{
//	string query = "SELECT qr_image FROM qr_information LIMIT 1 OFFSET " + to_string(index);
//	if (mysql_query(ConnPtr, query.c_str()) != 0) {
//		cerr << "Failed to execute MySQL query: " << mysql_error(ConnPtr) << endl;
//		return Mat();
//	}
//
//	MYSQL_RES* result = mysql_store_result(ConnPtr);
//	if (!result) {
//		cerr << "Failed to store MySQL result: " << mysql_error(ConnPtr) << endl;
//		return Mat();
//	}
//
//	MYSQL_ROW row = mysql_fetch_row(result);
//	if (!row) {
//		cerr << "No image data found in database" << endl;
//		mysql_free_result(result);
//		return Mat();
//	}
//
//	unsigned long* lengths = mysql_fetch_lengths(result);
//	size_t imageSize = lengths[0];
//	const char* imageBinary = row[0];
//
//	std::vector<unsigned char> imageData(imageBinary, imageBinary + imageSize);
//	mysql_free_result(result);
//
//	// 바이너리 데이터를 이미지로 디코딩
//	return imdecode(imageData, cv::IMREAD_COLOR);
//}
//
//vector<unsigned char> Process::matToBinary(const Mat& img) {
//	// 이미지를 JPEG 형식으로 인코딩하여 메모리 버퍼에 저장
//	vector<unsigned char> buffer;
//	vector<int> params;
//	params.push_back(IMWRITE_JPEG_QUALITY);
//	params.push_back(100);
//	imencode(".jpg", img, buffer, params);
//
//	return buffer;
//}