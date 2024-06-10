#pragma once

#include <QObject>

class LyricManager : public QObject
{
	Q_OBJECT

public:
	LyricManager(QObject *parent);
	~LyricManager();
};
