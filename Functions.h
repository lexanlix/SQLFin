#include <iostream>
#include <list>
#include <vector>
#include <clocale>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>
#include <string>

using namespace std;

void GetSums(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, int Nsumma);

void Initialization(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths);
// Инициализация данных в листах

void WriteTable(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months, int Nsumma);
// Вывод в консоль всей таблицы

template <typename T>
void PrintList(const list<T>& lst);

template <typename T>
void PrintList2(const list<T>& lst);

void InsertValue(vector<int>& Mon, int number, int NewValue);
//Изменить значение суммы в ячейке

void AddCat(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months);
// Добавление новой категории

void AddMonth(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths);
// Добавить следующий месяц

void ChangeCatName(list<string>& NamesRas, list<string>& NamesDoh);
//Изменить название категории

void AddValue(vector<int>& Mon);
// Изменить значение в ячейке суммированием новых данных


