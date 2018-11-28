//Franz Blancaflor
//R00153623

#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/

//When the collector receives a broadcast message
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
 
}

//The struct that tells the collector what methods to invoke
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

//When the collector receives a unicast message
static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from) 
{
  
}

//The struct that tells the collector what methods to invoke
static const struct unicast_callbacks unicast_callbacks = { recv_uc };
static struct unicast_conn uc;


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

    PROCESS_WAIT_EVENT();

    broadcast_send(&broadcast);
    
    etimer_reset(&etimer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
