CC = g++
CFLAGS = -Wall
# CFLAGS = -Wall -g

main: main.o color.o component.o object.o ray.o raytracer.o vector3.o
	$(CC) $(CFLAGS) -o main main.o color.o component.o object.o ray.o raytracer.o vector3.o

main.o: main.cpp vector3.h color.h ray.h component.h object.h raytracer.h
	$(CC) $(CFLAGS) -c main.cpp

color.o: color.cpp color.h
	$(CC) $(CFLAGS) -c color.cpp

component.o: component.cpp color.h vector3.h ray.h component.h
	$(CC) $(CFLAGS) -c component.cpp

object.o: object.cpp vector3.h ray.h color.h component.h object.h
	$(CC) $(CFLAGS) -c object.cpp

ray.o: ray.cpp vector3.h ray.h
	$(CC) $(CFLAGS) -c ray.cpp

raytracer.o: raytracer.cpp vector3.h color.h ray.h component.h object.h
	$(CC) $(CFLAGS) -c -pthread raytracer.cpp

vector3.o: vector3.cpp vector3.h
	$(CC) $(CFLAGS) -c vector3.cpp
