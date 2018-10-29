#ifndef ENTITY_H
#define ENTITY_H

#include <inc.h>
#include <renderer.h>
#include <transform.h>
#include <physics.h>

#include <map>

class Entity {
public:
	static GLFWwindow* window;
	static float delta;

	static Renderer* m_renderer;
	static PhysicsManager* m_physics;

	bool isDead;
	float lifeTime;
	float lifeTimeCount;

	int health;
	int maxHealth;

	bool visible;

	bool modelActive;
	RModelHandle modelHandle;

	Transform transform;

	PRigidBody* rigidBody;

	Entity();
	virtual ~Entity();

	static Entity* GetTail() { return tail; }
	static Entity* GetHead() { return head; }

	static void SetAllDead();
	static void ReapDeadEntities();
	static void UpdateAll();

	virtual std::string GetTypeString() = 0;

	virtual void Update() = 0;

	virtual void Damage(unsigned d);

	void UpdateMatrix();
	void UpdateVectors();

	Entity* FindByType(std::string);
	Entity* FindNearestByType(std::string);

 	Entity* GetNext() { return next; }
	Entity* GetPrev() { return prev; }
private:
	Entity* prev;
	Entity* next;

	static Entity* tail;
	static Entity* head;
};

#endif
