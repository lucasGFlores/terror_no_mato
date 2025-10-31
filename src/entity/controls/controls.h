#include <cmath>
#include <emmintrin.h>
#include <raylib.h>
#include <raymath.h>
#define GRAVITY 32.0f
#define MAX_SPEED 20.0f
#define CROUCH_SPEED 5.0f
#define JUMP_FORCE 12.0f
#define MAX_ACCEL 150.0f
// Grounded drag
#define FRICTION 0.86f
// Increasing air drag, increases strafing speed
#define AIR_DRAG 0.98f
// Responsiveness for turning movement direction to looked direction
#define CONTROL 15.0f
#define CROUCH_HEIGHT 0.5f
#define STAND_HEIGHT 1.0f
#define BOTTOM_HEIGHT 0.5f

#define NORMALIZE_INPUT 0

typedef struct {
  Vector3 velocity;
  Vector3 dir;
  bool isGrounded;
  float headLerp;
  float headTimer;
} Body;

class Controls {
private:
  Body body;
  Camera3D *camera;
  void Walk(char side, char forward, bool crouchHold) {

    Vector2 input = (Vector2){(float)-side, (float)forward};
#if defined(NORMALIZE_INPUT)
    // Slow down diagonal movement
    if ((side != 0) && (forward != 0))
      input = Vector2Normalize(input);
#endif

    float delta = GetFrameTime();

    if (!body.isGrounded)
      body.velocity.y -= GRAVITY * delta;

    Vector3 front =
        Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 right = {
        front.z, // Novo X é o Z original
        0.0f,    // O eixo Y (vertical) permanece 0
        -front.x // Novo Z é o negativo do X original
    };
    Vector3 desiredDir = (Vector3){
        input.x * right.x + input.y * front.x,
        0.0f,
        input.x * right.z + input.y * front.z,
    };
    body.dir = Vector3Lerp(body.dir, desiredDir,
                           CONTROL * delta); // smooth the movimentation change

    float decel = (body.isGrounded ? FRICTION : AIR_DRAG);
    Vector3 hvel =
        (Vector3){body.velocity.x * decel, 0.0f, body.velocity.z * decel};

    float hvelLength = Vector3Length(hvel); // Magnitude
    if (hvelLength < (MAX_SPEED * 0.01f))
      hvel = (Vector3){0};

    // This is what creates strafing
    float speed = Vector3DotProduct(hvel, body.dir);

    // Whenever the amount of acceleration to add is clamped by the maximum
    // acceleration constant, a Player can make the speed faster by bringing the
    // direction closer to horizontal velocity angle More info here:
    // https://youtu.be/v3zT3Z5apaM?t=165
    float maxSpeed = (crouchHold ? CROUCH_SPEED : MAX_SPEED);
    float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL * delta);
    hvel.x += body.dir.x * accel;
    hvel.z += body.dir.z * accel;

    body.velocity.x = hvel.x;
    body.velocity.z = hvel.z;
    camera->position.x += body.velocity.x * delta;
    camera->position.y += body.velocity.y * delta;
    camera->position.z += body.velocity.z * delta;
    // Fancy collision system against the floor
    if (camera->position.y <= 1.5f) {
      camera->position.y = 1.5f;
      body.velocity.y = 0.0f;
      body.isGrounded = true; // Enable jumping
    }
  }
  void WalkWable(char side, char forward) {
    if (!body.isGrounded || (side == 0 && forward == 0))
      return;
    body.headTimer += GetFrameTime() * 3.0f;
    Vector3 up = {0.0f, 1.0f, 0.f};
    float headSin = sin(body.headTimer * PI);
    float stepRotation = 0.1f;
    Vector3 bodyDirection = {camera->target.x, 0.0f, camera->target.z};
    camera->up =
        Vector3RotateByAxisAngle(up, bodyDirection, headSin * stepRotation);
  }
  void Crouch(bool crouching) {
    if (!body.isGrounded)
      return;
    body.headLerp = Lerp(body.headLerp, (crouching ? CROUCH_HEIGHT : 0),
                         20.0f * GetFrameTime());
    camera->position.y = -body.headLerp + camera->position.y;

    Jump(IsKeyDown(KEY_SPACE));
  }
  void Jump(bool jumpPressed) {
    if (body.isGrounded && jumpPressed) {
      body.velocity.y = JUMP_FORCE;
      body.isGrounded = false;

      // Sound can be played at this moment
      // SetSoundPitch(fxJump, 1.0f + (GetRandomValue(-100, 100)*0.001));
      // PlaySound(fxJump);
    }
  }
  void Interect() {}

public:
  Controls(Camera3D &player_camera) {
    this->body = {0};
    this->camera = &player_camera;
  };

  void ReactInputs() {
    char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
    Walk(sideway, forward, crouching);
    WalkWable(sideway, forward);
    // Jump(IsKeyDown(KEY_SPACE));
    // Crouch(crouching);
  }

  ~Controls() = default;
};
