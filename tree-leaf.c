//Franz Blancaflor
//R00153623

#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(example_unicast_process, "Tree Leaf");
AUTOSTART_PROCESSES(&example_unicast_process);
/*---------------------------------------------------------------------------*/

static int hop = -1;
static int sequence = -1;
static linkaddr_t sender;

struct beacon
{	
	int hop;
	int sequence;
};


/*---------------------------------------------------------------------------*/

//When the node receives a unicast message
static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{

	
}

//The struct that tells the node what methods to invoke
/*---------------------------------------------------------------------------*/
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
/*---------------------------------------------------------------------------*/

static struct broadcast_conn broadcast;

//When the nodes receive a broadcast message
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{

	struct beacon *message;

	message = packetbuf_dataptr();

	//Reading values received
	int newHop = message->hop;

	int newSequence = message->sequence;
	
	if(sequence < newSequence)
	{ 
		
		hop = newHop + 1;

		sequence = newSequence;

		sender.u8[0] = from->u8[0];
      
		sender.u8[1] = from->u8[1];

		printf("Sequence Number: %d, Hop Number: %d\n", sequence, hop);

		packetbuf_clear();

		message = (struct beacon *) packetbuf_dataptr(); 

		packetbuf_set_datalen(sizeof(struct beacon));

		message->hop = hop;

		message->sequence= sequence;

		broadcast_send(&broadcast);

	}

	else if(hop > newHop +1)
	{

		hop = newHop + 1;

		sender.u8[0] = from->u8[0];
      
		sender.u8[1] = from->u8[1];

		printf("Sequence Number: %d, Hop Number: %d\n", sequence, hop);

		packetbuf_clear();

		message = (struct values *) packetbuf_dataptr(); 
			
		packetbuf_set_datalen(sizeof(struct beacon));

		message->hop = hop;
			
		message->sequence= sequence;

		broadcast_send(&broadcast);

	}
	
}

//The struct that tells the node what methods to invoke
/*---------------------------------------------------------------------------*/
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(example_unicast_process, ev, data)
{

	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

	PROCESS_BEGIN();
	
	//Open broadcast connection in band 129 and unicast connection in band 135
	broadcast_open(&broadcast, 129, &broadcast_call);

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
