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
	// Как будет:
	/*cout << "\n\nВведите изначальную сумму средств, которой вы располагали перед вносимыми изменениями в бюджет: ";
	int N;
	cin >> N;
	Nsumma = N;*/

	Nsumma = 2400;
}


void Functions::ChangeCellValue(list<string>& NamesRas, list<string>& NamesDoh)
{		// Изменение значения в определенной ячейке
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

		if (categoryNum > NumberDohColumns) // если из категории расходов
		{
			id = "rasId";
			
			auto it = NamesRas.begin();
			advance(it, categoryNum - NumberDohColumns - 1);
			string name = *it;
			String^ One = gcnew String(name.c_str()); // string to System::String

			cmdText += "[ras_db] SET " + One + " = ";
		}
		else // если из доходов
		{
			id = "dohId";

			auto it = NamesDoh.begin();
			advance(it, categoryNum - 1);
			string name = *it;
			String^ One = gcnew String(name.c_str()); // string to System::String

			cmdText += "[doh_db] SET " + One + " = ";
		}
		
		cmdText += Convert::ToString(value) + " WHERE " + id + " = " + Convert::ToString(monthNum);

		// Пример запроса cmdText = "UPDATE [TestFin].[dbo].[doh_db] SET fromCard = 0 WHERE dohId = 3";

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
{		// вывод в консоль таблицы чисто с базы данных, без промежуточной записи в контейнеры
	cout << "\nНачальная сумма = " << Nsumma;

	vector<string> cconsts = { "Доходы", "Расходы", "Расход в месяц", "Остаток" };
	unsigned short int year = 2023;
	cout << "\n Год " << year << " ";
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

		int consumption = 0, balance = Nsumma; // расход и остаток

		// Вывод строки в консоль сразу после чтения с БД
		while (reader->Read())
		{
			cout << "\n" << *it << "    ";

			for (int i = 1; i < NumberDohColumns + NumberRasColumns + 2; i++) // первый и второй id таблиц пропускаем
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
{   // Чтение данных с БД для занесения в контейнеры, чтобы потом вывести в функции WriteTable

	try
	{
		// Подключаемся к БД
		ConnectToDB();
		String^ cmdText = "SELECT COLUMN_NAME FROM TestFin.INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'doh_db'";
		SqlCommand^ cmd = gcnew SqlCommand(cmdText, connect);
		connect->Open();

		SqlDataReader^ reader = cmd->ExecuteReader();

		// Цикл по таблице ДОХОДОВ, который получает имена столбцов

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

		// Цикл по таблице РАСХОДОВ, который получает имена столбцов
		while (reader->Read())
		{
			if (reader["COLUMN_NAME"]->ToString() == "rasId")
				continue;
			//cout << "\t" << msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString());
			NamesRas.push_back(msclr::interop::marshal_as<string>(reader["COLUMN_NAME"]->ToString()));
			NumberRasColumns++;
		}

		reader->Close();


		// Определение количества записей в таблицах: 
		// (Определяется по таблице Доходов, в обоих таблицах обязательно ОДИНАКОВОЕ кол-во записей)
		cmdText = "SELECT COUNT(*) as count FROM [TestFin].[dbo].[doh_db]";
		cmd = gcnew SqlCommand(cmdText, connect);
		reader = cmd->ExecuteReader();

		reader->Read();
		MonthNumbers = Convert::ToInt32(reader["count"]->ToString());

		reader->Close();


		cmdText = "SELECT * FROM [TestFin].[dbo].[doh_db]";
		cmd = gcnew SqlCommand(cmdText, connect);
		reader = cmd->ExecuteReader();

		// Цикл по строкам таблицы ДОХОДОВ, который получает значения
		Money.assign((NumberRasColumns + NumberDohColumns + 2) * MonthNumbers, 0); // задаем вектор с нулями

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

		// Цикл по строкам таблицы РАСХОДОВ, который получает значения

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
// Инициализация данных в листах
{
	// выполняю первичное считывание данных, заполняю рас, дох и мани, а также переменные кол-ва столбцов и строк:
	Read(NamesRas, NamesDoh, Money);

	AllMonths = { "Январь  ", "Февраль ", "Март    ", "Апрель  ", "Май     ", "Июнь    ", "Июль    ",
					"Август  ", "Сентябрь", "Октябрь ", "Ноябрь  ", "Декабрь " };

	list<string>::iterator itAllMonths = AllMonths.begin();
	//list<string>::iterator itMonths = Months.begin();

	/*advance(itMonths, Months.size() - 1);
	if (*itMonths != "Декабрь ")
		advance(itAllMonths, Months.size() % AllMonths.size());
	Months.push_back(*itAllMonths);*/

	for (int i = 0; i < MonthNumbers; i++)
	{
		Months.push_back(*itAllMonths);
		advance(itAllMonths, 1);
	}

	// Ищем нужные суммы
	GetSums(NamesRas, NamesDoh, Money, Months);

	WriteTable(NamesRas, NamesDoh, Money, Months);
}

void Functions::WriteTable(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months)     // Вывод в консоль всей таблицы
{
	cout << "\nНачальная сумма = " << Nsumma;

	vector<string> cconsts = { "Доходы", "Расходы", "Расход в месяц", "Остаток" };
	unsigned short int year = 2023;
	cout << "\n Год " << year << " ";
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
	list<string>::iterator check = Months.begin();  // Для предотвращения выхода за область данных list Money

	advance(end, size);
	advance(check, Months.size() - 1);
	for (auto i = Months.begin(); i != Months.end(); ++i)
	{
		if ((*i == "Январь  ") && (i != Months.begin()))
			cout << "\n Год " << ++year << " ";
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


// Не знаю для чего это
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

	// Сначала добавление в листы
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

	// Затем добавление столбца в базу данных
	try
	{
		ConnectToDB();

		String^ cmdText = "ALTER TABLE [TestFin].[dbo]."; // добавление столбца
		String^ cmdText2 = "UPDATE [TestFin].[dbo].";     // заполнение нулями

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

		// Примеры: ALTER TABLE [TestFin].[dbo].[doh_db] ADD salary int
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

	// Попробуем без изменения массива Money
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

	// Сначала удаление из листов
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

	// Затем из БД
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

		// Примеры: ALTER TABLE [TestFin].[dbo].[doh_db] DROP COLUMN salary

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



// Над остальными поработать:
void Functions::InsertValue(vector<int>& Mon, int number, int NewValue)    //Изменить значение суммы в ячейке
{
	Mon[number - 1] = NewValue;
}



// Добавить следующий месяц
void Functions::AddMonth(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths)
{   // и удаление
	list<string>::iterator it = AllMonths.begin();
	list<string>::iterator current = Months.begin();

	advance(current, Months.size() - 1);
	if (*current != "Декабрь ")
		advance(it, Months.size() % AllMonths.size());
	Months.push_back(*it);
	for (int i = 0; i < (NamesRas.size() + NamesDoh.size() + 2); ++i)
	{
		Money.push_back(0);
	}


	try
	{
		// Запрос на добавление нулей в таблицу ДОХОДОВ

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

		// Запрос на добавление нулей в таблицу РАСХОДОВ

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
		// получившийся текст запроса "INSERT INTO [TestFin].[dbo].[ras_db](Eat, Stationery, Transport, Phone) VALUES(0, 0, 0, 0)";
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

void Functions::ChangeCatName(list<string>& NamesRas, list<string>& NamesDoh)   //Изменить название категории
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

void Functions::AddValue(vector<int>& Mon)   // Изменить значение в ячейке суммированием новых данных
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

