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
#include <dqm4hep/AllocatorHelper.h>
#include <dqm4hep/XmlHelper.h>
#include <dqm4hep/MonitorElement.h>
#include <dqm4hep/Logging.h>

namespace dqm4hep {

  namespace core {
    
#define READ_ATTRIBUTE( type, name, required ) \
    type name; \
    { \
      core::StatusCode statusCode = XmlHelper::getAttribute(element, #name, name); \
      if(statusCode != core::STATUS_CODE_SUCCESS) { \
        if(statusCode != core::STATUS_CODE_NOT_FOUND) { \
          dqm_error( "Couldn't get attribute '{0}' needed for object allocation !", #name ); \
          return nullptr; \
        } \
        else if(required and statusCode == core::STATUS_CODE_NOT_FOUND) { \
          dqm_error( "Couldn't get attribute '{0}' needed for object allocation !", #name ); \
          return nullptr; \
        } \
      } \
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* DefaultXMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, type, true );
      TClass* cls = TClass::GetClass(type.c_str());
      if(nullptr == cls) {
        return nullptr;
      }
      TObject* object = (TObject*)cls->New();
      return object;
    }
    
    //-------------------------------------------------------------------------------------------------

    TObject* TH1XMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, type, true );
      READ_ATTRIBUTE( std::string, name, true );
      READ_ATTRIBUTE( std::string, title, false );
      READ_ATTRIBUTE( int, nBinsX, true );
      READ_ATTRIBUTE( float, minX, true );
      READ_ATTRIBUTE( float, maxX, true );
      if(type == "TH1D") {
        return new TH1D(name.c_str(), title.c_str(), nBinsX, minX, maxX);
      }
      else if(type == "TH1F") {
        return new TH1F(name.c_str(), title.c_str(), nBinsX, minX, maxX);
      }
      else if(type == "TH1I") {
        return new TH1I(name.c_str(), title.c_str(), nBinsX, minX, maxX);
      }
      else if(type == "TH1C") {
        return new TH1C(name.c_str(), title.c_str(), nBinsX, minX, maxX);
      }
      else if(type == "TH1S") {
        return new TH1S(name.c_str(), title.c_str(), nBinsX, minX, maxX);
      }
      return nullptr;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* TH2XMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, type, true );
      READ_ATTRIBUTE( std::string, name, true );
      READ_ATTRIBUTE( std::string, title, false );
      READ_ATTRIBUTE( int, nBinsX, true );
      READ_ATTRIBUTE( float, minX, true );
      READ_ATTRIBUTE( float, maxX, true );
      READ_ATTRIBUTE( int, nBinsY, true );
      READ_ATTRIBUTE( float, minY, true );
      READ_ATTRIBUTE( float, maxY, true );
      if(type == "TH2D") {
        return new TH2D(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY);
      }
      else if(type == "TH2F") {
        return new TH2F(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY);
      }
      else if(type == "TH2I") {
        return new TH2I(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY);
      }
      else if(type == "TH2C") {
        return new TH2C(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY);
      }
      else if(type == "TH2S") {
        return new TH2S(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY);
      }
      return nullptr;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* TH3XMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, type, true );
      READ_ATTRIBUTE( std::string, name, true );
      READ_ATTRIBUTE( std::string, title, false );
      READ_ATTRIBUTE( int, nBinsX, true );
      READ_ATTRIBUTE( float, minX, true );
      READ_ATTRIBUTE( float, maxX, true );
      READ_ATTRIBUTE( int, nBinsY, true );
      READ_ATTRIBUTE( float, minY, true );
      READ_ATTRIBUTE( float, maxY, true );
      READ_ATTRIBUTE( int, nBinsZ, true );
      READ_ATTRIBUTE( float, minZ, true );
      READ_ATTRIBUTE( float, maxZ, true );
      if(type == "TH3D") {
        return new TH3D(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY, nBinsZ, minZ, maxZ);
      }
      else if(type == "TH3F") {
        return new TH3F(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY, nBinsZ, minZ, maxZ);
      }
      else if(type == "TH3I") {
        return new TH3I(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY, nBinsZ, minZ, maxZ);
      }
      else if(type == "TH3C") {
        return new TH3C(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY, nBinsZ, minZ, maxZ);
      }
      else if(type == "TH3S") {
        return new TH3S(name.c_str(), title.c_str(), nBinsX, minX, maxX, nBinsY, minY, maxY, nBinsZ, minZ, maxZ);
      }
      return nullptr;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* TScalarXMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, type, true );
      READ_ATTRIBUTE( std::string, value, false );
      if(type == "int") {
        int typeValue = 0;
        stringToType(value, typeValue);
        return new TScalarObject<int>(typeValue);
      }
      else if(type == "real" || type == "float") {
        float typeValue = 0;
        stringToType(value, typeValue);
        return new TScalarObject<float>(typeValue);
      }
      else if(type == "double") {
        double typeValue = 0;
        stringToType(value, typeValue);
        return new TScalarObject<double>(typeValue);
      }
      else if(type == "short") {
        short typeValue = 0;
        stringToType(value, typeValue);
        return new TScalarObject<short>(typeValue);
      }
      else if(type == "long") {
        long typeValue = 0;
        stringToType(value, typeValue);
        return new TScalarObject<long>(typeValue);
      }
      else if(type == "long64") {
        Long64_t typeValue = 0;
        stringToType(value, typeValue);
        return new TScalarObject<Long64_t>(typeValue);
      }
      return nullptr;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* THStackXMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, name, true );
      READ_ATTRIBUTE( std::string, title, false );
      return new THStack(name.c_str(), title.c_str());      
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* TH2PolyXMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, name, true );
      READ_ATTRIBUTE( std::string, title, false );
      READ_ATTRIBUTE( double, xlow, true );
      READ_ATTRIBUTE( double, xup, true );
      READ_ATTRIBUTE( double, ylow, true );
      READ_ATTRIBUTE( double, yup, true );
      return new TH2Poly(name.c_str(), title.c_str(), xlow, xup, ylow, yup);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* TProfileXMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, name, true );
      READ_ATTRIBUTE( std::string, title, false );
      READ_ATTRIBUTE( int, nBinsX, true );
      READ_ATTRIBUTE( float, xlow, true );
      READ_ATTRIBUTE( float, xup, true );
      READ_ATTRIBUTE( float, ylow, true );
      READ_ATTRIBUTE( float, yup, true );
      return new TProfile(name.c_str(), title.c_str(), nBinsX, xlow, xup, ylow, yup);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    TObject* TProfile2DXMLAllocator::create(TiXmlElement *element) const {
      if(nullptr == element) {
        return nullptr;
      }
      READ_ATTRIBUTE( std::string, name, true );
      READ_ATTRIBUTE( std::string, title, false );
      READ_ATTRIBUTE( int, nBinsX, true );
      READ_ATTRIBUTE( int, nBinsY, true );
      READ_ATTRIBUTE( float, xlow, true );
      READ_ATTRIBUTE( float, xup, true );
      READ_ATTRIBUTE( float, ylow, true );
      READ_ATTRIBUTE( float, yup, true );
      READ_ATTRIBUTE( float, zlow, true );
      READ_ATTRIBUTE( float, zup, true );
      return new TProfile2D(name.c_str(), title.c_str(), nBinsX, xlow, xup, nBinsY, ylow, yup, zlow, zup);
    }
    
#undef READ_ATTRIBUTE

  }
  
}
