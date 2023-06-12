# 45s C++ Library
<work in progress, should be completed within the next week>
Implementing 45s so you don't have to. To make the library easier to use, there is a single large .cpp & .hpp file, so you can include that one. For a more organized look at the code / documentation, you can look in the `Files` folder.

There is a 45s class, a card class, a deck class, and an abstract Player class. You are encouraged to implement the Player class.

## How to use
<put description of the example main>

## x45s
Controls the game. Holds pointers to the 4 players, a deck, a discardDeck to store the cards the players play, the scores of both teams, the amount bid, the suit bid, the bidder (aka the player who bid), the initial score of the bidder (used to calculate if the bidder made their bid or not), and the player who dealt the hand.

The default constructor does not exist. You must provide 4 pointers to player objects. This constructor also calls the default constructor for the deck.

deal_players deals each player 5 cards.

shuffle calls shuffle on the deck 10 times.

reset resets the hands of the player and the deck.

deal_kiddie deals the kiddie to the player who won the bid. You need to pass the player who won the bid as a parameter.

evaluate_trick returns the best card of the 4 cards. TODO: Consider which player led the hand.

updateScores adds 5 points to the team that is passed in the parameter. Must be passed team 0 or 1.

hasWon returns true if either team has won the game.

whichPlayerWon returns the number of the team that won the game, or -1 if no one has won.

havePlayersDiscard calls the player discard method for each player.

getBidAmount returns the amount of the bid.

setBid takes (bid, bidderNum). These represent the bid amount and the number of the player who bid.

getBidder calls the getBid method & returns the player who won the bid (0, 1, 2, 3). This could be confusing with getBidAmount, I will think of how to resolve their naming similarities.

getBidSuit returns the suit of the bid.

determineIfWonBid returns true if the players won the bid, and false otherwise.

getTeamScore returns the score of the team number passed to it.

havePlayersPlayCards takes the number of the player thas is leading, and calls playCard on each of the players in the correct order. It keeps track of the cards played, and passes this information to the playCard method.

## Card
An int value, int suit that runs the entire program.

There is a default constructor to initalize the card to -1000, -1000

The other constructor takes two parameters: (value, suit)

The operator<< is defined, and outputs the card in a human readable format (e.g. King of Hearts).

Copy constructor is defined

Assignment operator is defined

getValue, getSuit, setValue, setSuit are all defined

operator< and all logical operators are defined

## Deck
There is only a default constructor. It initalizes the deck to all 52 cards.

shuffle has an optional int parameter to tell it how many times to shuffle. With no parameter, it shuffles once.

pop_back returns the last card in the deck and deletes it

peek_back returns the last card in the deck but does not delete it

reset is the same as the constructor, it deletes the deck and initalizes it again to all 52 cards.

removeCard takes either a Card or (value, suit). It searches for this card and deletes it if it is found. If the card is not found, it throws an exception.

findCard takes either a card of (value, suit). It searches for the card and returns true if it is found, and false otherwise. Only removeCard if you first confirm the card exists with findCard.

The copy assignment operator is defined

The operator<< is defined, and it outputs the cards separated by a space, with no newlines.
## Player
There is only a default constructor.

Each player's hand is a vector of Cards. 

### Player's virtual functions
discard (the player can choose to remove cards from their hand)

getBid (the player can choose a bid. It is <value, suit>)

playCard (the player can choose a card to play from their hand. They are passed the vector of cards played so far this hand)

I have no memory of why I added `virtual Card takeCardInput(std::string inp) = 0;` or what it does.

### Player's not virtual functions
dealCard is called by x45s to push back cards to the hand.

getSize returns the size of the hand.

resetHand is called to delete all cards of the hand after a round is completed.

printHand prints the entire hand on one line. If given a parameter it prints to whatever ostream you give it. With no parameter it prints to cout. Both include the trailing "\n".

## Suit
The enum class Suit has Hearts, Diamonds, Clubs, and Spades, as well as ACE_OF_HEARTS to represent the special case of the ace of hearts.

I handle the ace of hearts by giving it its own suit, and its own value of -1.

## trumpGlobalVariable
The program keeps track of trump and suitLed via global variables. Only main should update those.