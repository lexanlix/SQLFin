#include "Functions.h"


void Functions::ConnectToDB()
{
	// Data Source=DESKTOP-S26GEEH\SQLEXPRESS;Initial Catalog=TestFin;Integrated Security=True

	connStringBuilder = gcnew SqlConnectionStringBuilder();
	connStringBuilder->DataSource = "DESKTOP-S26GEEH\\SQLEXPRESS";
	connStringBuilder->InitialCatalog = "TestFin";
	connStringBuilder->IntegratedSecurity = true;

	connect = gcnew SqlConnection(Convert::ToString(connStringBuilder));
}

void Functions::SetNsumma()
{
	// ��� �����:
	/*cout << "\n\n������� ����������� ����� �������, ������� �� ����������� ����� ��������� ����������� � ������: ";
	int N;
	cin >> N;
	Nsumma = N;*/

	Nsumma = 2400;
}


void Functions::ChangeCellValue(list<string>& NamesRas, list<string>& NamesDoh)
{		// ��������� �������� � ������������ ������
	int categoryNum, monthNum, value;
	
	cout << "\nInput category number, then month number:\n";
	cin >> categoryNum >> monthNum;

	cout << "\nInput value: ";
	cin >> value;


	try
	{
		ConnectToDB();

		String^ cmdText = "UPDATE [TestFin].[dbo].";
		String^ id;

		if (categoryNum > NumberDohColumns) // ���� �� ��������� ��������
		{
			id = "rasId";
			
			auto it = NamesRas.begin();
			advance(it, categoryNum - NumberDohColumns - 1);
			string name = *it;
			String^ One = gcnew String(name.c_str()); // string to System::String

			cmdText += "[ras_db] SET " + One + " = ";
		}
		else // ���� �� �������
		{
			id = "dohId";

			auto it = NamesDoh.begin();
			advance(it, categoryNum - 1);
			string name = *it;
			String^ One = gcnew String(name.c_str()); // string to System::String

			cmdText += "[doh_db] SET " + One + " = ";
		}
		
		cmdText += Convert::ToString(value) + " WHERE " + id + " = " + Convert::ToString(monthNum);

		// ������ ������� cmdText = "UPDATE [TestFin].[dbo].[doh_db] SET fromCard = 0 WHERE dohId = 3";

		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);

		connect->Open();
		cmd->ExecuteNonQuery();
	}
	finally
	{
		if (connect != nullptr)
		{
			connect->Close();
		}
	}
}

void Functions::WriteDataFromDB(list<string>& NamesRas, list<string>& NamesDoh, list<string>& AllMonths)
{		// ����� � ������� ������� ����� � ���� ������, ��� ������������� ������ � ����������
	cout << "\n��������� ����� = " << Nsumma;

	vector<string> cconsts = { "������", "�������", "������ � �����", "�������" };
	unsigned short int year = 2023;
	cout << "\n ��� " << year << " ";
	cout << "               " << cconsts[0] << "         ";
	cout << "                                 " << cconsts[1] << "                                 \n           ";


	for (auto i = NamesDoh.begin(); i != NamesDoh.end(); ++i)
	{
		cout << "\t" << *i;
	}

	for (auto i = NamesRas.begin(); i != NamesRas.end(); ++i)
	{
		cout << "\t" << *i;
	}
	cout << "\t" << cconsts[2] << "\t" << cconsts[3];

	try
	{
		ConnectToDB();

		String^ cmdText = "SELECT  * FROM [TestFin].[dbo].[doh_db] FULL JOIN [TestFin].[dbo].[ras_db] ON doh_db.dohId=ras_db.rasId";
		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);
		connect->Open();
		
		SqlDataReader^ reader = cmd->ExecuteReader();

		auto it = AllMonths.begin();

		int consumption = 0, balance = Nsumma; // ������ � �������

		// ����� ������ � ������� ����� ����� ������ � ��
		while (reader->Read())
		{
			cout << "\n" << *it << "    ";

			for (int i = 1; i < NumberDohColumns + NumberRasColumns + 2; i++) // ������ � ������ id ������ ����������
			{
				if (i != NumberDohColumns + 1)
				{
					int currentCell = Convert::ToInt32(reader[i]->ToString());
					
					if (i <= NumberDohColumns)
						balance += currentCell;
					else
						consumption -= currentCell;
					
					cout << "     " << currentCell << "     ";
				}
			}
			balance += consumption;
			cout << "     " << consumption << "     " << balance << "     ";

			consumption = 0;
			advance(it, 1);
		}

	}
	finally
	{
		if (connect != nullptr)
		{
			connect->Close();
		}
	}
}

