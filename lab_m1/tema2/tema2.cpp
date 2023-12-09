#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/structures.h"
#include "lab_m1/lab4/lab4.h"
#include "lab_m1/lab4/transform3D.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <unordered_map>

#define GRASSP -1

using namespace std;
using namespace m1;

float timeSinceLastenemy = 0.0f;
float timeSinceLastprojectil = 0.0f;
float timeSinceLastcladire = 0.0f;
std::unordered_map<float, std::vector<Sphere>> angleProjectiles;
std::vector<Box> cladiri;
bool b1 = false;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}
float orthoLeft = -8.0f;
float orthoRight = 8.0f;
float orthoUp = 4.0f;
float orthoDown = -4.5f;
float FOV = glm::radians(60.0f);
float LENGTH = 4.0f;

void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera_hw();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

   
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);


    {
        Mesh *mesh = new Mesh("tanc");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tinker.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("senila");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senila.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("DisplacementShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "DisplacementVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "DisplacementFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
     // Light & material properties
    {
        lightPosition = glm::vec3(0, 0, 0);
        tankPosition = glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z);
        tankRotationAngle = 0.0f;
        angle = 0;
        
    }

    
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}



void Tema2::Update(float deltaTimeSeconds)
{
    currTime -= deltaTimeSeconds;
    if (currTime <= 0) {
        cout << "Game over!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        exit(1);
    }

    RenderPlayer();

    //Ground
    glm::mat4 modelMatrix0 = glm::mat4(1);
    modelMatrix0 = glm::translate(modelMatrix0, glm::vec3(0, 0, 0));
    modelMatrix0 = glm::scale(modelMatrix0, glm::vec3(1, 0.2, 1));
    RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix0, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0, 0.55, 0));
    RenderEnemies(deltaTimeSeconds);

    RenderProjectiles(deltaTimeSeconds);
    RenderCladiri(deltaTimeSeconds);

    //coliziune player-cladiri
     for(auto &clad : cladiri){
         float distance = glm::length(camera->GetTargetPosition() - clad.positions);
         
                if(distance < 1.2f) {
                    for (auto& enemy : enemyList) {
                    float dist = glm::length(enemy.position - clad.positions);
                    if(dist <= 0.5f)
                    {enemy.position += glm::vec3(0, 0, enemy.direction* 0.5);}
                    
                     else {
                        if(distance < 1.2f)
                       {camera->TranslateRight(0.1);}
                       }
                    }      
                }


    }

    //viewport

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    glClearColor(0.529, 0.808, 0.922, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    RenderPlayer();

    //ground
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1.5, 1.0));
    RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0, 0.55, 0));
    RenderEnemies(deltaTimeSeconds);

    RenderProjectiles(deltaTimeSeconds);
    RenderCladiri(deltaTimeSeconds);

    //coliziune player-cladiri
     for(auto &clad : cladiri){
         float distance = glm::length(camera->GetTargetPosition() - clad.positions);
         
                if(distance < 1.2f) {
                    for (auto& enemy : enemyList) {
                    float dist = glm::length(enemy.position - clad.positions);
                    if(dist <= 0.5f)
                    {enemy.position += glm::vec3(0, 0, enemy.direction* 0.5);}
                    
                     else {
                        if(distance < 1.2f)
                       {camera->TranslateRight(0.1);}
                       }
                    }      
                }


    }
     
}


glm::vec3 generateRandomPosition() {
    static bool isPositive = true;  // Static variable to keep track of the sign
    glm::vec3 randomPosition;

    randomPosition.y = 0.0f;

    // Alternate between positive and negative x values
    randomPosition.x = isPositive ? static_cast<float>(rand() % 300) / 100.0f : -static_cast<float>(rand() % 300) / 100.0f;

    int possibleZValues[] = {4, 5, -4};

    int randomIndex = rand() % 3;

    randomPosition.z = static_cast<float>(possibleZValues[randomIndex]);

    // Toggle the flag for the next call
    isPositive = !isPositive;

    return randomPosition;
}

