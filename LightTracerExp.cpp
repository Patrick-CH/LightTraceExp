#include <iostream>
#include <algorithm>
#include <vector>
#include <GL/glut.h>
#include "light.h"
#include "sphere.h"
#include "vector3d.h"

const int MAX_RECURSION_DEPTH = 1;
const float VIEWPORT_X = 15;
const float VIEWPORT_Y = 11.25;
const float FOCAL_LENGTH = -2.5;
const int WINDOW_W = 800;
const int WINDOW_H = 600;
const Ray camera(Vector3d(0.0f, 0.0f, 3.0f), Vector3d(0.0f, 0.0f, -1.0f));
const Vector3d upperRightCorner(VIEWPORT_X / 2, VIEWPORT_Y / 2, FOCAL_LENGTH);
const Vector3d bottomLeftCorner(-1 * VIEWPORT_X / 2, -1 * VIEWPORT_Y / 2, FOCAL_LENGTH);

Light light;
Color ambientLight;

std::vector<Object*> objects;
GLubyte pixelData[WINDOW_H][WINDOW_W][3];

void ConstructScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	light = Light(Color(1.0f, 1.0f, 1.0f), Vector3d(3.0f, 15.0f, -0.5f));
	ambientLight = Color(0.5f, 0.5f, 0.5f);
	Sphere* sphere = new Sphere;
	if (sphere == NULL) {
		std::cerr << "Can't allocate memory for Sphere." << std::endl;
		exit(-1);
	}
	sphere->center_ = Vector3d(0.0f, -1.25f, 0.0f);
	sphere->radius_ = 1.0;
	sphere->material_.diffuseColor_ = Color(0.0f, 0.0f, 0.8f);
	sphere->material_.ambientColor_ = Color(0.0f, 0.0f, 0.8f);
	sphere->material_.specularColor_ = Color(0.2f, 0.2f, 0.8f);
	sphere->material_.alpha_ = 0.6;
	sphere->material_.refractIndex_ = 1.5;
	sphere->material_.specExponent_ = 200.0;
	objects.push_back(sphere);
}

Hit DetectSceneHit(Ray& ray)
{
	Hit ret, hit;
	ret.material_.color_ = Color(25.0f, 25.0f, 25.0f);
	ret.t_ = std::numeric_limits<float>::infinity();
	for (std::vector<Object*>::iterator iter = objects.begin(); iter != objects.end();
		iter++) {
		Sphere* sphere = (Sphere*)(*iter);
		sphere->intersect(ray, hit);
		if (hit.t_ < ret.t_) {
			ret.material_ = hit.material_;
			ret.N_ = hit.N_;
			ret.P_ = hit.P_;
			ret.t_ = hit.t_;
		}
	}
	return ret;
}

//Determines the color of the passed in ray
Hit RayCast(Ray& ray, int depth)
{
	Hit hit;
	//Returns background color if maximum recursion depth was hit
	if (depth == 0) {
		hit.material_.color_ = Color(0.1f, 0.1f, 0.1f);
		hit.t_ = std::numeric_limits<float>::infinity();
		return hit;
	}
	//Find object intersection
	Hit rayHit = DetectSceneHit(ray);
	if (rayHit.t_ != std::numeric_limits<float>::infinity()) {
		// 此处填写代码，对rayHit.material_.color_赋值
		
		Sphere* obj = (Sphere*) *objects.begin();

		// 参数设置
		float ks = 0.8, kd = 0.83, n = obj->material_.specExponent_;

		// 环境光
		float r, g, b;
		float ir = obj->material_.ambientColor_.r_, ig = obj->material_.ambientColor_.g_, ib = obj->material_.ambientColor_.b_;
		r = ambientLight.r_ * ir;
		g = ambientLight.g_ * ig;
		b = ambientLight.b_ * ib;
		
		// 漫反射
		// 𝐼𝑑=𝐾𝑑*𝐼𝑖*max(0,𝐿∙𝑁)
		Vector3d N = rayHit.N_; //法向量
		Vector3d L, R, V, tmp;
		N.normalize();
		L = light.P_ - rayHit.P_; //交点指向光源
		L.normalize();

		r += kd * ir * std::max(0.0f, L.dot(N));
		g += kd * ig * std::max(0.0f, L.dot(N));
		b += kd * ib * std::max(0.0f, L.dot(N));
		
		// 反射光
		//𝐼s=𝐾𝑠*𝐼𝑖*pow(max(0,𝑅∙𝑉),n)
		tmp = N * L.dot(N);
		R = Vector3d(0.0f, 0.0f, 0.0f) - L + tmp * float(2); // R = 2*N - L
		V = Vector3d(0.0f, 0.0f, 0.0f) - ray.directionVector_;
		V.normalize();

		r += ks * ir * std::pow(std::max(0.0f, R.dot(V)), n);
		g += ks * ig * std::pow(std::max(0.0f, R.dot(V)), n);
		b += ks * ib * std::pow(std::max(0.0f, R.dot(V)), n);

		rayHit.material_.color_ = Color(r, g, b);
		//填写代码结束
		return rayHit;
	}
	else {
		rayHit.material_.color_ = Color(0.1f, 0.1f, 0.1f);
		return rayHit;
	}
}

void RenderScene()
{
	//Iterate through all screen pixels
	for (int i = 0; i < WINDOW_W; i++) {
		for (int j = 0; j < WINDOW_H; j++) {
			//Set background color
			pixelData[j][i][0] = 25;
			pixelData[j][i][1] = 25;
			pixelData[j][i][2] = 25;
			//Construct viewing ray
			float x = bottomLeftCorner.x_ + (upperRightCorner.x_ - bottomLeftCorner.x_)
				* ((i + 0.5) / WINDOW_W);
			float y = bottomLeftCorner.y_ + (upperRightCorner.y_ - bottomLeftCorner.y_)
				* ((j + 0.5) / WINDOW_H);
			Vector3d point(x, y, upperRightCorner.z_);
			Ray cameraRay;
			cameraRay.origin_ = camera.origin_;
			cameraRay.directionVector_ = point - (Vector3d)camera.origin_;
			Hit hit = RayCast(cameraRay, MAX_RECURSION_DEPTH);
			//If an object was hit, set the color for that pixel appropriately
			pixelData[j][i][0] = std::min((int)(hit.material_.color_.r_ * 255), 255);
			pixelData[j][i][1] = std::min((int)(hit.material_.color_.g_ * 255), 255);
			pixelData[j][i][2] = std::min((int)(hit.material_.color_.b_ * 255), 255);
		}
	}
	glDrawPixels(WINDOW_W, WINDOW_H, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
}


void DisplayFunc(void)
{
	ConstructScene();
	RenderScene();
	glFlush();
}


void MouseFunc(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			exit(-1);
		break;
	default:
		break;
	}
}


int main(int argc, char* argv[])
{
	std::cout << "Ray Casting" << std::endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ray Casting - 3D scene");
	glutDisplayFunc(DisplayFunc);
	glutMouseFunc(MouseFunc); // register myMouse function
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f); // background is light gray
	glutMainLoop();
	for (unsigned int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	return 0;
}

