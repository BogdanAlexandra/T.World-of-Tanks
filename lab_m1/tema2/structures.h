#pragma once

#include "utils/glm_utils.h"

struct Enemy {
        int id;
        int direction;
        float phaseCurrDist;
        Mesh* tanc;
        Mesh* turela;
        Mesh* senila1;
        Mesh* senila2;
        Mesh* tun;
        bool isDying;
        float timeToDie;
        glm::vec3 culoare;
        glm::vec3 position;

        Enemy() 
            : id(0), direction(0), phaseCurrDist(0.0f), tanc(nullptr), turela(nullptr),
            senila1(nullptr), senila2(nullptr), tun(nullptr), isDying(false),
            timeToDie(0.0f), culoare(glm::vec3(0.0f, 0.0f, 0.0f)), position(glm::vec3(0.0f, 0.0f, 0.0f)) {
        }

        Enemy(int _id, int _direction, Mesh* _tanc, Mesh* _turela,
            Mesh* _senila1, Mesh* _senila2, Mesh* _tun, bool _isDying, float _timeToDie,
            const glm::vec3& _culoare, const glm::vec3& _position)
            : id(_id), direction(_direction), tanc(_tanc),
            turela(_turela), senila1(_senila1), senila2(_senila2), tun(_tun),
            isDying(_isDying), timeToDie(_timeToDie), culoare(_culoare), position(_position) {
        }
    };

struct Sphere
    {
        float worldX;
        float worldY;
        float worldZ;
        float radius;
        Mesh *sphere;
        float time;

        Sphere() : worldX(0), worldY(0), worldZ(0), radius(0), sphere(nullptr), time(0){}
    
        Sphere(float worldX, float worldY, float worldZ, Mesh *sphere, float time)
            : worldX(worldX), worldY(worldY), worldZ(worldZ), sphere(sphere), time(time) {}
     
    };

struct Box
{
    Mesh *box;
    bool active;
    glm::vec3 positions;

       Box(Mesh* mesh, bool active, const glm::vec3& initialPosition)
        : box(mesh), active(false), positions(initialPosition) {}
};
