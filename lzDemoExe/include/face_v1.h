#pragma once

#include "metric_abstract.h"

namespace Metric
{
	class MetricFaceV1: public MetricAbstract
	{
	public:
		MetricFaceV1(const MetricInfo &mInfo);
		~MetricFaceV1();

		float computeSim(const FeatInfo &fInfo1, const FeatInfo &fInfo2) const;
	};
}