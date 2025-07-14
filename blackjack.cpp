#include "blackjack.h"
#include <limits>
using namespace std;

//Class Declarations

Card::Card(){
    m_value = 0;
    m_type = 1;
    m_name = "Card";
    m_next_card = nullptr;
}
Card::Card(int value, int type, string name, Card* next_card){
    m_value = value;
    m_name = name;
    m_next_card = next_card;
}
int Card::getValue(){
    return m_value;
}
int Card::getType(){
    return m_type;
}
string Card::getName(){
    return m_name;
}
Card* Card::getNext(){
    return m_next_card;
}
void Card::setNext(Card* card){
    m_next_card = card;
}
//Linked List

Deck::Deck(){
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}
Deck::Deck(Card* head){
    m_head = head;
    m_tail = head;
    m_size = 1;
}
Deck::~Deck(){//Confirm in valgrind that this catches all memory
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
void Deck::appendDeck(Card* card){//Appends card to the end of the deck
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
void Deck::shuffleDeck(){//Shuffles the deck by creating a new head
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
void Deck::printDeck(){//Prints the deck
    Card* currCard = m_head;
    int count = 1;
    while(currCard != nullptr){
        cout << "CARD " << count << ": " << currCard->getName() << ", " << currCard->getValue() << endl;
        /**** *
        if(currCard->getNext() != nullptr)
            cout << "next: " << currCard->getNext()->getName() << endl;
        else
            cout << endl;
        **/
        currCard = currCard->getNext();
        count++;
    }
}
int Deck::getSize(){
    return m_size;
}
Card* Deck::getHead(){
    return m_head;
}
Card* Deck::getTail(){
    return m_tail;
}
void Deck::setHead(Card* card){
    m_head = card;
}
void Deck::setTail(Card* card){
    m_tail = card;
}
void Deck::setSize(int num){
    m_size = num;
}
Card* Deck::takeTop(){
    Card* chosenCard = m_head;
    m_head = m_head->getNext();
    cout << m_head->getNext()->getValue() << endl;
    cout << chosenCard->getValue() << " " << m_head->getValue() << endl;
    m_size--;
    return chosenCard;
}

//Dealer class that holds cards and can play the game, parent of Player class.
void Dealer::drawCard(Deck& deck, int index){
    hand[index].push_back(deck.getHead());
    deck.setHead(deck.getHead()->getNext());
    deck.setSize(deck.getSize() - 1);
}
void Dealer::discard(Deck& deck){
    for(int i = 0; i < 5; i++){
        int size = hand[i].size();
        for(int j = size - 1; j >= 0; j--){
            deck.appendDeck(hand[i].at(j));
            hand[i].pop_back();
        }
    }
}
void Dealer::printHand(){
    cout << "DEALER HAND: " << hand[0].at(0)->getName() << ", " << hand[0].at(0)->getValue() << " -> "
    << "Card Hidden";
    cout << endl;
}
void Dealer::manageTurn(Deck& deck, int handValues[], bool ai){
    int sum = 0;
    int i = 0;
    int aceCount = 0;
    while(sum < 17){
        if(i > 1){
            drawCard(deck, 0);
        }
        sum += hand[0].at(i)->getValue();
        if(hand[0].at(i)->getValue() == 11){
            aceCount++;
        }
        if(ai == false){
            cout << "Dealer Card " << i + 1 << ": " << hand[0].at(i)->getName() << endl;
        }
        if(sum > 21){
            while(sum > 21 && aceCount != 0){
                aceCount--;
                sum -=10;
            }
        }
        i++;
    }
    handValues[10] = sum;
}
int Dealer::naturalCheck(){
    for(int i = 0; i < 5; i++){
        if(hand[i].size() == 2){
            if((hand[i][0]->getValue() + hand[i][1]->getValue()) == 21) return i;
        }
    }
    return -1;
}

Player::Player(bool ai){
    m_isAI = ai;
}
bool Player::getAI(){
    return m_isAI;
}
void Player::printHand(){
    for(int i = 0; i < 5; i++){
        cout << "HAND " << i << ": ";
        for(int j = 0; j < hand[i].size(); j++){
            cout << hand[i].at(j)->getName() << ", " << hand[i].at(j)->getValue() << " -> ";
        }
        cout << endl;
    }
}
void Player::manageTurn(Deck& deck, int handValues[]){
    int sum;
    int splitSum;
    int aceCount;
    int splitAceCount;
    bool canSplit;
    for(int i = 0; i < 5; i++){
        if(hand[i].size() > 0){
            string strChoice = "-1";
            int choice = stoi(strChoice);
            while(choice != 1){
                canSplit = false;
                sum = 0;
                splitSum = 0;
                aceCount = 0;
                splitAceCount = 0;
                cout << "What would you like to do with hand " << i + 1 << "?: ";
                for(int j = 0; j < hand[i].size(); j++){
                    cout << hand[i].at(j)->getName() << ", " << hand[i].at(j)->getValue() << " -> ";
                    sum += hand[i].at(j)->getValue();
                    if(hand[i].at(j)->getValue() == 11){
                        aceCount++;
                    }
                }
                if(sum == 21){
                    cout << "Your hand is already 21! Automatic stay." << endl;
                    handValues[i] = sum;
                    break;
                }
                else if(sum > 21){
                    for(int j = 0; j < hand[i].size(); j++){
                        if(hand[i].at(j)->getValue() == 11 && sum > 21){
                            sum -= 10;
                        }
                    }
                }

                cout << "Sum = " << sum << endl << endl;
                cout << "1. Stay Hand" << endl;
                cout << "2. Draw from Deck" << endl;
                if(splitHand[i].size() == 0 && hand[i].at(0)->getValue() == hand[i].at(1)->getValue()){
                    cout << "3. Split Hand" << endl;
                    canSplit = true;
                }
                getline(cin, strChoice);
                choice = stoi(strChoice);
                if(choice == 1){
                    handValues[i] = sum;
                }
                else if(choice == 2){
                    sum+= deck.getHead()->getValue();
                    drawCard(deck, i);
                }
                else if(choice == 3 && canSplit == true){
                    splitHand[i].push_back(hand[i].at(1));
                    hand[i].pop_back();
                    drawSplitInit(deck, i);
                }

                if(sum > 21){
                    if(hand[i].at(hand[i].size() - 1)->getValue() == 11){
                        sum -= 10;
                        choice = -1;
                    }
                    else{
                        cout << "Your hand has busted!" << endl;
                        handValues[i] = sum;
                        break;
                    }
                }
                if(sum == 21){
                    cout << "You now have 21 in hand. Nice!" << endl;
                    handValues[i] = 21;
                    break;
                }
                else if(sum < 21 && choice == 2){
                    choice = -1;
                }
            }
        }
        if(splitHand[i].size() > 0){
            string strChoice = "-1";
            int choice = stoi(strChoice);
            while(choice != 1){
                canSplit = false;
                splitSum = 0;
                splitAceCount = 0;
                cout << "What would you like to do with split hand " << i + 1 << "?: ";
                for(int j = 0; j < splitHand[i].size(); j++){
                    cout << splitHand[i].at(j)->getName() << ", " << splitHand[i].at(j)->getValue() << " -> ";
                    splitSum += splitHand[i].at(j)->getValue();
                    if(splitHand[i].at(j)->getValue() == 11){
                        splitAceCount++;
                    }
                }
                if(splitSum == 21){
                    cout << "Your hand is already 21! Automatic stay." << endl;
                    handValues[i + 5] = splitSum;
                    break;
                }
                else if(splitSum > 21){
                    for(int j = 0; j < splitHand[i].size(); j++){
                        if(splitHand[i].at(j)->getValue() == 11 && splitSum > 21){
                            splitSum -= 10;
                        }
                    }
                }

                cout << "Split Sum = " << splitSum << endl;
                cout << "1. Stay Hand" << endl;
                cout << "2. Draw from Deck" << endl;
                getline(cin, strChoice);
                choice = stoi(strChoice);
                if(choice == 1){
                    handValues[i + 5] = splitSum;
                }
                else if(choice == 2){
                    splitSum+= deck.getHead()->getValue();
                    drawSplit(deck, i);
                }

                if(splitSum > 21){
                    if(hand[i].at(hand[i].size() - 1)->getValue() == 11){
                        splitSum -= 10;
                        choice = -1;
                    }
                    else{
                        cout << "Your hand has busted!" << endl;
                        handValues[i + 5] = splitSum;
                        break;
                    }
                }
                if(splitSum == 21){
                    cout << "You now have 21 in hand. Nice!" << endl;
                    handValues[i + 5] = 21;
                    break;
                }
                else if(splitSum < 21 && choice == 2){
                    choice = -1;
                }
            }
        }
    }
}
void Player::aiManageTurn(Deck& deck, int handValues[]){
    int sum;
    int aceCount = 0;
    for(int i = 0; i < 5; i++){
        if(hand[i].size() > 0){
            int choice = 0;
            while(choice != 1){
                sum = 0;
                for(int j = 0; j < hand[i].size(); j++){
                    sum += hand[i].at(j)->getValue();
                    if(hand[i].at(j)->getValue() == 11){
                        aceCount++;
                    }
                }

                if(sum == 21){
                    handValues[i] = sum;
                    break;
                }
                else if(sum > 21){
                    for(int j = 0; j < hand[i].size(); j++){
                        if(hand[i].at(j)->getValue() == 11 && sum > 21){
                            sum -= 10;
                        }
                    }
                }

                if(sum > AI_STAY_COUNT){
                    choice = 1;
                }
                else{
                    choice = 2;
                }
                if(choice == 1){
                    handValues[i] = sum;
                }
                else if(choice == 2){
                    sum+= deck.getHead()->getValue();
                    drawCard(deck, i);
                }

                if(sum > 21){
                    if(hand[i].at(hand[i].size() - 1)->getValue() == 11){
                        sum -= 10;
                        choice = -1;
                    }
                    else{
                        handValues[i] = sum;
                        break;
                    }
                }
                if(sum == 21){
                    handValues[i] = 21;
                    break;
                }
                else if(sum < 21 && choice == 2){
                    choice = -1;
                }
            }
        }
    }
}
void Player::drawSplitInit(Deck& deck, int index){
    hand[index].push_back(deck.getHead());
    deck.setHead(deck.getHead()->getNext());
    deck.setSize(deck.getSize() - 1);

    splitHand[index].push_back(deck.getHead());
    deck.setHead(deck.getHead()->getNext());
    deck.setSize(deck.getSize() - 1);
}
void Player::drawSplit(Deck& deck, int index){
    splitHand[index].push_back(deck.getHead());
    deck.setHead(deck.getHead()->getNext());
    deck.setSize(deck.getSize() - 1);
}
void Player::discardSplit(Deck& deck){
    for(int i = 0; i < 5; i++){
        int size = splitHand[i].size();
        for(int j = size - 1; j >= 0; j--){
            deck.appendDeck(splitHand[i].at(j));
            splitHand[i].pop_back();
        }
    }
}

//global variables
const string TYPES[4] = {" of Spades", " of Hearts", " of Clubs", " of Diamonds"};
const string UNIQUE_VALUES[3] = {"Jack", "Queen", "King"};
const int DECK_COUNT = 1;
const int AI_STAY_COUNT = 16;
const int AI_HAND_SIZE = 1;

//function declarations
void initialize(Deck& deck){//Initializes the deck
    for(int i = 2; i < 15; i++){
        Card* newCard;
        int cardValue;
        string cardName;
        for(int k = 1; k < 5; k++){
            if(i == 14){
                cardValue = 11;
                cardName = "Ace" + TYPES[k-1];
            }
            else if(i > 10){
                cardValue = 10;
                cardName = UNIQUE_VALUES[13 - i] + TYPES[k-1];
            }
            else{
                cardValue = i;
                cardName = to_string(i) + TYPES[k-1];
            }
            for(int i = 0; i < DECK_COUNT; i++){
                newCard = new Card(cardValue, k, cardName, nullptr);
                deck.appendDeck(newCard);
            }    
        }
    }
}
void stackDeck(Deck &deck1, Deck &deck2){
    deck2.getTail()->setNext(deck1.getHead());
    deck1.setHead(deck2.getHead());
    deck2.setHead(nullptr);
    deck2.setTail(nullptr);
    deck1.setSize(deck1.getSize() + deck2.getSize());
    deck2.setSize(0);
}
void winChecker(int handValues[], int winArray[]){//Value of 1 = win, Value of 0 = Hand not played, Value of -1 = Hand lost, Value of -2 = Hands Tied
    if(handValues[10] > 21){
        for(int i = 0; i < 10; i++){
            if(handValues[i] != 0 && handValues[i] < 22){
                winArray[i] = 1;
            }
            else if(handValues[i] > 21){
                winArray[i] = -2;
            }
        }
    }
    else{
        for(int i = 0; i < 10; i++){
            if(handValues[i] != 0){
                if((handValues[i] > handValues[10] && handValues[i] < 22)){
                    winArray[i] = 1;
                }
                else if(handValues[i] == handValues[10]){
                    winArray[i] = -2;
                }
                else{
                    winArray[i] = -1;
                }
            }
        }
    }
}
void playRound(Deck &deck, Deck &discardDeck, Dealer dealer, Player player, int gameTable[][31], int winTable[][31]){//Plays 1 round of blackjack through
    int handSize;
    if(player.getAI() == true){
        handSize = AI_HAND_SIZE;
    }
    else{
        string strHandSize = "-1";
        handSize = stoi(strHandSize);
        while((handSize < 1 || handSize > 5)){
            cout << "Choose hand size from 1-5: ";
            getline(cin, strHandSize);
            handSize = stoi(strHandSize);
            cout << endl;
            if(handSize < 1 || handSize > 5){
                cout << "THAT IS AN INVALID HANDSIZE" << endl;
            }
        }
    }
    for(int a = 0; a < 2; a++){
        for(int i = 0; i < handSize; i++){
            player.drawCard(deck, i);
        }
        dealer.drawCard(deck, 0);
    }

    int handValues[11] = {0};
    if(player.getAI() == true){
        player.aiManageTurn(deck, handValues);
        dealer.manageTurn(deck, handValues, true);
    }
    else{
        player.manageTurn(deck, handValues);
        dealer.manageTurn(deck, handValues, false);
    }
    
    int winArray[10] = {0};

    winChecker(handValues, winArray);

    if(player.getAI() == false){
        for(int i = 0; i < 10; i++){
            if(winArray[i] != 0){
                cout << "Hand " << ((i > 4) ? i - 4 : i + 1) << ((i > 4) ? " Split: " : ": ");
            }
            
            if(winArray[i] == 1){
                cout << "WIN" << endl;
            }
            else if(winArray[i] == -1){
                cout << "LOSE" << endl;
            }
            else if(winArray[i] == -2){
                cout << "TIE" << endl;
            }
        }
    }

    for(int i = 0; i < handSize; i++){
        gameTable[dealer.getHand(0).at(0)->getValue() - 2][handValues[i] - 2]++;
        if(winArray[i] == 1){
            winTable[dealer.getHand(0).at(0)->getValue() - 2][handValues[i] - 2]++;
        }
    }

    player.discard(discardDeck);
    player.discardSplit(discardDeck);
    dealer.discard(discardDeck);

    if(deck.getSize() <= 52){
        stackDeck(deck, discardDeck);
        deck.shuffleDeck();
    }
}
void mainMenu(Deck &deck, Deck &discardDeck, Dealer dealer, Player player, int gameTable[][31], int winTable[][31]){//Function that displays and adds functionality to the main menu
    cout << "Welcome to FAIR Black Jack ;): " << endl << endl << "1. Play Game" << endl << "2. Configure Rules" << endl << "3. Quit" << endl << "--------------------------------" <<endl << "->";
    string response;
    getline(cin, response);

    for(char &c : response){
        c = tolower(c);
    }

    cout << endl;
    if(response == "1" || response == "play" || response == "game" || response == "play game"){
        playRound(deck, discardDeck, dealer, player, gameTable, winTable);
    }
    else if(response == "2" || response == "configure" || response == "rules" || response == "configure rules"){
        cout << "No settings currently implemented" << endl;
    }
    else if(response == "3" || response == "quit"){
        exit(0);
    }
}
