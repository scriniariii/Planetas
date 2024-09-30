#include "raylib.h"
#include "raymath.h"
#include <vector>

// Clase para la cámara
class CameraController {
public:
    Camera3D camera;

    CameraController(Vector3 position, Vector3 target) {
        camera.position = position;
        camera.target = target;
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    void Update() {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);  // Actualiza la cámara en tercera persona
    }

    void ToggleFullScreen() {
        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
        }
    }
};

// Clase para los planetas
class Planet {
public:
    Model model;
    Texture2D texture;
    float rotationSpeed;
    float rotationAngle;
    Vector3 position;  // Nueva propiedad para almacenar la posición del planeta

    Planet(const char* texturePath, float speed, Vector3 pos) {
        // Cargar la textura
        texture = LoadTexture(texturePath);
        if (!texture.id) {
            TraceLog(LOG_ERROR, "No se pudo cargar la textura.");
        }

        // Generar la esfera
        Mesh sphereMesh = GenMeshSphere(1.0f, 10, 10);
        model = LoadModelFromMesh(sphereMesh);
        model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;

        // Velocidad y ángulo de rotación inicial
        rotationSpeed = speed;
        rotationAngle = 0.0f;
        position = pos;  // Asignar la posición del planeta
    }

    void Update() {
        // Actualizar la rotación
        rotationAngle += rotationSpeed;
        model.transform = MatrixIdentity();
        Matrix rotationMatrix = MatrixRotate(Vector3{ 0.0f, 1.0f, 0.0f }, rotationAngle);
        model.transform = MatrixMultiply(model.transform, rotationMatrix);
    }

    void Draw() {
        // Dibujar el modelo en la posición especificada
        DrawModel(model, position, 1.0f, WHITE);
    }

    void Unload() {
        UnloadTexture(texture);
        UnloadModel(model);
    }
};

// Clase para gestionar el juego (ventana, loop principal)
class Game {
public:
    CameraController camera;
    std::vector<Planet> planets;

    Game() : camera(Vector3{ 4.0f, 4.0f, 4.0f }, Vector3{ 0.0f, 0.0f, 0.0f }) {
        InitWindow(1250, 680, "Planetasd");
        SetTargetFPS(30);

        // Crear planetas con posiciones diferentes
        planets.push_back(Planet("resources/planeta.png", 0.01f, Vector3{ 0.0f, 0.0f, 0.0f }));
        planets.push_back(Planet("resources/planeta.png", 0.005f, Vector3{ 3.0f, 0.0f, 0.0f }));  // Nuevo planeta en otra posición
        planets.push_back(Planet("resources/planeta.png", 0.02f, Vector3{ -3.0f, 0.0f, 0.0f })); // Otro planeta en el lado opuesto
    }

    void Run() {
        while (!WindowShouldClose()) {
            Update();
            Draw();
        }
    }

    void Update() {
        camera.Update();
        camera.ToggleFullScreen();

        // Actualizar todos los planetas
        for (Planet& planet : planets) {
            planet.Update();
        }
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(Color{ 52, 73, 94, 255 });

        BeginMode3D(camera.camera);
        for (Planet& planet : planets) {
            planet.Draw();
        }
        EndMode3D();

        EndDrawing();
    }

    ~Game() {
        for (Planet& planet : planets) {
            planet.Unload();
        }
        CloseWindow();
    }
};

// Función principal
int main() {
    Game game;
    game.Run();

    return 0;
}
