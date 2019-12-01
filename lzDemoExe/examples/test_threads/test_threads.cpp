#include <iostream>
#include <boost/thread/thread.hpp>
#include "testSigleton.h"

using namespace std;

boost::mutex  io_mutex;

QMManager::QMManager()
{
	printf("QMManager constructor\n");
	QMSqlite::instance()->do_something();
}

QMSqlite::QMSqlite()
{
	printf("QMSqlite constructor\n");
}

void QMSqlite::do_something()
{
	printf("QMSqlite do_something\n");
}

void hello(int iId)
{
	//boost::mutex::scoped_lock lock(io_mutex);
	cout << "ID: " << iId << "      Hello world, I'm a thread" << endl;
	return;
}

#if 0
int main()
{
	vector<boost::thread>  vecThrds;
	for (int i = 0; i < 10; i++)
	{
		vecThrds.push_back(boost::thread(boost::bind(&hello, i)));
	}
	
	for (auto &thrd:vecThrds)
	{
		thrd.join();
	}	

	system("pause");

	return 0;
}
#else

#endif