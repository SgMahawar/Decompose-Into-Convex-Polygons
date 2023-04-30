#include <iostream>
#include<bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <string>
using namespace std::chrono;

using namespace std;
/** @file */

/**
 * @brief This is the structure for a point in the plane .
 * It assumes vertices to be provided in clockwise direction.
 * 
 * @param x This is the x-coordinate of the point.
 * @param y This is the y coordinate of the point.
 */

struct Point {
    int x, y;int id;
};

// for implementation of DCEL data structure
struct HalfEdge;struct Face;
/**
 * @brief This is the struct of vertex.
 * It represents coordinates in an x-y plane.
 * 
 * @param x This is the x-coordinate
 * @param y This is the y coordinate
 * @param incident_edge A vector of all the incident edges on the vertex.
 */
struct Vertex {

    int x;
    int y;
    int id;
    vector<HalfEdge*> incident_edge;
};
/**
 * @brief This is a struct representing a half-edge in the DCEL. 
 * A half-edge is a directed edge that forms a boundary between two regions in a planar subdivision. 
 * @param twin Each half-edge is uniquely identified by its twin half edge which is used to navigate the edges of the subdivision.
 * @param next Each half-edge is uniquely identified by its next half edge which is used to navigate the edges of the subdivision.
 * @param prev Each half-edge is uniquely identified by its previous half-edges which is used to navigate the edges of the subdivision. 
 * @param incident_vertex is a pointer to the vertex at the start of the half-edge.
 * @param incident_face is a pointer to the face that the half-edge borders.
 */

struct HalfEdge {

    HalfEdge* twin;
    HalfEdge* next;
    HalfEdge* prev;
    Vertex* incident_vertex;
    Face* incident_face;
};

/**
 * @brief This is a struct representing a face in the DCEL.
 * @param incident_edge vector contains pointers to the half-edges that form the boundary of the face.
 
 */
struct Face {
public:
    vector<HalfEdge*> incident_edge;
};

//functions for implementation

    vector<Vertex*> vertices;
    vector<HalfEdge*> half_edges;
    vector<Face*> faces;
    unordered_map<int,int> u;
    /**
 * @brief This is a function that takes a Point object P as input and returns a pointer to a Vertex object. 
 * It first checks if the id value of P has already been inserted in a map u.
 * @return the corresponding vertex from a vector vertices if it has id value of P, otherwise a pointer to newly created vertex object.
 */
    Vertex* add_vertex(Point P) {
        int f1=P.id;
        auto it=u.find(f1);
        if(it!=u.end())
        {
            int j1=it->second;return vertices[j1];
        }

        Vertex* v=new Vertex();
        v->x =P.x;
        v->y =P.y;
        v->id=P.id;
       // v.incident_edge = -1;
        vertices.push_back(v);int l=vertices.size()-1;
        u.insert({f1,l});
        return v;
    }
/**
 * @brief This is a function for adding face to the DCEL that is bounded by the given half-edge incident_edge. 
 * It creates a new Face struct, adds the incident_edge to its incident_edge vector, adds the Face struct to the faces vector.
 * @return A pointer to the new Face.
 */
    Face* add_face(HalfEdge* incident_edge) {
        Face *f=new Face();
        f->incident_edge.push_back(incident_edge);
        faces.push_back(f);
        return f;
    }
/**
 * @brief This is a  function fthat creates and returns a new HalfEdge object. 
 * @param twin A pointer to the twin HalfEdge object.
 * @param prev A pointer to the previous HalfEdge object.
 * @param incident_vertex A pointer to the incident Vertex object.
 * @return A pointer to the newly created HalfEdge object.
 */
    HalfEdge* add_half_edge(HalfEdge* twin, HalfEdge* prev, Vertex* incident_vertex) {
        HalfEdge* e=new HalfEdge(); 
        e->twin = twin;
        
        e->prev = prev;
        if(twin!=nullptr)
        twin->twin=e;
        if(prev!=nullptr)
        prev->next=e;
        e->incident_vertex = incident_vertex;
        e->incident_face =add_face(e);
        half_edges.push_back(e); incident_vertex->incident_edge.push_back(e);
        return e;
    }
