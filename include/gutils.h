#ifndef GUTILS_H
#define GUTILS_H

#include <inc.h>

namespace {
	glm::vec2 p1_debug , p2_debug, q1_debug, q2_debug, res_debug;
}

namespace GUtils {
	void drawLineSegDebug();
	bool lineSegIntersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 q1,
				glm::vec2 q2, glm::vec2& res);
}

#endif 
