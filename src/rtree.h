#ifndef PROYECTO_EDA_RTREE_H
#define PROYECTO_EDA_RTREE_H

#include <float.h>
#include <vector>
#include "routeNode.h"

/** Key es el atributo clave con el cual se organizan los objetos en el RTree.
 *  Key tiene que ser un objeto Point definido en point.h
 * */
#define key start

const int N = 4;
const int n = 2;

/**RTree donde el resto de nodos son también RTrees.
 * Si un nodo se encuentra sin MBR y con contenido se considererará como nodo hoja.
 * El nodo root y los nodos hoja no cuenta con MBR.
 * El RTree solo utilizará el punto de partida del nodo.
 * */
template <typename T>
class RTree{
private:
    RTree *child[N], *parent;
    T content;
    MBR *mbr;

    /**Previo a realizar una inserción es necesario encontrar el MBR que se expanda menos
     * en el nivel. Si hubiese un empate es por el nodo de menor área. Cuando se llega a
     * un nodo hoja se retorna un puntero a este.
     * */
    RTree* minimumMBR(const Point &p){
        double min_enlarge = DBL_MAX, min_area = DBL_MAX;
        RTree *minChild = nullptr;
        for(auto &c:child){
            if(c == nullptr || c->child[0] == nullptr) break;
            if(c->mbr->enlargement_area(p) < min_enlarge){
                min_enlarge = c->mbr->enlargement_area(p);
                minChild = c;
            }else if(c->mbr->enlargement_area(p) == min_enlarge && c->mbr->area() < min_area){
                min_enlarge = c->mbr->enlargement_area(p);
                min_area = c->mbr->area() < min_area;
                minChild = c;
            }
        }
        if(minChild == nullptr) return this;
        if(minChild->child[0] == nullptr) return minChild;
        return minChild->minimumMBR(p);
    }

    void generateMBR(){
        if(child[0] == nullptr) return;
        if(child[0]->mbr == nullptr){
            mbr = new MBR(child[0]->content.key);
            for(int i = 1; i < N; i++){
                if(child[i] == nullptr) break;
                mbr->add(child[i]->content.key);
            }
        }else{
            mbr = new MBR(*(child[0]->mbr));
            for(int i = 1; i < N; i++){
                if(child[i] == nullptr) break;
                mbr->add(*(child[i]->mbr));
            }

        }
    }

    void insert_node(RTree *new_node){
        // Root insertion
        if(parent == nullptr){
            auto new_child = new RTree();
            for(int i = 0; i < N; i++){
                if(child[i] == nullptr) break;
                child[i]->parent = new_child;
                new_child->child[i] = child[i];
                child[i] = nullptr;
            }
            new_child->generateMBR();
            child[0] = new_child;
            child[0]->parent = this;
            child[1] = new_node;
            child[1]->parent = this;
            generateMBR();
            return;
        }

        // Leaf insertion
        auto old_parent = parent;
        for(int i = 0; i < N; i++){
            if(old_parent->child[i] == nullptr){
                old_parent->child[i] = new_node;
                new_node->parent = old_parent;
                return;
            }
        }

        RTree *aux[N+1], *new_parent = new RTree();
        new_parent->parent = old_parent->parent;
        for(int i = 0; i < N; i++){
            aux[i] = old_parent->child[i];
            old_parent->child[i] = nullptr;
        }
        aux[N] = new_node;

        double max_area = -1, area;
        for(int i = 0; i < N; i++){
            for(int j = i+1; j < N+1; j++){
                area = aux[i]->mbr->insert_area(*(aux[j]->mbr));
                if(max_area < area){
                    max_area = area;
                    old_parent->child[0] = aux[i];
                    new_parent->child[0] = aux[j];
                }
            }
        }
        old_parent->generateMBR();
        new_parent->generateMBR();

        int it1 = 1, it2 = 1;
        for(auto c:aux){
            if(c == new_parent->child[0] || c == old_parent->child[0]) continue;
            if((N-n) > it1 &&
               ((N-n) < it2 ||
               old_parent->mbr->enlargement_area(*c->mbr)<new_parent->mbr->enlargement_area(*c->mbr))){
                c->parent = old_parent;
                old_parent->child[it1] = c;
                old_parent->mbr->add(*(c->mbr));
                it1++;
            }else{
                c->parent = new_parent;
                new_parent->child[it2] = c;
                new_parent->mbr->add(*(c->mbr));
                it2++;
            }
        }
        old_parent->insert_node(new_parent);
    }

    explicit RTree(T _content):content(_content),mbr(nullptr),parent(nullptr){for(auto &c:child) c = nullptr;}

    void range_search_rec(MBR m, std::vector<T> *ans){
        if(mbr == nullptr){
            if(m.in_area(content.key)) ans->push_back(content);
            return;
        }
        if(!m.in_collision(*mbr)) return;
        for(auto &c:child){
            if(c == nullptr) break;
             c->range_search_rec(m,ans);
        }
    }

    void writeNode(RTree<T>& rtree){
      std::ofstream datos;
      datos.open("rtree.txt",  std::ios::app);
      if(datos.is_open()){
        if(rtree.mbr == nullptr){
          datos<<"H"<<rtree.content.start.x<<","<<rtree.content.start.y<<","<<rtree.content.finish.x<<","<<rtree.content.finish.y<<"\n";
          datos.close();
          return;
        }else{
          datos<<"M"<<rtree.mbr->LL.x<<","<<rtree.mbr->LL.y<<","<<rtree.mbr->UR.x<<","<<rtree.mbr->UR.y<<"\n";
          datos.close();
          return;
        }
        
      }
    }

