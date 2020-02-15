#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>


#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You can define helper functions inside Scene class implementation.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
    // TODO: Implement this function.


    for(int i=0;i<models.size();i++){
        Matrix4 overAllMatrix = getIdentityMatrix();
        Matrix4 camTransformationMatrix = getCamTransformationMatrix(camera);
        Matrix4 orthMatrix =getOrthMatrix(camera);
        Matrix4 p2oMatrix =getp2oMatrix(camera->far,camera->near);
        Matrix4 vpTransformMatrix_ = vpTransformMatrix(camera);
        Matrix4 transformationMatrix=getIdentityMatrix();
        for(int j=0;j<models[i]->numberOfTransformations;j++){
            if(models[i]->transformationTypes[j]=='s'){
                transformationMatrix = multiplyMatrixWithMatrix(getScalingMatrix(models[i]->transformationIds[j]), transformationMatrix);
            }
            else if(models[i]->transformationTypes[j]=='r'){
                transformationMatrix = multiplyMatrixWithMatrix(getRotationMatrix(models[i]->transformationIds[j]),transformationMatrix);
            }
            else if(models[i]->transformationTypes[j]=='t'){
                transformationMatrix = multiplyMatrixWithMatrix(getTranslationMatrix(models[i]->transformationIds[j]),transformationMatrix);
            }
        }
        overAllMatrix=multiplyMatrixWithMatrix(transformationMatrix,overAllMatrix);
        overAllMatrix=multiplyMatrixWithMatrix(camTransformationMatrix,overAllMatrix);
        if(projectionType==1)
            overAllMatrix=multiplyMatrixWithMatrix(p2oMatrix,overAllMatrix);
        overAllMatrix=multiplyMatrixWithMatrix(orthMatrix,overAllMatrix);
        vector<Vec4> transformedVertices;
        vector<Triangle> culledTriangle;
        for(int n = 0;n<vertices.size();n++){
            Vec4 point,transformedPoint;
            point.x=vertices[n]->x; point.y=vertices[n]->y; point.z=vertices[n]->z;
            point.t=1.0;
            transformedPoint = multiplyMatrixWithVec4(overAllMatrix,point);
            transformedPoint.colorId=vertices[n]->colorId;
            transformedVertices.push_back(transformedPoint);
        }
        if(cullingEnabled){

            for(int count =0;count<models[i]->numberOfTriangles;count++){
                Vec3 v_0,v_1,v_2,n,eye;
                v_0.x=transformedVertices[models[i]->triangles[count].vertexIds[0]-1].x;
                v_0.y=transformedVertices[models[i]->triangles[count].vertexIds[0]-1].y;
                v_0.z=transformedVertices[models[i]->triangles[count].vertexIds[0]-1].z;
                v_1.x=transformedVertices[models[i]->triangles[count].vertexIds[1]-1].x;
                v_1.y=transformedVertices[models[i]->triangles[count].vertexIds[1]-1].y;
                v_1.z=transformedVertices[models[i]->triangles[count].vertexIds[1]-1].z;
                v_2.x=transformedVertices[models[i]->triangles[count].vertexIds[2]-1].x;
                v_2.y=transformedVertices[models[i]->triangles[count].vertexIds[2]-1].y;
                v_2.z=transformedVertices[models[i]->triangles[count].vertexIds[2]-1].z;
                n=crossProductVec3(subtractVec3(v_2,v_0),subtractVec3(v_1,v_0));
                n=normalizeVec3(n);
                eye = subtractVec3(v_0,camera->v);
                eye=normalizeVec3(eye);
                if(dotProductVec3(n,eye)<0)
                    culledTriangle.push_back(models[i]->triangles[count]);
            }
        }
        else{
            culledTriangle = models[i]->triangles;
        }
        for(int tri=0;tri<vertices.size();tri++){
            Vec4 dividedPoint=persDivide(transformedVertices[tri]);
            Vec4 newPoint;
            newPoint=multiplyMatrixWithVec4(vpTransformMatrix_,dividedPoint);
            transformedVertices[tri].x=newPoint.x;
            transformedVertices[tri].y=newPoint.y;
            transformedVertices[tri].z=newPoint.z;
            transformedVertices[tri].t=newPoint.t;
        }
        if(models[i]->type==0){
            for(int j=0;j<culledTriangle.size();j++){
                Vec4 v_0;
                Vec4 v_1;
                Vec4 v_2;
                v_0 = transformedVertices[culledTriangle[j].vertexIds[0]-1];
                v_1 = transformedVertices[culledTriangle[j].vertexIds[1]-1];
                v_2 = transformedVertices[culledTriangle[j].vertexIds[2]-1];
                clipping(v_1,v_0,camera->horRes-1,0,camera->verRes-1,0);
                clipping(v_0,v_2,camera->horRes-1,0,camera->verRes-1,0);
                clipping(v_1,v_2,camera->horRes-1,0,camera->verRes-1,0);
            }
        }
        else{
            for(int j=0;j<culledTriangle.size();j++){
                Vec4 v_1,v_2,v_3;
                Color* c_1=new Color();
                Color* c_2=new Color();
                Color* c_3=new Color();
                v_1 = transformedVertices[culledTriangle[j].vertexIds[0]-1];
                v_2 = transformedVertices[culledTriangle[j].vertexIds[1]-1];
                v_3 = transformedVertices[culledTriangle[j].vertexIds[2]-1];
                c_1->r = colorsOfVertices[v_1.colorId-1]->r;
                c_1->g = colorsOfVertices[v_1.colorId-1]->g;
                c_1->b = colorsOfVertices[v_1.colorId-1]->b;
                c_2->r = colorsOfVertices[v_2.colorId-1]->r;
                c_2->g = colorsOfVertices[v_2.colorId-1]->g;
                c_2->b = colorsOfVertices[v_2.colorId-1]->b;
                c_3->r = colorsOfVertices[v_3.colorId-1]->r;
                c_3->g = colorsOfVertices[v_3.colorId-1]->g;
                c_3->b = colorsOfVertices[v_3.colorId-1]->b;
                triangleRasterization(v_1,v_2,v_3,*c_1,*c_2,*c_3);
            }
        }
    }
}

