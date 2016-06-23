#pragma once

#include "Grid.h"

#include "../JSON/Serializable.h"
#include <vector>

namespace SSAGES
{
	// Interface for a collective variable.
	class Grid2D : public Grid
	{
	private:

		std::vector<std::vector<float>> _values;

	public:

		Grid2D(std::vector<double> lower, std::vector<double> upper,
			std::vector<bool> periodic, std::vector<int> num_points)
		{
			_NDim = 2;
			for(size_t i =0; i <lower.size(); i++)
			{
				_lower.push_back(lower[i]);
				_upper.push_back(upper[i]);
				_periodic.push_back(periodic[i]);
				_num_points.push_back(num_points[i]);
				_spacing.push_back(0.0);
			}
			//Generate Grid
			for(size_t i = 0; i < _spacing.size(); i++)
				_spacing[i] = (_upper[i] - _lower[i])/double(_num_points[i] - 1);

			_values.resize(_num_points[0]);

			for(size_t i = 0; i < _values.size(); i++)
				_values[i].resize(_num_points[1],0);
		}
	
		// Return the nearest index for given values.
		// Out of bounds will return edge vertex
		std::vector<int> GetIndices(const std::vector<float> &val) const override
		{
			std::vector<int> vertices;

			for(size_t i = 0; i < val.size(); i++)
			{
				int vertex = int(val[i] - _lower[i] + (_spacing[i]/2.0));
				if(vertex < 0) // out of bounds
					vertex = 0;
				else if(vertex > _num_points[i] -1) // out of bounds
				{
					if(_periodic[i])
						vertex = 0;
					else
						vertex = _num_points[i] -1;
				}
				vertices.push_back(vertex);
			}

			return vertices;
		}

		// Return a pointer to the grid value allowing user to use it
		// or modify it.
		float GetValue(const std::vector<int>& indices) const override
		{
			return _values[indices[0]][indices[1]];
		}

		void SetValue(const std::vector<int>& indices, float value) override
		{
			_values[indices[0]][indices[1]] = value;
		}

		//Currently only for debugging
		void PrintGrid() const override
		{
			for(size_t i = 0; i<_values.size(); i++)
			{
				for(size_t j = 0; j< _values[i].size(); j++)
					std::cout << _values[i][j]<<" ";
				std::cout<<std::endl;
			}
			std::cout<<std::endl;
		}

	};	
}