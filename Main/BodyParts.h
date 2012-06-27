/*
 * BodyParts.h
 *
 *  Created on: Jul 5, 2011
 *      Author: Tobi
 */

#ifndef BODYPARTS_H_
#define BODYPARTS_H_

struct BodyParts{
	enum parts{
		HEAD=0,
		CHEST,
		LOWER_ARM_R,
		LOWER_ARM_L,
		UPPER_ARM_R,
		UPPER_ARM_L,
		LOWER_LEG_R,
		LOWER_LEG_L,
		UPPER_LEG_R,
		UPPER_LEG_L,

		BODY_COUNT

	};
};


#endif /* BODYPARTS_H_ */
