#pragma once

class MainUI;

class Process
{

public:
	enum DETECT { COLOR,CONTOUR,DETECTEND };
	enum MARK { DESTROY, MOISTURE, HANDLING, MARKEND };

public:
	explicit Process();
	~Process();


public:
	void Initialize();
	 bool Update();
	void LateUpdate();
	void Show();
public:
	void Render(MainUI * mainptr);



public:
	//상자 색 검출 코드
	bool detectAndDrawBoxes(Mat& frame, const Mat& mask);
	bool MakeMask(Mat frame);
public:
	//상자 윤곽선 검출 코드
	int findMaxArea(vector<vector<cv::Point>> contours);
	void getRectangleDimensions(const std::vector<cv::Point>& pts);
	bool CheckBoxContour(Mat img_input);
	void ResetContour();
public:
	//QR코드 읽고 INFO 데이터화 해주는 코드 
	void ReadQR();
	void MakeInfo(string& add, string strdo, string strsi, string strroad, string strnum, string send, string recieve, string content, BOXSIZE sz);
public:
	//SQL - 연결
	void SQL_Connection();
	//SQL문 - 데이터 삽입
	void SQL_Insert(string sender, string receiver, string contents, string strdo, string strsi, string strroad, string strnum, const Mat& img);
	//SQL문 - 데이터 불러오기
	MYSQL_RES* SQL_Select_All();
	void SQL_Select_sender(string& sender);
	//void SQL_Select_receiver(string receiver);
	//DB에 index행에 있는 사진 Mat으로 반환
	Mat SQL_ReadImage(MYSQL* ConnPtr, int index);
	//변환 함수
	vector<unsigned char> matToBinary(const cv::Mat& img);
	std::string utf8toansi(const std::string& utf8str);

public:
	bool markchecking();

public:
	DETECT Get_DETECT();
	Mat Get_Cam0();
	Mat Get_Cam1();

public:
	void Set_DETECT(DETECT d);

	String Detected_value(bool input);

public:
	//가격 책정 
	void AllotPrice(INFO& info);
	string ansi_to_utf8(string& ansi);
	int AllotPricereach(string si,int sizeprice);

public:
	//그 외 
	// 
	//variable
private:
	bool bqr=false;
	bool cqr = false;
	bool mqr = false;
	DETECT detectmethod;
private:
	int rndframe = 0;
	int Key = 0;
	int Minval = 0;

private:
	//디버그용 
	UINT oldCP;
	int testvaluemin = 50;
	int testvaluemax = 350;
	//공용변수 
	VideoCapture cap;
	VideoCapture cap2;
	Mat img;
	Mat testimg;
	Mat qrimg;


	//contour용 변수 
	vector<vector<Point>> contours;
	vector<vector<cv::Point>> resultcontours;
	vector<vector<cv::Point>> resultcontours2;
	vector <Point> curcontour;
	double fullwidth = 0;
	double fullheight = 0;
	double curwidth=0;
	double curheight=0;
	double curArea=0;
	BOXSIZE bsize;

	double samplewidthpixel = 0;
	double samplewidthcm = 0;
	double sampleheightpixel = 0;
	double sampleheightcm = 0;

	double realwidthcm = 0;
	double realheightcm = 0;

	

	//qr용변수
	String qrinfo;
	string laststr;
	QRCodeDetector detector;
	vector<Point> points;
	//도 시 도로명 건물번호 
	string strdo, strsi, strroad, strnum, sender, receiver, contents;

	
		//SQL용변수
	MYSQL Conn;
	MYSQL* ConnPtr = NULL;
	MYSQL_RES* Result;
	MYSQL_ROW Row;
	int Stat;


	MARK emark;
	Mat img_Fragile;
	Mat img_handling;
	Mat img_moisture;

	//INFO
	vector<INFO> vecInfo;
};

