#include <iostream>
#include "metric_factory.h"

namespace Metric
{
	MetricRegistry::CreatorRegistry& MetricRegistry::Registry()
	{
		static CreatorRegistry* gpRegsitry = new CreatorRegistry();
		return *gpRegsitry;
	}

	void MetricRegistry::addCreator(const std::string &strType, Creator creator)
	{
		CreatorRegistry &registry = Registry();
		if (registry.count(strType) != 0)
		{
			std::cout << "Metric type " << strType << " already registered." << std::endl;
		}
		registry[strType] = creator;

		return;
	}

	std::shared_ptr<MetricAbstract> MetricRegistry::createMetric(const MetricInfo &mMInfo)
	{
		CreatorRegistry &registry = Registry();

		std::string strTypeName = getTypeName(mMInfo.iVersion);
		if (registry.count(strTypeName) != 1)
		{
			std::cout << "Unknown layer type: " << strTypeName << std::endl;
			std::cout << "    (known types: " << getMetricTypeListString() << ")";
		}

		return registry[strTypeName](mMInfo);
	}

	std::vector<std::string> MetricRegistry::getMetricTypeList()
	{
		CreatorRegistry &registry = Registry();
		std::vector<std::string> metricTypes;

		for (auto &it:registry)
		{
			metricTypes.push_back(it.first);
		}

		return metricTypes;
	}

	std::string MetricRegistry::getMetricTypeListString()
	{
		std::vector<std::string> metricTypes = getMetricTypeList();
		std::string mTypesStr;

		for (std::vector<std::string>::iterator iter = metricTypes.begin(); iter != metricTypes.end(); iter++)
		{
			if (iter != metricTypes.end())
			{
				mTypesStr += ", ";
			}

			mTypesStr += *iter;
		}

		return mTypesStr;
	}

	std::string MetricRegistry::getTypeName(const int iVersion)
	{
		std::string strTypeName = getTaskName(iVersion);
		strTypeName += "_" + std::to_string(iVersion);

		return strTypeName;
	}

	std::string MetricRegistry::getDefaultTypeName(const int iVersion)
	{
		std::string strTypeName = getTaskName(iVersion);
		strTypeName += "_DEFAULT";

		return strTypeName;
	}

	std::string MetricRegistry::getTaskName(const int iVersion)
	{
		int iTaskEnum = iVersion / 10000000;
		switch (iTaskEnum)
		{
		case 1:
			return std::string("Face");
		default:
			throw std::runtime_error("Invalid Version Number!");
		}
	}

	MetricRegisterer::MetricRegisterer(const std::string &strType, const std::string &strVer, MetricRegistry::Creator creator)
	{
		std::string strTypeName = strType + "_" + strVer;
		MetricRegistry::addCreator(strTypeName, creator);
	}
}