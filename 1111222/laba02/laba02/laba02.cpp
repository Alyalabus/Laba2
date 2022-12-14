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
	static int MAX_TRUBKA_ID;
	string name;
	bool workingStatus;

	Trubka() {
		id = MAX_TRUBKA_ID++;
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
	void saveTrubka(ofstream& fout);//ofstream - для вывода (записи) данных в файл
	void loadTrubka(ifstream& fin);//ifstream - для ввода (чтения) данных из файла
	int GetID() { return id; }
	void SetWorkingStatus(bool status) { workingStatus = status; }

private:
	int id;
	double diameter;
	double length;
};

class Stancia {
public:
	static int MAX_STANCIA_ID;
	string name;

	Stancia() {
		id = MAX_STANCIA_ID++;
		count = 0;
		countInWork = 0;
		efficiency = 0;
	}

	friend istream& operator>> (istream& in, Stancia& stancia) {
		in.clear();
		in.ignore(INT_MAX, '\n');
		cout << "Имя: ";
		getline(in, stancia.name);//Getline () - функция  используемая для чтения строки или строки из входного потока.
		cout << "\nКоличество цехов: ";
		stancia.count = GetCorrectNumber(1, INT_MAX);
		cout << "\nКоличество цехов в работе:";
		stancia.countInWork = GetCorrectNumber(1, stancia.count);
		cout << "\nЭффективность: ";
		stancia.efficiency = GetCorrectNumber(0.0, 1.0);
		return in;
	}

	friend ostream& operator<< (ostream& out, Stancia& stancia) { //ostream-потоки вывода
		out << "Компрессорная станция: " << endl;
		out << "ID: " << stancia.id << endl;
		out << "Имя: " << stancia.name << endl;
		out << "Количество цехов: " << stancia.count << endl;
		out << "Количество цехов в работе: " << stancia.countInWork << endl;
		out << "Эфективность: " << stancia.efficiency << endl;
		return out;
	}

	void redactStancia();
	void saveStancia(ofstream& fout);//ofstream - для вывода (записи) данных в файл
	void loadStancia(ifstream& fin); //ifstream - для ввода (чтения) данных из файла
	int GetID() { return id; }
	double GetUnusedShopsPercentage() { return ((double)(count - countInWork) / count); }

private:
	int id;
	int count;
	int countInWork;
	double efficiency;
};

template <typename T>
using filterTrubkas = bool (*) (Trubka& trubka, T param);//Bool — тип, который может содержать в себе значения true (истина) и false (ложь)

template <typename T>
using filterStancians = bool (*) (Stancia& stancia, T param);

template <typename T>
vector<int> searchTrubkaByParam(unordered_map<int, Trubka>& trubkaline, filterTrubkas<T> f, T param) {
	vector<int> result;
	for (auto& trubka : trubkaline)
		if (f(trubka.second, param))
			result.push_back(trubka.second.GetID());//push_back () — способ, который увеличивает размер вектора на 1.
	return result;
}

template <typename T>
vector<int> searchStanciaByParam(unordered_map<int, Stancia>& stancia_group, filterStancians<T> f, T param) {
	vector<int> result;
	for (auto& stancia : stancia_group)
		if (f(stancia.second, param))
			result.push_back(stancia.second.GetID());
	return result;
}

bool checkTrubkaName(Trubka& trubka, string name) { return (trubka.name.find(name) != string::npos); }//String::npos — константа, которая была введена в язык для обозначения несуществующего индекса в массиве

bool checkTrubkaStatus(Trubka& trubka, bool status) { return (trubka.workingStatus == status); }

bool checkStationName(Stancia& stancia, string name) { return (stancia.name.find(name) != string::npos); }

bool checkStationUnusedShops(Stancia& stancia, double param) { return (stancia.GetUnusedShopsPercentage() >= param); }

void searchTrubkas(unordered_map<int, Trubka>& trubkaline, vector<int>& resultVector) {
	int number;
	cout << "Выберите фильтрацию (0 - по имени, 1 - по статусу трубы): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0) {
		string name;
		cout << "Введите имя для поиска труб: ";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		getline(cin, name);
		resultVector = searchTrubkaByParam(trubkaline, checkTrubkaName, name);
	}
	else if (number == 1) {
		bool status;
		cout << "\nВведите состояние трубы для поиска (0 - если она в ремонте, 1 - если она работает): ";
		status = GetCorrectNumber(0, 1);
		resultVector = searchTrubkaByParam(trubkaline, checkTrubkaStatus, status);
	}
}

