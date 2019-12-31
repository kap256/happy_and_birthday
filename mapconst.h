#pragma once
#include <d3d9.h>

const int MAP_SIZE_X			=100;
const int MAP_SIZE_Y			=100;
const int MAP_SIZE_Z			=32;
const int MAP_CHIP_NO			=32;
const int OBJECT_NO				=100;
const int MIN_MAP_SIZE			=6;
const int MAX_BG_COLOR			=8;
const int MAX_ENEMY_TYPE		=16;

const int ESCAPE_TIME			=128;
const int ACTION_BEFORE_TIME	=8;
const int ACTION_AFTER_TIME		=24;
const int ACTION_TIME			=ACTION_BEFORE_TIME+ACTION_AFTER_TIME;
	const int ACTION_NONE			=1;
	const int ACTION_THROW_STONE	=2;

const int BASE_GRAV			=0xff;
	const int GRAV_NONE			=0;
	const int GRAV_JUMP			=1;
	const int GRAV_NO_JUMP		=2;
	const int GRAV_LOCK			=3;
	const int GRAV_AIR			=4;
	const int GRAV_FALL_NONE	=5;
const int GRAV_FLOAT		=0x0100;
	const int GRAV_FLOAT_TIME	=12;
const int GRAV_HI_JUMP		=0x0200;
const int GRAV_WATER		=0x0400;
const int GRAV_WATER_SKI	=0x0800+GRAV_WATER;
const int GRAV_WATER_ONLY	=0x1000;

const int MAP_MES_LEFT_TOP		=0x000;
const int MAP_MES_LEFT_BOTTOM	=0x001;
const int MAP_MES_RIGHT_TOP		=0x010;
const int MAP_MES_RIGHT_BOTTOM	=0x011;
const int MAP_MES_CENTER		=0x100;

const int MAX_MAP_FLAG			=32;

extern const int MapChipParam[MAP_CHIP_NO];
	const int MCHIP_NONE			=0;
	const int MCHIP_V_GROUND		=0x1;
	const int MCHIP_H_GROUND		=0x2;
	const int MCHIP_GROUND			=MCHIP_V_GROUND | MCHIP_H_GROUND;
	const int MCHIP_ANIMATION		=0x4;
	const int MCHIP_WATER			=0x8;
	const int MCHIP_NO_WALK			=0x10	|	MCHIP_WATER;
	const int MCHIP_NO_MOVE			=0x20	|	MCHIP_NO_WALK;
	const int MCHIP_NO_BORDER		=0x40;
	const int MCHIP_NO_VIEW_OBST	=0x80;
	const int MCHIP_FENCE			=0x100;
	const int MCHIP_NO_SHADOW		=0x200;
	const int MCHIP_THRU_SHADOW		=0x400;
	const int MCHIP_CONVEYER		=0x800;
	const int MCHIP_ANIMATION_FAST	=0x1000;
	const int MCHIP_GET_SHADOW		=0x2000;
	
	const int MCHIP_SYSTEM		=0x10000000 | MCHIP_NO_BORDER | MCHIP_NO_VIEW_OBST | MCHIP_NO_SHADOW | MCHIP_THRU_SHADOW;

const int OBJOPT_BLOCK_SHADOW	=0x00000080;
const int OBJOPT_NO_SHADOW		=0x00000100;
const int OBJOPT_LIGHT_SHADOW	=0x00000200;
const int OBJOPT_SHOW_TURN		=0x00000400;

const int OBJOPT_HEAVY_1		=0x00000800;
const int OBJOPT_HEAVY_2		=0x00001000;

const int OBJOPT_WALL			=0x00006000;
	const int OBJOPT_WALL_0		=0x00002000;
	const int OBJOPT_WALL_1		=0x00004000;
const int OBJOPT_BLUE_SHADOW	=0x00008000;
const int OBJOPT_SMALL_SHADOW	=0x00010000;
const int OBJOPT_NO_GROUND_HIT	=0x00020000;


extern const D3DCOLOR cMapBGColor[MAX_BG_COLOR][2];
extern const int cPlayBGMMap[64];


