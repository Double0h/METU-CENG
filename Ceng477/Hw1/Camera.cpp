#include "Camera.h"



Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
       
	this->id = id;
       w = gaze.normalized();
       u = w.cross(up).normalized();
       v = up.normalized();
	e = pos;
	
	for(int i = 0 ;i<32;i++){
	this->imageName[i] = *(imageName+i);}
	this->imgPlane = imgPlane;

      /*---- q = m + Left.u + Top.v --------*/
       
	
   
       /*---- Pw = (Right - Left) / Image Width ------*/
       
   
       /*---- Ph = (Top - Bottom ) / Image Height -----*/
       
	
}
    

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
	Vector3f m = e + (w * imgPlane.distance);
	float pixel_width = ( imgPlane.right - imgPlane.left ) / imgPlane.nx;
	float pixel_height = (imgPlane.top - imgPlane.bottom) / imgPlane.ny;
	Vector3f q = m + ((u * imgPlane.left) + (v * imgPlane.top));
        Vector3f pixel = q + (u * ((row+0.5)* pixel_width)) - (v * ((col+0.5) * pixel_height));
        Vector3f dir = pixel - e;
        Vector3f normalised = dir.normalized();
        return Ray(e,normalised);
}

