#include <iostream>
#include <limits>
#include <array>
#include <vector>
#include <string>
#include "Random.h"
#include <string_view>

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool clearFailedExtraction()
{
	if (!std::cin)
	{
		if (std::cin.eof())
		{
			std::exit(0);
		}

		std::cin.clear();
		return true;
	}
	return false;
}

bool hasUnextractedInput()
{
	return((!std::cin.eof())) && (std::cin.peek() != '\n');
}

namespace Potion
{
	enum Type : std::size_t
	{
		healing,
		mana,
		speed,
		invisibility,

		max_potions,
	};

	constexpr std::array<std::size_t, max_potions> type {healing, mana, speed, invisibility};
	constexpr std::array<int, max_potions> potionCost{ {20, 30, 12, 50} };
	constexpr std::array<std::string_view, max_potions> potionName{{"healing", "mana", "speed", "invisibility"}};

	void shop()
	{
		std::cout << "\nHere is our selection for today: \n";
		for (std::size_t i{ 0 }; i < max_potions; i++)
		{
			std::cout << i << ") " << potionName[i] << " potion costs " << potionCost[i] << '\n';
		}
		std::cout << "Choose a potion you'd like to buy(0-3), or leave the shop(-1): ";
	}
}

std::ostream& operator<<(std::ostream& out, Potion::Type p)
{
	std::cout << Potion::potionName[p];
	return out;
}

class Player
{
private:
	std::string m_playerName{getUserName()};
	std::array < int, Potion::max_potions> m_playerInventory{};
	int m_playerGold{Random::get(100,120)};
public:
	Player()
	{
	}
	int getGold() const { return m_playerGold; }
	std::string_view const getPlayerName() const { return m_playerName; }
	void updateGold(int x) { m_playerGold -= Potion::potionCost[x]; }
	void updatePlayerInventory(std::size_t x) { ++(m_playerInventory[x]); }
	auto& getPlayerPotionCount(std::size_t x) const { return m_playerInventory[x]; }
	std::string getUserName()
	{
		std::string a{};
		std::cout << "What's your name: ";
		while (true)
		{
			std::cin >> a;
			if (clearFailedExtraction() || hasUnextractedInput())
			{
				ignoreLine();
				std::cout << "\nStop giving me fake name. Tell me your real name: ";
				continue;
			}
			return a;
		}
	}
};

bool buyingPotion(Player& p)
{
	while (true)
	{
		int input{};
		std::cin >> input;
		if (input == -1)
		{
			return true;
		}
		if (clearFailedExtraction() || hasUnextractedInput() || input > 3 || input < 0)
		{
			ignoreLine();
			std::cout << "\nWe don't have that potion in stock. Please pick the ones available: ";
			continue;
		}
		if (Potion::potionCost[input] > p.getGold())
		{
			ignoreLine();
			std::cout << "\nBroke alert!!! Get your money up not your funny up.\n";
			std::cout << "Pick a cheaper potion peasant or leave (-1): ";
			continue;
		}
		p.updateGold(static_cast<int>(input));
		p.updatePlayerInventory(static_cast<std::size_t>(input));
		std::cout << "You purchases a potion of " << static_cast<Potion::Type>(input) << ". You have " << p.getGold() << " gold left.\n";
		return false;
	}
}

int main()
{
	std::cout << "Welcome to Borpa's potion shop!\n";
	Player p{};
	int x{ p.getGold() };
	std::cout << "Hello, " << p.getPlayerName() << ", you have " << p.getGold() << " gold.\n";
	bool poor{ false };
	while (!poor)
	{

		Potion::shop();
		poor = buyingPotion(p);
	}
	if (x == p.getGold())
	{
		std::cout << "\nStop loitering around my shop!!! Leave before I call the guards.\n\n";
		std::cout << "You leave with " << p.getGold() << " gold left.\n";
		return 0;
	}
	std::cout << "\nYour inventory contains: \n";
	for (std::size_t i : Potion::type)
	{
		if (p.getPlayerPotionCount(i) > 0)
			std::cout << p.getPlayerPotionCount(i) << "x potion of " << static_cast<Potion::Type>(i) << '\n';
	}

	return 0;
}