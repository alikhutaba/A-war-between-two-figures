
//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************
//***********************														***********************
//***********************														***********************
//***********************				Name:	Ali khutaba						***********************
//***********************				Name:	Eyad zaid						***********************
//***********************														***********************
//***********************														***********************
//***********************					version 1.1.6						***********************
//***********************														***********************
//***********************		last update : 02/04/2019	21:00				***********************
//***********************														***********************
//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************

#include "GLUT.h"
#include <math.h>
#include <time.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "Point2D.h"
#include "Room.h"
#include "Node.h"
#include "BestNode.h"
#include "CompareNodes.h"
#include "Parent.h"
#include "Player.h"

using namespace std;

const int W = 600; // window width
const int H = 600; // window height
const int NUM_ROOMS = 10;
const int NUM_HEALTH_MUNITIONS = 2;
const int NUM_OBSTACLES = 3;            

const int SPACE = 1;
const int WALL = 2;
const int HEALTH = 3;
const int MUNITIONS = 4;
const int OBSTACLES = 5;            
const int HIDDEN_TARGET = 6;		
const int PLAYER1 = 8;
const int PLAYER2 = 9;

const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;

const int MSIZE = 100;
const double SQSIZE = 2.0 / MSIZE;
int maze[MSIZE][MSIZE];

bool start_game = false;

Room all_rooms[NUM_ROOMS];

// gray queue
vector <Point2D> gray;
vector <Point2D> black;
vector <Parent> parents;

priority_queue<Node, vector<Node>, CompareNodes> pq;

Point2D start,target;

Point2D health_arr[NUM_HEALTH_MUNITIONS];
Point2D munitions_arr[NUM_HEALTH_MUNITIONS];
Point2D obstacles_arr[NUM_HEALTH_MUNITIONS];      

int status_rooms[NUM_ROOMS] = { 0 };            

Point2D* start_first_player, *start_secend_player;
Point2D* health_point1, *health_point2;
Point2D* munition_point1, *munition_point2;

Player *first_player, *secend_player;


int counter = 0;

void SetupMaze();
Point2D* Setup_Health_munitions(int num_room);
void munition();
void health();
void obstacles();			

Point2D* RandomPoints()
{
	int x, y;
	bool isSpace = true;

	while (isSpace)
	{
		x = rand() % (MSIZE-1);
		y = rand() % (MSIZE-1);
		
		if (maze[x][y] == SPACE)
			isSpace = false;
	}
	return new Point2D(y, x);;
}

void init()
{
	int i, j;
	Point2D* pt;

	srand(time(0));

	// clean up the maze
	for (i = 0; i < MSIZE; i++)
		for (j = 0; j < MSIZE; j++)
			maze[i][j] = WALL;

	SetupMaze();


	start_first_player = RandomPoints();
	start_secend_player = RandomPoints();
	maze[start_first_player->GetY()][start_first_player->GetX()] = PLAYER1;
	maze[start_secend_player->GetY()][start_secend_player->GetX()] = PLAYER2;
	
	/*
	health_point1 = Setup_Health_munitions();
	health_point2 = Setup_Health_munitions();
	munition_point1 = Setup_Health_munitions();
	munition_point2 = Setup_Health_munitions();
	maze[health_point1->GetY()][health_point1->GetX()] = HEALTH;
	maze[health_point2->GetY()][health_point2->GetX()] = HEALTH;
	maze[munition_point1->GetY()][munition_point1->GetX()] = MUNITIONS;
	maze[munition_point2->GetY()][munition_point2->GetX()] = MUNITIONS;
	*/

	health();
	munition();
	obstacles();

	first_player = new Player("red",start_first_player, start_secend_player, maze , PLAYER1 , PLAYER2 , health_arr, munitions_arr);
	secend_player = new Player("blue",start_secend_player, start_first_player, maze , PLAYER2 , PLAYER1, health_arr, munitions_arr);

	first_player->set_player(secend_player);
	secend_player->set_player(first_player);
	glClearColor(0.7, 0.7, 0.7, 0);

	glOrtho(-1, 1, -1, 1, -1, 1);
}



