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
// ������������� ������ � ������

void WriteTable(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months, int Nsumma);
// ����� � ������� ���� �������

template <typename T>
void PrintList(const list<T>& lst);

template <typename T>
void PrintList2(const list<T>& lst);

void InsertValue(vector<int>& Mon, int number, int NewValue);
//�������� �������� ����� � ������

void AddCat(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months);
// ���������� ����� ���������

void AddMonth(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths);
// �������� ��������� �����

void ChangeCatName(list<string>& NamesRas, list<string>& NamesDoh);
//�������� �������� ���������

void AddValue(vector<int>& Mon);
// �������� �������� � ������ ������������� ����� ������


