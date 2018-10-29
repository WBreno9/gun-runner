#include <game.h>
#include <empty.h>
#include <light.h>
#include <player.h>
#include <map.h>
#include <box.h>

static void error_callback(int error, const char* desc)
{
        std::clog << desc << std::endl;
}

Game::Game() {
        window = nullptr;
        mainCamera = nullptr;
}

Game::~Game() {
        Entity::SetAllDead();
        Entity::ReapDeadEntities();

        delete renderer;

        glfwDestroyWindow(window);
        glfwTerminate();
}

void Game::createWindow(unsigned w, unsigned h)
{
        std::clog << "Initializing GLFW" << std::endl;
        glfwInit();

        std::clog << "Creating window" << std::endl;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_FLOATING, GL_FALSE);

        window = glfwCreateWindow(w, h, "DIS", 
                        nullptr, nullptr);

        std::clog << "Setting OGL context" << std::endl;
        glfwMakeContextCurrent(window);
        glfwSetErrorCallback(error_callback);

        glfwSwapInterval(1);

        std::clog << "Intializing GLEW" << std::endl;
        glewExperimental = GL_TRUE;
        glewInit();

        std::clog << "---------------------------------------" << std::endl;
        std::clog << "GPU Vendor: "
                  << (const char*)glGetString(GL_VENDOR) << std::endl;

        std::clog << "Renderer: "
                  << (const char*)glGetString(GL_RENDERER) << std::endl;

        std::clog << "OpenGL Version: "
                  << (const char*)glGetString(GL_VERSION) << std::endl;
        std::clog << "---------------------------------------" << std::endl;

        width = w;
        height = h;

        Entity::window = window;

        renderer = new Renderer();
        Entity::m_renderer = renderer;

        physics = new PhysicsManager();
        Entity::m_physics = physics;
}

void Game::start()
{
        if (window == nullptr) {
                std::cerr << "Failed to start game: no window" << std::endl;
                return;
        }

        delta = 0;
        pastTime = 0;
        currentTime = 0;

        restart();
        mainLoop();
}

void Game::updateDelta()
{
        currentTime = (float)glfwGetTime();
        delta = currentTime - pastTime;
        pastTime = currentTime;

        Entity::delta = delta;
}

void Game::restart()
{
        Entity::SetAllDead();
        Entity::ReapDeadEntities();

        glfwSetCursorPos(Entity::window, width/2, height/2);

        Game::mainCamera = new Camera(90.f, (float)width/height, 0.1f, 100.f);
        mainCamera->transform.translate(glm::vec3(1.f), Transform::WORLD_RELATIVE);
        mainCamera->LookAt(glm::vec3(0.f));

        EPlayer* player = new EPlayer(Game::mainCamera);
        player->transform.translate(glm::vec3(0.f, 1.f, 0.f), Transform::WORLD_RELATIVE);

        //EMap* map = new EMap("test_map");

        EBox* box = new EBox(1.f, 0.01f);
        box->transform.translate(glm::vec3(-1.f, 1.f, 0.f), Transform::WORLD_RELATIVE);

        EBox* box1 = new EBox(10.f, 0.f);
        box1->transform.translate(glm::vec3(0.f, -4.f, 0.f), Transform::WORLD_RELATIVE);
        
        ELight* light = new ELight();
        light->transform.m_pos =  glm::vec3(5.f, 7.f, 5.f);
        light->setColor(glm::vec3(20.f));
        //light->lifeTime = 2.0f;

        //ELight* light1 = new ELight();
        //light1->transform.m_pos =  glm::vec3(-5.f, 3.f, 0.f);
        //light1->setColor(glm::vec3(15.f));

}

void Game::DrawAll()
{
        renderer->bindModelVBO();

        glm::mat4 v = mainCamera->GetViewMatrix();
        glm::mat4 p = mainCamera->GetProjMatrix();

	for (Entity* ent = Entity::GetHead(); ent != nullptr;
		ent = ent->GetNext()) 
        {
                glm::mat4 m = ent->transform.getMatrix();
                if (ent->modelActive != false && ent->visible) {
                        renderer->drawModel(ent->modelHandle, m, v, p);
                }
        }
}

void Game::mainLoop()
{	
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        float currentTime, pastTime;
        while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();

                updateDelta();

                int height, width;
                glfwGetFramebufferSize(window, &width, &height);
                float ratio = width / (float)height;	

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glViewport(0, 0, width, height);

                physics->destroyInactiveRigidBodies();	
                physics->step();	
                physics->updateTransforms();

                Entity::UpdateAll();

		renderer->destroyInactiveLights();
                renderer->updateLights();
                DrawAll();

                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, GL_TRUE);

                if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
                        restart();

                glfwSwapBuffers(window);
        }
}
