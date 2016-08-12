//Author: Stephen Eldridge
//Tic Tac Toe with AI
//Section 1
//3/10/2016
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
typedef enum turn{
	PLAYER='X', COMP='O'
}turn;

class Game{
	int board;
	turn t;
	char pieces[3]={' ','X','O'};
public:
	Game(){
		board=0;
		t=(rand()%2==0)?PLAYER:COMP;
		cout<<(char)t<<" to move"<<endl;
	}
	void display(){
		cout<<pieces[getVal(0)]<<" |"<<pieces[getVal(1)]<<" |"<<pieces[getVal(2)]<<endl;
		cout<<"__|__|__"<<endl;
		cout<<pieces[getVal(3)]<<" |"<<pieces[getVal(4)]<<" |"<<pieces[getVal(5)]<<endl;
		cout<<"__|__|__"<<endl;
		cout<<pieces[getVal(6)]<<" |"<<pieces[getVal(7)]<<" |"<<pieces[getVal(8)]<<endl;
		cout<<"  |  |  "<<endl;
	}
	//10 comp win, -10 player win, 0 playing, 1 tie, +-11
	int getState(){
		return getGameState(board);
	}
	turn getTurn(){
		return t;
	}
	int getGameState(int a){
		int i;
		for(int j = 0; j<3; j++){
			i=(a%power(3,3*(j+1)))/power(3,3*j);//row win check
			if(i==18+6+2)return 10;
			if(i==9+3+1)return -10;
			if(getVal(j,a)==getVal(3+j,a)&&getVal(3+j,a)==getVal(6+j,a)&&getVal(j,a)!=0){//column win check
				return getVal(j,a)==2?10:-10;
			}
		}
		if(getVal(0,a)==getVal(4,a)&&getVal(4,a)==getVal(8,a)&&getVal(0,a)!=0){//diagonal win check
			return getVal(0,a)==2?10:-10;
		}
		if(getVal(2,a)==getVal(4,a)&&getVal(4,a)==getVal(6,a)&&getVal(2,a)!=0){//other diagonal win check
			return getVal(2,a)==2?10:-10;
		}
		for(i=0;i<9;i++){
			if(getVal(i,a)==0){
				i=0;
				break;
			}
		}
		if(i!=0)return 1;//tie
		return 0;
	}	
	void compMove(){
		int a[9]={-11};
		int i;
		for(i=0; i<9; i++){
			if(getVal(i)==0){
				a[i]=getGameState(setVal(i,board,COMP));//checks if move 'i' is made what the result is
				if(a[i]==2)break;
				if(a[i]==0)a[i]=analyzeSet(setVal(i,board,COMP),PLAYER);//checks if move 'i' is made what following moves are
			}else{
				a[i]=-11;//won't choose filled spots
			}
		}
		int b=0;
		int c = a[0];
		for(i=0;i<9;i++){
			if(c<a[i]){
				b=i;
				c=a[i];
			}
		}
		tryMove(b);	
	}
	int analyzeSet(int b, turn e){
		static int depth = 1;//subtracts from the point value to make the computer win faster/lose slower
		int multi = ((e==COMP)?-1:1);//multiplier for player/computer
		int a[9]={multi*11};
		int i;
		for(i=0; i<9; i++){
			if(getVal(i,b)==0){
				a[i]=getGameState(setVal(i,b,e));
				if(a[i]==(multi*2))break; //stops of there is a win or a loss found
				depth++;
				if(a[i]==0)a[i]=analyzeSet(setVal(i,b,e),(e==PLAYER)?COMP:PLAYER)-multi*depth;//checks next move if game not over
				depth--;
			}else{
				a[i]=multi*11;//won't choose filled spots
			}
		}
		int c = a[0];
		for(i=0;i<9;i++){
			if(e==PLAYER){
				if(c>a[i]){
					c=a[i];
				}
			}else{
				if(c<a[i]){
					c=a[i];
				}
			}
		}
		return c;
	}
	bool tryMove(int a){//returns true if moves
		if(getVal(a)==0&&a<=8&&a>=0){
			board=setVal(a);//makes move
			t=(t==PLAYER)?COMP:PLAYER;//swaps turns
			return true;
		}else if(t==PLAYER){
			cout<<"Invalid move"<<endl;
		}
		return false;
	}
private:
	int getVal(int a,int b) const{//returns the state of cell 'a' in board position 'b'
		return (b%power(3,a+1))/power(3,a);
	}
	int getVal(int a){//returns state of cell 'a' in current position
		return getVal(a,board);
	}
	int setVal(int a){
		return setVal(a,board,t);//returns   current board with move 'a' made by current player
	}
	int setVal(int a, int b,turn c){
		return b+((c==PLAYER)?1:2)*power(3,a);//returns board 'b' with move 'a' made by player 'c'
	}
	int power(int a, int b)const{//a^b
		int c = 1;
		for(int i = 0; i<b; i++){
			c*=a;
		}
		return c;
	}
};

int main(){
	srand(time(NULL));
	Game * game;
	int a;
	char c;
	cout<<"0 |1 |2 "<<endl<<"__|__|__"<<endl<<"3 |4 |5 "<<endl<<"__|__|__"<<endl<<"6 |7 |8 "<<endl<<"  |  |  "<<endl<<endl;
start:
	game = new Game();//allocates memory for game
	while(game->getState()==0){
		game->display();
		if(game->getTurn()==PLAYER){
			cout<<"Your move "<<(char)PLAYER<<endl;
			cin>>a;
			game->tryMove(a);
			
		}else if(game->getTurn()==COMP){
			cout<<"Computer moving"<<endl;
			game->compMove();
		}
	}
	game->display();
	switch(game->getState()){
		case 10:
			cout<<"You Lose"<<endl;
			break;
		case -10:
			cout<<"You Win!"<<endl;
			break;
		case 1:
			cout<<"Tie Game"<<endl;
			break;
		default:
			cout<<"Error"<<endl;
			break;
	}
	delete game; //deletes memory for game
	cout<<"Play again? (y)"<<endl;
	cin>>c;
	if(c=='y')goto start;
	cout<<"Bye"<<endl;
	return 0;
}