int getRandomOneOrMinusOne() {
    srand(static_cast<unsigned int>(time(0)));
    int randomValue = rand() % 2;
    return (randomValue == 0) ? 1 : -1;
}

void Tema2::RenderCladiri(float deltaTimeSeconds){
glm::vec3 randomPosition1 = generateRandomPosition();
timeSinceLastcladire += deltaTimeSeconds;
 if (timeSinceLastcladire >= 2 && cladiri.size() < 7) {

          Box cladire(meshes["box"], false, glm::vec3(0.5f + randomPosition1.x, 2.5f, 0.4f - randomPosition1.z));
          cladiri.push_back(cladire);
          timeSinceLastcladire = 0.0f;
    }
    for (int i = 0; i < cladiri.size(); i++) {
        for (auto& enemy : enemyList) {
            float dist = glm::length(enemy.position - cladiri[i].positions);
            if(dist <= 0.5f)
            cladiri[i].active = true;
        }

    }
    for (auto& clad : cladiri){
        if(clad.active == false){
        glm::mat4 modelMatrixc = glm::mat4(1);
        modelMatrixc = glm::translate(modelMatrixc, clad.positions);
        modelMatrixc = glm::scale(modelMatrixc, glm::vec3(1.0f, 2.9f, 0.9f));
        RenderSimpleMesh(clad.box, shaders["LabShader"], modelMatrixc, projectionMatrix, camera->GetViewMatrix(), glm::vec3(1, 0, 0));
        }
    }

}

