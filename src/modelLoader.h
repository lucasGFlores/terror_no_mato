#include "raylib.h"
#include <string>
class ModelLoader {
private:
  Model model;
  bool isLoaded;
  Vector3 position;
  Vector3 rotation;
  Vector3 scale;

public:
  ModelLoader()
      : isLoaded(false), position({0.0f, 0.0f, 0.0f}),
        rotation({0.0f, 0.0f, 0.0f}), scale({1.0f, 1.0f, 1.0f}) {}

  ~ModelLoader() {
    if (isLoaded) {
      UnloadModel(model);
    }
  }

  // Load model from file
  bool LoadModelFromFile(const std::string &filePath) {
    if (isLoaded) {
      UnloadModel(model);
    }

    model = ::LoadModel(filePath.c_str());

    if (model.meshCount > 0) {
      isLoaded = true;
      return true;
    }

    isLoaded = false;
    return false;
  }

  // Load model and texture separately
  bool LoadModelWithTexture(const std::string &modelPath,
                            const std::string &texturePath) {
    if (!LoadModelFromFile(modelPath)) {
      return false;
    }

    Texture2D texture = LoadTexture(texturePath.c_str());
    if (texture.id == 0) {
      return false;
    }

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    return true;
  }

  // Draw the model
  void Draw() {
    if (!isLoaded)
      return;

    DrawModelEx(model, position, {1.0f, 0.0f, 0.0f}, rotation.x, scale, WHITE);
  }

  // Draw with custom color
  void Draw(Color tint) {
    if (!isLoaded)
      return;

    DrawModelEx(model, position, {1.0f, 0.0f, 0.0f}, rotation.x, scale, tint);
  }

  // Setters
  void SetPosition(Vector3 pos) { position = pos; }
  void SetRotation(Vector3 rot) { rotation = rot; }
  void SetScale(Vector3 scl) { scale = scl; }
  void SetScale(float uniformScale) {
    scale = {uniformScale, uniformScale, uniformScale};
  }

  // Getters
  Vector3 GetPosition() const { return position; }
  Vector3 GetRotation() const { return rotation; }
  Vector3 GetScale() const { return scale; }
  Model &GetModel() { return model; }
  bool IsLoaded() const { return isLoaded; }

  // Animation support
  int GetAnimationCount() const { return isLoaded ? model.boneCount : 0; }
};
