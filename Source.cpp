
#include <iostream>
#include <stdio.h>
#include<thread>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


#include<random>

#include<string>

using std::string;

using std::cout;

int startI();

double gravity = 3500;

long long maxX, minX;
int offSet = 0;

#define ScreenWidht 16*32
#define ScreenHeight 600
double dT = 0.05;
int defualtGroundY;

int tempWidht = 15*32;
void draw();

int stoneNumber = 88;
int bushNumber = 3;
int cloudSize = 15;
int hillNumbers = 5;
int baseNumbers = 18 * 15; // 200 ScreenS
int questionNumbers = 13;
int brickNumber = 30;
int pipeNumber = 6;
int enemyNumber = 14;

ALLEGRO_SAMPLE *theme;
ALLEGRO_SAMPLE *jump;
ALLEGRO_SAMPLE *gOver;
ALLEGRO_SAMPLE *win;
ALLEGRO_SAMPLE *brickBreak;
ALLEGRO_SAMPLE *brickUnBreak;
ALLEGRO_SAMPLE *coin;
ALLEGRO_SAMPLE *bump;

void onlyDraw();

int bwallWidth ,bwallHeight ;
enum types{
	gameobject = 0,
	enemy = 2,
	_pipe = 4,
	question = 8,
	brick = 16,
	stone = 32,
	basewall = 64,
	supermario = 128,
	hill = 256,
	shill = 512,
	bush = 1024,
	sbush = 2048,
	cloud = 4096

};

class gameObject{

public:



	types RealType;
	bool ColisionAble;
	int groundY;
	bool isGravitical;
	string *states;
	int maxStates;
	int curState;
	ALLEGRO_BITMAP *pic;
	bool isEnabe;
	int x, y;
	long long  realX; // realX = Postion of first dastgah mokhtasat
	int width, height;
	double dx, dy;
	bool isInRange;
	double vx , vy;
	gameObject(bool Col = false ,int X = -1,int Y= -1)
	{
		RealType = gameobject;
		ColisionAble = Col;
		isGravitical = false;
		groundY = defualtGroundY; // defualt GroundY;
		isEnabe = true;
		x = realX = X;
		y = Y;
		dx = dy = 0;
		vx = vy = 0;
		isInRange = false;
	}


	void update(){
		if (!isEnabe)
		{
			isInRange = false;
			return;
		}
		if(isGravitical) // object effects by gravity
		{
			if(y + height < groundY) // Object Above Ground
			{

				dy = ( (.5*gravity*dT*dT + vy * dT ) / 3 ) * height;
			}
			if (y + height + dy >= groundY)
				y = groundY -height;
			else
				y += dy;

		}

		x = realX - offSet;
		realX += dx;








		if( x + width >= minX && x - width <=maxX && isEnabe){

			al_draw_bitmap(pic, x, y, 0);
			isInRange = true;
		}
		else
			isInRange = false;

	}

	void drawObject(){
		if(isInRange)
			al_draw_bitmap(pic, x, y, 0);
	}

	void gotoRight()
	{
		vx = 150;
	}
	void gotoLeft()
	{
		vx = -150;
	}

	//bool ColisionUP(gameObject *other)
	//{
	//	int y2 = other->y ;
	//	if ( y  > y2 || ( y + other->height > y2) )
	//		return false;
	//	for (int i = other->x; i <other->x + other->width; i++)
	//	{
	//		for (int j =x; j <x+ width; j++)
	//		{
	//
	//		if (j == i )
	//			return true;
	//		}
	//	}
	//	return false;
	//}

	//bool ColisionDown(gameObject *other)
	//{
	//
	//	int y2 = other->y ;
	//	if ( (  y + height  < y2  ||   ( y + height  + other->height < y2) ))
	//		return false;
	//	for (int i = other->x; i <other->x + other->width; i++)
	//	{
	//		for (int j =x; j <x+ width; j++)
	//		{
	//
	//		if (j == i )
	//			return true;
	//		}
	//	}
	//	return false;



	//	}
	bool Colision(gameObject *other)
	{

		if(!ColisionAble)
			return false;

		if ( y + height < other->y)
			return false;


		if ( y >= other->y + other->height)
			return false;
		for (int i = other->x ; i <other->x + other->width; i++)
		{
			for (int j =x ; j <x+ width; j++)
			{

				if (j == i  )
				{
					cout << RealType <<" colided with "<< other->RealType<<" with address of "<< other<<std::endl;
					return true;

				}
			}
		}
		return false;
		/*
		double d = sqrt( ( (double(x) - other->x)*(double(x) - other->x)  + (double(y) - other->y) * (double(y)  - other->y)  ) );
		double a = ( width > height)? height : width;
		double c = ( other->width > other->height)? other->height : other->width;

		if ( a + c  > d )
		return true;
		return false;*/
	}



};


class EndFlag : public gameObject{
public:
	EndFlag(int X) : gameObject(true){
		RealType = _pipe;
		pic = al_load_bitmap("media//ending.bmp");
		x = X;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);
		y = defualtGroundY - height;
	}


}* endFlag;



