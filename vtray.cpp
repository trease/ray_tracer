#include <iostream>
#include <vector>
#include <cstdlib>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <string>
#include <chrono> 

#include "json.hpp"
#include "ray_handler.hpp"

int main(int argc, char* argv[]){
	if(argc == 3){
		QFile file(argv[1]);
		file.open(QIODevice::ReadOnly | QIODevice::Text);

		QString out = file.readAll();
		Json newJson;
		try {
			newJson = Json::fromJSON(out);
		} catch (QJsonParseError error) {
			cerr << "Error in parsing" << endl;
			qDebug() << "Error " << error.errorString();
			exit(EXIT_FAILURE);
		}
		if (newJson.getError()){
			exit(EXIT_FAILURE);
		}
		string filename = argv[2];
		Rays R(filename);
		R.handler(newJson.getObject(), newJson.getLight(), newJson.getCamera(), 1);
		file.close();
	}
	else if (argc == 5) {
		//cout << "1 " << argv[1] << " 2 " << argv[2] << " 3 " << argv[3] << " 4 " << argv[4] << endl;
		QFile file(argv[3]);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QString out = file.readAll();
		Json newJson;
		try {
			newJson = Json::fromJSON(out);
		}
		catch (QJsonParseError error) {
			cerr << "Error in parsing" << endl;
			qDebug() << "Error " << error.errorString();
			exit(EXIT_FAILURE);
		}
		if (newJson.getError()) {
			exit(EXIT_FAILURE);
		}
		string filename = argv[4];
		Rays R(filename);
		int cores = 1;
		cores = stoi(argv[2]);
		chrono::time_point<chrono::system_clock> start, end;
		chrono::duration<double> elapsed_seconds;
		start = chrono::system_clock::now();
		R.handler(newJson.getObject(), newJson.getLight(), newJson.getCamera(), cores);
		end = chrono::system_clock::now();
		elapsed_seconds = end - start;
		cout << "time: " << elapsed_seconds.count() << endl;
		file.close();
	}
	else{
		cerr << "Error not enough cmd args" << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
