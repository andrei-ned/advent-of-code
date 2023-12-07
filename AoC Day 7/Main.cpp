#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

enum HandType : int
{
	HighCard,
	OnePair,
	TwoPair,
	ThreeOfAKind,
	FullHouse,
	FourOfAKind,
	FiveOfAKind,
};

struct Hand
{
	string cards;
	int bet;
	HandType type;
};

unordered_map<char, int> cardScore =
{
	{ '2', 2 },
	{ '3', 3 },
	{ '4', 4 },
	{ '5', 5 },
	{ '6', 6 },
	{ '7', 7 },
	{ '8', 8 },
	{ '9', 9 },
	{ 'T', 10 },
	{ 'J', 11 },
	{ 'Q', 12 },
	{ 'K', 13 },
	{ 'A', 14 },
};

void ComputeHandType(Hand& hand)
{
	unordered_map<char, int> cardCount;
	for (int i = 0; i < hand.cards.length(); i++)
	{
		char card = hand.cards[i];
		cardCount[card]++;
	}
	for (auto& it : cardCount)
	{
		int count = it.second;
		if (count == 5)
		{
			hand.type = HandType::FiveOfAKind;
			return;
		}
		if (count == 4)
		{
			hand.type = HandType::FourOfAKind;
			return;
		}
		if (count == 3)
		{
			hand.type = cardCount.size() == 2 ? HandType::FullHouse : HandType::ThreeOfAKind;
			return;
		}
		if (count == 2)
		{
			switch (cardCount.size())
			{
			case 2:
				hand.type = HandType::FullHouse;
				return;
			case 3:
				hand.type = HandType::TwoPair;
				return;
			case 4:
				hand.type = HandType::OnePair;
				return;
			}
		}
	}
	hand.type = HandType::HighCard;
	return;
}

bool SortHands(Hand hand1, Hand hand2)
{
	if (hand1.type == hand2.type)
	{
		for (int i = 0; i < hand1.cards.length(); i++)
		{
			int card1Score = cardScore[hand1.cards[i]];
			int card2Score = cardScore[hand2.cards[i]];
			if (card1Score != card2Score)
			{
				return card1Score > card2Score;
			}
		}
	}
	return hand1.type > hand2.type;
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	int currentBet;

	vector<Hand> hands;

	// read input
	while (fin >> currentIn >> currentBet)
	{
		Hand currentHand;
		currentHand.cards = currentIn;
		currentHand.bet = currentBet;
		hands.push_back(currentHand);
	}
	fin.close();

	// figure out the type
	for (int i = 0; i < hands.size(); i++)
	{
		ComputeHandType(hands[i]);
	}

	// sort the cards
	sort(hands.begin(), hands.end(), SortHands);

	// compute total winnings (part 1 answer)
	int totalWinnings = 0;
	for (int i = 0; i < hands.size(); i++)
	{
		int rank = hands.size() - i;
		totalWinnings += hands[i].bet * rank;
	}
	cout << "Total winnings= " << totalWinnings << "\n";
}