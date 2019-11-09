#ifndef CAMARA_H
#define CAMARA_H

#include <iostream>

//#include "glm/glm.hpp"
//#include "Eigen/Eigen"
#include "predef_types.h"


#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace camara_np {
	class Camara {
	public:
			v3f cam_pos = { 0,0,3 };
			v3f cam_front = { 0,0,-1 };
			v3f cam_up = { 0,1,0 };
		virtual  void key_handle(unsigned char c,int x,int y) {}
		virtual  void on_mouse_move(int x, int y) {}
		virtual void mouse_click_handle(int button, int state, int x, int y) {}
	protected:
			v2f mouse = { 400,300 };
			v2f angle = { 0,0 };
			bool first_enter_screen = true;

	};

	class Walking_Camara :public Camara {
	public:
		virtual  void key_handle(unsigned char c,int x,int y) {
			GLfloat speed = 1e-1;
			v3f right = cam_front.cross(cam_up);
				switch (c)
				{
				case 'a':
					cam_pos -= speed*right;
					break;
				case 'd':
					cam_pos += speed*right;
					break;
				case 'w':
					cam_pos += speed*cam_front;
					break;
				case 's':
					cam_pos -= speed*cam_front;
					break;

				default:
					break;
				}
		}
		virtual  void on_mouse_move(int x, int y) {

			//cam_front = glm::normalize(glm::vec3(x, y, 0) - cam_pos);
			v2f current_mouse = { x,y };
			//if (first_enter_screen) {
			//	first_enter_screen = false;
			//	mouse = current_mouse;
			//}
			v2f del = current_mouse - mouse;
			mouse = current_mouse;
			GLfloat sensitive = 1e-3;
			del *= sensitive;
			angle += del;
			if (angle[0] > 89.0f) {
				angle[0] = 90.0f;
			}
			if (angle[1] <- 89.0f) {
				angle[1] = -90.0f;
			}
			v3f front;
			front[2] = -cos(angle[1])*cos(-angle[0]);
			front[0] = cos(angle[1])*sin(-angle[0]);
			front[1] = sin(angle[1]);
			cam_front = front.normalized();
			v3f right;
			right[0] = cos(-angle[0]);
			right[2] = sin(-angle[0]);
			right[1] = 0;
			right.normalize();
			v3f up = right.cross( front);
			cam_up = up;
			
			
		}
		virtual void mouse_click_handle(int button, int state, int x, int y) {
			v2f current_mouse = { x,y };
			if (button == GLFW_MOUSE_BUTTON_LEFT&&state == GLFW_RELEASE) {
				first_enter_screen = true;
				std::cout << state<<" "<<first_enter_screen << std::endl;

			}
			if (button == GLFW_MOUSE_BUTTON_LEFT&&state==GLFW_PRESS) {
				first_enter_screen = false;
				mouse = current_mouse;
				std::cout << state<<" "<<first_enter_screen <<std:: endl;
			}
		}

	};
//
//	class Focus_Camara :public Walking_Camara {
//		glm::vec3 focus_center = { 0, 0, 0 };
//	public:	
//		virtual  void on_mouse_move(int x, int y) {
//			glm::vec2 current_mouse = { x,y };
//			glm::vec2 del = current_mouse - mouse;
//			mouse = current_mouse;
//			GLfloat sensitive = 1e-3;
//			del *= -sensitive;
//			angle += del;
//			if (angle[0] > 89.0f) {
//				angle[0] = 90.0f;
//			}
//			if (angle[1] <- 89.0f) {
//				angle[1] = -90.0f;
//			}
//			glm::vec3 front;
//			front.z = -cos(angle[1])*cos(-angle[0]);
//			front.x = cos(angle[1])*sin(-angle[0]);
//			front.y = sin(angle[1]);
//			cam_front = glm::normalize(front);
//			auto position_vector = cam_pos - focus_center;
//			auto distance = sqrt(glm::dot(position_vector, position_vector));
//			cam_pos = focus_center + (-cam_front)*distance;
//
//
//			glm::vec3 right;
//			right.x = cos(-angle[0]);
//			right.z = sin(-angle[0]);
//			right.y = 0;
//			right = glm::normalize(right);
//			glm::vec3 up = glm::cross(right, front);
//			cam_up = up;
//			
//		}
//	};

}



#endif