class Enemy : public gameObject{
public:
	bool startRunnig;
	bool nowDestruyed;
	int curState ;
	Enemy(int X,int Y) : gameObject(true){
		isGravitical = true;
		RealType = enemy;
		pic = al_load_bitmap("media//enemy_state1.png");
		x = X;
		y = Y;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);
		curState = 1;
		nowDestruyed = false;
		startRunnig = false;
	}

	void update(){
		if(nowDestruyed)
		{


			ColisionAble = false;
			x = realX - offSet;


			pic = al_load_bitmap("media//enemy_dead.bmp");
			al_draw_bitmap(pic, x, y, 0);
			curState++;
			if(curState==20)
				nowDestruyed = false;
			return;
		}

		if (!isEnabe)
		{
			isInRange = false;
			return;
		}





		if( x + width >= minX && x - width <=maxX && isEnabe){
			startRunnig = true;
			bool changeFlag = false;
			if(curState ==1 || curState ==2 ){
				pic = al_load_bitmap("media//enemy_state1.png");
				curState++;
				changeFlag = true;
			}
			else if(curState ==3|| curState ==4  && !changeFlag)
			{
				curState++;
				if(curState ==5)
					curState =1;
				pic = al_load_bitmap("media//enemy_state2.png");

			}
			al_draw_bitmap(pic, x, y, 0);
			isInRange = true;
		}
		else{
			isInRange = false;

			x = realX - offSet;
			if (startRunnig){

				dx = vx *dT;
				x = realX - offSet;
				realX += dx;
			}
			if ( x + ScreenWidht < 0)
				isEnabe = false;
			return;
		}


		dx = vx *dT;
		x = realX - offSet;
		realX += dx;




		if(isGravitical) // object effects by gravity
		{
			if(y + height < groundY) // Object Above Ground
			{

				dy = (  (.5*2000*dT*dT + vy * dT ) / 3 ) * height;
			}
			if (y + height + dy >= groundY)
				y = groundY -height;
			else
				y += dy;

		}


	}

	void Destroy()
	{

		al_play_sample(bump,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
		y += height ;
		y -= 12 ;
		cout<<"Enemy Destroyed\n";
		curState = 0;
		nowDestruyed= true;
		isEnabe = false;
		isInRange =false;
	}


}** enemies;

class Pipe : public gameObject{
public:
	Pipe(int X,int Y) : gameObject(true){
		RealType = _pipe;
		pic = al_load_bitmap("media//pipe.bmp");
		x = X;
		y = Y;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);
	}


}** pipes;





class Question : public gameObject{
public:
	bool isDestoyed;
	bool isMushrom;
	Question(int X,int Y) : gameObject(true){
		RealType = question;
		isMushrom = false;
		isDestoyed = false;
		pic = al_load_bitmap("media//question.jpg");
		x = X;
		y = Y;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);
	}

	void Destroy(){
		if (isDestoyed)
		{

			al_play_sample(brickUnBreak,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);

			return;
		}
		isDestoyed = true;

		al_play_sample(coin,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);


		pic = al_load_bitmap("media//hard_rock.bmp");
		if(pic == 0)
			cout<<"NO PIC";

		if( !isMushrom)
		{
			ALLEGRO_BITMAP *temp = al_load_bitmap("media//reward_coint.bmp");
			draw();
			al_draw_bitmap(temp,x + width/3 , y - al_get_bitmap_height(temp),0);
			al_flip_display();
			al_rest(dT/4);

			al_draw_bitmap(temp,x+  width/3, y - al_get_bitmap_height(temp),ALLEGRO_FLIP_HORIZONTAL);
			al_flip_display();
			al_rest(dT/4);
			al_draw_bitmap(temp,x + width/3, y - al_get_bitmap_height(temp),0);
			al_flip_display();
			al_rest(dT/4);

			al_draw_bitmap(temp,x + width/3, y - al_get_bitmap_height(temp),ALLEGRO_FLIP_HORIZONTAL);
			al_flip_display();
			al_rest(dT/4);
		}
	}

}** questions;



class Brick : public gameObject{
public:
	bool destroy;
	int x1, x2 ,rx1,rx2;
	int y1,y2;
	Brick(int X,int Y) : gameObject(true){
		RealType = brick;
		pic = al_load_bitmap("media//brick.bmp");
		x = X;
		y = Y;
		destroy = false;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);


	}

	void Destroy(){

		al_play_sample(brickBreak,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);

		destroy = true; vx = 130 ; vy = 600; ColisionAble = false; pic = al_load_bitmap("media//breaked_break.bmp");
		rx1 = rx2 = realX;
		x1 = width+ (x2= x);
		y1 = height + y;
		isEnabe = false;
		y2 = y;
	};

	void Update()
	{


		if(destroy){

			dx = vx*dT;
			rx1 += dx;
			rx2 -= dx;
			x1 = rx1 - offSet;
			x2 = rx2 - offSet;

			dy = ( -(.5*gravity*dT*dT + vy * dT ) / 3 ) ;
			y1 += dy;
			y2 += dy;


			al_draw_bitmap(pic,x1 , y1,0);
			al_draw_bitmap(pic, x2 , y1,0);
			al_draw_bitmap(pic,x1, y2,0);
			al_draw_bitmap(pic, x2 , y2,0);

			if ( y2 >=ScreenHeight )
				destroy = false;

			vy -= gravity * dT ;
		}

	}

}**bricks;

class Stone : public gameObject{
public:
	Stone(int X,int Y) : gameObject(true){
		RealType = stone;
		pic = al_load_bitmap("media//stone.bmp");
		x = X;
		y = Y;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);
	}


}** stones;


class baseWall : public gameObject{
public:


