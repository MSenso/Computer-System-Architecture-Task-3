
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <map>
#include <string>

// Метод для создания мап
std::map<int, char> makeMap() {
	std::map<int, char> codeTable;
	codeTable.insert(std::make_pair(1001, 'a'));
	codeTable.insert(std::make_pair(1021, 'b'));
	codeTable.insert(std::make_pair(1031, 'c'));
	codeTable.insert(std::make_pair(1041, 'd'));
	codeTable.insert(std::make_pair(1051, 'e'));
	codeTable.insert(std::make_pair(1061, 'f'));
	codeTable.insert(std::make_pair(1071, 'g'));
	codeTable.insert(std::make_pair(1081, 'h'));
	codeTable.insert(std::make_pair(1091, 'i')); 
	codeTable.insert(std::make_pair(1201, 'j'));
	codeTable.insert(std::make_pair(1221, 'k'));
	codeTable.insert(std::make_pair(1231, 'l'));
	codeTable.insert(std::make_pair(1241, 'm'));
	codeTable.insert(std::make_pair(1251, 'n'));
	codeTable.insert(std::make_pair(1261, 'o'));
	codeTable.insert(std::make_pair(1271, 'p'));
	codeTable.insert(std::make_pair(1281, 'q'));
	codeTable.insert(std::make_pair(1291, 'r'));
	codeTable.insert(std::make_pair(1301, 's'));
	codeTable.insert(std::make_pair(1321, 't'));
	codeTable.insert(std::make_pair(1331, 'u'));
	codeTable.insert(std::make_pair(1341, 'v'));
	codeTable.insert(std::make_pair(1351, 'w'));
	codeTable.insert(std::make_pair(1361, 'x'));
	codeTable.insert(std::make_pair(1371, 'y'));
	codeTable.insert(std::make_pair(1381, 'z'));
	return codeTable;
}

const int threadsNumber = 4; // Количество потоков
const std::map<int, char> codeTable = makeMap(); // Таблица шифров

typedef struct threadData {	// Структура для хранения данных, полученных в работе потока
	std::string res;	// Дешифрованная строка
	std::string work;	// Зашированная строка
	int returnValue;	// Число, характеризующее, верна ли входная зашифрованная строка
};

void Decoding(threadData &data) {	// Метод расшифровки
	std::string work = data.work;
	for (int i = 0; i < work.length() / 4; i++) {	// Делим длину на четыре, т.к. все буквы закодированы четырьмя цифрами
		int encodedLetter = std::stoi(work.substr(i * 4, 4));	// Парсинг строки
		if (codeTable.count(encodedLetter) > 0) {	// Если такое число есть в таблице шифров
			char decodedLetter = codeTable.at(encodedLetter);	// Находим, какая буква закодирована таким числом
			data.res += decodedLetter;	// Записываем букву в дешифрованную строку
		}
		else data.returnValue = 1;	// Возвращаем 1, т.к. входные данные некорректны
	}
}
bool IsDigitString(const std::string &text)	// Проверка, состоит ли строка только из чисел
{
	return text.find_first_not_of("0123456789") == std::string::npos;	// Проверка, что нет символа, не обозначающего цифру
}
int LetterIndex(int workSize[], int index) {	// Получение индекса текущей буквы
	int sum = 0;
	for (int i = 0; i < index; i++) {
		sum += workSize[i];	// Суммируем индексы всех предыдущих букв
	}
	return sum;
}

int main() {

	std::string text;
	std::cout << "Input your encdoded text: \n";
	std::cin >> text; // Ввод дешифрованной строки

	if (text.length() % threadsNumber != 0 || text.length() == 0 || text.length() > 100) {		// Если количество символов не кратно 4 (все буквы кодируются 4 цифрами) или строка пустая или длина больше 100, это некорректный ввод
		std::cout << "Incorrect text length!";
		return 0;
	}
	if (!IsDigitString(text)) {	// Если в строке есть не только цифры, это некорректный ввод (буквы кодируются только числовыми последовательностями)
		std::cout << "Sorry but there are incorrect symbols in your encoded text";
		return 0;
	}

	int letterCount = text.length() / 4;	// Т.к. буквы кодируются 4 цифрами, то количество букв в четыре раза меньше количества символов
	int workSize[threadsNumber] = { 0 };	// Массив, в котором будет храниться количество букв, которые нужно дешифровать, для каждого потока
	for (int i = 0; i < letterCount; i++) {	// Раздаем каждому потоку некоторое количество букв
		workSize[i % 4]++;	// Поочередно каждому потоку даем по еще одной букве, пока цикл не закончится
	}

	std::thread threads[threadsNumber];	// Массив потоков
	threadData threadsData[threadsNumber];	// Массив структур, хранящих данные о работе потоков

	for (int i = 0; i < threadsNumber; i++) {	// Запускаем потоки в цикле
		std::string work = text.substr(LetterIndex(workSize, i) * 4, workSize[i] * 4);	// Определяем первую букву для i-того потока и умножаем количество букв на 4 (равно количеству символов, которые надо обработать)
		threadsData[i].res = "";	// Присваиваем начальные данные для дешифрованной строки
		threadsData[i].work = work; // Присваиваем начальные данные для зашифрованной строки
		threadsData[i].returnValue = 0; // Присваиваем начальные данные для числа, показывающего, корректные ли данные в зашифрованной строке
		threads[i] = std::thread(Decoding, std::ref(threadsData[i]));	// Создание i-того потока, который обработает шифрованную строку в методе Decoding
	}

	std::string decodedText;	// Результирующая строка
	int outputValues = 0;	// Сумма чисел, характеризующих корректные данные в частях зашифрованной строки
	for (int i = 0; i < threadsNumber; i++) {
		threads[i].join();	// Завершаем работу i-того потока
		decodedText += threadsData[i].res; // Записываем в результирующую строку res
		outputValues += threadsData[i].returnValue; // Прибавляем число, характеризующее корректный ввод
	}
	if (outputValues != 0) std::cout << "Sorry but there are inappropriate values in your encoded text"; // Если сумма не равна нулю, то хотя бы в одной части зашифрованной строки есть некорректные символы
	else std::cout << "Decoded text: " + decodedText; // Вывод расшифрованной строки
}