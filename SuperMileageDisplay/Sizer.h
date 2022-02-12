#pragma once

#include <vector>

#include "ISizeable.h"
#include "Vertices.h"

template<typename PointType>
class Sizer {
public:
	Sizer(unsigned int initRows) {
		for (unsigned int i = 0; i < initRows; i++) {
			addRow();
		}
	}

	Sizer() : Sizer(0, 0) {

	}

	/// <summary>
	/// Adds a new row to the sizer.
	/// </summary>
	void addRow() {
		_grid.push_back(std::vector<ISizeable<PointType&>>());
	}

	/// <summary>
	/// Gets the number of rows in the sizer.
	/// </summary>
	void numRows() const {
		return _grid.size();
	}

	/// <summary>
	/// Gets the number of columns in a specific row in the sizer.
	/// Throws an exception if the row is not in the grid.
	/// TODO: make a custom exception.
	/// </summary>
	/// <param name="row">The row.</param>
	void getColumns(unsigned int row) const {
		return _grid[row].size();
	}

	/// <summary>
	/// Inserts a sizeable element into the row. Throws an exception
	/// if the row doesn't exist.
	/// TODO: make a custom exception.
	/// </summary>
	/// <param name="row">The row to insert into.</param>
	void insert(unsigned int row, ISizeable& element) {
		// I. Insert the element.
		_grid[row].push_back(element);

		// II. Resize all the elements with the row.
		float width = getWidths(row);
		float height = getHeights();

		float getCenterOffset = width / 2;

	}


private:
	std::vector<std::vector<ISizeable<PointType>&>> _grid;
	Vertices<PointType> _dimensions;

	/// <summary>
	/// Gets the height of every element in the grid.
	/// </summary>
	/// <returns>The height.</returns>
	PointType getHeights() const {
		return _dimensions.getHeight() / _grid.size();
	}

	/// <summary>
	/// Gets the width of every element in a row.
	/// </summary>
	/// <param name="row">The row.</param>
	/// <returns>The width.</returns>
	PointType getWidths(unsigned int row) const {
		return _dimensions.getWidth() / _grid[row].size();
	}

};