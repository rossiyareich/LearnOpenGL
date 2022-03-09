#pragma once

#define WINDOW_W 800
#define WINDOW_H 600
#define MOUSE_SENSITIVITY 0.1f

#define GL_ERROR_LENGTH 512
#include <glm/vec3.hpp>
using glErrorLog = char[GL_ERROR_LENGTH];

#define glm_vec3_up glm::vec3(0.0f, 1.0f, 0.0f)
#define glm_vec3_right glm::vec3(1.0f, 0.0f, 0.0f)
#define glm_vec3_front glm::vec3(0.0f, 0.0f, -1.0f)
