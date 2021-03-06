//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

#ifndef DQM4HEP_STORAGE_H
#define DQM4HEP_STORAGE_H

// -- dqm4hep headers
#include <dqm4hep/Directory.h>
#include <dqm4hep/Internal.h>
#include <dqm4hep/StatusCodes.h>

namespace dqm4hep {

  namespace core {

    template <typename T>
    class Storage {
    public:
      typedef typename Directory<T>::DirectoryPtr DirectoryPtr;
      typedef typename Directory<T>::ObjectPtr ObjectPtr;
      typedef typename Directory<T>::ObjectList ObjectList;
      typedef typename Directory<T>::DirectoryList DirectoryList;

      Storage();
      ~Storage();
      StatusCode mkdir(const std::string &dirName);
      void cd();
      StatusCode cd(const std::string &dirName);
      bool dirExists(const std::string &dirName) const;
      const std::string &pwd() const;
      StatusCode goUp();
      StatusCode rmdir(const std::string &dirName);
      StatusCode find(const std::string &dirName, DirectoryPtr &directory) const;
      DirectoryPtr root() const;
      DirectoryPtr current() const;
      StatusCode add(ObjectPtr object);
      StatusCode add(const std::string &dirName, ObjectPtr object);
      StatusCode add(ObjectPtr object, std::string &fullPath);
      StatusCode add(const std::string &dirName, ObjectPtr object, std::string &fullPath);
      template <typename F>
      StatusCode remove(F function);
      template <typename F>
      StatusCode remove(const std::string &dirName, F function);
      template <typename F>
      ObjectPtr findObject(F function) const;
      template <typename F>
      ObjectPtr findObject(const std::string &dirName, F function) const;
      bool containsObject(const ObjectPtr &object) const;
      bool containsObject(const std::string &dirName, const ObjectPtr &object) const;
      template <typename F>
      void iterate(F function) const;
      template <typename U>
      void getObjects(std::vector<std::shared_ptr<U>> &objectList) const;
      void clear();
      template <typename F>
      void dump(F function) const;

    private:
      template <typename F>
      bool iterate(const DirectoryPtr &directory, F function) const;
      
      template <typename F>
      void dump(const DirectoryPtr &directory, F function) const;

