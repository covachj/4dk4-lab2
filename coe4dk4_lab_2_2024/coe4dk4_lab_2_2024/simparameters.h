
/*
 * 
 * Simulation of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/******************************************************************************/

#define LAYER1_PACKET_ARRIVAL_RATE 750 /* packets per second */
#define LAYER2_PACKET_ARRIVAL_RATE 500 /* packets per second */
#define PACKET_LENGTH 1e3 /* bits */
#define LAYER1_LINK_BIT_RATE 2e6 /* bits per second */
#define LAYER2_LINK_BIT_RATE 1e6 /* bits per second */
#define RUNLENGTH 10e6 /* packets */
#define P_1_2 0.10 /*Used values {0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95}*/
/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400243271, 400307241 ,111111 ,222222,333333, 444444, 555555, 666666, 777777, 888888

#define LAYER1_PACKET_XMT_TIME ((double) PACKET_LENGTH/LAYER1_LINK_BIT_RATE)
#define LAYER2_PACKET_XMT_TIME ((double) PACKET_LENGTH/LAYER2_LINK_BIT_RATE)
#define BLIPRATE (RUNLENGTH/10000)

/******************************************************************************/

#endif /* simparameters.h */



