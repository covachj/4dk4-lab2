
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

/*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "simparameters.h"
#include "packet_arrival.h"
#include "cleanup_memory.h"
#include "trace.h"
#include "main.h"

/******************************************************************************/

/*
 * main.c declares and creates a new simulation_run with parameters defined in
 * simparameters.h. The code creates a fifo queue and server for the single
 * server queueuing system. It then loops through the list of random number
 * generator seeds defined in simparameters.h, doing a separate simulation_run
 * run for each. To start a run, it schedules the first packet arrival
 * event. When each run is finished, output is printed on the terminal.
 */

int
main(void)
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;
  FILE *fptr;
  fptr =fopen("P5_10.csv", "w+");
  fprintf(fptr, "SEED, Arrival Count, Transmitted Packet Count, Service Fraction, Arrival Rate Layer 1, Arrival Rate Layer 2, Mean Delay, Mean Delay(S1),Mean Delay(S2),Mean Delay(S3), P(1,2)\n");
  /*
   * Declare and initialize our random number generator seeds defined in
   * simparameters.h
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;
  int j=0;

  /* 
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */

  while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

    simulation_run = simulation_run_new(); /* Create a new simulation run. */

    /*
     * Set the simulation_run data pointer to our data object.
     */

    simulation_run_attach_data(simulation_run, (void *) & data);

    /* 
     * Initialize the simulation_run data variables, declared in main.h.
     */
    
    data.blip_counter = 0;
    data.arrival_count = 0;
    data.number_of_packets_processed = 0;
    data.accumulated_delay = 0.0;
    data.random_seed = random_seed;
 
    /* 
     * Create the packet buffer and transmission link, declared in main.h.
     */

    data.buffer1 = fifoqueue_new();
    data.link1   = server_new();
    data.buffer2 = fifoqueue_new();
    data.link2   = server_new();
    data.buffer3 = fifoqueue_new();
    data.link3   = server_new();

    /* 
     * Set the random number generator seed for this run.
     */

    random_generator_initialize(random_seed);

    /* 
     * Schedule the initial packet arrival for the current clock time (= 0).
     */

    schedule_packet_arrival_event(simulation_run, 
				  simulation_run_get_time(simulation_run),
                  data.buffer1);
    schedule_packet_arrival_event(simulation_run, 
				  simulation_run_get_time(simulation_run)+1,
                  data.buffer2);
    schedule_packet_arrival_event(simulation_run, 
				  simulation_run_get_time(simulation_run)+2,
                  data.buffer3);

    /* 
     * Execute events until we are finished. 
     */
    while(data.number_of_packets_processed < RUNLENGTH) {
      simulation_run_execute_event(simulation_run);
    }

    /*
     * Output results and clean up after ourselves.
     */

    output_results(simulation_run);
    

    Simulation_Run_Data_Ptr sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
    double xmtted_fraction = (double) sim_data->number_of_packets_processed /
        sim_data->arrival_count;
    fprintf(fptr,"%d, %ld, %ld, %.5f,%.3f,%.3f,%.2f,%.2f,%.2f,%.2f, %.2f\n", sim_data-> random_seed,
                                            sim_data->arrival_count,
                                            sim_data->number_of_packets_processed,
                                            xmtted_fraction,
                                            (double) LAYER1_PACKET_ARRIVAL_RATE,
                                            (double) LAYER2_PACKET_ARRIVAL_RATE,
                                            1e3*sim_data->accumulated_delay/sim_data->number_of_packets_processed,
                                            1e3*sim_data->accumulated_delay_switch1/sim_data->processed_switch1,
                                            1e3*sim_data->accumulated_delay_switch2/sim_data->processed_switch2,
                                            1e3*sim_data->accumulated_delay_switch3/sim_data->processed_switch3,
                                            P_1_2);

    cleanup_memory(simulation_run);
  }
  fclose(fptr);
  getchar();   /* Pause before finishing. */
  return 0;
}












