#include <assert.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "mesh.h"
#include "render_layer.h"
#include "rmath/vec3f.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

#define WIDTH 1920
#define HEIGHT 1080
#define ASPECT_RATIO ((float)WIDTH / (float)HEIGHT)

GLFWwindow *window_create_centered(int width, int height, const char *title);

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = window_create_centered(WIDTH, HEIGHT,
			"PS1 Graphics Test");

	assert(glfwRawMouseMotionSupported());

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	gladLoadGL(glfwGetProcAddress);
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	struct render_layer *layer = render_layer_create(WIDTH, HEIGHT);

	GLuint cube_shader =
		shader_create("shaders/base.vert", "shaders/base.frag");
	GLuint fbo_shader =
		shader_create("shaders/fbo.vert", "shaders/fbo.frag");

	int projection_loc = shader_get_loc(cube_shader, "u_projection");
	int view_loc = shader_get_loc(cube_shader, "u_view");
	int view_pos_loc = shader_get_loc(cube_shader, "u_view_pos");
	int model_loc = shader_get_loc(cube_shader, "u_model");

	rm_mat4 projection;
	struct camera cam = {{0, 0, 2}, RM_PI * 0.5f, 0};

	rm_mat4_perspective(70.0f, ASPECT_RATIO, 0.1f, 50, projection);

	struct mesh *test_mesh = mesh_create_file("models/sphere.glb");
	// GLuint crate_texture = texture_load("textures/test.png");

	float time_last = glfwGetTime();

	while(!glfwWindowShouldClose(window)) {
		rm_mat4 model;
		rm_mat4 view;
		float time_now = glfwGetTime();
		float time_delta = time_now - time_last;

		time_last = time_now;

		camera_get_view_mat4(cam, view);
		camera_update_rotation(&cam, window, 0.0006f);
		camera_update_position(&cam, window, time_delta);
		test_mesh->rot[1] += time_delta;

		render_layer_bind_and_clear(layer, 0.05f, 0.1f, 0.2f, 1.0f);

		mesh_get_model_mat4(*test_mesh, model);

		glUseProgram(cube_shader);
		shader_uni_mat4(model_loc, model);
		shader_uni_mat4(view_loc, view);
		shader_uni_mat4(projection_loc, projection);
		shader_uni_vec3f(view_pos_loc, cam.eye_pos);
		// mesh_draw(test_mesh, crate_texture);
		mesh_draw(test_mesh, cube_shader, 0);

		render_layer_draw(layer, fbo_shader, WIDTH, HEIGHT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(fbo_shader);
	glDeleteProgram(cube_shader);
	// glDeleteTextures(1, &crate_texture);
	mesh_destroy(test_mesh);
	render_layer_destroy(layer);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

GLFWwindow *window_create_centered(int width, int height, const char *title)
{
	GLFWwindow *window = glfwCreateWindow(width, height, title,
			NULL, NULL);

	glfwMakeContextCurrent(window);

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	int monitor_w, monitor_h;
	int window_x, window_y;

	glfwGetMonitorWorkarea(monitor, NULL, NULL, &monitor_w, &monitor_h);
	window_x = (monitor_w >> 1) - (width >> 1);
	window_y = (monitor_h >> 1) - (height >> 1);
	glfwSetWindowPos(window, window_x, window_y);

	return window;
}
