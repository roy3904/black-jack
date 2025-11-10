#include "blackjack.h"

int main(){
    srand(time(0));
    Deck deck1;
    Deck deck2;
    initialize(deck1);
    cout << endl << "NOW SHUFFLING DECK" << endl << endl;
    deck1.shuffleDeck();
    Dealer dealer1;
    Player player1(false);
    int gameTable[31][31] = {0};
    int winTable[31][31] = {0};

    initializeMoney(player1);

    while(1){
        mainMenu(deck1, deck2, dealer1, player1, gameTable, winTable);
    }
}