    private:
      DirectoryPtr m_rootDirectory;
      DirectoryPtr m_currentDirectory;
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Storage<T>::Storage() : m_rootDirectory(Directory<T>::make_shared("")), m_currentDirectory(m_rootDirectory) {
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Storage<T>::~Storage() = default;

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline StatusCode Storage<T>::mkdir(const std::string &dirName) {
      if (dirName.empty())
        return STATUS_CODE_INVALID_PARAMETER;

      Path path(dirName);

      if (!path.isValid())
        return STATUS_CODE_INVALID_PARAMETER;

      DirectoryPtr directory = !path.isRelative() ? m_rootDirectory : m_currentDirectory;
      StringVector directoryList = path.getSplitPath();

      for (auto iter = directoryList.begin(), endIter = directoryList.end(); endIter != iter; ++iter) {
        std::string dname = *iter;

        if (dname == ".")
          continue;

        if (dname == "..") {
          if (directory == m_rootDirectory)
            return STATUS_CODE_FAILURE;

          directory = directory->parent();
          continue;
        }

        // if sub dir doesn't exists, create it
        if (!directory->hasChild(dname))
          directory->mkdir(dname);

        // navigate forward
        RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, directory->find(dname, directory));
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void Storage<T>::cd() {
      m_currentDirectory = m_rootDirectory;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline StatusCode Storage<T>::cd(const std::string &dirName) {
      // go back to sub dir
      if (dirName.empty()) {
        this->cd();
        return STATUS_CODE_SUCCESS;
      }

      Path path(dirName);

      if (!path.isValid())
        return STATUS_CODE_INVALID_PARAMETER;

      DirectoryPtr directory = !path.isRelative() ? m_rootDirectory : m_currentDirectory;
      StringVector directoryList = path.getSplitPath();

      for (auto iter = directoryList.begin(), endIter = directoryList.end(); endIter != iter; ++iter) {
        std::string dname = *iter;

        if (dname == ".")
          continue;

        if (dname == "..") {
          if (directory == m_rootDirectory)
            return STATUS_CODE_FAILURE;

          directory = directory->parent();
          continue;
        }

        // navigate forward
        RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, directory->find(dname, directory));
      }

      if (nullptr == directory)
        return STATUS_CODE_FAILURE;

      m_currentDirectory = directory;

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Storage<T>::dirExists(const std::string &dirName) const {
      DirectoryPtr directory = nullptr;
      return (this->find(dirName, directory) == STATUS_CODE_SUCCESS);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline const std::string &Storage<T>::pwd() const {
      return m_currentDirectory->name();
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline StatusCode Storage<T>::goUp() {
      if (m_currentDirectory->isRoot())
        return STATUS_CODE_UNCHANGED;

      m_currentDirectory = m_currentDirectory->parent();

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline StatusCode Storage<T>::rmdir(const std::string &dirName) {
      if (dirName.empty())
        return STATUS_CODE_NOT_ALLOWED;

      DirectoryPtr directory = nullptr;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->find(dirName, directory));

      if (directory == m_rootDirectory)
        return STATUS_CODE_NOT_ALLOWED;

      std::string fullPathDirName = directory->fullPath().getPath();
      std::string currentFullPathDirName = m_currentDirectory->fullPath().getPath();
      size_t pos = currentFullPathDirName.find(fullPathDirName);

      // this mean that the directory that we try
      // to remove is a parent of the current one.
      if (pos == 0 || pos != std::string::npos)
        return STATUS_CODE_FAILURE;

      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, directory->parent()->rmdir(directory->name()));

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline StatusCode Storage<T>::find(const std::string &dirName, DirectoryPtr &directory) const {
      directory = nullptr;

      // go back to sub dir
      if (dirName.empty() || dirName == "." || dirName == "./") {
        directory = m_currentDirectory;
        return STATUS_CODE_SUCCESS;
      }

      Path path(dirName);

      if (!path.isValid())
        return STATUS_CODE_INVALID_PARAMETER;

      directory = !path.isRelative() ? m_rootDirectory : m_currentDirectory;
      StringVector directoryList = path.getSplitPath();

      for (auto iter = directoryList.begin(), endIter = directoryList.end(); endIter != iter; ++iter) {
        std::string dname = *iter;

        if (dname == ".")
          continue;

        if (dname == "..") {
          if (directory == m_rootDirectory)
            return STATUS_CODE_FAILURE;

          directory = directory->parent();
          continue;
        }

        // navigate forward
        StatusCode statusCode = directory->find(dname, directory);

        if (statusCode != STATUS_CODE_SUCCESS)
          return statusCode;
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline typename Storage<T>::DirectoryPtr Storage<T>::root() const {
      return m_rootDirectory;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline typename Storage<T>::DirectoryPtr Storage<T>::current() const {
      return m_currentDirectory;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline StatusCode Storage<T>::add(ObjectPtr object) {
      return m_currentDirectory->add(object);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline StatusCode Storage<T>::add(const std::string &dirName, ObjectPtr object) {
      DirectoryPtr directory = nullptr;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->mkdir(dirName));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->find(dirName, directory));
      return directory->add(object);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    template <typename T>
    inline StatusCode Storage<T>::add(ObjectPtr object, std::string &fullPath) {
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_currentDirectory->add(object));
      fullPath = m_currentDirectory->fullPath().getPath();
      return STATUS_CODE_SUCCESS;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    template <typename T>
    inline StatusCode Storage<T>::add(const std::string &dirName, ObjectPtr object, std::string &fullPath) {
      DirectoryPtr directory = nullptr;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->mkdir(dirName));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->find(dirName, directory));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, directory->add(object));
      fullPath = directory->fullPath().getPath();
      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    template <typename F>
    inline StatusCode Storage<T>::remove(F function) {
      return m_currentDirectory->remove(function);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    template <typename F>
    inline StatusCode Storage<T>::remove(const std::string &dirName, F function) {
      DirectoryPtr directory = nullptr;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->find(dirName, directory));
      return directory->remove(function);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    template <typename F>
    inline typename Storage<T>::ObjectPtr Storage<T>::findObject(F function) const {
      return m_currentDirectory->find(function);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    template <typename F>
    inline typename Storage<T>::ObjectPtr Storage<T>::findObject(const std::string &dirName, F function) const {
      DirectoryPtr directory;

      if (this->find(dirName, directory))
        return nullptr;

      return directory->find(function);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Storage<T>::containsObject(const ObjectPtr &object) const {
      return m_currentDirectory->containsObject(object);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Storage<T>::containsObject(const std::string &dirName, const ObjectPtr &object) const {
      DirectoryPtr directory = NULL;

      if (this->find(dirName, directory))
        return false;

      return directory->containsObject(object);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    template <typename F>
    inline bool Storage<T>::iterate(const DirectoryPtr &directory, F function) const {
      auto contents(directory->contents());

      for (auto &obj : contents)
        if (!function(directory, obj))
          return false;

      auto subdirs(directory->subdirs());

      for (const auto &dir : subdirs)
        if (!this->iterate(dir, function))
          return false;

      return true;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    template <typename F>
    inline void Storage<T>::iterate(F function) const {
      this->iterate(m_rootDirectory, function);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    template <typename U>
    inline void Storage<T>::getObjects(std::vector<std::shared_ptr<U>> &objectList) const {
      this->iterate([&](const DirectoryPtr &/*directory*/, ObjectPtr object) {
        objectList.push_back(std::dynamic_pointer_cast<U>(object));
        return true;
      });
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void Storage<T>::clear() {
      m_rootDirectory->clear();
      m_currentDirectory = m_rootDirectory;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    template <typename T>
    template <typename F>
    inline void Storage<T>::dump(F function) const {
      dump(m_rootDirectory, function);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    template <typename T>
    template <typename F>
    inline void Storage<T>::dump(const DirectoryPtr &directory, F function) const {
      int nParents = directory->nParents();
      dqm_info( "{0}+ {1}", std::string(nParents*2, ' '), directory->isRoot() ? "/" : directory->name()+"/" );
      for(auto obj : directory->contents()) {
        std::string objectStr = function(obj);
        dqm_info( "{0}|- {1}", std::string(nParents*2, ' '), objectStr );
      }
      for(auto dir : directory->subdirs()) {
        dump(dir, function);
      }
    }
  }
}

#endif //  DQM4HEP_STORAGE_H
