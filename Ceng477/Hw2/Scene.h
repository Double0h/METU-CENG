#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix4.h"

using namespace std;

class Scene
{
public:
    Color backgroundColor;
    bool cullingEnabled;
    int projectionType;

    vector< vector<Color> > image;
    vector< Camera* > cameras;
    vector< Vec3* > vertices;
    vector< Color* > colorsOfVertices;
    vector< Scaling* > scalings;
    vector< Rotation* > rotations;
    vector< Translation* > translations;
    vector< Model* > models;

    Scene(const char *xmlPath);

    void initializeImage(Camera* camera);
    void forwardRenderingPipeline(Camera* camera);
    int makeBetweenZeroAnd255(double value);
    void writeImageToPPMFile(Camera* camera);
    void convertPPMToPNG(string ppmFileName, int osType);
    Matrix4 getScalingMatrix(int i);
    Matrix4 getRotationMatrix(int id);
    Matrix4 getTranslationMatrix(int id);
    static Matrix4 getCamTransformationMatrix(Camera *cam);
    static Matrix4 getOrthMatrix(Camera *cam);
    static Matrix4 getp2oMatrix(double far, double near);

    static Matrix4 vpTransformMatrix(Camera *cam);

    static Vec4 persDivide(Vec4 point);

    void lineRasterization(double x0,double x1,double y0, double y1,Color c0,Color c1);

    void draw(double x, double y, Color c);

    void triangleRasterization(Vec4 v_1, Vec4 v_2, Vec4 v_3, Color c_1, Color c_2, Color c_3);

    static bool visible(double den, double num, double* t_e, double* t_l);

    void clipping(Vec4 v_0, Vec4 v_1, int x_max, int x_min, int y_max, int y_min);


};

#endif