	baseWall(int X,int Y) : gameObject(false,X,Y){


		RealType = basewall;
		pic = al_load_bitmap("media//newBase2.bmp");
		if(pic == NULL)
		{

			cout<<"coudnt find ";
		}else{
			width = al_get_bitmap_width(pic);
			height = al_get_bitmap_height(pic);
		}}


}** baseWalls;

class superMario : public gameObject{

public:

	int maxVx;
	int maxVy ;
	double T;
	bool startJumping;
	bool rotateLeft;
	bool isRunState;
	int j;
	superMario() :gameObject(true)
	{
		isInRange =true;
		RealType = supermario;
		T = 0;
		startJumping = false;
		rotateLeft = false;
		maxVx = 300;
		vx =0;
		isEnabe = true;
		isRunState = false;
		curState = 0;
		// toDo jumpStates
		maxStates = 6;
		states = new string[maxStates];

		states[0] = "media//mario_small//m6.png";
		states[1] = "media//mario_small//m1.png";
		states[2] = "media//mario_small//m2.png";
		states[3] = "media//mario_small//m3.png";
		states[4] = "media//mario_small//m4.png";
		states[5] = "media//mario_small//m5.png";

		j = 0;
		width =30;
		height = 38;

		x = realX = width * 3;
		maxVy =  1200 ;
		vy = 0;
		y  = defualtGroundY - height;

	}

