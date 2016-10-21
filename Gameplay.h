#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include<iostream>
#include<string>
#include "Deck.h"
using namespace std;

class Gameplay {
public:
	int money;
	Deck deck;         //Card deck to draw
	Deck hand;         //Cards in hand
	Deck discarded;    //Cards discarded
	
	Gameplay () {
		money = 10;
		deck.init ();
	}

	//return 0 to exit;
	//return -1 to input again;
	//return 1 to play next round;
	//return 2 to go back to hand contains and options;
	int parseOption () {
		Position ptr;
		int optionFlag = 0;
		int returnFlag;

		int reserve[5] = { 0 };
		for (int i = 0; i < 5; i++)
			reserve[i] = 0;
				
		string choice;
		getline (cin, choice);
		
		for (size_t i = 0; i < choice.size (); i++) {

			if (choice[i] < 'a' || choice[i] > 'e') {
				optionFlag = 1;    //gotta set all the elements in reserve[] array to be 0 later

				if (choice.compare ("deck") == 0) {            //print out the deck
					returnFlag = 2;
					deck.printDeck ();					
				} else if (choice.compare ("none") == 0) {     //remove all the cards in hands
					returnFlag = 1;
					ptr = hand.getHead ();
					for (int i = 0; i < 5; i++) {
						draw (ptr);
						ptr = ptr->next;
					}
					cout << "You kept 0 and drew 5 cards. " << endl;
					handOutput ();
					judge ();		
				} else if (choice.compare ("all") == 0) {      //keep all the cards in hands
					returnFlag = 1;
					judge ();					
				} else if (choice.compare ("exit") == 0) {     //exit
					returnFlag = 0;				
				} else if (choice.compare ("swap") == 0) {     //swap selected cards
					returnFlag = 2;
					swap ();		
				} else {
					cout << "Invalid input. Please type your choice again:";
					returnFlag = -1;
				}

				break;

			} else {
				int j = choice[i] - 'a';              //parse the letters in input string to reserve
				reserve[j] = 1;
			}
		}

		if (optionFlag == 0) {			
			ptr = hand.getHead ();
			for (int i = 0; i < 5; i++) {
				if (reserve[i] == 0) {
					draw (ptr);
				}
				ptr = ptr->next;
			}
			cout << "Hand deck is: " << endl;
			handOutput ();
			cout << "Discarded deck is: " << endl;
			discarded.printDeck ();
			cout << "\nThe deck contains " << deck.deskCount << " cards.\n" << endl;

			returnFlag = 1;
			judge ();
		}

		return returnFlag;
	}

	void draw (Position ptr) {
		Position tmp;
		if (deck.countDeck () < 1) {
			ptr = deck.getHead ();
			deck.setHead (discarded.getHead ());
			discarded.setHead (ptr);
		}
		tmp = deck.drawCard ();
		string suit1 = ptr->suit;
		string rank1 = ptr->rank;
		ptr->suit = tmp->suit;
		ptr->rank = tmp->rank;
		deck.removeCard (tmp);
		cout << "discarded desk add card during draw: " << rank1 << " of " << suit1 << endl;
		discarded.addCard (rank1, suit1);
	}

	void swap () {
		Position ptr, tmp, tmp1, tmp2;

		cout << "Enter the letter of the card in hand(from a to e): " << endl;
		char ans;
		cin >> ans;
		cin.ignore (10000, '\n');
		while (ans < 'a' || ans > 'e') {
			cout << "Error: Out of boundary. Enter the letter of the card in hand(from a to e): " << endl;
			cin >> ans;
			cin.ignore (10000, '\n');
		}
		int offset = tolower (ans) - 'a';

		cout << "Enter the value of the card in the deck to swap with(integer from 1 to 14): " << endl;
		int num;
		cin >> num;
		cin.ignore (10000, '\n');

		while (cin.fail () || num > 14 || num < 1) {
			cout << "I'm sorry, I don't understand. Please try again(integer from 1 to 14): ";
			cin.clear ();
			cin.ignore (10000, '\n');
			cin >> num;
			cin.ignore (10000, '\n');
		}

		string rank1 = deck.rankToString (num);

		cout << "Enter the suit (c,d,h,s) of the card in the deck to swap with: " << endl;
		cin >> ans;
		cin.ignore (10000, '\n');
		while (cin.fail () || !(ans == 'c' || 'd' || 'h' || 's')) {
			cout << "Error: Wrong input. Enter the suit (c,d,h,s) of the card in the deck to swap with: " << endl;
			cin >> ans;
			cin.ignore (10000, '\n');
		}

		string suit1 = deck.suitToString (ans);

		ptr = hand.getHead ();
		for (int i = 0; i < offset; i++) {
			ptr = ptr->next;
		}
		discarded.addCard (ptr);
		tmp = deck.findNode (rank1, suit1);
		tmp1 = hand.findNode (rank1, suit1);
		tmp2 = discarded.findNode (rank1, suit1);

		if (tmp == nullptr || tmp1 != nullptr || tmp2 != nullptr) {
			cout << "Invalid deck card." << endl;
			return;
		} else {
			deck.removeCard (tmp);
			ptr->rank = rank1;
			ptr->suit = suit1;
		}
	}

	int judgeFlush () {
		Position ptr;
		string suit1;		
		ptr = hand.getHead ();			
		suit1 = ptr->suit;

		while (ptr->next != nullptr) {
			ptr = ptr->next;
			if (ptr->suit.compare(suit1) != 0)
				return 0;			
		}
		return 1;
	}
	
	int judgeThree () {
		Position ptr;
		ptr = hand.getHead ();
		int count = 0;

		while(ptr->next->next != nullptr){
			count = hand.findAllCount (ptr->rank);
			if (count >= 3 && count < 5)
				return 1;
			ptr = ptr->next;
		}
		return 0;
	}

