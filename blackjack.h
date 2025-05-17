#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

class Card{
    public:
    Card();
    Card(int value, int type, string name, Card* next_card);
    int getValue();
    int getType();
    string getName();
    Card* getNext();
    void setNext(Card* card);
    private:
    int m_value;
    int m_type;
    string m_name;
    Card* m_next_card;
};
class Deck{
    public:
    Deck();
    Deck(Card* head);
    ~Deck();
    void appendDeck(Card* card);
    void shuffleDeck();
    void printDeck();
    int getSize();
    Card* getHead();
    Card* getTail();
    void setHead(Card* card);
    void setTail(Card* card);
    void setSize(int num);
    Card* takeTop();
    private:
    Card* m_head;
    Card* m_tail;
    int m_size;
};
class Dealer{
    public:
    vector<Card*> getHand(int index){
        return hand[index];
    }
    void drawCard(Deck& deck, int index);
    void discard(Deck& deck);
    virtual void printHand();
    virtual void manageTurn(Deck& deck, int handValues[], bool ai);
    int naturalCheck();
    protected:
    vector<Card*> hand[5];
};
class Player: public Dealer{
    public:
    Player(bool ai);
    bool getAI();
    virtual void printHand();
    virtual void manageTurn(Deck& deck, int handValues[]);
    void aiManageTurn(Deck& deck, int handValues[]);
    private:
    bool m_isAI;
};

extern const string TYPES[4];
extern const string UNIQUE_VALUES[3];
extern const int DECK_COUNT;
extern const int AI_STAY_COUNT;
extern const int AI_HAND_SIZE;

void initialize(Deck& deck);
void stackDeck(Deck &deck1, Deck &deck2);
void winChecker(int handValues[], int winArray[]);
void playRound(Deck &deck, Deck &discardDeck, Dealer dealer, Player player, int gameTable[][31], int winTable[][31]);
void mainMenu(Deck &deck, Deck &discardDeck, Dealer dealer, Player player, int gameTable[][31], int winTable[][31]);