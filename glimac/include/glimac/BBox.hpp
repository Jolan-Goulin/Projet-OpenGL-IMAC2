#pragma once

#include "glm.hpp"

namespace glimac {

struct BBox3f
{
    static const auto dim = 3;
    glm::vec3 lower, upper;

    BBox3f           ( )                   { }
    BBox3f           ( const BBox3f& other ) { lower = other.lower; upper = other.upper; }
    BBox3f& operator=( const BBox3f& other ) { lower = other.lower; upper = other.upper; return *this; }

    BBox3f ( const glm::vec3& v                 ) : lower(v),   upper(v) {}
    BBox3f ( const glm::vec3& lower, const glm::vec3& upper ) : lower(lower), upper(upper) {}

    void grow(const glm::vec3   & other) { lower = glm::min(lower,other    ); upper = glm::max(upper,other    ); }

    bool empty() const { for (auto i = 0u; i < dim; i++) if (lower[i] > upper[i]) return true; return false; }

    glm::vec3 size() const { return upper - lower; }
};

/*! computes the center of the box */
inline const glm::vec3 center (const BBox3f& box) { return .5f * (box.lower + box.upper); }
inline const glm::vec3 center2(const BBox3f& box) { return box.lower + box.upper; }

/*! computes the size of the box */
inline const glm::vec3 size(const BBox3f& box) { return box.size(); }

/*! merges bounding boxes and points */
inline const BBox3f merge( const BBox3f& a, const       glm::vec3& b ) { return BBox3f(glm::min(a.lower, b    ), glm::max(a.upper, b    )); }
inline const BBox3f merge( const       glm::vec3& a, const BBox3f& b ) { return BBox3f(glm::min(a    , b.lower), glm::max(a    , b.upper)); }
inline const BBox3f merge( const BBox3f& a, const BBox3f& b ) { return BBox3f(glm::min(a.lower, b.lower), glm::max(a.upper, b.upper)); }
inline const BBox3f merge( const BBox3f& a, const BBox3f& b, const BBox3f& c ) { return merge(a,merge(b,c)); }
inline const BBox3f& operator+=( BBox3f& a, const BBox3f& b ) { return a = merge(a,b); }
inline const BBox3f& operator+=( BBox3f& a, const       glm::vec3& b ) { return a = merge(a,b); }

/*! Merges four boxes. */
inline BBox3f merge(const BBox3f& a, const BBox3f& b, const BBox3f& c, const BBox3f& d) {
  return merge(merge(a,b),merge(c,d));
}

/*! Merges eight boxes. */
inline BBox3f merge(const BBox3f& a, const BBox3f& b, const BBox3f& c, const BBox3f& d,
                                                 const BBox3f& e, const BBox3f& f, const BBox3f& g, const BBox3f& h) {
  return merge(merge(a,b,c,d),merge(e,f,g,h));
}

/*! Comparison Operators */
inline bool operator==( const BBox3f& a, const BBox3f& b ) { return a.lower == b.lower && a.upper == b.upper; }
inline bool operator!=( const BBox3f& a, const BBox3f& b ) { return a.lower != b.lower || a.upper != b.upper; }

/*! scaling */
inline BBox3f operator *( const float& a, const BBox3f& b ) { return BBox3f(a*b.lower,a*b.upper); }

/*! intersect bounding boxes */
inline const BBox3f intersect( const BBox3f& a, const BBox3f& b ) { return BBox3f(glm::max(a.lower, b.lower), glm::min(a.upper, b.upper)); }
inline const BBox3f intersect( const BBox3f& a, const BBox3f& b, const BBox3f& c ) { return intersect(a,intersect(b,c)); }


/*! output operator */
inline std::ostream& operator<<(std::ostream& cout, const BBox3f& box) {
  return cout << "[" << box.lower << "; " << box.upper << "]";
}

}
