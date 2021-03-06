//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

// -- dqm4hep headers
#include "dqm4hep/Internal.h"
#include "dqm4hep/StatusCodes.h"
#include "dqm4hep/PluginManager.h"
#include "dqm4hep/Logging.h"
#include "dqm4hep/RemoteLogger.h"
#include "dqm4hep/RunControlServer.h"
#include "dqm4hep/DQM4hepConfig.h"

// -- tclap headers
#include "tclap/CmdLine.h"
#include "tclap/Arg.h"

// -- std headers
#include <iostream>
#include <signal.h>

using namespace std;
using namespace dqm4hep::online;
using namespace dqm4hep::core;

bool process = true;

std::shared_ptr<RunControlServer> runControlServer;

//-------------------------------------------------------------------------------------------------

// key interrupt signal handling
void int_key_signal_handler(int) {
  dqm_info( "Caught CTRL+C. Stopping run control..." );
  if(runControlServer) {
    runControlServer->stop();
  }
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
  dqm4hep::core::screenSplash();

  std::string cmdLineFooter = "Please report bug to <dqm4hep@gmail.com>";
  TCLAP::CmdLine *pCommandLine = new TCLAP::CmdLine(cmdLineFooter, ' ', DQM4hep_VERSION_STR);

  TCLAP::ValueArg<std::string> runControlNameArg(
      "n"
      , "run-control-name"
      , "The run control name"
      , true
      , ""
      , "string");
  pCommandLine->add(runControlNameArg);

  TCLAP::ValueArg<std::string> passwordArg(
      "k"
      , "password"
      , "The run control password to execute command from interfaces"
      , false
      , ""
      , "string");
  pCommandLine->add(passwordArg);
  
  TCLAP::ValueArg<std::string> interfaceArg(
      "i"
      , "interface"
      , "The run control interface to load (plugin name)"
      , true
      , ""
      , "string");
  pCommandLine->add(interfaceArg);

  StringVector verbosities(Logger::logLevels());
  TCLAP::ValuesConstraint<std::string> verbosityConstraint(verbosities);
  TCLAP::ValueArg<std::string> verbosityArg(
      "v"
      , "verbosity"
      , "The logging verbosity"
      , false
      , "info"
      , &verbosityConstraint);
  pCommandLine->add(verbosityArg);
  
  TCLAP::MultiArg<std::string> parameterArg(
      "p"
      , "parameter"
      , "List of parameters to pass to user run control interface"
      , false
      , "string");
  pCommandLine->add(parameterArg);

  // parse command line
  pCommandLine->parse(argc, argv);

  // install signal handlers
  signal(SIGINT,  int_key_signal_handler);
  
  // set log level
  std::string verbosity(verbosityArg.getValue());
  std::string loggerName("run-ctrl:" + runControlNameArg.getValue());
  Logger::createLogger(loggerName, {Logger::coloredConsole(), RemoteLogger::make_shared()});
  Logger::setMainLogger(loggerName);
  Logger::setLogLevel(Logger::logLevelFromString(verbosity));

  // parse user parameters
  const StringVector &parameters(parameterArg.getValue());
  StringMap parameterMap;
  
  for(const auto &parameter : parameters) {
    StringVector keyValuePair;
    dqm4hep::core::tokenize(parameter, keyValuePair, "=");
    
    if(keyValuePair.size() != 2) {
      dqm_error( "Invalid parsing of user argument: '{0}'. Expected key=value !" );
      throw StatusCodeException(STATUS_CODE_FAILURE);
    }
    
    parameterMap[keyValuePair.at(0)] = keyValuePair.at(1);
  }
  
  // load user plugins
  THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PluginManager::instance()->loadLibraries());
  
  // create run control server
  runControlServer = std::make_shared<RunControlServer>();

  runControlServer->setName(runControlNameArg.getValue());
  runControlServer->setInterface(interfaceArg.getValue());
  
  if( passwordArg.isSet() )
    runControlServer->setPassword(passwordArg.getValue());

  runControlServer->setUserParameters(parameterMap);

  // run it !
  try
  {
    runControlServer->run();
  }
  catch(StatusCodeException &exception)
  {
    dqm_error( "Caught exception while running: {0}", exception.toString() );
    return exception.getStatusCode();
  }

  return 0;
}
