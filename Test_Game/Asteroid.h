#pragma once

class Asteroid {

public:

	Asteroid(int NumOfAst, const char* path);
	~Asteroid();


private:

	int m_numOfAst;
	int Xhigh, Xlow;
	int Yhigh, Ylow;
	int Zhigh, Zlow;



};