void Tema2::RenderEnemies(float deltaTimeSeconds){

    static float globalTimer = 0.0f; 
    timeSinceLastenemy += deltaTimeSeconds;
    
    if (enemyList.size() < 2) {
        glm::vec3 randomPosition = generateRandomPosition();
    int randomResult = getRandomOneOrMinusOne();
    //  for (int i = 0; i < enemyList.size(); i++) {
    //         //coliziunile cu ceilalti inamici
    //             for (int j = 0; j < enemyList.size(); j++) {
    //                 if (i != j && enemyList[j].isDying == false) {
    //                     float distance = glm::length(enemyList[i].position - enemyList[j].position);
    //                     if (distance <= 1.22f) {
    //                         randomPosition.z *= -1;
    //                         randomPosition.x += 0.1f;
    //                         enemyList[i].position = randomPosition;
    //                     }
    //                 }
    //             }
    //         // coliziunile cu clădirile
    //         for (auto& clad : cladiri) {
    //             float distance = glm::length(clad.positions - enemyList[i].position);
    //             if (distance <= 2.0f) {
    //                 randomPosition.z *= -1;
                
    //                  enemyList[i].position = randomPosition;
    //             }
    //         }
    //             }

            Enemy enemy1(1, randomResult, meshes["tanc"], meshes["turela"], meshes["senila"], meshes["senila"], meshes["tun"], false, 3.0f, glm::vec3(1.0f, 1.0f, -1.0f), camera->GetTargetPosition() + glm::vec3(0.0f, -0.2f, 0.0f) + randomPosition );
            enemyList.push_back(enemy1);
    }

    for (int i = 0; i < enemyList.size(); i++) {
    
        if(enemyList[i].isDying == false) {
            float displacement = deltaTimeSeconds * 0.2f;
            globalTimer += deltaTimeSeconds;
            if (globalTimer >= 4.0f) {
                for (auto& enemy : enemyList) {
                    enemy.direction *= -1;
                }
                globalTimer = 0.0f;
            }
        
            if (enemyList[i].direction == 1) {
                
                enemyList[i].position.z += displacement;
            } else {
                enemyList[i].position.z -= displacement;
            }
    }
}

 for (auto& enemy : enemyList) {
    if(enemy.timeToDie >= 0){
        for (int i = 0; i < enemyList.size(); i++) {
            //coliziunile cu ceilalti inamici
                for (int j = 0; j < enemyList.size(); j++) {
                    if (i != j && enemyList[j].isDying == false) {
                        float distance = glm::length(enemyList[i].position - enemyList[j].position);
                        if (distance <= 1.6f) {
                            enemyList[i].position.z *= -1;
                            enemyList[i].position.x += 0.1f;
                            //enemyList[i].position = randomPosition;
                        }
                    }
                }
            // coliziunile cu clădirile
            for (auto& clad : cladiri) {
                float distance = glm::length(clad.positions - enemyList[i].position);
                if (distance <= 2.0f) {
                    enemyList[i].position.z *= -1;                
                     //enemyList[i].position = randomPosition;
                }
            }
                }

        if(enemy.timeToDie < 3 ){
        glm::mat4 modelMatrixe = glm::mat4(1);
        modelMatrixe = glm::translate(modelMatrixe, enemy.position);
        modelMatrixe = glm::scale(modelMatrixe, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.tanc, shaders["DisplacementShader"], modelMatrixe, projectionMatrix, camera->GetViewMatrix(),  glm::vec3(0.704, 0.604, 0), 0.3 * enemy.timeToDie);

        glm::mat4 modelMatrixt = glm::mat4(1);
        modelMatrixt = glm::translate(modelMatrixt, enemy.position);
        modelMatrixt = glm::translate(modelMatrixt, glm::vec3(-0.25, 0.25, -0.8));
        modelMatrixt= glm::rotate(modelMatrixt, RADIANS(270.0f), glm::vec3(1, 0, 0));
        modelMatrixt= glm::rotate(modelMatrixt, RADIANS(90.0f), glm::vec3(0, 0, 1));
        modelMatrixt = glm::scale(modelMatrixt, glm::vec3(0.03f, 0.02f, 0.02f));
        RenderSimpleMeshdisp(enemy.turela, shaders["DisplacementShader"], modelMatrixt, projectionMatrix, camera->GetViewMatrix(),   glm::vec3(0.804, 0.804, 0), 0.3 * enemy.timeToDie);

        glm::mat4 modelMatrixs1 = glm::mat4(1); 
        modelMatrixs1 = glm::translate(modelMatrixs1, enemy.position);
        modelMatrixs1 = glm::translate(modelMatrixs1, glm::vec3(0.01, 0.7, -0.5));
        modelMatrixs1= glm::rotate(modelMatrixs1, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixs1 = glm::scale(modelMatrixs1, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.senila1, shaders["DisplacementShader"], modelMatrixs1, projectionMatrix, camera->GetViewMatrix(),glm::vec3(0.7, 0.7, 0.7), 0.3 * enemy.timeToDie);

        glm::mat4 modelMatrixs2 = glm::mat4(1); 
        modelMatrixs2 = glm::translate(modelMatrixs2, enemy.position);
        modelMatrixs2 = glm::translate(modelMatrixs2, glm::vec3(0.43, 0.7, -0.5));
        modelMatrixs2= glm::rotate(modelMatrixs2, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixs2 = glm::scale(modelMatrixs2, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.senila2, shaders["DisplacementShader"], modelMatrixs2, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7), 0.3 * enemy.timeToDie);
   
        glm::mat4 modelMatrixtun = glm::mat4(1); 
        modelMatrixtun = glm::translate(modelMatrixtun, enemy.position);
        modelMatrixtun = glm::translate(modelMatrixtun, glm::vec3(0.13, -0.31, enemy.direction * 0.4));
        modelMatrixtun= glm::rotate(modelMatrixtun, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixtun = glm::scale(modelMatrixtun, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.tun, shaders["DisplacementShader"], modelMatrixtun, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7), 0.3 * enemy.timeToDie);
        }
    
    else {
         glm::mat4 modelMatrixe = glm::mat4(1);
        modelMatrixe = glm::translate(modelMatrixe, enemy.position);
        modelMatrixe = glm::scale(modelMatrixe, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.tanc, shaders["LabShader"], modelMatrixe, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.704, 0.604, 0),0.3);

        glm::mat4 modelMatrixt = glm::mat4(1);
        modelMatrixt = glm::translate(modelMatrixt, enemy.position);
        modelMatrixt = glm::translate(modelMatrixt, glm::vec3(-0.25, 0.25, -0.8));
        modelMatrixt= glm::rotate(modelMatrixt, RADIANS(270.0f), glm::vec3(1, 0, 0));
        modelMatrixt= glm::rotate(modelMatrixt, RADIANS(90.0f), glm::vec3(0, 0, 1));
        modelMatrixt = glm::scale(modelMatrixt, glm::vec3(0.03f, 0.02f, 0.02f));
        RenderSimpleMeshdisp(enemy.turela, shaders["LabShader"], modelMatrixt, projectionMatrix, camera->GetViewMatrix(),glm::vec3(0.804, 0.804, 0),0.3);

        glm::mat4 modelMatrixs1 = glm::mat4(1); 
        modelMatrixs1 = glm::translate(modelMatrixs1, enemy.position);
        modelMatrixs1 = glm::translate(modelMatrixs1, glm::vec3(0.01, 0.7, -0.5));
        modelMatrixs1= glm::rotate(modelMatrixs1, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixs1 = glm::scale(modelMatrixs1, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.senila1, shaders["LabShader"], modelMatrixs1, projectionMatrix, camera->GetViewMatrix(),glm::vec3(0.7, 0.7, 0.7), 0.3);

        glm::mat4 modelMatrixs2 = glm::mat4(1); 
        modelMatrixs2 = glm::translate(modelMatrixs2, enemy.position);
        modelMatrixs2 = glm::translate(modelMatrixs2, glm::vec3(0.43, 0.7, -0.5));
        modelMatrixs2= glm::rotate(modelMatrixs2, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixs2 = glm::scale(modelMatrixs2, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.senila2, shaders["LabShader"], modelMatrixs2, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7), 0.3);
   
        glm::mat4 modelMatrixtun = glm::mat4(1); 
        modelMatrixtun = glm::translate(modelMatrixtun, enemy.position);
        modelMatrixtun = glm::translate(modelMatrixtun, glm::vec3(0.13, -0.31, enemy.direction * 0.4));
        modelMatrixtun= glm::rotate(modelMatrixtun, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixtun = glm::scale(modelMatrixtun, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.tun, shaders["LabShader"], modelMatrixtun, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7), 0.3);
        }
        
    }
    else {

        enemy.isDying = true;
        glm::mat4 modelMatrixe = glm::mat4(1);
        modelMatrixe = glm::translate(modelMatrixe, enemy.position);
        modelMatrixe = glm::scale(modelMatrixe, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.tanc, shaders["DisplacementShader"], modelMatrixe, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.2, 0.2, 0), 0.3 * enemy.timeToDie);

        glm::mat4 modelMatrixt = glm::mat4(1);
        modelMatrixt = glm::translate(modelMatrixt, enemy.position);
        modelMatrixt = glm::translate(modelMatrixt, glm::vec3(-0.25, 0.25, -0.8));
        modelMatrixt= glm::rotate(modelMatrixt, RADIANS(270.0f), glm::vec3(1, 0, 0));
        modelMatrixt= glm::rotate(modelMatrixt, RADIANS(90.0f), glm::vec3(0, 0, 1));
        modelMatrixt = glm::scale(modelMatrixt, glm::vec3(0.03f, 0.02f, 0.02f));
        RenderSimpleMeshdisp(enemy.turela, shaders["DisplacementShader"], modelMatrixt, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.25, 0.25, 0), 0.3 * enemy.timeToDie);

        glm::mat4 modelMatrixs1 = glm::mat4(1); 
        modelMatrixs1 = glm::translate(modelMatrixs1, enemy.position);
        modelMatrixs1 = glm::translate(modelMatrixs1, glm::vec3(0.01, 0.7, -0.5));
        modelMatrixs1= glm::rotate(modelMatrixs1, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixs1 = glm::scale(modelMatrixs1, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.senila1, shaders["DisplacementShader"], modelMatrixs1, projectionMatrix, camera->GetViewMatrix(),glm::vec3(0.7, 0.7, 0.7), 0.3 * enemy.timeToDie);

        glm::mat4 modelMatrixs2 = glm::mat4(1); 
        modelMatrixs2 = glm::translate(modelMatrixs2, enemy.position);
        modelMatrixs2 = glm::translate(modelMatrixs2, glm::vec3(0.43, 0.7, -0.5));
        modelMatrixs2= glm::rotate(modelMatrixs2, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixs2 = glm::scale(modelMatrixs2, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.senila2, shaders["DisplacementShader"], modelMatrixs2, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7), 0.3 * enemy.timeToDie);
   
        glm::mat4 modelMatrixtun = glm::mat4(1); 
        modelMatrixtun = glm::translate(modelMatrixtun, enemy.position);
        modelMatrixtun = glm::translate(modelMatrixtun, glm::vec3(0.13, -0.31, -0.4));
        modelMatrixtun= glm::rotate(modelMatrixtun, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrixtun = glm::scale(modelMatrixtun, glm::vec3(0.02f));
        RenderSimpleMeshdisp(enemy.tun, shaders["DisplacementShader"], modelMatrixtun, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7), 0.3 * enemy.timeToDie);

    }

 }
 //coliziune tanc-tanc
        for(auto& enemy : enemyList) {
            float ok = 1;
            if(enemy.isDying == false) {
                float dif = glm::length(camera->GetTargetPosition() - enemy.position);
                //std::cout<<1<<" "<<dif<<endl;
                float P = 1.22 - dif;

                if(dif <= 1.22f ){
                    if(camera->position.z < 0) ok = ok *(-1);
                    //coliziune tanc-cladire
                    for(auto &clad : cladiri){
                        float distance = glm::length(clad.positions - enemy.position);
                        if(distance <= 2.0f){
                            // enemy.position.z += 1.0f;
                            enemy.position.z = enemy.position.z + (-1) * ok * P * 0.5f;
                        }
                        else {
                            // enemy.position.x += (-1) * ok * P * 0.5f;
                            enemy.position.z = enemy.position.z + (-1) * ok * P * 0.5f;
                        }
                        
                    }


                }
            }
        }

}

