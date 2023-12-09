#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera_hw.h"
#include "lab_m1/tema2/structures.h"
#include "lab_m1/tema2/structures.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:

     struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;


     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::mat4 proj, glm::mat4 view,const glm::vec3 &color);
        void RenderPlayer();
        void RenderEnemies(float deltaTimeSeconds);
        void RenderProjectiles(float deltaTimeSeconds);
        void RenderCladiri(float deltaTimeSeconds);
        void RenderSimpleMeshdisp(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::mat4 proj, glm::mat4 view,const glm::vec3 &color, const float hp);


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderMeshPlan(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));


     protected:
        implemented::Camera_hw *camera;
        glm::mat4 projectionMatrix;
        glm::mat4 carMatrix;
        bool renderCameraTarget;
        GLenum polygonMode;
        ViewportArea miniViewportArea;

        int viewportX = 0;          // Poziția orizontală a viewportului mic
        int viewportY = 0;          // Poziția verticală a viewportului mic
        int viewportWidth = 200;    // Lățimea viewportului mic
        int viewportHeight = 200;   // Înălțimea viewportului mic
        glm::vec3 lightPosition;
        glm::vec3 tankPosition;
        glm::mat4 modelMatrix;
        float tankRotationAngle;
        float angle;
        float currTime = 65;
        bool rightMouseButtonPressed = false;
        float cameraRotationAngle = 0.0f;


        std::vector<Enemy> enemyList;
        float timetochange;

        std::vector<Sphere> projectiles;
        glm::mat4 projectilesm;

    };
}   // namespace m1