void Scene::triangleRasterization(Vec4 v_1,Vec4 v_2,Vec4 v_3,Color c_1,Color c_2,Color c_3){
    int x_1,x_2,x_3,y_1,y_2,y_3;
    Color c;
    double minx = std::numeric_limits<double>::max();
    double miny = std::numeric_limits<double>::max();
    double maxx = std::numeric_limits<double>::min();
    double maxy = std::numeric_limits<double>::min();
    double alpha,beta,gama;
    minx=min(min(min(minx,v_1.x),v_2.x),v_3.x);
    miny=min(min(min(miny,v_1.y),v_2.y),v_3.y);
    maxx=max(max(max(maxx,v_1.x),v_2.x),v_3.x);
    maxy=max(max(max(maxy,v_1.y),v_2.y),v_3.y);
    x_1=v_1.x;  x_2=v_2.x;  x_3=v_3.x;
    y_1=v_1.y;  y_2=v_2.y;  y_3=v_3.y;

    for(int y= int(miny);y<=int(maxy);y++){
        for(int x=int(minx);x<=int(maxx);x++){
            alpha = double((x*(y_2-y_3))+(y*(x_3-x_2))+(x_2*y_3)-(y_2*x_3))/
                    double((x_1*(y_2-y_3))+(y_1*(x_3-x_2))+(x_2*y_3)-(y_2*x_3));
            beta  = double((x*(y_3-y_1))+(y*(x_1-x_3))+(x_3*y_1)-(y_3*x_1)) /
                    double((x_2*(y_3-y_1))+(y_2*(x_1-x_3))+(x_3*y_1)-(y_3*x_1));
            gama = double((x*(y_1-y_2))+(y*(x_2-x_1))+(x_1*y_2)-(y_1*x_2)) /
                    double((x_3*(y_1-y_2))+(y_3*(x_2-x_1))+(x_1*y_2)-(y_1*x_2));

            if(alpha>=0 && beta>=0 && gama>=0){
                c.r=(alpha * c_1.r) + (beta * c_2.r) + (gama * c_3.r);
                c.g=(alpha * c_1.g) + (beta * c_2.g) + (gama * c_3.g);
                c.b=(alpha * c_1.b) + (beta * c_2.b) + (gama * c_3.b);
                draw(x, y, c);
            }
        }
    }

}

