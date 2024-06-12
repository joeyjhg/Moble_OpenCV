#pragma once


class Process
{

public:
	enum DETECT { COLOR,CONTOUR,DETECTEND };

public:
	explicit Process();
	~Process();


public:
	void Initialize();
	 bool Update();
	void LateUpdate();
	void Show();
public:
	void Render();



public:
	//���� �� ���� �ڵ�


	bool detectAndDrawBoxes(Mat& frame, const Mat& mask);
	bool MakeMask(Mat frame);
public:
	//���� ������ ���� �ڵ�
	int findMaxArea(vector<vector<cv::Point>> contours);
	void getRectangleDimensions(const std::vector<cv::Point>& pts);
	bool CheckBoxContour(Mat img_input);
	void ResetContour();
	void RealSize();
public:
	//QR�ڵ� �а� INFO ������ȭ ���ִ� �ڵ� 
	void ReadQR();
	void MakeInfo(string& add, string strdo, string strsi, string strroad, string strnum, string send, string recieve, string content, BOXSIZE sz);
public:
	//SQL - ����
	//void SQL_Connection();
	////SQL�� - ������ ����
	//void SQL_Insert(string sender, string receiver, string contents, string strdo, string strsi, string strroad, string strnum, const Mat& img);
	////SQL�� - ������ �ҷ�����
	//void SQL_Select_sender(string& sender);
	////void SQL_Select_receiver(string receiver);
	////DB�� index�࿡ �ִ� ���� Mat���� ��ȯ
	//Mat SQL_ReadImage(MYSQL* ConnPtr, int index);

public:
	DETECT Get_DETECT();
	Mat Get_Cam0();
	Mat Get_Cam1();

public:
	void Set_DETECT(DETECT d);


public:
	//���� å�� 
	void AllotPrice(INFO& info);
	string ansi_to_utf8(string& ansi);
	int AllotPricereach(string si,int sizeprice);

public:
	//�� �� 
	// 
	//variable
private:
	bool bqr=false;
	bool cqr = false;
	DETECT detectmethod;
private:
	int rndframe = 0;
	int Key = 0;
	int Minval = 0;

private:
	//����׿� 
	UINT oldCP;
	int testvaluemin = 50;
	int testvaluemax = 350;
	//���뺯�� 
	VideoCapture cap;
	VideoCapture cap2;
	Mat img;
	Mat testimg;
	Mat qrimg;


	//contour�� ���� 
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

	

	//qr�뺯��
	String qrinfo;
	string laststr;
	QRCodeDetector detector;
	vector<Point> points;
	//�� �� ���θ� �ǹ���ȣ 
	string strdo, strsi, strroad, strnum, sender, receiver, contents;

	
		//SQL�뺯��
	/*MYSQL Conn;
	MYSQL* ConnPtr = NULL;
	MYSQL_RES* Result;
	MYSQL_ROW Row;
	int Stat;
	vector<unsigned char> matToBinary(const cv::Mat& img);*/

	//INFO
	vector<INFO> vecInfo;
};

