#include "face_v1.h"
#include "metric_factory.h"

namespace Metric
{
	MetricFaceV1::MetricFaceV1(const MetricInfo &mInfo)
		: MetricAbstract(mInfo)
	{}

	MetricFaceV1::~MetricFaceV1()
	{}

	float MetricFaceV1::computeSim(const FeatInfo &fInfo1, const FeatInfo &fInfo2) const
	{
		return 999.0;
	}

	REGISTER_METRIC_FACE_VER_CLASS(10200000, MetricFaceV1);
	REGISTER_METRIC_FACE_VER_CLASS(10210000, MetricFaceV1);
}