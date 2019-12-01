#include "metric_abstract.h"

namespace Metric
{
	MetricAbstract::MetricAbstract(const MetricInfo &mInfo)
		: mMInfo(mInfo)
	{}

	MetricAbstract::~MetricAbstract()
	{}

	int MetricAbstract::getVersion() const
	{
		return mMInfo.iVersion;
	}

	const MetricInfo& MetricAbstract::getInitInfo() const
	{
		return mMInfo;
	}

}