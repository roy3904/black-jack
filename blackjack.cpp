#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int DECK_COUNT = 1;

//Class Declarations

class Card{//Node in linked list
    public:
    Card(){
        m_value = 0;
        m_type = 1;
        m_name = "Card";
        m_next_card = nullptr;
    }
    Card(int value, int type, string name, Card* next_card){
        m_value = value;
        m_name = name;
        m_next_card = next_card;
    }
    int getValue(){
        return m_value;
    }
    int getType(){
        return m_type;
    }
    string getName(){
        return m_name;
    }
    Card* getNext(){
        return m_next_card;
    }
    void setNext(Card* card){
        m_next_card = card;
    }
    private:
    int m_value;
    int m_type;
    string m_name;
    Card* m_next_card;
};
class Deck{//Linked List
    public:
    Deck(){
        m_head = nullptr;
        m_tail = nullptr;
        m_size = 0;
    }
    Deck(Card* head){
        m_head = head;
        m_tail = head;
        m_size = 1;
    }
    ~Deck(){//Confirm in valgrind that this catches all memory
        Card* currCard = m_head;
        while(currCard != nullptr){
            m_head = currCard;
            currCard = m_head->getNext();
            delete m_head;
            m_size--;
        }
        m_head = nullptr;
        m_tail = nullptr;
    }
    void appendDeck(Card* card){//Appends card to the end of the deck
        card->setNext(nullptr);
        if(m_head == nullptr){
            m_head = card;
            m_tail = card;
        }
        else{
            m_tail->setNext(card);
            m_tail = m_tail->getNext();
        }
        m_size++;
    }
    void shuffleDeck(){//Shuffles the deck by creating a new head
        int chosenOne;
        int cardsLeft = m_size;
        Card* currCard;
        Card* prevCard;
        Card* newHead = nullptr;
        Card* newTail = nullptr;
        while(cardsLeft > 0){//Selects a new head, and then slowly takes cards from the "old" list by random, and adds them to the "new" list.
            cardsLeft--;
            chosenOne = rand() % (cardsLeft + 1);
            currCard = m_head;
            for(int i = 0; i < chosenOne; i++){
                prevCard = currCard;
                currCard = currCard->getNext();
            }
            if(currCard == m_head && m_head->getNext() != nullptr){//If randomly selected card is m_head, we must re assign m_head to not break the old list
                m_head = m_head->getNext();
            }
            else if(currCard == m_tail && cardsLeft > 0){//If m_tail is randoml selected, we must re assign m_tail to not break the old list
                m_tail = prevCard;
            }
            else if(currCard->getNext() != nullptr){//In most cases, a card picked will be in the middle of the list. We must make sure the previous card to the one selected has its next pointer relinked to the selected card's next pointer
                prevCard->setNext(currCard->getNext());
            }
            currCard->setNext(nullptr);
            if(newHead == nullptr){
                newHead = currCard;
                newTail = currCard;
            }
            else{
                currCard->setNext(newHead);
                newHead = currCard;
            }
        }
        m_head = newHead;
        m_tail = newTail;
    }
    void printDeck(){//Prints the deck
        Card* currCard = m_head;
        while(currCard != nullptr){
            cout << currCard->getName() << ", " << currCard->getValue();
            /**** *
            if(currCard->getNext() != nullptr)
                cout << "next: " << currCard->getNext()->getName() << endl;
            else
                cout << endl;
            **/
            currCard = currCard->getNext();
        }
    }
    int getSize(){
        return m_size;
    }
    Card* getHead(){
        return m_head;
    }
    Card* getTail(){
        return m_tail;
    }
    void setHead(Card* card){
        m_head = card;
    }
    Card* takeTop(){
        Card* chosenCard = m_head;
        m_head = m_head->getNext();
        cout << m_head->getNext()->getValue() << endl;
        cout << chosenCard->getValue() << " " << m_head->getValue() << endl;
        return chosenCard;
    }
    private:
    Card* m_head;
    Card* m_tail;
    int m_size;
};

class Dealer{//Dealer class that holds cards and can play the game, parent of Player class.
    public:
    void drawCard(Deck& deck){
       hand.push_back(deck.getHead());
       deck.setHead(deck.getHead()->getNext());
    }
    void discard(Deck& deck){
        int size = hand.size();
        for(int i = size - 1; i >=  0; i--){
            cout << "NOW DISCARDING " << hand.at(i)->getName() << "Whose next node is: " << hand.at(i)->getNext()->getName() << endl;
            deck.appendDeck(hand.at(i));
            //hand.pop_back();
        }
    }
    void printHand(){
        for(int i = 0; i < hand.size(); i++)
            cout << hand.at(i)->getName() << ", " << hand.at(i)->getValue() << endl;
    }
    private:
    vector<Card*> hand;
};

class Player: public Dealer{
    public:

};

int main(){
    srand(time(0));
    string types[4] = {" of Spades", " of Hearts", " of Clubs", " of Diamonds"};
    string unique_values[3] = {"Jack", "Queen", "King"};
    Deck deck1;
    Card* newCard;
    int cardValue;
    string cardName;
    for(int i = 2; i < 15; i++){
        for(int k = 1; k < 5; k++){
            if(i == 14){
                cardValue = 11;
                cardName = "Ace" + types[k-1];
            }
            else if(i > 10){
                cardValue = 10;
                cardName = unique_values[13 - i] + types[k-1];
            }
            else{
                cardValue = i;
                cardName = to_string(i) + types[k-1];
            }
            for(int i = 0; i < DECK_COUNT; i++){
                newCard = new Card(cardValue, k, cardName, nullptr);
                deck1.appendDeck(newCard);
            }    
        }
    }
    cout << endl << "NOW SHUFFLING DECK" << endl << endl;
    deck1.shuffleDeck();
    deck1.printDeck();
    Deck discardPile;
    Dealer dealer1;
    dealer1.drawCard(deck1);
    dealer1.drawCard(deck1);
    dealer1.drawCard(deck1);
    dealer1.printHand();
    dealer1.discard(discardPile);
    discardPile.printDeck();//In this example, last 2 are infinitely looping when printing. Solve this before moving on.
}