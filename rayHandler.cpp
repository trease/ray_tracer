#include "rayHandler.hpp"
Rays::Rays(string filename_in){
	filename = QString::fromStdString(filename_in);	
	highColorValue = 255; //highest color value in the image, is added to only if value is above acceptable values
}
Rays::~Rays(){
}

void Rays::handler(vector<Object> O, vector<Light> L, Camera C, int cores){
	chrono::time_point<chrono::system_clock> startTime = chrono::system_clock::now();

	QRgb colorValue = qRgb(0,0,0);
	QImage pic(C.size.x, C.size.y, QImage::Format_RGB32);
	pic.fill(Qt::black);

	vector<thread> threads;

	for (int a = 0; a < cores; a++) {
		threads.push_back(thread(&Rays::tracer, this, (C.size.x / cores) * a, (((C.size.x / cores)*(a + 1))), O, L, C));
	}
	for (int b = 0; b < cores; b++) {
		threads[b].join();
	}

	chrono::time_point<chrono::system_clock> endTime = chrono::system_clock::now();

	double convert = highColorValue / 255;
	for (int j = 0; j < threadSafeQueue.size(); j++) {
		colorValue = qRgb(threadSafeQueue.front().r/ convert, threadSafeQueue.front().g/ convert, threadSafeQueue.front().b/ convert);
		pic.setPixel(threadSafeQueue.front().x, threadSafeQueue.front().y, colorValue);
		threadSafeQueue.wait_and_pop();
	}
	
	save(&pic);
	chrono::duration<double> elapsed_seconds = endTime - startTime;
	cout << "time to run: " << elapsed_seconds.count() << endl;
}

Object Rays::intersection(vector<Object> O, Camera C, Coordinates viewer) {
	double focus = -1 * C.focus;
	double x1 = (C.resolution.x*(viewer.x - (C.size.x / 2)));
	double y1 = (C.resolution.y*(viewer.y - (C.size.y / 2)));
	double dx = x1 - C.center.x;
	double dy = y1 - C.center.y;
	double dz = 0 - focus;
	double t = 1000000;
	double tTemp = 1000000;
	Object saveObj;
	for (unsigned int i = 0; i < O.size(); i++) {
		if (O[i].type == "sphere") {
			double a = dx*dx + dy*dy + dz*dz;
			double b = 2 * dx*(C.center.x - O[i].center.x) + 2 * dy*(C.center.y - O[i].center.y) + 2 * dz*(focus - O[i].center.z);
			double c = O[i].center.x*O[i].center.x + O[i].center.y*O[i].center.y + O[i].center.z*O[i].center.z + C.center.x*C.center.x + C.center.y*C.center.y + focus*focus + -2 * (O[i].center.x*C.center.x + O[i].center.y*C.center.y + O[i].center.z*focus) - O[i].radius*O[i].radius;
			double operand = b*b - 4 * a*c;
			if ((operand) > 0) {
				double t0 = (-b - sqrt(operand)) / (2 * a);
				double t1 = (-b + sqrt(operand)) / (2 * a);
				tTemp = t0;
			}
			else
				tTemp = 1000000;
		}
		else if (O[i].type == "plane") {
			double t0 = 10000000;
			Coordinates nv = norm(dx, dy, dz, false);
			double plane = nv.x*O[i].normal.x + nv.y*O[i].normal.y + nv.z*O[i].normal.z;
			if (fabs(plane) > 1e-6) {
				Coordinates nr = norm(O[i].center.x - C.center.x, O[i].center.y - C.center.y, O[i].center.z - focus, false);
				
				t0 = (nr.x*O[i].normal.x + nr.y*O[i].normal.y + nr.z*O[i].normal.z);
				t0 = t0 / plane;
			}
			if (t0 >= 1e-6)
				tTemp = t0;
			else
				tTemp = 1000000;
		}
		if (tTemp < t) {
			t = tTemp;
			saveObj = O[i];
		}
		tTemp = 1000000;
	}
	saveObj.t = t;
	return saveObj;
}

bool Rays::isIntersection(Object O, Coordinates A, Coordinates viewer){
	double dx = viewer.x-A.x;
	double dy = viewer.y-A.y;
	double dz = viewer.z-A.z;
		if(O.type == "sphere"){
			double a = dx*dx + dy*dy + dz*dz;
			double b = 2*dx*(A.x-O.center.x) + 2*dy*(A.y-O.center.y) + 2*dz*(A.z-O.center.z);
			double c = O.center.x*O.center.x + O.center.y*O.center.y + O.center.z*O.center.z + A.x*A.x + A.y*A.y + A.z*A.z + -2*(O.center.x*A.x + O.center.y*A.y + O.center.z*A.z) - O.radius*O.radius;
			double operand = b*b - 4*a*c;
			return ((operand > 1e-6));
		}
		if(O.type == "plane"){
			Coordinates nv = norm(dx, dy, dz, false);
			double plane = nv.x*O.normal.x + nv.y*O.normal.y + nv.z*O.normal.z;
			if (plane > 1e-6){
				Coordinates nr = norm(O.center.x - A.x, O.center.y - A.y, O.center.z - A.z, false);
				double t = (nr.x*O.normal.x + nr.y*O.normal.y + nr.z*O.normal.z);
				t = t/plane;
				return ((t > 1e-6));
			}
		}
		return false;
}