	void update(){


		bool flag = false;
		//		if ( startJumping)
		//			groundY = defualtGroundY;
		if( y + height < groundY
			|| startJumping
			)
		{
			if(y + height >= groundY && vy != maxVy)
			{

				startJumping = false;
				vy = 0;
				T = 0;
			}else{

				T += dT;

				dy = ( -1*(   .3*-1*gravity*dT*dT + vy * dT ) )   ;

				superMario m(*this);
				m.y = y + dy ;

				int t = startI();

				if (m.y + height >= defualtGroundY) // cheking Ground Colision
				{
					bool endFlag = true;
					for (int i = t; i < t+ 18 ; i++)
					{
						baseWall *b = baseWalls[i];
						if (m.Colision(b) && b->isEnabe )
						{
							endFlag = false;
							groundY = defualtGroundY;
							break;
						}

					}
					if(endFlag)
						GameOver();
				}
				// Enemy Colisions
				for (int i = 0; i < enemyNumber; i++)
				{
					Enemy *b = enemies[i];
					if(b->isInRange && b->isEnabe)
						if(m.Colision(b) && !Colision(b))
						{

							if(dy <0)
							{
								GameOver();
							}
							else{
								b->Destroy();
							}

						}

				}
				for (int i = 0; i < questionNumbers; i++) // Questions Colisions
				{
					Question *b = questions[i];
					if(b->isInRange && b->isEnabe)
						if(m.Colision(b) && !Colision(b))
						{

							if(dy <0)
							{
								b->Destroy();
								y = b->y + b->height ;
								vy = -maxVy/2;
								T = 0;

							}
							else{
								groundY = b->y;
								y = groundY - height;
								vy = 0;
								T= 0;
								startJumping = false;
								cout<<"means colided on Top";
							}
							flag = true;
						}
				}
				for (int i = 0; i < brickNumber; i++)
				{
					Brick *b = bricks[i];
					if(b->isInRange && b->isEnabe)
						if(m.Colision(b) && !Colision(b))
						{

							if(dy <0)
							{
								b->Destroy();
								y = b->y + b->height ;
								vy = -maxVy/2;
								T = 0;
							}
							else{
								groundY = b->y;
								y = groundY - height;
								vy = 0;
								T= 0;
								startJumping = false;
								cout<<"means colided on Top";
							}
							flag = true;

						}
				}
				double mTemp = 0;
				for (int i = 0; i < stoneNumber; i++)
				{
					Stone *b = stones[i];
					if(b->isInRange && b->isEnabe)
						if(m.Colision(b)|| Colision(b))
						{
							if(dy >0 ){
								//groundY = b->y;
								if (mTemp ==0 || mTemp > b->y)
									mTemp = b->y;

							}
						}


				}
				if(mTemp){
					groundY = mTemp;
					y = groundY - height;
					vy = 0;
					T= 0;
					startJumping = false;
					cout<<"means colided on Top";
					flag = true;
				}

				for (int i = 0; i < pipeNumber; i++)
				{
					if(pipes[i]->isInRange &&pipes[i]->isEnabe)

						if(m.Colision(pipes[i]) && !Colision(pipes[i]))
						{

							if(dy <0)
							{

								y = pipes[i]->y + pipes[i]->height ;
								vy = -maxVy;
								T = 0;
							}
							else{
								groundY = pipes[i]->y;
								y = groundY - height;
								vy = 0;
								T= 0;
								startJumping = false;
								cout<<"means colided on Top";
							}
							flag = true;

						}


				}/*
				 if(m.Colision(b) && !Colision(b))
				 {

				 if(dy <0)
				 {

				 y = b->y + b->height ;
				 vy = -maxVy;
				 T = 0;
				 }
				 else{
				 groundY = b->y;
				 y = groundY - height;
				 vy = 0;
				 T= 0;
				 startJumping = false;
				 cout<<"means colided on Top";
				 }
				 }*/
				if(!flag){
					y = ( y + dy  + height > groundY ) ?  groundY - height : y + dy;

					vy -= gravity * dT ;

				}
			}
		}
		else
		{
			vy = 0;
			T= 0;
			startJumping = false;

		}


		dx = vx * dT;
		superMario temp(*this);
		temp.x += dx;
		bool endFlag = true;
		int t = startI();
		bool flag2 = true;
		if (dx && y + height >= defualtGroundY){
			for (int i = t; i <t + 18 ; i++)
			{
				baseWall *b = baseWalls[i];

				if (temp.Colision(b) && b->isEnabe){
					endFlag = false;
					break;
				}
			}
			if (endFlag)
				GameOver() ;
		}

		bool isAItemAbove = false;

		for (int i = 0; i < enemyNumber; i++)
		{
			Enemy *b = enemies[i];
			if(b->isInRange && b->isEnabe){
				if(Colision(b) )
				{

					GameOver();
				}
			}
		}

		if(dx )	{

			for (int i = 0; i < pipeNumber; i++)
			{

				Pipe *b = pipes[i];
				if(b->isInRange && b->isEnabe){
					if(temp.Colision(b)  && y + height != b->y)
					{


						if (dx < 0)
							x = b->x + b->width ;
						else
							x = b->x - width ;
						isRunState = false;
						flag2 = false;

						break;
					}

					if(temp.Colision(b))
					{
						if ( y + height == b->y)
							isAItemAbove = true;
					}
				}
			}
			for (int i = 0; i < brickNumber; i++)
			{

				Brick *b = bricks[i];
				if(b->isInRange && b->isEnabe ){
					if(!Colision(b) && temp.Colision(b) && y + height != b->y)
					{
						if (dx < 0)
							x = b->x + b->width;
						else
							x = b->x - width;
						isRunState = false;
						flag2 = false;
						break;
					}
					if(temp.Colision(b))
					{
						if ( y + height == b->y)
							isAItemAbove = true;
					}
				}

			}

			for (int i = 0; i < stoneNumber; i++)
			{

				Stone *b = stones[i];
				if(b->isInRange && b->isEnabe ){
					if(!Colision(b) && temp.Colision(b) && y + height != b->y)
					{
						if (dx < 0)
							x = b->x + b->width;
						else
							x = b->x - width;
						isRunState = false;
						flag2 = false;
						break;
					}
					if(temp.Colision(b))
					{
						if ( y + height == b->y)
							isAItemAbove = true;
					}
				}

			}



			for (int i = 0; i < questionNumbers; i++)
			{

				Question *b = questions[i];
				if(b->isInRange && b->isEnabe)
				{
					if(!Colision(b) && temp.Colision(b)&& y + height != b->y )

					{
						if (dx < 0)
							x = b->x + b->width;
						else
							x = b->x - width;
						isRunState = false;
						flag2 = false;
						break;
					}
					if(temp.Colision(b))
					{
						if ( y + height == b->y)
							isAItemAbove = true;
					}

				}


			}

			if(!isAItemAbove)
			{
				groundY = defualtGroundY;
			}
		}


		if(flag2 && dx)
		{
			if(x >= ScreenWidht/2 && dx > 0)
			{
				offSet += 25 ;
				isRunState = true;


			}

			else if(x <3 && dx <0)
				isRunState = false;
			else
			{

				x += dx;
				isRunState = true;
			}

		}

		vx = 0;
		//if (Colision(b) && y + height != b->y && y != b->y + b->height ) // if not on Top
		//{

		//	cout<<"colided\n";

		//	if(rotateLeft){ // means mario headed left

		//		x = b->x + b->width;
		//	}
		//	else // mario headed right
		//	{
		//		x = b->x - width;

		//	}
		//	isRunState = false;
		//}

		//else if( !Colision(b) && y + height == b->y)
		//	groundY = defualtGroundY;






		if(!isRunState)
		{
			if ( T !=0)
				pic = al_load_bitmap(states[0].c_str());
			else
				pic = al_load_bitmap(states[1].c_str());

			if(rotateLeft)
				al_draw_bitmap(pic,x,y,ALLEGRO_FLIP_HORIZONTAL);
			else
				al_draw_bitmap(pic,x,y,0);
		}
		else
		{
			if ( T !=0)
			{
				pic = al_load_bitmap(states[0].c_str());

				if(rotateLeft)
					al_draw_bitmap(pic,x,y,ALLEGRO_FLIP_HORIZONTAL);
				else
					al_draw_bitmap(pic,x,y,0);

				return;

			}
			if ( curState == 0)
			{
				pic = al_load_bitmap(states[2].c_str());

				curState = 1;
				if(rotateLeft)
					al_draw_bitmap(pic,x,y,ALLEGRO_FLIP_HORIZONTAL);
				else
					al_draw_bitmap(pic,x,y,0);

				return;
			}
			pic = al_load_bitmap(states[3].c_str());

			curState = 0;
			if(rotateLeft)

				al_draw_bitmap(pic,x,y,ALLEGRO_FLIP_HORIZONTAL);
			else
				al_draw_bitmap(pic,x,y,0);
			isRunState = false;
			return;

		}

	}


	void GameOver()
	{
		vy = maxVy;

		al_stop_samples();
		al_play_sample(gOver,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);

		ALLEGRO_BITMAP *lose = al_load_bitmap("media//lose.png");
		pic = al_load_bitmap(states[1].c_str());
		while( y <=ScreenHeight)
		{
			vy -= gravity * dT ;
			dy = ( -1*(   -1*.5*gravity*dT*dT + vy * dT ) ) ;
			y+=dy;

			onlyDraw();
			al_draw_bitmap(lose,0,0,0);
			al_draw_bitmap(pic,x,y,0);
			al_flip_display();
			al_rest(dT);
		}
		al_rest(5);
		exit(0);
	}

