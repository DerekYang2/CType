#include "BezierPath.h"

BezierPath::BezierPath(int segments)
{
    controlPoints.clear();
    SEGMENTS_PER_CURVE = segments;
    MINIMUM_SQR_DISTANCE = 0.01f;
    DIVISION_THRESHOLD = -0.99f;
}

void BezierPath::SetControlPoints(std::vector<Vector2> newControlPoints)
{
    controlPoints = newControlPoints;
    curveCount = (controlPoints.size() - 1) / 3;
}

std::vector<Vector2> BezierPath::GetControlPoints()
{
    return controlPoints;
}

void BezierPath::Interpolate(std::vector<Vector2> segmentPoints, float scale)
{
    controlPoints.clear();

    if (segmentPoints.size() < 2)
    {
        return;
    }

    for (int i = 0; i < segmentPoints.size(); i++)
    {
        if (i == 0)
        {
            Vector2 p1 = segmentPoints[i];
            Vector2 p2 = segmentPoints[i + 1];

            Vector2 tangent = (p2 - p1);
            Vector2 q1 = p1 + tangent * scale;

            controlPoints.push_back(p1);
            controlPoints.push_back(q1);
        } 
        else if (i == segmentPoints.size() - 1)
        {
            Vector2 p0 = segmentPoints[i - 1];
            Vector2 p1 = segmentPoints[i];
            Vector2 tangent = (p1 - p0);
            Vector2 q0 = p1 - tangent * scale;

            controlPoints.push_back(q0);
            controlPoints.push_back(p1);
        } 
        else
        {
            Vector2 p0 = segmentPoints[i - 1];
            Vector2 p1 = segmentPoints[i];
            Vector2 p2 = segmentPoints[i + 1];
            Vector2 tangent = Vector2Normalize(p2 - p0);
            Vector2 q0 = p1 - tangent * (scale * Vector2Length(p1 - p0));
            Vector2 q1 = p1 + tangent * (scale * Vector2Length(p2 - p1));
            controlPoints.push_back(q0);
            controlPoints.push_back(p1);
            controlPoints.push_back(q1);
        }
    }

    curveCount = (controlPoints.size() - 1) / 3;
}

void BezierPath::SamplePoints(std::vector<Vector2> sourcePoints, float minSqrDistance, float maxSqrDistance, float scale)
{
    if (sourcePoints.size() < 2)
    {
        return;
    }

    std::vector<Vector2> samplePoints;

    samplePoints.push_back(sourcePoints[0]);

    Vector2 potentialSamplePoint = sourcePoints[1];

    int i = 2;

    for (i = 2; i < sourcePoints.size(); i++)
    {
        if (
            (Vector2LengthSqr(potentialSamplePoint - sourcePoints[i]) > minSqrDistance) &&
            (Vector2LengthSqr(samplePoints.back() - sourcePoints[i]) < maxSqrDistance))
        {
            samplePoints.push_back(potentialSamplePoint);
        }

        potentialSamplePoint = sourcePoints[i];
    }

    Vector2 p1 = samplePoints.back(); 
    samplePoints.pop_back();
    Vector2 p0 = samplePoints.back(); 
    Vector2 tangent = Vector2Normalize(p0 - potentialSamplePoint);
    float d2 = Vector2Length(potentialSamplePoint - p1);
    float d1 = Vector2Length(p1 - p0);
    p1 = p1 + tangent * ((d1 - d2) / 2);

    samplePoints.push_back(p1);
    samplePoints.push_back(potentialSamplePoint);
    Interpolate(std::vector<Vector2>(samplePoints), scale);
}
/**
    Caluclates a point on the path.

    @param curveIndex The index of the curve that the point is on. For example,
    the second curve (index 1) is the curve with controlpoints 3, 4, 5, and 6.

    @param t The paramater indicating where on the curve the point is. 0 corresponds
    to the "left" point, 1 corresponds to the "right" end point.
*/
Vector2 BezierPath::CalculateBezierPoint(int curveIndex, float t)
{
    int nodeIndex = curveIndex * 3;

    Vector2 p0 = controlPoints[nodeIndex];
    Vector2 p1 = controlPoints[nodeIndex + 1];
    Vector2 p2 = controlPoints[nodeIndex + 2];
    Vector2 p3 = controlPoints[nodeIndex + 3];

    return CalculateBezierPoint(t, p0, p1, p2, p3);
}

