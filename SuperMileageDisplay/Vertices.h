#pragma once

#include <JuceHeader.h>
#include <functional>

/// <summary>
/// A sizer for graphical widgets, allowing for scaling and auto-scaling functionality.
/// </summary>
/// <typeparam name="PointType">The type of JUCE point to use.</typeparam>
template<typename PointType>
class Vertices {
public:

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="center">The center of the sized object.</param>
	/// <param name="hRadius">The horizontal radius of the object.</param>
	/// <param name="vRadius">The vertical radius of the object.</param>
	Vertices(juce::Point<PointType> center, PointType hRadius, PointType vRadius) {
		_center = center;
		_h_radius = hRadius;
		_v_radius = vRadius;
	}

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="center">The center of the sized object.</param>
	/// <param name="radius">The radius of the object.</param>
	Vertices(juce::Point<PointType> center, PointType radius) : Vertices(center, radius, radius) {

	}

	/// <summary>
	/// Gets the center of the object.
	/// </summary>
	/// <returns>The center point.</returns>
	juce::Point<PointType> getCenter() const {
		return _center;
	}

	/// <summary>
	/// Gets the top-left point of the object.
	/// </summary>
	/// <returns>The top-left point.</returns>
	juce::Point<PointType> getTopLeft() const {
		return juce::Point<PointType>(_center.getX() - _h_radius, _center.getY() + _v_radius);
	}

	/// <summary>
	/// Gets the top-right point of the object.
	/// </summary>
	/// <returns>The top-right point.</returns>
	juce::Point<PointType> getTopRight() const {
		return juce::Point<PointType>(_center.getX() - _h_radius, _center.getY() + _v_radius);
	}

	/// <summary>
	/// Gets the bottom-left point of the object.
	/// </summary>
	/// <returns>The bottom-left point.</returns>
	juce::Point<PointType> getBottomLeft() const {
		return juce::Point<PointType>(_center.getX() + _h_radius, _center.getY() - _v_radius);
	}

	/// <summary>
	/// Gets the bottom-right point of the object.
	/// </summary>
	/// <returns>The bottom-right  point.</returns>
	juce::Point<PointType> getBottomRight() const {
		return juce::Point<PointType>(_center.getX() + _h_radius, _center.getY() + _v_radius);
	}

	/// <summary>
	/// Sets the center of the sizer.
	/// </summary>
	/// <param name="center">The new center.</param>
	void setCenter(juce::Point<PointType> center) {
		_center = center;

		callback();
	}

	/// <summary>
	/// Sets the horizontal radius of the sizer.
	/// </summary>
	/// <param name="hRadius">The new radius.</param>
	void setHorizontalRadius(PointType hRadius) {
		_h_radius = hRadius;

		callback();
	}

	/// <summary>
	/// Sets the vertical radius of the sizer.
	/// </summary>
	/// <param name="hRadius">The new radius.</param>
	void setVerticalRadius(PointType vRadius) {
		_v_radius = vRadius;

		callback();
	}

	/// <summary>
	/// Sets the callback for if the sizer is updated.
	/// </summary>
	/// <param name="callback">The callback function.</param>
	void setUpdateCallback(std::function<void()> callback) {
		_on_size_update = callback;
	}

	/// <summary>
	/// Gets the height of the object.
	/// </summary>
	void getHeight() const {
		return _v_radius * 2;
	}

	/// <summary>
	/// Gets the width of the object.
	/// </summary>
	void getWidth() const {
		return _h_radius * 2;
	}

private:
	juce::Point<PointType> _center;
	PointType _h_radius;
	PointType _v_radius;
	std::function<void()> _on_size_update;

	void callback() {
		if (_on_size_update) {
			_on_size_update();
		}
	}

};