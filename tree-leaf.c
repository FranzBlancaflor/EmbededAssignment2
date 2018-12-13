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

//RIME Address 
static linkaddr_t destination;

//Creating Struct Beacon
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

	//Creating pointer beacon called message
	struct beacon *message;

	//Pointer for data buffer
	message = packetbuf_dataptr();

	//Reading values received
	int newHop = message->hop;

	int newSequence = message->sequence;
	
	//If sequence received is bigger than sequence, then execute if statement
	if(sequence < newSequence)
	{ 
		//Make hop count to the kop that was recieved a +1
		hop = newHop + 1;
		
		//Make Sequence Received the new sequence
		sequence = newSequence;

		destination.u8[0] = from->u8[0];
      
		destination.u8[1] = from->u8[1];

		//Print Out
		printf("Sequence Number: %d, Hop Number: %d\n", sequence, hop);

		//Buffer Memory
		message = (struct beacon *) packetbuf_dataptr(); 

		//Getting the length
		packetbuf_set_datalen(sizeof(struct beacon));

		//Get current hop value
		message->hop = hop;

		//Get current sequence value
		message->sequence= sequence;

		broadcast_send(&broadcast);

	}

	//This will only really run if we receive a broadcast from a node closer to the sink
	//after we have already received a broadcast from a node further away.
	else if(hop > newHop)
	{

		hop = newHop + 1;

		destination.u8[0] = from->u8[0];
      
		destination.u8[1] = from->u8[1];

		printf("Sequence Number: %d, Hop Number: %d\n", sequence, hop);

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
	
	//Open broadcast connection in band 129
	broadcast_open(&broadcast, 129, &broadcast_call);

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
