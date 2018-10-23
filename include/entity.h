#ifndef ENTITY_H
#define ENTITY_H

#include <inc.h>
#include <renderer.h>

#include <map>

class Entity
{
public:
	static GLFWwindow* window;
	static float delta;

	bool isDead;
	float lifeTime;

	int health;
	int maxHealth;

	bool visible;

        bool modelActive;
        ModelHandle modelHandle;
        void SetModel(const std::string &modelName);
        static const std::map<std::string, ModelHandle> *modelMap;

        /* TODO */
        /* Create a transform class */
	glm::vec3 pos;
	glm::vec3 forward;
        glm::vec3 rotation;
        glm::vec3 scale;

	float DeAccelMul;
	glm::vec3 accelDir;

	glm::vec3 BBoxMax;
	glm::vec3 BBoxMin;

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
	float realRotation;

	float lifeTimeCount;
	glm::mat4 model_matrix;

	Entity* prev;
	Entity* next;

	static Entity* tail;
	static Entity* head;

	void DrawBoundingBox();
	static void DrawPlayArea();
};

#endif
