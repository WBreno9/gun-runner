#include <entity.h>

Entity* Entity::tail = nullptr;
Entity* Entity::head = nullptr;
GLFWwindow* Entity::window = nullptr;
float Entity::delta = 0;

const std::map<std::string, ModelHandle>* Entity::modelMap = nullptr;


Entity::Entity() 
{
	lifeTime = 0.0f;
	lifeTimeCount = 0.0f;
	isDead = false;

        modelActive = false;
        modelHandle = 0;

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

void Entity::UpdateMatrix()
{
	model_matrix = glm::translate(glm::mat4(1.0f), 
					glm::vec3(pos));
        /*
	model_matrix = glm::rotate(model_matrix, rotation, 
					glm::vec3(0.0f, 0.0f, 1.0f));
                                        */
        /*
	model_matrix = glm::scale(model_matrix, glm::vec3(size));
        */
}

void Entity::UpdateVectors()
{
}

void Entity::Damage(unsigned d)
{
	health -= d;

	if (health <= 0) 
		isDead = true;
}

void Entity::SetModel(const std::string &modelName) {
        modelActive = true;

        auto result = modelMap->find(modelName);
        if (result != modelMap->end()) {
                modelHandle = result->second;
        } else {
                std::clog << "Failed to find Model: " + modelName
                          << std::endl;
                modelHandle = 0;
        }
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
				dist = glm::distance(pos, ent->pos);
				nearest = ent;
			}

			if ((tmp = glm::distance(pos, ent->pos)) < dist) {
				dist = tmp;
				nearest = ent;
			}
		}
	}

	return nearest;
}