void Rays::tracer(int a, int b, vector<Object> O, vector<Light> L, Camera C) {
	double t1 = 0;
	double t2 = 0;
	for (int i = a; i < b; i++) {
		for (int j = 0; j < C.size.y; j++) {
			Coordinates pass;
			pass.x = i; pass.y = j; pass.z = 0;
			Object saveObj = intersection(O, C, pass);
			pixS pix;
			pix.r = 0; pix.g = 0; pix.b = 0;
			double scale = 0; double fscale = 0; double focus = -C.focus;
			double x1 = (C.resolution.x*(i - (C.size.x / 2)));
			double y1 = (C.resolution.y*(j - (C.size.y / 2)));
			double dx = x1 - C.center.x;
			double dy = y1 - C.center.y;
			double dz = 0 - focus;
			Coordinates ret = norm(C.center.x + saveObj.t*dx, C.center.y + saveObj.t*dy, focus + saveObj.t*dz, false);
			for (unsigned int n = 0; n < L.size(); n++) {
				Coordinates N;
				N.x = 0; N.y = 0; N.z = 0;
				Coordinates Lt;
				Coordinates light = norm(L[n].location.x, L[n].location.y, L[n].location.z, false);
				if (saveObj.type == "sphere") {
					N.x = ((ret.x - saveObj.center.x) / saveObj.radius);
					N.y = ((ret.y - saveObj.center.y) / saveObj.radius);
					N.z = ((ret.z - saveObj.center.z) / saveObj.radius);
				}
				else if (saveObj.type == "plane") {
					N.x = saveObj.normal.x;
					N.y = saveObj.normal.y;
					N.z = saveObj.normal.z;
				}
				Lt = norm((L[n].location.x - ret.x), (L[n].location.y - ret.y), (L[n].location.z - ret.z), true);
				scale = (N.x*Lt.x + N.y*Lt.y + N.z*Lt.z)*saveObj.lambert*L[n].intensity;
				if (scale < 1e-6 || saveObj.t == 1000000)
					scale = 0;
				for (unsigned int l = 0; l < O.size(); l++) {
					if ((O[l].center.x != saveObj.center.x || O[l].center.y != saveObj.center.y || O[l].center.z != saveObj.center.z) && isIntersection(O[l], ret, light) && saveObj.t != 1000000) {
						scale = 0;
					}
				}
				fscale = scale + fscale;
			}
			pix.x = i; pix.y = j; 
			pix.r = saveObj.color.r*fscale; 
			pix.g = saveObj.color.g*fscale; 
			pix.b = saveObj.color.b*fscale;
			if (pix.r < 0) 
				pix.r = 0;
			else if (pix.g < 0) 
				pix.g = 0;
			else if (pix.b < 0) 
				pix.b = 0;

			if (pix.r > highColorValue) 
				highColorValue = pix.r;
			else if (pix.g > highColorValue) 
				highColorValue = pix.g;
			else if (pix.b > highColorValue) 
				highColorValue = pix.b;
			threadSafeQueue.push(pix);
		}
	}
}

Coordinates Rays::norm(Coordinates value, bool op) {
	Coordinates ret; ret.x = 0; ret.y = 0; ret.z = 0;
	if (op) {
		double mag = sqrt(value.x*value.x + value.y*value.y + value.z*value.z);
		ret.x = value.x / mag;
		ret.y = value.y / mag;
		ret.z = value.z / mag;
	}
	return ret;
}

Coordinates Rays::norm(double x, double y, double z, bool op) {
	Coordinates ret; 
	ret.x = x; 
	ret.y = y; 
	ret.z = z;
	if (op) {
		Coordinates value;
		value.x = x;
		value.y = y;
		value.z = z;
		double mag = sqrt(value.x*value.x + value.y*value.y + value.z*value.z);
		ret.x = value.x / mag;
		ret.y = value.y / mag;
		ret.z = value.z / mag;
	}
	return ret;
}

void Rays::save(QImage* obj) {
	obj->save(filename, "PNG");
	obj = nullptr;
	delete obj;
}