	int judgeTwoPairs () {
		int first = 0;
		Position ptr, tmp;
		ptr = hand.getHead ();

		while (ptr != nullptr) {
			tmp = hand.findRankPair (ptr);
			if (tmp != nullptr) {
				if (first == 1) {
					return 1;
				}
				if (first == 0) {
					first = 1;
				}
			}
			ptr = ptr->next;
		}
		return 0;
	}

	int judgeOnePair () {
		Position ptr, tmp;
		ptr = hand.getHead ();
		while(ptr != nullptr){
			if(ptr->rank == "Jack" || "Queen" || "King" || "Ace"){
				tmp = hand.findRankPair (ptr);
				if (tmp != nullptr)
					return 1;
			}
			ptr = ptr->next;
		}
		return 0;
	}

	int judgeStraight () {
		int i = 0;
		int value[5] = { 0 };
		Position ptr;
		ptr = hand.getHead ();

		while (ptr != nullptr && i < 5) {	
			value[i++] = hand.rankToValue (ptr->rank);
			ptr = ptr->next;
		}

		if (value[4] == 14) {
			for (i = 0; i < 4; i++) {
				if (value[i] != i + 2)
					return 0;
			}
			return 1;
		}

		if (value[0] >= 2 && value[0] <= 10) {
			for (i = 1; i < 5; i++) {
				if (value[i] - value[i - 1] != 1)
					return 0;
			}
			return 1;
		}
		return 0;
	}

	void judge () {
		Position ptr, tmp;

		if (hand.countDeck () != 5){
			cout << "Error: The number of cards in hand desk is not 5." << endl;
			return;
		}

		if (judgeFlush ()) {
			cout << "Flush!" << endl;
			cout << "You earned $6" << endl;
			cout << "You now have $" << (money = money + 6) << endl;
		} else if (judgeStraight ()) {
			cout << "Straight!" << endl;
			cout << "You earned $4" << endl;
			cout << "You now have $" << (money = money + 4) << endl;
		} else if (judgeThree ()) {
			cout << "Three of a kind!" << endl;
			cout << "You earned $3" << endl;
			cout << "You now have $" << (money = money + 3) << endl;
		} else if (judgeTwoPairs ()) {
			cout << "Two Pairs!" << endl;
			cout << "You earned $2" << endl;
			cout << "You now have $" << (money = money + 2) << endl;
		} else if (judgeOnePair ()) {
			cout << "Pair of Jacks or better!" << endl;
			cout << "You earned $1" << endl;
			cout << "You now have $" << (++money) << endl;
		}
				
		ptr = hand.getHead ();
		while (ptr != nullptr) {
			discarded.addCard (ptr);			
			tmp = ptr;
			ptr = ptr->next;
			hand.removeCard (tmp);
		}
		cout << "Discarded desk: " << endl;
		discarded.printDeck ();
		cout << "\nThe discarded deck contains " << discarded.deskCount << " cards.\n" << endl;

		cout << "Press enter to play another round...";
		getchar ();
		cout << "--------------------------------------------------\n" << endl;
	}

	//return 0 to exit;
	//return 1 to play next round;
	//return 2 to go back to hand contains and options;
	int handContains () {
		int returnFlag;
		
		handOutput ();
		hand.printDeck ();
		cout << "\nThe deck contains " << deck.countDeck () << " cards.\n" << endl;

		cout << "OPTIONS..." << endl;
		cout << "- Type the letters for the cards you wish to keep. (i.e., \"acd\")." << endl;
		cout << "- Type \"deck\" to view the cards remaining in the deck." << endl;
		cout << "- Type \"none\" to discard all cards in your hand." << endl;
		cout << "- Type \"all\" to keep all cards in your hand." << endl;
		cout << "- Type \"exit\" to exit the game." << endl;
		cout << "- Type \"swap\" to CHEAT and swap a card in your hand with one in the deck." << endl;
		cout << "YOUR CHOICE: ";

		while ((returnFlag = parseOption ()) == -1) {}       //parse the option of player's choice and then return to the menu
		return returnFlag;
	}

	void handOutput () {
		cout << "\nYour hand contains: " << endl;

		Position ptr = hand.getHead ();
		hand.setHead (hand.merge_sort (ptr));
		ptr = hand.getHead ();

		if (ptr == nullptr) {
			cout << "Error: hand deck is empty but trying to output." << endl;
			return;
		}
		
		for (int i = 0; i < 5 && ptr != nullptr; i++) {
			char tmp;
			tmp = 'A' + i;
			cout << tmp << ": " << ptr->rank << " of " << ptr->suit << endl;
			ptr = ptr->next;
		}
	}

	//return 0 to exit;
	//return 1 to play next round;
	int menu () {
		Position ptr;
		int returnFlag;
		cout << "You pay a $1 ante and now have $" << --money << endl;
		if (money <= 0) {
			cout << "You are out of money! GAME OVER!" << endl;
			return 0;
		}
		
		if (deck.countDeck () < 5) {
			ptr = deck.getHead ();
			deck.setHead (discarded.getHead ());
			discarded.setHead (ptr);
		}

		for (int i = 0; i < 5; i++) {
			ptr = deck.drawCard ();    //draw 5 cards for each round

			if (ptr == nullptr) {
				cout << "Error of adding a nullptr to the deck after drawing the cards." << endl;
				return 0;
			} else {
				hand.addCard (ptr);
				deck.removeCard (ptr);
			}
		}
		
		while ((returnFlag = handContains ()) == 2) {}    //list cards in hand desk and list menu options
		return returnFlag;
	}
};