#pragma once

#include <string>

namespace Metric
{
	struct MetricInfo
	{
		int iVersion;
		std::string strLicPath;
		std::string strLogPath;
	};

	struct FeatInfo
	{
		void *pvFeatData;
		int  iFeatSize;
	};
}