void Scene::lineRasterization(double x_0,double x_1,double y_0,double y_1, Color c_0,Color c_1){
    int x, y;
    bool reverse = abs(y_1-y_0) > abs(x_1-x_0);
    if(reverse) {
        swap(x_0, y_0);   swap(x_1, y_1);
    }
    if(x_0>x_1) {
        swap(x_0, x_1);   swap(y_0, y_1);   swap(c_0, c_1);
    }

    int dx = (int)x_1 - (int)x_0;
    int dy = abs((int)y_1 - (int)y_0);
    int d  = -dy + 0.5*(dx);

    Color c = c_0;
    double dc_r = (c_1.r - c_0.r) / (x_1-x_0);    double dc_g = (c_1.g - c_0.g) / (x_1-x_0);    double dc_b = (c_1.b - c_0.b) / (x_1-x_0);

    int ystep = (y_0 < y_1) ? 1 : -1;
    y = (int)y_0;
    for(x = (int)x_0; x <= (int)x_1; x++) {
        reverse ? draw(y, x, c):draw(x, y, c);
        if(d < 0) {
            y += ystep;
            d += -dy + dx;
        }
        else {
            d += -dy;
        }
        c.r += dc_r;    c.g += dc_g;    c.b += dc_b;
    }
}

void Scene::draw(double x,double y,Color c){
    image[x][y].r=c.r;  image[x][y].g=c.g;  image[x][y].b=c.b;
}

void Scene::clipping(Vec4 v_0,Vec4 v_1,int x_max,int x_min,int y_max,int y_min) {
    double t_e,t_l,d_x,d_y;
    d_x = v_1.x-v_0.x;
    d_y = v_1.y-v_0.y;
    t_e=0;t_l=1;
    if(visible(-d_x,v_0.x-x_min,&t_e,&t_l))
        if(visible(d_x,x_max-v_0.x,&t_e,&t_l))
            if(visible(-d_y,v_0.y-y_min,&t_e,&t_l))
                if(visible(d_y,y_max-v_0.y,&t_e,&t_l))
                {
                    if(t_l<1){
                        v_1.x=v_0.x+d_x*t_l;
                        v_1.y=v_0.y+d_y*t_l;
                    }
                    if(t_e>0){
                        v_0.x=v_0.x+d_x*t_e;
                        v_0.y=v_0.y+d_y*t_e;
                    }
                    lineRasterization(v_0.x,v_1.x,v_0.y,v_1.y,*colorsOfVertices[v_0.colorId-1],*colorsOfVertices[v_1.colorId-1]);
                }
}

bool Scene::visible(double den,double num,double* t_e,double* t_l){
    double t;
    if(den<0){
        t=num/den;
        if(t>*t_l)
            return false;
        if(t>*t_e)
            *t_e=t;
    }
    else if(den>0){
        t=num/den;
        if(t<*t_e)
            return false;
        if(t<*t_l)
            *t_l=t;
    }
    else if(num<0)
        return false;
    return true;
}

Matrix4 Scene::vpTransformMatrix(Camera *cam){
    Matrix4 result = getIdentityMatrix();
    result.val[0][0] = cam->horRes/2.0;   result.val[0][3] = (cam->horRes-1)/2.0;
    result.val[1][1] = cam->verRes/2.0;   result.val[1][3] = (cam->verRes-1)/2.0;
    result.val[2][2] = 0.5; result.val[2][3] = 0.5;
    return result;
}

