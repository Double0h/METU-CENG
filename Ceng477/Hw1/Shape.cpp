#include "Shape.h"
#include "Scene.h"
#include <cstdio>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	this->center = pVertices->at(cIndex-1);
	this->radius = R;
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	ReturnVal r;
	float b = 2*(ray.direction.dot(ray.origin - center));
	//printf("%f",b);
	float a = ray.direction.dot(ray.direction);
	float c = (ray.origin - center).dot(ray.origin - center) - radius*radius;
	float delta = sqrt(b*b -4*a*c);
	//printf("%f",b*b -4*a*c);
	
	if ((b*b -4*a*c) >= 0) {
		//printf("%f",delta);
		r.intersects = true;
		float t1 = (-1*b + delta)/(2*a);
		float t2 = (-1*b - delta)/(2*a);
		if(t1 >= t2)
			r.t=t2;
		else
			r.t=t1;
		r.point = ray.getPoint(r.t);
		r.normal = (r.point - center).normalized();
		r.Matindex = this->matIndex;
}
	else
		r.intersects = false;	
	return r;
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	this->A = pVertices->at(p1Index-1);
	
	this->B = pVertices->at(p2Index-1);
	
	this->C = pVertices->at(p3Index-1);
	
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	ReturnVal r;
	float beta,gamma,t,det;
	Matrix3f divider,Bet,Gam,T;
	divider << A.x()-B.x(), A.x()-C.x(), ray.direction.x(),
			A.y()-B.y(), A.y()-C.y(), ray.direction.y(),
			A.z()-B.z(), A.z()-C.z(), ray.direction.z();

	det = divider.determinant();
	if(det == 0){
		r.intersects = false;
		return r; }

	Bet << A.x()-ray.origin.x(), A.x()-C.x(), ray.direction.x(),
			A.y()-ray.origin.y(), A.y()-C.y(), ray.direction.y(),
			A.z()-ray.origin.z(), A.z()-C.z(), ray.direction.z();

	beta = Bet.determinant()/det;

	

	Gam << A.x()-B.x(), A.x()-ray.origin.x(), ray.direction.x(),
			A.y()-B.y(), A.y()-ray.origin.y(), ray.direction.y(),
			A.z()-B.z(), A.z()-ray.origin.z(), ray.direction.z();

	gamma = Gam.determinant()/det;

	T << A.x()-B.x(), A.x()-C.x(), A.x()-ray.origin.x(),
			A.y()-B.y(), A.y()-C.y(), A.y()-ray.origin.y(),
			A.z()-B.z(), A.z()-C.z(), A.z()-ray.origin.z();

	t = T.determinant()/det;
	if(t > 0 && beta + gamma <= 1 && 0 <= beta && 0 <= gamma){
		
		r.intersects = true;
		r.point = ray.getPoint(t);
		r.normal = (C-B).cross(A-B).normalized();
		r.Matindex = this->matIndex;
		r.t=t;
		
		return r;
}
	r.intersects = false;
	return r;
	
	
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	this->face = faces;
	this->vertices = pVertices;
	this->indices = pIndices;
	
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	ReturnVal r;
	ReturnVal r2;
	for(int i =0;i<face.size();i++){
		
		
		
		
		r=face[i].intersect(ray);
		if(r.intersects == true && r2.t>r.t){
			r2=r;
		}
		
	}
	return r2;
	
	
}
