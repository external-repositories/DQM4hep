//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

#ifndef DQM4HEP_ARCHIVER_H
#define DQM4HEP_ARCHIVER_H

// -- stl headers
#include <functional>

// -- dqm4hep headers
#include "dqm4hep/Internal.h"
#include "dqm4hep/StatusCodes.h"

class TFile;
class TDirectory;

namespace dqm4hep {

  namespace core {

    template <typename T>
    class Directory;
    template <typename T>
    class Storage;

    /** 
     *  @brief  Archiver class
     */
    class Archiver {
    public:
      Archiver(const Archiver &) = delete;
      Archiver& operator=(const Archiver &) = delete;
      
      typedef std::function<bool(MonitorElementPtr)> SelectorFunction;
      
      /**
       *  @brief  Default constructor
       */
      Archiver();

      /** 
       *  @brief  Destructor
       */
      ~Archiver();

      /** 
       *  @brief  Open a new archive.
       *          Close the current file if opened. Supported opening mode are the 
       *          TFile opening mode option (see TFile). If overwrite is set to false,
       *          an id is appended before the .root extension to ensure the file name
       *          is unique and no archive is overwritten. If the run number is positive
       *          then it is appended in the file name as _IRunNumber.
       *
       *  @param  fname the ROOT file name to open
       *  @param  opMode the ROOT file opening mode
       *  @param  overwrite whether to allow for overwrite
       *  @param  runNumber the run number to append to the archive name
       */
      StatusCode open(const std::string &fname, const std::string &opMode = "RECREATE",
                      bool overwrite = true, int runNumber = -1);

      /** 
       *  @brief  Close the current archive
       */
      StatusCode close();
      
      /**
       *  @brief  Set the selector function to use on write operation.
       *          The function should return true to archive the element.
       *          By default, every monitor element is written.
       *            
       *  @param  func the selector function
       */
      void setSelectorFunction(std::function<bool(MonitorElementPtr)> func);

      /** 
       *  @brief  Archive the monitor element storage in the ROOT file.
       *          The result can be written in a specific directory using the dirName argument.
       *          By default, the content is written directly in the top-level directory
       *
       *  @param  storage the storage to archive
       *  @param  dirName the directory in which to archive the storage (optional)
       */
      StatusCode archive(const Storage<MonitorElement> &storage, const std::string &dirName = "");
      
      /** 
       *  @brief  Archive the monitor element storage in the ROOT file.
       *          Also archive the references along with the monitor object in the same directory.
       *          The reference is written with the same name as the object plus a suffix (default "_ref").
       *          The result can be written in a specific directory using the dirName argument.
       *          By default, the content is written directly in the top-level directory.
       *
       *  @param  storage the storage to archive
       *  @param  dirName the directory in which to archive the storage (optional)
       *  @param  refSuffix the reference name suffix (optional)
       */
      StatusCode archiveWithReferences(const Storage<MonitorElement> &storage, const std::string &dirName = "", const std::string &refSuffix = "_ref");

      /** 
       *  @brief  Get the file name
       */
      const std::string &fileName() const;

      /** 
       *  @brief  Whether the archive is opened
       */
      bool isOpened() const;

      /** 
       *  @brief  Get the opening mode of this archive
       */
      const std::string &openingMode() const;

    private:
      /**
       *  @brief  Prepare the ROOT file for archiving
       *   
       *  @param  dirName the top-level root directory in which the elements will be archived
       *  @param  directory the top-level directory pointer to receive
       */
      StatusCode prepareForArchiving(const std::string &dirName, TDirectory *&directory);
      
      /** 
       *  @brief  Fill recursively the TDirectory with the Directory
       *
       *  @param  directory the monitor element storage directory
       *  @param  rootDirectory the corresponding ROOT directory
       *  @param  refSuffix the suffix to add for object references
       */
      StatusCode recursiveWrite(MonitorElementDir directory, TDirectory *rootDirectory, const std::string &refSuffix = "");

      /** 
       *  @brief  Write the monitor elements contained in the Directory in the TDirectory
       *
       *  @param  directory the monitor element storage directory
       *  @param  rootDirectory the corresponding ROOT directory
       *  @param  refSuffix the suffix to add for object references
       */
      StatusCode writeMonitorElements(MonitorElementDir directory, TDirectory *rootDirectory, const std::string &refSuffix = "");

    private:
      /// The archive file name
      std::string                    m_fileName = {""};
      /// The root file opening mode
      std::string                    m_openingMode = {"RECREATE"};
      ///< Whether the archive is opened
      bool                           m_isOpened = {false};
      /// The selector function
      SelectorFunction               m_selectorFunction = {nullptr};
      /// The actual archive implementation (root file)
      std::unique_ptr<TFile>         m_file = {nullptr};
    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    /**
     *  @brief  ArchiverSelector class
     *          Helper class to select monitor elements while archiving
     *          It is a combinaison of selector functions.
     *          Returns true if at least one function returns true, otherwise false. 
     */
    class ArchiverSelector {
    public:
      /**
       *  @brief  Constructor
       */
      ArchiverSelector();

      /**
       *  @brief  Add a selector function
       *  
       *  @param selector [description]
       */
      void addSelector(Archiver::SelectorFunction selector);
      
      /**
       *  @brief  Get the global function combining all selector functions
       */
      const Archiver::SelectorFunction &function() const;
      
    private:
      /// The global selector function
      Archiver::SelectorFunction                 m_function = {};
      /// The list of selector functions
      std::vector<Archiver::SelectorFunction>    m_selectorFunctions = {};
    };
  }
}

#endif //  DQM4HEP_ARCHIVER_H
