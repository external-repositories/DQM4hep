//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

#ifndef DQM4HEP_ROOTSTYLE_H
#define DQM4HEP_ROOTSTYLE_H

// -- dqm4hep headers
#include <dqm4hep/Internal.h>
#include <dqm4hep/StatusCodes.h>
#include <dqm4hep/tinyxml.h>

class TObject;

namespace dqm4hep {

  namespace core {
    
    /**
     *  @brief  RootStyle class
     *          
     * Class holding marker, line and fill attribute. </br>
     * Provide convinience methods for settings attributes to monitor element (object or reference). </br>
     * Some builtin themes are provided:
     * - "default" : plain black and dashed red
     * - "thick" : same as default with larger marker size and line width
     * - "polar" : blue-ish colors
     * - "sunrise" : red-ish colors
     * - "forest" : green-ish colors
     * 
     * To be used as:
     * @code{.cpp}
     * RootStyle objStyle, refStyle;
     * RootStyle::builtinStyle(RootStyle::Theme::SUNRISE, objStyle, refStyle);
     * @endcode 
     */
    class RootStyle {
    public:
      /**
       *  @brief  Theme class
       *          Builtin themes for customizing monitor element
       */
      class Theme {
      public:
        /// Default theme : plain black and dashed red
        static constexpr const char* DEFAULT = "default";
        /// Same as default with larger marker size and line width
        static constexpr const char* THICK = "thick";
        /// Blue-ish colors
        static constexpr const char* POLAR = "polar";
        /// Red-ish colors
        static constexpr const char* SUNRISE = "sunrise";
        /// Green-ish colors
        static constexpr const char* FOREST = "forest";
      };
      
      RootStyle() = default;
      RootStyle(const RootStyle&) = default;
      RootStyle& operator=(const RootStyle&) = default;
      ~RootStyle() = default;
      
      /**
       *  @brief  Apply current style to target object
       *  
       *  @param  object the object to apply style
       */
      void applyTo(TObject *object) const;
            
      /**
       *  @brief  Read the style properties from the xml handle
       *  
       *  @param  handle the xml input handle
       */
      StatusCode fromXml(const TiXmlHandle &handle);
      
      /**
       *  @brief  Get the ROOT color from string. Returns -1 on fail
       *  
       *  @param  colorStr the color as string (i.e "kBlack")
       */
      static int stringToColor(const std::string &colorStr);
      
      /**
       *  @brief  Get a builtin style for monitor object and reference from builtin themes
       *
       *  @param  theme the theme among the builtins
       *  @param  objectStyle the object style to receive
       *  @param  referenceStyle the reference style to receive
       */
      static StatusCode builtinStyle(const std::string &theme, RootStyle &objectStyle, RootStyle &referenceStyle);
      
    private:
      /**
       *  @brief  Read color from the xml handle 
       * 
       *  @param  handle the xml input handle
       *  @param  name the parameter name to read
       *  @param  color the color to receive
       */
      StatusCode readColor(const TiXmlHandle &handle, const std::string &name, int &color);
      
    public:
      /// The ROOT marker style
      int              m_markerStyle = {1};
      /// The ROOT marker color
      int              m_markerColor = {1};
      /// The ROOT marker size
      int              m_markerSize = {1};
      /// The ROOT line style
      int              m_lineStyle = {1};
      /// The ROOT line color
      int              m_lineColor = {1};
      /// The ROOT line width
      int              m_lineWidth = {1};
      /// The ROOT fill style
      int              m_fillStyle = {0};
      /// The ROOT fill color
      int              m_fillColor = {0};
    };

  }
}

#endif //  DQM4HEP_ROOTSTYLE_H
