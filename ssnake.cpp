#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

#define U 1
#define D 2
#define L 3
#define R 4

typedef struct snake{
	int x;
	int y;
	struct snake *next;
}snake;

int score=0,add=10;
int HighScore=0;
int status,sleeptime=200;
snake *head,*food;
snake *q;
int endgamestatus=0;
HANDLE hOut;//控制台句柄

int color(int c){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
	return 0;
}

void gotoxy(int x,int y){
	COORD c;
	c.X=x;
	c.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

void printsnake(){
	gotoxy(50,1);
	color(6);
	printf("tcsnake\n");
}

void creatmap(){
	int i,j;
	for(i=0;i<58;i+=2){
		gotoxy(i,0);
		color(5);
		printf("O");
		gotoxy(i,26);
		printf("O");
	}
	
	for(i=1;i<26;i++){
		gotoxy(0,i);
		printf("O");
		gotoxy(56,i);
		printf("O");
	}

	for(i=2;i<56;i+=2){
		for(j=1;j<26;j++){
			gotoxy(i,j);
			color(3);
			printf("A\n\n");
		}
	}
}

void File_out(){
	FILE *fp;
	fp=fopen("save.txt","a+");
	fscanf(fp,"%d",&HighScore);
	fclose(fp);
}

void initsnake(){
	snake *tail;
	int i;
	tail=(snake*)malloc(sizeof(snake));
	tail->x=24;
	tail->y=5;
	tail->next=NULL;
	for(i=1;i<=4;i++){
		head=(snake*)malloc(sizeof(snake));
		head->next=tail;
		head->x=24+2*i;
		head->y=5;
		tail=head;
	}
	while(tail!=NULL){
		gotoxy(tail->x,tail->y);
		color(14);
		printf("⭐");
		tail=tail->next;
	}
}

void creatfood(){
	snake *food_1;
	srand((unsigned)time(NULL));
	food_1=(snake*)malloc(sizeof(snake));
	while((food_1->x%2)!=0){
		food_1->x=rand()%52+2;
	}
	food_1->y=rand()%24+1;
	q=head;
	while(q->next==NULL){
		if(q->x==food_1->x&&q->y==food_1->y){
			free(food_1);
			creatfood();
		}
		q=q->next;
	}
	gotoxy(food_1->x,food_1->y);
	food=food_1;
	color(12);
	printf("●");
}

void scoreandtips(){
	File_out();
	gotoxy(80,4);
	color(11);
	printf("The highest score：%d",HighScore);
	gotoxy(80,8);
	color(14);
	printf("your score: %d",score);
	color(13);
	gotoxy(89,11);
	printf("some tips");
	gotoxy(76,13);
	color(6);
	printf("-----------------------");
	gotoxy(76,25);
	printf("-----------------------");
	color(3);
	gotoxy(80,14);
	printf("each food score: %d",add);
	gotoxy(80,16);
	printf("you cannot knock the wall and yourself");
	gotoxy(80,18);
	printf("you can control the direction with:ydrl");
	gotoxy(80,20);
	printf("F1 for speed up,F2 for speed down");
	gotoxy(80,22);
	printf("space : stop the game");
	gotoxy(80,24);
	printf("ESC : exit the game");
}



int bitself(){
	snake *self;
	self=head->next;
	while(self!=NULL){
		if(self->x==head->x&&self->y==head->y){
			return 1;
		}
		self=self->next;
	}
	return 0;
}

void cantcrosswall(){
	//if it knocked the wall
	if(head->x==0||head->y==26||head->x==56||head->y==0){
		endgamestatus=1;
	}
}

void speedup(){
	if(sleeptime>=50){
		sleeptime-=10;
		add+=2;
		if(sleeptime==320){
			add=2;
		}
	}
}

void speeddown(){
	if(sleeptime<350){
		sleeptime=sleeptime+30;
		add-=20;
		if(sleeptime==350){
			add=1;
		}
	}
}

void snakemove(){
	snake *nexthead;
	cantcrosswall();
	nexthead=(snake*)malloc(sizeof(snake));
	if(status==U){
		nexthead->x=head->x;
		nexthead->y=head->y-1;
		nexthead->next=head;
		head=nexthead;
		q=head;
		if(nexthead->x==food->x&&nexthead->y==food->y){
			while(q!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q-q->next;
			}
			score=score+add;
			speedup();
			creatfood();
		}
		else{
			while(q->next->next!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q=q->next;
			}
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("A");
			free(q->next);
			q->next=NULL;
		}
	}

	if(status==D){
		nexthead->x=head->x;
		nexthead->y=head->y+1;
		nexthead->next=head;
		head=nexthead;
		q=head;
		if(nexthead->x==food->x&&nexthead->y==food->y){
			while(q!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q-q->next;
			}
			score=score+add;
			speedup();
			creatfood();
		}
		else{
			while(q->next->next!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q=q->next;
			}
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("A");
			free(q->next);
			q->next=NULL;
		}
	}

	if(status==L){
		nexthead->x=head->x-2;
		nexthead->y=head->y;
		nexthead->next=head;
		head=nexthead;
		q=head;
		if(nexthead->x==food->x&&nexthead->y==food->y){
			while(q!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q-q->next;
			}
			score=score+add;
			speedup();
			creatfood();
		}
		else{
			while(q->next->next!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q=q->next;
			}
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("A");
			free(q->next);
			q->next=NULL;
		}
	}

	if(status==R){
		nexthead->x=head->x+2;
		nexthead->y=head->y;
		nexthead->next=head;
		head=nexthead;
		q=head;
		if(nexthead->x==food->x&&nexthead->y==food->y){
			while(q!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q-q->next;
			}
			score=score+add;
			speedup();
			creatfood();
		}
		else{
			while(q->next->next!=NULL){
				gotoxy(q->x,q->y);
				color(14);
				printf("⭐");
				q=q->next;
			}
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("A");
			free(q->next);
			q->next=NULL;
		}
	}

	if(bitself()==1){
		endgamestatus=2;
		
	}

}

void keyboardControl(){
	status=R;
	while(1){
		scoreandtips();
		if(GetAsyncKeyState(VK_UP)&&status!=D){
			status=U;
		}
		else if(GetAsyncKeyState(VK_DOWN)&&status!=U){
			status=D;
		}
		else if(GetAsyncKeyState(VK_RIGHT)&&status!=L){
			status=R;
		}
		else if(GetAsyncKeyState(VK_LEFT)&&status!=R){
			status=L;
		}

		if(GetAsyncKeyState(VK_SPACE)){
			while(1){
				Sleep(300);
				if(GetAsyncKeyState(VK_SPACE)){
					break;
				}
			}
		}

		else if(GetAsyncKeyState(VK_ESCAPE)){
			endgamestatus=3;
			break;
		}
		else if(GetAsyncKeyState(VK_F1)){
			speedup();
		}
		else if(GetAsyncKeyState(VK_F2)){
			if(sleeptime<350){
				sleeptime+=30;
				add-=2;
				if(sleeptime==350){
					add=1;
				}
			}
		}

		Sleep(sleeptime);
		snakemove();
	}
}

void welcometogame(){
	int n;
	int i,j;
	gotoxy(43,18);
	color(11);
	printf("贪吃蛇小游戏\n");
	color(14);
	for(i=20;i<=26;i++){
		for(j=27;j<=74;j++){
			gotoxy(j,i);//为什么不是gotoxy(i,j)
			if(i==20||i==26){
				printf("-");
			}
			else if(j==27||j==74){
				printf("|");
			}
		}
	}
	color(12);
	gotoxy(35,22);
	printf("1.start game");
	gotoxy(55,22);
	printf("2.help");
	gotoxy(35,24);
	printf("3.exit");
	gotoxy(29,27);
	color(3);
	printf("please choose[1 2 3]:[ ]\b\b");
	color(14);
	scanf("%d",&n);
	switch(n){
	case 1:
		system("cls");
		creatmap();
		initsnake();
		creatfood();
		keyboardControl();
		break;
	case 2:
		scoreandtips();
		break;
	case 3:
		exit(0);
		break;
	default:
		color(12);
		gotoxy(40,28);
		printf("please choose the number within 1 2 3");
		getch();//输入任意键
		system("cls");//清屏
		printsnake();
		welcometogame();
	}
}


int main(){
	//system("mode con cols=10 lines=30");//规定屏幕大小
	printsnake();
	welcometogame();
	File_out();
	keyboardControl();
	return 0;
}