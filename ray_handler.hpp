#ifndef RAY_HPP
#define RAY_HPP

#include "json.hpp"
#include "threadsafequeue.hpp"

#include <QImage>
#include <QString>
#include <QColor>
#include <string>
#include <cmath>
#include <queue>

#include <thread>
#include <future>

using namespace std;

struct pixS{
	int x;
	int y;
	int r;
	int g;
	int b;
};

class Rays{
public:
	Rays(string filename_in);
	~Rays();
	Object intersection(vector<Object> O, Camera C, cord viewer);
	bool isIntersection(Object O, cord A, cord viewer);
	void handler(vector<Object> O, vector<Light> L, Camera C,int cores);
	void save(QImage* obj);
	void tracer(int i, int j, vector<Object> O, vector<Light> L, Camera C);
	cord norm(cord value, bool op);
	cord norm(double x, double y, double z, bool op);
private:
	QString filename;
	QImage* obj;
	vector<pixS> disp;
	atomic<double> high;
	ThreadSafeQueue<pixS> qu;
};


#endif