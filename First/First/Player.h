#pragma once

#include "BestNode.h"
#include "Point2D.h"
#include "CompareBestNodes.h"
#include <math.h>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int MAZE_SIZE = 100;
const int HEALTH_MUNITIONS_SIZE = 2;


class Player
{
public:
	Player();
	~Player();


private:
	int g;
	int *maze[MAZE_SIZE][MAZE_SIZE];
	int me;
	int enemy;
	int health;
	int munitions;
	char* name;
	bool *a_star_started;
	bool do_once;
	Point2D *target_seatch;
	Point2D health_arr[HEALTH_MUNITIONS_SIZE];
	Point2D munition_arr[HEALTH_MUNITIONS_SIZE];
	Point2D *enemy_point;
	BestNode current;
	BestNode* node;
	BestNode* pointer_node;
	priority_queue <BestNode, vector <BestNode>, CompareBestNodes> priority;
	vector <BestNode> gray;
	vector <BestNode> black;
	vector<BestNode>::iterator black_iterator;
	vector<BestNode>::iterator gray_iterator;
	Player* enemy1;

	

public:
	Player(char* , Point2D* start, Point2D* target, int maze[MAZE_SIZE][MAZE_SIZE] , int me , int enemy , Point2D* health_point , Point2D* munition_point);

	//new constructor
	Player(char*, Point2D* start, Point2D* target, int maze[MAZE_SIZE][MAZE_SIZE], int me, int enemy, Point2D health_arr[HEALTH_MUNITIONS_SIZE], Point2D* munition_arr[HEALTH_MUNITIONS_SIZE]);

	void set_player(Player *enemy);

	void A_start_algorithm(int target, Point2D* target_point);
	void start_game(bool *a_star_started);
	void attack();
	bool check_player_in_room();
	int get_nearby_point(Point2D arr[HEALTH_MUNITIONS_SIZE]);
	int get_distance(Point2D *p);
	void search_health();
	void search_munitions();
	void Go_up(int target);
	void Go_down(int target);
	void Go_right(int target);
	void Go_left(int target);
	bool search_Neighbors(int distance);
	void search_Neighbors();
	void clean_leftover();
	void clear();
	void print(char* action);


};

