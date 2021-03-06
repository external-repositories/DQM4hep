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
#include "dqm4hep/EventReader.h"

namespace dqm4hep {

  namespace core {
    
    EventReader::~EventReader() {
      /* nop */
    }
    
    //-------------------------------------------------------------------------------------------------
    
    core::Signal<core::EventPtr> &EventReader::onEventRead() {
      return m_onEventRead;
    }

  }

}