void Functions::Read(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money)
{   // ������ ������ � �� ��� ��������� � ����������, ����� ����� ������� � ������� WriteTable

	try
	{
		// ������������ � ��
		ConnectToDB();
		String^ cmdText = "SELECT COLUMN_NAME FROM TestFin.INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'doh_db'";
		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);
		connect->Open();

		SqlDataReader^ reader = cmd->ExecuteReader();

		// ���� �� ������� �������, ������� �������� ����� ��������

		while (reader->Read())
		{
			if (reader["COLUMN_NAME"]->ToString() == "dohId")
				continue;
			//cout << "\t" << msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString());
			NamesDoh.push_back(msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString()));
			NumberDohColumns++;
		}

		reader->Close();

		cmdText = "SELECT COLUMN_NAME FROM TestFin.INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'ras_db'";
		cmd = gcnew SqlCommand(cmdText, connect);
		reader = cmd->ExecuteReader();

		// ���� �� ������� ��������, ������� �������� ����� ��������
		while (reader->Read())
		{
			if (reader["COLUMN_NAME"]->ToString() == "rasId")
				continue;
			//cout << "\t" << msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString());
			NamesRas.push_back(msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString()));
			NumberRasColumns++;
		}

		reader->Close();


		// ����������� ���������� ������� � ��������: 
		// (������������ �� ������� �������, � ����� �������� ����������� ���������� ���-�� �������)
		cmdText = "SELECT COUNT(*) as count FROM [TestFin].[dbo].[doh_db]";
		cmd = gcnew SqlCommand(cmdText, connect);
		reader = cmd->ExecuteReader();

		reader->Read();
		MonthNumbers = Convert::ToInt32(reader["count"]->ToString());

		reader->Close();


		cmdText = "SELECT * FROM [TestFin].[dbo].[doh_db]";
		cmd = gcnew SqlCommand(cmdText, connect);
		reader = cmd->ExecuteReader();

		// ���� �� ������� ������� �������, ������� �������� ��������
		Money.assign((NumberRasColumns + NumberDohColumns + 2) * MonthNumbers, 0); // ������ ������ � ������

		int counter = 0;
		while (reader->Read())
		{
			for (int i = 0; i < NumberDohColumns; i++)
			{
				Money[counter * (NumberDohColumns + NumberRasColumns + 2) + i] = Convert::ToInt32(reader[i + 1]->ToString());
			}
			counter++;
		}

		reader->Close();

		cmdText = "SELECT * FROM [TestFin].[dbo].[ras_db]";
		cmd = gcnew SqlCommand(cmdText, connect);
		reader = cmd->ExecuteReader();

		// ���� �� ������� ������� ��������, ������� �������� ��������

		counter = 0;
		while (reader->Read())
		{
			for (int i = 0; i < NumberRasColumns; i++)
			{
				Money[counter * (NumberDohColumns + NumberRasColumns + 2) + i + NumberDohColumns] = Convert::ToInt32(reader[i + 1]->ToString());
			}
			counter++;
		}

		reader->Close();

	}
	finally
	{
		if (connect != nullptr)
		{
			connect->Close();
		}
	}

}

void Functions::GetSums(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months)
{
	int i = 0,
		ostatok = Nsumma;
	for (auto it = Months.begin(); it != Months.end(); ++it)
	{
		int rMonth = 0;
		for (int h = i; h < i + NamesDoh.size(); ++h)
		{
			ostatok += Money[h];
		}
		for (int h = i + NamesDoh.size(); h < i + NamesDoh.size() + NamesRas.size(); ++h)
		{
			rMonth += Money[h];
		}
		Money[i + NamesDoh.size() + NamesRas.size()] = -rMonth;
		ostatok += Money[i + NamesDoh.size() + NamesRas.size()];
		Money[i + NamesDoh.size() + NamesRas.size() + 1] = ostatok;
		i += NamesDoh.size() + NamesRas.size() + 2;
	}
}

void Functions::Initialization(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths)
// ������������� ������ � ������
{
	// �������� ��������� ���������� ������, �������� ���, ��� � ����, � ����� ���������� ���-�� �������� � �����:
	Read(NamesRas, NamesDoh, Money);

	AllMonths = { "������  ", "������� ", "����    ", "������  ", "���     ", "����    ", "����    ",
					"������  ", "��������", "������� ", "������  ", "������� " };

	list<string>::iterator itAllMonths = AllMonths.begin();
	//list<string>::iterator itMonths = Months.begin();

	/*advance(itMonths, Months.size() - 1);
	if (*itMonths != "������� ")
		advance(itAllMonths, Months.size() % AllMonths.size());
	Months.push_back(*itAllMonths);*/

	for (int i = 0; i < MonthNumbers; i++)
	{
		Months.push_back(*itAllMonths);
		advance(itAllMonths, 1);
	}

	// ���� ������ �����
	GetSums(NamesRas, NamesDoh, Money, Months);

	WriteTable(NamesRas, NamesDoh, Money, Months);
}

