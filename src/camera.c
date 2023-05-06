#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "camera.h"
#include "rmath/helpers.h"
#include "rmath/mat4.h"
#include "rmath/vec3f.h"

void camera_get_look_pos(struct camera c, rm_vec3f out)
{
	rm_vec3f look_pos = {
		cosf(c.yaw * RM_PI) * cosf(c.pitch * RM_PI),
		sinf(c.pitch * RM_PI),
		sinf(c.yaw * RM_PI) * cosf(c.pitch * RM_PI),
	};

	rm_vec3f_add(c.eye_pos, look_pos, out);
}

void camera_get_forward_vec(struct camera c, rm_vec3f out)
{
	rm_vec3f look_pos;

	camera_get_look_pos(c, look_pos);
	rm_vec3f_sub(look_pos, c.eye_pos, out);
	rm_vec3f_normalize(out);
}

void camera_get_right_vec(struct camera c, rm_vec3f out)
{
	rm_vec3f forward_vec;

	camera_get_forward_vec(c, forward_vec);
	rm_vec3f_cross(forward_vec, RM_VEC3F_Y, out);
	rm_vec3f_normalize(out);
}

void camera_get_view_mat4(struct camera c, rm_mat4 out)
{
	rm_vec3f look_pos;

	camera_get_look_pos(c, look_pos);
	rm_mat4_look_at(c.eye_pos, look_pos, out);
	camera_get_forward_vec(c, look_pos);
}

void camera_update_rotation(struct camera *c, GLFWwindow *win, float sens)
{
	double mouse_x, mouse_y;

	glfwGetCursorPos(win, &mouse_x, &mouse_y);
	mouse_y = rm_clampf(mouse_y, -830, 830);
	glfwSetCursorPos(win, mouse_x, mouse_y);

	c->yaw = mouse_x * sens;
	c->pitch = mouse_y * sens;
}

void camera_update_position(struct camera *c, GLFWwindow *win, float dt)
{
	float move_speed = RM_PI * dt;

	if(glfwGetKey(win, GLFW_KEY_LEFT_SHIFT)) {
		move_speed *= 2;
	}

	float forward_move = (glfwGetKey(win, GLFW_KEY_S) -
			glfwGetKey(win, GLFW_KEY_W));
	float right_move = (glfwGetKey(win, GLFW_KEY_A) -
			glfwGetKey(win, GLFW_KEY_D));

	rm_vec3f forward_vec;
	rm_vec3f right_vec;
	rm_vec3f move_vec;

	camera_get_forward_vec(*c, forward_vec);
	camera_get_right_vec(*c, right_vec);
	rm_vec3f_scale(forward_vec, forward_move);
	rm_vec3f_scale(right_vec, right_move);

	rm_vec3f_add(forward_vec, right_vec, move_vec);
	rm_vec3f_normalize(move_vec);
	rm_vec3f_scale(move_vec, move_speed);
	rm_vec3f_add(c->eye_pos, move_vec, c->eye_pos);
}
