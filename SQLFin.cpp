#include "Functions.h"


int main()
{
    
    SetConsoleCP(1251);     //setlocale(LC_ALL, "ru");
    SetConsoleOutputCP(1251);

    list<string> NamesRas;      // Лист названий категорий расходов
    list<string> NamesDoh;      // Лист названий категорий доходов
    list<string> Months;        // Текущие месяцы
    list<string> AllMonths;
    vector<int> Money;            // Числа, обозначающие денюжки

    Functions F1;

    F1.SetNsumma();

    F1.Initialization(NamesRas, NamesDoh, Money, Months, AllMonths);
    // Загрузка данных с БД и вывод в консоль - все тут сразу

    

    int choice = 0;
    bool a = true;
    while (a)
    {
        cout << "\n  Do your choice('1' to change some sum in the category, '2' to add category, '3' to delete category,";
        cout << "\n'4' to change Cat Name, '5' to add new sum in the category, '6' to add month, '7' to write from DB, '0 to exit'): ";
        cin >> choice; cout << "\n";

        switch (choice)
        {
        case 1:
        {
            F1.ChangeCellValue(NamesRas, NamesDoh);
            // Без обновления vector Money
            F1.WriteDataFromDB(NamesRas, NamesDoh, AllMonths);
            break;
        }
        case 2:
        {
            F1.AddNewCategory(NamesRas, NamesDoh);
            // Без обновления vector Money
            F1.WriteDataFromDB(NamesRas, NamesDoh, AllMonths);
            break;
        }
        case 3:
        {
            F1.DeleteCategory(NamesRas, NamesDoh);
            F1.WriteDataFromDB(NamesRas, NamesDoh, AllMonths);
            break;
        }
        case 4:
        {
            /*ChangeCatName(NamesRas, NamesDoh);
            WriteTable(NamesRas, NamesDoh, Money, Months);*/
            break;
        }
        case 5:
        {
            /*AddValue(Money);
            GetSums(NamesRas, NamesDoh, Money, Months);
            WriteTable(NamesRas, NamesDoh, Money, Months);*/
            break;
        }
        case 6:
        {
            F1.AddMonth(NamesRas, NamesDoh, Money, Months, AllMonths);
            F1.GetSums(NamesRas, NamesDoh, Money, Months);
            F1.WriteTable(NamesRas, NamesDoh, Money, Months);
            break;
        }
        case 7:
        {
            F1.WriteDataFromDB(NamesRas, NamesDoh, AllMonths);
            break;
        }
        case 0:
            a = false;
            break;
        }
    }





    
    
    
    
    
    return 0;
}
