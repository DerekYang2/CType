#pragma once
#include "globals.h"
#include "UIObject.h"
#include "BezierPath.h"

#define NORMAL 0
#define RAW 1

class Graph : public UIObject
{
    vector<Vector2> points[2],
                    draw_points[2],  // draw points for triangle strip 
                    errors;
    vector<vector<Vector2>> draw_segments;
    Rectangle rect;
    float max_v;
    int max_err;
    float gap;
    float thick;
    float grid_y_gap;
    float time; 
public:
    Graph(float x, float y, float w, float h, float thick);
    void reset();
    void config_max(vector<float>& plot_points, int wpm_type = NORMAL);  // call on both lists to calculate the max value 
    void init_polygon(vector<Vector2>&& curve_p);
    void set_plot(vector<float>& plot_points, int wpm_type = NORMAL);
    void set_error(vector<int>& error_list);
    void update() override;
    void draw() override;
    void set_time(float t);
    float width();
};
extern Graph* graph;
