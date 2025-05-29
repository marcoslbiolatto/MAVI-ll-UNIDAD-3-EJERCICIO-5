#include "Box2DHelper.h"
#include "Game.h"

int main() {
    Box2DEmptyApp app;
    app.run();
    return 0;
}

void Box2DEmptyApp::run() {
    Game game;
    game.start(); // Inicia la simulación y el renderizador
}
