#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

#include "Odometry.hpp"

class Inspector {
public:
	Inspector(Odometry* odometry);
	void render();
private:
	Odometry* odometry;
};

#endif