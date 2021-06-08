#include "record_frequency_response.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <fstream>

#include <jack/jack.h>

jack_port_t *input_port, *output_port;
jack_client_t *client;

double sin_itr = 0.0;
#define MIN_FREQ 0
#define MAX_FREQ 24000.0
#define FREQ_SPEED 1000.0
#define FREQ_ITR (FREQ_SPEED/48000.0)

#define DIVISIONS 257
#define SAMPLES_PER_DIVISION ((double)(MAX_FREQ-MIN_FREQ)/FREQ_SPEED*48000.0/(double)DIVISIONS)

bool done = false;
bool begin = false;

double RMS[DIVISIONS];

double freq = MIN_FREQ;

int main() {
	std::ofstream freq_response_file("resp.txt");
	std::ofstream eq_file("eq.txt");

    jack_status_t status;

    client = jack_client_open ("Jack Response - Record Freq Res", JackNullOption, &status, NULL);
    if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}


    jack_set_process_callback (client, process, NULL);

    input_port = jack_port_register (client, "input",
                    JACK_DEFAULT_AUDIO_TYPE,
                    JackPortIsInput, 0);

    output_port = jack_port_register (client, "output",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

    if ((input_port == NULL) || (output_port == NULL)) {
		fprintf(stderr, "no more JACK ports available\n");
		exit (1);
	}

	memset(RMS, 0, sizeof(RMS));

    if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

	getchar();
	begin = true;

    while(!done) {
        
    }

	double full_RMS = 0.0;

	for(int i = 0; i < DIVISIONS; i++) {
		RMS[i] = sqrt(RMS[i]/SAMPLES_PER_DIVISION);
		full_RMS += RMS[i]*RMS[i];
	}

	full_RMS = sqrt(full_RMS/(double)DIVISIONS);

	for(int i = 0; i < DIVISIONS; i++) {
		std::cout << i << ": " << RMS[i] << "  " << float_to_db(RMS[i]/full_RMS) << "db  " << full_RMS/RMS[i] << "\n";
		freq_response_file << RMS[i]/full_RMS << "\n";
		eq_file << full_RMS/RMS[i] << "\n";
	}

	std::cout << "Full RMS: " << full_RMS << "\n";
}

int process (jack_nframes_t nframes, void *arg)
{    
    jack_default_audio_sample_t *input, *output;
    input = (jack_default_audio_sample_t*)jack_port_get_buffer (input_port, nframes);
	output = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port, nframes);

	if(begin) {
		std::cout << (int)freq << "hz" << std::endl;
		for(int i = 0; i < nframes; i++) {
			output[i] = sin(sin_itr);
			sin_itr+=0.0001309*freq;
			freq+=FREQ_ITR;
			if(freq >= MAX_FREQ) {
				done = true;
				return 0;
			}

			int RMS_index = floor((freq-MIN_FREQ) / ((MAX_FREQ-MIN_FREQ)/(double)DIVISIONS));

			RMS[RMS_index] += input[i]*input[i];
		}
	}

	return 0;      
}