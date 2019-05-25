#include "Player.h"
#include "GLUT.h"


const int SPACE = 1;
const int WALL = 2;
const int HEALTH = 3;
const int MUNITIONS = 4;
const int PLAYER1 = 8;
const int PLAYER2 = 9;
const int DISTANCE = 3;

Player::Player()
{
}


Player::~Player()
{
}


// new constructor ***********************
Player::Player(char*name, Point2D* start, Point2D* target, int maze[MAZE_SIZE][MAZE_SIZE], int me, int enemy, Point2D health_arr[HEALTH_MUNITIONS_SIZE], Point2D munition_arr[HEALTH_MUNITIONS_SIZE])
{
	int i, j;
	this->g = 0;
	this->name = name;
	this->health = 20;
	this->munitions = 10;
	this->me = me;
	this->enemy = enemy;
	this->enemy_point = target;
	this->target_seatch = target;
	this->do_once = true;
	this->node = new BestNode(start, target);
	this->priority.emplace(*this->node);
	this->gray.push_back(*this->node);
	this->pointer_node = NULL;

	for (i = 0; i < MAZE_SIZE; i++)
		for (j = 0; j < MAZE_SIZE; j++)
			this->maze[i][j] = &maze[i][j];

	for (i = 0; i < HEALTH_MUNITIONS_SIZE; i++)
	{
		this->health_arr[i] = health_arr[i];
		this->munition_arr[i] = munition_arr[i];
	}

}

void Player::set_player(Player *enemy)
{
	this->enemy1 = enemy;
}


void Player::start_game(bool *a_star_started)
{


	if (*a_star_started == false)
		return;

	this->a_star_started = a_star_started;
	
	if (this->health < 0)
	{
		(*a_star_started) = false;
		printf("THE WINNER IS %s", this->enemy1->name);
		return;
	}

	if (this->health < 25)
	{
		search_health();
		return;
	}

	if (this->munitions < 10)
	{
		search_munitions();
		return;
	}
	attack();
}


void Player::attack()
{
	//printf("%s : ACTION : Attacking\n", this->name);

	if (this->do_once == true)
	{
		print("Attacking");
		this->do_once = false;
		this->target_seatch = this->enemy_point;
		clear();

	}

	A_start_algorithm(this->enemy , this->enemy_point);

	if ((*a_star_started) == false)
	{
		this->do_once = true;
		*this->a_star_started = true;
		//this->munitions = 5;

	}


}


void Player::search_health()
{
	//printf("%s : ACTION : Search Health\n", this->name);

	int index = 0;

	if(this->do_once == true)
	{
		print("Search Health");
		this->do_once = false;
		clear();

	}
	index = get_nearby_point(this->health_arr);
	this->target_seatch = &this->health_arr[index];


	A_start_algorithm(HEALTH  , target_seatch);

	this->health_arr[index].SetIsValid(true);
	if ((*a_star_started) == false)
	{
		this->health_arr[index].SetIsValid(true);
		this->do_once = true;
		this->health = 100;
		printf("i am %s fount the medicament and my health : %d/100\n",this->name , this->health);
		*this->a_star_started = true;
	}

}


void Player::search_munitions()
{
	//printf("%s : ACTION : Search Munition\n", this->name);

	int index = 0;

	if (this->do_once == true)
	{
		print("Search Munitions");
		this->do_once = false;
		clear();

	}
	index = get_nearby_point(this->munition_arr);
	this->target_seatch = &this->munition_arr[index];


	A_start_algorithm(MUNITIONS , &this->munition_arr[index]);
	this->munition_arr[index].SetIsValid(false);

	if ((*a_star_started) == false)
	{
		this->munition_arr[index].SetIsValid(false);
		this->do_once = true;
		this->munitions = 100;
		printf("i am %s fount the munitions and my munitions : %d/100\n",this->name , this->munitions);
		*this->a_star_started = true;

	}

}


int Player::get_distance(Point2D* p)
{
	int target_i, target_j;

	target_i = this->node->GetPoint()->GetY();
	target_j = this->node->GetPoint()->GetX();

	return abs(p->GetY() - target_i) + abs(p->GetX() - target_j);
}


int Player::get_nearby_point(Point2D arr[HEALTH_MUNITIONS_SIZE])
{
	int i, index = 0;

	for (i = 0; i < HEALTH_MUNITIONS_SIZE - 1; i++)
	{
		if (arr[i].GetIsValid() == true)
			if (get_distance(&arr[i]) < get_distance(&arr[i + 1]))
				index = i;
			else
				index = i + 1;

	}
	arr[index].SetIsValid(false);
	return index;
}