/**
 * @brief Calculates the angle between two line segments defined by three points.
 * @param p1 The first point of the first line segment.
 * @param p2 The common point of the two line segments.
 * @param p3 The second point of the second line segment.
 * @return The angle between the two line segments in radians.
 */
double angle(Point p1, Point p2, Point p3) {
    double dx1 = p1.x - p2.x;
    double dy1 = p1.y - p2.y;
    double dx2 = p3.x - p2.x;
    double dy2 = p3.y - p2.y;
    double dot = dx1 * dx2 + dy1 * dy2;
    double cross = dx1 * dy2 - dy1 * dx2;
    return atan2(cross, dot);
}

/**
 * @brief Checks whether a vertex is a notch or not, based on the angle between two line segments.
 * @param p1 The first point of the first line segment.
 * @param p2 The common point of the two line segments.
 * @param p3 The second point of the second line segment.
 * @return true if the angle between the two line segments is greater than pi radians (180 degrees), false otherwise.
 */
bool isNotch(Point p1, Point p2, Point p3) {
    double a = angle(p1, p2, p3);
    return a > M_PI;
}

/**
@brief Finds all the notches in a given polygon represented by a vector of points.
@param points A vector of points representing the polygon.
@return An unordered set of integers representing the indices of the vertices that are notches.
*/
unordered_set<int> findNotches(vector<Point> points) {
    int n = points.size();
    unordered_set<int> notches;
    for (int i = 0; i < n; i++) {
        Point p1 = points[(i-1+n)%n];
        Point p2 = points[i];
        Point p3 = points[(i+1)%n];
        if (isNotch(p1, p2, p3)) {
            notches.insert(i);
        }
    }
    return notches;
}

/**
 * @brief Check if a point lies inside a polygon using the ray casting algorithm.
 * @param polygon Vector of Point objects representing the polygon vertices.
 * @param p Point object representing the point to check.
 * @return True if the point is inside the polygon, false otherwise.
 */
bool isPointInsidePolygon( vector<Point>& polygon,  Point& p) {
    int n = polygon.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}
/**
 * @brief Check if the angle formed by three points is reflex or not.
 * 
 * @param p1 First Point object.
 * @param p2 Second Point object.
 * @param p3 Third Point object.
 * @return True if the angle is reflex, false otherwise.
 */
bool isReflex(Point p1, Point p2, Point p3) {
    int crossProduct = (p2.x - p1.x) * (p3.y - p2.y) - (p3.x - p2.x) * (p2.y - p1.y);
    return crossProduct > 0;
}
/**
 * @brief Calculate the coordinates of a rectangle with minimum area that can contain polygon.
 * 
 * @param polygon Vector of Point objects representing the polygon vertices.
 * @return Vector of Point objects representing the coordinates of the rectangle vertices.
 */
vector<Point> minARect(vector<Point>& polygon)
{
    int xmin=INT_MAX;int xmax=INT_MIN;int ymin=INT_MAX;int ymax=INT_MIN;

    for(int i=0;i<polygon.size();i++)
    {
        int i1=polygon[i].x;int j1=polygon[i].y;
        xmin=min(xmin,i1);xmax=max(xmax,i1);ymin=min(ymin,j1);ymax=max(ymax,j1);
    }
    
    vector<Point> res;Point p1;Point p2;Point p3;Point p4;p1.x=xmin;p1.y=ymin;p4.x=xmax;p4.y=ymin;p2.x=xmin;p2.y=ymax;p3.x=xmax;p3.y=ymax;
    res.push_back(p1);res.push_back(p2);res.push_back(p3);res.push_back(p4);

    return res;
}
/**
 * @brief Constructs a monotone polygon from the given polygon using the first type of monotone partitioning algorithm (MP1).
 * 
 * @param polygon The input polygon from which the monotone polygon has to be constructed.
 * @return A vector of vertices representing the monotone polygon.
 * This function takes in a DCEL polygon as input and constructs a monotone polygon from it using the MP1 algorithm.
 * If the input polygon has less than 4 vertices, the input polygon is returned as it is, since it is already a monotone polygon. 
 * Otherwise, the algorithm starts by considering the first two vertices of the input polygon and adds them to the output polygon. 
 * Then, it considers the next vertex of the input polygon and checks if it can be added to the output polygon while maintaining monotonicity. 
 * If yes, it is added to the output polygon and the process continues with the next vertex. 
 * If no, the algorithm stops and the vertices added so far are returned as a monotone polygon. 
 * If the output polygon has only two vertices, it means that the input polygon is a monotone polygon with only one chain. 
 * In this case, the output polygon is returned as it is. 
 * If the output polygon has more than two vertices and is not yet a monotone polygon, the function recursively partitions the remaining portion of the input polygon into monotone polygons until all subpolygons are monotone. 
 * Finally, the vertices of all monotone subpolygons are concatenated to form the vertices of the final monotone polygon which is returned.
 */
