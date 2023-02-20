#include "DataBase.h"
#include "Functions.h"

const int Nsumma = 2400;





int main()
{
    
    SetConsoleCP(1251);     //setlocale(LC_ALL, "ru");
    SetConsoleOutputCP(1251);

    list<string> NamesRas;      // Лист названий категорий расходов
    list<string> NamesDoh;      // Лист названий категорий доходов
    list<string> Months;        // Текущие месяцы
    list<string> AllMonths;
    vector<int> Money;            // Числа, обозначающие денюжки

    DataBase DB1;

    DB1.Read(NamesRas, NamesDoh, Money, Months, Nsumma);

    /*Initialization(NamesRas, NamesDoh, Money, Months, AllMonths);
    WriteTable(NamesRas, NamesDoh, Money, Months);

    int choice = 0;
    bool a = true;
    while (a)
    {
        cout << "\n  Do you choice('1' to change some sum in the category, '2' to add category,\n'3' to change Cat Name,";
        cout << "'4' to add new sum in the category, '5' to add month, '0 to exit'): ";
        cin >> choice; cout << "\n";

        switch (choice)
        {
        case 1:
        {
            cout << "\nWhat category value you want to change?\n";
            int number;
            cin >> number;
            cout << "New value: ";
            int NewValue;
            cin >> NewValue;
            InsertValue(Money, number, NewValue);
            GetSums(NamesRas, NamesDoh, Money, Months);
            WriteTable(NamesRas, NamesDoh, Money, Months);
            break;
        }
        case 2:
        {
            AddCat(NamesRas, NamesDoh, Money, Months);
            GetSums(NamesRas, NamesDoh, Money, Months);
            WriteTable(NamesRas, NamesDoh, Money, Months);
            break;
        }
        case 3:
        {
            ChangeCatName(NamesRas, NamesDoh);
            WriteTable(NamesRas, NamesDoh, Money, Months);
            break;
        }
        case 4:
        {
            AddValue(Money);
            GetSums(NamesRas, NamesDoh, Money, Months);
            WriteTable(NamesRas, NamesDoh, Money, Months);
            break;
        }
        case 5:
        {
            AddMonth(NamesRas, NamesDoh, Money, Months, AllMonths);
            GetSums(NamesRas, NamesDoh, Money, Months);
            WriteTable(NamesRas, NamesDoh, Money, Months);
            break;
        }
        case 0:
            a = false;
            break;
        }
    }*/





    
    
    
    
    
    return 0;
}
