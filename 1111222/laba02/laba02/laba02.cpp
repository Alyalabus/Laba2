#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <float.h>
#include <unordered_set>

using namespace std;

template <typename T>
T GetCorrectNumber(T min, T max) {
	T x;
	while (!(cin >> x) || cin.fail() || (std::cin.peek() != '\n') || (x < min) || (x > max)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Введите корректные данные (число в пределах: " << min << "-" << max << "):";
	}
	return x;
}

class Trubka {
public:
	//static int MAX_TRUBKA_ID;
	string name;
	bool workingStatus;

	Trubka() {
		//id = MAX_TRUBKA_ID++;
		diameter = 0;
		length = 0;
		workingStatus = false;
	}

	friend istream& operator>> (istream& in, Trubka& trubka) {
		in.clear();
		in.ignore(INT_MAX, '\n');
		cout << "Имя: ";
		getline(in, trubka.name);
		cout << "\nДиаметр: ";
		trubka.diameter = GetCorrectNumber(0.0, DBL_MAX);
		cout << "\nДлина: ";
		trubka.length = GetCorrectNumber(0.0, DBL_MAX);
		cout << "\nСтатус трубы: ";
		trubka.workingStatus = GetCorrectNumber(0, 1);
		return in;
	}

	friend ostream& operator<< (ostream& out, Trubka& trubka) {
		out << "Труба: " << endl;
		out << "ID: " << trubka.id << endl;
		out << "Имя: " << trubka.name << endl;
		out << "Диаметр: " << trubka.diameter << endl;
		out << "Длина: " << trubka.length << endl;
		if (trubka.workingStatus) {
			out << "Работает" << endl;
		}
		else {
			out << "В ремонте" << endl;
		}
		return out;
	}

	void redactTrubka();
	void saveTrubka(ofstream& fout);
	void loadTrubka(ifstream& fin);
	int GetID() { return id; }
	void SetWorkingStatus(bool status) { workingStatus = status; }

private:
	int id;
	double diameter;
	double length;
};

class Stancia {
public:
	//static int MAX_STANCIA_ID;
	string name;

	Stancia() {
		//id = MAX_STANCIA_ID++;
		count = 0;
		countInWork = 0;
		efficiency = 0;
	}

	friend istream& operator>> (istream& in, Stancia& stancia) {
		in.clear();
		in.ignore(INT_MAX, '\n');
		cout << "Имя: ";
		getline(in, stancia.name);
		cout << "\nКоличество цехов: ";
		stancia.count = GetCorrectNumber(1, INT_MAX);
		cout << "\nКоличество цехов в работе:";
		stancia.countInWork = GetCorrectNumber(1, stancia.count);
		cout << "\nЭффективность: ";
		stancia.efficiency = GetCorrectNumber(0.0, 1.0);
		return in;
	}

	friend ostream& operator<< (ostream& out, Stancia& stancia) {
		out << "Компрессорная станция: " << endl;
		out << "ID: " << stancia.id << endl;
		out << "Имя: " << stancia.name << endl;
		out << "Количество цехов: " << stancia.count << endl;
		out << "Количество цехов в работе: " << stancia.countInWork << endl;
		out << "Эфективность: " << stancia.efficiency << endl;
		return out;
	}

	void redactStancia();
	void saveStancia(ofstream& fout);
	void loadStancia(ifstream& fin);
	int GetID() { return id; }
	double GetUnusedShopsPercentage() { return ((double)(count - countInWork) / count); }

private:
	int id;
	int count;
	int countInWork;
	double efficiency;
};

template <typename T>
using filterPipes = bool (*) (Trubka& trubka, T param);

template <typename T>
using filterStations = bool (*) (Stancia& stancia, T param);

void menu() {
	cout << "1. Добавить трубу" << endl;
	cout << "2. Добавить компрессорную станцию" << endl;
	cout << "3. Просмотр всех объектов " << endl;
	cout << "4. Редактировать трубу " << endl;
	cout << "5. Редактировать компрессорную станцию " << endl;
	cout << "6. Сохранить " << endl;
	cout << "7. Загрузить " << endl;
	cout << "8. Найти объекты по фильтру " << endl;
	cout << "0. Выход " << endl;
}

void Trubka::redactTrubka() {
	cout << "Состояние трубы: ";
	if (workingStatus) {
		cout << "Работает" << endl;
	}
	else {
		cout << "В ремонте" << endl;
	}
	cout << "Статус трубы (0 - если труба в ремонте, 1 - если труба работает): ";
	workingStatus = GetCorrectNumber(0, 1);
	cout << "Труба успешно изменена!" << endl;
}

void Stancia::redactStancia() {
	cout << "ID: " << id << endl;
	cout << "Количество цехов: " << count << endl;
	cout << "Рабочих цехов: " << countInWork << endl;
	cout << "Введите количество рабочих цехов: ";
	countInWork = GetCorrectNumber(0, count);
	cout << "Компрессорная станция успешно изменена!" << endl;
}

void Trubka::saveTrubka(ofstream& fout) {
	fout << id << endl;
	fout << name << endl;
	fout << diameter << endl;
	fout << length << endl;
	fout << workingStatus << endl;
}

void Stancia::saveStancia(ofstream& fout) {
	fout << id << endl;
	fout << name << endl;
	fout << count << endl;
	fout << countInWork << endl;
	fout << efficiency << endl;
}

void save(unordered_map<int, Trubka>& trubkaline, unordered_map<int, Stancia>& stationsGroup, string filepath) {
	ofstream fout;
	fout.open(filepath);

	if (!fout) {
		cout << "Не получилось открыть файл!";
	}
	else {
		fout << trubkaline.size() << " " << stationsGroup.size() << endl;
		for (auto& trubka : trubkaline)
			trubka.second.saveTrubka(fout);
		for (auto& station : stationsGroup)
			station.second.saveStancia(fout);
		fout.close();
	}
}



int main()
{
	setlocale(LC_ALL, "Russian");

	int number;
	unordered_map <int, Trubka> trubkaline;
	unordered_map <int, Stancia> Stancia_group;

	while (true) {

		menu();

		cout << "Пункт номер: ";

		number = GetCorrectNumber(0, 8);

		cout << endl << endl;

		switch (number)
		{
		case 0:
			return 0;

		case 1:
		{
			Trubka newTrubka;
			cin >> newTrubka;
			cout << "Труба добавлена" << endl;
			trubkaline.insert({ newTrubka.GetID(), newTrubka });
			break;
		}
		case 2:
		{
			Stancia newStancia;
			cin >> newStancia;
			Stancia_group.insert({ newStancia.GetID(), newStancia });
			cout << "Компрессорная станция добавлена" << endl;
			break;
		}
		case 3:
		{
			cout << "Трубы: " << endl;
			for (auto& trubka : trubkaline)
				cout << trubka.second << endl << endl;
			cout << "Компрессорные станции: " << endl;
			for (auto& Stancia : Stancia_group)
				cout << Stancia.second << endl << endl;

			break;
		}
		case 4:
		{

		}
		case 5:
		{

		}
		case 6:
		{
			string pathToFile;
			cout << "Введите название файла сохранения: ";
			cin >> pathToFile;
			save(trubkaline, Stancia_group, pathToFile);
			break;
		}
		case 7:
		{
		}
		case 8:
		{

		}
		default:
			break;

			cout << endl << endl;
		}
	}
}


