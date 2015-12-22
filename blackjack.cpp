#include <iostream>
#include <string>
#include <math>
#include <stdlib>
#include <vector>

#define ACE 11
#define JACK 10
#define	QUEEN 10 
#define KING 10

enum suite_t {HEARTS, CLUBS, SPADES, DIAMONDS};

class card{
	
	public:
	int value;
	suite_t suite;
};

//function for random draw
card random_value_draw(void){
	
	int result;
	card new_card;	
	suite_t new_suite;
	
	result = rand() % 11 + 1;
	
	if(result < 10){
		new_card.value = result;
	} elseif (result == 10) {
		
		result = rand()%2;
		
		switch(result){
			case 0:
				new_card.value = JACK;
				break;
			case 1:
				new_card.value = QUEEN;
				break;
			case 2:
				new_card.value = KING;
				break;
			default:
				new_card.value = 10;
				break;
		}
	} else {
		new_card.value = ACE;
	}

	new_suite = static_cast<suite_t>(rand()%DIAMONDS);
	
	new_card.suite = new_suite;

	return new_card;	

}

int get_bet(int money) {
	
	int new_bet;
	
	cout << "You have $" << money << ". Enter bet:";
	cin >> new_bet;

	if(new_bet > money){
		get_bet(money);
	} else {
	return new_bet;
	}
}

int win_or_lose(vector<card> dealer, vector<card> player){
	
	//if we return 1, then player wins
	//if we return 0, then player loses
	//if we return 2, then it's a draw.

	int win = -1;
	int player_tally, dealer_tally = 0;

	cout << "Dealer's Cards are: " << endl;
	
	for(int i=0; i<dealer.size(); dealer++){
	
		cout << dealer[i].value << " of " << dealer[i].suite << endl;
	}

	while(!player.empty()){
		player_tally += player.back().value;
		player.pop_back();
	}

	while(!dealer.empty()){
		dealer_tally += dealer.back().value;
		dealer.pop_back();
	}

	if( (player_tally > dealer_tally) && (player_tally <= 21)) {
		win = 1;
		cout << "Player Wins!" << endl;
	} elseif (player_tally < dealer_tally) {
		win = 0;
		cout << "Dealer wins. Better luck next time. " << endl;
	} elseif (player_tally > 21) {
		win = 0;
		cout << "Player busts.  Better luck next time. " << endl;
	} elseif ((player_tally==dealer_tally) && (player_tally <= 21)){
		win = 2; 
		cout << "It's a draw! Nice playing. " << endl;
	}

}
		

int main(void){

	//variables
	int money = 500;
	vector<card> current_hand;
	vector<card> dealer_hand;
	int dealer_tally, bet = 0;
	int reckon = -1;
	char play;


	while(money > 0){

		bet = get_bet(money);

	//deal
		cout << "Your hand is "	;	

		for(int i=0; i<2; i++){
			current_hand.push_back(random_value_draw());
			dealer_hand.push_back(random_value_draw());
			dealer_tally += dealer_hand.back().value;
			cout << current_hand[i].value << " of " << current_hand[i].suite << endl;
		}

	//play
		
		cout << "Would you like another card? (y/n)" << endl;
		cin >> play;
		
		if(play == "y"){
			current_hand.push_back(random_value_draw());
			if(dealer_tally <= 16)
				dealer_hand.push_back(random_value_draw());
				dealer_tally += dealer_hand.back()value;
			}

		} else {
			reckon = win_or_lose(dealer_hand, current_hand);
		}	
	
	//reckoning
	switch(reckon) {
		
		case 0:
		//player loses
			money =- bet;
			break;
		
		case 1:
		//player wins
			money =+ bet;
			break;

		case 2:
		// it's a draw
			break;
		
		default:
			break;
	}
	
	//reset variables
	bet = 0;
	dealer_tally = 0;
	reckon = -1;

	

	}

	cout << "Game Over" << endl;

	return 0;



}
