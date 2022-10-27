#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <Windows.h>

using namespace std;

int RandomNumber(int minInclusive, int maxInclusive)
{
    return (rand() % (maxInclusive - minInclusive) + minInclusive);
}

#pragma region classes
class NPC;
class Player;

class NPC
{
protected:
    int HP;
    string weapon;
    string name;
    bool isNpc;

public:
    NPC()
    {
        this->HP = 20;
        this->isNpc = true;
    }

    NPC(string const &_weapon, string const &_name)
    {
        this->HP = 20;
        this->weapon = _weapon;
        this->name = _name;
        this->isNpc = true;
    }

    void Attack(NPC *target)
    {
        int damage = RandomNumber(1, 20);
        *target->getHP() -= damage;
        std::cout << this->getName() << " zaatakował " << target->getName() << " za pomocą " << this->getWeapon() << " zadając " << damage << " punktów obrażeń. ";
        if (*target->getHP() <= 0)
            std::cout << target->getName() << " umarł(-a).";
        else
            std::cout << endl
                      << target->getName() << " ma teraz " << *target->getHP() << " HP.";
        std::cout << endl;
    }

    void Attack(Player *target);

    bool getIsNpc()
    {
        return this->isNpc;
    }

    int *getHP()
    {
        return &this->HP;
    }

    string getWeapon()
    {
        return this->weapon;
    }

    string getName()
    {
        return this->name;
    }
};

class Player : public NPC
{
public:
    Player(string const &_weapon, string const &_name)
    {
        this->HP = 20;
        this->weapon = _weapon;
        this->name = _name;
        this->isNpc = false;
    }
};

void NPC::Attack(Player *target)
{
    *target->getHP() -= RandomNumber(1, 20);
}
#pragma endregion

void Setup()
{
    srand((unsigned)time(0));
}

int main()
{
    setlocale(LC_CTYPE, "Polish");
    Setup();
    vector<NPC *> players;

    // Tworzenie graczy
    {
        string name, wep;
        char bot;
        while (true)
        {
            std::cout << "Podaj nazwe gracza:" << endl;
            name, wep = "";
            cin >> name;
            if (name == "end")
                break;
            std::cout << "Z jakiej broni korzysta?" << endl;
            cin >> wep;
            do
            {
                std::cout << "Czy jest botem? [T/N] ";
                cin >> bot;
                bot = char(toupper(bot));
                if (bot != 'T' && bot != 'N')
                    std::cout << endl
                              << "Niepoprawna wartosc" << endl;
            } while (bot != 'T' && bot != 'N');
            std::cout << endl;

            if (bot == 'T')
                players.push_back(new NPC(wep, name));
            else
                players.push_back(new Player(wep, name));
        }
    }

    // Tasowanie wektora graczy
    {
        size_t n = players.size();
        for (int i = 0; i < n - 1; i++)
        {
            int j = i + rand() % (n - i);
            swap(players[i], players[j]);
        }
    }

    std::cout << "\033[2J\033[1;1H";
    // Walka
    while (players.size() > 1)
    {
        for (auto const &el : players)
        {
            if (*el->getHP() <= 0)
                continue;
            else if (players.size() <= 1)
                break;
            std::cout << endl
                      << "Obecna tura: " << el->getName() << endl;
            if (el->getIsNpc())
            {
                std::cout << el->getName() << " jest NPC" << endl;
                NPC *target;
                int index;
                do
                {
                    index = RandomNumber(0, players.size());
                    target = players[index];
                } while (target->getName() == el->getName());
                el->Attack(target);
                if (*target->getHP() <= 0)
                    players.erase(players.begin() + index);
                Sleep(3000);
            }
            else
            {
                std::cout << "Wybierz cel ataku:";
                for (size_t i = 0; i < players.size(); ++i)
                {
                    if (players[i]->getName() == el->getName())
                        continue;
                    std::cout << endl
                              << i << " " << players[i]->getName() << " - " << *players[i]->getHP();
                }
                std::cout << endl
                          << endl;
                int index;
                cin >> index;
                el->Attack(players[index]);
                if (*players[index]->getHP() <= 0)
                    players.erase(players.begin() + index);
                Sleep(1500);
            }
        }
    }
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Zwyciezca tej edycji Dice Royale jest " << players[0]->getName() << "! Gratulacje!";

    return 0;
}