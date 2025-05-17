#include <fstream>
#include "blackjack.h"

const int TEST_ROUNDS = 10000;

void test1(ofstream& file){//Tests winrate comparing a player's first card drawn vs dealer's first card
    int gameTable[31][31] = {0};
    int winTable[31][31] = {0};
    Deck deck;
    Deck discardDeck;
    initialize(deck);
    deck.shuffleDeck();
    Dealer dealer1;
    Player player1(true);

    for(int i = 0; i < TEST_ROUNDS; i++){
        playRound(deck, discardDeck, dealer1, player1, gameTable, winTable);
    }

    for(int i = 0; i < 31; i++){
        for(int j = 0; j < 31; j++){
            if(gameTable[i][j] != 0){
                file << "Games With Dealer Hand Value " << i + 2 << " VS. Player Hand Value " << j + 2 << ": Games Played = " << gameTable[i][j] << ", Games Won: " << winTable[i][j] << ", Winrate = " <<  (float(winTable[i][j]) / gameTable[i][j]) * 100 << "%" << endl << endl;
            }
        }
    }
}

void mainMenu(ofstream& file){
    int choice = 0;
    string strChoice = "0";
    while(choice != 1 && choice != 2){
        cout << "Which Test Option Would You Like To Do?" << endl << endl;
        cout << "1. Value Combo Test" << endl;
        cout << endl << ">> ";
        getline(cin, strChoice);
        choice = stoi(strChoice);
    }
    if(choice == 1){
        test1(file);
    }
}

int main(){
    if(AI_STAY_COUNT >= 21){
        cerr << "ERROR: CANNOT SET AI STAY COUNT TO 21+!";
        exit(1);
    }
    if(AI_STAY_COUNT < 2){
        cerr << "ERROR: CANNOT SET AI STAY COUNT TO UNDER 2!";
        exit(1);
    }
    ofstream testFile("gameResults.txt");

    mainMenu(testFile);

    testFile.close();
}