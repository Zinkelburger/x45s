# 45s C++ Library
<work in progress, should be completed within the next week>

Implementing 45s so you don't have to. To make the library easier to use, there is a single large .cpp & .hpp file, so you can include that one. For a more organized look at the code/documentation, you can look in the `Files` folder.

There is a 45s class, a card class, a deck class, and an abstract Player class. You are encouraged to implement the Player class.

## How to use
<put description of the example main>

## x45s
### Description
Controls the game. Holds pointers to the 4 players, a deck, a discardDeck to store the cards the players play, the scores of both teams, the amount bid, the suit bid, the bidder (aka the player who bid), the initial score of the bidder (used to calculate if the bidder made their bid or not), and the player who dealt the hand.

The default constructor does not exist. There are two ways to initialize a x45s object.

1. You can initialize a derived player object with `new`, upcast it, and pass 4 Player pointers to the x45s constructor. You are responsible for managing the Players' memory.

2. You can pass four constructors to x45s when you call it. This allows x45s to manage the memory for the players. The constructor can be called like:

`x45s arbiter([](){ return new derivedPlayer1(); }, [](){ return new derivedPlayer2(); }, [](){ return new derivedPlayer3(); }, [](){ return new derivedPlayer4(); })`

### Methods
`dealBidAndFullFiveTricks()` deals to the players, has the players bid, plays an entire hand (5 tricks), and returns a pair of <int, bool>. The int is the bidder, and the bool is whether they won or not.

`deal_players` deals each player 5 cards.

`shuffle` calls shuffle on the deck 10 times.

`reset` resets the hands of the player and the deck.

`deal_kiddie` deals the kiddie to the player who won the bid. You need to pass the player who won the bid as a parameter.

`evaluate_trick` returns the best card of the 4 cards. TODO: Consider which player led the hand.

`updateScores` adds 5 points to the team that is passed in the parameter. Must be passed team 0 or 1.

`hasWon` returns true if either team has won the game.

`whichPlayerWon` returns the number of the team that won the game, or -1 if no one has won.

`havePlayersDiscard` calls the player discard method for each player.

`getBidAmount` returns the amount of the bid.

`setBid` takes (bid, bidderNum). These represent the bid amount and the number of the player who bid.

`getBidder` calls the `getBid` method & returns the player who won the bid (0, 1, 2, 3). bidHistory stores the record of previous bids, and this information is passed to the players. This could be confusing with getBidAmount, I will think of how to resolve their naming similarities.

`getBidSuit` returns the suit of the bid.

`int getHandSize(int playerNum)` returns the size of the player's hand.

`determineIfWonBidAndDeduct` returns true if the players won the bid, and false otherwise. It also deducts points if the player lost their bid. It should only be called once at the end of each hand (5 tricks).

`getTeamScore` returns the score of the team number passed to it.

`getNumPlayers` returns the number of players playing the game. This should always be 4.

`havePlayersPlayCards` takes the number of the player that is leading, and calls playCard on each of the players in the correct order. It keeps track of the cards played and passes this information to the `playCard` method.

## Player
There is a default constructor, a variadic constructor that takes as many Cards as you want, and a constructor that takes a vector of Cards.

Each player's hand is a vector of Cards. 

### Player's virtual functions
`discard` (the player can choose to remove cards from their hand)

`getBid` (the player can choose a bid. It is <value, suit>). The function is always called with a vector of ints, so that the player can consider other players' bids when they make their bid.

`bagged` The player dealt and was bagged. They are forced to bid. There are no parameters, as if you are bagged then no one else has bid.

`playCard` (the player can choose a card to play from their hand. They are passed the vector of cards played so far this hand)

### Player's non-virtual functions
dealCard is called by x45s to push back cards to the hand.

getSize returns the size of the hand.

resetHand is called to delete all cards of the hand after a round is completed.

printHand prints the entire hand on one line. If given a parameter it prints to whatever ostream you give it. With no parameter, it prints to cout. Both include the trailing "\n".

## Suit
The enum class Suit has Hearts, Diamonds, Clubs, and Spades, as well as ACE_OF_HEARTS to represent the special case of the ace of hearts.

I handle the ace of hearts by giving it its own suit, and its own value of -1.

It is used like `Suit::HEARTS`

## Deck
There is only a default constructor. It initializes the deck to all 52 cards.

`shuffle` has an optional int parameter to tell it how many times to shuffle. With no parameter, it shuffles once.

`pop_back` returns the last card in the deck and deletes it

`peek_back` returns the last card in the deck but does not delete it

`reset` is the same as the constructor, it deletes the deck and initializes it again to all 52 cards.

`removeCard` takes either a Card or (value, suit). It searches for this card and deletes it if it is found. If the card is not found, it throws an exception.

`findCard` takes either a card of (value, suit). It searches for the card and returns true if it is found, and false otherwise. Only removeCard if you first confirm the card exists with findCard.

The `operator<<` is defined, and it outputs the cards separated by a space, with no newlines.

## Card
### Description
Two ints: (value and suit) run the entire program.

The user probably does not need to interact directly with Cards.

There is a default constructor to initialize the card to -1000, -1000. I probably will remove this, as it makes no sense.

The other constructor takes two parameters: `(value, suit)`

### Methods
The operator<< is defined, and outputs the card in a human-readable format (e.g. King of Hearts).

Copy constructor and assignment operator are implicitly defined.

getValue, getSuit, setValue, setSuit are all defined

operator< and all logical operators are defined

## trumpGlobalVariable
The program keeps track of the trump and suitLed via global variables. Only x45s should update them.