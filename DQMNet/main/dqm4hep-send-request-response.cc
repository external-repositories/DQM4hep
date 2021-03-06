//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

#include "dqm4hep/Client.h"

using namespace dqm4hep::net;

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage : dqm4hep-send-request-response name [content]" << std::endl;
    return 1;
  }

  std::string name(argv[1]);
  std::string request;

  for (int i = 2; i < argc; i++)
    request += argv[i] + std::string(" ");

  Client client;
  Buffer requestBuffer;
  requestBuffer.adopt(request.c_str(), request.size());

  client.sendRequest(name, requestBuffer, [](const Buffer &response) {
    std::cout << std::string(response.begin(), response.size()) << std::endl;
  });

  return 0;
}
