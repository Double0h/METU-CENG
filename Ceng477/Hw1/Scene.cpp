#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"

using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */

//x+tw_i;

Vector3f Mirror(Ray ray, ReturnVal r){
	Vector3f point_to_cam = (ray.origin -r.point).normalized();
	Vector3f cam_to_point = point_to_cam*(-1);
	return (cam_to_point + (r.normal*2)*((r.normal).dot(point_to_cam))).normalized();
}

Vector3f specular(Ray &ray, ReturnVal r, PointLight* lights){
	float R,G,B;
	Vector3f v ;
    //Wi + W0 / || Wi+W0 ||
    Vector3f point_to_camera = (ray.origin -r.point).normalized();
    Vector3f h = (point_to_camera + (lights->position-r.point).normalized()).normalized();
    float dot_product = h.dot(r.normal);
    
    //max(0,n.h)
    if(dot_product <= 0)
    {
        v = {0,0,0};
        
    }
    else{
        float phonged = pow(dot_product,pScene->materials[r.Matindex-1]->phongExp);

        R = pScene->materials[r.Matindex-1]->specularRef.x() * phonged * lights->computeLightContribution(r.point).x() ;
        G = pScene->materials[r.Matindex-1]->specularRef.y() * phonged * lights->computeLightContribution(r.point).y() ;
        B = pScene->materials[r.Matindex-1]->specularRef.z() * phonged * lights->computeLightContribution(r.point).z() ;
	v = {R,G,B};
        
    }
	return  v;

}

Vector3f diffuse(ReturnVal r, PointLight* lights){
	 float R,G,B;
    //max(0,W_i.n)
	Vector3f v ;
    float dot_product = (lights->position-r.point).normalized().dot(r.normal);
    if(dot_product <= 0)
    {
	v = {0,0,0};
        //return v;
    }
    else{
              
        R = pScene->materials[r.Matindex-1]->diffuseRef.x() * dot_product * lights->computeLightContribution(r.point).x() ;
        G = pScene->materials[r.Matindex-1]->diffuseRef.y() * dot_product * lights->computeLightContribution(r.point).y() ;
        B = pScene->materials[r.Matindex-1]->diffuseRef.z() * dot_product * lights->computeLightContribution(r.point).z() ;
	v = {R,G,B};}
	//return v; }
	return v;


}

Vector3f ambient(ReturnVal r){
		Vector3f p;
		float R,G,B;
    
   		R = pScene->materials[r.Matindex-1]->ambientRef.x() *  pScene->ambientLight.x() ;
    		G = pScene->materials[r.Matindex-1]->ambientRef.y() *  pScene->ambientLight.y() ;
    		B = pScene->materials[r.Matindex-1]->ambientRef.z() *  pScene->ambientLight.z() ;
		p = {R,G,B};
		return p;

	
}



void Scene::renderScene(void)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	Color color;
	Vector3f v;
	float tmin,tL;
	Shape * obj;
	ReturnVal r,r2,r3;
	
	int sizeofCam = pScene->cameras.size();
	for(int i=0;i<sizeofCam;i++){
		Camera *cam=pScene->cameras[i];
	
	Image *img = new Image(cam->imgPlane.nx,cam->imgPlane.ny);
	for(int i = 0; i < cam->imgPlane.ny; i++ ){
		for(int j = 0 ; j < cam->imgPlane.nx ; j++ ){
			Ray casted = cam->getPrimaryRay(j,i);
			tmin=numeric_limits<float>::infinity();
			obj =NULL;
			for(int k=0;k<pScene->objects.size();k++){
				r = pScene->objects[k]->intersect(casted);
				if(r.intersects == true){
					if(r.t<tmin){
						tmin=r.t;
						obj = pScene->objects[k];
						r3=r;
					}
				}
			}
			if(obj!=NULL){
				v=ambient(r3);
				for(int m =0;m<pScene->lights.size();m++){
					Ray s((r3.point.normalized() + (((lights[m]->position-r3.point).normalized())*(pScene->shadowRayEps))), (lights[m]->position-r3.point).normalized());
					
					for(int n=0;n<pScene->objects.size();n++){
						r2=pScene->objects[n]->intersect(s);
						
						tL=(pScene->lights[m]->position-s.origin).normalized().x()/s.direction.normalized().x();
						if(tL>r2.t)
							break;
					}
					
					v += diffuse(r3,pScene->lights[m]) + specular(casted,r3,pScene->lights[m]);

				}
			}
			else
				v=pScene->backgroundColor;
			if(v.x()>255)
				v.x()=255;
			if(v.y()>255)
				v.y()=255;
			if(v.z()>255)
				v.z()=255;
			color.red = v.x();
			color.grn = v.y();
			color.blu = v.z();
			img->setPixelValue(i,j,color);
		}
}

	img->saveImage(cam->imageName); 	
}


}

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.x(), &backgroundColor.y(), &backgroundColor.z());

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x(), &pos.y(), &pos.z());
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x(), &gaze.y(), &gaze.z());
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x(), &up.y(), &up.z());
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.x(), &materials[curr]->ambientRef.y(), &materials[curr]->ambientRef.z());
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.x(), &materials[curr]->diffuseRef.y(), &materials[curr]->diffuseRef.z());
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.x(), &materials[curr]->specularRef.y(), &materials[curr]->specularRef.z());
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.x(), &materials[curr]->mirrorRef.y(), &materials[curr]->mirrorRef.z());
		}
				else
		{
			materials[curr]->mirrorRef.x() = 0.0;
			materials[curr]->mirrorRef.y() = 0.0;
			materials[curr]->mirrorRef.z() = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x() = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y() = atof(str + cursor);
			else
				tmpPoint.z() = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.x(), &ambientLight.y(), &ambientLight.z());

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x(), &position.y(), &position.z());
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.x(), &intensity.y(), &intensity.z());

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

