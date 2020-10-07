#include "Asteroid.h"

Asteroid::Asteroid(int NumOfAst, const char* path) {
	
	m_numOfAst = NumOfAst;
	Xhigh = 10100;
	Xlow = 1000;
	Yhigh = 1000;
	Ylow = -1000;
	Zhigh = 1000;
	Zlow = -1000; 
}