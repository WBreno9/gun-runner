#include <transform.h>

void Transform::setIndentity() {
	m_pos = glm::vec3(0.f);
	m_scale = glm::vec3(1.f);

	m_forward = glm::vec3(0.f, 0.f, -1.f);
	m_right = glm::vec3(1.f, 0.f, 0.f);
	m_up = glm::vec3(0.f, 1.f, 0.f);

	m_orientation = glm::quat(glm::vec3(0.f));
}

void Transform::rotate(glm::vec3 rotation, int relativeTo) {
	glm::quat orientation;

	if (relativeTo == SELF_RELATIVE) {
		orientation = glm::angleAxis(rotation.x, m_right) *
			glm::angleAxis(rotation.y, m_up) * 
			glm::angleAxis(rotation.z, m_forward);
	} else if (relativeTo == WORLD_RELATIVE) {
		orientation = glm::quat(rotation);
	}

	orientation = glm::normalize(orientation);

	m_right   = orientation * m_right;
	m_forward = orientation * m_forward;
	m_up      = orientation * m_up;

	m_orientation *= orientation;
		m_orientation = glm::normalize(m_orientation);
}

void Transform::translate(glm::vec3 translation, int relativeTo) {
	if (relativeTo == SELF_RELATIVE) {
		m_pos += m_right * translation.x; 
		m_pos += m_up * translation.y; 
		m_pos += m_forward * translation.z; 
	} else if (relativeTo == WORLD_RELATIVE) {
		m_pos.x += translation.x;
		m_pos.y += translation.y;
		m_pos.z += translation.z;
	}
}

void Transform::scale(glm::vec3 scale, int relativeTo) {
	if (relativeTo == SELF_RELATIVE) {
		m_scale += scale.x * m_right;
		m_scale += scale.y * m_up;
		m_scale += scale.z * m_forward; 
	} else if (relativeTo == WORLD_RELATIVE) {
		m_scale.x += scale.x;
		m_scale.y += scale.y;
		m_scale.z += scale.z;
	}
}

glm::mat4 Transform::getMatrix() {
	return  glm::translate(m_pos) * 
		glm::mat4_cast(m_orientation) * 
		glm::scale(m_scale);
}