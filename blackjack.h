#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

extern const string TYPES[4];
extern const string UNIQUE_VALUES[3];
extern const int DECK_COUNT;
extern const int AI_STAY_COUNT;
extern const int AI_HAND_SIZE;
extern const int MAX_HAND_SIZE;

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
    Card* m_split_card;
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
    vector<Card*> hand[10];//Double the amount you want your max hand size to be, every other index will be room for a split hand
};
class Player: public Dealer{
    public:
    Player(bool ai);
    bool getAI();
    void setMoney(int money);
    int getMoney();
    virtual void printHand(int index);
    virtual void manageTurn(Deck& deck, int handValues[]);
    void aiManageTurn(Deck& deck, int handValues[]);
    int handValueSum(int index);
    bool canSplit(int index);
    private:
    bool m_isAI;
    int m_money;
};

void initialize(Deck& deck);
void stackDeck(Deck &deck1, Deck &deck2);
void initializeMoney(Player player);
void winChecker(int handValues[], int winArray[]);
void playRound(Deck &deck, Deck &discardDeck, Dealer dealer, Player player, int gameTable[][31], int winTable[][31]);
void mainMenu(Deck &deck, Deck &discardDeck, Dealer dealer, Player player, int gameTable[][31], int winTable[][31]);