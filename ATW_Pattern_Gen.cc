/*
 * Copyright (c) 2015-2016 - Massachusetts Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The copyright holder hereby grants to the U.S. Government a copyright
 * license to use this computer software/computer software documentation
 * that is of the same scope as the rights set forth in the definition of
 * "unlimited rights" found in DFARS 252.227-7014(a)(15)(June 1995).
 */

// ATW Transmit Pattern Generator 

// To build and run:
// g++ -W -Wall -std=gnu++11 atw_tx_pattern_gen.cc -o atw_tx_pattern_gen
// ./atw_tx_pattern_gen_version

// Given the same set of input parameters, this program should generate
// the same patterns when invoked repeatedly, across any systems utilizing
// the standard C++ Library

// For each pulse in each transmit pattern, the pulse delay (jitter) should be
// applied after transmission of the sync or data pulse on the specified channel,
// and then after delaying the fixed pulse guard time.

// Given that the pseudo-random jitter could be 0 us, the pulse guard time
// represents the minimal time the transmitter or receiver hardware requires
// to start transmitting or receiving the subsequent pulse.

#include <iostream>
#include <sstream>
#include <cmath>
#include <random>
#include <set>

#if 0
#define DIAGS	// output diagnostic messages
#endif

#define CHECKSUM	// calculate checksum for table data

#ifdef CHECKSUM
#define MAX_PATTERNS 1024
#define MAX_PULSES 64
#define OCTETS_PER_PULSE 3	// channel[1], offset[2]
// Serialized TX Pattern Table Data (binary - for checksum calculation)
std::uint8_t tx_pattern_table[MAX_PATTERNS*MAX_PULSES*OCTETS_PER_PULSE];
std::uint8_t *tx_pattern_table_ptr;
std::uint16_t tx_pattern_table_size;		// in octets
std::uint16_t tx_pattern_table_checksum;
#endif // CHECKSUM


// Selection of unique channels within each frame is done pseudo-randomly.
// This protects against the small probability that a unique channel is not found
// within a fixed number of attempts...
#define ALLOCATE_DATA_CHANNEL_ATTEMPTS 1000

// Set of unique data channels per pattern to track duplicates
std::set<int> data_channel_set;