    void writeTree(RTree<T>& rtree){
      if(mbr == nullptr){
        writeNode(rtree);
        return;
      }
      writeNode(rtree);
      for(auto &c:rtree.child){
        if(c == nullptr){
          std::ofstream datos;
          datos.open("rtree.txt",  std::ios::app);
          datos<<")"<<"\n";
          datos.close();
        }else{
          c->writeTree(*c);
        }
      }
    }

    RTree<RouteNode> readNode(std::string & line){
      std::string type = line.substr(0,1);
      line.erase(0,1);
      double x = stod(line.substr(0,line.find(",")));
      line.erase(0, line.find(",") + 1);
      double y = stod(line.substr(0,line.find(",")));
      line.erase(0, line.find(",") + 1);
      Point p1(x,y);
      x = stod(line.substr(0,line.find(",")));
      line.erase(0, line.find(",") + 1);
      y = stod(line.substr(0,line.find(",")));
      Point p2(x,y);
      RTree<RouteNode>* rtree = new RTree<RouteNode>;
      if(type == "H"){
        RouteNode* r = new RouteNode(p1,p2);
        rtree->updateContent(*r);
      }else{
        MBR *m = new MBR(p1,p2);
        rtree->updateMBR(*m);
      }
      return *rtree;
    }

    int readTree(RTree<T>& rtree, std::ifstream &datos, RTree<T>* parent){
      std::string line;
      getline(datos, line);
      if(line == ")") return 0;
      rtree = readNode(line);
      rtree.updateParent(parent);
      if(rtree.isLeaf()){
        return 1;
      }else{
        for(int i = 0; i < N; i++){
          RTree<RouteNode>* child = new RTree<RouteNode>;
          if(readTree(*child, datos, &rtree) != 0){
            rtree.updateChild(*child,i);
          }
        }
      }
      return 2;
    }

public:
    RTree():mbr(nullptr),parent(nullptr){for(auto &c:child) c = nullptr;}
    /**Pasos para el insert:
     * 1. Encontrar el MBR mínimo (Ver minimumMBR) y expandir el MBR de los nodos previos.
     * 2. Si hay espacio en el nodo hoja, se inserta el nuevo elemento y se finaliza. Caso
     *    contrario se procede a realizar un split (Linear split).
     * 3. Se toman todos los nodos hijos y el nuevo elemento y se colocan en un arreglo.
     * 4. De ese arreglo se toman los 2 nodos con mayor distancia entre ellos y se colocan en
     *    el nodo previo y en un nodo nuevo.
     * 5. Insertar el resto de hijos en los nodos dependediendo de cuál hace crecer el MBR menos.
     *    En el caso hubiera un empate seria por menor distancia.
     * 6. Insertar el nuevo nodo en el padre del nodo separado.
     * 7. En el caso el padre se llene realizar un split.
     * */
    void insert(T node){
        // 1.
        RTree *target = minimumMBR(node.key);
        RTree *target_it = target;

        while(target_it != nullptr){
            if(target->mbr == nullptr){
                target->mbr = new MBR(node.key);
                continue;
            }
            target_it->mbr->add(node.key);
            target_it = target_it->parent;
        }

        // 2.
        for(int i = 0; i < N; i++){
            if(target->child[i] == nullptr){
                auto new_child = new RTree(node);
                target->child[i] = new_child;
                new_child->parent = target;
                return;
            }
        }
        // 3.
        auto new_node = new RTree();
        RTree *aux[N+1];
        for(int i = 0; i < N; i++){
            aux[i] = target->child[i];
            target->child[i] = nullptr;
        }
        aux[N] = new RTree(node);
        // 4.
        double max_dist = -1, dist;
        for(int i = 0; i < N; i++){
            for(int j = i+1; j < N+1; j++){
                dist = aux[i]->content.key.distance(aux[j]->content.key);
                if(max_dist < dist){
                    max_dist = dist;
                    target->child[0] = aux[i];
                    new_node->child[0] = aux[j];
                }
            }
        }
        target->generateMBR();
        new_node->generateMBR();
        // 5.
        int it1 = 1, it2 = 1;
        for(auto c:aux){
            if(c==target->child[0] || c==new_node->child[0]) continue;
            if((N-n) > it1 &&
            ((N-n) < it2 ||
            target->mbr->enlargement_area(c->content.key) < new_node->mbr->enlargement_area(c->content.key) ||
            (target->mbr->enlargement_area(c->content.key) == new_node->mbr->enlargement_area(c->content.key) &&
            target->mbr->distance(c->content.key) < new_node->mbr->distance(c->content.key)))){
                target->child[it1] = c;
                c->parent = target;
                target->mbr->add(c->content.key);
                it1++;
            }else{
                new_node->child[it2] = c;
                c->parent = new_node;
                new_node->mbr->add(c->content.key);
                it2++;
            }
        }

        target->insert_node(new_node);
    };


    std::vector<T> range_search(const MBR &m){
        std::vector<T> ans;
        range_search_rec(m,&ans);
        return ans;
    };

    void writeRTree(RTree<RouteNode>& rtree){
      writeTree(rtree);
    }
    void readRTree(RTree<RouteNode>& rtree, std::ifstream &datos){
      readTree(rtree, datos, nullptr);
    }
    void updateMBR(MBR &m){
      mbr = &m;
    }
    void updateContent(T c){
      content = c;
    }
    void updateParent(RTree * p){
      parent = p;
    }
    void updateChild(RTree<T>& r, int n){
      if(n>=0 && n<=3){
        child[n] = &r;
      }
    }
    bool isLeaf(){
      if(mbr == nullptr) return true;
      return false;
    }
};

#endif //PROYECTO_EDA_RTREE_H