void searchStancia(unordered_map<int, Stancia>& stancia_group, vector<int>& resultVector) {
	int number;
	cout << "Выберите фильтрацию (0 - по имени, 1 - по проценту не задействованных цехов): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0) {
		string name;
		cout << "\nВведите имя для поиска компрессорных станций: ";
		cin >> name;
		resultVector = searchStanciaByParam(stancia_group, checkStationName, name);
	}
	else if (number == 1) {
		double param;
		cout << "\nВведите процент не задействованных цехов для поиска (в долях от единицы): ";
		param = GetCorrectNumber(0.0, 1.0);
		resultVector = searchStanciaByParam(stancia_group, checkStationUnusedShops, param);
	}
}

int Trubka::MAX_TRUBKA_ID = 0;
int Stancia::MAX_STANCIA_ID = 0;

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

void save(unordered_map<int, Trubka>& trubkaline, unordered_map<int, Stancia>& stanciansGroup, string filepath) {
	ofstream fout;
	fout.open(filepath);

	if (!fout) {
		cout << "Не получилось открыть файл!";
	}
	else {
		fout << trubkaline.size() << " " << stanciansGroup.size() << endl;
		for (auto& trubka : trubkaline)
			trubka.second.saveTrubka(fout);
		for (auto& stancia : stanciansGroup)
			stancia.second.saveStancia(fout);
		fout.close();
	}
}

void Trubka::loadTrubka(ifstream& fin) {
	fin >> this->id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> this->diameter;
	fin >> this->length;
	fin >> this->workingStatus;
}

void Stancia::loadStancia(ifstream& fin) {
	fin >> id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> count;
	fin >> countInWork;
	fin >> efficiency;
}

