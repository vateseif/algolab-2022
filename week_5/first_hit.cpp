///1
#include <iostream>
#include <vector>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt KS;
typedef K::Point_2      P;
typedef K::Segment_2    S;
typedef K::Ray_2        R;


double floor_to_double(const KS::FT& x){
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}


bool testcase(){
    int n; std::cin >> n;
    if (n==0) return true;

    double x, y, a, b;
    std::cin >> x >> y >> a >> b;
    R ray = R(P(x, y), P(a, b));

    bool found_closest = false;
    S closest;
    K::FT min_d;
    for (int i=0; i<n; i++){
        double r, s, t, u;
        std::cin >> r >> s >> t >> u;
        S segment = S(P(r, s), P(t, u));
        if (CGAL::do_intersect(ray, segment)){
            if (!found_closest) found_closest = !found_closest;
            K::FT d = CGAL::squared_distance(ray, segment);
            if (i==0){
                min_d = d;
                closest = segment;
            }else if (d < min_d){
                min_d = d;
                closest = segment;
            }
            trace(i);
            //trace(floor_to_double(d));
        }
    }
    if (!found_closest){
        std::cout << "no" << std::endl;
    }else{
        auto inter = CGAL::intersection(ray, closest);
        if (const P* op = boost::get<P>(&*inter)){
        std::cout << floor_to_double((*op).x()) << " " << floor_to_double((*op).y()) << std::endl;
        } else if (const S* os = boost::get<S>(&*inter)){
        //std::cout << os->source() << std::endl;
        std::cout << floor_to_double((os->source()).x()) << " " << floor_to_double((os->source()).y()) << std::endl;
        }
    }
    
    return false;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    while (true) if (testcase()) break;
    return 0;
}