#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstdlib> // Для rand() и srand()
#include <ctime>   // Для time
#include <iomanip>

using namespace std;

void rus() { setlocale(LC_ALL, "Russian"); }


class Pet {
public:
    int food;
    int water;
    int coins;
    int hungerRate;
    int luck;
    int sugar;

    Pet() : food(100), water(100), coins(1110), hungerRate(30), luck(45), sugar(20) {}

    void feed() {
        if (coins >= 20) {
            coins -= 20;
            coins -= 20;
            food += 10;
            cout << "Сытость = " << food << "\n"
                << "Баланс = " << coins << endl;
        }
        else {
            displayPoorMessage();
        }
    }


    void drink() {
        if (coins >= 10) {
            coins -= 10;
            water += 10;
            cout << "Насыщение = " << water << "\n"
                << "Баланс = " << coins << endl;
        }
        else {
            displayPoorMessage();
        }
    }

    void walk() {
        int randomEvent = rand() % 100;

        if (randomEvent < luck) { 
            findMoney();
        }
        else if (randomEvent < luck + 20) {

            cout << "Гуляя по парку, Пупизоид нашёл на земле шоколадку и съел. Сахар - " << sugar << endl;
            sugar += 20;
        }
        else {
            cout << "Вы вышли на прогулку по городу. По дороге Пупизоид сильно проголодался." << endl;
            food -= hungerRate;
            cout << "Сытость = " << food << endl;
        }
    }

    void findMoney() {
        coins += 100;
        cout << "Гуляя по парку, вы нашли на земле 100 денег! Ура!" << "\n"
            << "Ваш баланс = " << coins << endl;
    }

    void displayStats() {
        cout << "Статистика:" << endl;
        cout << "Еда = " << food << "\n"
            << "Вода = " << water << "\n"
            << "Деньги = " << coins << "\n"
            << "Сахар = " << sugar << "\n"
            << "Удача = " << luck << endl;
    }

    void upgradeZheludok() {
        if (coins >= 1000) {
            hungerRate -= 5;
            coins -= 1000;
            cout << "Желудок Пупизоида налился Железом! Теперь ваш Пупизоид будет голодать меньше! Ура!" << endl;
        }
        else {
            displayPoorMessage();
        }
    }

    void upgradeLuck() {
        if (coins >= 500) {
            luck += 5;
            coins -= 500;
            cout << "Клевер теперь всегда с вами! Ваша удача увеличилась на 5. Ура!" << endl;
        }
        else {
            displayPoorMessage();
        }
    }

    void podrochitel() {
        if (coins >= 200) {
            sugar -= 10;
            coins -= 200;
            cout << "Инсулин сделал своё дело! Сахар Пупизоида уменьшился на 10. Ура!" << endl;
        }
        else {
            displayPoorMessage();
        }
    }

    void Gama() {
        srand(static_cast<unsigned int>(time(0)));

        int score = 0;
        int numQuestions = 3;
        char operation;

        for (int i = 0; i < numQuestions; ++i) {
            int a = rand() % 100;
            int b = rand() % 100;

            int op = rand() % 3;
            if (op == 0) {
                operation = '+';
            }
            else if (op == 1) {
                operation = '-';
            }
            else {
                operation = '*';
            }

            int correctAnswer;
            if (operation == '+') {
                correctAnswer = a + b;
            }
            else if (operation == '-') {
                correctAnswer = a - b;
            }
            else {
                correctAnswer = a * b;
            }

            cout << "Вопрос " << (i + 1) << " ->" << a << " " << operation << " " << b << " = ?" << endl;
            auto startTime = chrono::steady_clock::now();
            int userAnswer;
            bool answered = false;

            thread answerThread([&userAnswer, &answered]() {
                cin >> userAnswer;
                answered = true;
                });

            while (!answered) {
                auto currentTime = chrono::steady_clock::now();
                chrono::duration<double> elapsed = currentTime - startTime;
                if (elapsed.count() > 10.0) {
                    cout << "Долго думаешь! Время вышло." << endl;
                    break;
                }
            }

            if (answered) {
                answerThread.join();
                if (userAnswer == correctAnswer) {
                    cout << "Правильно!" << endl;
                    cout << "Ваш счёт пополнен на 10 денег" << endl;
                    score++;
                    coins += 10;
                }
                else {
                    cout << "Неправильно. Правильный ответ: " << correctAnswer << endl;
                }
            }
            else {
                answerThread.detach();
            }
        }
    }
private:
    void displayPoorMessage() {
        cout << "Ты слишком нищий! У тебя всего " << coins << " денег!" << endl;
    }
};

