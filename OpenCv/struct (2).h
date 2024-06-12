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

		std::cout << "�ּ�:";
		Rndutfnoline(strdo);
		Rndutfnoline(strsi);
		Rndutfnoline(strroad);
		Rndutf(strnum);
		std::cout << "��ǰ:";
		Rndutf(contents);
		std::cout << "��������:";
		Rndutf(sender);
		std::cout << "�޴���:";
		Rndutf(reciever);

		switch (size)
		{
		case LARGE:
			std::cout << "�ڽ�������: ��"  << std::endl;
			break;
		case MIDIUM:
			std::cout << "�ڽ�������: ��" << std::endl;
			break;
		case SMALL:
			std::cout << "�ڽ�������: ��" << std::endl;
			break;
		default:
			std::cout << "�ڽ�������: none" << std::endl;
			break;
		}

		std::cout <<"����:" << price << std::endl;
		std::cout << "//////////////////////////////////////////////////////////////" << std::endl;

		SetConsoleOutputCP(CP_UTF8);

	}

}INFO;