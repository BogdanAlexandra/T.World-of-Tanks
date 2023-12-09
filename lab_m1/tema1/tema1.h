#pragma once
#define MOVE_RATIO1			(.10f)
#define LIMIT				(.2f)

#include "components/simple_scene.h"
#include "strucures.h"

struct limits {
	int leftlimit;
	int rightlimit;
};
namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void renderscena();
        void renderrombs();
        void renderstars();
        void renderlife();
        void renderEnvironment();
        void CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderInamic(float deltaTimeSeconds);
        void renderromb(float deltaTimeSeconds);
        void RenderInamici(float deltaTimeSeconds);
        void RenderStars(float deltaTimeSeconds, int romb, int i, int j);
        void renderStars();
        void generateStar();
        void rendermoney(float deltaTimeSeconds);
        void misca(float deltaTimeSeconds, int k);
        void misca_proiectile(float deltaTimeSeconds);
        bool IsMouseInsideSquare(int mouseX, int mouseY, const Square& square);
        void RenderDistrugatori(float deltaTimeSeconds);


        protected:

        glm::mat3 modelMatrix;
        glm::mat3 modelMatrix1;
        glm::mat3 modelMatrixs;
        float translateX, translateY;
        float firsthx, firsthy, secondhx, secondhy, starX1, starY1, translatesteax;
        float translateX1, translateY1;
        float scaleX, scaleY;
        float scx, scy;
        float angularStep;
        float rotation = 0;
        float cx, cy;
        bool ok;
        bool direction;
        int i;

        GLboolean moveRight;
        GLboolean moveRightinamic = true;
        GLboolean moveLeft;
        
        Square square1 = {60.0f, 546.0f, 48.0f, 48.0f};
        Square square2 = {135.0f, 548.0f, 48.0f, 48.0f};
        Square square3 = {210.0f, 547.0f, 48.0f, 48.0f};
        Square square4 = {60.0f, 472.0f, 48.0f, 48.0f};
        Square square5 = {135.0f, 472.0f, 48.0f, 48.0f};
        Square square6 = {210.0f, 472.0f, 48.0f, 48.0f};
        Square square7 = {60.0f, 398.0f, 48.0f, 48.0f};
        Square square8 = {135.0f, 398.0f, 48.0f, 48.0f};
        Square square9 = {210.0f, 398.0f, 48.0f, 48.0f};
        Square square10 = {7.5f, 220.0f, 48.0f, 44.0f};
        Square square11 = {100.0f, 220.0f, 48.0f, 44.0f};
        Square square12 = {200.0f, 220.0f, 48.0f, 44.0f};
        Square square13 = {300.0f, 220.0f, 48.0f, 44.0f};


     
    };
}