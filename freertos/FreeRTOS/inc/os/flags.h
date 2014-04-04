/*
 * flags.h
 *
 *  Created on: 3 apr. 2014
 *      Author: ES29
 */

#ifndef FLAGS_H_
#define FLAGS_H_

#define OS_FLAG_WAIT_SET_ANY								0x01
#define OS_FLAG_CONSUME										0x80

#define OS_FLAG_SET											0x01
#define OS_FLAG_CLR											0x02

#define HAS_FLAG(flags, challenge)							(((flags & challenge) == challenge) ? pdTRUE : pdFALSE)

#endif /* FLAGS_H_ */
