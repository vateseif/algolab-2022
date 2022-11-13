///1
#include <limits>
#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2      P;
typedef K::Segment_2    S;
typedef K::Ray_2        R;

std::vector<S> segs;

double floor_to_double(const K::FT& x){
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}


bool testcase(){
    int n; std::cin >> n;
    if (n==0) return true;

    long x, y, a, b;
    std::cin >> x >> y >> a >> b;
    const P start(x, y);
    const R ray = R(start, P(a, b));

    bool found_closest = false;
    P closest;
    K::FT min_d(std::numeric_limits<double>::max());

    segs.clear();
    for (int i=0; i<n; i++){
        long r, s, t, u;
        std::cin >> r >> s >> t >> u;
        const S seg(P(r, s), P(t, u));
        segs.push_back(seg);
    } 
    std::random_shuffle(segs.begin(),segs.end());
    
    for (S segment:segs){
        if (min_d <= K::FT(1)) break;
        if(min_d<CGAL::squared_distance(start,segment)) continue;
        
        if (CGAL::do_intersect(ray, segment)){
            if (!found_closest) found_closest = true;
            auto inter = CGAL::intersection(ray, segment);
            K::FT d;
            P interP;
            if (const P* op = boost::get<P>(&*inter)){
                d = CGAL::squared_distance(start, *op);
                interP = *op;    
            } else if (const S* os = boost::get<S>(&*inter)){
                K::FT ds = CGAL::squared_distance(start, os->source());
                K::FT dt = CGAL::squared_distance(start, os->target());
                if (ds < dt){
                  interP = os->source();
                  d = ds;
                }else{
                  interP = os->target();
                  d = dt;
                }
            }
            if (d < min_d){
                min_d = d;
                closest = interP;
            }
        }
    }
    if (!found_closest){
        std::cout << "no" << std::endl;
    }else{
        std::cout << floor_to_double(closest.x()) << " " << floor_to_double(closest.y()) << std::endl;
    }
    
    return false;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);
    while (true) if (testcase()) break;
    return 0;
}