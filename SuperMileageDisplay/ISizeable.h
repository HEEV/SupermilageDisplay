#pragma once

#include "Vertices.h"

/// <summary>
/// An interface for objects which can be resized with a sizer.
/// </summary>
/// <typeparam name="PointType"></typeparam>
template<typename PointType>
class ISizeable {
public:
	/// <summary>
	/// Gets the size of the sizeable object.
	/// </summary>
	/// <returns>The vertices.</returns>
	virtual Vertices<PointType>& getSize() const = 0;

	/// <summary>
	/// Resizes the sizeable object.
	/// </summary>
	/// <param name="v">The vertices.</param>
	virtual void resize(const Vertices<PointType>& v) = 0;
};