void Tema2::RenderProjectiles(float deltaTimeSeconds) {
    timeSinceLastprojectil += deltaTimeSeconds;

    glm::vec3 tankPosition = camera->GetTargetPosition();
    glm::mat4 tankRotationMatrix = glm::mat4(1);
    tankRotationMatrix = glm::rotate(tankRotationMatrix, angle, glm::vec3(0, 1, 0));
    if (b1 == true) {
        if (timeSinceLastprojectil >= 2 && angleProjectiles[angle].size() < 1) {
            glm::vec3 initialProjectilePosition = tankPosition;
            Sphere sph(initialProjectilePosition.x, initialProjectilePosition.y, initialProjectilePosition.z, meshes["sphere"], 0.0f);
            angleProjectiles[angle].push_back(sph);
            timeSinceLastprojectil = 0.0f;
            b1 = false;
        }
    }

    for (auto& angleProjPair : angleProjectiles) {
        float currentAngle = angleProjPair.first;
        auto& projectilesForAngle = angleProjPair.second;

        glm::mat4 angleRotationMatrix = glm::mat4(1);
        angleRotationMatrix = glm::rotate(angleRotationMatrix, currentAngle, glm::vec3(0, 1, 0));

        for (auto it = projectilesForAngle.begin(); it != projectilesForAngle.end(); ) {
            float displacement = deltaTimeSeconds * 0.4f;

            glm::vec4 displacementVector = glm::vec4(0.0f, 0.0f, -displacement, 1.0f);
            displacementVector = angleRotationMatrix * displacementVector;

            it->worldX += displacementVector.x;
            it->worldY += displacementVector.y;
            it->worldZ += displacementVector.z;

            it->time += deltaTimeSeconds;

            if (it->time >= 20.0f) {
                it = projectilesForAngle.erase(it);
            } else {
                ++it;
            }
        }

        for ( int i = 0; i < projectilesForAngle.size() ; i++) {
            
            glm::mat4 modelMatrixp = glm::mat4(1);
            modelMatrixp = glm::translate(modelMatrixp, glm::vec3(projectilesForAngle[i].worldX, projectilesForAngle[i].worldY, projectilesForAngle[i].worldZ));
            modelMatrixp = glm::rotate(modelMatrixp, currentAngle, glm::vec3(0, 1, 0));
            modelMatrixp = glm::translate(modelMatrixp, glm::vec3(0.15, 0.18, -1));
            modelMatrixp = glm::scale(modelMatrixp, glm::vec3(0.1f));
            RenderSimpleMesh(projectilesForAngle[i].sphere, shaders["LabShader"], modelMatrixp, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0, 0, 0));
        }

        //coliziune proiectil-cladire
        for(auto & clad: cladiri){
            for(int i = 0; i < projectilesForAngle.size() ; i++) {
            glm::vec3 projectilePosition(projectilesForAngle[i].worldX, projectilesForAngle[i].worldY, projectilesForAngle[i].worldZ);

            float distance = glm::length(clad.positions - projectilePosition);
               if(distance <= 1.38f) {
                    projectilesForAngle.erase(projectilesForAngle.begin() + i);
               }
            }
        }

        //coliziune proiectil-tanc
        for (auto& enemy : enemyList) {
            for(int i = 0; i < projectilesForAngle.size() ; i++) {
                glm::vec3 enemyPosition = glm::vec3(enemy.position.x, enemy.position.y + 0.2f, enemy.position.z);
                glm::vec3 enemyTurretPosition = glm::vec3(enemy.position.x, enemy.position.y + 0.25f, enemy.position.z - 0.8f);

                float collisionRadius = 0.01f + 0.05;

                glm::vec3 projectilePosition(projectilesForAngle[i].worldX, projectilesForAngle[i].worldY, projectilesForAngle[i].worldZ);
                float distance = glm::length(enemyTurretPosition - projectilePosition);
                // std::cout<<"enemy: "<<"x = "<<enemyPosition.x<<" y = "<<enemyPosition.y<<" z = "<<enemyPosition.z<<endl;
                // std::cout<<"projectil: "<<"x = "<<projectilePosition.x<<" y = "<<projectilePosition.y<<" z = "<<projectilePosition.z<<endl;
                // std::cout<<"distance: " << distance<<endl;
                if(distance < 1.38f) {
                    projectilesForAngle.erase(projectilesForAngle.begin() + i);
                   
                    if(enemy.isDying == false){
                    enemy.timeToDie --;}
                }
            }
        }
    }
}




