#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

#include "ObjectTypes.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

class CGameObject
{
protected:

	float x; 
	float y;

	float vx;
	float vy;

	int nx;	 

	int state;

	bool isDeleted; 

	Type type;

public: 
	void SetType(Type type) { this->type = type; }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void SetNx(int nx) { this->nx = nx; }

	Type GetType() { return this->type; }
	int GetState() { return this->state; }
	int GetNx() { return this->nx; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }
	bool CheckAABB(float l_a, float t_a, float r_a, float b_a, float l_b, float t_b, float r_b, float b_b)
	{
		return (l_a < r_b && r_a > l_b && t_a < b_b && b_a > t_b);
	}

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y, Type type) : CGameObject() { this->x = x; this->y = y; this->type = type; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void ChangeDirection() {
		vx = -vx;
		nx = -nx;
	}

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};
