#include <iostream>
#include <memory>
#include "face_v1.h"
#include "metric_factory.h"

using namespace Metric;

int main()
{
	MetricInfo mMInfo = { 10200000, " ", " " };

	FeatInfo mFInfo1;
	FeatInfo mFInfo2;

	{
		MetricFaceV1  mFaceV1(mMInfo);
		float fSim = mFaceV1.computeSim(mFInfo1, mFInfo2);
		std::cout << "version: " << mFaceV1.getVersion() << std::endl;
		std::cout << "similarity: " << fSim << std::endl;
		std::cout << "111111111111111111" << std::endl;
	}
	
	{
		std::shared_ptr<MetricAbstract> pFaceV1(new MetricFaceV1(mMInfo));
		float fSim1 = pFaceV1->computeSim(mFInfo1, mFInfo2);
		std::cout << "version: " << pFaceV1->getVersion() << std::endl;
		std::cout << "similarity: " << fSim1 << std::endl;
		std::cout << "222222222222222222" << std::endl;
	}
	
	{
		std::shared_ptr<MetricAbstract> pFaceV1 = MetricRegistry::createMetric(mMInfo);
		float fSim1 = pFaceV1->computeSim(mFInfo1, mFInfo2);
		std::cout << "version: " << pFaceV1->getVersion() << std::endl;
		std::cout << "similarity: " << fSim1 << std::endl;
		std::cout << "3333333333333333" << std::endl;
	}

	{
		mMInfo.iVersion = 10210000;
		std::shared_ptr<MetricAbstract> pFaceV1 = MetricRegistry::createMetric(mMInfo);
		float fSim1 = pFaceV1->computeSim(mFInfo1, mFInfo2);
		std::cout << "version: " << pFaceV1->getVersion() << std::endl;
		std::cout << "similarity: " << fSim1 << std::endl;
		std::cout << "44444444444444444" << std::endl;
	}
	
	return 0;
}