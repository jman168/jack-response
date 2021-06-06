#include "record_frequency_response.h"

#include <stdio.h>
#include <stdlib.h>

#include <jack/jack.h>

jack_port_t *input_port, *output_port;
jack_client_t *client;

int main() {
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


    output_port = jack_port_register (client, "output",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

	input_port = jack_port_register (client, "input",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsInput, 0);

    if ((output_port == NULL) || (input_port == NULL)) {
		fprintf(stderr, "no more JACK ports available\n");
		exit (1);
	}

    if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}
}

int process (jack_nframes_t nframes, void *arg)
{    
	return 0;      
}