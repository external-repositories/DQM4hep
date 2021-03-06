//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

#ifndef CLIENT_H
#define CLIENT_H

// -- dqm4hep headers
#include "dqm4hep/NetBuffer.h"
#include "dqm4hep/RequestHandler.h"
#include "dqm4hep/Service.h"
#include "dqm4hep/ServiceHandler.h"
#include "dqm4hep/json.h"

// -- dim headers
#include "dic.hxx"

// -- std headers
#include <algorithm>

namespace dqm4hep {

  namespace net {

    /**
     *  @brief  Client class.
     *          Main interface to server. User can send request to
     *          a server with or without expecting a response using
     *          the sendRequest() function. User can also subscribe to
     *          a particular service run on a server by using the
     *          subscribe() method and by providing a callback function.
     */
    class Client {
    public:
      /**
       *  @brief  Constructor
       */
      Client();

      /**
       *  @brief  Destructor
       */
      ~Client();

      /**
       *  @brief  Query server information
       *
       *  @param  serverName the server name
       *  @param  serverInfo the json value describing the server information
       */
      void queryServerInfo(const std::string &serverName, core::json &serverInfo) const;

      /**
       *  @brief  Send a command. Do not wait for any response
       *
       *  @param  name the request name
       *  @param  command the request content
       */
      template <typename Request>
      void sendRequest(const std::string &name, const Request &request) const;

      /**
       *  @brief  Send a request. Wait for the server response (blocking)
       *
       *  @param  name the request name
       *  @param  request the request to send
       *  @param  operation the callback operation to perform on data reception
       */
      template <typename Operation>
      void sendRequest(const std::string &name, const Buffer &request, Operation operation) const;

      /**
       *  @brief  Send a command.
       *
       *  @param  name the command name
       *  @param  command the command to send
       *  @param  blocking whether to wait for command reception on server side
       */
      template <typename Command>
      void sendCommand(const std::string &name, const Command &command, bool blocking = false) const;

      /**
       *  @brief  Subscribe to service
       *
       *  @param  serviceName the service name
       *  @param  pController the class instance that will receive the service updates
       *  @param  function the class method that will receive the service update
       *
       *  @code{.cpp}
       *  // the class callback method
       *  void MyClass::myfunction(const int &integer) { ... }
       *  Client client;
       *  MyClass c;
       *  client.subscribe("service-name", &c, &MyClass::myfunction);
       *  @endcode
       */
      template <typename Controller>
      void subscribe(const std::string &serviceName, Controller *pController,
                     void (Controller::*function)(const Buffer &));

      /**
       *  @brief  Unsubscribe from a particular service
       *
       *  @param  serviceName the service name
       *  @param  pController the controller class handling the service update
       */
      template <typename Controller>
      void unsubscribe(const std::string &serviceName, Controller *pController);

      /**
       *  @brief  Whether this client already registered a service subscription
       *
       *  @param  serviceName the service name
       */
      bool hasSubscribed(const std::string &serviceName) const;

      /**
       *  @brief  Get the number of subscriptions for a specific service
       *
       *  @param  serviceName the service name
       */
      unsigned int numberOfSubscriptions(const std::string &serviceName) const;

      /**
       *  @brief  Tell the specified server to be notified when this client exits
       *
       *  @param  serverName the server name to inform
       */
      void notifyServerOnExit(const std::string &serverName);

    private:
      typedef std::map<std::string, ServiceHandler *> ServiceHandlerMap;
      typedef std::vector<ServiceHandler *> ServiceHandlerList;
      ServiceHandlerMap m_serviceHandlerMap = {}; ///< The service map
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename Request>
    inline void Client::sendRequest(const std::string &name, const Request &request) const {
      DimRpcInfo rpcInfo(const_cast<char *>(name.c_str()), (void *)nullptr, 0);
      Buffer contents;
      auto model = contents.createModel<Request>();
      model->copy(request);
      contents.setModel(model);
      rpcInfo.setData((void *)contents.begin(), contents.size());
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void Client::sendRequest(const std::string &name, const Buffer &request) const {
      DimRpcInfo rpcInfo(const_cast<char *>(name.c_str()), (void *)nullptr, 0);
      Buffer contents;
      contents.adopt(request.begin(), request.size());
      rpcInfo.setData((void *)contents.begin(), contents.size());
    }

    //-------------------------------------------------------------------------------------------------

    template <typename Operation>
    inline void Client::sendRequest(const std::string &name, const Buffer &request, Operation operation) const {
      DimRpcInfo rpcInfo(const_cast<char *>(name.c_str()), (void *)nullptr, 0);

      // send request
      rpcInfo.setData((void *)request.begin(), request.size());

      // wait for answer from server
      char *data = (char *)rpcInfo.getData();
      int size = rpcInfo.getSize();

      Buffer response;

      if (nullptr != data && 0 != size)
        response.adopt(data, size);

      operation(response);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename Command>
    inline void Client::sendCommand(const std::string &name, const Command &command, bool blocking) const {
      Buffer contents;
      auto model = contents.createModel<Command>();
      model->copy(command);
      contents.setModel(model);

      if (blocking) {
        DimClient::sendCommand(const_cast<char *>(name.c_str()), (void *)contents.begin(), contents.size());
      } else {
        DimClient::sendCommandNB(const_cast<char *>(name.c_str()), (void *)contents.begin(), contents.size());
      }
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void Client::sendCommand(const std::string &name, const Buffer &buffer, bool blocking) const {
      if (blocking) {
        DimClient::sendCommand(const_cast<char *>(name.c_str()), (void *)buffer.begin(), buffer.size());
      } else {
        DimClient::sendCommandNB(const_cast<char *>(name.c_str()), (void *)buffer.begin(), buffer.size());
      }
    }

    //-------------------------------------------------------------------------------------------------

    template <typename Controller>
    inline void Client::subscribe(const std::string &name, Controller *pController,
                                  void (Controller::*function)(const Buffer &)) {
      auto findIter = m_serviceHandlerMap.find(name);

      if (findIter != m_serviceHandlerMap.end()) {
        findIter->second->onServiceUpdate().connect(pController, function);
        return;
      }

      m_serviceHandlerMap.insert(
          ServiceHandlerMap::value_type(name, new ServiceHandler(this, name, pController, function)));
    }

    //-------------------------------------------------------------------------------------------------

    template <typename Controller>
    inline void Client::unsubscribe(const std::string &serviceName, Controller *pController) {
      for (auto iter = m_serviceHandlerMap.begin(), endIter = m_serviceHandlerMap.end(); endIter != iter; ++iter) {
        if (serviceName != iter->first)
          continue;

        if (iter->second->onServiceUpdate().disconnect(pController))
          break;
      }
    }
  }
}

#endif //  CLIENT_H