	void jump(){if(!startJumping && vy == 0){

		al_play_sample(::jump,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);

		vy = maxVy;
		startJumping = true;}
	}

	void gotoRight()
	{
		rotateLeft = false;
		vx = maxVx;
	}
	void gotoLeft()
	{
		rotateLeft = true;
		vx = -1 * maxVx ;
	}


}* mario;


class Hill : public gameObject{
public:
	Hill(int X) : gameObject(){
		RealType = hill;
		pic = al_load_bitmap("media//hill.png");
		x = X;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);

		y = defualtGroundY - height;
	}


}** hills;

class smallHill : public gameObject{
public:
	smallHill(int X) : gameObject(){
		RealType = shill;
		pic = al_load_bitmap("media//small_hill.png");
		x = X;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);

		y = defualtGroundY - height;
	}


} **sHills;


class Bush : public gameObject{
public:
	Bush(int X) : gameObject(){
		RealType = bush;
		pic = al_load_bitmap("media//bush.bmp");
		x = X;


		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);


		x-= width;
		realX =  x;

		y = defualtGroundY - height;
	}


}** bushes;

class sBush : public gameObject{
public:
	sBush(int X) : gameObject(){
		RealType = sbush;
		pic = al_load_bitmap("media//small_bush.bmp");
		x = X;
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);

		y = defualtGroundY - height;
	}


} ** sBushes;

class Cloud : public gameObject{
public:
	Cloud(int X) : gameObject(){
		pic = al_load_bitmap("media//cloud.bmp");
		x = X;
		RealType = cloud;
		y = rand() % ( ScreenHeight/2 );
		realX =  x;
		width = al_get_bitmap_width(pic);
		height = al_get_bitmap_height(pic);

	}


}** clouds;


//Turtle *turtle;


int startI()
{

	for (int i = 0; i < baseNumbers; i++)
	{
		if(baseWalls[i]->isInRange)
			return i;
	}

}

