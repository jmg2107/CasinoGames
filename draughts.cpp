#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "draughts.h"

using namespace std;

int main(void){

    //initialize an empty board and game token to play
    char Gameboard[8][8];
    char in_play=1, get_move, comp_turn;

        

    //initialize the pieces of the two players;
    vector <gamepiece*> ai;
    vector <gamepiece*> player;
    string current_position, next_position;
    position current_pos, next_pos;

    initialize_board(player, ai, Gameboard);
    cout << "..........................\n";
    cout << "Let's Play Draughts!" << endl;
    cout << "O's are the A.I., You are X's" << endl;
    cout << "Enter moves by typing in Grid positions. \n";
    cout << "Positions are case-sensitve.\n";

    while(in_play){
        
        //reprint the board after every move and await a new move 
        print_board(Gameboard);

        get_move = 1;
        comp_turn = 1;
        
        //stay in this loop till we get a valid move
        while(get_move){
            bool valid=0;
            cout << "Select Position of Piece to Move: ";
            cin >> current_position;

            current_pos =  parse_move(current_position);
            //cout << current_pos.x << current_pos.y << endl;

            cout <<  "Move to: ";
            cin >> next_position;
            next_pos = parse_move(next_position);
            //cout << next_pos.x << next_pos.y << endl;
            valid = valid_move(current_pos, next_pos, player, ai, Gameboard);
            if (((current_pos.x==10) && (current_pos.y=10)) || 
               ((next_pos.x==10) && (next_pos.y=10))) {
                continue;
            } else if (valid==0){
                cout << "Please try again. \n";
                continue;
            }
            else{
                cout << "Move accepted!" << endl;
                get_move = 0;
                comp_turn = 0;
            }
        }
        
        while(comp_turn==0){
            comp_turn = ai_move(player, ai, Gameboard);
        }

        in_play = !win_or_draw(player, ai, Gameboard);

    }
    
    
    return 0;
}