void Player::search_Neighbors() {
	int distance;
	for (distance = 1; distance <= 3; distance++)
		if (search_Neighbors(distance))
		{
			switch (distance)
			{
			case 1:
				this->enemy1->health -= 6;
				this->munitions -= 4;
				break;
			case 2:
				this->enemy1->health -= 4;
				this->munitions -= 4;

				break;
			case 3:
				this->enemy1->health -= 2;
				this->munitions -= 4;

				break;
			}
			//printf("-------------------->>>>> %d \n", distance);
			printf("i am %s and the enemy attacked me and my health is : %d/100\n", this->name , this->health);


		}
}


bool Player::search_Neighbors(int distance) {
	int i, j, start_i, start_j;
	start_i = current.GetPoint()->GetY();
	start_j = current.GetPoint()->GetX();

	//Right
	for (i = start_i, j = start_j; j<MAZE_SIZE && j <= (start_j + distance); j++)
		if ((*maze[i][j]) == this->enemy)
		{
			return true;
		}


	//Left
	for (i = start_i, j = start_j; j>0 && j >= (start_j - distance); j--)
		if ((*maze[i][j]) == this->enemy)
		{
			return true;
		}


	//Down
	for (i = start_i; i<MAZE_SIZE && i <= (start_i + distance); i++)
		for (j = start_j - distance; j>0 && j<MAZE_SIZE && j <= (start_j + distance); j++)
			if ((*maze[i][j]) == this->enemy)
			{
				return true;
			}


	//Up
	for (i = start_i; i>0 && i >= (start_i - distance); i--)
		for (j = start_j - distance; j>0 && j<MAZE_SIZE && j <= (start_j + distance); j++)
			if ((*maze[i][j]) == this->enemy)
			{
				return true;
			}
	return false;

}


bool Player::check_player_in_room() {
	int i, j, start_i, start_j;
	start_i = current.GetPoint()->GetY();
	start_j = current.GetPoint()->GetX();

	bool right_wall, left_wall, up_wall, down_wall;
	right_wall = left_wall = up_wall = down_wall = false;

	//Right
	for (i = start_i, j = start_j; i < MAZE_SIZE && i <= (start_i + 2); i++)
		if ((*maze[i][j]) == WALL)
			right_wall = true;

	//Left
	for (i = start_i, j = start_j; i > 0 && i >= (start_i - 2); i--)
		if ((*maze[i][j]) == WALL)
			left_wall = true;

	//Up
	for (i = start_i, j = start_j; j > 0 && j >= (start_j - 2); j--)
		if ((*maze[i][j]) == WALL)
			up_wall = true;

	//down
	for (i = start_i, j = start_j; j < MAZE_SIZE && j <= (start_j + 2); j++)
		if ((*maze[i][j]) == WALL)
			down_wall = true;

	//Tunnel Vertical
	if (left_wall && right_wall && !(up_wall) && !(down_wall))
		return false;

	//Tunnel Horizontal
	if (!(left_wall) && !(right_wall) && up_wall && down_wall)
		return false;

	return true;
}


void Player::A_start_algorithm(int target , Point2D* target_point)
{
	if (priority.empty())
		return;
	else
	{
		current = priority.top();
		priority.pop();
		black.push_back(current);
		gray_iterator = find(gray.begin(), gray.end(), current);
		gray.erase(gray_iterator);

		//-------------------------------------------------------------------------------------------
		//if(check_player_in_room() == true && this->munitions >= 4)
			search_Neighbors();
		//-------------------------------------------------------------------------------------------


		if (*maze[current.GetPoint()->GetY()][current.GetPoint()->GetX()] == target)
			(*a_star_started) = false;
		else
		{
			if (*maze[current.GetPoint()->GetY()][current.GetPoint()->GetX()] != this->me)
				*maze[current.GetPoint()->GetY()][current.GetPoint()->GetX()] = this->me;

				node->setPoint(current.GetPoint()->GetY(), current.GetPoint()->GetX());
				
				// change traget of search
				current.setTargetPoint(target_point);

				Go_up(target);
				Go_down(target);
				Go_right(target);
				Go_left(target);

				clean_leftover();

				return;
		}
		return;
	}
}


