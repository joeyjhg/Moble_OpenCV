#pragma once

typedef struct BOXINFO
{
	BOXSIZE size = BOXSIZE::SIZEEND;
	
	std::string all = "";

	std::string strdo = "";
	std::string strsi = "";
	std::string strroad = "";
	std::string strnum = "";

	std::string contents = "";
	std::string sender = "";
	std::string reciever = "";
	std::string caution = "";


	int price=0;


	void Rndutf(std::string utf)
	{
		SetConsoleOutputCP(CP_UTF8);
		std::cout << utf << std::endl;
		SetConsoleOutputCP(949);
	}
	void Rndutfnoline(std::string utf)
	{
		SetConsoleOutputCP(CP_UTF8);
		std::cout << utf;
		SetConsoleOutputCP(949);
	}


	void Render()
	{
		SetConsoleOutputCP(949);

		std::cout << "주소:";
		Rndutfnoline(strdo);
		Rndutfnoline(strsi);
		Rndutfnoline(strroad);
		Rndutf(strnum);
		std::cout << "물품:";
		Rndutf(contents);
		std::cout << "보내는이:";
		Rndutf(sender);
		std::cout << "받는이:";
		Rndutf(reciever);

		switch (size)
		{
		case LARGE:
			std::cout << "박스사이즈: 대"  << std::endl;
			break;
		case MIDIUM:
			std::cout << "박스사이즈: 중" << std::endl;
			break;
		case SMALL:
			std::cout << "박스사이즈: 소" << std::endl;
			break;
		default:
			std::cout << "박스사이즈: none" << std::endl;
			break;
		}

		std::cout <<"가격:" << price << std::endl;
		std::cout << "주의사항 :" << caution << std::endl;
		std::cout << "//////////////////////////////////////////////////////////////" << std::endl;

		SetConsoleOutputCP(CP_UTF8);

	}

}INFO;