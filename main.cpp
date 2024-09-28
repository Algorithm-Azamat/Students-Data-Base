#include <iostream>
#include <sqlite3.h>

using namespace std;

// Функция для создания таблицы студентов, если она еще не существует
void createTable(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS Students ("
                      "student_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "first_name TEXT NOT NULL, "
                      "last_name TEXT NOT NULL, "
                      "age INTEGER, "
                      "major TEXT, "
                      "gpa REAL);";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Ошибка создания таблицы: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Таблица успешно создана или уже существует." << endl;
    }
}

// Функция для добавления студента в базу данных
void addStudent(sqlite3 *db) {
    string firstName, lastName, major;
    int age;
    double gpa;

    cout << "Введите имя: ";
    cin >> firstName;
    cout << "Введите фамилию: ";
    cin >> lastName;
    cout << "Введите возраст: ";
    cin >> age;
    cout << "Введите специальность: ";
    cin >> major;
    cout << "Введите средний балл (GPA): ";
    cin >> gpa;

    string sql = "INSERT INTO Students (first_name, last_name, age, major, gpa) VALUES ('" + firstName + "', '" + lastName + "', " + to_string(age) + ", '" + major + "', " + to_string(gpa) + ");";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Ошибка добавления студента: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Студент успешно добавлен!" << endl;
    }
}

// Функция для отображения всех студентов
void viewStudents(sqlite3 *db) {
    const char *sql = "SELECT * FROM Students;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Ошибка выполнения запроса: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *firstName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *lastName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int age = sqlite3_column_int(stmt, 3);
        const char *major = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        double gpa = sqlite3_column_double(stmt, 5);

        cout << "ID: " << id << ", Имя: " << firstName << ", Фамилия: " << lastName << ", Возраст: " << age
             << ", Специальность: " << major << ", GPA: " << gpa << endl;
    }

    sqlite3_finalize(stmt);
}

// Функция для обновления данных студента
void updateStudent(sqlite3 *db) {
    int studentId;
    string firstName, lastName, major;
    int age;
    double gpa;

    cout << "Введите ID студента для обновления: ";
    cin >> studentId;
    cout << "Введите новое имя: ";
    cin >> firstName;
    cout << "Введите новую фамилию: ";
    cin >> lastName;
    cout << "Введите новый возраст: ";
    cin >> age;
    cout << "Введите новую специальность: ";
    cin >> major;
    cout << "Введите новый GPA: ";
    cin >> gpa;

    string sql = "UPDATE Students SET first_name = '" + firstName + "', last_name = '" + lastName + "', age = " + to_string(age) + ", major = '" + major + "', gpa = " + to_string(gpa) + " WHERE student_id = " + to_string(studentId) + ";";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Ошибка обновления данных студента: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Данные студента успешно обновлены!" << endl;
    }
}

// Функция для удаления студента
void deleteStudent(sqlite3 *db) {
    int studentId;

    cout << "Введите ID студента для удаления: ";
    cin >> studentId;

    string sql = "DELETE FROM Students WHERE student_id = " + to_string(studentId) + ";";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Ошибка удаления студента: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Студент успешно удалён!" << endl;
    }
}

// Основное меню программы
void mainMenu(sqlite3 *db) {
    while (true) {
        cout << "\nМеню:" << endl;
        cout << "1. Добавить студента" << endl;
        cout << "2. Показать всех студентов" << endl;
        cout << "3. Обновить данные студента" << endl;
        cout << "4. Удалить студента" << endl;
        cout << "5. Выход" << endl;

        int choice;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent(db);
                break;
            case 2:
                viewStudents(db);
                break;
            case 3:
                updateStudent(db);
                break;
            case 4:
                deleteStudent(db);
                break;
            case 5:
                cout << "Выход из программы." << endl;
                return;
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
        }
    }
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("students.db", &db);

    if (rc) {
        cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    createTable(db);  // Создаем таблицу студентов, если её нет

    mainMenu(db);  // Запуск основного меню программы

    sqlite3_close(db);  // Закрытие базы данных
    return 0;
}
