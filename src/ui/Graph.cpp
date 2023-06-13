#include "Graph.h"

#define iter_f(x) (-0.451106 * (x) + 32.7561)

Graph::Graph(float x, float y, float w, float h, float thick) : thick(thick)
{
    rect = { x, y, w, h };
}

void Graph::reset()
{
    for (int t = 0; t <= 1; t++)
    {
        points[t].clear();
        draw_points[t].clear();
    }
    errors.clear();
    draw_segments.clear();
    max_v = 20;  // cannot be smaller than 20
    max_err = 0;
}

void Graph::config_max(vector<float>& plot_points, int wpm_type)
{
    float min_val = INT_MAX, max_val = INT_MIN;
    for (float& v : plot_points)
    {
        min_val = min(min_val, v), max_val = max(max_val, v);
    }
    if (wpm_type == NORMAL)
    {
        max_v = max(max_v, min_val + max_val); // same distance from 0-min and max-top
    } else {
        max_v = max(max_v, max_val * 1.05f); // if raw, max_v is just max value with a bit of padding
    }
}

void Graph::init_polygon(vector<Vector2> &&curve_p)
{
    // make sure curve doesn't cross out of the bottom 
    for (int i = 0; i < curve_p.size(); i++)
    {
        if (curve_p[i].y > rect.y + rect.height)
        {
            curve_p[i].y = rect.y + rect.height;
        }
    }
    draw_segments.clear();
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
            } else if (curve_p[i-1].x - curve_p[st].x >= 300) // too wide
            {
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
            } else if (curve_p[i-1].x - curve_p[st].x >= 300) // too wide
            {
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

void Graph::set_plot(vector<float>& plot_points, int wpm_type)
{
    assert(!plot_points.empty());
    float gap = rect.width / (plot_points.size() - 1);
    
    // transform points into rectangle graph
    float x_pos = rect.x;
    for (int i = 0; i < plot_points.size(); i++)
    {
        points[wpm_type].push_back({ x_pos, rect.y + rect.height * (1.f - plot_points[i] / max_v) });
        x_pos += gap;
    }
    bool zero_jump = false;
    if (wpm_type == NORMAL)
    {
        for (int i = 1; i < plot_points.size() - 1; i++)
        {
            if (plot_points[i-1] <= 0.01 && plot_points[i] >= 1 && plot_points[i+1] <= 0.01)
            {
                zero_jump = true;
                break;
            }
        }
    }

    // calculate root mean square dist 
    float avg_dist = 0;
    for (int i = 1; i < points[wpm_type].size(); i++)
        avg_dist += Vector2DistanceSqr(points[wpm_type][i], points[wpm_type][i - 1]);
    avg_dist /= points[wpm_type].size(), avg_dist = sqrt(avg_dist);
    
    float f = 9.f;
    float scale = 0.1 + (f - 0.1) * (sqrt(abs(avg_dist - 10))) / 300;
    if (zero_jump) scale = 0.2f;
    // calculate bezier curve array
    BezierPath bezier(iter_f(plot_points.size()));
    bezier.Interpolate(points[wpm_type], scale);

    if (wpm_type == RAW)
    {
        draw_points[wpm_type] = bezier.GetDrawingPoints();
        // make sure curve doesn't cross out of the bottom 
        for (int i = 0; i < draw_points[wpm_type].size(); i++)
            if (draw_points[wpm_type][i].y > rect.y + rect.height)
                draw_points[wpm_type][i].y = rect.y + rect.height;
        
        draw_points[wpm_type] = bezier.applyThickness(draw_points[wpm_type], thick);
        init_polygon(bezier.GetDrawingPoints());  // curve points
    } else
    {
        draw_points[wpm_type] = bezier.GetDrawingPoints();
        // make sure curve doesn't cross out of the bottom 
        for (int i = 0; i < draw_points[wpm_type].size(); i++)
            if (draw_points[wpm_type][i].y > rect.y + rect.height)
                draw_points[wpm_type][i].y = rect.y + rect.height;
        draw_points[wpm_type] = bezier.applyThickness(draw_points[wpm_type], thick);
    }

    if (wpm_type == NORMAL)  // only need to do once
    {
        // y-grid
        // choose y gap size  
        int target_lines = 10;  // aim for approx 10 lines
        grid_y_gap = max_v / target_lines;
        // round grid_y_gap to 1, 2, 5, 10, 20, 30, 40, etc...
        if (grid_y_gap < 1.5) grid_y_gap = 1;
        else if (1.5 <= grid_y_gap && grid_y_gap < 3.5) grid_y_gap = 2;
        else if (3.5 <= grid_y_gap && grid_y_gap < 7.5) grid_y_gap = 5;
        else if (7.5 <= grid_y_gap && grid_y_gap < 15) grid_y_gap = 10;
        else grid_y_gap = 10 * (int)round(grid_y_gap / 10);  // round to the nearest 10
        //convert value to actual coordinate y value
        grid_y_gap *= rect.height / max_v;
    }
}

void Graph::set_error(vector<float>& error_list)
{
    // calculate error list 
    float err_dist = 10;
    float t_gap = err_dist * (time-1) / rect.width;
    int err_i = 0;
    //for (auto& x : error_list) cout << x << ","; cout << endl;
    vector<int> err_amt;
    // trim out all errors less than t = 1
    while (err_i < error_list.size() && error_list[err_i] < 1)
        err_i++;
    for (float t = 1; t + t_gap <= time; t += t_gap)
    {
        int error_amt = 0;
        while (err_i < error_list.size() && t <= error_list[err_i] && error_list[err_i] < t + t_gap)
            error_amt++, err_i++;
        err_amt.push_back(error_amt);
    }
    
    max_err = 0;
    for (int &err: err_amt)
        max_err = max(max_err, err);
    max_err++;  // just a bit of space at top
    if (max_err > 5)
        max_err = (int)round(ceil((float)max_err / 10) * 10);  // round up to nearest 10 multiple
    // transform points into rectangle graph
    float x_pos = rect.x;
    for (int i = 0; i < err_amt.size(); i++)
    {
        if (err_amt[i] > 0)
            errors.push_back({ x_pos, rect.y + rect.height * (1.f - (float)err_amt[i] / max_err) });
        x_pos += err_dist;
    }
}

void Graph::update()
{

}

void Graph::draw()
{
    Color line_col = rgba(0, 0, 0, 0.3);
    DrawRectangleLinesEx(rect, 1, line_col);

    // DRAW y-grid
    for (float y = rect.y + rect.height; y >= rect.y; y -= grid_y_gap)
    {
        if (y > rect.y && y < rect.y + rect.height) // no need to redraw
            DrawLine(rect.x, y, rect.x + rect.width, y, line_col);
        // wpm
        float y_v = max_v * ((rect.y + rect.height) - y) / rect.height;
        DrawTextAlign(t_s((int)round(y_v)), rect.x - 10, y, 20, theme.sub, RIGHT, CENTER);
    }

    float err_y_gap = max_err <= 5 ? (rect.height / max_err) : (rect.height / (10));
    for (float y = rect.y + rect.height; y >= rect.y; y -= err_y_gap)
    {
         // errors
        float y_v = max_err * ((rect.y + rect.height) - y) / rect.height;
        DrawTextAlign(t_s((int)y_v), rect.x + rect.width + 10, y, 20, theme.sub, LEFT, CENTER);
    }
    // Draw x-axis
    int sec_gap = round(time / 15);
    if (sec_gap < 1) sec_gap = 1;
    float x_gap = rect.width * (sec_gap / (time-1));  // theere are time-1 seconds in plot because plot starts at 1 second
    int sec = 1;
    for (float x = rect.x; x <= rect.x + rect.width; x += x_gap)
    {
        if (x > rect.x && x < rect.x + rect.width)  // no need to redraw
            DrawLine(x, rect.y, x, rect.y + rect.height, line_col);
        DrawTextAlign(t_s(sec), x, rect.y + rect.height + 10, 20, theme.sub, CENTER, TOP);
        sec += sec_gap;
    }    
    
    // RAW curve polygon
    for (vector<Vector2>& segment : draw_segments)
        DrawTriangleFan(&segment[0], segment.size(), rgba(0, 0, 0, 0.15));
    
    // RAW curve
    DrawTriangleStrip(&draw_points[RAW][0], draw_points[RAW].size(), theme.sub);
    // NORMAL curve
    DrawTriangleStrip(&draw_points[NORMAL][0], draw_points[NORMAL].size(), theme.main);
    // error points 
    for (Vector2& p : errors)
        DrawCircleV(p, 3, RED);

    if (IsKeyDown(KEY_SPACE))
    {
        for (Vector2& p : points[NORMAL])
        {
            DrawCircleV(p, 3, BLUE);
        }
    }
}

void Graph::set_time(float t)
{
    time = t;
}

float Graph::width()
{
    return rect.width;
}