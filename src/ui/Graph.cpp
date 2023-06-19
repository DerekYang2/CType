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
        LUTy[t].clear();
    }
    errors.clear();
    draw_segments.clear();
    max_v = 20;  // cannot be smaller than 20
    max_err = 0;
}

void Graph::fillLUT(int wpm_type)
{
    LUTy[wpm_type].clear();
    LUTy[wpm_type].insert({ draw_points[wpm_type][0].x, draw_points[wpm_type][0].y });
    for (int i = 1; i < draw_points[wpm_type].size(); i++)
    {
        int x_l = round(draw_points[wpm_type][i - 1].x);
        int x_r = round(draw_points[wpm_type][i].x);
        for (int x = x_l + 1; x <= x_r; x++)  // do not include x_l, done on previous iteration
        {
            if (!LUTy[wpm_type].contains(x))
            {
                LUTy[wpm_type][x] = draw_points[wpm_type][i - 1].y + (x - draw_points[wpm_type][i - 1].x) * (draw_points[wpm_type][i].y - draw_points[wpm_type][i - 1].y) / (draw_points[wpm_type][i].x - draw_points[wpm_type][i - 1].x);
            }
        }
    }
}

void Graph::config_max(vector<float>& plot_points, int wpm_type)
{
    float min_val = INT_MAX, max_val = INT_MIN;
    for (float& v : plot_points)
    {
        min_val = min(min_val, v), max_val = max(max_val, v);
    }
    min_val = max(min_val, max_val * 0.1f);  // keep max_val at least * 1.1
    if (wpm_type == NORMAL)
    {
        max_v = max(max_v, max_val + min_val); // same distance from 0-min and max-top
    } else {
        max_v = max(max_v, max_val * 1.1f); // if raw, max_v is just max value with a bit of padding
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
    const float max_contiguous = 100;
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
            } else if (curve_p[i-1].x - curve_p[st].x >= max_contiguous) // too wide
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
            } else if (curve_p[i-1].x - curve_p[st].x >= max_contiguous) // too wide
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
    
    // calculate interpolation curve array
    PiecewiseCubic curve(iter_f(plot_points.size()));
    curve.Interpolate(points[wpm_type]);
    
    if (wpm_type == RAW)
    {
        draw_points[wpm_type] = curve.GetDrawingPoints();
        // make sure curve doesn't cross out of the bottom 
        for (int i = 0; i < draw_points[wpm_type].size(); i++)
            if (draw_points[wpm_type][i].y > rect.y + rect.height)
                draw_points[wpm_type][i].y = rect.y + rect.height;
        fillLUT(wpm_type);
        draw_points[wpm_type] = curve.applyThickness(draw_points[wpm_type], thick);
        init_polygon(curve.GetDrawingPoints());  // curve points
    } else
    {
        draw_points[wpm_type] = curve.GetDrawingPoints();
        // make sure curve doesn't cross out of the bottom 
        for (int i = 0; i < draw_points[wpm_type].size(); i++)
            if (draw_points[wpm_type][i].y > rect.y + rect.height)
                draw_points[wpm_type][i].y = rect.y + rect.height;
        fillLUT(wpm_type);
        draw_points[wpm_type] = curve.applyThickness(draw_points[wpm_type], thick);
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
    if (max_err > 9)
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
    show_hint = false;
    const float min_dist = 12;  // minmimum distance to display error

    if (CheckCollisionPointRec(mouse, rect) && mouse_focus)
    {
        float normal_y = LUTy[NORMAL][(int)round(mouse.x)];
        float instant_y = LUTy[RAW][(int)round(mouse.x)];
        float closest_err_x = INT_MAX;
        for (auto& [x_p, y_p] : errors)
        {
            if (abs(y_p - mouse.y) < min_dist)  // only check x if y is close enough
                closest_err_x = min(closest_err_x, abs(x_p - mouse.x));
        }
        if (min(abs(mouse.y - normal_y), abs(mouse.y - instant_y)) <= 3 * min_dist || (closest_err_x <= min_dist))  // if close to curve or error point
            show_hint = true;
    }
        
    errors_hint = 0;
    error_index = -1;  // reset to nothing
    if (show_hint)
    {
        if (errors.empty())
        {
            errors_hint = 0;
        } else
        {
            Vector2 closest = { INT_MAX, errors[0].y };
            int best_idx = -1;
            for (int i = 0; i < errors.size(); i++)
            {
                if (abs(errors[i].x - mouse.x) < closest.x)
                {
                    closest = { abs(errors[i].x - mouse.x), errors[i].y };
                    best_idx = i;
                }
            }
            if (closest.x <= min_dist)
            {
               errors_hint = round(pos_to_value(closest.y, max_err));
               error_index = best_idx;  
            }
        }
    }
}