void Tema2::RenderPlayer(){

    {
        // Player tank
        glm::mat4 modelMatrix1 = glm::mat4(1);
        modelMatrix1 = glm::translate(modelMatrix1, camera->GetTargetPosition());
        modelMatrix1 = glm::rotate(modelMatrix1, angle, glm::vec3(0, 1, 0));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(0,-0.2,0));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(0.02f));
        
        RenderSimpleMesh(meshes["tanc"], shaders["LabShader"], modelMatrix1, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0, 0.2, 0));

        
        glm::mat4 modelMatrix2 = glm::mat4(1); 
        modelMatrix2 = glm::translate(modelMatrix2, camera->GetTargetPosition());
        modelMatrix2 = glm::rotate(modelMatrix2, angle, glm::vec3(0, 1, 0));
        modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(-0.26, 0.05, -0.5));
        modelMatrix2= glm::rotate(modelMatrix2, RADIANS(270.0f), glm::vec3(1, 0, 0));
        modelMatrix2= glm::rotate(modelMatrix2, RADIANS(90.0f), glm::vec3(0, 0, 1));
        modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.02f, 0.02f, 0.02f));
        RenderSimpleMesh(meshes["turela"], shaders["LabShader"], modelMatrix2, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0, 0.3, 0));
        

        glm::mat4 modelMatrix3 = glm::mat4(1); 
        modelMatrix3 = glm::translate(modelMatrix3, camera->GetTargetPosition());
        modelMatrix3= glm::rotate(modelMatrix3, angle, glm::vec3(0, 1, 0));
        modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0.03, 0.46, -0.5));
        modelMatrix3= glm::rotate(modelMatrix3, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(0.02f));
        RenderSimpleMesh(meshes["senila"], shaders["LabShader"], modelMatrix3, projectionMatrix, camera->GetViewMatrix(),glm::vec3(0.7, 0.7, 0.7));


        glm::mat4 modelMatrix4 = glm::mat4(1); 
        modelMatrix4 = glm::translate(modelMatrix4, camera->GetTargetPosition());
        modelMatrix4= glm::rotate(modelMatrix4, angle, glm::vec3(0, 1, 0));
        modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0.41, 0.46, -0.5));
        modelMatrix4= glm::rotate(modelMatrix4, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.02f));
        RenderSimpleMesh(meshes["senila"], shaders["LabShader"], modelMatrix4, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7));


        glm::mat4 modelMatrix5 = glm::mat4(1); 
        modelMatrix5 = glm::translate(modelMatrix5, camera->GetTargetPosition());
        modelMatrix5= glm::rotate(modelMatrix5, angle, glm::vec3(0, 1, 0));
        modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(0.09, -0.5, -0.5));
        modelMatrix5= glm::rotate(modelMatrix5, RADIANS(270.0f), glm::vec3(0, 1, 0));
        modelMatrix5 = glm::scale(modelMatrix5, glm::vec3(0.02f));
        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix5, projectionMatrix, camera->GetViewMatrix(), glm::vec3(0.7, 0.7, 0.7));
        projectilesm = modelMatrix5;
    }
}
void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}



