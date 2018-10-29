#include <entity.h>

Entity* Entity::tail = nullptr;
Entity* Entity::head = nullptr;
GLFWwindow* Entity::window = nullptr;
float Entity::delta = 0;
Renderer* Entity::m_renderer = nullptr;
PhysicsManager* Entity::m_physics = nullptr;

Entity::Entity() 
{
	lifeTime = 0.0f;
	lifeTimeCount = 0.0f;
	isDead = false;

        modelActive = false;
        modelHandle = 0;

	transform.setIndentity();

	if (head == nullptr) {
		tail = head = this;
		next = prev = nullptr;
	} else {
		tail->next = this;
		prev = tail;
		tail = this;
		next = nullptr;
	}
}

Entity::~Entity()
{
	if (next != nullptr)
		next->prev = prev;
	if (prev != nullptr)
		prev->next = next;
	if (this == tail)
		tail = prev;
	if (this == head)
		head = next;
}

void Entity::SetAllDead()
{
	for (Entity* ent = Entity::GetHead(); ent != nullptr;
		ent = ent->GetNext()) 
			ent->isDead = true;
}

void Entity::ReapDeadEntities()
{
	Entity* ent = head;
	Entity* tmp;

	while (ent != nullptr) {
		tmp = ent->next;

		if (ent->isDead)
			delete ent;

		ent = tmp;
	}
}

void Entity::UpdateAll()
{
	for (Entity* ent = Entity::GetHead(); ent != nullptr;
		ent = ent->GetNext()) 
	{	
		if (ent->lifeTime > 0.0f) {	
			ent->lifeTimeCount += delta;

			if (ent->lifeTimeCount > ent->lifeTime) 
				ent->isDead = true;
		}

		ent->Update();
	}
	ReapDeadEntities();
}

void Entity::Damage(unsigned d)
{
	health -= d;
	if (health <= 0) 
		isDead = true;
}

Entity* Entity::FindByType(std::string arg)
{
	for (Entity* ent = Entity::GetHead(); ent != nullptr;
		ent = ent->GetNext()) 
	{
		if (ent->GetTypeString() == arg && ent != this)
			return ent;
	}

	return nullptr;
}
	
Entity* Entity::FindNearestByType(std::string arg)
{
	float tmp, dist = -1.0f;
	Entity* nearest = nullptr;
	for (Entity* ent = Entity::GetHead(); ent != nullptr;
		ent = ent->GetNext()) 
	{
		if (ent->GetTypeString() == arg) {
			if (dist < 0.0f) {
				dist = glm::distance(transform.m_pos, ent->transform.m_pos);
				nearest = ent;
			}

			if ((tmp = glm::distance(transform.m_pos, ent->transform.m_pos)) < dist) {
				dist = tmp;
				nearest = ent;
			}
		}
	}

	return nearest;
}
