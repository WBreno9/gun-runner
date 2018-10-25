#ifndef ENTITY_H
#define ENTITY_H

#include <inc.h>
#include <renderer.h>

#include <map>

struct Transform {
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 rotation;
	glm::vec3 scale;

	void setIndentity() {
		pos = glm::vec3(0.f);
		forward = glm::vec3(0.f, 0.f, -1.f);
		rotation = glm::vec3(0.f);
		scale = glm::vec3(1.f);
	}
};

class Entity {
public:
	static GLFWwindow* window;
	static float delta;

	static Renderer* m_renderer;

	bool isDead;
	float lifeTime;
	float lifeTimeCount;

	int health;
	int maxHealth;

	bool visible;

	bool modelActive;
	ModelHandle modelHandle;

	Transform transform;

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
