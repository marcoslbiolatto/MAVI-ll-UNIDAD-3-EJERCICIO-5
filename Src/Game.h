#ifndef GAME_H
#define GAME_H

#include "SFMLRenderer.h"
#include <vector>

class Game {
private:
    b2World* world;           // Mundo f�sico
    SFMLRenderer renderer;    // Renderizador
    std::vector<b2Body*> bodies; // Partes del cuerpo del ragdoll

    void createRagdoll();     // M�todo para crear el ragdoll

public:
    Game();   // Constructor
    ~Game();  // Destructor
    void start(); // Inicia la simulaci�n y el renderizado
};

#endif // GAME_H
