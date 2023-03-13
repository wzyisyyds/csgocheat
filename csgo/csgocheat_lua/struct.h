#pragma once

struct vec3
{
	float x = 0;
	float y = 0;
	float z = 0;
};

struct playerinfo
{
	PVOID addr;
	int health;
	bool dormant;
	int team;
	vec3 vec;
	vec3 head;
	vec3 move;
	bool moveflag;
};


struct EntityList
{
	UINT32 addr;
	UINT32 id;
	UINT32 ptr0;
	UINT32 ptr1;
};