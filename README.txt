Project:
	Project is to create a ray tracer in c++ using threads. The idea is to trace a light vector 
	from a camera through each pixel in a screen into a scene. If the vector doesn’t hit an object
	it is set to black, if it does it an object a shadow ray from that point to each light in 
	the scene is cast to calculate the light contribution from that light.
vtray:
	Contains main function. Reads in json file, calls Json parser and ray_handler. The threads 
	are called from here. I avoided pointers for easy memory leak prevention.
Json:
	Contains a class called Json to parse a json file. It contains a series of structs to contain 
	objects, lights and camera. The Objects and Lights are stored in vectors. There are getters 
	for objects and lights. The things in the scene are represented by structs because they 
	weren’t complex enough to use a class. The error passing would be better if done by throwing
	but for easy of implantation and testing using catch I chose to pass the errors as a modified object.
Ray_handler:
	Contains a class called Rays that checks for intersections between objects and determines the
	colors of each object based on the intersections. There is a function called handler that for
	each pixel in the camera screen checks whether a ray from the focus through the pixel intersects 
	any of the objects in the scene. Then if there is an intersection it computes the intersection 
	point from the first ray, the light ray to each of the light sources in the scene and determines
	that light sources contribution to the color of the original pixel. The light from the light 
	source is added unless there is an object between it and the light source in which case the 
	pixel is in shadow. Each object except the object the one the shadow ray originated from must
	be checked. There is another function to save the final image. I decided to contain everything
	within the same class for simplicity and for easier threading. 
unittests.cpp:
	This file contains unittests for tests. There are specific tests for each part of ray_handler to find memory leaks.
Threads:
	The threading will be added for the final version. The number of threads will be determined by 
	the number available. The total number of pixels will be divided by the number of pixels to be 
	processed to determine how many pixels each thread will process. The camera frame will then be 
	divided up between each of the threads. Each thread will be divided up and have to complete its
	own set of calculations and return its set of pixels to the vector or thread safe queue. From there
	after all the threads have joined again the normal process will continue again. I used timers 
	within handler to time the function.
