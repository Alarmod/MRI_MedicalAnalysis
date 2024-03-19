#ifndef CONCAVE_H
#define CONCAVE_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <cassert>
#include <unordered_map>
#include <cstdint>

#pragma warning(push, 0)
#include <flann/flann.hpp>
#pragma warning(pop)

//#define USE_OPENMP

#if defined USE_OPENMP
#if !defined _OPENMP
#pragma message("You've chosen to want OpenMP usage but have not made it a compilation option. Compile with /openmp")
#endif
#endif

using std::uint64_t;

struct ConcavePoint
{
  double x = 0.0;
  double y = 0.0;
  uint64_t id = 0;

  ConcavePoint() = default;

  ConcavePoint(double x, double y)
    : x(x)
    , y(y)
  {}
};

struct ConcavePointValue
{
  ConcavePoint point;
  double distance = 0.0;
  double angle = 0.0;
};

static const size_t stride = 24; // size in bytes of x, y, id

using ConcavePointVector = std::vector<ConcavePoint>;
using ConcavePointValueVector = std::vector<ConcavePointValue>;
using LineSegment = std::pair<ConcavePoint, ConcavePoint>;

// Floating point comparisons
auto EqualCO(double a, double b) -> bool;
auto ZeroCO(double a) -> bool;
auto LessThanCO(double a, double b) -> bool;
auto LessThanOrEqualCO(double a, double b) -> bool;
auto GreaterThanCO(double a, double b) -> bool;

// Algorithm-specific
auto NearestNeighboursflann(::flann::Index<::flann::L2<double>> &index, const ConcavePoint &p, size_t k)->ConcavePointValueVector;
auto ConcaveHull(ConcavePointVector &dataset, size_t k, bool iterate)->ConcavePointVector;
auto ConcaveHull(ConcavePointVector &dataset, size_t k, ConcavePointVector &hull) -> bool;
auto SortByAngle(ConcavePointValueVector &values, const ConcavePoint &p, double prevAngle)->ConcavePointVector;
auto AddConcavePoint(ConcavePointVector &points, const ConcavePoint &p) -> void;

// General maths
auto ConcavePointsEqual(const ConcavePoint &a, const ConcavePoint &b) -> bool;
auto Angle(const ConcavePoint &a, const ConcavePoint &b) -> double;
auto NormaliseAngle(double radians) -> double;
auto ConcavePointInPolygon(const ConcavePoint &p, const ConcavePointVector &list) -> bool;
auto Intersects(const LineSegment &a, const LineSegment &b) -> bool;

// ConcavePoint list utilities
auto FindMinYConcavePoint(const ConcavePointVector &points)->ConcavePoint;
auto RemoveDuplicates(ConcavePointVector &points) -> void;
auto IdentifyConcavePoints(ConcavePointVector &points) -> void;
auto RemoveHull(ConcavePointVector &points, const ConcavePointVector &hull)->ConcavePointVector::iterator;
auto MultipleConcavePointInPolygon(ConcavePointVector::iterator begin, ConcavePointVector::iterator end, const ConcavePointVector &hull) -> bool;

// Iteratively call the main algorithm with an increasing k until success
auto ConcaveHull(ConcavePointVector &dataset, size_t k, bool iterate) -> ConcavePointVector
{
  while (k < dataset.size())
  {
    ConcavePointVector hull;
    if (ConcaveHull(dataset, k, hull) || !iterate) return hull;

    k++;
  }

  return{};
}

