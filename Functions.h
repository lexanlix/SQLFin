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


ref class Functions
{

	SqlConnection^ connect;
	SqlConnectionStringBuilder^ connStringBuilder;


	int NumberDohColumns = 0, NumberRasColumns = 0;// если число столбцов меняется - они ТОЖЕ должны меняться
	int MonthNumbers;
	int Nsumma;

	void ConnectToDB();

public:
	void SetNsumma();
	
	void ChangeCellValue(list<string>& NamesRas, list<string>& NamesDoh);

	void Read(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money);

	void WriteDataFromDB(list<string>& NamesRas, list<string>& NamesDoh, list<string>& AllMonths);

	void AddNewCategory(list<string>& NamesRas, list<string>& NamesDoh);
	
	void DeleteCategory(list<string>& NamesRas, list<string>& NamesDoh);

	// Функции из бывшего Functions.h
	void GetSums(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months);

	void Initialization(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths);
	// Инициализация данных в листах

	void WriteTable(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months);
	// Вывод в консоль всей таблицы

	template <typename T>
	void PrintList(const list<T>& lst);

	template <typename T>
	void PrintList2(const list<T>& lst);

	void InsertValue(vector<int>& Mon, int number, int NewValue);
	//Изменить значение суммы в ячейке

	void AddMonth(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths);
	// Добавить следующий месяц

	void ChangeCatName(list<string>& NamesRas, list<string>& NamesDoh);
	//Изменить название категории

	void AddValue(vector<int>& Mon);
	// Изменить значение в ячейке суммированием новых данных

};

