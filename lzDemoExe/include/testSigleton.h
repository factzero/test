#pragma once

class QMManager
{
protected:
	static QMManager instance_;
	QMManager();
	~QMManager(){ };
public:
	static QMManager *instance()
	{
		return &instance_;
	}
};

class QMSqlite
{
protected:
	static QMSqlite instance_;
	QMSqlite();
	~QMSqlite(){ };

public:
	static QMSqlite *instance()
	{
		return &instance_;
	}

	void do_something();

};

QMManager QMManager::instance_;
QMSqlite  QMSqlite::instance_;