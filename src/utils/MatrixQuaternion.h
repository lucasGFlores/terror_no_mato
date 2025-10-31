#include "raylib.h"
#include "raymath.h" // Necessário para QuaternionFromEuler e MatrixFromTransform

// Converte graus para radianos (necessário para QuaternionFromEuler)
Matrix MatrixFromTransform(Transform transformacao);

inline Matrix CriarMatrizComQuaternion(Vector3 posicao,
                                       Vector3 rotacaoEulerGraus,
                                       Vector3 escala) {
  // 1. Converter os ângulos de Euler (em Graus) para um Quaternion (em
  // Radianos) NOTE: A raylib usa a ordem (Roll/X, Pitch/Y, Yaw/Z).
  Quaternion quat = QuaternionFromEuler(rotacaoEulerGraus.x * DEG2RAD,
                                        rotacaoEulerGraus.y * DEG2RAD,
                                        rotacaoEulerGraus.z * DEG2RAD);

  // 2. Criar a estrutura Transform
  // Essa estrutura armazena os 3 vetores (Posição, Quatérnio e Escala)
  Transform transformacao = {0};
  transformacao.translation = posicao;

  transformacao.rotation = quat;
  transformacao.scale = escala;

  // 3. Gerar a Matriz 4x4
  // Esta função combina SCALE * ROTATION * TRANSLATION em uma única matriz.
  Matrix matFinal = MatrixFromTransform(transformacao);
  return matFinal;
}
inline Matrix MatrixFromTransform(Transform transformacao) {
  // 1. Criar a Matriz de Escala (Scale Matrix)
  Matrix matrizEscala = MatrixScale(
      transformacao.scale.x, transformacao.scale.y, transformacao.scale.z);

  // 2. Criar a Matriz de Rotação (Rotation Matrix) a partir do Quaternion
  Matrix matrizRotacao = QuaternionToMatrix(transformacao.rotation);

  // 3. Criar a Matriz de Translação (Translation Matrix)
  Matrix matrizTranslacao =
      MatrixTranslate(transformacao.translation.x, transformacao.translation.y,
                      transformacao.translation.z);

  // 4. Multiplicar as matrizes na ordem inversa de aplicação (da direita para a
  // esquerda): Matriz Final = Translação * Rotação * Escala

  // Passos de multiplicação: (Matriz Temporária = Rotação * Escala)
  Matrix matrizTemporaria = MatrixMultiply(matrizRotacao, matrizEscala);

  // Passos de multiplicação: (Matriz Final = Translação * Matriz Temporária)
  Matrix matrizFinal = MatrixMultiply(matrizTranslacao, matrizTemporaria);

  return matrizFinal;
}
// =========================================================================
// Exemplo de Uso
// =========================================================================