void Functions::WriteTable(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months)     // ����� � ������� ���� �������
{
	cout << "\n��������� ����� = " << Nsumma;

	vector<string> cconsts = { "������", "�������", "������ � �����", "�������" };
	unsigned short int year = 2023;
	cout << "\n ��� " << year << " ";
	cout << "               " << cconsts[0] << "         ";
	cout << "                                 " << cconsts[1] << "                                 \n           ";


	for (auto i = NamesDoh.begin(); i != NamesDoh.end(); ++i)
	{
		cout << "\t" << *i;
	}

	for (auto i = NamesRas.begin(); i != NamesRas.end(); ++i)
	{
		cout << "\t" << *i;
	}
	cout << "\t" << cconsts[2] << "\t" << cconsts[3];

	int size = NamesRas.size() + NamesDoh.size() + 2;

	vector<int>::iterator end = Money.begin(), begin = Money.begin();
	list<string>::iterator check = Months.begin();  // ��� �������������� ������ �� ������� ������ list Money

	advance(end, size);
	advance(check, Months.size() - 1);
	for (auto i = Months.begin(); i != Months.end(); ++i)
	{
		if ((*i == "������  ") && (i != Months.begin()))
			cout << "\n ��� " << ++year << " ";
		cout << "\n" << *i << "   ";

		for (auto it = begin; it != end; ++it)
		{
			cout << "     " << *it << "     ";
		}
		if (i != check)
		{
			advance(begin, size);
			advance(end, size);
		}
	}
	cout << "\n";
}


// �� ���� ��� ���� ���
template <typename T>
void Functions::PrintList(const list<T>& lst)
{
	cout << "\n";
	for (auto i = lst.cbegin(); i != lst.cend(); ++i)
	{
		cout << *i << "    ";
	}
	cout << "\n";
}

template <typename T>
void Functions::PrintList2(const list<T>& lst)
{
	cout << "\n";
	for (auto i = lst.cbegin(); i != lst.cend(); ++i)
	{
		cout << *i << "          ";
	}
	cout << "\n";
}


void Functions::AddNewCategory(list<string>& NamesRas, list<string>& NamesDoh)
{
	char choice;
	string CategName;
	int size;

	cout << "\nInput type of new category(R or D):\n";
	cin >> choice;

	cout << "\nInput name of category: ";
	cin >> CategName;

	// ������� ���������� � �����
	if (choice == 'D')
	{
		NamesDoh.push_back(CategName);
		size = NamesDoh.size() - 1;
		NumberDohColumns++;
	}
	else if (choice == 'R')
	{
		NamesRas.push_back(CategName);
		size = NamesRas.size() + NamesDoh.size() - 1;
		NumberRasColumns++;
	}

	// ����� ���������� ������� � ���� ������
	try
	{
		ConnectToDB();

		String^ cmdText = "ALTER TABLE [TestFin].[dbo]."; // ���������� �������
		String^ cmdText2 = "UPDATE [TestFin].[dbo].";     // ���������� ������

		if (choice == 'D')
		{
			cmdText += "[doh_db]";
			cmdText2 += "[doh_db]";
		}
		else if (choice == 'R')
		{
			cmdText += "[ras_db]";
			cmdText2 += "[ras_db]";
		}

		String^ OneName = gcnew String(CategName.c_str()); // string to System::String

		cmdText += " ADD " + OneName + " int";
		cmdText2 += " SET " + OneName + " = 0";

		// �������: ALTER TABLE [TestFin].[dbo].[doh_db] ADD salary int
		//			UPDATE[TestFin].[dbo].[doh_db] SET salary = 0

		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);
		connect->Open();
		cmd->ExecuteNonQuery();
		connect->Close();

		cmd = gcnew SqlCommand(cmdText2, connect);
		connect->Open();
		cmd->ExecuteNonQuery();
	}
	finally
	{
		if (connect != nullptr)
		{
			connect->Close();
		}
	}

	// ��������� ��� ��������� ������� Money
	/*for (auto i = Months.begin(); i != Months.end(); ++i)
	{
		Money.insert(Money.begin() + size, 0);
		size += NamesDoh.size() + NamesRas.size() + 2;
	}*/
}

