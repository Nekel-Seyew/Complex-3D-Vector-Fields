#pragma once
#include <cstdlib>
#include "nanoflann.hpp"

template <typename Derived>
struct VectorDefinerKDAdaptor{
	Derived obj; //!< A const ref to the data set origin

						/// The constructor that sets the data set source
	VectorDefinerKDAdaptor(const Derived obj_){ this->obj = obj_; }

	/// CRTP helper method
	inline const Derived derived() const { return obj; }

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return derived()->space->size(); }

	// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
	inline float kdtree_distance(const float *p1, const size_t idx_p2, size_t /*size*/) const
	{
		const Derived obj = derived();
		const float d0 = p1[0] - obj->space->at(idx_p2)->xyz()[0];
		const float d1 = p1[1] - obj->space->at(idx_p2)->xyz()[1];
		const float d2 = p1[2] - obj->space->at(idx_p2)->xyz()[2];
		return d0*d0 + d1*d1 + d2*d2;
	}

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline float kdtree_get_pt(const size_t idx, int dim) const
	{
		if (dim == 0) return derived()->space->at(idx)->xyz()[0];
		else if (dim == 1) return derived()->space->at(idx)->xyz()[1];
		else return derived()->space->at(idx)->xyz()[2];
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }

}; // end of VectorDefinerKDAdaptor