int tempY2,tempY;
void initializionObjects()
{
	srand(time(0));
	bwallHeight = baseWall(0,0).height;

	bwallWidth = baseWall(0,0).width;
	defualtGroundY = ScreenHeight - bwallHeight;
	baseWalls = new baseWall*[baseNumbers];

	mario = new superMario();
	for (size_t i = 0; i < baseNumbers; i++)
	{

		baseWalls[i] = new baseWall(i*bwallWidth, ScreenHeight -  bwallHeight);

		if ( i == 4 * 16 +6 || i == 4 * 16 +7 ||i == 5 * 16 +7 || i == 5 * 16 +8 || i == 5 * 16 +9 || i == 10* 16 - 5 || i == 10* 16 - 6 )
			baseWalls[i]->isEnabe = false;
	}

	hills = new Hill *[hillNumbers];
	sHills = new smallHill *[hillNumbers];

	for (int i = 0; i < hillNumbers; i++)
	{
		hills[i] = new Hill(	3*i*ScreenWidht);
		sHills[i] = new smallHill(( (3*i+1)*ScreenWidht));
	}


	clouds = new Cloud*[cloudSize];
	for (int i = 0; i < cloudSize; i++)
	{
		clouds[i] = new Cloud(i*ScreenWidht + rand()%ScreenWidht);
	}

	//9
	stones = new Stone*[stoneNumber];
	int stemp = 4;
	int sN = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < stemp; j++)
		{

			stones[sN++] = new Stone(9 * ScreenWidht  - ( 7 + j)*bwallWidth,defualtGroundY - (i+1)*32);

		}
		for (int j = 0; j < stemp; j++)
		{

			stones[sN++] = new Stone(9 * ScreenWidht  - ( 4 - j)*bwallWidth,defualtGroundY - (i+1)*32);

		}

		stemp--;
	}
	stemp = 4;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < stemp; j++)
		{

			stones[sN++] = new Stone(10 * ScreenWidht  - ( 8 + j)*bwallWidth,defualtGroundY - (i+1)*32);

		}
		for (int j = 0; j < stemp; j++)
		{

			stones[sN++] = new Stone(10 * ScreenWidht  - ( 4 - j)*bwallWidth,defualtGroundY - (i+1)*32);

		}

		stemp--;
	}

	for (int i = 0; i < 4; i++)
	{
		stones[sN++] = new Stone(10*ScreenWidht - 7 * bwallWidth,defualtGroundY - (i+1)*32);
	}

	stemp = 9;
	for (int i = 0; i < 9; i++)
	{
		if(stemp == 1)
			break;
		for (int j = 0; j < stemp; j++)
		{
			stones[sN++] = new Stone (12 * ScreenWidht - ( 1 + j ) *bwallWidth, defualtGroundY - (i+1) * 32);
		}
		stemp--;
	}
	/*for (int i = 0; i < 10; i++)
	{
	stones[sN++] = new Stone(12*ScreenWidht + bwallWidth,defualtGroundY - (i+1)*32);
	}*/

	bushes = new Bush*[bushNumber];
	bushes[0] = new Bush(1 * ScreenWidht - 16);
	bushes[1] = new Bush(4 * ScreenWidht - 16);
	bushes[2] = new Bush(7 * ScreenWidht - 16);
	tempY = 1*defualtGroundY /3 + 32;
	tempY2 = 2*defualtGroundY /3;
	questions = new Question*[questionNumbers];
	questions[0] = new Question(1 * ScreenWidht,tempY2);
	questions[1] = new Question(1 * ScreenWidht + 5 * bwallWidth,tempY2);
	questions[2] = new Question(1 * ScreenWidht + 6 * bwallWidth ,tempY);
	questions[2]->isMushrom = true;
	questions[3] = new Question(1 * ScreenWidht + 7 * bwallWidth,tempY2);
	questions[4] = new Question(5 * ScreenWidht - 2 * bwallWidth,tempY2);
	questions[4]->isMushrom = true;
	questions[5] = new Question( 6 * ScreenWidht - 2 * bwallWidth,tempY);
	questions[6] =  new Question(7 * ScreenWidht - 6 * bwallWidth,tempY2);
	questions[7] =  new Question(7 * ScreenWidht - 3 * bwallWidth,tempY2);
	questions[8] =  new Question(7 * ScreenWidht - 3 * bwallWidth,tempY); // this is mushrom
	questions[9] =  new Question(7 * ScreenWidht ,tempY2);
	questions[10] =  new Question(8 * ScreenWidht +  bwallWidth,tempY);
	questions[11] =  new Question(8 * ScreenWidht +  2*bwallWidth,tempY);
	questions[12] = new Question( 11 * ScreenWidht - 5*bwallWidth , tempY2);


	pipes = new Pipe*[pipeNumber];

	pipes[0] = new Pipe( 2 * ScreenWidht - bwallWidth * 2,defualtGroundY - 2*Pipe(0,0).height/4) ;
	pipes[1] = new Pipe( 2 * ScreenWidht + bwallWidth * 6,defualtGroundY - 3*Pipe(0,0).height/4) ;
	pipes[2] = new Pipe( 3 * ScreenWidht - bwallWidth * 2,defualtGroundY - Pipe(0,0).height) ;
	pipes[3] = new Pipe( 4 * ScreenWidht - bwallWidth * 7,defualtGroundY - Pipe(0,0).height) ;
	pipes[4] = new Pipe( 10 * ScreenWidht + 4* bwallWidth ,defualtGroundY - 2*Pipe(0,0).height/4) ;
	pipes[5] = new Pipe( 11 * ScreenWidht + 4* bwallWidth ,defualtGroundY - 2*Pipe(0,0).height/4) ;


	bricks = new Brick*[brickNumber];
	bricks[0] = new Brick( 1 * ScreenWidht + 4 * bwallWidth,tempY2);
	bricks[1] = new Brick( 1 * ScreenWidht + 6 * bwallWidth,tempY2);
	bricks[2] = new Brick( 1 * ScreenWidht + 8 * bwallWidth,tempY2);
	bricks[3] = new Brick( 5 * ScreenWidht - 3 * bwallWidth,tempY2);
	bricks[4] = new Brick( 5 * ScreenWidht - 1 * bwallWidth,tempY2);
	for (int i = 5; i < 14; i++)
	{
		bricks[i] = new Brick( 5 * ScreenWidht + (i-5) * bwallWidth,tempY);
	}
	bricks[14] = new Brick( 6 * ScreenWidht - 3 * bwallWidth,tempY);
	bricks[15] = new Brick( 6 * ScreenWidht - 4 * bwallWidth,tempY);
	bricks[16] = new Brick( 6 * ScreenWidht - 5 * bwallWidth,tempY);
	bricks[17] = new Brick( 6 * ScreenWidht - 2 * bwallWidth,tempY2); // this is many coins
	bricks[18] = new Brick( 6 * ScreenWidht + 5 * bwallWidth,tempY2);
	bricks[19] = new Brick( 6 * ScreenWidht + 6 * bwallWidth,tempY2); // this is star
	bricks[20] = new Brick( 7 * ScreenWidht + 6 * bwallWidth,tempY2);
	bricks[21] = new Brick( 7 * ScreenWidht + 9 * bwallWidth,tempY);
	bricks[22] = new Brick( 7 * ScreenWidht + 10 * bwallWidth,tempY);
	bricks[23] = new Brick( 7 * ScreenWidht + 11 * bwallWidth,tempY);
	bricks[24] = new Brick( 8 * ScreenWidht ,tempY);
	bricks[25] = new Brick( 8 * ScreenWidht + 3 * bwallWidth ,tempY);
	bricks[26] = new Brick( 8 * ScreenWidht + 2 * bwallWidth ,tempY2);
	bricks[27] = new Brick( 8 * ScreenWidht +  bwallWidth ,tempY2);
	bricks[28] = new Brick( 11 * ScreenWidht - 4*bwallWidth , tempY2);
	bricks[29] = new Brick( 11 * ScreenWidht - 6*bwallWidth , tempY2);
	bricks[30] = new Brick( 11 * ScreenWidht - 7*bwallWidth , tempY2);





	enemies = new Enemy*[enemyNumber];
	enemies[0] = new Enemy(2 *ScreenWidht - ScreenWidht/2 - 100 ,defualtGroundY);
	enemies[0]->gotoLeft();

	enemies[1] = new Enemy(2 *ScreenWidht + ScreenWidht/2, defualtGroundY);
	enemies[1]->gotoLeft();


	enemies[2] = new Enemy(3 *ScreenWidht + ScreenWidht/2, defualtGroundY);
	enemies[2]->gotoLeft();


	enemies[3] = new Enemy(3 *ScreenWidht + ScreenWidht/2 - 100, defualtGroundY);
	enemies[3]->gotoLeft();



	enemies[4] = new Enemy(5 *ScreenWidht -2 *bwallWidth , ScreenHeight/2 );
	enemies[4]->gotoLeft();
	enemies[4]->groundY = ScreenHeight + 100;

	enemies[5] = new Enemy(5 *ScreenWidht + 3*bwallWidth , tempY);
	enemies[5]->groundY = ScreenHeight + 100;
	enemies[5]->gotoLeft();

	enemies[6] = new Enemy(6 * ScreenWidht - bwallWidth, defualtGroundY);
	enemies[6]->gotoLeft();

	enemies[7] = new Enemy(6 * ScreenWidht + bwallWidth, defualtGroundY);
	enemies[7]->gotoLeft();


	enemies[8] = new Enemy( 8 * ScreenWidht - 3*bwallWidth,defualtGroundY);
	enemies[8]->gotoLeft();

	enemies[9] = new Enemy( 8 * ScreenWidht - 2*bwallWidth,defualtGroundY);
	enemies[9]->gotoLeft();

	enemies[10] = new Enemy( 8 * ScreenWidht - bwallWidth,defualtGroundY);
	enemies[10]->gotoLeft();

	enemies[11] = new Enemy( 8 * ScreenWidht +  1* bwallWidth,defualtGroundY);
	enemies[11]->gotoLeft();

	enemies[12] = new Enemy( 11 * ScreenWidht - 2 * bwallWidth,defualtGroundY);
	enemies[12]->gotoLeft();

	enemies[13] = new Enemy( 11 * ScreenWidht + 2 * bwallWidth,defualtGroundY);
	enemies[13]->gotoLeft();


	endFlag = new EndFlag(12.5*ScreenWidht);


}

