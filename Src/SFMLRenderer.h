#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>

class SFMLRenderer {
public:
    void run(b2World* world, const std::vector<b2Body*>& bodies);
};

#endif // SFML_RENDERER_H
