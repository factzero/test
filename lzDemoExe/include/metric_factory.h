#pragma once
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "metric_abstract.h"

namespace Metric
{
	class MetricRegistry
	{
	public:
		typedef std::shared_ptr<MetricAbstract> (* Creator)(const MetricInfo &);
		typedef std::map<std::string, Creator> CreatorRegistry;

		static CreatorRegistry& Registry();
		static void addCreator(const std::string &strType, Creator creator);
		static std::shared_ptr<MetricAbstract> createMetric(const MetricInfo &mMInfo);

		static std::vector<std::string> getMetricTypeList();

	private:
		MetricRegistry();

		static std::string getMetricTypeListString();
		static std::string getTypeName(const int iVersion);
		static std::string getDefaultTypeName(const int iVersion);
		static std::string getTaskName(const int iVersion);
	};

	class MetricRegisterer
	{
	public:
		MetricRegisterer(const std::string &strType, const std::string &strVer, MetricRegistry::Creator creator);
	};

	#define REGISTER_METRIC_CLASS(task, ver, type)                                                 \
		std::shared_ptr<MetricAbstract>  Creator_##type##_##ver##_Metric(const MetricInfo &mMInfo) \
		{                                                                                          \
			return std::shared_ptr<MetricAbstract>(new type(mMInfo));                              \
		}                                                                                          \
		static MetricRegisterer gCreator_##task##_##ver(#task, #ver, Creator_##type##_##ver##_Metric)

	#define REGISTER_METRIC_FACE_VER_CLASS(ver, type) REGISTER_METRIC_CLASS(Face, ver, type)
}