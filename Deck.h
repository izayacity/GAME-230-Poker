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
#include<time.h>
using namespace std;

struct Card {
	string suit;
	string rank;
	Card *next;
};
typedef struct Card *Position;

class Deck {
private:
	Position head; // a pointer to the first Card

public:	
	int deskCount;
	Deck () {
		head = nullptr;
		deskCount = 0;
	}

	~Deck () {
		freeDeck ();
	}

	Position getHead () {
		return head;
	}

	void setHead (Position ptr) {
		head = ptr;;
	}

	// This returns the number of cards in the deck
	int countDeck () {
		int count = 0;
		Position ptr = head;
		if (ptr == nullptr) {
			deskCount = 0;
			return 0;
		}
		while (ptr) {
			ptr = ptr->next;
			count++;
		}
		deskCount = count;
		return count;
	}

	// This prepends a new value at the beginning of the list
	void addCard (string rank, string suit) {
		Card *n = new Card ();   // create new Card
		n->rank = rank;
		n->suit = suit;
		n->next = head;         // make the Card point to the next Card.
								//  If the list is empty, this is NULL, so the end of the list --> OK
		head = n;               // last but not least, make the head point at the new Card.
		deskCount++;
	}

	void addCard (Position ptr) {
		if (ptr == nullptr){
			cout << "Error of adding a nullptr to the deck." << endl;
			return;
		} else {
			Card *n = new Card ();   // create new Card
			n->rank = ptr->rank;
			n->suit = ptr->suit;
			n->next = head;         // make the Card point to the next Card.
									//  If the list is empty, this is NULL, so the end of the list --> OK
			head = n;               // last but not least, make the head point at the new Card.*/
			deskCount++;
		}	
	}

	// This construct a deck
	void init () {
		addCard ("Ace", "Hearts");
		addCard ("King", "Hearts");
		addCard ("Queen", "Hearts");
		addCard ("Jack", "Hearts");
		addCard ("10", "Hearts");
		for (int i = 9; i >= 2; i--) {
			string j (1, '0' + i);
			addCard (j, "Hearts");
		}

		addCard ("Ace", "Spades");
		addCard ("King", "Spades");
		addCard ("Queen", "Spades");
		addCard ("Jack", "Spades");
		addCard ("10", "Spades");

		for (int i = 9; i >= 2; i--) {
			string j (1, '0' + i);
			addCard (j, "Spades");
		}

		addCard ("Ace", "Diamonds");
		addCard ("King", "Diamonds");
		addCard ("Queen", "Diamonds");
		addCard ("Jack", "Diamonds");
		addCard ("10", "Diamonds");

		for (int i = 9; i >= 2; i--) {
			string j (1, '0' + i);
			addCard (j, "Diamonds");
		}

		addCard ("Ace", "Clubs");
		addCard ("King", "Clubs");
		addCard ("Queen", "Clubs");
		addCard ("Jack", "Clubs");
		addCard ("10", "Clubs");

		for (int i = 9; i >= 2; i--) {
			string j (1, '0' + i);
			addCard (j, "Clubs");
		}
		deskCount = 52;
	}

	int parseChar (char ch) {
		int val;
		if (ch >= '2' && ch <= '9') {
			val = ch - '0';
		} else {
			switch (ch) {
			case ('1'):
				val = 10; break;
			case 'J':
				val = 11; break;
			case 'Q':
				val = 12; break;
			case 'K':
				val = 13; break;
			case 'A':
				val = 14; break;
			}
		}
		return val;
	}

	int compareValue (Position a, Position b) {
		char a1 = (a->rank)[0];
		char b1 = (b->rank)[0];
		int a2 = parseChar (a1);
		int b2 = parseChar (b1);
		return (a2 <= b2) ? 1 : 0;		
	}

	//The main function
	Position merge_sort (Position head) {
		if (head == nullptr || head->next == nullptr) { 
			return head;
		}
		Position middle = getMiddle (head);      //get the middle of the list
		Position sHalf = middle->next; 

		middle->next = nullptr;   //split the list into two halfs

		return merge (merge_sort (head), merge_sort (sHalf));  //recurse on that
	}