// The main algorithm from the Moreira-Santos paper.
auto ConcaveHull(ConcavePointVector &pointList, size_t k, ConcavePointVector &hull) -> bool
{
  hull.clear();

  if (pointList.size() < 3)
  {
    return true;
  }
  if (pointList.size() == 3)
  {
    hull = pointList;
    return true;
  }

  // construct a randomized kd-tree index using 4 kd-trees
  // 2 columns, but stride = 24 bytes in width (x, y, ignoring id)
  ::flann::Matrix<double> matrix(&(pointList.front().x), pointList.size(), 2, stride);
  ::flann::Index<::flann::L2<double>> flannIndex(matrix, ::flann::KDTreeIndexParams(4));
  flannIndex.buildIndex();

  // Initialise hull with the min-y point
  ConcavePoint firstConcavePoint = FindMinYConcavePoint(pointList);
  AddConcavePoint(hull, firstConcavePoint);

  // Until the hull is of size > 3 we want to ignore the first point from nearest neighbour searches
  ConcavePoint currentConcavePoint = firstConcavePoint;
  flannIndex.removePoint(firstConcavePoint.id);

  double prevAngle = 0.0;
  int step = 1;

  // Iterate until we reach the start, or until there's no points left to process
  while ((!ConcavePointsEqual(currentConcavePoint, firstConcavePoint) || step == 1) && hull.size() != pointList.size())
  {
    if (step == 4)
    {
      // Put back the first point into the dataset and into the flann index
      firstConcavePoint.id = pointList.size();
      ::flann::Matrix<double> firstConcavePointMatrix(&firstConcavePoint.x, 1, 2, stride);
      flannIndex.addPoints(firstConcavePointMatrix);
    }

    ConcavePointValueVector kNearestNeighbours = NearestNeighboursflann(flannIndex, currentConcavePoint, k);
    ConcavePointVector cConcavePoints = SortByAngle(kNearestNeighbours, currentConcavePoint, prevAngle);

    bool its = true;
    size_t i = 0;

    while (its && i < cConcavePoints.size())
    {
      size_t lastConcavePoint = 0;
      if (ConcavePointsEqual(cConcavePoints[i], firstConcavePoint))
        lastConcavePoint = 1;

      size_t j = 2;
      its = false;

      while (!its && j < hull.size() - lastConcavePoint)
      {
        auto line1 = std::make_pair(hull[step - 1], cConcavePoints[i]);
        auto line2 = std::make_pair(hull[step - j - 1], hull[step - j]);
        its = Intersects(line1, line2);
        j++;
      }

      if (its)
        i++;
    }

    if (its)
      return false;

    currentConcavePoint = cConcavePoints[i];

    AddConcavePoint(hull, currentConcavePoint);

    prevAngle = Angle(hull[step], hull[step - 1]);

    flannIndex.removePoint(currentConcavePoint.id);

    step++;
  }

  // The original points less the points belonging to the hull need to be fully enclosed by the hull in order to return true.
  ConcavePointVector dataset = pointList;

  auto newEnd = RemoveHull(dataset, hull);
  bool allEnclosed = MultipleConcavePointInPolygon(begin(dataset), newEnd, hull);

  return allEnclosed;
}

// Compare a and b for equality
auto EqualCO(double a, double b) -> bool
{
  return fabs(a - b) <= DBL_EPSILON;
}

// Compare value to zero
auto ZeroCO(double a) -> bool
{
  return fabs(a) <= DBL_EPSILON;
}

// Compare for a < b
auto LessThanCO(double a, double b) -> bool
{
  return a < (b - DBL_EPSILON);
}

// Compare for a <= b
auto LessThanOrEqualCO(double a, double b) -> bool
{
  return a <= (b + DBL_EPSILON);
}

// Compare for a > b
auto GreaterThanCO(double a, double b) -> bool
{
  return a > (b + DBL_EPSILON);
}

// Compare whether two points have the same x and y
auto ConcavePointsEqual(const ConcavePoint &a, const ConcavePoint &b) -> bool
{
  return EqualCO(a.x, b.x) && EqualCO(a.y, b.y);
}

// Remove duplicates in a list of points
auto RemoveDuplicates(ConcavePointVector &points) -> void
{
  sort(begin(points), end(points), [](const ConcavePoint & a, const ConcavePoint & b)
  {
    if (EqualCO(a.x, b.x))
      return LessThanCO(a.y, b.y);
    else
      return LessThanCO(a.x, b.x);
  });

  auto newEnd = unique(begin(points), end(points), [](const ConcavePoint & a, const ConcavePoint & b)
  {
    return ConcavePointsEqual(a, b);
  });

  points.erase(newEnd, end(points));
}

