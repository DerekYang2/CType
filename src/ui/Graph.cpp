#include "Graph.h"

Graph::Graph(float x, float y, float w, float h, float thick) : thick(thick)
{
    rect = { x, y, w, h };
}

void Graph::set_plot(vector<float> plot_points)
{
    assert(!plot_points.empty());
    gap = rect.width / (plot_points.size() - 1);
    float x_pos = rect.x;
    float min_v = INT_MAX, max_v = INT_MIN;
    for (float v : plot_points)
    {
        min_v = min(min_v, v), max_v = max(max_v, v);
    }
    max_v += min_v;  // same distance from 0-min and max-top
    
    points.clear();
    // transform points into rectangle graph
    for (float v : plot_points)
    {
        points.push_back({x_pos, rect.y + rect.height * (1.f - v/max_v) });
        x_pos += gap;
    }

    // calculate root mean square dist 
    for (int i = 1; i < points.size(); i++)
    {
        avg_dist += Vector2DistanceSqr(points[i], points[i - 1]);
    }
    avg_dist /= points.size(), avg_dist = sqrt(avg_dist);
    f = 0.8;
    float scale = 0.1 + (f - 0.1) * (avg_dist - 10) / 300;
    // calculate bezier curve array
    BezierPath bezier(20);
    bezier.Interpolate(points, scale);
    draw_points = bezier.GetDrawingPoints(thick);
}

void Graph::update()
{
    if (IsKeyPressed(KEY_EQUAL))
    {
        f += 0.1;
        float scale = 0.1 + (f - 0.1) * (avg_dist - 10) / 300;
        // calculate bezier curve array
        BezierPath bezier(20);
        bezier.Interpolate(points, scale);
        draw_points = bezier.GetDrawingPoints(thick);
    } else if (IsKeyPressed(KEY_MINUS))
    {
        f -= 0.1;
        float scale = 0.1 + (f - 0.1) * (avg_dist - 10) / 300;
        // calculate bezier curve array
        BezierPath bezier(20);
        bezier.Interpolate(points, scale);
        draw_points = bezier.GetDrawingPoints(thick);
    }
}

void Graph::draw()
{
    DrawRectangleRec(rect, rgb(70, 70, 70));
    DrawText(to_string(f), rect.x, rect.y, 20, RED);
    DrawTriangleStrip(&draw_points[0], draw_points.size(), RED);
    if (IsKeyDown(KEY_SPACE))
    for (Vector2& p : points)
    {
        DrawCircleV(p, 3, BLACK);
    }
}