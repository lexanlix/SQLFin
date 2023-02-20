#include "Functions.h"

void GetSums(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, int Nsumma)
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

void Initialization(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths)
// ������������� ������ � ������
{
    NamesDoh = { " �������� ", " � �����  " };
    NamesRas = { "   ���    ", "����������", "��������� ", " �������  ", };

    AllMonths = { "������  ", "������� ", "����    ", "������  ", "���     ", "����    ", "����    ",
                    "������  ", "��������", "������� ", "������  ", "������� " };
    Months = { "������  ", "������� ", "����    ", "������  ", "���     ", "����    " };
    Money.assign((NamesRas.size() + NamesDoh.size() + 2) * Months.size(), 0);

    // ��������� ����� ��� �������� ������������
    auto sdvig = Money.begin();
    advance(sdvig, 12);
    int kek = 5;

    auto Seven = Money.begin(),
        Eight = Money.begin(),
        itDoh = Money.begin(),
        itRas = Money.begin();
    advance(Seven, NamesRas.size() + NamesDoh.size());
    advance(Eight, NamesRas.size() + NamesDoh.size() + 1);
    advance(itRas, NamesDoh.size());

    for (auto it = Money.begin(); it != sdvig; ++it)    // ��������� money �� ��������� (��� �����������)
    {
        if ((it != Seven) && (it != Eight))
        {
            *it = (kek++) * 2;
        }
        else if (it == Eight)
        {
            *it = 0;
            advance(Seven, NamesRas.size() + NamesDoh.size() + 2);
            advance(Eight, NamesRas.size() + NamesDoh.size() + 2);
        }
    }
    // ���� ������ �����
    GetSums(NamesRas, NamesDoh, Money, Months, 2400);

}

void WriteTable(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months, int Nsumma)     // ����� � ������� ���� �������
{
    cout << "\n��������� ����� = " << Nsumma;

    vector<string> cconsts = { "������", "�������", "������ � �����", "�������" };
    unsigned short int year = 2022;
    cout << "\n ��� " << year << " ";
    cout << "         " << cconsts[0] << "         ";
    cout << "                                 " << cconsts[1] << "                                 \n           ";


    for (auto i = NamesDoh.begin(); i != NamesDoh.end(); ++i)
    {
        cout << *i << "  ";
    }

    for (auto i = NamesRas.begin(); i != NamesRas.end(); ++i)
    {
        cout << *i << "  ";
    }
    cout << cconsts[2] << "  " << cconsts[3];

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

template <typename T>
void PrintList(const list<T>& lst)
{
    cout << "\n";
    for (auto i = lst.cbegin(); i != lst.cend(); ++i)
    {
        cout << *i << "    ";
    }
    cout << "\n";
}

template <typename T>
void PrintList2(const list<T>& lst)
{
    cout << "\n";
    for (auto i = lst.cbegin(); i != lst.cend(); ++i)
    {
        cout << *i << "          ";
    }
    cout << "\n";
}


void InsertValue(vector<int>& Mon, int number, int NewValue)    //�������� �������� ����� � ������
{
    Mon[number - 1] = NewValue;
}

void AddCat(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string> Months)  // ���������� ����� ���������
{
    cout << "What category you want to change(R or D)?:";
    char choice;
    cin >> choice;
    cout << "Write name of category: ";
    string CatName;
    cin >> CatName;
    int size;
    if (choice == 'D')
    {
        NamesDoh.push_back(CatName);
        size = NamesDoh.size() - 1;
    }
    else if (choice == 'R')
    {
        NamesRas.push_back(CatName);
        size = NamesRas.size() + NamesDoh.size() - 1;
    }

    for (auto i = Months.begin(); i != Months.end(); ++i)
    {
        Money.insert(Money.begin() + size, 0);
        size += NamesDoh.size() + NamesRas.size() + 2;
    }
}

void AddMonth(list<string>& NamesRas, list<string>& NamesDoh, vector<int>& Money, list<string>& Months, list<string>& AllMonths)   // �������� ��������� �����
{
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
}

void ChangeCatName(list<string>& NamesRas, list<string>& NamesDoh)   //�������� �������� ���������
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

void AddValue(vector<int>& Mon)   // �������� �������� � ������ ������������� ����� ������
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