// Uniquely id the points for binary searching
auto IdentifyConcavePoints(ConcavePointVector &points) -> void
{
  uint64_t id = 0;

  for (auto itr = begin(points); itr != end(points); ++itr, ++id)
  {
    itr->id = id;
  }
}

// Find the point having the smallest y-value
auto FindMinYConcavePoint(const ConcavePointVector &points) -> ConcavePoint
{
  assert(!points.empty());

  auto itr = min_element(begin(points), end(points), [](const ConcavePoint & a, const ConcavePoint & b)
  {
    if (EqualCO(a.y, b.y))
      return GreaterThanCO(a.x, b.x);
    else
      return LessThanCO(a.y, b.y);
  });

  return *itr;
}

// Lookup by ID and remove a point from a list of points
auto RemoveConcavePoint(ConcavePointVector &list, const ConcavePoint &p) -> void
{
  auto itr = std::lower_bound(begin(list), end(list), p, [](const ConcavePoint & a, const ConcavePoint & b)
  {
    return a.id < b.id;
  });

  assert(itr != end(list) && itr->id == p.id);

  if (itr != end(list))
    list.erase(itr);
}

// Add a point to a list of points
auto AddConcavePoint(ConcavePointVector &points, const ConcavePoint &p) -> void
{
  points.push_back(p);
}

// Return the k-nearest points in a list of points from the given point p (uses flann library).
auto NearestNeighboursflann(::flann::Index<::flann::L2<double>>& index, const ConcavePoint& p, size_t k) -> ConcavePointValueVector
{
  std::vector<int> vIndices(k);
  std::vector<double> vDists(k);
  double test[] = { p.x, p.y };

  ::flann::Matrix<double> query(test, 1, 2);
  ::flann::Matrix<int> mIndices(vIndices.data(), 1, static_cast<int>(vIndices.size()));
  ::flann::Matrix<double> mDists(vDists.data(), 1, static_cast<int>(vDists.size()));

  int count_ = index.knnSearch(query, mIndices, mDists, k, ::flann::SearchParams(128));
  size_t count = static_cast<size_t>(count_);

  ConcavePointValueVector result(count);

  for (size_t i = 0; i < count; ++i)
  {
    int id = vIndices[i];
    const double *point = index.getPoint(id);
    result[i].point.x = point[0];
    result[i].point.y = point[1];
    result[i].point.id = id;
    result[i].distance = vDists[i];
  }

  return result;
}

// Returns a list of points sorted in descending order of clockwise angle
auto SortByAngle(ConcavePointValueVector &values, const ConcavePoint &from, double prevAngle) -> ConcavePointVector
{
  for_each(begin(values), end(values), [from, prevAngle](ConcavePointValue & to)
  {
    to.angle = NormaliseAngle(Angle(from, to.point) - prevAngle);
  });

  sort(begin(values), end(values), [](const ConcavePointValue & a, const ConcavePointValue & b)
  {
    return GreaterThanCO(a.angle, b.angle);
  });

  ConcavePointVector angled(values.size());

  transform(begin(values), end(values), begin(angled), [](const ConcavePointValue & pv)
  {
    return pv.point;
  });

  return angled;
}

// Get the angle in radians measured clockwise from +'ve x-axis
auto Angle(const ConcavePoint &a, const ConcavePoint &b) -> double
{
  double angle = -atan2(b.y - a.y, b.x - a.x);

  return NormaliseAngle(angle);
}

// Return angle in range: 0 <= angle < 2PI
auto NormaliseAngle(double radians) -> double
{
  if (radians < 0.0)
    return radians + M_PI + M_PI;
  else
    return radians;
}

// Return the new logical end after removing points from dataset having ids belonging to hull
auto RemoveHull(ConcavePointVector &points, const ConcavePointVector &hull) -> ConcavePointVector::iterator
{
  std::vector<uint64_t> ids(hull.size());

  transform(begin(hull), end(hull), begin(ids), [](const ConcavePoint & p)
  {
    return p.id;
  });

  sort(begin(ids), end(ids));

  return remove_if(begin(points), end(points), [&ids](const ConcavePoint & p)
  {
    return binary_search(begin(ids), end(ids), p.id);
  });
}

