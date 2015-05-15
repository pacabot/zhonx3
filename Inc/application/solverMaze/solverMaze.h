/*
 * resolution_maze.h
 *
 *  Created on: 27 sept. 2014
 *      Author: zhoe
 */

#ifndef RESOLUTION_MAZE_H_
#define RESOLUTION_MAZE_H_

#include "config/module_id.h"
#include "config/errors.h"

/* Error codes */
#define MAZE_SOLVER_E_SUCCESS  0
#define MAZE_SOLVER_E_ERROR    MAKE_ERROR(MAZE_SOLVER_MODULE_ID, 1)

//Define for the maze

#define MAZE_SIZE 16

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define FORWARD 0
#define RIGHT 1
#define UTURN 2
#define LEFT 3
#define NO_KNOWN 0
#define WALL_PRESENCE 1
#define NO_WALL 2

#define MAX_SPEED_ROTATION		300
#define MAX_SPEED_TRANSLATION	450
#define END_SPEED_TRANSLATION	450

#include <stdlib.h>

//Structures typedef
typedef struct
{
  char wall_north;
  char wall_south;
  char wall_east;
  char wall_west;
  int length;
}one_cell;

typedef struct
{
  one_cell cell[MAZE_SIZE][MAZE_SIZE];
}labyrinthe;

typedef struct
{
	char next_front;
	char front;
	char left;
	char right;
}walls;

//typedef struct
//{
//  int right;
//  int front;
//  int left;
//}inputs;

typedef struct coordinate
{
  int x;
  int y;
  struct coordinate *next;
  struct coordinate *previous;
}coordinate;
typedef struct
{
    char x;
    char y;
    char orientation;
    char midOfCell;
} positionRobot;


// fonctions
extern int maze(void);
void exploration(labyrinthe *maze, positionRobot* poitionZhonx,char xFinish, char yFinish);
void moveVirtualZhonx(labyrinthe maze, positionRobot positionZhonxVirtuel,coordinate *way, char xFinish, char yFinish);
void new_cell(walls new_walls, labyrinthe *maze, positionRobot positionZhonx);
void new_dot(coordinate **old_dot,int x,int y);
void poids(labyrinthe *maze, int xFinish, int yfinish, char wallNoKnow);
void maze_init (labyrinthe *maze);
void* calloc_s (size_t nombre, size_t taille);
void print_maze(const labyrinthe maze, const int x_robot, const int y_robot);
void print_length(const labyrinthe maze);
void clearMazelength(labyrinthe* maze);
char mini_way_find(labyrinthe *maze,char xStart, char yStart, char xFinish, char yFinish);
void moveRealZhonx(labyrinthe *maze, positionRobot *positionZhonx, coordinate *way, char *endX, char *endY);
void run1 (labyrinthe *maze,positionRobot *positionZhonx, char xFinish, char yFinish);
void run2(labyrinthe *maze, positionRobot *positionZhonx,char posXStart, char posYStart);
void moveRealZhonxArc(labyrinthe *maze, positionRobot *positionZhonx, coordinate *way);
void move_zhonx_arc (int direction_to_go, positionRobot *positionZhonx, int numberOfCase, char end_mid_of_case, char chain);
void doUTurn(positionRobot *positionZhonx);
void waitStart(void);
void goOrientation(char *orientationZhonx, char directionToGo);
void calibrateSimple();
char diffWay(coordinate *way1,coordinate *way2);
void deleteWay(coordinate *way);
int sensor_calibrate(void);
int wait_validation(unsigned long timeout);
#endif /* RESOLUTION_MAZE_H_ */
