#include "raylib.h"

int main() {
    // Inicializamos la ventana
    InitWindow(1250, 680, "Planetasd");

    
    

    // Definimos una cámara
    Camera3D camera = { 0 };
    camera.position = Vector3{ 4.0f, 4.0f, 4.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Cargar la textura del fondo
   // Texture2D background = LoadTexture("resources/space_background.png");

    // Cargar la textura para el planeta
    Texture2D texture = LoadTexture("resources/planeta.png");
    if (!texture.id) {
        TraceLog(LOG_ERROR, "No se pudo cargar la textura.");
    }

    // Modelo de la esfera
    Mesh sphereMesh = GenMeshSphere(1.0f, 10, 10);
    Model sphereModel = LoadModelFromMesh(sphereMesh);
    sphereModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_F)){
            ToggleFullscreen(); 
        }
        

        // Actualizar la cámara
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        BeginDrawing();
        Color myColor = {52, 73, 94, 255};
        ClearBackground(myColor );

        // Iniciar modo 3D
        BeginMode3D(camera);

        // Dibujar el modelo de la esfera
        DrawModel(sphereModel, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);

        EndMode3D();  // Terminar modo 3D

        EndDrawing();  // Terminar dibujo
    }

    // Liberar recursos
    UnloadTexture(texture);
    UnloadModel(sphereModel);
    CloseWindow();

    return 0;
}
