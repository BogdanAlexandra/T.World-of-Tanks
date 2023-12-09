#include "lab_m1/tema1/tema1.h"
#include "lab_m1/lab3/object2D.h"
#include "lab_m1/lab3/transform2D.h"


#include <vector>
#include <iostream>

float delta;

using namespace std;
using namespace m1;

/*

DE MENTIONAT:  Am Macbook si randarea nu este la fel, deci inamici nu fac coliziune la distance < suma razelor deoarece resolution nu se potriveste;

*/

std::vector<Romb> romburi;
std::vector<Star> stele;
std::vector<Inamic> listaInamici;
std::vector<Romb> listaRomburi;
std::vector<Star> bani;
std::vector<Square1> listadist;
std::vector<std::vector<std::vector<Star>>> proiectile (3, std::vector<std::vector<Star>>(3, std::vector<Star>(0)));

std::vector<std::vector<int>> projectiles{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};


float squareSide = 100;
float rombside = 29;
float timeSinceLastEnemy = 0.0f;
float timeSinceLaststar = 0.0f;
float timeSinceLastStar = 0.0f;
float timeSinceLastdist = 0.0f;
float timeSinceLastproiectil = 0.0f;
float enemySpawnDelay = 3.0f; 
float scalefactor = 1.0f;
float mx = 0, my = 0;

bool isp = false, isr = false;

int mousx, mousy;
int culoare = 0;
int viata = 3;
int numstars = 5;
std::vector<std::vector<int>> p = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};


std::vector<std::vector<int>> romburi_plasare = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
std::vector<std::vector<int>> ocupat = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
std::vector<std::vector<bool>> romb_dispare = {{false, false, false},{false, false, false}, {false, false, false}};
std::vector<glm::vec2> heartPositions = {
    glm::vec2(900, 970),
    glm::vec2(1010, 970),
    glm::vec2(1120, 970)
};

glm::vec3 culoareRomb;
std::string culoareRomb1;

struct CoordonateInamic {
    int i;
    int j;
    int k;
};

std::vector<std::vector<CoordonateInamic>> m(3, std::vector<CoordonateInamic>(3, {0, 0, -1}));

std::vector<std::vector<std::tuple<float, float, bool>>> listaSteleInamici = {{{0.0f, 0.0f, false}}, {{0.0f, 0.0f, false}}, {{0.0f,0.0f,false}},{{0.0f, 0.0f, false}} };




