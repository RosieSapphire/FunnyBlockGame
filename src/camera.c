#include <math.h>
#include "camera.h"
#include "rmath/mat4.h"
#include "rmath/vec3f.h"

void camera_get_look_pos(struct camera c, rm_vec3f out)
{
	rm_vec3f look_pos = {cosf(c.yaw), 0.0f, sinf(c.yaw)};

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

void camera_update_position(struct camera *c, GLFWwindow *win, float dt)
{
	float move_speed = RM_PI * dt;
	float turn_speed = RM_PI * 0.5f * dt;

	if(glfwGetKey(win, GLFW_KEY_LEFT_SHIFT)) {
		move_speed *= 2;
		turn_speed *= 2;
	}

	float turn_amount = (glfwGetKey(win, GLFW_KEY_D) -
			glfwGetKey(win, GLFW_KEY_A)) * turn_speed;
	float forward_move = (glfwGetKey(win, GLFW_KEY_S) -
			glfwGetKey(win, GLFW_KEY_W));
	float right_move = (glfwGetKey(win, GLFW_KEY_Q) -
			glfwGetKey(win, GLFW_KEY_E));

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

	c->yaw += turn_amount;
}