vector<vector<Point>> algoImp(vector<Point>& polygon)
{
   
    int n=polygon.size();vector<vector<Point>> res;vector<vector<int>> res1;
    
    //implementation of part 3 in algo 1 as given in paper
    while(n>3)
    {
        //cout<<"Work";
        //impementation of part 3.1 and 3.2 in algo 1 as given in paper
        vector<Point> L;vector<int> index;
        Point v1=polygon[0];int ind=0;
        if(res1.size()>0)
        {
        
            ind=res1[res1.size()-1][res1[res1.size()-1].size()-1];v1=polygon[ind];
            
        }
        int i1=ind;int i2=(ind+1)%(polygon.size());
        Point v2=polygon[i2];L.push_back(v1);L.push_back(v2);index.push_back(i1);index.push_back(i2);
        int next=(index[index.size()-1]+1)%(polygon.size());

        //end of impementation of part 3.1 and 3.2 in algo 1 as given in paper

        //impementation of part 3.3 in algo 1 as given in paper
while((!isReflex(L[L.size()-2],L[L.size()-1],polygon[next])) &&(!isReflex(L[L.size()-1],polygon[next],L[0])) && (!isReflex(polygon[next],L[0],L[1])) && L.size()<n )
        {
            L.push_back(polygon[next]);index.push_back(next);next=(index[index.size()-1]+1)%(polygon.size());

        }
        //end of impementation of part 3.3 in algo 1 as given in paper
        //impementation of part 3.4 in algo 1 as given in paper
        unordered_set<int> u(index.begin(),index.end());
        if(L.size()!=n)
        {
            unordered_set<int> LPVS;
            //vector<int> notchIndex=findNotches(polygon);
            vector<Point> polygonNotch;
            for(int lp=0;lp<polygon.size();lp++)
            {
                if(u.find(lp)==u.end())
                polygonNotch.push_back(polygon[lp]);
            }
            LPVS=findNotches(polygonNotch);

            while(LPVS.size()>0 && L.size()>2)
            {
                vector<Point> miRect=minARect(L);vector<int> rem;
                for(auto x:LPVS)
                {
                        bool b=isPointInsidePolygon(miRect,polygon[x]);
                        if(b==false)
                        rem.push_back(x);
                }
                for(int lp1=0;lp1<rem.size();lp1++)
                LPVS.erase(rem[lp1]);

                vector<int> rem1;

                 for(auto x:LPVS)
                {
                        bool b=isPointInsidePolygon(L,polygon[x]);
                        if(b==false)
                        rem1.push_back(x);
                }
                 for(int lp1=0;lp1<rem1.size();lp1++)
                LPVS.erase(rem1[lp1]);

                if(LPVS.size()>0 && L.size()>0)
                {L.pop_back();u.erase(index[index.size()-1]);
                index.pop_back();}

            }
            
        }
//end of impementation of part 3.4 in algo 1 as given in paper
// impementation of part 3.5 in algo 1 as given in paper
        if(L.size()>2)
        {
            //adding the diagnol to DCEL
            Vertex* diagV1=add_vertex(L[0]);Vertex* diagV2=add_vertex(L[L.size()-1]);
            HalfEdge* diagEdge1=add_half_edge(nullptr,nullptr,diagV1);
            HalfEdge* diagEdge2=add_half_edge(diagEdge1,nullptr,diagV2);
            //end of adding diagnol to DCEL
            res.push_back(L);res1.push_back(index);polygon.push_back(L[0]);n=n-index.size()+2;

        }
      //end of impementation of part 3.5 in algo 1 as given in paper  
    }
//end of implementation of part 3 in algo 1 as given in paper
if(n==3)
{
    
    int lenP=polygon.size();
    vector<Point> L;L.push_back(polygon[lenP-3]);L.push_back(polygon[lenP-2]);L.push_back(polygon[lenP-1]);
    res.push_back(L);
}
return res;
}

