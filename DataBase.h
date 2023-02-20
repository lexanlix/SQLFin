#include <iostream>
#include <list>
#include <vector>
#include <clocale>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>
#include <string>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Data;
using namespace Data;
using namespace Data::SqlClient;
using namespace std;


ref class DataBase
{

	SqlConnection^ connect;
	SqlConnectionStringBuilder^ connStringBuilder;


	int NumberDohColumns = 0, NumberRasColumns = 0;
	void ConnectToDB();

public:
	void InsertToDB(String^ Months);

	void Read(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, int Nsumma);

};