// Uses OpenMP to determine whether a condition exists in the specified range of elements. https://msdn.microsoft.com/en-us/library/ff521445.aspx
template <class InIt, class Predicate>
bool omp_parallel_any_of(InIt first, InIt last, const Predicate &pr)
{
  typedef typename std::iterator_traits<InIt>::value_type item_type;

  // A flag that indicates that the condition exists.
  bool found = false;

#pragma omp parallel for
  for (int i = 0; i < static_cast<int>(last - first); ++i)
  {
    if (!found)
    {
      item_type &cur = *(first + i);

      // If the element satisfies the condition, set the flag to cancel the operation.
      if (pr(cur))
      {
        found = true;
      }
    }
  }

  return found;
}

// Check whether all points in a begin/end range are inside hull.
auto MultipleConcavePointInPolygon(ConcavePointVector::iterator begin, ConcavePointVector::iterator end, const ConcavePointVector &hull) -> bool
{
  auto test = [&hull](const ConcavePoint & p)
  {
    return !ConcavePointInPolygon(p, hull);
  };

  bool anyOutside = true;

#if defined USE_OPENMP

  anyOutside = omp_parallel_any_of(begin, end, test); // multi-threaded

#else

  anyOutside = std::any_of(begin, end, test); // single-threaded

#endif

  return !anyOutside;
}

// ConcavePoint-in-polygon test
auto ConcavePointInPolygon(const ConcavePoint &p, const ConcavePointVector &list) -> bool
{
  if (list.size() <= 2)
    return false;

  const double &x = p.x;
  const double &y = p.y;

  int inout = 0;
  auto v0 = list.begin();
  auto v1 = v0 + 1;

  while (v1 != list.end())
  {
    if ((LessThanOrEqualCO(v0->y, y) && LessThanCO(y, v1->y)) || (LessThanOrEqualCO(v1->y, y) && LessThanCO(y, v0->y)))
    {
      if (!ZeroCO(v1->y - v0->y))
      {
        double tdbl1 = (y - v0->y) / (v1->y - v0->y);
        double tdbl2 = v1->x - v0->x;

        if (LessThanCO(x, v0->x + (tdbl2 * tdbl1)))
          inout++;
      }
    }

    v0 = v1;
    v1++;
  }

  if (inout == 0)
    return false;
  else if (inout % 2 == 0)
    return false;
  else
    return true;
}

// Test whether two line segments intersect each other
auto Intersects(const LineSegment &a, const LineSegment &b) -> bool
{
  const double &ax1 = a.first.x;
  const double &ay1 = a.first.y;
  const double &ax2 = a.second.x;
  const double &ay2 = a.second.y;
  const double &bx1 = b.first.x;
  const double &by1 = b.first.y;
  const double &bx2 = b.second.x;
  const double &by2 = b.second.y;

  double a1 = ay2 - ay1;
  double b1 = ax1 - ax2;
  double c1 = a1 * ax1 + b1 * ay1;
  double a2 = by2 - by1;
  double b2 = bx1 - bx2;
  double c2 = a2 * bx1 + b2 * by1;
  double det = a1 * b2 - a2 * b1;

  if (ZeroCO(det))
  {
    return false;
  }
  else
  {
    double x = (b2 * c1 - b1 * c2) / det;
    double y = (a1 * c2 - a2 * c1) / det;

    bool on_both = true;
    on_both = on_both && LessThanOrEqualCO(std::min(ax1, ax2), x) && LessThanOrEqualCO(x, std::max(ax1, ax2));
    on_both = on_both && LessThanOrEqualCO(std::min(ay1, ay2), y) && LessThanOrEqualCO(y, std::max(ay1, ay2));
    on_both = on_both && LessThanOrEqualCO(std::min(bx1, bx2), x) && LessThanOrEqualCO(x, std::max(bx1, bx2));
    on_both = on_both && LessThanOrEqualCO(std::min(by1, by2), y) && LessThanOrEqualCO(y, std::max(by1, by2));
    return on_both;
  }
}

#endif
