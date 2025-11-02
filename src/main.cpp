#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "raylib.h"
#include "raymath.h"

const float G = 1000.0f;
const int NUM_POINTS = 15;
const float REST_SPACING = 8.0f;
const float DAMPING = 0.99f;
const int CONSTRAINT_ITERATIONS = 20;
const float DT = 1.0f / 60.0f;

using namespace std;

using vec2 = Vector2;

class Rope {
  vec2 points[NUM_POINTS];
  vec2 prev_points[NUM_POINTS];

public:
  Rope() {
    for (int i = 0; i < NUM_POINTS; ++i) {
      points[i] = {400.0f, 50.0f + i * REST_SPACING};
      prev_points[i] = {400.0f, 50.0f + i * REST_SPACING};
    }
  }

  void apply_constraints() {
    for (int i = 1; i < NUM_POINTS; ++i) {
      vec2 delta = points[i] - points[i - 1];
      float dist = Vector2Length(delta);
      float diff = (dist - REST_SPACING) / dist;
      vec2 correction = delta * 0.5f * diff;
      if (i != 0)
        points[i] = points[i] - correction;
      points[i - 1] = points[i - 1] + correction;
    }
  }

  void simulate() {
    // Simple gravity effect
    for (int i = 0; i < NUM_POINTS; ++i) {
      vec2 accel = {0.0f, G};
      vec2 temp = points[i];

      // integrate
      points[i] +=
          (points[i] - prev_points[i]) * DAMPING + accel * (float)(DT * DT);
      prev_points[i] = temp;
    }

    // enforce distance constraints multiple times
    for (int j = 0; j < CONSTRAINT_ITERATIONS; ++j)
      apply_constraints();

    points[0] = vec2{(float)GetMouseX(), (float)GetMouseY()};
  }

  void draw() {
    DrawLineStrip(points, NUM_POINTS, WHITE);
    DrawCircleV(points[0], 8.0f, WHITE);
    DrawCircleV(points[NUM_POINTS - 1], 8.0f, WHITE);
  }
};

int main() {
  // Initialize raylib
  InitWindow(800, 600, "Raylib and Miniaudio Example");
  SetTargetFPS(60);

  Rope rope;

  // Main loop
  while (!WindowShouldClose()) {
    rope.simulate();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Playing sound with Miniaudio!", 200, 280, 20, LIGHTGRAY);
    // draw fps
    DrawFPS(10, 10);

    rope.draw();

    EndDrawing();
  }

  // Clean up
  CloseWindow();

  return 0;
}
