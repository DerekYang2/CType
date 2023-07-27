#include "globals.h"

// overloads
//Addition overloads: componentwise addition
Vector2 operator+(const Vector2& a, const Vector2& b) {
return Vector2Add(a,b);
}

Vector3 operator+(const Vector3& a, const Vector3& b) {
return Vector3Add(a,b);
}

Vector4 operator+(const Vector4& a, const Vector4& b) {
return (Vector4){a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
}

Vector2& operator+=(Vector2& a, const Vector2& b) {
    a=a+b;
return a;
}

Vector3& operator+=(Vector3& a, const Vector3& b) {
    a=a+b;
return a;
}

Vector4& operator+=(Vector4& a, const Vector4& b) {
    a=a+b;
return a;
}

Matrix operator+(const Matrix& left, const Matrix& right) {
return MatrixAdd(left,right);
}

Matrix& operator+=(Matrix& left, const Matrix& right) {
    left=MatrixAdd(left,right);
return left;
}

Color operator+(const Color& a, const Color& b) {
    int red,green,blue,alpha;  //To ensure that addition of a+b does not result in components greater than unsigned char's max of 255, we upconvert to int, then check the sum, and cast back to unsigned char
    Color c;
    red=(int)a.r+(int)b.r;
    green=(int)a.g+(int)b.g;
    blue=(int)a.b+(int)b.b;
    alpha=(int)a.a+(int)b.a;

    c.r=(unsigned char)((red>255)?255:red);
    c.g=(unsigned char)((green>255)?255:green);
    c.b=(unsigned char)((blue>255)?255:blue);
    c.r=(unsigned char)((alpha>255)?255:alpha);
return c;
}

Color& operator+=(Color&a, const Color& b) {
    a=a+b;
return a;
}

//Negation: Unary Minus operator
Vector2& operator-(Vector2& a) {
    a=Vector2Negate(a);
return a;
}

Vector3& operator-(Vector3& a) {
    a=Vector3Negate(a);
return a;
}

//Since Quaternion is a Vector4 typedef, I did not provide a negation for Vector4 to avoid confusion with QuaternionInvert();

//Subtraction overloads: componentwise subtraction
Vector2 operator-(const Vector2& a, const Vector2& b) {
return Vector2Subtract(a,b);
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
return Vector3Subtract(a,b);
}

Vector4 operator-(const Vector4& a, const Vector4& b) {
return (Vector4){a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
}

Vector2& operator-=(Vector2& a, const Vector2& b) {
    a=a-b;
return a;
}

Vector3& operator-=(Vector3& a, const Vector3& b) {
    a=a-b;
return a;
}

Vector4& operator-=(Vector4& a, const Vector4& b) {
    a=a-b;
return a;
}

Matrix operator-(const Matrix& left, const Matrix& right) {
return MatrixSubtract(left,right);
}

Matrix& operator-=(Matrix& left, const Matrix& right) {
    left=MatrixSubtract(left,right);
return left;
}

Color operator-(const Color& a, const Color& b) {
    int red,green,blue,alpha;  //Convert to int, clamp to min zero, convert back to unsigned char
    Color c;
    red=(int)a.r-(int)b.r;
    green=(int)a.g-(int)b.g;
    blue=(int)a.b-(int)b.b;
    alpha=(int)a.a-(int)b.a;

    c.r=(unsigned char)((red<0)?0:red);
    c.g=(unsigned char)((green<0)?0:green);
    c.b=(unsigned char)((blue<0)?0:blue);
    c.r=(unsigned char)((alpha<0)?0:alpha);
return c;
}

Color& operator-=(Color&a, const Color& b) {
    a=a-b;
return a;
}

//Multiplication overload only provides for multiplying a vector by a scalar.   Vector * Vector is not overloaded to avoid confusion whether one intends a dot product, cross product, etc.
Vector2 operator*(const Vector2& a, float b) {
return Vector2Scale(a,b);
}
Vector2 operator*(float b, const Vector2& a)
{
return Vector2Scale(a,b);
}

Vector3 operator*(const Vector3& a, float b)
{
return Vector3Scale(a,b);
}

Vector3 operator*(float b, const Vector3& a)
{
return Vector3Scale(a,b);
}

Matrix operator*(const Matrix& left, const Matrix& right) {
return MatrixMultiply(left,right);
}

Matrix& operator*=(Matrix& left, const Matrix& right) {
    left=MatrixMultiply(left,right);
return left;
}

Vector2& operator*=(Vector2& a, const float b) {
    a=a*b;
return a;
}

Vector3& operator*=(Vector3& a, const float b) {
    a=a*b;
return a;
}

//I'm not sure if the following is useful, but here it is anyway for completeness
Color operator*(const Color& a, const Color& b) {
    int red,green,blue,alpha;
    Color c;
    red=(int)a.r*(int)b.r;
    green=(int)a.g*(int)b.g;
    blue=(int)a.b*(int)b.b;
    alpha=(int)a.a*(int)b.a;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator*=(Color&a, const Color& b) {
    a=a*b;
return a;
}

Color operator*(const Color& a, const float b) {
    float red,green,blue,alpha;  //We cast to float, clamp, then recast back to unsigned char
    Color c;
    red=(float)a.r*b;
    green=(float)a.g*b;
    blue=(float)a.b*b;
    alpha=(float)a.a*b;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator*=(Color&a, const float b) {
    a=a*b;
return a;
}

//Division overload: Merely scalar multiplication by the reciprocal, with a Divide-By-Zero check.
Vector2 operator/(const Vector2& a, const float b) {
    if (b==0.0) {
        std::cerr<<"Division by zero error."<<std::endl;
        throw std::domain_error("Division by zero error");
    }
    float recip=1.0/b;
return a*recip;
}

Vector3 operator/(const Vector3& a, const float b) {
    if (b==0.0) {
        std::cerr<<"Division by zero error."<<std::endl;
        throw std::domain_error("Division by zero error");
    }
    float recip=1.0/b;
return a*recip;
}

Vector2& operator/=(Vector2& a, const float b) {
    a=a/b;
return a;
}

Vector3& operator/=(Vector3& a, const float b) {
    a=a/b;
return a;
}

//I'm not sure if the following is useful either, but here it is for completeness
Color operator/(const Color& a, const Color& b) {
    float red,green,blue,alpha;
    Color c;
    red=(float)a.r/(float)b.r;
    green=(float)a.g/(float)b.g;
    blue=(float)a.b/(float)b.b;
    alpha=(float)a.a/(float)b.a;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator/=(Color&a, const Color& b) {
    a=a/b;
return a;
}

Color operator/(const Color& a, const float b) {
    float red,green,blue,alpha;  //We cast to float, clamp, then recast back to unsigned char
    Color c;
    red=(float)a.r/b;
    green=(float)a.g/b;
    blue=(float)a.b/b;
    alpha=(float)a.a/b;

    c.r=(unsigned char)( ( (red>255)?255:(red<0)?0:red) ); //Nested ternary clamps return between 0 and 255
    c.g=(unsigned char)( ( (green>255)?255:(green<0)?0:green) );
    c.b=(unsigned char)( ( (blue>255)?255:(blue<0)?0:blue) );
    c.r=(unsigned char)( ( (alpha>255)?255:(alpha<0)?0:alpha) );
return c;
}

Color& operator/=(Color&a, const float b) {
    a=a/b;
return a;
}


// Equality operator overloads
//
// Integer equality
bool operator==(const Color& c1, const Color& c2) {
    if ( (c1.r==c2.r) && (c1.g==c2.g) && (c1.b==c2.b) && (c1.a==c2.a) ) return true;
return false;
}

// Float equality for Vector2 and Vector3
//
//Comparing float values requires care.  Choose EQUALITY_OPERATOR_SIMPLE, EQUALITY_OPERATOR_KNUTH, or neither in the #defines at the top of the file
#ifdef EQUALITY_OPERATOR_SIMPLE
bool operator==(const Vector2& a, const Vector2& b) {
    if ( (a.x==b.x) && (a.y==b.y)) return true;
return false;
}

bool operator==(const Vector3& a, const Vector3& b) {
    if ( (a.x==b.x) && (a.y==b.y) && (a.z==b.z)) return true;
return false;
}

bool operator==(const Vector4& a, const Vector4& b) {
    if ( (a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)) return true;
return false;
}
#endif // EQUALITY_OPERATOR_SIMPLE

#ifdef EQUALITY_OPERATOR_KNUTH
//Takes a conservative approach and only affirms that two vectors are equal if all of their respective components are equal within machine precision.
#include <limits>
#include <cmath>
bool operator==(const Vector2& a, const Vector2& b){
    bool XIsEqual=false;
    bool YIsEqual=false;
    XIsEqual = std::fabs(a.x - b.x) <= ( (std::fabs(a.x) > std::fabs(b.x) ? std::fabs(b.x) : std::fabs(a.x)) * std::numeric_limits<float>::epsilon() );
    YIsEqual = std::fabs(a.y - b.y) <= ( (std::fabs(a.y) > std::fabs(b.y) ? std::fabs(b.y) : std::fabs(a.y)) * std::numeric_limits<float>::epsilon() );
return (XIsEqual && YIsEqual);
}

bool operator==(const Vector3& a, const Vector3& b){
    bool XIsEqual=false;
    bool YIsEqual=false;
    bool ZIsEqual=false;
    XIsEqual = std::fabs(a.x - b.x) <= ( (std::fabs(a.x) > std::fabs(b.x) ? std::fabs(b.x) : std::fabs(a.x)) * std::numeric_limits<float>::epsilon() );
    YIsEqual = std::fabs(a.y - b.y) <= ( (std::fabs(a.y) > std::fabs(b.y) ? std::fabs(b.y) : std::fabs(a.y)) * std::numeric_limits<float>::epsilon() );
    ZIsEqual = std::fabs(a.z - b.z) <= ( (std::fabs(a.z) > std::fabs(b.z) ? std::fabs(b.z) : std::fabs(a.z)) * std::numeric_limits<float>::epsilon() );
return (XIsEqual && YIsEqual && ZIsEqual);
}

bool operator==(const Vector4& a, const Vector4& b){
    bool XIsEqual=false;
    bool YIsEqual=false;
    bool ZIsEqual=false;
    bool WIsEqual=false;
    XIsEqual = std::fabs(a.x - b.x) <= ( (std::fabs(a.x) > std::fabs(b.x) ? std::fabs(b.x) : std::fabs(a.x)) * std::numeric_limits<float>::epsilon() );
    YIsEqual = std::fabs(a.y - b.y) <= ( (std::fabs(a.y) > std::fabs(b.y) ? std::fabs(b.y) : std::fabs(a.y)) * std::numeric_limits<float>::epsilon() );
    ZIsEqual = std::fabs(a.z - b.z) <= ( (std::fabs(a.z) > std::fabs(b.z) ? std::fabs(b.z) : std::fabs(a.z)) * std::numeric_limits<float>::epsilon() );
    WIsEqual = std::fabs(a.w - b.w) <= ( (std::fabs(a.w) > std::fabs(b.w) ? std::fabs(b.w) : std::fabs(a.w)) * std::numeric_limits<float>::epsilon() );
return (XIsEqual && YIsEqual && ZIsEqual && WIsEqual);
}
#endif // EQUALITY_OPERATOR_KNUTH