#ifndef GAME_H
#define GAME_H

#include "SFMLRenderer.h"
#include <vector>

class Game {
private:
    b2World* world;           // Mundo físico
    SFMLRenderer renderer;    // Renderizador
    std::vector<b2Body*> bodies; // Partes del cuerpo del ragdoll

    void createRagdoll();     // Método para crear el ragdoll

public:
    Game();   // Constructor
    ~Game();  // Destructor
    void start(); // Inicia la simulación y el renderizado
};

#endif // GAME_H
