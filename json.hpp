#ifndef JSON_HPP
#define JSON_HPP

#include <vector>
#include <string>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QObject>
#include <iostream>
#include <QDebug>

using namespace std;

struct Coordinates{
	double x;
	double y;
	double z;
};

struct RGBValue{
	int b;
	int g;
	int r;
};

struct Resolution{
	double x;
	double y;
};
struct CanvasSize{
	int x;
	int y;
};
struct Ray{
	Coordinates start;
	double direction;
};

struct Camera{
	Coordinates center;
	double focus;
	Coordinates normal;
	Resolution resolution;
	CanvasSize size;
};
	
struct Light{
	double intensity;
	Coordinates location;
};

struct Object{
	Coordinates center;
	RGBValue color;
	double lambert;
	Coordinates normal;
	double radius;
	QString type;
	double t;
};

class Json{
public:
	Json();
	Json(Camera camera_in, vector <Light> light_in, vector <Object> object_in);
	Json(bool flag);
	~Json();
	static Json fromJSON(QString json);
	Camera getCamera();
	vector <Object> getObject();
	vector <Light> getLight();
	bool getError();
private:
	Camera camera;
	vector <Light> lights;
	vector <Object> objects;
	bool errorFlag;
};

#endif