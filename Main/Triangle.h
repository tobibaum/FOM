/*
 * Triangle.h
 *
 *  Created on: Jul 1, 2011
 *      Author: Tobi
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

struct triangle{
	btVector3 p1;
	btVector3 p2;
	btVector3 p3;

	bool operator< (const triangle& x) const{
		bool result = false;

		float area1, area2;

		float lengthAB = p1.distance(p2);
		float lengthAC = p1.distance(p3);
		area1 = (lengthAB*lengthAC)/2;

		lengthAB = x.p1.distance(x.p2);
		lengthAC = x.p1.distance(x.p3);
		area2 = (lengthAB*lengthAC)/2;

		if(area1 < area2){
			result = true;
		}

		return result;
	};

	inline
	float area(){
		float lengthAB = p1.distance(p2);
		float lengthAC = p1.distance(p3);

		return (lengthAB*lengthAC)/2;
	}
};

//inline
//triangle::operator ()(const triangle&x, const triangle&y)
//{return false;}

#endif /* TRIANGLE_H_ */
