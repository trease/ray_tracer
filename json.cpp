#include "json.hpp"

#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

using namespace std;

Json::Json(){
	errorFlag = false;
}
Json::Json(bool flag){
	errorFlag = flag;
}
Json::Json(Camera camera_in, vector <Light> lights_in, vector <Object> objects_in){
	camera = camera_in;
	lights = lights_in;
	objects = objects_in;
	errorFlag = false;
}

Json::~Json() {
}

Json Json::fromJSON(QString json){
	QJsonParseError error; QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8(), &error);
	if (error.error != QJsonParseError::NoError) {
		throw error;
	} QJsonObject obj = jsonDoc.object(); 

	/////////////////////// CAMERA ///////////////////////////////
	Camera tempCamera;
	if (!obj.contains(QString("camera"))) {
		cerr << "Error in camera" << endl;
		return Json(true);
	} 
	QJsonValue one_toObj = obj.value(QString("camera")); 
	QJsonObject obj1 = one_toObj.toObject(); 
	QJsonValue cent_toObj = obj1.value(QString("center"));
	QJsonObject cent = cent_toObj.toObject(); 
	QJsonValue cent_x = cent["x"]; QJsonValue cent_y = cent["y"]; 
	QJsonValue cent_z = cent["z"]; tempCamera.center.x = cent_x.toDouble(); 
	tempCamera.center.y = cent_y.toDouble(); 
	tempCamera.center.z = cent_z.toDouble();
	QJsonValue focus = obj1.value(QString("focus")); 
	tempCamera.focus = focus.toDouble(); 
	QJsonValue norm_toObj = obj1.value(QString("normal")); 
	QJsonObject norm = norm_toObj.toObject();
	QJsonValue norm_x = norm["x"]; 
	QJsonValue norm_y = norm["y"]; 
	QJsonValue norm_z = norm["z"]; 
	tempCamera.normal.x = norm_x.toDouble(); 
	tempCamera.normal.y = norm_y.toDouble(); 
	tempCamera.normal.z = norm_z.toDouble();
	QJsonValue res = obj1.value(QString("resolution")); 
	QJsonArray resolution = res.toArray(); 
	QJsonValue resValue = resolution[0];
	if (resValue.toDouble() < 0) {
		cerr << "Error in resolution" << endl;
		return Json(true);
	} tempCamera.resolution.x = resValue.toDouble(); QJsonValue resValue1 = resolution[1];
	if (resValue1.toDouble() < 0) {
		cerr << "Error in resolution" << endl;
		return Json(true);
	} tempCamera.resolution.y = resValue1.toDouble(); 
	QJsonValue si = obj1.value(QString("size")); 
	QJsonArray size = si.toArray(); 
	QJsonValue sizeValue = size[0];
	if (sizeValue.toDouble() < 0) {
		cerr << "Error in size" << endl;
		return Json(true);
	}
	 tempCamera.size.x = sizeValue.toDouble(); 
	QJsonValue sizeValue1 = size[1];
	if (sizeValue1.toDouble() < 0) {
		cerr << "Error in size" << endl;
		return Json(true);
	} 
	tempCamera.size.y = sizeValue1.toDouble(); 

	/////////////////////////// LIGHTS ///////////////////////////
	Light tempLight;
	QJsonValue two_toObj = obj.value(QString("lights")); 
	QJsonArray lights = two_toObj.toArray(); 
	vector<Light> light_vector;
	for (QJsonValue lightValue : lights) {
		QJsonObject obj2 = lightValue.toObject(); 
		QJsonValue inte = obj2.value(QString("intensity"));
		if (inte.toDouble() < 0) {
			cerr << "Error in intensity" << endl;
			return Json(true);
		} tempLight.intensity = inte.toDouble(); 
		QJsonValue loc_toObj = obj2.value(QString("location")); 
		QJsonObject loc = loc_toObj.toObject(); QJsonValue loc_x = loc["x"]; QJsonValue loc_y = loc["y"];
		QJsonValue loc_z = loc["z"]; 
		cord temp; temp.x = loc_x.toDouble(); 
		temp.y = loc_y.toDouble(); 
		temp.z = loc_z.toDouble(); 
		tempLight.location = temp; 
		light_vector.push_back(tempLight);
	} 

	/////////////////////////// OBJECTS ///////////////////////////
	Object tempObject; 
	QJsonValue three_toObj = obj.value(QString("objects"));
	QJsonArray objects = three_toObj.toArray(); vector<Object> object_vector;
	for (QJsonValue objectValue : objects) {
		QJsonObject obj3 = objectValue.toObject(); 
		QJsonValue center_toObj = obj3.value(QString("center")); 
		QJsonObject center = center_toObj.toObject();
		QJsonValue center_x = center["x"]; 
		QJsonValue center_y = center["y"]; 
		QJsonValue center_z = center["z"];
		cord temp; temp.x = center_x.toDouble(); 
		temp.y = center_y.toDouble(); 
		temp.z = center_z.toDouble();
		tempObject.center = temp; 
		QJsonValue color_toObj = obj3.value(QString("color"));  
		QJsonObject color = color_toObj.toObject(); 
		QJsonValue color_r = color["r"]; 
		QJsonValue color_g = color["g"];
		 QJsonValue color_b = color["b"];
		if (color_r.toDouble() < 0 || color_r.toDouble() > 255 || color_g.toDouble() < 0 || color_g.toDouble() > 255 || color_b.toDouble() < 0 || color_b.toDouble() > 255) {
			cerr << "Error color value out of range" << endl;
			return Json(true);
		} 
		col temp1; 
		temp1.r = color_r.toDouble(); 
		temp1.b = color_b.toDouble(); 
		temp1.g = color_g.toDouble(); 
		tempObject.color = temp1; 
		QJsonValue lamb = obj3.value(QString("lambert"));
		if (lamb.toDouble() < 0 || lamb.toDouble() > 1) {
			cerr << "Error in lambert" << endl;
			return Json(true);
		} tempObject.lambert = lamb.toDouble();
		QJsonValue normal_toObj = obj3.value(QString("normal")); 
		QJsonObject normal = normal_toObj.toObject(); 
		QJsonValue normal_x = normal["x"]; 
		QJsonValue normal_y = normal["y"]; 
		QJsonValue normal_z = normal["z"];
		cord temp2; 
		temp2.x = normal_x.toDouble(); 
		temp2.y = normal_y.toDouble(); 
		temp2.z = normal_z.toDouble(); 
		tempObject.normal = temp2; 
		QJsonValue rad = obj3.value(QString("radius"));
		if (rad.toDouble() < 0) {
			cerr << "Error in radius" << endl;
			return Json(true);
		} tempObject.radius = rad.toDouble(); 
		QJsonValue ty = obj3.value(QString("type")); 
		tempObject.type = ty.toString(); 
		object_vector.push_back(tempObject);
	} 
	Json input(tempCamera, light_vector, object_vector); 
	return input;
}

vector<Object> Json::getObject(){
	return objects;
}

Camera Json::getCamera(){
	return camera;
}

vector<Light> Json::getLight(){
	return lights;
}

bool Json::getError(){
	return errorFlag;
}
