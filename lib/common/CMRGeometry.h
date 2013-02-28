/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_GEOMETRY_H
#define CMR_GEOMETRY_H

/********************  HEADERS  *********************/
#include <ostream>

/*********************  CLASS  **********************/
struct CMRVect2D
{
	CMRVect2D(int x = 0, int y = 0);
	CMRVect2D & operator+=(const CMRVect2D & vect);
	CMRVect2D operator+( const CMRVect2D& vect ) const;
	CMRVect2D & operator-=(const CMRVect2D & vect);
	CMRVect2D operator-(const CMRVect2D & vect) const;
	void set(int x,int y);
	int x;
	int y;
};

/*********************  CLASS  **********************/
struct CMRRect
{
	CMRRect(int x = 0,int y = 0,int width = 0,int height = 0);
	void set(int x,int y,int width,int height);
	bool contains(const CMRRect & rect,bool partially = false) const;
	bool contains(const CMRVect2D  & point) const;
	CMRVect2D getPoint1(void) const;
	CMRVect2D getPoint2(void) const;
	int x;
	int y;
	int width;
	int height;
};

/*******************  FUNCTION  *********************/
std::ostream & operator << (std::ostream & out,const CMRRect & rect);
std::ostream & operator << (std::ostream & out,const CMRVect2D & vect);
bool operator == (const CMRRect & rect1 , const CMRRect & rect2);
bool operator == (const CMRVect2D & v1 , const CMRVect2D & v2);

#endif // CMR_GEOMETRY_H
