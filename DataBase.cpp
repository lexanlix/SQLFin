#include "DataBase.h"


void DataBase::ConnectToDB()
{
	// Data Source=DESKTOP-S26GEEH\SQLEXPRESS;Initial Catalog=TestFin;Integrated Security=True

	connStringBuilder = gcnew SqlConnectionStringBuilder();
	connStringBuilder->DataSource = "DESKTOP-S26GEEH\\SQLEXPRESS";
	connStringBuilder->InitialCatalog = "TestFin";
	connStringBuilder->IntegratedSecurity = true;

	connect = gcnew SqlConnection(Convert::ToString(connStringBuilder));
}


void DataBase::InsertToDB(String^ Months)
{
	int Eat = 12000;
	try
	{
		ConnectToDB();

		String^ cmdText = "INSERT INTO dbo.test_db(Months, Eat, Stationery, Transport, Phone) VALUES(@MonthsIns, @EatIns, 0, 0, 0)";
		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);

		cmd->Parameters->AddWithValue("@MonthsIns", Months);
		cmd->Parameters->AddWithValue("@EatIns", Eat);
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

void DataBase::Read(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, int Nsumma)
{

	cout << "\n��������� ����� = " << Nsumma;

	vector<string> cconsts = { "������", "�������", "������ � �����", "�������" };
	unsigned short int year = 2022;
	cout << "\n ��� " << year << " ";
	cout << "         " << cconsts[0] << "         ";
	cout << "                                 " << cconsts[1] << "                                 \n           ";

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
			if (reader["COLUMN_NAME"]->ToString() == "id")
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
			if (reader["COLUMN_NAME"]->ToString() == "id")
				continue;
			//cout << "\t" << msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString());
			NamesRas.push_back(msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString()));
			NumberRasColumns++;
		}
		
		reader->Close();
		
		cout << "\n\nNumberDohColumns = " << NumberDohColumns << "\nNumberRasColumns = " << NumberRasColumns << "\n";
		
		// ����������� ���������� ������� � ��������: 
		// (������������ �� ������� �������, � ����� �������� ����������� ���������� ���-�� �������)
		cmdText = "SELECT COUNT(*) as count FROM [TestFin].[dbo].[doh_db]";
		cmd = gcnew SqlCommand(cmdText, connect);
		reader = cmd->ExecuteReader();

		//int MonthNumbers = reader->GetInt32(1);
		reader->Read();
		int MonthNumbers = Convert::ToInt32(reader["count"]->ToString());
		// ������� ��������� ���������� ������� � �������
		// �������� ���� � MONEY
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
				Money[counter*(NumberDohColumns+NumberRasColumns) + i] = Convert::ToInt32(reader[i+1]->ToString());
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
			for (int i = 0; i < NumberDohColumns; i++)
			{
				Money[counter * (NumberDohColumns + NumberRasColumns) + i + 2] = Convert::ToInt32(reader[i + 1]->ToString());
			}
			counter++;
		}

		reader->Close();

		if (Money[2] == 4)
		{
			int c = 0;
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