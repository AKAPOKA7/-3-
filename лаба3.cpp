
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
struct Dish {
    std::string name;
    std::string type;
    double price;
};

    const std::string menuFilePath = "menu.txt";
    const std::string outputFilePath = "output.txt";

// Функция для вывода результатов в файл
void WriteToOutputFile(const std::string& outputFilePath, const std::string& content) {
    // Создаем объект ofstream и открываем файл с путем outputFilePath для добавления содержимого (std::ios::app)
    // Если файл не существует, он будет создан.
    std::ofstream output(outputFilePath, std::ios::app);
    
    // Записываем содержимое, переданное в функцию, в файл.
    // Оператор << используется для записи данных, а std::endl добавляет символ новой строки в конец.
    output << content << std::endl;
    
    // Закрываем файл после завершения записи.
    // Это важно для освобождения ресурсов и корректного сохранения данных в файл.
    output.close();
}

// Функция для добавления блюда в файл
void AddDish(const std::string& filePath) {
    // Создаем и открываем файловый поток для записи, файл указан путем filePath.
    // Флаг std::ios::app указывает, что запись будет производиться в конец файла,
    // то есть новые данные не затрут старые, а добавятся после них.
    std::ofstream file(filePath, std::ios::app);
    
    Dish dish; // Создаем объект структуры Dish для хранения информации о блюде.
    
    // Вывод запроса на ввод названия блюда в консоль и чтение введенного значения.
    std::cout << "Введите название блюда (оставьте пустым для завершения): ";
    getline(std::cin, dish.name); // Чтение строки до перевода строки.
    
    // Если введена пустая строка, функция завершает работу (не добавляем блюдо).
    if(dish.name.empty()) return;

    // Вывод запроса на ввод типа блюда и чтение введенного значения.
    std::cout << "Введите тип блюда: ";
    getline(std::cin, dish.type);
    
    // Вывод запроса на ввод цены блюда и чтение введенного значения.
    std::cout << "Введите цену блюда: ";
    std::cin >> dish.price;
    
    // Очищаем поток ввода, чтобы удалить лишние символы, такие как символ новой строки,
    // который остается в потоке после вызова std::cin >> dish.price.
    std::cin.ignore();

    // Запись данных блюда в файл в формате "название;тип;цена".
    file << dish.name << ";" << dish.type << ";" << dish.price << std::endl;

    // Вызов функции WriteToOutputFile для записи информации о добавленном блюде в другой файл.
    // Нужно передать корректный путь к файлу вывода, переменная outputFilePath должна быть определена вне этого фрагмента.
    WriteToOutputFile(outputFilePath, "Добавлено блюдо: " + dish.name + ", " + dish.type + ", " + std::to_string(dish.price));
    
    // Закрытие файла после завершения записи для сохранения изменений и освобождения ресурсов.
    file.close();
}

// Функция поиска блюда по названию
void SearchDish(const std::string& filePath) {
    // Открываем файловый поток для чтения из файла, путь к которому передан в filePath.
    std::ifstream file(filePath);

    // Флаг для отслеживания, найдено ли блюдо в файле.
    bool found = false;

    // Запрашиваем у пользователя название блюда для поиска и считываем в переменную name.
    std::string name;
    std::cout << "Введите название блюда для поиска: ";
    getline(std::cin, name);

    // Переменная для хранения текущей считываемой строки из файла.
    std::string line;
    // Чтение файла построчно.
    while(getline(file, line)) {
        // Используем поток строк для разбора каждой строки, разделенной точкой с запятой.
        std::istringstream iss(line);
        Dish dish; // Создаем временный объект для хранения данных о блюде.
        
        // Считываем имя блюда, тип и цену из строки.
        getline(iss, dish.name, ';');
        getline(iss, dish.type, ';');
        iss >> dish.price;

        // Если название блюда совпадает с искомым, выводим информацию о блюде и записываем в файл.
        if(dish.name == name) {
            std::cout << "Блюдо найдено: " << dish.name << ", " << dish.type << ", " << dish.price << std::endl;
            // Передаем информацию о найденном блюде в функцию записи в файл вывода.
            // Переменная outputFilePath должна быть определена вне этого фрагмента.
            WriteToOutputFile(outputFilePath, "Найдено блюдо: " + dish.name + ", " + dish.type + ", " + std::to_string(dish.price));
            found = true;
            // Прерываем функцию, поскольку блюдо найдено.
            return;
        }
    }

    // Если блюдо не найдено, информируем пользователя и записываем это в файл вывода.
    if (!found) {
        std::cout << "Блюдо не найдено." << std::endl;
        // Записываем информацию о неудачном поиске в файл вывода.
        WriteToOutputFile(outputFilePath, "Блюдо не найдено: " + name);
    }

    // Закрываем файл после завершения операций чтения.
    file.close();
}

