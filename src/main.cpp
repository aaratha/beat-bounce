#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "raylib.h"
#include "raymath.h"

#include <cmath>

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

  void set_end_point(vec2 point) { points[NUM_POINTS - 1] = point; }

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

class Box {
  // clockwise order
  vec2 vertices[4];

public:
  Box(float x, float y, float width, float height, float rotation_deg) {
    float rad = rotation_deg * (PI / 180.0f);
    float w2 = width / 2.0f;
    float h2 = height / 2.0f;

    // clockwise: top-left, top-right, bottom-right, bottom-left
    float localX[4] = {-w2, w2, w2, -w2};
    float localY[4] = {-h2, -h2, h2, h2};

    for (int i = 0; i < 4; i++) {
      vertices[i] = {x + localX[i] * cosf(rad) - localY[i] * sinf(rad),
                     y + localX[i] * sinf(rad) + localY[i] * cosf(rad)};
    }
  }

  void draw() {
    DrawTriangle(vertices[0], vertices[3], vertices[1], WHITE); // now clockwise
    DrawTriangle(vertices[1], vertices[3], vertices[2], WHITE);
  }
};

int main() {
  // Initialize raylib
  InitWindow(800, 600, "Raylib and Miniaudio Example");
  SetTargetFPS(60);

  Rope rope;
  Box box(600.0f, 200.0f, 200.0f, 20.0f, -120.0f);

  // Main loop
  while (!WindowShouldClose()) {
    rope.simulate();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Playing sound with Miniaudio!", 200, 280, 20, LIGHTGRAY);
    // draw fps
    DrawFPS(10, 10);

    rope.draw();
    box.draw();

    EndDrawing();
  }

  // Clean up
  CloseWindow();

  return 0;
}
