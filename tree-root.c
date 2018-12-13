//Franz Blancaflor
//R00153623

#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Tree Root");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/

static int hop = 0;
static int sequence = -1;

//Struct of the beacon that will be sent out
static struct beacon
{

	int hop; 
	int sequence;

};

/*---------------------------------------------------------------------------*/

//When the collector receives a broadcast message
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
	
	
}

//The struct that tells the collector what methods to invoke
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

//When the collector receives a unicast message
recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{

}

//The struct that tells the collector what methods to invoke
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
  //Making an etimer
  static struct etimer et;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  //Open broadcast connection in band 129 and unicast connection in band 135
  broadcast_open(&broadcast, 129, &broadcast_call);

  unicast_open(&uc, 135, &unicast_callbacks);

  while(1) {

    /* Every 10 Seconds */
    etimer_set(&et, CLOCK_SECOND * 10);

    PROCESS_WAIT_EVENT();
    
    //Making a pointer beacon called message
    struct beacon *message;
    
    //Buffer Memory
    message = (struct beacon *) packetbuf_dataptr();

    //Getting the length
    packetbuf_set_datalen(sizeof(struct beacon));

    //Getting the hop value
    message->hop = hop;
    
    //Increment the sequence value. So that when a new beacon is sent it will be a different number
    sequence = sequence + 1;

    //Getting the sequence value
    message->sequence = sequence;
  
    printf("Message Sent");

    //Send broadcast
    broadcast_send(&broadcast);


  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
