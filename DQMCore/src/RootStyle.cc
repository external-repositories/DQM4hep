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
#include <dqm4hep/RootStyle.h>
#include <dqm4hep/RootHeaders.h>
#include <dqm4hep/Logging.h>
#include <dqm4hep/XmlHelper.h>

#define ROOT_COLOR_TABLE(d)\
  d(kWhite) d(kBlack) d(kGray) d(kRed) d(kGreen)\
  d(kBlue) d(kYellow) d(kMagenta) d(kCyan) d(kOrange)\
  d(kSpring) d(kTeal) d(kAzure) d(kViolet) d(kPink)

#define ROOT_COLOR_TO_COLOR(color) if(str == #color) return color;

namespace dqm4hep {

  namespace core {

    void RootStyle::applyTo(TObject *object) const {
      if(nullptr == object) {
        return;
      }
      TAttMarker *markerAtt = dynamic_cast<TAttMarker*>(object);
      if(nullptr != markerAtt) {
        markerAtt->SetMarkerColor(m_markerColor);
        markerAtt->SetMarkerStyle(m_markerStyle);
        markerAtt->SetMarkerSize(m_markerSize);
      }
      TAttLine *lineAtt = dynamic_cast<TAttLine*>(object);
      if(nullptr != lineAtt) {
        lineAtt->SetLineColor(m_lineColor);
        lineAtt->SetLineStyle(m_lineStyle);
        lineAtt->SetLineWidth(m_lineWidth);
      }
      TAttFill *fillAtt = dynamic_cast<TAttFill*>(object);
      if(nullptr != fillAtt) {
        fillAtt->SetFillColor(m_fillColor);
        fillAtt->SetFillStyle(m_fillStyle);
      }
    }
    
    //-------------------------------------------------------------------------------------------------

    int RootStyle::stringToColor(const std::string &str) {
      int color = -1;
      if(!stringToType(str, color)) {
        ROOT_COLOR_TABLE(ROOT_COLOR_TO_COLOR)
        return -1;
      }
      return color;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    StatusCode RootStyle::fromXml(const TiXmlHandle &handle) {
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::readParameter(handle, 
        "MarkerStyle", m_markerStyle));
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, RootStyle::readColor(handle, 
        "MarkerColor", m_markerColor));
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::readParameter(handle, 
        "MarkerSize", m_markerSize));
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::readParameter(handle, 
        "LineStyle", m_lineStyle));
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, RootStyle::readColor(handle, 
        "LineColor", m_lineColor));
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::readParameter(handle, 
        "LineWidth", m_lineWidth));
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::readParameter(handle, 
        "FillStyle", m_fillStyle));
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, RootStyle::readColor(handle, 
        "FillColor", m_fillColor));
      return STATUS_CODE_SUCCESS;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    StatusCode RootStyle::readColor(const TiXmlHandle &handle, const std::string &name, int &color) {
      std::string colorStr;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::readParameter(handle, 
        name, colorStr));
      color = RootStyle::stringToColor(colorStr);
      if(color < 0) {
        return STATUS_CODE_FAILURE;
      }
      return STATUS_CODE_SUCCESS;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    StatusCode RootStyle::builtinStyle(const std::string &theme, RootStyle &objectStyle, RootStyle &referenceStyle) {
      objectStyle = RootStyle();
      referenceStyle = RootStyle();
      if(theme == RootStyle::Theme::DEFAULT) {
        referenceStyle.m_markerColor = kRed;
        referenceStyle.m_lineColor = kRed;
        referenceStyle.m_fillColor = kRed;
        referenceStyle.m_lineStyle = 7;
        referenceStyle.m_fillStyle = 3244;
        return STATUS_CODE_SUCCESS;
      }
      else if(theme == RootStyle::Theme::THICK) {
        objectStyle.m_markerSize = 2;
        objectStyle.m_lineWidth = 2;
        referenceStyle.m_markerSize = 2;
        referenceStyle.m_lineWidth = 1;
        referenceStyle.m_markerColor = kRed;
        referenceStyle.m_lineColor = kRed;
        referenceStyle.m_fillColor = kRed;
        referenceStyle.m_lineStyle = 7;
        referenceStyle.m_fillStyle = 3244;
        return STATUS_CODE_SUCCESS;
      }
      else if(theme == RootStyle::Theme::POLAR) {
        objectStyle.m_markerSize = 2;
        objectStyle.m_lineWidth = 2;
        objectStyle.m_markerColor = kBlue;
        objectStyle.m_lineColor = kBlue;
        objectStyle.m_lineColor = kBlue;
        referenceStyle.m_markerSize = 2;
        referenceStyle.m_lineWidth = 1;
        referenceStyle.m_markerColor = kCyan+1;
        referenceStyle.m_lineColor = kCyan+1;
        referenceStyle.m_fillColor = kCyan+1;
        referenceStyle.m_lineStyle = 7;
        referenceStyle.m_fillStyle = 3244;
        return STATUS_CODE_SUCCESS;
      }
      else if(theme == RootStyle::Theme::SUNRISE) {
        objectStyle.m_markerSize = 2;
        objectStyle.m_lineWidth = 2;
        objectStyle.m_markerColor = kRed+1;
        objectStyle.m_lineColor = kRed+1;
        objectStyle.m_lineColor = kRed+1;
        referenceStyle.m_markerSize = 2;
        referenceStyle.m_lineWidth = 1;
        referenceStyle.m_markerColor = kOrange+7;
        referenceStyle.m_lineColor = kOrange+7;
        referenceStyle.m_fillColor = kOrange+7;
        referenceStyle.m_lineStyle = 7;
        referenceStyle.m_fillStyle = 3244;
        return STATUS_CODE_SUCCESS;
      }
      else if(theme == RootStyle::Theme::FOREST) {
        objectStyle.m_markerSize = 2;
        objectStyle.m_lineWidth = 2;
        objectStyle.m_markerColor = kGreen+1;
        objectStyle.m_lineColor = kGreen+1;
        objectStyle.m_lineColor = kGreen+1;
        referenceStyle.m_markerSize = 2;
        referenceStyle.m_lineWidth = 1;
        referenceStyle.m_markerColor = kOrange-7;
        referenceStyle.m_lineColor = kOrange-7;
        referenceStyle.m_fillColor = kOrange-7;
        referenceStyle.m_lineStyle = 7;
        referenceStyle.m_fillStyle = 3244;
        return STATUS_CODE_SUCCESS;
      }
      return STATUS_CODE_NOT_FOUND;
    }

  }
  
}

// not needed anymore ...
#undef ROOT_COLOR_TO_COLOR
#undef ROOT_COLOR_TABLE
