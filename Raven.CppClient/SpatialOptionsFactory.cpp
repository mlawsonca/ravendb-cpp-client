#include "stdafx.h"
#include "SpatialOptionsFactory.h"
#ifdef PPC
       	#include "xxhash.h"
#else
       	#include "xxhash.hpp"
#endif

namespace ravendb::client::documents::indexes::spatial
{
	SpatialOptionsFactory::GeographySpatialOptionsFactory SpatialOptionsFactory::geography()
	{
		return GeographySpatialOptionsFactory();
	}

	SpatialOptionsFactory::CartesianSpatialOptionsFactory SpatialOptionsFactory::cartesian()
	{
		return CartesianSpatialOptionsFactory();
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::default_options() const
	{
		return default_options(SpatialUnits::KILOMETERS);
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::default_options(
		SpatialUnits circle_radius_units) const
	{
		return geohash_prefix_tree_index(0, circle_radius_units);
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::bounding_box_index() const
	{
		return bounding_box_index(SpatialUnits::KILOMETERS);
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::bounding_box_index(
		SpatialUnits circle_radius_units) const
	{
		auto opts = SpatialOptions();
		opts.type = SpatialFieldType::GEOGRAPHY;
		opts.strategy = SpatialSearchStrategy::BOUNDING_BOX;
		opts.units = circle_radius_units;

		return opts;
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::geohash_prefix_tree_index(
		int32_t max_tree_level) const
	{
		return geohash_prefix_tree_index(max_tree_level, SpatialUnits::KILOMETERS);
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::geohash_prefix_tree_index(
		int32_t max_tree_level, SpatialUnits circle_radius_units) const
	{
		if(max_tree_level == 0)
		{
			max_tree_level = SpatialOptions::DEFAULT_GEOHASH_LEVEL;
		}
		auto opts = SpatialOptions();
		opts.type = SpatialFieldType::GEOGRAPHY;
		opts.max_tree_level = max_tree_level;
		opts.strategy = SpatialSearchStrategy::GEOHASH_PREFIX_TREE;
		opts.units = circle_radius_units;

		return opts;
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::quad_prefix_tree_index(
		int32_t max_tree_level) const
	{
		return quad_prefix_tree_index(max_tree_level, SpatialUnits::KILOMETERS);
	}

	SpatialOptions SpatialOptionsFactory::GeographySpatialOptionsFactory::quad_prefix_tree_index(int32_t max_tree_level,
		SpatialUnits circle_radius_units) const
	{
		if (max_tree_level == 0)
		{
			max_tree_level = SpatialOptions::DEFAULT_QUAD_TREE_LEVEL;
		}
		auto opts = SpatialOptions();
		opts.type = SpatialFieldType::GEOGRAPHY;
		opts.max_tree_level = max_tree_level;
		opts.strategy = SpatialSearchStrategy::QUAD_PREFIX_TREE;
		opts.units = circle_radius_units;

		return opts;
	}

	SpatialOptions SpatialOptionsFactory::CartesianSpatialOptionsFactory::bounding_box_index() const
	{
		auto opts = SpatialOptions();
		opts.type = SpatialFieldType::CARTESIAN;
		opts.strategy = SpatialSearchStrategy::BOUNDING_BOX;

		return opts;
	}

	SpatialOptions SpatialOptionsFactory::CartesianSpatialOptionsFactory::quad_prefix_tree_index(
		int32_t max_tree_level,
		SpatialBounds bounds) const
	{
		auto opts = SpatialOptions();
		opts.type = SpatialFieldType::CARTESIAN;
		opts.max_tree_level = max_tree_level;
		opts.strategy = SpatialSearchStrategy::QUAD_PREFIX_TREE;
		opts.minX = bounds.minX;
		opts.maxX = bounds.maxX;
		opts.minY = bounds.minY;
		opts.maxY = bounds.maxY;

		return opts;
	}

	SpatialOptionsFactory::SpatialBounds::SpatialBounds(double minX_param, double minY_param, double maxX_param, double maxY_param)
		: minX(minX_param)
		, maxX(maxX_param)
		, minY(minY_param)
		, maxY(maxY_param)
	{}

	std::size_t SpatialOptionsFactory::SpatialBounds::hash_code() const
	{
		XXH64_state_t* const state = XXH64_createState();
		XXH64_update(state, &maxX, 1);
		XXH64_update(state, &maxY, 1);
		XXH64_update(state, &minX, 1);
		XXH64_update(state, &minY, 1);
		return (XXH64_digest(state));
	}

	bool operator==(const SpatialOptionsFactory::SpatialBounds& lhs, const SpatialOptionsFactory::SpatialBounds& rhs)
	{
		return lhs.minX == rhs.minX
			&& lhs.maxX == rhs.maxX
			&& lhs.minY == rhs.minY
			&& lhs.maxY == rhs.maxY;
	}

	bool operator!=(const SpatialOptionsFactory::SpatialBounds& lhs, const SpatialOptionsFactory::SpatialBounds& rhs)
	{
		return !(lhs == rhs);
	}
}