void endGame();
ALLEGRO_COLOR color ;
void draw(){

	al_clear_to_color(color);
	//pipes befor basewall
	for (int i = 0; i < pipeNumber; i++)
	{
		pipes[i]->update();
	}
	for (size_t i = 0; i < baseNumbers; i++)
	{


		baseWalls[i]->update();

	}
	for (int i = 0; i < hillNumbers; i++)
	{
		hills[i]->update();
		sHills[i]->update();
	}

	for (int i = 0; i < bushNumber; i++)
	{
		bushes[i]->update();
	}

	for (int i = 0; i < cloudSize; i++)
	{
		clouds[i]->update();
	}
	for (int i = 0; i < questionNumbers; i++)
	{
		questions[i]->update();
	}

	for (int i = 0; i < brickNumber; i++)
	{
		bricks[i]->Update();
		bricks[i]->update();
	}

	for (int i = 0; i < stoneNumber; i++)
	{
		stones[i]->update();
	}

	Enemy temp = *enemies[1];
	temp.x += temp.dx;
	if(temp.isEnabe){

		if(temp.Colision(pipes[1]))
			enemies[1]->gotoRight();
		if(temp.Colision(pipes[2]))
			enemies[1]->gotoLeft();
	}
	temp = *enemies[2];
	temp.x += temp.dx;
	if(temp.isEnabe){
		if(temp.Colision(pipes[2]))
			enemies[2]->gotoRight();
		if(temp.Colision(pipes[3]))
			enemies[2]->gotoLeft(); // dx = x2 - x1

		if (enemies[3]->isEnabe && temp.Colision(enemies[3])  && !enemies[2]->Colision(enemies[3]))
		{

			enemies[2]->vx *= -1;
			enemies[3]->vx *= -1;
		}
	}
	temp = *enemies[3];
	temp.x += temp.vx * dT;
	if(temp.isEnabe){
		if(temp.Colision(pipes[2]))
			enemies[3]->gotoRight();
		if(temp.Colision(pipes[3]))
			enemies[3]->gotoLeft();

	}

	temp = *enemies[4];

	if(temp.isEnabe)
	{
		if ( enemies[4]->x + enemies[4]->width >  bricks[3]->x )
			enemies[4]->groundY = tempY2;
		else
			enemies[4]->groundY = defualtGroundY;

		if(enemies[4]->x + enemies[4]->width  < baseWalls[16 * 4 + 8]->x)
			enemies[4]->groundY = ScreenHeight + 100;
	}


	temp = *enemies[5];
	if (temp.isEnabe)
	{
		if (enemies[5]->x + enemies[5]->width >  bricks[5]->x)
			enemies[5]->groundY = tempY;
		else if ( enemies[5]->x + enemies[5]->width >  bricks[3]->x )
			enemies[5]->groundY = tempY2;
		else
			enemies[5]->groundY = defualtGroundY;
		if(enemies[5]->x + enemies[5]->width  < baseWalls[16 * 4 + 8]->x)
			enemies[5]->groundY = ScreenHeight + 100;
	}
	for (int i = 6	; i < 12; i++)
	{
		temp = *enemies[i];
		if(temp.isInRange)
			if(temp.x + temp.width  < baseWalls[16 * 4 + 17  + 9]->x)
				enemies[i]->groundY = ScreenHeight + 100;
	}

	temp = *enemies[12];
	temp.x += temp.dx;
	if(temp.isEnabe){
		if(temp.Colision(pipes[4]))
			enemies[12]->gotoRight();
		if(temp.Colision(pipes[5]))
			enemies[12]->gotoLeft(); // dx = x2 - x1

		if ( enemies[13]->isEnabe && temp.Colision(enemies[13])  && !enemies[12]->Colision(enemies[13]))
		{

			enemies[12]->vx *= -1;
			enemies[13]->vx *= -1;
		}
	}
	temp = *enemies[13];
	temp.x += temp.vx * dT;
	if(temp.isEnabe){
		if(temp.Colision(pipes[4]))
			enemies[13]->gotoRight();
		if(temp.Colision(pipes[5]))
			enemies[13]->gotoLeft();

	}

	for (int i = 0; i < enemyNumber; i++)
	{

		enemies[i]->update();
	}

	endFlag->update();

	if ( mario->Colision(endFlag))
		endGame();


}