Point2D* Setup_obstacles_munitions(int num_room)
{
	int x, y, min, max;
	max = all_rooms[num_room].GetCenter().GetY() + (all_rooms[num_room].GetHeight() / 2);
	x = max - 2;

	max = all_rooms[num_room].GetCenter().GetX() + (all_rooms[num_room].GetWidth() / 2);
	y = max - 1;

	return new Point2D(y, x);
}


void obstacles()
{
	int num_room, i;
	int rooms_status[NUM_ROOMS] = { 0 };

	for (i = 0; i < NUM_OBSTACLES; ) 
	{
		num_room = rand() % NUM_ROOMS;
		if (rooms_status[num_room] == 0)
		{
			obstacles_arr[i] = *(Setup_obstacles_munitions(num_room));
			maze[obstacles_arr[i].GetY()][obstacles_arr[i].GetX()] = OBSTACLES;
			maze[obstacles_arr[i].GetY() + 1][obstacles_arr[i].GetX()] = OBSTACLES;
			maze[obstacles_arr[i].GetY() + 1][obstacles_arr[i].GetX() + 1] = HIDDEN_TARGET;
			rooms_status[num_room] = 1;
			i++;
		}
	}
}


void health()
{
	int num_room, i;

	for (i = 0; i < NUM_HEALTH_MUNITIONS; )
	{
		num_room = rand() % NUM_ROOMS;
		if (status_rooms[num_room] == 0)
		{
			health_arr[i] = *(Setup_Health_munitions(num_room));
			maze[health_arr[i].GetY()][health_arr[i].GetX()] = HEALTH;
			status_rooms[num_room] = 1;
			i++;
		}
	}
}


void munition()
{
	int num_room, i;

	for (i = 0; i < NUM_HEALTH_MUNITIONS; )
	{
		num_room = rand() % NUM_ROOMS;
		if (status_rooms[num_room] == 0)
		{
			munitions_arr[i] = *(Setup_Health_munitions(num_room));
			maze[munitions_arr[i].GetY()][munitions_arr[i].GetX()] = MUNITIONS;
			status_rooms[num_room] = 1;
			i++;
		}
	}
}



/*
Point2D* Setup_Health_munitions()
{
	int r , x , y;
	r = rand() % NUM_ROOMS;
	x = (rand() % ((all_rooms[r].GetCenter().GetY() + all_rooms[r].GetHeight() / 2) - (all_rooms[r].GetCenter().GetY() - all_rooms[r].GetHeight() / 2))) + (all_rooms[r].GetCenter().GetY() - all_rooms[r].GetHeight() / 2);
	y = (rand() % ((all_rooms[r].GetCenter().GetX() + all_rooms[r].GetWidth() / 2) - (all_rooms[r].GetCenter().GetX() - all_rooms[r].GetWidth() / 2) )) + (all_rooms[r].GetCenter().GetX() - all_rooms[r].GetWidth() / 2);
	return new Point2D(y, x);
}
*/


Point2D* Setup_Health_munitions(int num_room)
{
	int x, y , min , max;

	max = all_rooms[num_room].GetCenter().GetY() + (all_rooms[num_room].GetHeight() / 2);
	min = all_rooms[num_room].GetCenter().GetY() - (all_rooms[num_room].GetHeight() / 2);
	x = (rand() % ((max - min)-1)) + min;
	
	max = all_rooms[num_room].GetCenter().GetX() + (all_rooms[num_room].GetWidth() / 2);
	min = all_rooms[num_room].GetCenter().GetX() - (all_rooms[num_room].GetWidth() / 2);
	y = (rand() % ((max - min)-1)) + min;

	return new Point2D(y, x);
}