class Game {
public:
    Game() : running(true) {
        pet = new Pet();
    }

    ~Game() {
        delete pet;
    }

    void endGame() {
        running = false;
        cout << "Игра окончена!" << endl;
    }

    void start() {
        thread eventThread(&Game::randomEvent, this);
        cout << "Привет! Я - Пупизоид, твой новый питомец. Меня надо кормить и поить. Пока что всё, я бесполезен." << endl;
        showBase();

        while (running) {
            int choice;
            cin >> choice;
            handleUserChoice(choice);
        }
        eventThread.join();
    }

private:
    Pet* pet;
    volatile bool running;

    void showBase() {
        cout << "Покормить меня - 1" << "\n"
            << "Напоить меня - 2" << "\n"
            << "Выйти на прогулку - 3" << "\n"
            << "Сходить в магазин - 4" << "\n"
            << "Сыграть в игру - 5" << "\n"
            << "Посмотреть статистику - 6" << endl;
    }

    void handleUserChoice(int choice) {
        switch (choice) {
        case 1:
            pet->feed();
            break;
        case 2:
            pet->drink();
            break;
        case 3:
            pet->walk();
            break;
        case 4:
            shop();
            break;
        case 5:
            pet->Gama();
            break;
        case 6:
            pet->displayStats();
            break;
        default:
            cout << "Че ты от меня хочешь? Не понимаю" << endl;
            showBase();
            break;
        }
        checkStatus();
    }

    void checkStatus() {
        if (pet->food > 100) {
            running = false;
            cout << "Ты перекормил Пупизоида. Игра окончена!" << endl;
        }
        if (pet->food <= 0) {
            running = false;
            cout << "Ты не покормил Пупизоида. Игра окончена!" << endl;
        }
        if (pet->water <= 0) {
            running = false;
            cout << "Ты не напоил Пупизоида. Игра окончена!" << endl;
        }\
            if (pet->water > 100) {
                running = false;
                cout << "Ты перепоил Пупизоида. Игра окончена!" << endl;
            }
        if (pet->sugar >= 100) {
            running = false;
            cout << "Пупизоид упал в обморок от повышенного сахара и больше не встал. Игра окончена!" << endl;
        }
        if (pet->sugar <= 0) {
            running = false;
            cout << "Пупизоид упал в обморок от повышенного сахара. Игра окончена!" << endl;
        }
    }

    void shop() {
        cout << "Вы пришли в магазин." << endl;
        bool inShop = true;
        while (inShop) {
            cout << "1 - Купить улучшение \"Стальной Желудок\" - 1000 денег" << "\n"
                << "2 - Купить улучшение \"Четырехлистный клевер\" - 500 денег" << "\n"
                << "3 - Купить препарат \"Ампула Инсулина\" - 200 денег" << "\n"
                << "4 - Вернуться домой" << endl;
            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                pet->upgradeZheludok();
                break;
            case 2:
                pet->upgradeLuck();
                break;
            case 3:
                pet->podrochitel();
                break;
            case 4:
                inShop = false;
                break;
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                break;
            }
        }
    }

    void randomEvent() {
        while (running) {
            int waitTime = rand() % 60 + 10;
            this_thread::sleep_for(chrono::seconds(waitTime));

            pet->food -= pet->hungerRate;

            cout << "Хозяин, я проголодался! Пора меня кормить! Мой уровень сытости - " << pet->food << "\n";

            if (pet->food <= 0) {

                running = false;
                cout << "Я взорвался от голода!!!!!" << endl;
            }
        }
    }
};

int main() {
    rus();
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.start();
    return 0;
}