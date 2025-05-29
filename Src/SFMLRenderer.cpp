#include "SFMLRenderer.h"

static const float SCALE = 30.f; // Escala para convertir metros a píxeles

void SFMLRenderer::run(b2World* world, const std::vector<b2Body*>& bodies) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ragdoll");
    b2Body* selectedBody = nullptr; // Cuerpo seleccionado al hacer clic

    sf::RectangleShape floorVisual(sf::Vector2f(800.f, 0.5f * SCALE)); // Representar el piso
    floorVisual.setFillColor(sf::Color::Blue);
    floorVisual.setOrigin(floorVisual.getSize().x / 2, floorVisual.getSize().y / 2);
    floorVisual.setPosition(400.f, 19.5f * SCALE);

    std::vector<sf::RectangleShape> visuals;

    // Crear representaciones visuales para cada parte del cuerpo
    for (size_t i = 0; i < bodies.size(); ++i) {
        sf::RectangleShape shape;

        if (bodies[i]->GetFixtureList()->GetShape()->GetType() == b2Shape::e_circle) {
            float radius = static_cast<b2CircleShape*>(bodies[i]->GetFixtureList()->GetShape())->m_radius * SCALE;
            shape.setSize(sf::Vector2f(2 * radius, 2 * radius)); // Dimensiones del círculo
            shape.setOrigin(radius, radius); // Centrar origen
        }
        else {
            auto box = static_cast<b2PolygonShape*>(bodies[i]->GetFixtureList()->GetShape());
            float width = 2 * box->m_vertices[2].x * SCALE;
            float height = 2 * box->m_vertices[2].y * SCALE;
            shape.setSize(sf::Vector2f(width, height)); // Dimensiones del rectángulo
            shape.setOrigin(width / 2, height / 2); // Centrar origen
        }

        shape.setFillColor(sf::Color::White); 
        visuals.push_back(shape); // Agregar a visuales
    }

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Detectar clic del mouse
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                b2Vec2 mouseWorldPos(mousePos.x / SCALE, mousePos.y / SCALE);

                // Comprobar si el clic coincide con alguna parte del cuerpo
                for (size_t i = 0; i < bodies.size(); ++i) {
                    b2Body* body = bodies[i];
                    for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
                        if (fixture->TestPoint(mouseWorldPos)) {
                            selectedBody = body; // Selecciona el cuerpo bajo el clic
                            break;
                        }
                    }
                }
            }

            // Detectar movimiento del mouse mientras se mantiene presionado
            if (event.type == sf::Event::MouseMoved && selectedBody) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                b2Vec2 targetPosition(mousePos.x / SCALE, mousePos.y / SCALE);

                // Mover el cuerpo seleccionado al lugar del mouse
                selectedBody->SetTransform(targetPosition, selectedBody->GetAngle());
            }

            // Soltar el cuerpo cuando se suelta el botón del mouse
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                selectedBody = nullptr;
            }
        }

        // Simulación de Box2D
        float deltaTime = clock.restart().asSeconds();
        world->Step(deltaTime, 10, 8);

        // Actualizar visuales
        for (size_t i = 0; i < bodies.size(); ++i) {
            visuals[i].setPosition(bodies[i]->GetPosition().x * SCALE, bodies[i]->GetPosition().y * SCALE);
            visuals[i].setRotation(bodies[i]->GetAngle() * 180.f / b2_pi);
        }

        window.clear();
        window.draw(floorVisual);
        for (auto& visual : visuals) {
            window.draw(visual);
        }
        window.display();
    }
}

