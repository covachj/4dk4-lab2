
/*
 * 
 * Simulation_Run of A Single Server Queueing System
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run,
			      double event_time, Fifoqueue_Ptr buffer)
{
  Event event;

  event.description = "Packet Arrival";
  event.function = packet_arrival_event;
  event.attachment = (void *) buffer;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */

void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * buffer)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;
  Server_Ptr link;
  double arrival_rate = LAYER2_PACKET_ARRIVAL_RATE;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time(2);
  new_packet->status = WAITING;

 
  
  /*Check which buffer is being targeted and set the appropriate link*/
  if((Fifoqueue_Ptr) buffer == data->buffer1 ) {
    link = data->link1;
    arrival_rate = LAYER1_PACKET_ARRIVAL_RATE;
    new_packet->service_time = get_packet_transmission_time(1);
    new_packet->origin = 1;
  }
  if((Fifoqueue_Ptr) buffer == data->buffer2 ) {
    link = data->link2;
    new_packet->origin = 2; 
  }
  if((Fifoqueue_Ptr) buffer == data->buffer3 ) {
    link = data->link3;
    new_packet->origin = 3;
  }

 /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */
  
  if(server_state(link) == BUSY) {
        fifoqueue_put((Fifoqueue_Ptr) buffer, (void*) new_packet);
    } else {
        start_transmission_on_link(simulation_run, new_packet, link);
    }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

  schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) + exponential_generator((double) 1/arrival_rate),
            (Fifoqueue_Ptr) buffer);
}