void Functions::DeleteCategory(list<string>& NamesRas, list<string>& NamesDoh)
{
	int categNumber;
	string CategName;

	cout << "\nInput number of deleting category:\n";
	cin >> categNumber;

	// ������� �������� �� ������
	if (categNumber <= NumberDohColumns)
	{
		auto it = NamesDoh.begin();

		for (int i = 0; i < (categNumber - 1); i++)
		{
			advance(it, 1);
		}
		CategName = *it;
		NamesDoh.erase(it);
	}
	else
	{
		auto it = NamesRas.begin();

		for (int i = 0; i < (categNumber - NumberDohColumns - 1); i++)
		{
			advance(it, 1);
		}
		CategName = *it;
		NamesRas.erase(it);
	}

	// ����� �� ��
	try
	{
		ConnectToDB();

		String^ cmdText = "ALTER TABLE [TestFin].[dbo].";

		if (categNumber <= NumberDohColumns)
		{
			cmdText += "[doh_db]";
			NumberDohColumns--;
		}
		else
		{
			cmdText += "[ras_db]";
			NumberRasColumns--;
		}

		String^ OneName = gcnew String(CategName.c_str()); // string to System::String

		cmdText += " DROP COLUMN " + OneName;

		// �������: ALTER TABLE [TestFin].[dbo].[doh_db] DROP COLUMN salary

		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);
		connect->Open();
		cmd->ExecuteNonQuery();
	}
	finally
	{
		if (connect != nullptr)
		{
			connect->Close();
		}
	}

}



// ��� ���������� ����������:
void Functions::InsertValue(vector<int>& Mon, int number, int NewValue)    //�������� �������� ����� � ������
{
	Mon[number - 1] = NewValue;
}



// �������� ��������� �����
void Functions::AddMonth(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths)
{   // � ��������
	list<string>::iterator it = AllMonths.begin();
	list<string>::iterator current = Months.begin();

	advance(current, Months.size() - 1);
	if (*current != "������� ")
		advance(it, Months.size() % AllMonths.size());
	Months.push_back(*it);
	for (int i = 0; i < (NamesRas.size() + NamesDoh.size() + 2); ++i)
	{
		Money.push_back(0);
	}


	try
	{
		// ������ �� ���������� ����� � ������� �������

		String^ cmdText = "INSERT INTO [TestFin].[dbo].[doh_db](";
		String^ cmdText2 = "VALUES(";

		auto itEnd = NamesDoh.begin();
		advance(itEnd, NamesDoh.size() - 1);

		for (auto it = NamesDoh.begin(); it != NamesDoh.end(); ++it)
		{
			string name = *it;
			String^ One = gcnew String(name.c_str()); // string to System::String
			if (*it != *itEnd)
			{
				cmdText += One + ", ";
				cmdText2 += "0, ";
			}
			else
			{
				cmdText += One + ") ";
				cmdText2 += "0)";
			}

		}
		cmdText += cmdText2;
		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);

		connect->Open();
		cmd->ExecuteNonQuery();

		connect->Close();

		// ������ �� ���������� ����� � ������� ��������

		cmdText = "INSERT INTO [TestFin].[dbo].[ras_db](";
		cmdText2 = "VALUES(";

		itEnd = NamesRas.begin();
		advance(itEnd, NamesRas.size() - 1);

		for (auto it = NamesRas.begin(); it != NamesRas.end(); ++it)
		{
			string name = *it;
			String^ One = gcnew String(name.c_str());
			if (*it != *itEnd)
			{
				cmdText += One + ", ";
				cmdText2 += "0, ";
			}
			else
			{
				cmdText += One + ") ";
				cmdText2 += "0)";
			}

		}
		cmdText += cmdText2;
		// ������������ ����� ������� "INSERT INTO [TestFin].[dbo].[ras_db](Eat, Stationery, Transport, Phone) VALUES(0, 0, 0, 0)";
		cmd = gcnew SqlCommand(cmdText, connect);

		connect->Open();
		cmd->ExecuteNonQuery();

	}
	finally
	{
		if (connect != nullptr)
		{
			connect->Close();
		}
	}
}

void Functions::ChangeCatName(list<string>& NamesRas, list<string>& NamesDoh)   //�������� �������� ���������
{
	cout << "\nWhat category you want to change?\n";
	int number;
	cin >> number;
	cout << "New name: ";
	string NewName;
	cin >> NewName;

	if (number > NamesDoh.size())
	{
		auto it = NamesRas.begin();
		advance(it, number - NamesDoh.size() - 1);
		*it = NewName;
	}
	else
	{
		auto it = NamesDoh.begin();
		advance(it, number - 1);
		*it = NewName;
	}
}

void Functions::AddValue(vector<int>& Mon)   // �������� �������� � ������ ������������� ����� ������
{
	cout << "\nWhat category value you want to add?\n";
	int number;
	cin >> number;
	cout << "New values(down to '0'): ";
	int NewValue, sum = 0;
	do
	{
		cin >> NewValue;
		sum += NewValue;
	} while (NewValue != 0);

	Mon[number - 1] += sum;

}