int main(int argc, char *argv[])
{

  std::cout << "The LL ACENet Transmit Pattern Generator" << std::endl;

  std::cout << "generated the following Transmit Pattern Table..." << std::endl;

  // initialize all options to default values
  int pattern_number = 32;
  int data_channel_number = 22;
  int sync_channel_number = 2;
  int frame_duration = 20;		// ms
  int sync_pulse_number = 4;
  int sync_pulse_duration = 160;	// us
  int data_pulse_number = 16;
  int data_pulse_duration = 192;	// us
  int pulse_guard_duration = 38;	// us
  int random_number_seed = 0;

  std::vector<std::string> cla;
  for (int i = 1; i < argc; i++)
  {
    cla.push_back(argv[i]);
  }

  bool next_is_pattern_number = false;
  bool next_is_data_channel_number = false;
  bool next_is_sync_channel_number = false;
  bool next_is_frame_duration = false;
  bool next_is_sync_pulse_number = false;
  bool next_is_sync_pulse_duration = false;
  bool next_is_data_pulse_number = false;
  bool next_is_data_pulse_duration = false;
  bool next_is_pulse_guard_duration = false;
  bool next_is_random_number_seed = false;
     
  for (unsigned int i = 0; i < cla.size(); i++)
  {      
    if (next_is_pattern_number) 
    {
      std::stringstream ss(cla[i]);
      ss >> pattern_number;
#ifdef CHECKSUM
      if (pattern_number > MAX_PATTERNS) {
        std::cout << "ERROR: pattern_number = " << pattern_number << " exceeds MAX_PATTERNS = " << MAX_PATTERNS << " !" << std::endl;
        std::cout << std::flush;
        exit(0);
      }
#endif
      next_is_pattern_number = false;
    }
    else if (next_is_sync_channel_number)
    {
      std::stringstream ss(cla[i]);
      ss >> sync_channel_number;
      if ( (sync_channel_number % 2) != 0 ) {
        std::cout << "ERROR: sync_channel_number = " << sync_channel_number << " -- must be an even number!" << std::endl;
        std::cout << std::flush;
        exit(0);
      }
      next_is_sync_channel_number = false;
    }
    else if (next_is_data_channel_number)
    {
      std::stringstream ss(cla[i]);
      ss >> data_channel_number;
      next_is_data_channel_number = false;
    }
    else if (next_is_frame_duration)
    {
      std::stringstream ss(cla[i]);
      ss >> frame_duration;
      next_is_frame_duration = false;
    }
    else if (next_is_sync_pulse_number)
    {
      std::stringstream ss(cla[i]);
      ss >> sync_pulse_number;
      if ( (sync_pulse_number % 2) != 0 ) {
        std::cout << "ERROR: sync_pulse_number = " << sync_pulse_number << " -- must be an even number!" << std::endl;
        std::cout << std::flush;
        exit(0);
      }
      next_is_sync_pulse_number = false;
    }
    else if (next_is_sync_pulse_duration)
    {
      std::stringstream ss(cla[i]);
      ss >> sync_pulse_duration;
      next_is_sync_pulse_duration = false;
    }
    else if (next_is_data_pulse_number)
    {
      std::stringstream ss(cla[i]);
      ss >> data_pulse_number;
      next_is_data_pulse_number = false;
    }
    else if (next_is_data_pulse_duration)
    {
      std::stringstream ss(cla[i]);
      ss >> data_pulse_duration;
      next_is_data_pulse_duration = false;
    }
    else if (next_is_pulse_guard_duration)
    {
      std::stringstream ss(cla[i]);
      ss >> pulse_guard_duration;
      next_is_pulse_guard_duration = false;
    }
    else if (next_is_random_number_seed)
    {
      std::stringstream ss(cla[i]);
      ss >> random_number_seed;
      next_is_random_number_seed = false;
    }
    else
    {
      if (cla[i] == "-p")
      {
        next_is_pattern_number = true;
      }
      else if (cla[i] == "-scn")
      {
        next_is_sync_channel_number = true;
      }
      else if (cla[i] == "-dcn")
      {
        next_is_data_channel_number = true;
      }
      else if (cla[i] == "-fd")
      {
        next_is_frame_duration = true;
      }
      else if (cla[i] == "-spn")
      {
        next_is_sync_pulse_number = true;
      }
      else if (cla[i] == "-spd")
      {
        next_is_sync_pulse_duration = true;
      }
      else if (cla[i] == "-dpn")
      {
        next_is_data_pulse_number = true;
      }
      else if (cla[i] == "-dpd")
      {
        next_is_data_pulse_duration = true;
      }
      else if (cla[i] == "-pgd")
      {
        next_is_pulse_guard_duration = true;
      }
      else if (cla[i] == "-s")
      {
        next_is_random_number_seed = true;
      }
      else if (cla[i] == "-h")
      {
        std::cout << "Following are available options, with [default] values:" << std::endl;
        std::cout << "-p pattern_number [" << pattern_number << "]" << std::endl;
        std::cout << "-scn sync_channel_number (even) [" << sync_channel_number << "]" << std::endl;
        std::cout << "-dcn data_channel_number [" << data_channel_number << "]" << std::endl;
        std::cout << "-fd frame_duration (ms) [" << frame_duration << "]" << std::endl;
        std::cout << "-spn sync_pulse_number (even) [" << sync_pulse_number << "]" << std::endl;
        std::cout << "-spd sync_pulse_duration (us) [" << sync_pulse_duration << "]" << std::endl;
        std::cout << "-dpn data_pulse_number [" << data_pulse_number << "]" << std::endl;
        std::cout << "-dpd data_pulse_duration (us) [" << data_pulse_duration << "]" << std::endl;
        std::cout << "-pgd pulse_guard_duration (us) [" << pulse_guard_duration << "]" << std::endl;
        std::cout << "-s random_number_seed [" << random_number_seed << "]" << std::endl;
        std::cout << std::flush;
        exit(0);
      }
    }
  }

  std::cout << std::endl;
  std::cout << "option -p pattern_number = " << pattern_number << std::endl;
  std::cout << "option -scn sync_channel_number = " << sync_channel_number << std::endl;
  std::cout << "option -dcn data_channel_number = " << data_channel_number << std::endl;
  std::cout << "option -fd frame_duration = " << frame_duration << " ms" << std::endl;
  std::cout << "option -spn sync_pulse_number = " << sync_pulse_number << std::endl;
  std::cout << "option -spd sync_pulse_duration = " << sync_pulse_duration << " us" << std::endl;
  std::cout << "option -dpn data_pulse_number = " << data_pulse_number << std::endl;
  std::cout << "option -dpd data_pulse_duration = " << data_pulse_duration << " us" << std::endl;
  std::cout << "option -pgd pulse_guard_duration = " << pulse_guard_duration << " us" << std::endl;
  std::cout << "option -s random_number_seed = " << random_number_seed << std::endl;
  std::cout << std::endl;

  // initialize random number generators

  std::mt19937 e2_channel(random_number_seed);
  std::uniform_real_distribution<> dist_channel;

  std::mt19937 e2_jitter(random_number_seed);
  std::uniform_real_distribution<> dist_jitter;

  // JitterDelay = (1 – (1 – x)^(1/(b-1)))^(1/a)*TIME_SCALE
  // for uniform random x in [0,1], so for the i-th pulse:
  // JitterDelay_i = (1 – (1-x)^(1/(NHOPS-i-1)))*TIME_REMAINING
  
  // Calculate total number of channels
  int total_channel_number = data_channel_number + sync_channel_number;

  // Calculate total number of pulses
  int total_pulse_number = sync_pulse_number + data_pulse_number;

#ifdef CHECKSUM
  if (total_pulse_number > MAX_PULSES) {
    std::cout << "ERROR: total_pulse_number = " << total_pulse_number << " exceeds MAX_PULSES = " << MAX_PULSES << " !" << std::endl;
    std::cout << std::flush;
    exit(0);
  }
#endif

  // If there are at least as many data channels as data pulses, then
  // ensure that the selected data channels are all unique within the each...
  bool enforce_unique_data_channel;
  if (data_channel_number >= data_pulse_number) {
    enforce_unique_data_channel = true;
    std::cout << "# data channels >= # data pulses, so ARE enforcing unique channels per pattern" << std::endl;
  }
  else {
    enforce_unique_data_channel = false;
    std::cout << "# data channels < # data pulses, so NOT enforcing unique channels per pattern" << std::endl;
  }

#ifdef CHECKSUM
  // initialize pointer to serialized TX Pattern Table Data (binary)
  tx_pattern_table_ptr = &tx_pattern_table[0];
#endif

  // Generate a unique transmit pattern for each frame...
  int i; 		// transmit pattern ID
  for (i=0; i<pattern_number; i++) {
    std::cout << std::endl;
    std::cout << "Transmit Pattern ID = " << i << std::endl;
    std::cout << std::endl;
    // output the table header
    std::cout << "\tpulse\tchannel\toffset (us)" << std::endl;
    std::cout << std::endl;

    // calculate jitter time remaining in frame (us)
    int time_remaining = ( (frame_duration*1000) 
                          - sync_pulse_number*(sync_pulse_duration+pulse_guard_duration) 
                          - data_pulse_number*(data_pulse_duration+pulse_guard_duration) );

    int total_jitter = 0;	// total jitter per frame
    int offset = 0;		// accumulated per pulse offset (from start of frame - in us)
    int pulse_duration;		// duration of sync or data pulse in us
    int jitter;			// per pulse jitter delay (us)
    float x;			// uniform random x in [0,1]
    int channel;		// per pulse frequency channel
    int j; 			// pulse ID

    // Reset data channel set per pattern - used to check for duplicates...
    data_channel_set.clear();

    // Generate a pulse entry for each transmit pattern...
    for (j=0; j<total_pulse_number; j++) {

      if (j < sync_pulse_number) {  // sync pulse
        if ((j % 2) == 0) { // even sync pulses use lower channels
          channel = (j % sync_channel_number)/2;
        } 
        else { // odd sync pulses use upper channels
          channel = total_channel_number - (j % sync_channel_number)/2 - 1;
        }
        pulse_duration = sync_pulse_duration;
      }

      else {  // data pulse
        bool allocated_channel = false;
        // Limit number of attempts to allocate a data channel...
        for (int k = 0; k < ALLOCATE_DATA_CHANNEL_ATTEMPTS; k++) {       
          x = dist_channel(e2_channel);  // uniform random x in [0,1]
          channel = (x * (data_channel_number)) + sync_channel_number/2;
          pulse_duration = data_pulse_duration;
      
          if (enforce_unique_data_channel) {
             // Check if selected data channel is unique
             if (data_channel_set.find(channel) != data_channel_set.end()) {
               // This channel is already in unique set - look for another...
               continue;
             } else {
               allocated_channel = true;
               break;		// break out of for loop
             }
          } else {
             allocated_channel = true;
             break;		// break out of for loop
          }
        } // end of while()

        if (allocated_channel == false) {
           std::cout << "ERROR: could not allocate a data pulse!!!" << std::endl;
           std::cout << std::flush;
           exit(0);           
        }

        if (enforce_unique_data_channel) {
          // Add selected data channel to set of unique data channels for this pattern
          data_channel_set.insert(channel);
        }
      }

      x = dist_jitter(e2_jitter);  // uniform random x in [0,1]
      jitter = ( 1.0 - pow((1.0-x), 1.0/(total_pulse_number-j-1)) )*time_remaining;

#ifdef CHECKSUM
      *tx_pattern_table_ptr++ = (std::uint8_t)channel;
      *tx_pattern_table_ptr++ = (std::uint8_t)((offset>>8) & 0xff);	// high-order octet
      *tx_pattern_table_ptr++ = (std::uint8_t)(offset & 0xff);		// low-order octet
#endif // CHECKSUM
      std::cout << "\t" << j << "\t" << channel << "\t" << offset << std::endl;
      offset += pulse_duration + pulse_guard_duration + jitter;
      total_jitter += jitter;
      time_remaining -= jitter;
    } // end of for all j

#ifdef DIAGS
    int actual_frame_duration = sync_pulse_number*(sync_pulse_duration+pulse_guard_duration) 
                                + data_pulse_number*(data_pulse_duration+pulse_guard_duration)
                                + total_jitter;
    std::cout << std::endl;
    std::cout << "  Intended frame duration = " << frame_duration*1000 << " us - " << 
                     "Actual frame duration = " << actual_frame_duration << " us" << std::endl;
#endif

  } // end of for all i

#ifdef CHECKSUM
  // Calculate checksum for serialized TX Pattern Table Data

  tx_pattern_table_checksum = 0;
  tx_pattern_table_size = tx_pattern_table_ptr - &tx_pattern_table[0];
  for (int i = 0; i < tx_pattern_table_size/2; i=i+2)
  {
    tx_pattern_table_checksum += (uint16_t)tx_pattern_table[i];
  }
  std::cout << std::endl << "Transmit Pattern Table Checksum = 0x" << std::hex << std::uppercase << tx_pattern_table_checksum << std::endl;
#endif // CHECKSUM

  return 0;
}

/* Examples of mapping sync pulses to sync channels:

    if scn = 2, spn = 2
      pulse   channel
      0       0
      1       c-1

    if scn = 2, spn = 4
      pulse   channel
      0       0
      1       c-1
      2       0
      3       c-1

    if scn = 4, spn = 2
      pulse   channel
      0       0
      1       c-1

    if scn = 4, spn = 4
      pulse   channel
      0       0
      1       c-1
      2       1
      3       c-2

    if scn = 4, spn = 8
      pulse   channel
      0       0
      1       c-1
      2       1
      3       c-2
      4       0
      5       c-1
      6       1
      7       c-2
*/
