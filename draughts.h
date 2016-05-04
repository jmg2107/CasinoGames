#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//declare and initialize letter array for grid
const string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};



//desginate the type of player
enum player_t{
    computer,
    human
    };

struct position{
    int x,y; // position of the player on the board
};

struct gamepiece{
    player_t player_type; 
    position current_position; //current position on the board
    char value; //how the piece is represented on the board, X=human, O=comp
};

//print the board 
void print_board(char board[8][8]){
    
    //iterate through the board from top to bottom, left to right
    for(int j=8; j>=0; j--){
       
        
        //print out the letter guide numbers on the bottom
        if(j==0){
            cout << " ";
            for(int k=0; k<8; k++){

                cout << " " << letters[k];
            }
            cout << endl;
            break;
        } else {
            cout << j; //at the beginning of each row, print the row #
        }

        for(int i=0; i<8; i++){ //print out the value of that space on the board
            cout << " " << board[i][j-1];
            if(i==7){
                cout << endl;
            }
        }
    }

}

void initialize_board(vector <gamepiece*>& player, vector <gamepiece*>& ai, char board[8][8]){
   
   
   //set an empty board with all the spaces
   for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                board[i][j] = '.';
            }
    }
 
    
    //set the type and positions of the player pieces
    for(int j=0; j<3; j++){
        
        if(j%2==0){
            for(int i=0; i<8; i+=2){
                gamepiece* temp = new gamepiece;
                temp->player_type=human;
                temp->current_position.x = i;
                temp->current_position.y = j;
                temp->value =  'X'; 
                player.push_back(temp);
                board[i][j] = 'X';
            }
        } else {
            for(int i=1; i<9; i+=2){
                gamepiece* temp = new gamepiece;
                temp->player_type=human;
                temp->current_position.x = i;
                temp->current_position.y = j;
                temp->value = 'X';
                player.push_back(temp);
                board[i][j] = 'X';
            }
        }
    }

    //set the type and positions of the ai pieces
    for(int j=7; j>4; j--){
        
        if(j%2==0){
            for(int i=0; i<8; i+=2){
                gamepiece* temp = new gamepiece;
                temp->player_type=computer;
                temp->current_position.x = i;
                temp->current_position.y = j;
                temp->value = 'O';
                ai.push_back(temp);
                board[i][j] = 'O';
            }
        } else {
            for(int i=1; i<9; i+=2){
                gamepiece* temp = new gamepiece;
                temp->player_type=computer;
                temp->current_position.x = i;
                temp->current_position.y = j;
                temp->value = 'O';
                ai.push_back(temp);
                board[i][j] = 'O';
            }
        }
    }

}

//this function will parse the player move input string and 
//convert it to positions
position parse_move(string player_move){
    
    string x_pos, y_pos;
    int x_position = -1, y_position;
    position pos;
    pos.x = 10;
    pos.y = 10;

    x_pos = player_move.substr(0,1);
    for(int i=0; i<8; i++){
        if(x_pos.compare(letters[i])==0){
            x_position = i;
            break;
        }   
    }
    if(x_position == -1){
            cout << x_pos << " isn't a valid column letter." << endl;
            cout << "Please try again";
            return pos;
    }


    y_pos = player_move.substr(1,1);
    y_position = stoi(y_pos);

    //do a quick sanity check for y_position
    if(y_position > 8 || y_position < 1){
        cout << y_position << "isn't a valid row number." << endl;
        cout << "Please try again";
        return pos;
    }

    pos.x = x_position;
    pos.y = y_position - 1;

    return pos;

}

//function used to move a piece and update vectors and board
bool move_piece(position old, position next, vector <gamepiece*>& player, int index, char board[8][8]){
    
    int success = 0;

    //Validate that the space we're moving to is empty
    if (board[next.x][next.y] != '.'){
        cout << "There is already a game piece at the space you're moving to.\n";
        return success;
    }
    
    //Update the vectors and the board
    player[index]->current_position.x = next.x;
    player[index]->current_position.y = next.y;
    cout << "Token moved to " << letters[player[index]->current_position.x] << player[index]->current_position.y + 1 << endl;
    board[next.x][next.y] = 'X';
    board[old.x][old.y] = '.';
    success = 1;
    return success;

}