void AddNewNode(Node current, int direction)
{
	Node* tmp;
	Point2D* pt;
	vector<Point2D>::iterator gray_it;
	vector<Point2D>::iterator black_it;
	double space_weight = 0.1, wall_weight = 5, weight;
	int dx, dy;

	switch (direction)
	{
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = 1;
		break;
	case LEFT:
		dx = -1;
		dy = 0;
		break;
	case RIGHT:
		dx = 1;
		dy = 0;
		break;
	}// switch

	if (direction==UP && current.GetPoint().GetY() > 0 ||
		direction == DOWN && current.GetPoint().GetY() < MSIZE-1 ||
		direction == LEFT && current.GetPoint().GetX() > 0 ||
		direction == RIGHT && current.GetPoint().GetX() < MSIZE - 1)
	{
		pt = new Point2D(current.GetPoint().GetX()+dx, current.GetPoint().GetY() +dy);
		gray_it = find(gray.begin(), gray.end(), *pt);
		black_it = find(black.begin(), black.end(), *pt);
		if (gray_it == gray.end() && black_it == black.end()) // this is a new point
		{
			// very important to tunnels
			if (maze[current.GetPoint().GetY() +dy][current.GetPoint().GetX()+dx] == WALL)
				weight = wall_weight;
			else weight = space_weight;
			// weight depends on previous weight and wheater we had to dig
			// to this point or not
			tmp = new Node(*pt, target, current.GetG() + weight);
			pq.emplace(*tmp); // insert first node to priority queue
			gray.push_back(*pt); // paint it gray
			// add Parent
			parents.push_back(Parent(tmp->GetPoint(), current.GetPoint(), true));
		}
	}

}

void RunAStar4Tunnels()
{
	Node current;
	Node* tmp;
	Point2D* pt;
	vector<Point2D>::iterator gray_it;
	vector<Point2D>::iterator black_it;
	bool finished = false;
	double space_weight = 0.5, wall_weight = 0.5,weight;

	while (!pq.empty() && !finished)
	{
		current = pq.top();
		pq.pop(); // remove it from pq

		if (current.GetH() == 0) // the solution has been found
		{
			vector<Parent>::iterator itr;
			finished = true;
			// go back to start and change WALL to SPACE
			itr = find(parents.begin(), parents.end(),
				Parent(current.GetPoint(), current.GetPoint(), true));
			while (itr->HasParent())
			{
				Point2D tmp_prev = itr->GetPrev();
				Point2D tmp_cur = itr->GetCurrent();
				// set SPACE
				if (maze[tmp_cur.GetY()][tmp_cur.GetX()] == WALL)
					maze[tmp_cur.GetY()][tmp_cur.GetX()] = SPACE;
				itr = find(parents.begin(), parents.end(),
					Parent(tmp_prev, current.GetPoint(), true));
			}
		}
		else // check the neighbours
		{
			// remove current from gray 
			gray_it = find(gray.begin(), gray.end(), current.GetPoint());
			if (gray_it != gray.end())
				gray.erase(gray_it);
			// and paint it black
			black.push_back(current.GetPoint());
			// try to go UP
			AddNewNode(current, UP);
			// try to go DOWN
			AddNewNode(current, DOWN);
			// try to go LEFT
			AddNewNode(current, LEFT);
			// try to go RIGHT
			AddNewNode(current, RIGHT);

		}

	} // while
}


void DigTunnels()
{
	int i, j;

	for(i=0;i<NUM_ROOMS;i++)
		for (j = i + 1; j < NUM_ROOMS; j++)
		{
			start = all_rooms[i].GetCenter();
			target = all_rooms[j].GetCenter();

			printf("Start: %d      Target: %d\n", i, j);

			Node* tmp = new Node(start, target, 0);
			while (!pq.empty())
				pq.pop();

			pq.emplace(*tmp); // insert first node to priority queue
			gray.clear();
			gray.push_back(start); // paint it gray
			black.clear();
			parents.clear();
			parents.push_back(Parent(tmp->GetPoint(),
				tmp->GetPoint(), false));
			RunAStar4Tunnels();
			delete tmp;
		}
}