void Player::Go_up(int target)
{
	if (current.GetPoint()->GetY() + 1 < MAZE_SIZE && current.GetPoint()->GetX() < MAZE_SIZE)
	{

		if (*maze[current.GetPoint()->GetY() + 1][current.GetPoint()->GetX()] == target)
			*a_star_started = false;
		if (a_star_started && *maze[current.GetPoint()->GetY() + 1][current.GetPoint()->GetX()] == SPACE)
		{
			current.move_up();
			pointer_node = new BestNode(current);
			current.setG(g - 1);
			black_iterator = find(black.begin(), black.end(), *pointer_node);
			gray_iterator = find(gray.begin(), gray.end(), *pointer_node);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				priority.emplace(*pointer_node);
				gray.push_back(*pointer_node);
			}
			current.move_down();
			current.setG(g - 1);
		}

	}
	
}


void Player::Go_down(int target)
{
	if (current.GetPoint()->GetY() - 1 < MAZE_SIZE && current.GetPoint()->GetX() < MAZE_SIZE)
	{

		if (*maze[current.GetPoint()->GetY() - 1][current.GetPoint()->GetX()] == target)
			*a_star_started = false;
		if (a_star_started && *maze[current.GetPoint()->GetY() - 1][current.GetPoint()->GetX()] == SPACE)
		{
			current.move_down();
			pointer_node = new BestNode(current);
			current.setG(g - 1);
			black_iterator = find(black.begin(), black.end(), *pointer_node);
			gray_iterator = find(gray.begin(), gray.end(), *pointer_node);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				priority.emplace(*pointer_node);
				gray.push_back(*pointer_node);
			}
			current.move_up();
			current.setG(g - 1);
		}


	}
	
}


void Player::Go_right(int target)
{
	if (current.GetPoint()->GetY() < MAZE_SIZE && current.GetPoint()->GetX() + 1 < MAZE_SIZE)
	{

		if (*maze[current.GetPoint()->GetY()][current.GetPoint()->GetX() + 1] == target)
			*a_star_started = false;
		if (a_star_started && *maze[current.GetPoint()->GetY()][current.GetPoint()->GetX() + 1] == SPACE)
		{
			current.move_right();
			pointer_node = new BestNode(current);
			current.setG(g - 1);
			black_iterator = find(black.begin(), black.end(), *pointer_node);
			gray_iterator = find(gray.begin(), gray.end(), *pointer_node);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				priority.emplace(*pointer_node);
				gray.push_back(*pointer_node);
			}
			current.move_left();
			current.setG(g - 1);
		}


	}
	
}


void Player::Go_left(int target)
{
	if (current.GetPoint()->GetY() < MAZE_SIZE && current.GetPoint()->GetX() - 1 < MAZE_SIZE)
	{

		if (*maze[current.GetPoint()->GetY()][current.GetPoint()->GetX() - 1] == target)
			*a_star_started = false;
		if (a_star_started && *maze[current.GetPoint()->GetY()][current.GetPoint()->GetX() - 1] == SPACE)
		{
			current.move_left();
			pointer_node = new BestNode(current);
			current.setG(g - 1);
			black_iterator = find(black.begin(), black.end(), *pointer_node);
			gray_iterator = find(gray.begin(), gray.end(), *pointer_node);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				priority.emplace(*pointer_node);
				gray.push_back(*pointer_node);
			}
			current.move_right();
			current.setG(g - 1);
		}


	}
	
}


void Player::clean_leftover()
{
	
	//printf("FUN : clean_leftover : black size : %d\n", black.size());

	for (black_iterator = black.begin(); black_iterator != black.end(); black_iterator++)
		if (!black_iterator->operator==(current))
			if (*maze[black_iterator->GetPoint()->GetY()][black_iterator->GetPoint()->GetX()] != this->enemy)
				*maze[black_iterator->GetPoint()->GetY()][black_iterator->GetPoint()->GetX()] = SPACE;

	
	if(black.size() > 100)
		black.erase(black.begin());

}

void Player::clear()
{

	this->black.clear();
	this->gray.clear();
	this->priority = priority_queue <BestNode, vector <BestNode>, CompareBestNodes>();
	this->node->setTargetPoint(this->target_seatch);
	this->priority.emplace(*this->node);
	this->gray.push_back(*this->node);
	this->pointer_node = NULL;

}

void Player::print(char* action)
{
	printf("<<<<<<<<<<<<<<<-------------------%s------------------->>>>>>>>>>>>>>>\n", this->name);
	printf("%s : HEALTH		 : %d\n", this->name, this->health);
	printf("%s : MUNITIONS	 : %d\n", this->name, this->munitions);
	printf("%s : ACTION		 : %s\n", this->name , action);
	printf("------------------------------------------------------------------\n", this->name);

}








