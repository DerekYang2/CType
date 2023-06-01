#include "Graph.h"

#define inter_f(x) (-0.451106 * (x) + 32.7561)

/**
 * TODO: MAKE BEIZIER CUTS GENERIC
*/

Graph::Graph(float x, float y, float w, float h, float thick) : thick(thick)
{
    rect = { x, y, w, h };
}

void Graph::set_plot(vector<float> plot_points, int wpm_type)
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
    
    points[wpm_type].clear();
    // transform points into rectangle graph
    for (int i = 0; i < plot_points.size(); i++)
    {
        points[wpm_type].push_back({ x_pos, rect.y + rect.height * (1.f - plot_points[i] / max_v) });
        x_pos += gap;
    }
    avg_dist = 0;
    // calculate root mean square dist 
    for (int i = 1; i < points[wpm_type].size(); i++)
    {
        avg_dist += Vector2DistanceSqr(points[wpm_type][i], points[wpm_type][i - 1]);
    }
    avg_dist /= points[wpm_type].size(), avg_dist = sqrt(avg_dist);
    f = 0.9;
    float scale = 0.1 + (f - 0.1) * (avg_dist - 10) / 300;
    cout << inter_f(plot_points.size()) << endl;
    // calculate bezier curve array
    BezierPath bezier(inter_f(plot_points.size()));
    bezier.Interpolate(points[wpm_type], scale);

    draw_points[wpm_type] = bezier.GetDrawingPoints(thick);
    
    if (wpm_type == RAW)
    {
        draw_segments.clear();
        
        vector<Vector2> curve_p = bezier.GetDrawingPoints();  // curve points
        
        // get all ranges of strictly increasing/decreasing
        bool decreasing = curve_p[0].y > curve_p[1].y;
        int st = 0;
        for (int i = 1; i < curve_p.size(); i++)
        {
            if (decreasing)
            {
                if (curve_p[i].y < curve_p[i-1].y)  // not decreasing (note operator is flipped because y is inverse)
                {
                    decreasing = false;
                    draw_segments.push_back(vector<Vector2>());
                    draw_segments.back().push_back({ curve_p[st].x, rect.y + rect.height });
                    draw_segments.back().push_back({ curve_p[i - 1].x, rect.y + rect.height });
                    for (int j = i - 1; j >= st; j--)
                    {
                        draw_segments.back().push_back(curve_p[j]);
                    }
                    st = i - 1;
                }
            } else
            {
                if (curve_p[i].y >= curve_p[i-1].y)  // decreasing
                {
                    decreasing = true;
                    draw_segments.push_back(vector<Vector2>());
                    draw_segments.back().push_back({curve_p[i-1].x, rect.y + rect.height});
                    for (int j = i-1; j >= st; j--)
                    {
                        draw_segments.back().push_back(curve_p[j]);
                    }
                    draw_segments.back().push_back({ curve_p[st].x, rect.y + rect.height });
                    st = i - 1;
                }
            }
        }
        // last range 
        if (decreasing)
        {
            draw_segments.push_back(vector<Vector2>());
            draw_segments.back().push_back({ curve_p[st].x, rect.y + rect.height });
            draw_segments.back().push_back({ curve_p.back().x, rect.y + rect.height });
            for (int i = curve_p.size() - 1; i >= st; i--)
            {
                draw_segments.back().push_back(curve_p[i]);
            }
        } else
        {
            draw_segments.push_back(vector<Vector2>());
            draw_segments.back().push_back({ curve_p.back().x, rect.y + rect.height });
            for (int i = curve_p.size() - 1; i >= st; i--)
            {
                draw_segments.back().push_back(curve_p[i]);
            }
            draw_segments.back().push_back({ curve_p[st].x, rect.y + rect.height });
        }
    }
}

void Graph::update()
{
    int wpm_type = NORMAL;
    if (IsKeyPressed(KEY_EQUAL))
    {
        f += 0.1;
        float scale = 0.1 + (f - 0.1) * (avg_dist - 10) / 300;
        // calculate bezier curve array
        BezierPath bezier(20);
        bezier.Interpolate(points[wpm_type], scale);
        draw_points[wpm_type] = bezier.GetDrawingPoints(thick);
    } else if (IsKeyPressed(KEY_MINUS))
    {
        f -= 0.1;
        float scale = 0.1 + (f - 0.1) * (avg_dist - 10) / 300;
        // calculate bezier curve array
        BezierPath bezier(20);
        bezier.Interpolate(points[wpm_type], scale);
        draw_points[wpm_type] = bezier.GetDrawingPoints(thick);
    }
}

void Graph::draw()
{
    DrawRectangleRec(rect, rgb(70, 70, 70));
    DrawText(to_string(f), rect.x, rect.y, 20, RED);

    for (vector<Vector2>& segment : draw_segments)
    {
        DrawTriangleFan(&segment[0], segment.size(), rgba(0, 0, 0, 0.1));
    }
    DrawTriangleStrip(&draw_points[RAW][0], draw_points[RAW].size(), rgba(0, 0, 0, 0.2));
    DrawTriangleStrip(&draw_points[NORMAL][0], draw_points[NORMAL].size(), RED);

    if (IsKeyDown(KEY_SPACE))
    {
        for (Vector2& p : points[NORMAL])
        {
            DrawCircleV(p, 3, BLUE);
        }
    }


}

float Graph::width()
{
    return rect.width;
}