/**
    Gets the drawing points. This implementation simply calculates a certain number
    of points per curve.
*/
vector<Vector2> BezierPath::GetDrawingPoints0()
{
    vector<Vector2> drawingPoints;

    for (int curveIndex = 0; curveIndex < curveCount; curveIndex++)
    {
        if (curveIndex == 0) //Only do this for the first end point. 
        //When i != 0, this coincides with the 
        //end point of the previous segment,
        {
            drawingPoints.push_back(CalculateBezierPoint(curveIndex, 0));
        }

        for (int j = 1; j <= SEGMENTS_PER_CURVE; j++)
        {
            float t = j / (float)SEGMENTS_PER_CURVE;
            drawingPoints.push_back(CalculateBezierPoint(curveIndex, t));
        }
    }

    return drawingPoints;
}

/**
    Gets the drawing points. This implementation simply calculates a certain number
    of points per curve.

    This is a lsightly different inplementation from the one above.
*/
vector<Vector2> BezierPath::GetDrawingPoints()
{
    vector<Vector2> drawingPoints;

    for (int i = 0; i < controlPoints.size() - 3; i += 3)
    {
        Vector2 p0 = controlPoints[i];
        Vector2 p1 = controlPoints[i + 1];
        Vector2 p2 = controlPoints[i + 2];
        Vector2 p3 = controlPoints[i + 3];

        if (i == 0) //only do this for the first end point. When i != 0, this coincides with the end point of the previous segment,
        {
            drawingPoints.push_back(CalculateBezierPoint(0, p0, p1, p2, p3));
        }

        for (int j = 1; j <= SEGMENTS_PER_CURVE; j++)
        {
            float t = j / (float)SEGMENTS_PER_CURVE;
            drawingPoints.push_back(CalculateBezierPoint(t, p0, p1, p2, p3));
        }
    }

    return drawingPoints;
}

vector<Vector2> BezierPath::applyThickness(vector<Vector2> &points, float thickness)
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

vector<Vector2> BezierPath::GetDrawingPoints(float thickness)
{
    auto points = GetDrawingPoints0();
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


vector<Vector2> BezierPath::FindDrawingPoints(int curveIndex)
{
    vector<Vector2> pointList;

    Vector2 left = CalculateBezierPoint(curveIndex, 0);
    Vector2 right = CalculateBezierPoint(curveIndex, 1);

    pointList.push_back(left);
    pointList.push_back(right);

    FindDrawingPoints(curveIndex, 0, 1, pointList, 1);

    return pointList;
}


/**
    @returns the number of points added.
*/
int BezierPath::FindDrawingPoints(int curveIndex, float t0, float t1,
    vector<Vector2> pointList, int insertionIndex)
{
    Vector2 left = CalculateBezierPoint(curveIndex, t0);
    Vector2 right = CalculateBezierPoint(curveIndex, t1);

    if (Vector2LengthSqr(left - right) < MINIMUM_SQR_DISTANCE)
    {
        return 0;
    }

    float tMid = (t0 + t1) / 2;
    Vector2 mid = CalculateBezierPoint(curveIndex, tMid);

    Vector2 leftDirection = Vector2Normalize(left - mid);
    Vector2 rightDirection = Vector2Normalize(right - mid);

    if (Vector2DotProduct(leftDirection, rightDirection) > DIVISION_THRESHOLD || abs(tMid - 0.5f) < 0.0001f)
    {
        int pointsAddedCount = 0;

        pointsAddedCount += FindDrawingPoints(curveIndex, t0, tMid, pointList, insertionIndex);
        pointList.insert(pointList.begin() + insertionIndex + pointsAddedCount, mid);
        pointsAddedCount++;
        pointsAddedCount += FindDrawingPoints(curveIndex, tMid, t1, pointList, insertionIndex + pointsAddedCount);

        return pointsAddedCount;
    }

    return 0;
}



/**
    Caluclates a point on the Bezier curve represented with the four controlpoints given.
*/
Vector2 BezierPath::CalculateBezierPoint(float t, Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Vector2 p = uuu * p0; //first term

    p += 3 * uu * t * p1; //second term
    p += 3 * u * tt * p2; //third term
    p += ttt * p3; //fourth term

    return p;

}