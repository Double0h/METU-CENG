#include "Ray.h"





Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : origin(origin), direction(direction)
{
}

/* Takes a parameter t and returns the point according to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3f Ray::getPoint(float t) const 
{

    Vector3f point;

    point.x() = origin.x()+t*direction.x();
    point.y() = origin.y()+t*direction.y();
    point.z() = origin.z()+t*direction.z();

    return point;

}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
    return (p.x()-origin.x())/direction.x();
}