void SetupMaze()
{
	int i, j,counter;
	int left, right, top, bottom;
	bool isValidRoom;
	Room* pr=NULL;

	for (i = 0; i < MSIZE; i++)
	{
		maze[i][0] = WALL;
		maze[i][1] = WALL;

		maze[0][i] = WALL;
		maze[1][i] = WALL;

		maze[i][MSIZE - 1] = WALL;
		maze[i][MSIZE - 2] = WALL;

		maze[MSIZE - 1][i] = WALL;
		maze[MSIZE - 2][i] = WALL;
	}
	

	for (counter = 0; counter < NUM_ROOMS; counter++)
	{
		// create room
		do
		{
			free(pr);
			pr = new Room(Point2D(rand()%MSIZE,
			rand() % MSIZE), 5 + rand() % 15, 5 + rand() % 25);
			top = pr->GetCenter().GetY() - pr->GetHeight() / 2;
			if (top < 0) top = 0;
			bottom = pr->GetCenter().GetY() + pr->GetHeight() / 2;
			if (bottom >= MSIZE) bottom = MSIZE - 1;
			left = pr->GetCenter().GetX() - pr->GetWidth() / 2;
			if (left < 0) left = 0;
			right = pr->GetCenter().GetX() + pr->GetWidth() / 2;
			if (right >= MSIZE) right = MSIZE - 1;

			isValidRoom = true;
			for (i = 0; i < counter && isValidRoom; i++)
				if (all_rooms[i].IsOverlap(*pr))
					isValidRoom = false;

		} while (!isValidRoom);

		all_rooms[counter] = *pr;
		for (i = top; i <= bottom; i++)
			for (j = left; j <= right; j++)
				maze[i][j] = SPACE;


	}

	DigTunnels();
}

void DrawMaze()
{
	int i, j;

	for(i = 0;i<MSIZE;i++)
		for (j = 0; j < MSIZE; j++)
		{
			switch (maze[i][j])
			{
			case WALL:
				glColor3d(0.4, 0, 0); // dark red;
				break;
			case SPACE:
				glColor3d(1, 1, 1); // white;
				break;
			case HEALTH:
				glColor3f(0.0f, 1.0f, 0.0f);//Green
				break;
			case MUNITIONS:
				glColor3d(1, .8, 0); // ORANGE;
				break;
			case PLAYER1:
				glColor3d(1, 0, 0); // RED
				break;
			case PLAYER2:
				glColor3d(0, 0, 1); // BLUE
				break;
			case OBSTACLES:
				glColor3d(0.5, 0.3, 0.3); // Brown 
				break;
			case HIDDEN_TARGET:
				glColor3d(0, 0, 0); // Change to White  ////  Black  
				break;
			}
			// draw square
			glBegin(GL_POLYGON);
				glVertex2d(j*SQSIZE - 1- SQSIZE/2, i*SQSIZE - 1+SQSIZE/2);
				glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 + SQSIZE / 2);
				glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
				glVertex2d(j*SQSIZE - 1 - SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
			glEnd();
		}

}



//*************************************** My Game ****************************************************


void StartGame()
{

	if (start_game)
	{
		first_player->start_game(&start_game);
		secend_player->start_game(&start_game);
		Sleep(100);
		counter++;
		//printf("%d	after the sleep in the start game main\n" , counter);

	}

	
}


//********************************************************************************************************


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawMaze();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void idle()
{
	glutPostRedisplay();// calls indirectly to display

	if (start_game) // start the game 
		StartGame();

}

void Menu(int choice)
{

	switch (choice)
	{
	case 1:
		start_game = true;
		break;
	case 2:
		start_game = false;
		break;
	}

}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Digits Example ");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	init();

	glutCreateMenu(Menu);
	glutAddMenuEntry("Start Game", 1);
	glutAddMenuEntry("stop Game", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutMainLoop();
}