Vec4 Scene::persDivide(Vec4 point){
    Vec4 result;
    result.x = point.x/point.t;   result.y = point.y/point.t;   result.z = point.z/point.t; result.t = 1.0;
    return result;
}

Matrix4 Scene::getp2oMatrix(double far, double near) {
    Matrix4 result = getIdentityMatrix();
    result.val[0][0]=near;  result.val[1][1]=near;
    result.val[2][2]=near+far;  result.val[2][3]=near*far;
    result.val[3][2]=-1;    result.val[3][3]=0;
    return result;

}

Matrix4 Scene::getOrthMatrix(Camera *cam) {
    Matrix4 result = getIdentityMatrix();
    double l,r,t,b,n,f;
    l = cam->left;  r = cam->right; t = cam->top;
    b = cam->bottom;    n = cam->near;  f = cam->far;
    result.val[0][0]=2.0/(r-l);   result.val[0][3]=(r+l)/((r-l)*(-1));
    result.val[1][1]=2.0/(t-b);   result.val[1][3]=(t+b)/((t-b)*(-1));
    result.val[2][2]=2.0/(n-f);   result.val[2][3]=(f+n)/((f-n)*(-1));
    return result;
}

Matrix4 Scene::getCamTransformationMatrix(Camera * cam) {
    Matrix4 result = getIdentityMatrix();
    double ux,uy,uz,vx,vy,vz,wx,wy,wz,ex,ey,ez;
    ux = cam->u.x;  uy = cam->u.y;  uz = cam->u.z;
    vx = cam->v.x;  vy = cam->v.y;  vz = cam->v.z;
    wx = cam->w.x;  wy = cam->w.y;  wz = cam->w.z;
    ex = cam->pos.x;    ey = cam->pos.y;    ez = cam->pos.z;
    result.val[0][0]=ux;    result.val[0][1]=uy;    result.val[0][2]=uz;    result.val[0][3]=(ux*ex+uy*ey+uz*ez)*-1;
    result.val[1][0]=vx;    result.val[1][1]=vy;    result.val[1][2]=vz;    result.val[1][3]=(vx*ex+vy*ey+vz*ez)*-1;
    result.val[2][0]=wx;    result.val[2][1]=wy;    result.val[2][2]=wz;    result.val[2][3]=(wx*ex+wy*ey+wz*ez)*-1;
    return result;
}

Matrix4 Scene::getScalingMatrix(int id) {
    Matrix4 result=getIdentityMatrix();
    double x =scalings[id-1]->sx;
    double y =scalings[id-1]->sy;
    double z =scalings[id-1]->sz;
    result.val[0][0]=x; result.val[1][1]=y; result.val[2][2]=z;
    return result;
}

Matrix4 Scene::getRotationMatrix(int id) {
    Vec3 u,v,w;
    Matrix4 result = getIdentityMatrix();
    Matrix4 _result = getIdentityMatrix();
    Matrix4 R_x = getIdentityMatrix();
    u.x = rotations[id-1]->ux;  u.y = rotations[id-1]->uy;  u.z = rotations[id-1]->uz;
    double alpha = rotations[id-1]->angle;
    u=normalizeVec3(u);

    if(ABS(u.x)>=ABS(u.y) && ABS(u.z)>=ABS(u.y)){
        v.x = u.z*-1;
        v.y = 0.0;
        v.z = u.x;
    }
    else if(ABS(u.y)>=ABS(u.x) && ABS(u.z)>=ABS(u.x)){
        v.y = u.z*-1;
        v.x = 0.0;
        v.z = u.y;
    }
    else if(ABS(u.y)>=ABS(u.z) && ABS(u.x)>=ABS(u.z)){
        v.x = u.y*-1;
        v.z = 0.0;
        v.y = u.x;
    }
    w = crossProductVec3(u,v);  v = normalizeVec3(v);   w = normalizeVec3(w);
    result.val[0][0]=u.x;   result.val[0][1]=u.y;   result.val[0][2]=u.z;
    result.val[1][0]=v.x;   result.val[1][1]=v.y;   result.val[1][2]=v.z;
    result.val[2][0]=w.x;   result.val[2][1]=w.y;   result.val[2][2]=w.z;

    _result.val[0][0]=u.x;   _result.val[0][1]=v.x;   _result.val[0][2]=w.x;
    _result.val[1][0]=u.y;   _result.val[1][1]=v.y;   _result.val[1][2]=w.y;
    _result.val[2][0]=u.z;   _result.val[2][1]=v.z;   _result.val[2][2]=w.z;

    R_x.val[1][1] = cos(alpha*M_PI/180);    R_x.val[1][2] = sin(alpha*M_PI/180)*-1;
    R_x.val[2][1] = sin(alpha*M_PI/180);    R_x.val[2][2] = cos(alpha*M_PI/180);

    return multiplyMatrixWithMatrix(_result,multiplyMatrixWithMatrix(R_x,result));
}

