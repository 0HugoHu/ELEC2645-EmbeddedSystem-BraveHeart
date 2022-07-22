#pragma once
#include <cstdlib>
#include <ctime>

enum ROOM_TYPE { SMALL, NORMAL, BIG, BOSS, ERR };
#define NORMAL_SIZE_W 126
#define NORMAL_SIZE_H 72
#define SMALL_SIZE_W 48
#define SMALL_SIZE_H 48
#define BIG_SIZE_W 168
#define BIG_SIZE_H 96
#define BOSS_SIZE_W 96
#define BOSS_SIZE_H 96

typedef struct _layer_room{
	int small = 0;
	int normal = 0;
	int big = 0;
	int boss = 1;
};

// 0: space; 1: wall; 2: door
int map_l1_r1_normal[72][126] = { 0 };
int map_l1_r2_normal[72][126] = { 0 };
int map_l1_r3_normal[126][72] = { 0 };
int map_l1_r4_normal[126][72] = { 0 };
int map_l1_r1_small[48][48] = { 0 };
int map_l1_r2_small[48][48] = { 0 };
int map_l1_r1_big[96][168] = { 0 };
int map_l1_r2_big[96][168] = { 0 };
int map_l1_r1_boss[96][96] = { 0 };

class map {
public:
	map();
	void layer1_init();
private:
	struct _layer_room layer;
	ROOM_TYPE layer_room[6] = { ERR };
	void random_time();
	void identify_room();
	void generate_room(ROOM_TYPE _room_type);
};

