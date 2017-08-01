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

struct cord{
	double x;
	double y;
	double z;
};

struct col{
	int b;
	int g;
	int r;
};

struct res{
	double x;
	double y;
};
struct si{
	int x;
	int y;
};
struct Ray{
	cord start;
	double direction;
};

struct Camera{
	cord center;
	double focus;
	cord normal;
	res resolution;
	si size;
};
	
struct Light{
	double intensity;
	cord location;
};

struct Object{
	cord center;
	col color;
	double lambert;
	cord normal;
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