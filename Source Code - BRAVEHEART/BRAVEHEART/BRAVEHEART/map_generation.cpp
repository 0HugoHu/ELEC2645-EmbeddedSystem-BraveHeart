#include "map_generation.h"
#include "sprites.h"

map::map() {
	layer1_init();

}

void map::random_time() {
	// initialize time seeds
	unsigned seed;
	seed = time(0);
	srand(seed);
	// total room number = 6
	// always a boss room = 1
	layer.small = rand() % 2 + 1;		// range:1-2
	layer.normal = rand() % 3 + 2;		// range:2-4
	if (layer.small == 2 && layer.normal == 4) layer.normal = 3;
	layer.big = 6 - layer.small - layer.normal - 1;
}

void map::identify_room() {
	// give each room to the 1-6 positions
	layer_room[5] = BOSS;
	if (layer.small == 2) {
		layer_room[0] = SMALL;
		layer_room[1] = SMALL;
		switch (layer.normal) {
		case 2:
			layer_room[2] = NORMAL;
			layer_room[3] = NORMAL;
			layer_room[4] = BIG;
			break;
		case 3:
			layer_room[2] = NORMAL;
			layer_room[3] = NORMAL;
			layer_room[4] = NORMAL;
			break;
		}
	}
	else if (layer.small == 1) {
		layer_room[0] = SMALL;
		switch (layer.normal) {
		case 2:
			layer_room[1] = NORMAL;
			layer_room[2] = NORMAL;
			layer_room[3] = BIG;
			layer_room[4] = BIG;
			break;
		case 3:
			layer_room[1] = NORMAL;
			layer_room[2] = NORMAL;
			layer_room[3] = NORMAL;
			layer_room[4] = BIG;
			break;
		case 4:
			layer_room[1] = NORMAL;
			layer_room[2] = NORMAL;
			layer_room[3] = NORMAL;
			layer_room[4] = NORMAL;
			break;
		}
	}
}

void map::generate_room(ROOM_TYPE _room_type) {
	switch (_room_type) {
	case SMALL: {
		layer.small--;
		// ****temp****
		if (layer.small == 1 || 1) {
			// draw walls
			for (int i = 0; i < SMALL_SIZE_H; i++)
				for (int j = 0; j < SMALL_SIZE_W; j++)
					if ((i <= 1 || i >= SMALL_SIZE_H - 2) || (j <= 1 || j >= SMALL_SIZE_W - 2))
						map_l1_r1_small[i][j] = 1;
			// draw doors
			
			// draw sprites
			// chest
			int x0 = (SMALL_SIZE_W - 8) / 2;
			int y0 = (SMALL_SIZE_H - 8) / 2;
			int* sprite = (int*)t_chest;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					int pixel = *((sprite + i * 8) + j);
					if (pixel) {
						map_l1_r1_small[x0 + j][y0 + i] = 1;
					}
					else {
						map_l1_r1_small[x0 + j][y0 + i] = 0;
					}
				}
			}
		}
		else {
			// shop

		}
		break;
	}
	case NORMAL: {
		layer.normal--;
		if (layer.normal == 3) {

		}
		else if (layer.normal == 2) {

		}
		else if (layer.normal == 1) {

		}
		else {

		}
		break;
	}
	case BIG: {
		break;
	}
	case BOSS: {

		break;
	}
	}
}

void map::layer1_init() {
	
	random_time();
	identify_room();
	for (int i = 0; i < 6; i++)
		generate_room(layer_room[i]);

	

	// generate room


}
