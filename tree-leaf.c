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

//When the nodes receive a broadcast message
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
   
}

//The struct that tells the node what methods to invoke
/*---------------------------------------------------------------------------*/
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/


PROCESS_THREAD(example_unicast_process, ev, data)
{
  PROCESS_EXITHANDLER(unicast_close(&uc);)
    
  PROCESS_BEGIN();

  //Open broadcast connection in band 129 and unicast connection in band 135
  broadcast_open(&broadcast, 129, &broadcast_call);

  unicast_open(&uc, 135, &unicast_callbacks);	

 
PROCESS_END();
}
/*---------------------------------------------------------------------------*/
