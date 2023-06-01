#pragma once
#include "globals.h"
#include "UIObject.h"
#include "BezierPath.h"

#define NORMAL 0
#define RAW 1

class Graph : public UIObject
{
    vector<Vector2> points[2];
    vector<Vector2> draw_points[2];  // draw points for triangle strip
    vector<vector<Vector2>> draw_segments;
    Rectangle rect;
    float gap;
    float thick;
    float f;
    float avg_dist;
public:
    Graph(float x, float y, float w, float h, float thick);
    void set_plot(vector<float> plot_points, int wpm_type = NORMAL);
    void update() override;
    void draw() override;
    float width();
};
extern Graph* graph;
