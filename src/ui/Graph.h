#pragma once
#include "globals.h"
#include "UIObject.h"
#include "BezierPath.h"
class Graph : public UIObject
{
    vector<Vector2> points;
    vector<Vector2> draw_points;  // draw points for triangle strip
    Rectangle rect;
    float gap;
    float thick;
    float f;
    float avg_dist;
public:
    Graph(float x, float y, float w, float h, float thick);
    void set_plot(vector<float> plot_points);
    void update() override;
    void draw() override;
};