//function used to capture a piece and update vectors and board
//args:
//positions of old to next
//vectors of gamepieces
//index of piece we're moving
//global board
//player_move = flag to see if this is a player_move or an ai_move
bool capture_piece(position old, position next, vector <gamepiece*>& offensive, int index, vector <gamepiece*>& defensive, char board[8][8], int player_move){
    
    position capture;
    int token_found=0, def_index=0, success=0;


    //Validate that the space we're moving to is empty
    if (board[next.x][next.y] != '.'){
        cout << "There is already a game piece at the space you're moving to.\n";
        return success;
    }

    //Validate that we're capturing a piece
    if (next.x > old.x){
        capture.x = old.x+1;
    } else { 
        capture.x = old.x-1;
    }
    if(player_move){
        capture.y = old.y+1;
    } else {
        capture.y = old.y-1;
    }

    for (int i=0; i< defensive.size(); i++){
       // cout << "capture pos= " << capture.x << capture.y << endl;
        if((capture.x==defensive[i]->current_position.x)&&
          (capture.y==defensive[i]->current_position.y)){
            token_found = 1;
            def_index = i;
            cout << "Defensive Token found!" << endl;
            break;
        }
    }

    if(token_found == 0){
        cout << "There was no piece to capture" << endl;
        return success;
    } 

    //Update the vectors and the board
    offensive[index]->current_position.x = next.x;
    offensive[index]->current_position.y = next.y;
    cout << "Token moved to " << letters[offensive[index]->current_position.x] << offensive[index]->current_position.y+1 << endl;
    cout << "Enemy Piece Captured! \n";
    if(player_move){
        board[next.x][next.y] = 'X';
    } else {
        board[next.x][next.y] = 'O';
    }

    board[old.x][old.y] = '.';

    delete defensive[def_index];
    defensive.erase(defensive.begin() + (def_index));
    board[capture.x][capture.y] = '.';
    success = 1;
    return success;

}
//if this is a valid move, return true
bool valid_move(position old, position next, vector <gamepiece*>& player, vector <gamepiece*>& comp, char board[8][8]){
  
    bool valid = 0;
    bool token_found = 0;
    int player_index = 0;
    int player_move = 1;

    //cout << "size of player vector= " << player.size() << endl;
    //is there a game piece at the old position
    for (int i=0; i<player.size(); i++){
        if((old.x==player[i]->current_position.x)&&
          (old.y==player[i]->current_position.y)){
            token_found = 1;
            player_index = i;
            cout << "Token found!" << endl;
            break;
        }
    }
    if(token_found==0){
        cout << "There is no game piece at that Position" << endl;
        return valid;
    }

    //Process the case for a regular move
    if((((old.x+1)== next.x) ||  ((old.x-1)== next.x)) && ((old.y + 1)==next.y)){
        valid = move_piece(old, next, player, player_index, board);
        return valid;
    }

    //Process the case for a capture
    else if((((old.x+2)== next.x) ||  ((old.x-2)== next.x)) && ((old.y + 2)==next.y)){
        valid = capture_piece(old, next, player, player_index, comp, board, player_move);
        return valid;

    }

    //This is not a normal move
    else {
        cout << "This isn't a valid move.  Please try again.\n";
        return valid;

    }
}
bool ai_move(vector <gamepiece*>& player, vector <gamepiece*>& comp, char board[8][8]){
        
        bool move_done=0;
        int comp_size, can_move, do_capture, comp_index, seed;
        position new_pos, old_pos, cap_pos;

        comp_size = comp.size();
        can_move=0, do_capture=0;
        seed = rand() % 100; //with every move generate a new random seed.

       // cout<< "size of computer array is " << comp_size << endl;

        for(int i=0; i<comp_size; i++){
           old_pos.x = comp[i]->current_position.x; 
           old_pos.y = comp[i]->current_position.y;  
           // cout << "computer " << i << "=" << comp[i]->current_position.x << comp[i]->current_position.y << endl;
           //if the seed is even, move right, else move left.
           //cout << "seed = "<< seed << endl;
           if(seed%2==0){
            new_pos.x = comp[i]->current_position.x + 1;
           } else {
            new_pos.x = comp[i]->current_position.x - 1;
           }
            new_pos.y = comp[i]->current_position.y - 1;

            //this is a valid move, check for an empty space and do a range check
            if((board[new_pos.x][new_pos.y] == '.') &&
            ((new_pos.x >=0) && (new_pos.x <=7)) &&
             ((new_pos.y >=0) && (new_pos.y <=7))){
                comp_index = i;
                can_move = 1;
                break;
            }
            //if the seed is odd, try a capture
            if(seed%2 != 0){
                new_pos.y = comp[i]->current_position.y - 2;
                cap_pos.y = comp[i]->current_position.y - 1;

                if(seed>50){
                    new_pos.x = comp[i]->current_position.x + 2;
                    cap_pos.x = comp[i]->current_position.x + 1;
                }
                else{
                    new_pos.x = comp[i]->current_position.x - 2;
                    cap_pos.x = comp[i]->current_position.x - 1;
                }
                
                //check to see if we're jumping over a player and do a range check
                if((board[cap_pos.x][cap_pos.y] == 'X') &&
                ((new_pos.x >=0) && (new_pos.x <=7)) &&
                ((new_pos.y >=0) && (new_pos.y <=7))){
           
                    do_capture = capture_piece(old_pos, new_pos, comp, i, player, board, 0);
                    if(do_capture==1){
                        move_done = 1;
                        return move_done;
                    }
                }
            }
        }

        if(can_move==0){
            //cout << "There are no valid moves left for the computer. \n";
            return move_done;
        }

        //move the piece
        if((can_move==1)&&(do_capture==0)){
            
            comp[comp_index]->current_position.x = new_pos.x;
            comp[comp_index]->current_position.y = new_pos.y;
            board[old_pos.x][old_pos.y] = '.';
            board[new_pos.x][new_pos.y] = 'O';
            move_done = 1;
            cout << "Computer moved to " << letters[comp[comp_index]->current_position.x] << comp[comp_index]->current_position.y+1 << endl;
            return move_done;
        }
}