void load(unordered_map<int, Trubka>& trubkaline, unordered_map<int, Stancia>& stanciansGroup, string filepath) {
	ifstream fin;

	fin.open(filepath, ios::in);
	if (!fin) {
		cout << "Не получилось открыть файл!";
	}
	else {
		Trubka::MAX_TRUBKA_ID = 0;
		Stancia::MAX_STANCIA_ID = 0;
		Trubka newTrubka;
		Stancia newStancia;
		trubkaline.clear();
		stanciansGroup.clear();
		int TrubkaNumber, stanciaNumber;
		fin >> TrubkaNumber >> stanciaNumber;
		for (int i(0); i < TrubkaNumber; i++) {
			newTrubka.loadTrubka(fin);
			trubkaline.insert({ newTrubka.GetID(), newTrubka });
			Trubka::MAX_TRUBKA_ID = (Trubka::MAX_TRUBKA_ID < newTrubka.GetID() ? newTrubka.GetID() : Trubka::MAX_TRUBKA_ID);
		}
		for (int i(0); i < stanciaNumber; i++) {
			newStancia.loadStancia(fin);
			stanciansGroup.insert({ newStancia.GetID(), newStancia });
			Stancia::MAX_STANCIA_ID = (Stancia::MAX_STANCIA_ID < newStancia.GetID() ? newStancia.GetID() : Stancia::MAX_STANCIA_ID);
		}

		Trubka::MAX_TRUBKA_ID++;
		Stancia::MAX_STANCIA_ID++;

		fin.close();
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
			if (trubkaline.size() == 0) {
				cout << "Редактирование не возможно: не была создана ни одна труба. Попробуйте снова." << endl;
				break;
			}
			int number;
			cout << "Выберите работу с одной трубой (введите 0) или с множеством (введите 1): ";
			number = GetCorrectNumber(0, 1);
			if (number == 0) {
				int TrubkaID;
				cout << "Введите id трубы: ";
				TrubkaID = GetCorrectNumber(0, Trubka::MAX_TRUBKA_ID);
				if (trubkaline.find(TrubkaID) == trubkaline.end()) {
					cout << "Труба с введённым id не была найдена" << endl;
					break;
				}
				cout << endl;
				int choice;
				cout << "Выберите действие с трубой (0 - редактирование, 1 - удаление): ";
				choice = GetCorrectNumber(0, 1);
				if (choice == 0)
					trubkaline[TrubkaID].redactTrubka();
				else if (choice == 1)
					trubkaline.erase(trubkaline.find(TrubkaID));

			}
			else if (number == 1) {
				int choicePackage;
				cout << "Введите \"0\" для редактирования труб по фильтру или \"1\" для ввода id труб для редактирования: ";
				choicePackage = GetCorrectNumber(0, 1);
				if (choicePackage == 0) {
					bool statusToSet;
					cout << "\nСтатус для установки выбранным трубам (0 - \"в ремонте\", 1 - \"в работе\"): ";
					statusToSet = (bool)GetCorrectNumber(0, 1);
					vector<int> searchVector;
					searchTrubkas(trubkaline, searchVector);
					for (auto& id : searchVector) {
						trubkaline[id].SetWorkingStatus(statusToSet);
					}
				}
				else if (choicePackage == 1) {
					unordered_set<int> ids;
					cout << "Введите количество редактируемых труб: ";
					int idsAmount = GetCorrectNumber(0, Trubka::MAX_TRUBKA_ID);
					cout << "\nВведите id труб для редактирования: ";
					for (int i(0); i < idsAmount; i++) {
						int id = GetCorrectNumber(0, Trubka::MAX_TRUBKA_ID - 1);
						if (trubkaline.find(id) != trubkaline.end())
							ids.insert(id);
						else {
							cout << "Труба с данным id не существует." << endl;
							i--;
						}
					}
					cout << "Статус для установки выбранным трубам (0 - \"в ремонте\", 1 - \"в работе\"): ";
					bool statusToSet = (bool)GetCorrectNumber(0, 1);
					for (auto& id : ids) {
						trubkaline[id].SetWorkingStatus(statusToSet);
					}
				}
			}
			break;
		}
		case 5:
		{
			if (Stancia_group.size() == 0) {
				cout << "Редактирование не возможно: не была создана ни одна компрессорная станция. Попробуйте снова." << endl;
				break;
			}

			int number;
			cout << "Выберите работу с одной компрессорной станцией (введите 0) или с множеством (введите 1): ";
			number = GetCorrectNumber(0, 1);
			if (number == 0) {
				int stanciaID;
				cout << "\nВведите id компрессорной станции: ";
				stanciaID = GetCorrectNumber(0, Stancia::MAX_STANCIA_ID);
				if (Stancia_group.find(stanciaID) == Stancia_group.end()) {
					cout << "Станция с введённым id не была найдена" << endl;
					break;
				}
				cout << endl;
				int choice;
				cout << "Выберите действие со станцией (0 - редактирование, 1 - удаление): ";
				choice = GetCorrectNumber(0, 1);
				if (choice == 0)
					Stancia_group[stanciaID].redactStancia();
				else if (choice == 1)
					Stancia_group.erase(Stancia_group.find(stanciaID));

			}
			else if (number == 1) {

				int choicePackage;
				cout << "Введите \"0\" для редактирования станций по фильтру или \"1\" для ввода id станций для редактирования: ";
				choicePackage = GetCorrectNumber(0, 1);
				if (choicePackage == 0) {
					vector<int> searchVector;
					searchStancia(Stancia_group, searchVector);
					cout << "Отредактируйте выбранные станции." << endl;
					for (auto& id : searchVector) {
						Stancia_group[id].redactStancia();
					}
				}
				else if (choicePackage == 1) {
					unordered_set<int> ids;
					cout << "\nВведите количество редактируемых станций: ";
					int idsAmount = GetCorrectNumber(0, Stancia::MAX_STANCIA_ID);
					cout << "\nВведите id станций для редактирования: ";
					for (int i(0); i < idsAmount; i++) {
						int id = GetCorrectNumber(0, Stancia::MAX_STANCIA_ID - 1);
						if (Stancia_group.find(id) != Stancia_group.end())
							ids.insert(id);
						else {
							cout << "Станции с данным id не существует." << endl;
							i--;
						}
					}
					cout << "Отредактируйте выбранные станции." << endl;
					for (auto& id : ids) {
						Stancia_group[id].redactStancia();
					}
				}
			}
			break;
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
			string pathToFile;
			cout << "Введите название файла для загрузки: ";
			cin >> pathToFile;
			load(trubkaline, Stancia_group, pathToFile);
			break;
		}
		case 8:
		{
			vector<int> result;
			int number;
			cout << "Выберите объект для поиска (0 - если трубы, 1 - если компрессорные станции): ";
			number = GetCorrectNumber(0, 1);
			if (number == 0) {
				searchTrubkas(trubkaline, result);
				for (auto& trubka : result)
					cout << trubkaline[trubka] << endl << endl;
			}
			else if (number == 1) {
				searchStancia(Stancia_group, result);
				for (auto& stancia : result)
					cout << Stancia_group[stancia] << endl << endl;
			}
		}
		default:
			break;

			cout << endl << endl;
		}
	}
}


