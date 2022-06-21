#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <locale>
#include <Windows.h>

using namespace std;

int symbolCount = 32;

// Функция перевода в двоичную систему
string FuncTo2(int number)
{
    if (number == 1)
    {
        return "1";
    }
    else
        return FuncTo2(number / 2) + to_string(number % 2);
}

int main()
{
    setlocale(LC_ALL, "RUS");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int i = 0;

    // Нахождения числа информационных разрядов
    while (pow(2, i) < symbolCount)
    {
        i++;
    }

    // Контейнер для хранения пар - символ - код Хемминга
    map<char, string> Hamming;

    char a[] = { 'а', 'б', 'в', 'г', 'д', 'е', ' ', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'э', 'ю', 'я' };

    Hamming[a[0]] = "00000";

    for (int j = 1; j < symbolCount; j++)
    {
        string s = FuncTo2(j);
        while (s.size() < i)
            s = "0" + s;
        Hamming[a[j]] = s;
    }

    // Первичный вывод бинарных кодов
    for (auto it : Hamming)
    {
        cout << it.first << " " << it.second << endl;
    }

    for (auto it : Hamming)
    {
        string s = it.second;
        int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0;

        // Первая проверка
        if (((s[0]-'0') + (s[1] - '0') + (s[3] - '0') + (s[4] - '0')) % 2 != 0)
            k1 = 1;

        // Вторая проверка
        if (((s[0] - '0') + (s[2] - '0') + (s[3] - '0')) % 2 != 0)
            k2 = 1;

        // Третья проверка
        if (((s[1] - '0') + (s[2] - '0') + (s[3] - '0')) % 2 != 0)
            k3 = 1;

        // Четвертая проверка
        if (((s[4] - '0')) % 2 != 0)
            k4 = 1;

        if ((k1 + k2 + (s[0] - '0') + k3 + (s[1] - '0') + (s[2] - '0') + (s[3] - '0') + k4 + (s[4] - '0')) % 2 != 0)
            k5 = 1;
        Hamming[it.first] = (to_string(k1) + to_string(k2) + s[0] + to_string(k3) + s[1] + s[2] + s[3] + to_string(k4) + s[4] + to_string(k5));
    }
        // Вторичный вывод конечных кодов Хемминга
    for (auto it : Hamming)
    {
        cout << it.first << " " << it.second << endl;
    }

    string targetStr;
    cout << "Введите строку для кодирования: " << endl;
    getline(cin, targetStr);
    cout << "Строка для кодирования : " << targetStr << endl;
    string encodedStr = "";
    for (auto c : targetStr)
    {
        encodedStr += Hamming[c];
    }
    cout << "Закодированная строка : " << endl << encodedStr << endl;
    
    targetStr = "";
    int k = 0;

    vector<string> decodedStr;

    for (auto c : encodedStr)
    {
        targetStr += c;
        if (targetStr.size() == 10)
        {
            decodedStr.push_back(targetStr);
            targetStr = "";
            k = 0;
        }
        k++;
    }
    // Правильный код Хэмминга
    // decodedStr[0] = "0011001001";

    // Неправильный прием первого кода (Двойная ошибка)
    // decodedStr[0] = "0011011001";

    cout << "Декодированная строка: " << endl;

    for (auto it : decodedStr)
    {
        int number = 0, result = 0;
        {
            for (auto c : it)
                result += c - '0';
        }

        if (result % 2 == 0)
            result = 0;
        else
            result = 1;

        // Проверка
        string S1, S2, S3, S4;

        S1 = to_string(((it[0]-'0') + (it[2]-'0') + (it[4]-'0') + (it[6]-'0') + (it[8]-'0')) % 2);
        S2 = to_string(((it[1] - '0') + (it[2] - '0') + (it[5] - '0') + (it[6] - '0')) % 2);
        S3 = to_string(((it[3] - '0') + (it[4] - '0') + (it[5] - '0') + (it[6] - '0')) % 2);
        S4 = to_string(((it[7] - '0') + (it[8] - '0')) % 2);

        if (stoi(S1) % 2 != 0 || stoi(S2) % 2 != 0 || stoi(S3) % 2 != 0 || stoi(S4) % 2 != 0)
            result++;

        string resultString = "";


        switch (result)
        {
        case 0:
        {
            for (auto ham : Hamming)
            {
                if (ham.second == it)
                    resultString += ham.first;
            }
        }
        break;
        case 2:
        {
            int number = 0;
            resultString = S4 + S3 + S2 + S1;
            for (int m = 0; m < resultString.size(); m++)
            {
                number *= 2;
                number += resultString[m] - '0';  // '0'-'0' равно 0, '1'-'0' Равно 1
            }
            
            if (it[number - 1] == 0)
                it[number - 1] = '1';
            else
                it[number - 1] = '0';

            cout << "Обнаружена и исправлена одиночная ошибка в " << number << " разряде" << endl;

            resultString = "";
            for (auto ham : Hamming)
            {
                if (ham.second == it)
                    resultString += ham.first;
            }
        }
        break;
        default:
            cout << "Обнаружена двойная ошибка" << endl;
            break;
        }

        cout << resultString;
    }
    return 0;
}