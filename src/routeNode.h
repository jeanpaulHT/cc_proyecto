
#ifndef PROYECTO_EDA_ROUTENODE_H
#define PROYECTO_EDA_ROUTENODE_H

#include "point.h"

/**
 * Toda la información relevante de una ruta es guardada aquí
 * */
struct RouteNode{
    Point start;
    Point finish;

    RouteNode()= default;
    RouteNode(const Point &start, const Point &finish) : start(start), finish(finish) {}
    RouteNode(double xs, double ys, double xf, double yf){
        start = Point(xs,ys);
        finish = Point(xf,yf);
    }
    bool operator ==(RouteNode &rn){
        return start.x == rn.start.x && start.y == rn.start.y &&
                finish.x == rn.finish.x && finish.y == rn.finish.y;
    }
};

#endif //PROYECTO_EDA_ROUTENODE_H