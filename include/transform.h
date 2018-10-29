#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <inc.h>

struct Transform {
	glm::vec3 m_pos;

	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;

	glm::vec3 m_scale;

	glm::quat m_orientation;

	enum {
		WORLD_RELATIVE,
		SELF_RELATIVE
	};

	void setIndentity();

	void rotate(glm::vec3 rotation, int relativeTo);
	void translate(glm::vec3 translation, int relativeTo);
	void scale(glm::vec3 scale, int relativeTo);

	glm::mat4 getMatrix();
	glm::vec3 getForward();
};

#endif