// Функция для сортировки списка блюд, хранящихся в файле.
void SortDishes(const std::string& filePath) {
    std::vector<Dish> dishes; // Вектор для хранения блюд.
    std::ifstream file(filePath); // Открытие файла для чтения.
    std::string line; // Строка для хранения прочитанных данных.

    // Загрузка данных о блюдах из файла в вектор dishes.
    while(getline(file, line)) {
        std::istringstream iss(line); // Поток для разбора строки.
        Dish dish; // Временный объект для хранения данных о блюде.
        getline(iss, dish.name, ';'); // Чтение названия блюда.
        getline(iss, dish.type, ';'); // Чтение типа блюда.
        iss >> dish.price; // Чтение цены блюда.
        dishes.push_back(dish); // Добавление блюда в вектор.
    }
    file.close(); // Закрытие файла после чтения данных.

    int choice; // Переменная для хранения выбора пользователя.
    // Запрос у пользователя критерия сортировки.
    std::cout << "Сортировать по (1) типу или (2) цене? ";
    std::cin >> choice;
    std::cin.ignore(); // Очистка потока ввода после чтения числа.

    // Сортировка вектора блюд в зависимости от выбора пользователя.
    if (choice == 1) {
        // Сортировка по типу блюда.
        std::sort(dishes.begin(), dishes.end(), [](const Dish& a, const Dish& b) {
            return a.type < b.type;
        });
    } else if (choice == 2) {
        // Сортировка по цене блюда.
        std::sort(dishes.begin(), dishes.end(), [](const Dish& a, const Dish& b) {
            return a.price < b.price;
        });
    }

    // Перезапись файла с отсортированным списком блюд.
    std::ofstream outfile(filePath); // Открытие файла для записи.
    for(const auto& dish : dishes) {
        // Запись информации о каждом блюде в файл.
        outfile << dish.name << ";" << dish.type << ";" << dish.price << std::endl;
    }

    // Запись в файл вывода информации о типе совершенной сортировки.
    if (choice == 1) {
        WriteToOutputFile(outputFilePath, "Сортировка блюд по типу:\n");
    } else if (choice == 2) {
        WriteToOutputFile(outputFilePath, "Сортировка блюд по цене:\n");
    }
    
    outfile.close(); // Закрытие файла после завершения записи.
}


void OutputDishesByPrice(const std::string& filePath, const std::string& outputFilePath) {
    std::ifstream file(filePath); // Открытие файла для чтения.
    // Открытие файла для записи результатов с добавлением к существующим (не перезаписывая файл).
    std::ofstream outputFile(outputFilePath, std::ios::app);

    double maxPrice; // Переменная для хранения максимальной цены.
    // Запрос у пользователя максимальной цены и чтение в переменную maxPrice.
    std::cout << "Введите максимальную цену: ";
    std::cin >> maxPrice;
    std::cin.ignore(); // Очистка входного буфера потока ввода.

    std::string line; // Переменная для хранения считываемой строки из файла.
    // Построчное чтение файла.
    while(getline(file, line)) {
        std::istringstream iss(line); // Поток для разбора строки файла.
        Dish dish; // Временный объект для хранения данных о блюде.
        getline(iss, dish.name, ';'); // Чтение названия блюда.
        getline(iss, dish.type, ';'); // Чтение типа блюда.
        iss >> dish.price; // Чтение цены блюда.

        // Если цена блюда меньше или равна заданной пользователем максимальной цене...
        if(dish.price <= maxPrice) {
            // ...выводим информацию о блюде на экран...
            std::cout << dish.name << ", " << dish.type << ", " << dish.price << std::endl;
            // ...и записываем её в выходной файл.
            outputFile << dish.name << ", " << dish.type << ", " << dish.price << std::endl;
        }
    }

    // Записываем в выходной файл информацию о выполненной операции.
    WriteToOutputFile(outputFilePath, "Вывод блюд ценой ниже заданной");

    file.close(); // Закрываем файл после чтения.
    outputFile.close(); // Закрываем выходной файл после записи.
}

// Функция для считывания всех блюд из файла
vector<Dish> ReadDishesFromFile(const string& filePath) {
    ifstream file(filePath); // Создаем объект ifstream для чтения из файла.
    vector<Dish> dishes; // Вектор для хранения блюд.
    string line; // Строка для чтения содержимого файла.

    // Читаем файл построчно до конца файла.
    while (getline(file, line)) {
        stringstream iss(line); // Используем stringstream для разбора строки.
        Dish dish; // Временный объект для хранения информации о блюде.
        getline(iss, dish.name, ';'); // Получаем название блюда до разделителя ';'.
        getline(iss, dish.type, ';'); // Получаем тип блюда до следующего разделителя ';'.
        iss >> dish.price; // Считываем цену блюда.
        dishes.push_back(dish); // Добавляем объект dish в конец вектора dishes.
    }

    file.close(); // Закрываем файл после завершения чтения.
    return dishes; // Возвращаем вектор с данными о блюдах.
}

// Функция для вывода названий уже добавленных блюд
void PrintDishNames(const vector<Dish>& dishes) {
    // Перебираем все элементы вектора блюд
    for (const Dish& dish : dishes) {
        cout << dish.name << endl; // Выводим название каждого блюда на новой строке
    }
}

int main() {

    int choice;

    do {
        std::cout << "1. Добавить блюдо\n";
        std::cout << "2. Найти блюдо\n";
        std::cout << "3. Сортировать блюда\n";
        std::cout << "4. Вывести блюда по максимальной цене\n";
        std::cout << "5. Вывести все блюда\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите опцию: ";
        std::cin >> choice;
        std::cin.ignore(); // очистить поток ввода

        switch (choice) {
            case 1:
                AddDish(menuFilePath);
                break;
            case 2:
                SearchDish(menuFilePath);
                break;
            case 3:
                SortDishes(menuFilePath);
                break;
            case 4:
                OutputDishesByPrice(menuFilePath, outputFilePath);
                break;
            case 5: { 
                vector<Dish> dishes = ReadDishesFromFile(menuFilePath);
                cout << "Названия блюд:" << endl;
                PrintDishNames(dishes);
                break;
            }
            case 0:
                std::cout << "Выход из программы..." << std::endl;
                break;
            default:
                std::cout << "Некорректный ввод. Пожалуйста, попробуйте еще раз." << std::endl;
        }
    } while (choice != 0);

    return 0;
}