//helper function to see if there are free spaces available to that vector
bool can_we_move(vector <gamepiece*>& sample, char board[8][8], int player){
    
    position temp1, temp2, temp3, temp4, cap3, cap4;
     
    bool we_can_move=1;

    for(int i=0; i<sample.size(); i++){
            if(sample[i]->current_position.y==7){ //this piece can't move
                continue;
            }
            else{ //check to see if this piece can move to the right or left. or if we can capture
                temp1.x = sample[i]->current_position.x+1;
                temp2.x = sample[i]->current_position.x-1;
                temp3.x = sample[i]->current_position.x+2;
                cap3.x = temp1.x;
                temp4.x = sample[i]->current_position.x-2;
                cap4.x = temp2.x;
                if(player){
                    temp1.y = sample[i]->current_position.y+1;
                    temp3.y = sample[i]->current_position.y+2;
                    cap3.y = temp1.y;
                } else{ 
                    temp1.y = sample[i]->current_position.y-1;
                    temp3.y = sample[i]->current_position.y-2;
                    cap3.y = temp1.y;
                }

                temp2.y = temp1.y;
                temp4.y = temp3.y;
                cap4.y = cap3.y;

                //first two cases check if we can move to the right or the left
                if((board[temp1.x][temp1.y]=='.') && 
                    ((temp1.x >= 0) || (temp1.x < 8)) &&
                    ((temp1.y >= 0) || (temp1.y < 8))){
                    return we_can_move;
                
                } else if((board[temp2.x][temp2.y]=='.') && 
                    ((temp2.x >= 0) || (temp2.x < 8)) &&
                    ((temp2.y >= 0) || (temp2.y < 8))){
                    return we_can_move;
                
                } //next two cases check to see if anyone can be captured 
                
                else if((board[temp3.x][temp3.y]=='.') && 
                    ((temp3.x >= 0) || (temp3.x < 8)) &&
                    ((temp3.y >= 0) || (temp3.y < 8))){
                    if(player){
                        if(board[cap3.x][cap3.y] == 'O'){
                            we_can_move = 1;
                            return we_can_move;
                        }
                    } else {
                        if(board[cap3.x][cap3.y] == 'X'){
                            we_can_move = 1;
                            return we_can_move;
                        }
                    }

                   continue;     
                
                } else if((board[temp4.x][temp4.y]=='.') && 
                    ((temp4.x >= 0) || (temp4.x < 8)) &&
                    ((temp4.y >= 0) || (temp4.y < 8))){

                    if(player){
                        if(board[cap4.x][cap4.y] == 'O'){
                            we_can_move = 1;
                            return we_can_move;
                        }
                    } else {
                        if(board[cap4.x][cap4.y] == 'X'){
                            we_can_move = 1;
                            return we_can_move;
                        }
                    }

                   continue;
                
                }
                else {
                    continue;
                }
            }

        }
    we_can_move=0;
    return we_can_move;
}

bool win_or_draw(vector <gamepiece*>& player, vector <gamepiece*>& comp, char board[8][8]){
    
    int game_over=0, player_can_move=0, comp_can_move=0;

    if(player.size()==0){
        cout << "Computer won. Better luck next time. \n";
        game_over=1;
        return game_over;
    } else if(comp.size()==0){
        cout << "You Won!!  Nice Job!. \n";
        game_over = 1;
        return game_over;
    } else {
        
        //Are there still places to move
        player_can_move = can_we_move(player, board, 1);
        comp_can_move = can_we_move(comp, board, 0);  
    
        if((player_can_move==0) || (comp_can_move==0)){
                cout << "It's a Draw! \n";
                game_over = 1;
                return game_over;
        }
    }

   game_over = 0;

   return game_over; 


}