/**

@brief This function reads a set of points from a file, and applies an algorithm

to find all convex polygons that can be formed from those points. The function

outputs the time taken by the algorithm and the set of convex polygons found.

@return An integer representing the exit status of the function.
*/
int main()
{
vector<Point> polygon;
int n=0;
int i=0;
std::string line;
    std::ifstream myfile("input.txt"); // replace example.txt with your file name

    if (myfile.is_open()) { 
        while (std::getline(myfile, line)) {    
           Point p1;p1.id=i;i++;
           int fi=line.find(',');
           string sl1=line.substr(0,fi);string sl2=line.substr(fi+1);
           p1.x=stoi(sl1);p1.y=stoi(sl2);
           
           polygon.push_back(p1);
        }
        myfile.close(); 
    }
    else {
        std::cout << "Unable to open file";
    }

vector<HalfEdge*> h1;
for(int i1=0;i1<polygon.size()-1;i1++)
{
    Point P1=polygon[i1];
    Vertex* vcr=add_vertex(P1);HalfEdge* h2;
        if(h1.size()==0)
        {
             h2=add_half_edge(nullptr,nullptr,vcr);
        }
        else
        {
             h2=add_half_edge(nullptr,h1[h1.size()-1],vcr);
        }
        h1.push_back(h2);
}
 int co=h1.size()-1;vector<HalfEdge*> h3;
 for(int i1=polygon.size()-1;i1>0;--i1)
 {
    Point P1=polygon[i1];
    Vertex* vcr=add_vertex(P1);HalfEdge* h2;

         if(h3.size()==0)
        {
             h2=add_half_edge(h1[co],nullptr,vcr);
        }
        else
        {
             h2=add_half_edge(h1[co],h3[h3.size()-1],vcr);
        }
        h3.push_back(h2);co--;


 }
  Vertex* diagV1=add_vertex(polygon[0]);
  Vertex* diagV2=add_vertex(polygon[polygon.size()-1]);
           
HalfEdge* h2= add_half_edge(nullptr,h3[h3.size()-1],diagV1);
add_half_edge(h2,h1[h1.size()-1],diagV2);

auto start = high_resolution_clock::now();

vector<vector<Point>> res=algoImp(polygon);
auto stop = high_resolution_clock::now();
auto duration = duration_cast<microseconds>(stop - start);
 
    // cout << "Time taken by algorithm: "
    //      << duration.count() << " microseconds" << endl;
    cout<<"Runtime : "<<clock()<<" ms";

    std::ofstream outputFile("output.txt");
    outputFile<<"[\n";
    for (int i = 0; i < res.size(); i++)
    {
        // cout<<"([";
        outputFile<<"[";
        for (int i1 = 0; i1 < res[i].size(); i1++)
        {
            Point P1 = res[i][i1];
            // cout << "(" << P1.x << "," << P1.y<<")"<<",";
            outputFile<<"("<<P1.x<<","<<P1.y<<")";
            if(i1<res[i].size()-1) outputFile<<",";
        }
        // cout << "])"<<endl;
        outputFile<<"]";
        if(i<res.size()-1) outputFile<<",\n";
    }
    outputFile<<"\n]";
    outputFile.close();



}