	//Merge subroutine to merge two sorted lists
	Position merge (Position a, Position b) {
		Card *dummyHead = new Card ();
		Position curr = dummyHead;

		while (a != nullptr && b != nullptr) {
			if (compareValue(a, b)) {
				curr->next = a; a = a->next;
			} else {
				curr->next = b; b = b->next; 
			}
			curr = curr->next;
		}
		curr->next = (a == nullptr) ? b : a;

		Position tmp = dummyHead->next;
		delete (dummyHead);
		return tmp;
	}

	//Finding the middle element of the list for splitting
	Position getMiddle (Position head) {
		if (head == nullptr) { 
			return head; 
		}
		Position slow, fast; 
		slow = fast = head;

		while (fast->next != nullptr && fast->next->next != nullptr) {
			slow = slow->next; fast = fast->next->next;
		}
		return slow;
	}

	// This print out the current deck
	void printDeck () {
		Position ptr = head;
		if (ptr == nullptr) {
			cout << "head is empty while printing" << endl;
			return;
		}
		while (ptr) {
			cout << "List item: " << ptr->rank << " of " << ptr->suit << endl;
			ptr = ptr->next;
		}
	}

	// This dismiss the link list deck
	void freeDeck () {
		Position ptr = head, tmp;
		if (ptr == nullptr) {
			return;
		}
		while (ptr) {
			tmp = ptr;
			ptr = ptr->next;
			delete (tmp);
		}
	}

	void removeCard (Position ptr) {
		if (ptr == nullptr) {
			cout << "Trying to remove a nullptr" << endl;
			return;
		}

		Position prev = findPrevious (ptr);
		if (prev == nullptr && ptr == head) {
			head = head->next;
		}else{
			prev->next = ptr->next;
		}
		delete (ptr);
		deskCount--;
	}

	Position drawCard () {
		Position ptr = head;
		srand ((unsigned int)time (NULL));
		int val = rand () % countDeck();

		for (int i = 1; i <= val && ptr->next != nullptr; i++) {
			ptr = ptr->next;
		}
		return ptr;
	}
	//lack of check of prev
	Position findPrevious (Position ptr) {
		Position prev = head;
		if (head == nullptr) {
			//cout << "Empty deck while trying to find previous node" << endl;
			return nullptr;
		}
		if (ptr == head) {
			//cout << "Trying to find previous node of head" << endl;
			return nullptr;
		}
		while (prev->next != nullptr && prev->next != ptr)
			prev = prev->next;
		return prev;
	}

	string suitToString (char ch) {
		switch (ch) {
		case 'c':
			return "Clubs";
		case 'd':
			return "Diamonds";
		case 'h':
			return "Hearts";
		case 's':
			return "Spades";
		}
	}

	string rankToString (int val) {
		if (val == 1 || val == 14)
			return "Ace";
		switch (val) {
		case 10:
			return "10";
		case 11:
			return "Jack";
		case 12:
			return "Queen";
		case 13:
			return "King";
		}
		string j (1, '0' + val);
		return j;
	}

	int rankToValue (string rank) {
		char ch = rank[0];
		if (ch >= '2' && ch <= '9') {
			return ch - '0';
		} else if (rank.compare ("10") == 0){
			return 10;
		} else if (rank.compare ("Jack") == 0) {
			return 11;
		} else if (rank.compare ("Queen") == 0) {
			return 12;
		} else if (rank.compare ("King") == 0) {
			return 13;
		} else if (rank.compare ("Ace") == 0) {
			return 14;
		}
	}

	Position findNode (string rank, string suit) {
		Position ptr = head;
		if (ptr == nullptr) {
			return nullptr;
		}

		while (ptr->suit != suit) {
			ptr = ptr->next;
			if (ptr == nullptr) {
				return nullptr;
			}			
		}
		while (ptr->rank != rank) {
			ptr = ptr->next;
			if (ptr == nullptr) {
				return nullptr;
			}			
		}
		return ptr;
	}

	int findAllCount (string rank) {
		Position ptr = head;
		int count = 0;

		if (ptr == nullptr)
			return 0;

		while (ptr != nullptr) {
			if ((ptr->rank).compare (rank) == 0) {
				count++;
			}
			ptr = ptr->next;
		}
		return count;
	}

	Position findRankPair (Position init) {
		Position ptr = init;

		while (ptr->next != nullptr) {
			ptr = ptr->next;
			if ((ptr->rank).compare (init->rank) == 0) {
				return ptr;
			}
		}
		return nullptr;
	}
};