Matrix4 Scene::getTranslationMatrix(int id) {
    Matrix4 result=getIdentityMatrix();
    double x =translations[id-1]->tx;
    double y =translations[id-1]->ty;
    double z =translations[id-1]->tz;
    result.val[0][3] = x;   result.val[1][3] = y;   result.val[2][3] = z;
    return result;
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
    const char *str;
    XMLDocument xmlDoc;
    XMLElement *pElement;

    xmlDoc.LoadFile(xmlPath);

    XMLNode *pRoot = xmlDoc.FirstChild();

    // read background color
    pElement = pRoot->FirstChildElement("BackgroundColor");
    str = pElement->GetText();
    sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

    // read culling
    pElement = pRoot->FirstChildElement("Culling");
    if (pElement != NULL)
        pElement->QueryBoolText(&cullingEnabled);

    // read projection type
    pElement = pRoot->FirstChildElement("ProjectionType");
    if (pElement != NULL)
        pElement->QueryIntText(&projectionType);

    // read cameras
    pElement = pRoot->FirstChildElement("Cameras");
    XMLElement *pCamera = pElement->FirstChildElement("Camera");
    XMLElement *camElement;
    while (pCamera != NULL)
    {
        Camera *cam = new Camera();

        pCamera->QueryIntAttribute("id", &cam->cameraId);

        camElement = pCamera->FirstChildElement("Position");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

        camElement = pCamera->FirstChildElement("Gaze");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

        camElement = pCamera->FirstChildElement("Up");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

        cam->gaze = normalizeVec3(cam->gaze);
        cam->u = crossProductVec3(cam->gaze, cam->v);
        cam->u = normalizeVec3(cam->u);

        cam->w = inverseVec3(cam->gaze);
        cam->v = crossProductVec3(cam->u, cam->gaze);
        cam->v = normalizeVec3(cam->v);

        camElement = pCamera->FirstChildElement("ImagePlane");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
               &cam->left, &cam->right, &cam->bottom, &cam->top,
               &cam->near, &cam->far, &cam->horRes, &cam->verRes);

        camElement = pCamera->FirstChildElement("OutputName");
        str = camElement->GetText();
        cam->outputFileName = string(str);

        cameras.push_back(cam);

        pCamera = pCamera->NextSiblingElement("Camera");
    }

    // read vertices
    pElement = pRoot->FirstChildElement("Vertices");
    XMLElement *pVertex = pElement->FirstChildElement("Vertex");
    int vertexId = 1;

    while (pVertex != NULL)
    {
        Vec3 *vertex = new Vec3();
        Color *color = new Color();

        vertex->colorId = vertexId;

        str = pVertex->Attribute("position");
        sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

        str = pVertex->Attribute("color");
        sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

        vertices.push_back(vertex);
        colorsOfVertices.push_back(color);

        pVertex = pVertex->NextSiblingElement("Vertex");

        vertexId++;
    }

    // read translations
    pElement = pRoot->FirstChildElement("Translations");
    XMLElement *pTranslation = pElement->FirstChildElement("Translation");
    while (pTranslation != NULL)
    {
        Translation *translation = new Translation();

        pTranslation->QueryIntAttribute("id", &translation->translationId);

        str = pTranslation->Attribute("value");
        sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

        translations.push_back(translation);

        pTranslation = pTranslation->NextSiblingElement("Translation");
    }

    // read scalings
    pElement = pRoot->FirstChildElement("Scalings");
    XMLElement *pScaling = pElement->FirstChildElement("Scaling");
    while (pScaling != NULL)
    {
        Scaling *scaling = new Scaling();

        pScaling->QueryIntAttribute("id", &scaling->scalingId);
        str = pScaling->Attribute("value");
        sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

        scalings.push_back(scaling);

        pScaling = pScaling->NextSiblingElement("Scaling");
    }

    // read rotations
    pElement = pRoot->FirstChildElement("Rotations");
    XMLElement *pRotation = pElement->FirstChildElement("Rotation");
    while (pRotation != NULL)
    {
        Rotation *rotation = new Rotation();

        pRotation->QueryIntAttribute("id", &rotation->rotationId);
        str = pRotation->Attribute("value");
        sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

        rotations.push_back(rotation);

        pRotation = pRotation->NextSiblingElement("Rotation");
    }

    // read models
    pElement = pRoot->FirstChildElement("Models");

    XMLElement *pModel = pElement->FirstChildElement("Model");
    XMLElement *modelElement;
    while (pModel != NULL)
    {
        Model *model = new Model();

        pModel->QueryIntAttribute("id", &model->modelId);
        pModel->QueryIntAttribute("type", &model->type);

        // read model transformations
        XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
        XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

        pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

        while (pTransformation != NULL)
        {
            char transformationType;
            int transformationId;

            str = pTransformation->GetText();
            sscanf(str, "%c %d", &transformationType, &transformationId);

            model->transformationTypes.push_back(transformationType);
            model->transformationIds.push_back(transformationId);

            pTransformation = pTransformation->NextSiblingElement("Transformation");
        }

        // read model triangles
        XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
        XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

        pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

        while (pTriangle != NULL)
        {
            int v1, v2, v3;

            str = pTriangle->GetText();
            sscanf(str, "%d %d %d", &v1, &v2, &v3);

            model->triangles.push_back(Triangle(v1, v2, v3));

            pTriangle = pTriangle->NextSiblingElement("Triangle");
        }

        models.push_back(model);

        pModel = pModel->NextSiblingElement("Model");
    }
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
    if (this->image.empty())
    {
        for (int i = 0; i < camera->horRes; i++)
        {
            vector<Color> rowOfColors;

            for (int j = 0; j < camera->verRes; j++)
            {
                rowOfColors.push_back(this->backgroundColor);
            }

            this->image.push_back(rowOfColors);
        }
    }
        // if image is filled before, just change color rgb values with the background color
    else
    {
        for (int i = 0; i < camera->horRes; i++)
        {
            for (int j = 0; j < camera->verRes; j++)
            {
                this->image[i][j].r = this->backgroundColor.r;
                this->image[i][j].g = this->backgroundColor.g;
                this->image[i][j].b = this->backgroundColor.b;
            }
        }
    }
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
    if (value >= 255.0)
        return 255;
    if (value <= 0.0)
        return 0;
    return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
    ofstream fout;

    fout.open(camera->outputFileName.c_str());

    fout << "P3" << endl;
    fout << "# " << camera->outputFileName << endl;
    fout << camera->horRes << " " << camera->verRes << endl;
    fout << "255" << endl;

    for (int j = camera->verRes - 1; j >= 0; j--)
    {
        for (int i = 0; i < camera->horRes; i++)
        {
            fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
                 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
        }
        fout << endl;
    }
    fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
    string command;

    // call command on Ubuntu
    if (osType == 1)
    {
        command = "convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // call command on Windows
    else if (osType == 2)
    {
        command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // default action - don't do conversion
    else
    {
    }
}


