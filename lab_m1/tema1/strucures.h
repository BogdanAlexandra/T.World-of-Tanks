
        struct Square {
            float x;      // Coordonata X a pătratului
            float y;      // Coordonata Y a pătratului
            float width;  // Lățimea pătratului
            float height; // Înălțimea pătratului
        };

          struct Square1 {
            Mesh *patrat;
            float x;      
            float y;    
            float translateX = 0, translateY = 0; 
            Square1 (Mesh* patrat, float x, float y)
            : patrat(patrat), x(x), y(y) {}
        };

struct Romb {
    int pozitieX, pozitieY;  // Poziția inamicului
     glm::vec3 culoare;  // Culoarea inamicului
};

struct Star {
    Mesh *stea;
    float pozitieX,pozitieY;  
    float pozitieXnou = 0,pozitieYnou = 0;  /// Poziția stelei
    glm::vec3 culoare;
    bool active;
    float translateX = 0, translateY = 0;
    bool moveright = false;
    int inamic = -1 ;
   
     Star(Mesh* stea, float x, float y, const glm::vec3& culoare, bool active)
     : stea(stea), pozitieX(x), pozitieY(y), culoare(culoare), active(true)  {}

     Star(Mesh* stea, float x, float y, const glm::vec3& culoare, bool active, int inamic)
     : stea(stea), pozitieX(x), pozitieY(y), culoare(culoare), active(true), inamic(inamic) {}

     Star()
        : stea(nullptr), pozitieX(0), pozitieY(0), culoare(glm::vec3(0)), active(false) {}

        void clear() {
        stea = nullptr;
        pozitieX = 0;
        pozitieY = 0;
        culoare = glm::vec3(0);
        active = false;
        translateX = 0;
        translateY = 0;
        moveright = false;
        inamic = -1;
    }
};
struct Inamic {
    Mesh* hexagon1; // Mesh pentru primul hexagon
    Mesh* hexagon2; // Mesh pentru al doilea hexagon
    float hp = 3;

    float pozitieX;
    float pozitieY;    
    glm::vec3 culoare; // Culoarea inamicului
    bool active;
    Star stea;
    float translateX = 2000, translateY = 0;
    bool moveright = false;
    float pozitieXnou = 0,pozitieYnou = 0; 

    Inamic(Mesh* h1, Mesh* h2, float x, float y, const glm::vec3& culoare, bool active)
        : hexagon1(h1), hexagon2(h2), pozitieX(x), pozitieY(y), culoare(culoare), active(true) {}
};