/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);


    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scx = 1;
    scy = 1;
    cx = corner.x;
    cy = corner.y;

    // Initialize angularStep
    angularStep = 0;

    //dreptunghi
    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square4);

    Mesh *rectangle1 = object2D::CreateRectangle("rectangle1", corner, 50, 400, glm::vec3(1,0,0), true);
    AddMeshToList(rectangle1);

    //SQUARES 
    Mesh* square7 = object2D::CreateSquare("square7", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square7);

    Mesh* square8 = object2D::CreateSquare("square8", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square8);

    Mesh* square9 = object2D::CreateSquare("square9", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square9);

    Mesh* square10 = object2D::CreateSquare("square10", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square10);

    Mesh* square11 = object2D::CreateSquare("square11", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square11);

    Mesh* square12 = object2D::CreateSquare("square12", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square12);

    Mesh* square13 = object2D::CreateSquare("square13", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square13);

    Mesh* square14 = object2D::CreateSquare("square14", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square14);

    Mesh* square15 = object2D::CreateSquare("square15", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square15);


    //romb 
    Mesh *romb1 = object2D::CreateDiamond("romb1", corner, rombside + 5, rombside + 19, rombside - 12 , glm::vec3(1, 0.5, 0), true);
    AddMeshToList(romb1);

    Mesh *romb2 = object2D::CreateDiamond("romb2", corner, rombside + 5, rombside + 19, rombside - 12 , glm::vec3(0, 0, 1), true);
    AddMeshToList(romb2);

    Mesh *romb3 = object2D::CreateDiamond("romb3", corner, rombside + 5, rombside + 19, rombside - 12 , glm::vec3(1, 1, 0), true);
    AddMeshToList(romb3);

    Mesh *romb4 = object2D::CreateDiamond("romb4", corner, rombside + 5, rombside + 19, rombside - 12 , glm::vec3(1, 0, 1), true);
    AddMeshToList(romb4);

    Mesh* square16 = object2D::CreateSquare("square16", corner, squareSide, glm::vec3(0.5, 0.5, 0.5));
    AddMeshToList(square16);


    //stea
    Mesh *star1 = object2D::CreateStar("star1", corner, 3, 14.5, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(star1);

    Mesh *star2 = object2D::CreateStar("star2", corner, 7, 35, glm::vec3(255, 215, 1), true);
    AddMeshToList(star2);


    Mesh* starorange = object2D::CreateStar("starorange", glm::vec3(0.0f, 0.0f, 0.0f), 7, 35, glm::vec3(1.0f, 0.5f, 0.0f), true);
    AddMeshToList(starorange);

    Mesh *staryellow = object2D::CreateStar("staryellow",glm::vec3(0.0f, 0.0f, 0.0f), 7, 35, glm::vec3(1.0f, 1.0f, 0.0f), true);
    AddMeshToList(staryellow);

    Mesh* starblue = object2D::CreateStar("starblue", glm::vec3(0.0f, 0.0f, 0.0f), 7, 35, glm::vec3(0.0f, 0.0f, 1.0f), true);
    AddMeshToList(starblue);

    Mesh* starpurple = object2D::CreateStar("starpurple", glm::vec3(0.0f, 0.0f, 0.0f), 7, 35, glm::vec3(1.0f, 0.0f, 1.0f), true);
    AddMeshToList(starpurple);


    // hexagoane
    Mesh* horange = object2D::CreateHexagon("horange", glm::vec3(0.0f, 0.0f, 0.0f), 35, glm::vec3(1.0f, 0.5f, 0.0f), true);
    AddMeshToList(horange);

    Mesh* hblue = object2D::CreateHexagon("hblue", glm::vec3(0.0f, 0.0f, 0.0f), 35, glm::vec3(0.0f, 0.0f, 1.0f), true);
    AddMeshToList(hblue);

    Mesh* hyellow = object2D::CreateHexagon("hyellow", glm::vec3(0.0f, 0.0f, 0.0f), 35, glm::vec3(1.0f, 1.0f, 0.0f), true);
    AddMeshToList(hyellow);

    Mesh* hpurple = object2D::CreateHexagon("hpurple", glm::vec3(0.0f, 0.0f, 0.0f), 35, glm::vec3(1.0f, 0.0f, 1.0f), true);
    AddMeshToList(hpurple);

    Mesh* h2 = object2D::CreateHexagon("h2", glm::vec3(0.0f, 0.0f, 0.0f), 25, glm::vec3(0.0f, 0.5f, 0.0f), true);
    AddMeshToList(h2);

    //inima 
    Mesh *heart = object2D::CreateHeart("heart", corner, 2.5, glm::vec3(1, 0, 0), true);
    AddMeshToList(heart);


}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void m1::Tema1::renderscena() {

    //dreptughi 
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(40, 250);
    RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);

  
    //squares
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(120, 250);
    RenderMesh2D(meshes["square7"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(270, 250);
    RenderMesh2D(meshes["square8"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(420, 250);
    RenderMesh2D(meshes["square9"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(120, 400);
    RenderMesh2D(meshes["square10"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(270, 400);
    RenderMesh2D(meshes["square11"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(420, 400);
    RenderMesh2D(meshes["square12"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(120, 550);
    RenderMesh2D(meshes["square13"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(270, 550);
    RenderMesh2D(meshes["square14"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(420, 550);
    RenderMesh2D(meshes["square15"], shaders["VertexColor"], modelMatrix);


 

}

void m1::Tema1::renderlife(){

    for (const auto& position : heartPositions) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(position.x, position.y);
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
    }


}

void m1::Tema1::renderrombs(){

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(70, 950);
    RenderMesh2D(meshes["romb1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(250, 950);
    RenderMesh2D(meshes["romb2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(450, 950);
    RenderMesh2D(meshes["romb3"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650, 950);
    RenderMesh2D(meshes["romb4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600, 900);
    RenderMesh2D(meshes["square16"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400, 900);
    RenderMesh2D(meshes["square16"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(200, 900);
    RenderMesh2D(meshes["square16"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(15, 900);
    RenderMesh2D(meshes["square16"], shaders["VertexColor"], modelMatrix);

}

void m1::Tema1::renderstars(){

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(30, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(210, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(250, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(410, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(450, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(610, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(690, 870);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

    for(int i = 0 ;i< numstars; i++){
          modelMatrix = glm::mat3(1.0f);
    modelMatrix = transform2D::Translate(900 + i * 40, 900);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);
    }

}

void m1::Tema1::renderEnvironment() {

    //scena
    renderscena();

    //romburi sus
    renderrombs();

    //stele sus
    renderstars();

    //vieti 
    renderlife();


}


void Tema1::renderromb(float deltaTimeSeconds) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int rombValue = romburi_plasare[i][j];
            if (rombValue != 0) {
                    GLint originalDepthFunc;
                    glGetIntegerv(GL_DEPTH_FUNC, &originalDepthFunc);
                    glDepthFunc(GL_ALWAYS);
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(170 + j * 150, 300 + i * 150);

                    if(romb_dispare[i][j] == true){

                        if (scaleX >= 0.1) {
                            scaleX -= deltaTimeSeconds;

                            modelMatrix *= transform2D::Translate(cx, cy);
                            modelMatrix *= transform2D::Scale(scaleX, scaleX);
                            modelMatrix *= transform2D::Translate(-cx, -cy);
                            std::string rombMeshName = "romb" + std::to_string(rombValue);
                            RenderMesh2D(meshes[rombMeshName], shaders["VertexColor"], modelMatrix);
                        }

                        else { 

                            scaleX = 1;

                            romb_dispare[i][j] = false;
                            romburi_plasare[i][j] = 0;
                            // for(int pr = 0 ; pr < proiectile[i][j].size(); pr++){
                            //     proiectile[i][j][pr].active = false;
                            // }
                            
                        }
                    }       
                    
                    else {
                        

                        std::string rombMeshName = "romb" + std::to_string(rombValue);
                        RenderMesh2D(meshes[rombMeshName], shaders["VertexColor"], modelMatrix);
                
                    }

            }
            else {
                projectiles[i][j] = 0;
                p[i][j] = 0;

            }
        }
    }
}


glm::vec3 GenerateRandomColor() {
    int randomColor = rand() % 4;

    switch (randomColor) {
        case 0: // Portocaliu
            return glm::vec3(1.0f, 0.5f, 0.0f);
        case 1: // Galben
            return glm::vec3(1.0f, 1.0f, 0.0f);
        case 2: // Mov
            return glm::vec3(1.0f, 0.0f, 1.0f);
        case 3: // Albastru
            return glm::vec3(0.0f, 0.0f, 1.0f);
        default:
            return glm::vec3(1.0f); 
    }
}


void Tema1::Update(float deltaTimeSeconds)
{

    rotation += deltaTimeSeconds;
   
    glm::ivec2 resolution1 = window->GetResolution();
    std::cout <<"RESOLUTION = "<< resolution1.x << " " << resolution1.y <<endl;

   //grafica
    renderEnvironment();

    //std::cout<<numstars<<endl;
    
    if(isp == true && numstars >= 0){
     GLint originalDepthFunc1;
        glGetIntegerv(GL_DEPTH_FUNC, &originalDepthFunc1);
        glDepthFunc(GL_ALWAYS);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate( mousx, fabs(720 - mousy));
        if(culoare == 1){
        RenderMesh2D(meshes["romb1"], shaders["VertexColor"], modelMatrix);}
        else if(culoare == 2){
        RenderMesh2D(meshes["romb2"], shaders["VertexColor"], modelMatrix);}
        else if(culoare == 3){
        RenderMesh2D(meshes["romb3"], shaders["VertexColor"], modelMatrix);}
        else if(culoare == 4){
        RenderMesh2D(meshes["romb4"], shaders["VertexColor"], modelMatrix);}

    }
    
    //pune romburi in patrate 
    renderromb(deltaTimeSeconds);

    //vin inamicii 
    RenderInamici(deltaTimeSeconds);

    //afisare resurse
    rendermoney(deltaTimeSeconds);

    int i = 0;
    glm::vec3 culoareRomb;
    std::vector<std::vector<int>> inamic = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};


    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            inamic[i][j] = 0;

            if (romburi_plasare[i][j] != 0) {

                for (int k = 0; k < listaInamici.size(); k++) {

                    if (listaInamici[k].pozitieXnou <= (170 + j * 150 + 84) 
                        && listaInamici[k].pozitieXnou >= (170 + j * 150)
                        && listaInamici[k].pozitieYnou == 300 + i * 150) {
                        romb_dispare[i][j] = true;
                        proiectile[i][j].clear();
                        renderromb(deltaTimeSeconds);
                    }
                }
            }
        }
    }

    timeSinceLastStar +=  deltaTimeSeconds;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                std::string culoareRomb;
                glm::vec3 culoareRomb1;
                if(romburi_plasare[i][j] != 0){
                    if (romburi_plasare[i][j] == 1) culoareRomb = "orange";
                    else if (romburi_plasare[i][j] == 2) culoareRomb = "blue";
                    else if (romburi_plasare[i][j] == 3) culoareRomb = "yellow";
                    else if (romburi_plasare[i][j] == 4) culoareRomb = "purple";

                    if (romburi_plasare[i][j] == 1) culoareRomb1 = glm::vec3(1.0f, 0.5f, 0.0f);
                    else if (romburi_plasare[i][j] == 2) culoareRomb1 = glm::vec3(0.0f, 0.0f, 1.0f);
                    else if (romburi_plasare[i][j] == 3) culoareRomb1 = glm::vec3(1.0f, 1.0f, 0.0f);
                    else if (romburi_plasare[i][j] == 4) culoareRomb1 = glm::vec3(1.0f, 0.0f, 1.0f);
                
                    for(int k = 0; k < listaInamici.size(); k++){
              
                         if(listaInamici[k].pozitieY == 300 + i * 150 && listaInamici[k].culoare == culoareRomb1){
                  
                            if (timeSinceLastStar >= 3.0f) {
                                float randomX = 170 + j * 150 + 50;
                                float randomY =  300 + i * 150;

                                Mesh* meshInamic = nullptr;
                                meshInamic = meshes["star" + culoareRomb];

                                if (meshInamic) {
                                Star star1(meshInamic, randomX, randomY, culoareRomb1, true, k);
                                proiectile[i][j].push_back(star1);
                                timeSinceLastStar = 0.0f;

                                }
                            }
                        }
                    }
                }
            }
        }




    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::string culoareRomb;
            glm::vec3 culoareRomb1;
            if(romburi_plasare[i][j] != 0){
                if (romburi_plasare[i][j] == 1) culoareRomb = "orange";
                else if (romburi_plasare[i][j] == 2) culoareRomb = "blue";
                else if (romburi_plasare[i][j] == 3) culoareRomb = "yellow";
                else if (romburi_plasare[i][j] == 4) culoareRomb = "purple";

                if (romburi_plasare[i][j] == 1) culoareRomb1 = glm::vec3(1.0f, 0.5f, 0.0f);
                else if (romburi_plasare[i][j] == 2) culoareRomb1 = glm::vec3(0.0f, 0.0f, 1.0f);
                else if (romburi_plasare[i][j] == 3) culoareRomb1 = glm::vec3(1.0f, 1.0f, 0.0f);
                else if (romburi_plasare[i][j] == 4) culoareRomb1 = glm::vec3(1.0f, 0.0f, 1.0f);
                for(int pr = 0; pr < proiectile[i][j].size(); pr++){
                for(int k = 0; k < listaInamici.size(); k++){
              
                    if(listaInamici[k].pozitieYnou == 300 + i * 150 && 
                        listaInamici[k].culoare == culoareRomb1 ){
                    
                            modelMatrix = glm::mat3(1);
                            modelMatrix *= transform2D::Translate(proiectile[i][j][pr].pozitieX , proiectile[i][j][pr].pozitieY);
                    
                            if (proiectile[i][j][pr].moveright) {
                                proiectile[i][j][pr].translateX += 0.03f * deltaTimeSeconds * resolution1.x;
                                proiectile[i][j][pr].pozitieXnou = proiectile[i][j][pr].translateX;
                                proiectile[i][j][pr].pozitieYnou = proiectile[i][j][pr].translateY;
                                modelMatrix *= transform2D::Translate(proiectile[i][j][pr].pozitieXnou, proiectile[i][j][pr].pozitieYnou);

                                if (proiectile[i][j][pr].translateX > 1280) {
                                    proiectile[i][j][pr].moveright = false;
                                    proiectile[i][j][pr].active = false;

                                }
                            }
                            else {
                                if (proiectile[i][j][pr].translateX <= 0) {
                                    proiectile[i][j][pr].moveright = true;
                                }
                            }
                            proiectile[i][j][pr].inamic = k;
                            modelMatrix *= transform2D::Translate(3.5, 3.5);
                            modelMatrix *= transform2D::Rotate(-3 * rotation);
                            modelMatrix *= transform2D::Translate(-3.5, -3.5);
                            if(proiectile[i][j][pr].active == true){
                                RenderMesh2D(proiectile[i][j][pr].stea, shaders["VertexColor"], modelMatrix);
                            }
                        }
                    }
                }
            }
        }
    }
 

    for (int i = 0; i < 3; i++) {
         for (int j = 0; j < 3; j++) {
            std::string culoareRomb;
            glm::vec3 culoareRomb1;
            if(romburi_plasare[i][j] != 0){
                if (romburi_plasare[i][j] == 1) culoareRomb = "orange";
                else if (romburi_plasare[i][j] == 2) culoareRomb = "blue";
                else if (romburi_plasare[i][j] == 3) culoareRomb = "yellow";
                else if (romburi_plasare[i][j] == 4) culoareRomb = "purple";

                if (romburi_plasare[i][j] == 1) culoareRomb1 = glm::vec3(1.0f, 0.5f, 0.0f);
                else if (romburi_plasare[i][j] == 2) culoareRomb1 = glm::vec3(0.0f, 0.0f, 1.0f);
                else if (romburi_plasare[i][j] == 3) culoareRomb1 = glm::vec3(1.0f, 1.0f, 0.0f);
                else if (romburi_plasare[i][j] == 4) culoareRomb1 = glm::vec3(1.0f, 0.0f, 1.0f);
                for(int pr = 0; pr < proiectile[i][j].size(); pr++){
                    for(int k = 0; k < listaInamici.size(); k++){  
                      
                        if(listaInamici[k].pozitieYnou == 300 + i * 150 && listaInamici[k].pozitieXnou > proiectile[i][j][pr].pozitieXnou 
                            && listaInamici[k].culoare == proiectile[i][j][pr].culoare && proiectile[i][j][pr].inamic == k){

                            glm::vec2 p1(listaInamici[k].pozitieXnou, listaInamici[k].pozitieYnou);
                            glm::vec2 p0(proiectile[i][j][pr].pozitieXnou, proiectile[i][j][pr].pozitieYnou);
                
                            float distance = glm::distance(p0, p1);
                            std::cout <<"i = "<<i<<" j = "<<j<<" proiectil = "<< pr<<" distance = "<< distance << endl ;
                            if(pr < 2){ 
                                if (listaInamici[k].pozitieXnou / 2 - proiectile[i][j][pr].pozitieXnou < 32) {
                                    //proiectile[i][j].erase(proiectile[i][j].begin() + pr);
                                    proiectile[i][j][pr].active = false;

                                }
                            }
                            if(pr == 2){ 
                                if (listaInamici[k].pozitieXnou / 2 - proiectile[i][j][pr].pozitieXnou < 32 ) {
                                    listaInamici[k].active = false;
                                    proiectile[i][j][pr].active = false;
                                    //proiectile[i][j].erase(proiectile[i][j].begin() + pr); 
                                }
                            }
                        }
                    }
                }
            }
        }
    }

if (viata < 3) {

    RenderDistrugatori(deltaTimeSeconds);

}

if(viata == 0){

    exit(1);
}
    

}

void Tema1::rendermoney(float deltaTimeSeconds) {

    timeSinceLaststar += deltaTimeSeconds;

    if (timeSinceLaststar >= 4.0f && stele.size() < 3) {
   
         for (int i = 0; i < 3; ++i) {
            float randomX = 170 + (rand() % 10) * 150;
            float randomY = 300 + (rand() % 7) * 150;
    
            glm::vec3 culoareAleatoare = GenerateRandomColor(); 

            Mesh* meshstea = meshes["star2"];

            if (meshstea) {
                    Star inamic(meshstea, randomX, randomY, glm::vec3(1, 0, 1), true);
                    stele.push_back(inamic);
             }


        }
        timeSinceLaststar = 0.0f;
    }

    for (const Star &str : stele) {

        GLint originalDepthFunc;
        glGetIntegerv(GL_DEPTH_FUNC, &originalDepthFunc);
        glDepthFunc(GL_ALWAYS);
        glm::mat3 modelMatrix = glm::mat3(1.0f);
        modelMatrix = transform2D::Translate(str.pozitieX, str.pozitieY);
        RenderMesh2D(str.stea, shaders["VertexColor"], modelMatrix);
        glDepthFunc(originalDepthFunc);
    }
}

void Tema1::RenderDistrugatori(float deltaTimeSeconds){
    glm::ivec2 resolution2 = window->GetResolution();
    timeSinceLastdist += deltaTimeSeconds;

    if (timeSinceLastdist >= 10){
        float randomX = 40;
        float randomY = 250.0f + (rand() % 3) * 150.0f;

        Mesh* meshInamic = nullptr;
        meshInamic = meshes["square1"];

        if (meshInamic) {
            Square1 inamic(meshInamic, randomX, randomY);
            listadist.push_back(inamic);
            timeSinceLastdist= 0.0f;
        }
        
    }
    for (int i = 0; i < listadist.size(); ++i) {
        if(listaInamici[i].active == true)
        { 

            listadist[i].translateX += 0.05f * deltaTimeSeconds * resolution2.x;
            listadist[i].x = listadist[i].translateX;
            //listadist[i].y = listadist[i].translateY;
            if (listadist[i].translateX > 1280.0f) {
                listadist.erase(listadist.begin() + i);
            }
        }
    }
 for (int k = 0; k < listadist.size(); k++) {
        GLint originalDepthFunc;
        glGetIntegerv(GL_DEPTH_FUNC, &originalDepthFunc);
        glDepthFunc(GL_ALWAYS);
        glm::mat3 modelMatrix = glm::mat3(1.0f);
        modelMatrix = transform2D::Translate(listadist[k].x, listadist[k].y);
        RenderMesh2D(listadist[k].patrat, shaders["VertexColor"], modelMatrix);
        

}

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3 ; j++){
            for (int k = 0; k < listadist.size(); k++) {
                if(listadist[k].y == 250 && listadist[k].x <= 420) {
                    if(romburi_plasare[0][0] != 0) romb_dispare[0][0] = true;
                    if(romburi_plasare[0][1] != 0) romb_dispare[0][1] = true;
                    if(romburi_plasare[0][2] != 0) romb_dispare[0][2] = true;
                }
                else if(listadist[k].y == 400 && listadist[k].x <= 420) {
                    if(romburi_plasare[1][0] != 0) romb_dispare[1][0] = true;
                    if(romburi_plasare[1][1] != 0) romb_dispare[1][1] = true;
                    if(romburi_plasare[1][2] != 0) romb_dispare[1][2] = true;
                }
                else if(listadist[k].y == 550 && listadist[k].x <= 420) {
                    if(romburi_plasare[2][0] != 0) romb_dispare[2][0] = true;
                    if(romburi_plasare[2][1] != 0) romb_dispare[2][1] = true;
                    if(romburi_plasare[2][2] != 0) romb_dispare[2][2] = true;
                }
            }
        }
    }
}
void Tema1::RenderInamici(float deltaTimeSeconds){
    glm::ivec2 resolution2 = window->GetResolution();

    timeSinceLastEnemy += deltaTimeSeconds;

    if (timeSinceLastEnemy >= (9.0f + rand() % 10) && listaInamici.size() < 3) {
        float randomX = 2000;
        float randomY = 300.0f + (rand() % 3) * 150.0f;

        glm::vec3 culoareAleatoare = GenerateRandomColor();
        bool culoareExistenta = false;
        for (const auto& inamic : listaInamici) {if (inamic.pozitieYnou == randomY && inamic.culoare == culoareAleatoare) {
            culoareExistenta = true;
             break;}}

    if (!culoareExistenta) {
        Mesh* meshInamic = nullptr;

        if (culoareAleatoare == glm::vec3(1, 0.5, 0)) {
            meshInamic = meshes["horange"];
        } else if (culoareAleatoare == glm::vec3(1, 1, 0)) {
            meshInamic = meshes["hyellow"];
        } else if (culoareAleatoare == glm::vec3(1, 0, 1)) {
            meshInamic = meshes["hpurple"];
        } else if (culoareAleatoare == glm::vec3(0, 0, 1)) {
            meshInamic = meshes["hblue"];
        }

        if (meshInamic) {
            Inamic inamic(meshInamic, meshes["h2"], randomX, randomY, culoareAleatoare, true);
            listaInamici.push_back(inamic);
            timeSinceLastEnemy = 0.0f;
        }
    }
    
}



    for (int i = 0; i < listaInamici.size(); ++i) {
        if(listaInamici[i].active == true)
        { 
            float viteza = -100.0f;
            listaInamici[i].translateX -= 0.02f * deltaTimeSeconds * resolution2.x;
            listaInamici[i].pozitieXnou = listaInamici[i].translateX;
            listaInamici[i].translateY = listaInamici[i].pozitieY;
            listaInamici[i].pozitieYnou = listaInamici[i].translateY;

            if (listaInamici[i].translateX < 75.0f) {
                listaInamici.erase(listaInamici.begin() + i);

                viata --; 

                heartPositions.erase(heartPositions.begin() + viata);   
            }
        }
    }

    for (int k = 0; k < listaInamici.size(); k++) {
        GLint originalDepthFunc;
        glGetIntegerv(GL_DEPTH_FUNC, &originalDepthFunc);
        glDepthFunc(GL_ALWAYS);
        glm::mat3 modelMatrix = glm::mat3(1.0f);
        modelMatrix = transform2D::Translate(listaInamici[k].pozitieXnou, listaInamici[k].pozitieYnou);
        if(listaInamici[k].active == false){
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                proiectile[i][j].clear();}
            }
             if (scx >= 0.1) {
                            scx -= deltaTimeSeconds;
                            modelMatrix = transform2D::Translate(listaInamici[k].pozitieXnou, listaInamici[k].pozitieYnou);
                            modelMatrix *= transform2D::Translate(cx, cy);
                            modelMatrix *= transform2D::Scale(scx, scx);
                            modelMatrix *= transform2D::Translate(-cx, -cy);
                            RenderMesh2D(listaInamici[k].hexagon1, shaders["VertexColor"], modelMatrix);
                             RenderMesh2D(listaInamici[k].hexagon2, shaders["VertexColor"], modelMatrix);
                        }

                        else { 
                            scx = 1;
                    
                            listaInamici.erase(std::remove_if(listaInamici.begin(), listaInamici.end(), 
                            [](const Inamic& inamic) {
                            return !inamic.active;
                             }), listaInamici.end());
                        }
        }
        else{

        RenderMesh2D(listaInamici[k].hexagon1, shaders["VertexColor"], modelMatrix);
        RenderMesh2D(listaInamici[k].hexagon2, shaders["VertexColor"], modelMatrix);
        glDepthFunc(originalDepthFunc);}
        
    }

}

void Tema1::FrameEnd()
{
}



void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event

}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    

    if(isp == true && numstars >= 0){
        isp = false;


        if (IsMouseInsideSquare(mouseX, mouseY, square1) && romburi_plasare[0][0] == 0) {
         
            romburi_plasare[0][0] = culoare;
     
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square2) && romburi_plasare[0][1] == 0) {


            romburi_plasare[0][1] = culoare;

         
       
        }
        else if (IsMouseInsideSquare(mouseX, mouseY, square3) && romburi_plasare[0][2] == 0) {
           

            romburi_plasare[0][2] = culoare;

      
   
        } 
        if (IsMouseInsideSquare(mouseX, mouseY, square4) && romburi_plasare[1][0] == 0) {
           

           romburi_plasare[1][0] = culoare;

        } 
        if (IsMouseInsideSquare(mouseX, mouseY, square5) && romburi_plasare[1][1] == 0) {
           

          romburi_plasare[1][1] = culoare;
   
        } 
        if (IsMouseInsideSquare(mouseX, mouseY, square6) && romburi_plasare[1][2] == 0) {
           
  
            romburi_plasare[1][2] = culoare;
   
   
        } 
        if (IsMouseInsideSquare(mouseX, mouseY, square7) && romburi_plasare[2][0] == 0) {
           

            romburi_plasare[2][0] = culoare;
        
   
        } 
        if (IsMouseInsideSquare(mouseX, mouseY, square8) && romburi_plasare[2][1] == 0) {
           

            romburi_plasare[2][1] = culoare;
   
        } 
        if (IsMouseInsideSquare(mouseX, mouseY, square9) && romburi_plasare[2][2] == 0) {
           

           romburi_plasare[2][2] = culoare;
   
   
        }
     
    }


    if(isr == true) {

        isr = false;

    }
}




void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
  if(isp == true){
    mousx = mouseX;
    mousy = mouseY;
  }
}

bool Tema1::IsMouseInsideSquare(int mouseX, int mouseY, const Square& square)
{
    return ( mouseX >= square.x && mouseX <= square.x + square.width && mouseY >= square.y && mouseY <= square.y + square.height);
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1) {
         isp = true;
        if (IsMouseInsideSquare(mouseX, mouseY, square10)) {
            //isp = true;
            mousx = mouseX;
            mousy = mouseY;
            culoare = 1;       
          
        }
        else if (IsMouseInsideSquare(mouseX, mouseY, square11)) {
            //isp = true;
            mousx = mouseX;
            mousy = mouseY;
            culoare = 2;
            
        }
        else if (IsMouseInsideSquare(mouseX, mouseY, square12)) {
            //isp = true;
            mousx = mouseX;
            mousy = mouseY;
            culoare = 3;

            
        }
        else if (IsMouseInsideSquare(mouseX, mouseY, square13)) {
            //isp = true;
            mousx = mouseX;
            mousy = mouseY;
            culoare = 4;
            
        }

        if(culoare == 1) numstars -= 1;
        else if(culoare == 2) numstars -= 2;
        else if(culoare == 3) numstars -= 2;
        else if(culoare == 4) numstars -= 3;

    }
    if(button == 2)
    {
    
        if (IsMouseInsideSquare(mouseX, mouseY, square1) && romburi_plasare[0][0] != 0 ) {
         
            romb_dispare[0][0] = true;
            projectiles[0][0] = 0;

            p[0][0] = 0;

     
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square2) && romburi_plasare[0][1] != 0 ) {


            romb_dispare[0][1] = true;
            projectiles[0][1] = 0;
            p[0][1] = 0;
        }
        else if (IsMouseInsideSquare(mouseX, mouseY, square3) && romburi_plasare[0][2] != 0 ) {
           

            romb_dispare[0][2] = true;
            projectiles[0][2] = 0;
            p[0][2] = 0;
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square4) && romburi_plasare[1][0] != 0 ) {
           

            romb_dispare[1][0] = true;
            projectiles[1][0] = 0;
            p[1][0] = 0;
   
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square5) && romburi_plasare[1][1] != 0 ) {
           

            romb_dispare[1][1] = true;
            projectiles[1][1] = 0;
            p[1][1] = 0;
   
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square6) && romburi_plasare[1][2] != 0 ) {
           
  
            romb_dispare[1][2] = true;
            projectiles[1][2] = 0;
            p[1][2] = 0;
   
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square7) && romburi_plasare[2][0] != 0) {
           

            romb_dispare[2][0] = true;
            projectiles[2][0] = 0;
            p[2][0] = 0;
   
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square8) && romburi_plasare[2][1] != 0) {
           

            romb_dispare[2][1] = true;
            projectiles[2][1] = 0;
            p[2][1] = 0;
   
        } 
        else if (IsMouseInsideSquare(mouseX, mouseY, square9) && romburi_plasare[2][2] != 0) {
           

            romb_dispare[2][2] = true;
            projectiles[2][2] = 0;
            p[2][2] = 0;
   
        }

        else {
                
            isr = true;
            mx = mouseX;
            my = mouseY;

            for (int i = 0; i < stele.size(); i++) {
                float starLeft = stele[i].pozitieX / 2 - 35;
                float starRight = stele[i].pozitieX / 2 + 35;
                float starTop = stele[i].pozitieY / 2 + 35;
                float starBottom = stele[i].pozitieY / 2- 35;
        
                float mouseRev = fabs(720 - my);

                if (mx >= starLeft && mx <= starRight && mouseRev >= starBottom && mouseRev <= starTop) {
                        stele.erase(stele.begin() + i);
                        numstars++;
            
                }
            }
        }       
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
