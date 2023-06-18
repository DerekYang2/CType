#pragma once
#include "globals.h"

class PiecewiseCubic
{
private:
    int SEGMENTS_PER_CURVE = 15;
    int n; 
    std::vector<Vector2> points;
    vector<float> pnt;
    int curveCount;

public:
    PiecewiseCubic(int segments);
    std::vector<Vector2> get_points();
    Vector2 GetPoint(float t);
    void Interpolate(std::vector<Vector2> segmentPoints);
    std::vector<Vector2> GetDrawingPoints();
    vector<Vector2> applyThickness(vector<Vector2>& points, float thickness);
    vector<Vector2> GetDrawingPoints(float thickness);  // array, size
    void fill_LUT(unordered_map<int, float> &lut);
};