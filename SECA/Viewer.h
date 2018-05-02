#ifndef SECA_VIEWER_H_
#define SECA_VIEWER_H_

#include <memory>
#include <vector>
#include <assert.h>
#include <string>
#include <malloc.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "util/Log.h"

#include "component/InputComponent.h"
#include "component/PhysicsComponent.h"
#include "component/GraphicsComponent.h"

#include "scene/Scene.h"
#include "scene/OBJViewerScene.h"

//void OnScrollStub(GLFWwindow * window, double offsetx, double offsety)
//{
//	ImGui_ImplGlfwGL3_ScrollCallback(window, offsetx, offsety);
//
//	glfwGetWindowUserPointer(window);
//
//	g_camera->OnScroll(offsetx, offsety);
//}
//
//void OnMouseButtonStub(GLFWwindow * window, int button, int action, int mods)
//{
//	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
//
//	glfwGetWindowUserPointer(window);
//
//	g_camera->OnMouseButton(window, button, action, mods);
//}

namespace seca
{
	class Viewer
	{
	private:
		GLFWwindow * m_window;
		std::vector<std::shared_ptr<scene::Scene>> m_scenes;
		std::list<component::InputComponent*> m_inputComponents;
		std::list<component::PhysicsComponent*> m_physicsComponents;
		std::list<component::GraphicsComponent*> m_graphicsComponents;

		component::ImguiInputComponent* m_imguiComponenetsMain;
		std::list<component::ImguiInputComponent*> m_imguiComponents;

		render::World *world;

		int m_windowW = 0;
		int m_windowH = 0;

		struct SceneWindow
		{
			std::string m_windowTitle = "tmp";
			GLuint m_texture_id;
			GLuint* m_scenePixel;
		};
		std::vector<SceneWindow*> m_sceneWindow;


	public:
		Viewer(int _w = 1920, int _h = 1080)
			: m_windowW(_w), m_windowH(_h)
		{
			if (!glfwInit()) assert("failed glfwinit");

			m_window = glfwCreateWindow(m_windowW, m_windowH, "Viewer", NULL, NULL);

			if (!m_window)
			{
				glfwTerminate();
				assert("failed create glfw");
			}

			glfwMakeContextCurrent(m_window);

			if (gl3wInit())
			{
				assert("failed gl3winit");
			}

			ImGui_ImplGlfwGL3_Init(m_window, true);
			ImGui::StyleColorsClassic();

			m_inputComponents.clear();
			component::InputComponent *t_input
				= new component::ImguiInputComponent();
			m_imguiComponenetsMain = (component::ImguiInputComponent*)t_input;
			m_imguiComponents.push_back((component::ImguiInputComponent*)t_input);

			world = new render::World(m_windowW, m_windowH);

			//glfwSetMouseButtonCallback(m_window, OnMouseButtonStub);
			//glfwSetScrollCallback(m_window, OnScrollStub);

			SECA_CONSOLE_INFO("HELLO");
		}

		~Viewer()
		{
			ImGui_ImplGlfwGL3_Shutdown();
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		void reset()
		{
			for (std::shared_ptr<scene::Scene> s : m_scenes) {
				s->reset();
			}
		}

		void setup()
		{
			reset();
			for (std::shared_ptr<scene::Scene> s : m_scenes) {
				s->setup();

				// pixel
				m_sceneWindow.push_back(new SceneWindow());
				m_sceneWindow.back()->m_texture_id = m_sceneWindow.size() + 1;
				m_sceneWindow.back()->m_windowTitle = "scene " + std::to_string(m_sceneWindow.size());
				m_sceneWindow.back()->m_scenePixel = s->getPixel();
			}
		}

		void addScene()
		{
			// init Components
			component::CameraInputComponent *inputCamera
				= new component::CameraInputComponent();

			component::ImguiInputComponent *inputImgui
				= new component::ImguiInputComponent;

			component::PhysicsComponent *physics
				= new component::PhysicsComponent();

			component::OBJRenderComponent *renderOBJ
				= new component::OBJRenderComponent();

			std::shared_ptr<scene::OBJViewerScene> scene
					= std::make_shared<scene::OBJViewerScene>
					(inputCamera, inputImgui, physics, renderOBJ);


			scene->setWorld(world);
			scene->setSceneSize(m_windowW, m_windowH);
			scene->setSingleMode(true);
			scene->setup();

			m_inputComponents.push_back(inputImgui);
			m_physicsComponents.push_back(physics);
			m_graphicsComponents.push_back(renderOBJ);

			m_scenes.push_back(scene);

			// SECA_CONSOLE_INFO("add new scene");
		}

		void run()
		{
			setup();

			bool show_demo_window = false;
			ImVec4 param_color_clear = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			while (!glfwWindowShouldClose(m_window))
			{
				int width, height;
				glfwMakeContextCurrent(m_window);
				glfwGetFramebufferSize(m_window, &width, &height);
				glViewport(0, 0, width, height);
				glClearColor(
					m_imguiComponenetsMain->getVec4Clear()->x,
					m_imguiComponenetsMain->getVec4Clear()->y,
					m_imguiComponenetsMain->getVec4Clear()->z,
					m_imguiComponenetsMain->getVec4Clear()->w
				);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				for (component::InputComponent* _inputComponent
					: m_inputComponents)
				{
					_inputComponent->update();
				}

				for (component::PhysicsComponent* _physicsComponenet
					: m_physicsComponents)
				{
					_physicsComponenet->update();
				}

				for (component::GraphicsComponent* _graphicsComponenet
					: m_graphicsComponents) {
					_graphicsComponenet->render();
				}

				glfwMakeContextCurrent(m_window);
				ImGui_ImplGlfwGL3_NewFrame();

				// GUI Input Compnents
				ImTextureID tmp_fontTexture_id = ImGui::GetIO().Fonts->TexID;
				glBindTexture(GL_TEXTURE_2D, (GLuint)tmp_fontTexture_id);
				for (component::ImguiInputComponent* _imguiComponent
					: m_imguiComponents)
				{
					_imguiComponent->render();
				}

				// Scene Componenets
				for (SceneWindow* sceneWindow : m_sceneWindow)
				{
					ImGui::Begin(sceneWindow->m_windowTitle.c_str());
					glBindTexture(GL_TEXTURE_2D, sceneWindow->m_texture_id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 400, 400, 0,
						GL_RGBA, GL_UNSIGNED_INT, sceneWindow->m_scenePixel);
					ImGui::Image((ImTextureID)sceneWindow->m_texture_id,
						ImVec2(400, 400),
						ImVec2(0, 1),
						ImVec2(1, 0)
					);
					ImGui::End();
				}
				ImGui::Render();

				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}
		}
	};
}


#endif // SECA_VIEWER_H_