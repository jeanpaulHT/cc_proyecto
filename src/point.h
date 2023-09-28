#ifndef PROYECTO_EDA_POINT_H
#define PROYECTO_EDA_POINT_H

#include <math.h>

struct Point{
    double x;
    double y;

    Point(){};
    Point(double x, double y) : x(x), y(y) {}
    double distance(double x2, double y2){return pow(pow(x-x2,2)+pow(y-y2,2),1/2);}
    double distance(Point p2){
        return pow(pow(x-p2.x,2)+pow(y-p2.y,2),0.5);
    }
};

/**
 * El área representa un rectangulo por dos puntos, siendo LL el inferior
 * izquierda (lower left) y UR el superior derecha (upper right).
 * */
struct MBR{
    Point LL;
    Point UR;

    explicit MBR(const Point& p):LL({p.x,p.y}),UR({p.x,p.y}){}

    MBR(const MBR& m):LL({m.LL.x,m.LL.y}),UR({m.UR.x,m.UR.y}){}

    MBR(const Point &p1, const Point &p2){
        LL.x = std::min(p1.x,p2.x);
        UR.x = std::max(p1.x,p2.x);
        LL.y = std::min(p1.y,p2.y);
        UR.y = std::max(p1.y,p2.y);
    }
    bool in_area(const Point &p){
        return p.x>=LL.x&&p.x<=UR.x&&p.y>=LL.y&&p.y<=UR.y;
    }
    bool in_collision(MBR &m){
        return m.in_area(LL) || m.in_area({UR.x,LL.y}) ||
               m.in_area({LL.x,UR.y}) || m.in_area({UR}) ||
               in_area(m.LL) || in_area({m.LL.x,m.LL.y}) ||
               in_area({m.UR.x,m.LL.y}) || in_area(m.UR);
    }
    double area(){return (UR.x-LL.x)*(UR.y-LL.y);}
    double distance(Point &p){
        if(p.x >= LL.x && p.x <= UR.x){
            if(p.y >= LL.y && p.y <= UR.y) return 0;
            else{
                if(p.y < LL.y) return LL.y-p.y;
                else return p.y-UR.y;
            }
        }else{
            if(p.y >= LL.y && p.y <= UR.y){
                if(p.y < LL.y) return LL.x-p.x;
                else return p.x-UR.x;
            }
            else{
                if(p.x > UR.x && p.y > UR.y) return p.distance(UR);
                if(p.x > UR.x && p.y < LL.y) return p.distance(UR.x,LL.y);
                if(p.x < LL.x && p.y > UR.y) return p.distance(LL.x,UR.y);
                return p.distance(LL);
            }
        }
    }

    void add(const Point &p){
        LL.x = std::min(LL.x,p.x);
        UR.x = std::max(UR.x,p.x);
        LL.y = std::min(LL.y,p.y);
        UR.y = std::max(UR.y,p.y);
    }

    void add(const MBR &m){
        LL.x = std::min(LL.x,m.LL.x);
        UR.x = std::max(UR.x,m.UR.x);
        LL.y = std::min(LL.y,m.LL.y);
        UR.y = std::max(UR.y,m.UR.y);
    }

    double insert_area(const Point &p){
        return (std::max(UR.x,p.x)-std::min(LL.x,p.x))*(std::max(UR.y,p.y)-std::min(LL.y,p.y));
    }

    double insert_area(const MBR &m){
        return (std::max(UR.x,m.UR.x)-std::min(LL.x,m.LL.x))*(std::max(UR.y,m.UR.y)-std::min(LL.y,m.LL.y));
    }

    double enlargement_area(const Point &p){
        return insert_area(p)-area();
    }

    double enlargement_area(const MBR &m){
        return insert_area(m)-area();
    }
};

#endif //PROYECTO_EDA_POINT_H