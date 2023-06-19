#include "PiecewiseCubic.h"

PiecewiseCubic::PiecewiseCubic(int segments)
{
    SEGMENTS_PER_CURVE = segments;
    points.clear();
    pnt.clear();
}

std::vector<Vector2> PiecewiseCubic::get_points()
{
    return points;
}

Vector2 PiecewiseCubic::GetPoint(float t)
{
    int n2 = 2 * n;
    int i, ii;
    float p0[2], p1[2], p2[2], p3[2], a0, a1, a2, a3, d1, d2, tt, ttt;
    
    float p[2];
    // handle t out of range
    if (t <= 0.0f)
    {
        return Vector2(pnt[0], pnt[1]);
    }
    if (t >= float(n - 1))
    {
        return Vector2(pnt[n2 - 2], pnt[n2 - 1]);
    }
    // select patch
    i=floor(t);             // start point of patch
    t-=i;                   // parameter <0,1>
    i<<=1; tt=t*t; ttt=tt*t;
    // control points
    ii = i - 2; if (ii < 0) ii = 0; if (ii >= n2) ii = n2 - 2;
    p0[0] = pnt[ii], p0[1] = pnt[ii + 1];
    
    ii = i; if (ii < 0) ii = 0; if (ii >= n2) ii = n2 - 2;
    p1[0] = pnt[ii], p1[1] = pnt[ii + 1];
    
    ii = i + 2; if (ii < 0) ii = 0; if (ii >= n2) ii = n2 - 2;
    p2[0] = pnt[ii], p2[1] = pnt[ii + 1];
    
    ii = i + 4; if (ii < 0) ii = 0; if (ii >= n2) ii = n2 - 2;
    p3[0] = pnt[ii], p3[1] = pnt[ii + 1];
    
    // loop all dimensions
    for (i=0;i<2;i++)
    {
        // compute polynomial coeficients
        d1=0.5f*(p2[i]-p0[i]);
        d2=0.5f*(p3[i]-p1[i]);
        a0=p1[i];
        a1=d1;
        a2=(3.0f*(p2[i]-p1[i]))-(2.0f*d1)-d2;
        a3=d1+d2+(2.0f*(-p2[i]+p1[i]));
        // compute point coordinate
        p[i]=a0+(a1*t)+(a2*tt)+(a3*ttt);
    }
    return Vector2(p[0], p[1]);
}

void PiecewiseCubic::Interpolate(std::vector<Vector2> segmentPoints)
{
    points = segmentPoints;
    n = points.size();
    pnt.clear();
    for (int i = 0; i < n; i++)
    {
        pnt.push_back(points[i].x);
        pnt.push_back(points[i].y);
    }
}

std::vector<Vector2> PiecewiseCubic::GetDrawingPoints()
{
    vector<Vector2> drawingPoints;
    float t_inc = 1.0f / SEGMENTS_PER_CURVE;
    for (float t = 0.f; t < float(n - 1); t += t_inc)
    {
        drawingPoints.push_back(GetPoint(t));
    }
    drawingPoints.push_back(GetPoint(n - 1));
    return drawingPoints;
}

vector<Vector2> PiecewiseCubic::applyThickness(vector<Vector2>& points, float thickness)
{
    Vector2 point_arr[2 * points.size()];

    Vector2 previous = points[0];
    Vector2 current = { 0 };
    for (int i = 1; i < points.size(); i++)
    {
        current = points[i];
        float dy = current.y - previous.y;
        float dx = current.x-previous.x;
        float size = 0.5f*thickness/sqrtf(dx*dx+dy*dy);

        if (i==1)
        {
            point_arr[0].x = previous.x+dy*size;
            point_arr[0].y = previous.y-dx*size;
            point_arr[1].x = previous.x-dy*size;
            point_arr[1].y = previous.y+dx*size;
        }

        point_arr[2*i+1].x = current.x-dy*size;
        point_arr[2*i+1].y = current.y+dx*size;
        point_arr[2*i].x = current.x+dy*size;
        point_arr[2*i].y = current.y-dx*size;
        previous = current;
    }
    return vector<Vector2>(point_arr, point_arr + 2 * points.size());
}

vector<Vector2> PiecewiseCubic::GetDrawingPoints(float thickness)
{
    auto drawingPoints = GetDrawingPoints();
    return applyThickness(drawingPoints, thickness);
}

void PiecewiseCubic::fill_LUT(unordered_map<int, float>& lut)
{
    lut.clear();
    auto draw_p = GetDrawingPoints();
    lut.insert({ draw_p[0].x, draw_p[0].y });
    for (int i = 1; i < draw_p.size(); i++)
    {
        int x_l = round(draw_p[i - 1].x);
        int x_r = round(draw_p[i].x);
        for (int x = x_l + 1; x <= x_r; x++)  // do not include x_l, done on previous iteration
        {
            if (!lut.contains(x))
            {
                lut[x] = draw_p[i - 1].y + (x - draw_p[i - 1].x) * (draw_p[i].y - draw_p[i - 1].y) / (draw_p[i].x - draw_p[i - 1].x);
            }
        }
    }
}