void onlyDraw()
{
	al_clear_to_color(color);
	//pipes befor basewall
	for (int i = 0; i < pipeNumber; i++)
	{
		pipes[i]->drawObject();
	}
	for (size_t i = 0; i < baseNumbers; i++)
	{


		baseWalls[i]->drawObject();

	}
	for (int i = 0; i < hillNumbers; i++)
	{
		hills[i]->drawObject();
		sHills[i]->drawObject();
	}

	for (int i = 0; i < bushNumber; i++)
	{
		bushes[i]->drawObject();
	}

	for (int i = 0; i < cloudSize; i++)
	{
		clouds[i]->drawObject();
	}
	for (int i = 0; i < questionNumbers; i++)
	{
		questions[i]->drawObject();
	}

	for (int i = 0; i < brickNumber; i++)
	{
		bricks[i]->drawObject();
		bricks[i]->drawObject();
	}

	for (int i = 0; i < stoneNumber; i++)
	{
		stones[i]->drawObject();
	}

	for (int i = 0; i < enemyNumber; i++)
	{

		enemies[i]->drawObject();
	}

	endFlag->drawObject();

	mario->drawObject();

}

void endGame()
{
	al_stop_samples();
	mario->vx = 0;

	al_play_sample(win,1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
	mario->pic = al_load_bitmap("media//mario_small//m1.png");
	while (true)
	{
		if(mario->y + mario->height >= defualtGroundY)
		{
			mario->y = defualtGroundY - mario->height;
			break;
		}
		else
			mario->y += gravity *dT *dT *2 ;
		onlyDraw();
		al_draw_bitmap( al_load_bitmap("media//Won.gif"),0,0,0);
		al_flip_display();
		al_rest(dT);
	}
	al_rest(5);
	exit(0);
}


int main()
{


  if(!al_init()){
	 fprintf(stderr, "failed to initialize allegro!\n");
  }

  if(!al_install_audio()){
	 fprintf(stderr, "failed to initialize audio!\n");

	 cout<<"faild to initilize audio \n";
  }

  if(!al_init_acodec_addon()){
	 fprintf(stderr, "failed to initialize audio codecs!\n");
  }

  if (!al_reserve_samples(5)){
	 fprintf(stderr, "failed to reserve samples!\n");
  }


	cout<<al_filename_exists("mariotheme.wav")<<"\n";

	theme = al_load_sample("mariotheme.wav");
	cout<<al_get_errno()<<" al get error \n";
	jump= al_load_sample("media/audio/jump.wav");
	gOver = al_load_sample("media//audio//gameOver.wav");
	win =  al_load_sample("media//audio//end.wav");
	brickBreak =  al_load_sample("media//audio//brickBreak.wav");
	brickUnBreak =  al_load_sample("media//audio//unbreakable.wav");
	coin =  al_load_sample("media//audio//coin.wav");
	bump =  al_load_sample("media//audio//bump.wav");
	if(!theme)
		cout<<"not found theme \n";



	al_install_keyboard();
	al_init_image_addon();
	ALLEGRO_DISPLAY * display;
	display = al_create_display(ScreenWidht, ScreenHeight);

	color = al_map_rgb(93, 148, 252);
	minX = 0; maxX = ScreenWidht;
	al_clear_to_color(color);
	al_set_window_title(display,"Super Mario");

	ALLEGRO_EVENT_QUEUE *event = NULL;
	double time = 0;
	ALLEGRO_BITMAP *pic = al_load_bitmap("media//levelLoading.bmp");
	al_draw_bitmap(pic,0,0,0);
	al_flip_display();

	al_play_sample(theme,.5, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,0);
	while (time <= 2)
	{
		time += .2;
		al_rest(dT);


	}

	event = al_create_event_queue();

	al_register_event_source(event, al_get_keyboard_event_source());
	initializionObjects();
	//std::thread *th;
	//th = new std::thread(initializionObjects);

	//th->join();
	ALLEGRO_EVENT e;
	ALLEGRO_KEYBOARD_STATE key_state;


	while (true)
	{
		//	al_get_next_event(event,&e);
		al_get_keyboard_state(&key_state);
		//al_wait_for_event(event, &e);
		//if (e.type == ALLEGRO_EVENT_KEY_CHAR){
		if (al_key_down(&key_state,ALLEGRO_KEY_A) )
			mario->gotoLeft();
		if (al_key_down(&key_state,ALLEGRO_KEY_D))
			mario->gotoRight();
		if (al_key_down(&key_state,ALLEGRO_KEY_ESCAPE))
			return 0;
		if (al_key_down(&key_state,ALLEGRO_KEY_SPACE))
			mario->jump();

		//	/}

		draw();

		mario->update();

		al_flip_display();


		//al_flip_display();


		al_rest(0.06);



	}

	al_rest(3.);

	al_destroy_display(display);

	return 0;
}
