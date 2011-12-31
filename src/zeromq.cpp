#include <zmq.h>
#include "zeromq.h"
#include "util.h"

static void* zeromq_context = 0;
static void* zeromq_blockpub = 0;

int zeromq_init() 
{
  std::string blockpub = GetArg("-zmqblockpub", "");
  if (blockpub.size() == 0) {
    return 0;
  }
  zeromq_context = zmq_init (1);
  if (!zeromq_context) {
    zeromq_term();
    return 0;
  }

  zeromq_blockpub = zmq_socket (zeromq_context, ZMQ_PUB);
  if (!zeromq_blockpub) {
    zeromq_term();
    return 0;
  }

  int r = zmq_bind (zeromq_blockpub, blockpub.c_str());
  if (r != 0) {
    zeromq_term();
    return 0;
  }

  return 1; 
}

void zeromq_term() 
{
  if (zeromq_blockpub) {
    zmq_close (zeromq_blockpub);
    zeromq_blockpub = 0;
  }

  if (zeromq_context) {
    zmq_term (zeromq_context);
    zeromq_context = 0;
  }
}

int zeromq_newblock(int height)
{
  if (!zeromq_blockpub) {
    return 0;
  }

  char buffer[128];
  sprintf(buffer, "newblock %d", height);

  zmq_msg_t message;
  zmq_msg_init_size (&message, strlen (buffer));
  memcpy (zmq_msg_data (&message), buffer, strlen (buffer));
  int r = zmq_sendmsg (zeromq_blockpub, &message, 0);
  zmq_msg_close (&message);
  return r == 0 ? 1 : 0;
}