void Graph::draw_hint()
{

    float normal_y = LUTy[NORMAL][(int)round(mouse.x)];
    float instant_y = LUTy[RAW][(int)round(mouse.x)];
    string time_str = convertSeconds(1 + (time - 1) * (mouse.x - rect.x) / rect.width, time);
    if (time < 60)
        time_str = TextFormat("%.1f", 1 + (time - 1) * (mouse.x - rect.x) / rect.width);
    string wpm_str = t_s((int)round(pos_to_value(normal_y, max_v)));
    string instant_str = t_s((int)round(pos_to_value(instant_y, max_v)));
    string error_str = t_s(errors_hint);
    float hint_fs = 22;
    float time_h = MeasureTextEx(time_str, hint_fs).y, normal_h = MeasureTextEx(wpm_str, hint_fs).y, instant_h = MeasureTextEx(instant_str, hint_fs).y, error_h = MeasureTextEx(error_str, hint_fs).y;
    
    float text_max_w = max({ MeasureTextEx("wpm  " + wpm_str, hint_fs).x, MeasureTextEx("instant  " + instant_str, hint_fs).x, MeasureTextEx("errors  " + error_str, hint_fs).x });
    float padding = 0.25f * MeasureTextEx("I", hint_fs).y;
    float box_height = 5 * padding + time_h + normal_h + instant_h + error_h;
    Vector2 hint_offset;
    float offset_amt = 2 * padding;
    Rectangle hint_box = { 0, 0, text_max_w + 2 * padding, box_height};
    if (mouse.x < rect.x + rect.width * 0.5f)  // left side
    {
        hint_offset = { offset_amt, offset_amt };
        if (normal_y - 1.1f * hint_box.height + hint_offset.y < rect.y) // top-left corner
        {
            hint_box.x = mouse.x + hint_offset.x, hint_box.y = normal_y + hint_offset.y;
        } else // bottom_left corner
        {
            hint_offset = { offset_amt, -offset_amt };
            hint_box.x = mouse.x + hint_offset.x, hint_box.y = normal_y - hint_box.height + hint_offset.y;
        }
    } else 
    {
        hint_offset = { -offset_amt, offset_amt };
        if (normal_y - 1.1f * hint_box.height + hint_offset.y < rect.y) // top-right corner
        {
            hint_box.x = mouse.x - hint_box.width + hint_offset.x, hint_box.y = normal_y + hint_offset.y;
        } else // bottom_right corner
        {
            hint_offset = { -offset_amt, -offset_amt };
            hint_box.x = mouse.x - hint_box.width + hint_offset.x, hint_box.y = normal_y - hint_box.height + hint_offset.y;
        }
    }
    // Draw circle points
    DrawCircle(mouse.x, instant_y, 5, theme.sub);
    DrawCircle(mouse.x, normal_y, 5, theme.main);
    
    DrawRectangleRounded(hint_box, 0.1f, 7, rgba(0, 0, 0, 0.95f));
    float ypos = padding;
    DrawTextAlign(time_str, hint_box.x + padding, hint_box.y + ypos, hint_fs, WHITE), ypos += time_h;
    ypos += padding;

    DrawTextAlign("wpm", hint_box.x + padding, hint_box.y + ypos, hint_fs, WHITE);
    DrawTextAlign(wpm_str, hint_box.x + hint_box.width - padding, hint_box.y + ypos, hint_fs, WHITE, RIGHT);
    ypos += normal_h + padding;

    DrawTextAlign("instant", hint_box.x + padding, hint_box.y + ypos, hint_fs, WHITE);
    DrawTextAlign(instant_str, hint_box.x + hint_box.width - padding, hint_box.y + ypos, hint_fs, WHITE, RIGHT);
    ypos += instant_h + padding;

    DrawTextAlign("errors", hint_box.x + padding, hint_box.y + ypos, hint_fs, WHITE);
    DrawTextAlign(error_str, hint_box.x + hint_box.width - padding, hint_box.y + ypos, hint_fs, WHITE, RIGHT);
    // Draw rectangle 
    //DrawRectangleRoundedAlign()
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
        int y_v = round(pos_to_value(y, max_v));
        DrawTextAlign(t_s(y_v), rect.x - 10, y, 20, theme.sub, RIGHT, CENTER);
    }

    float err_y_gap = max_err <= 9 ? (rect.height / max_err) : (rect.height / (10));
    for (float y = rect.y + rect.height; y >= rect.y; y -= err_y_gap)
    {
         // errors
        int y_v = pos_to_value(y, max_err);
        DrawTextAlign(t_s(y_v), rect.x + rect.width + 10, y, 20, theme.sub, LEFT, CENTER);
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

    if (show_hint)
        draw_hint();

    // error points 
    for (int i = 0; i < errors.size(); i++)
    {
        float stroke = 4.f;
        if (i == error_index && show_hint)
            stroke += sin(rad(globalFrame * 6));
        DrawRectanglePro({ errors[i].x, errors[i].y, stroke, 3 * stroke }, { stroke * 0.5f, 3 * stroke * 0.5f }, 45, RED);
        DrawRectanglePro({ errors[i].x, errors[i].y, stroke, 3 * stroke }, { stroke * 0.5f, 3 * stroke * 0.5f }, 45 + 90, RED);
    }

    if (IsKeyDown(KEY_SPACE))
    {
        for (int i = 0; i <= 1; i++)
        for (Vector2& p : points[i])
        {
            DrawCircleV(p, 3, GREEN);
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

float Graph::pos_to_value(float position, float max_val)
{
    return max_val * ((rect.y + rect.height) - position) / rect.height;
}