void Tema2::RenderMeshPlan(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

 
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // // Bind view matrix
    // glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    // int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    // glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // // Bind projection matrix
    // glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    // int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    // glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint loc = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(loc, 1, glm::value_ptr(color));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::mat4 proj, glm::mat4 view, const glm::vec3& color)
{
   if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

   
    int location = glGetUniformLocation(shader->program, "Model");
   
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
   
    int locationview = glGetUniformLocation(shader->program, "View");
   
    glm::mat4 viewMatrix = view;
    glUniformMatrix4fv(locationview, 1, GL_FALSE, glm::value_ptr(viewMatrix));
   
    int locationproject = glGetUniformLocation(shader->program, "Projection");

    glm::mat4 projectionMatrix = proj;
    glUniformMatrix4fv(locationproject, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    GLint loc = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(loc, 1, glm::value_ptr(color));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::RenderSimpleMeshdisp(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::mat4 proj, glm::mat4 view, const glm::vec3& color, const float hp)
{
   if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

   
    int location = glGetUniformLocation(shader->program, "Model");
   
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
   
    int locationview = glGetUniformLocation(shader->program, "View");
   
    glm::mat4 viewMatrix = view;
    glUniformMatrix4fv(locationview, 1, GL_FALSE, glm::value_ptr(viewMatrix));
   
    int locationproject = glGetUniformLocation(shader->program, "Projection");

    glm::mat4 projectionMatrix = proj;
    glUniformMatrix4fv(locationproject, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    GLint loc = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(loc, 1, glm::value_ptr(color));

    int time_location = glGetUniformLocation(shader->program, "hp");
    glUniform1f(time_location, hp);


    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
 
if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
    float cameraSpeed = 2.0f;
      if (window->KeyHold(GLFW_KEY_W)) {
            // Modificarea poziției pe axa X și Z în funcție de direcția și deltaTime
            lightPosition.z -= 0.2 * deltaTime * cameraSpeed ;
             camera->MoveForward(deltaTime * cameraSpeed);

        }
        if (window->KeyHold(GLFW_KEY_S)) {
            // Mers înapoi - inversul direcției actuale
             lightPosition.z += 0.2 * deltaTime * cameraSpeed;
             camera->MoveForward(deltaTime * cameraSpeed * (-1));
            
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            
             angle +=  cameraSpeed * deltaTime;
             camera->RotateFirstPerson_OY(deltaTime * cameraSpeed);
    
        }
         if (window->KeyHold(GLFW_KEY_D)) {
            angle -= cameraSpeed * deltaTime;
            camera->RotateFirstPerson_OY(-deltaTime * cameraSpeed);
        }
    }
   
}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if(b1 == true) {

        b1 = false;
    }
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
 
}



void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if(button == GLFW_MOUSE_BUTTON_2){
         b1 = true;
    }
   
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event

}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
