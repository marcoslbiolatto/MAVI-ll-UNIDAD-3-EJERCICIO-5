#include "Game.h"

Game::Game() {
    // Crear el mundo físico con gravedad hacia abajo
    b2Vec2 gravity(0.f, 9.8f);
    world = new b2World(gravity);

    // Crear el piso y el ragdoll
    createRagdoll();
}

Game::~Game() {
    delete world; // Limpieza del mundo físico
}

void Game::createRagdoll() {
    // Crear el piso
    b2BodyDef floorDef;
    floorDef.type = b2_staticBody;
    floorDef.position.Set(13.33f, 19.5f); // Posición del piso en la parte inferior

    b2Body* floor = world->CreateBody(&floorDef);

    b2PolygonShape floorShape;
    floorShape.SetAsBox(13.33f, 0.5f); // Ancho del piso para cubrir todo el borde
    b2FixtureDef floorFixture;
    floorFixture.shape = &floorShape;
    floorFixture.friction = 0.9f; // Alta fricción para evitar deslizamiento
    floorFixture.restitution = 0.0f; // Sin rebote
    floor->CreateFixture(&floorFixture);

    // Crear la cabeza
    b2BodyDef headDef;
    headDef.type = b2_dynamicBody;
    headDef.position.Set(13.33f, 8.f); // Posición inicial de la cabeza
    b2Body* head = world->CreateBody(&headDef);

    b2CircleShape headShape;
    headShape.m_radius = 0.5f; // Tamaño de la cabeza
    b2FixtureDef headFixture;
    headFixture.shape = &headShape;
    headFixture.density = 0.5f;
    headFixture.friction = 0.8f;
    headFixture.restitution = 0.0f;
    head->CreateFixture(&headFixture);
    bodies.push_back(head); // Agregar la cabeza a la lista de cuerpos

    // Crear el torso
    b2BodyDef torsoDef;
    torsoDef.type = b2_dynamicBody;
    torsoDef.position.Set(13.33f, 9.f); // Posición inicial del torso
    b2Body* torso = world->CreateBody(&torsoDef);

    b2PolygonShape torsoShape;
    torsoShape.SetAsBox(0.5f, 1.f); // Tamaño del torso
    b2FixtureDef torsoFixture;
    torsoFixture.shape = &torsoShape;
    torsoFixture.density = 1.0f;
    torsoFixture.friction = 0.8f;
    torsoFixture.restitution = 0.0f;
    torso->CreateFixture(&torsoFixture);
    bodies.push_back(torso); // Agregar el torso a la lista de cuerpos

    // Crear los brazos
    for (int i = -1; i <= 1; i += 2) { // -1: brazo izquierdo, 1: brazo derecho
        b2BodyDef armDef;
        armDef.type = b2_dynamicBody;
        armDef.position.Set(13.33f + (i * 0.8f), 9.f); // Posición inicial de los brazos
        b2Body* arm = world->CreateBody(&armDef);

        b2PolygonShape armShape;
        armShape.SetAsBox(0.25f, 0.75f); // Tamaño del brazo
        b2FixtureDef armFixture;
        armFixture.shape = &armShape;
        armFixture.density = 0.3f;
        armFixture.friction = 0.8f;
        armFixture.restitution = 0.0f;
        arm->CreateFixture(&armFixture);
        bodies.push_back(arm); // Agregar el brazo a la lista de cuerpos

        // Conectar el brazo al torso
        b2RevoluteJointDef jointDef;
        jointDef.bodyA = torso;
        jointDef.bodyB = arm;
        jointDef.localAnchorA.Set(i * 0.5f, 0.f);
        jointDef.localAnchorB.Set(0.f, 0.75f);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.5f * b2_pi; // Límite inferior
        jointDef.upperAngle = 0.5f * b2_pi;  // Límite superior
        world->CreateJoint(&jointDef);
    }

    // Crear las piernas
    for (int i = -1; i <= 1; i += 2) { // -1: pierna izquierda, 1: pierna derecha
        b2BodyDef legDef;
        legDef.type = b2_dynamicBody;
        legDef.position.Set(13.33f + (i * 0.3f), 10.5f); // Posición inicial de las piernas
        b2Body* leg = world->CreateBody(&legDef);

        b2PolygonShape legShape;
        legShape.SetAsBox(0.25f, 0.75f); // Tamaño de la pierna
        b2FixtureDef legFixture;
        legFixture.shape = &legShape;
        legFixture.density = 0.6f;
        legFixture.friction = 0.8f;
        legFixture.restitution = 0.0f;
        leg->CreateFixture(&legFixture);
        bodies.push_back(leg); // la pierna 

        // Conectar la pierna al torso
        b2RevoluteJointDef jointDef;
        jointDef.bodyA = torso;
        jointDef.bodyB = leg;
        jointDef.localAnchorA.Set(i * 0.3f, -1.f);
        jointDef.localAnchorB.Set(0.f, 0.75f);
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.5f * b2_pi; // Límite inferior
        jointDef.upperAngle = 0.5f * b2_pi;  // Límite superior
        world->CreateJoint(&jointDef);

        // Conectar la cabeza al torso
        b2RevoluteJointDef neckJointDef;
        neckJointDef.bodyA = torso; // Torso como base
        neckJointDef.bodyB = head;  // Cabeza como cuerpo conectado
        neckJointDef.localAnchorA.Set(0.f, 1.f); // Punto de unión en el torso (parte superior)
        neckJointDef.localAnchorB.Set(0.f, -0.5f); // Punto de unión en la cabeza (parte inferior)
        neckJointDef.enableLimit = true;
        neckJointDef.lowerAngle = -0.25f * b2_pi; // Límite inferior del movimiento de la cabeza
        neckJointDef.upperAngle = 0.25f * b2_pi;  // Límite superior del movimiento de la cabeza
        world->CreateJoint(&neckJointDef);

    }
}

void Game::start() {
    renderer.run(world, bodies); // Inicia el renderizado del ragdoll y el piso
}
