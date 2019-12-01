#pragma once

#include "metric_common.h"

namespace Metric
{
	class MetricAbstract
	{
	public:
		MetricAbstract(const MetricInfo &mInfo);
		virtual ~MetricAbstract();

		virtual float computeSim(const FeatInfo &fInfo1, const FeatInfo &fInfo2) const = 0;
		int getVersion() const;
		const MetricInfo& getInitInfo() const;

	private:
		const MetricInfo mMInfo;
	};
}