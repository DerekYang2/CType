#pragma once
#include "globals.h"

class BezierPath
{
private:
    int SEGMENTS_PER_CURVE = 15;
    float MINIMUM_SQR_DISTANCE = 0.01f;
    float DIVISION_THRESHOLD = -0.99f;

    std::vector<Vector2> controlPoints;
    int curveCount;

public:
    BezierPath(int segments);
    void SetControlPoints(std::vector<Vector2> newControlPoints);
    std::vector<Vector2> GetControlPoints();
    void Interpolate(std::vector<Vector2> segmentPoints, float scale);
    void SamplePoints(std::vector<Vector2> sourcePoints, float minSqrDistance, float maxSqrDistance, float scale);
    Vector2 CalculateBezierPoint(int curveIndex, float t);
    std::vector<Vector2> GetDrawingPoints0();
    std::vector<Vector2> GetDrawingPoints();
    vector<Vector2> applyThickness(vector<Vector2>& points, float thickness);
    vector<vector<Vector2>> GetPolygonPoints();
    vector<Vector2> GetDrawingPoints(float thickness);  // array, size
    std::vector<Vector2> FindDrawingPoints(int curveIndex);
    int FindDrawingPoints(int curveIndex, float t0, float t1, std::vector<Vector2> pointList, int insertionIndex);
    Vector2 CalculateBezierPoint(float t, Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3);
};