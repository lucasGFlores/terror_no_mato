#include "../utils/MatrixQuaternion.h"
#include "Ientity.h"
#include "controls/controls.h"
#include <raylib.h>
#include <raymath.h>
#define STAND_HEIGHT 1.0f
#define BOTTOM_HEIGHT 0.5f
typedef struct {
  Vector2 position;
  Vector2 sensibility;
} Mouse;
static Mouse mouse = {0, 0, 0.001f, 0.001f};
class Player {
private:
  ModelLoader model;
  Matrix orientationMatrix;
  Camera3D camera;
  Body body;
  Controls controler;

  void ConfigCamera() {
    body = {0};
    body.headLerp = STAND_HEIGHT;
    orientationMatrix = CriarMatrizComQuaternion({0.0f}, {0.0f}, {0.0f});
    camera = {0};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.position = GetPosition();
    camera.target = Vector3Add(camera.position, (Vector3){0.0f, 0.0f, -1.0f});
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  }
  void updateCameraTarget() {
    Vector2 mouseDelta = GetMouseDelta();
    mouse.position.x -= mouseDelta.x * mouse.sensibility.x;
    mouse.position.y -=
        Clamp(mouseDelta.y * mouse.sensibility.y, -PI * 0.49f, PI * 0.49f);
    float cosYaw = cosf(mouse.position.x);
    float sinYaw = sinf(mouse.position.x);
    float cosPitch = cosf(mouse.position.y);
    float sinPitch = sinf(mouse.position.y);

    Vector3 direction = {sinYaw * cosPitch, sinPitch, cosYaw * cosPitch};

    direction = Vector3Normalize(direction);

    // Target = posição + direção
    camera.target = Vector3Add(camera.position, direction);
  }

public:
  Player() : controler(camera) { ConfigCamera(); }
  Vector3 GetPosition() {
    return {
        orientationMatrix.m12,
        orientationMatrix.m13 + (BOTTOM_HEIGHT),
        orientationMatrix.m14,
    };
  }
  void Tick() {
    controler.ReactInputs();
    updateCameraTarget();
  };
  Camera GetCamera() { return camera; }
  ~Player() = default;
};
