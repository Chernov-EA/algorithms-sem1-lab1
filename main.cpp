#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>
#include <cmath>

using namespace std;

constexpr char SAVE_FILE[] = "save.dat";

void saveGame(int year, int people, int land, int grain, float totalStarved) {
    ofstream fout(SAVE_FILE);
    if (fout) {
        fout << year << " " << people << " " << land << " "
             << grain << " " << totalStarved;
    }
}

bool loadGame(int &year, int &people, int &land, int &grain, float &totalStarved) {
    ifstream fin(SAVE_FILE);
    if (!fin) return false;
    fin >> year >> people >> land >> grain >> totalStarved;
    return true;
}

void clearSave() {
    remove(SAVE_FILE);
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    srand(time(nullptr));

    int year = 1;
    int people = 100;
    int land = 1000;
    int grain = 2800;

    float totalStarved = 0;

    ifstream test(SAVE_FILE);
    if (test) {
        char choice;
        cout << "Найдено сохранение. Продолжить игру? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            loadGame(year, people, land, grain, totalStarved);
        } else {
            clearSave();
        }
    }

    while (year <= 10) {
        cout << "\n=============================\n";
        cout << "Год " << year << " правления\n";
        cout << "Население: " << people << endl;
        cout << "Земля: " << land << " акров\n";
        cout << "Пшеница: " << grain << " бушелей\n";

        cout << "Выйти и сохранить игру? (y/n): ";
        char exitChoice;
        cin >> exitChoice;
        if (exitChoice == 'y' || exitChoice == 'Y') {
            saveGame(year, people, land, grain, totalStarved);
            cout << "Игра сохранена.\n";
            return 0;
        }

        int landPrice = rand() % 10 + 17;
        cout << "Цена земли: " << landPrice << " бушелей за акр\n";

        int buy = 0, sell = 0, feed = 0, plant = 0;

        while (true) {
            cout << "Сколько акров купить? ";
            cin >> buy;
            if (buy >= 0 && buy * landPrice <= grain) break;
            cout << "Ошибка ввода!\n";
        }

        while (true) {
            cout << "Сколько акров продать? ";
            cin >> sell;
            if (sell >= 0 && sell <= land) break;
            cout << "Ошибка ввода!\n";
        }

        const int difference = buy - sell;
        land += difference;
        grain -= difference * landPrice;

        while (true) {
            cout << "Сколько пшеницы пустить на еду? ";
            cin >> feed;
            if (feed >= 0 && feed <= grain) break;
            cout << "Ошибка ввода!\n";
        }

        grain -= feed;

        while (true) {
            cout << "Сколько акров засеять? ";
            cin >> plant;
            if (plant >= 0 &&
                plant <= land &&
                plant <= people * 10 &&
                plant * 0.5 <= grain)
                break;
            cout << "Ошибка ввода!\n";
        }

        grain -= static_cast<int>(plant * 0.5);

        int canFeed = feed / 20;
        int starved = 0;
        if (canFeed < people) {
            starved = people - canFeed;
            people = canFeed;
        }

        if (starved > 0) {
            cout << starved << " человек умерли от голода.\n";
            totalStarved += static_cast<float>(starved) / (people + starved);
        }

        if (starved > 0.45 * (people + starved)) {
            cout << "Более 45% населения умерло от голода. Вы проиграли!\n";
            clearSave();
            return 0;
        }

        int yield = rand() % 6 + 1;
        int harvest = plant * yield;
        grain += harvest;

        cout << "Собрано " << harvest << " бушелей (" << yield << " с акра)\n";

        int rats = static_cast<int>(rand() / RAND_MAX * 0.07 * grain);
        grain -= rats;
        if (rats > 0)
            cout << "Крысы съели " << rats << " бушелей.\n";

        int newcomers = starved / 2 + (5 - yield) * grain / 600 + 1;
        if (newcomers < 0) newcomers = 0;
        if (newcomers > 50) newcomers = 50;
        people += newcomers;

        if (newcomers > 0)
            cout << newcomers << " человек прибыли в город.\n";

        if (rand() % 100 < 15) {
            people = floor(people / 2.0);
            cout << "Чума уничтожила половину населения!\n";
        }

        year++;
    }

    clearSave();

    float P = totalStarved * 10;
    int L = land / people;

    cout << "\n=============================\n";
    cout << "Итоги правления:\n";
    cout << "Средний процент умерших от голода: " << P << "%\n";
    cout << "Акров на человека: " << L << endl;

    if (P > 33 && L < 7)
        cout << "«Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города."
                " Теперь вы вынуждены влачить жалкое существование в изгнании.\n";
    else if (P > 10 && L < 9)
        cout << "«Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением,"
                " и никто больше не желает видеть вас правителем.\n";
    else if (P > 3 && L < 10)
        cout << "«Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели,"
                " но многие хотели бы увидеть вас во главе города снова.\n";
    else
        cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше!\n";

    return 0;
}
