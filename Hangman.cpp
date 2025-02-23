#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <set>

using namespace std;

string decrypt(const string& encrypted, int shift) {
    string decrypted = encrypted;
    for (char& c : decrypted) {
        if (isalpha(c)) {
            c = (c - shift - 'a' + 26) % 26 + 'a';
        }
    }
    return decrypted;
}

vector<string> loadWords(const string& filename, int shift) {
    ifstream file(filename);
    vector<string> words;
    string encryptedWord;
    while (file >> encryptedWord) {
        words.push_back(decrypt(encryptedWord, shift));
    }
    return words;
}

void displayStats(time_t startTime, int attempts, const string& word, const string& guessed, const string& incorrectGuesses) {
    time_t endTime = time(nullptr);
    cout << "Игра закончена!\n";
    cout << "Время: " << difftime(endTime, startTime) << " секунд\n";
    cout << "Число попыток: " << attempts << "\n";
    cout << "Загаданное слово: " << word << "\n";
    cout << "Буквы игрока: " << guessed << " " << incorrectGuesses << "\n";
}

bool playAgain() {
    char choice;
    cout << "Хотите сыграть снова? (y/n): ";
    cin >> choice;
    return (choice == 'y' || choice == 'Y');
}

// Функция для выбора уровня сложности
string chooseDifficulty() {
    cout << "Выберите уровень сложности:\n"
        << "1. Легкий\n"
        << "2. Нормальный\n"
        << "3. Сложный\n"
        << "Введите номер уровня: ";
    int level;
    cin >> level;

    switch (level) {
    case 1:
        return "easy.txt";
    case 2:
        return "normal.txt";
    case 3:
        return "hard.txt";
    default:
        cout << "Некорректный выбор. По умолчанию выбран Легкий уровень.\n";
        return "easy.txt";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    // При желании можно изменить сдвиг шифра
    const int shift = 3; // Сдвиг шифра Цезаря

    do {
        // Определяем, какой файл загрузить в зависимости от выбранной сложности
        string filename = chooseDifficulty();

        // Загружаем слова из соответствующего файла
        vector<string> words = loadWords(filename, shift);

        srand(static_cast<unsigned>(time(nullptr)));

        // Выбираем случайное слово
        string word = words[rand() % words.size()];
        string guessed(word.length(), '_');
        string incorrectGuesses;
        int attempts = 0;
        time_t startTime = time(nullptr);

        // Множество для всех уже введенных букв
        set<char> usedLetters;

        // Цикл игры
        while (guessed != word && incorrectGuesses.length() < 6) {
            cout << "Текущее слово: " << guessed << "\n";
            cout << "Неправильные предположения: " << incorrectGuesses << "\n";
            cout << "Угадайте букву: ";

            char guess;
            cin >> guess;

            // Проверяем, вводилась ли эта буква ранее
            if (usedLetters.find(guess) != usedLetters.end()) {
                cout << "Буква " << guess << " уже использовалась!\n";
                continue; // Не считаем повторную букву за попытку
            }

            // Запоминаем эту букву среди использованных
            usedLetters.insert(guess);

            // Ищем букву в слове
            if (word.find(guess) != string::npos) {
                // Открываем все вхождения
                for (size_t i = 0; i < word.length(); ++i) {
                    if (word[i] == guess) {
                        guessed[i] = guess;
                    }
                }
            }
            else {
                incorrectGuesses += guess;
            }
            attempts++;
        }

        // Подведение итогов
        displayStats(startTime, attempts, word, guessed, incorrectGuesses);
    } while (playAgain());

    return 0;
}
