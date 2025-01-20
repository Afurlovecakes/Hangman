#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>

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
    return choice == 'y' || choice == 'Y';
}

int main() {
    setlocale(LC_ALL, "Russian");

    const string filename = "words.txt";
    const int shift = 3; // Сдвиг шифра Цезаря

    do {
        vector<string> words = loadWords(filename, shift);
        srand(time(nullptr));
        string word = words[rand() % words.size()];
        string guessed(word.length(), '_');
        string incorrectGuesses;

        int attempts = 0;
        time_t startTime = time(nullptr);

        while (guessed != word && incorrectGuesses.length() < 6) {
            cout << "Текущее слово: " << guessed << "\n";
            cout << "Неправильные предположения: " << incorrectGuesses << "\n";
            cout << "Угадайте букву: ";

            char guess;
            cin >> guess;

            if (word.find(guess) != string::npos) {
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

        displayStats(startTime, attempts, word, guessed, incorrectGuesses);
    } while (playAgain());

    return 0;
}
