//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

#ifndef DQM4HEP_NETBUFFER_H
#define DQM4HEP_NETBUFFER_H

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>

namespace dqm4hep {

  namespace net {

    /**
     *  @brief  NullBuffer struct
     */
    struct NullBuffer {
      static const char buffer[2]; ///< The buffer
      static const size_t size;    ///< The buffer size
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     *  @brief  RawBuffer class
     */
    class RawBuffer {
    public:
      RawBuffer(const RawBuffer &) = delete;
      RawBuffer(RawBuffer &&) = delete;
      RawBuffer &operator=(const RawBuffer &) = delete;
      RawBuffer &&operator=(RawBuffer &&) = delete;

      /**
       *  @brief  Constructor
       */
      RawBuffer();

      /**
       *  @brief  Get the buffer begin address
       */
      const char *begin() const;

      /**
       *  @brief  Get the buffer end address
       */
      const char *end() const;

      /**
       *  @brief Get the buffer size
       */
      size_t size() const;

      /**
       *  @brief  Adopt a new buffer (does not own it !)
       *
       *  @param  buffer the buffer start address
       *  @param  size the buffer size
       */
      void adopt(const char *buffer, size_t size);

    private:
      const char *m_pBuffer = {nullptr}; ///< The buffer
      size_t m_size = {0};               ///< The buffer size
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     *  @brief  BufferModel class
     */
    class BufferModel {
    public:
      /**
       *  @brief  Constructor
       */
      BufferModel() = default;

      /**
       *  @brief  Get the raw buffer
       */
      const RawBuffer &raw() const;

      /**
       *  @brief  Handle the buffer. The buffer is still owned by the user
       *
       *  @param buffer the start buffer address
       *  @param size the buffer size
       */
      void handle(const char *buffer, size_t size);

    protected:
      RawBuffer m_rawBuffer = {}; ///< The raw buffer
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     *  @brief  BufferModelT class
     */
    template <typename T>
    class BufferModelT : public BufferModel {
    public:
      /**
       *  @brief  Constructor
       */
      BufferModelT();

      /**
       *  @brief  Copy the value for further use
       *
       *  @param  value the input value to copy
       */
      void copy(const T &value);

      /**
       *  @brief  Move (std::move) the value for further use
       *
       *  @param  value the input value to move
       */
      void move(T &&value);

    private:
      T m_value = {}; ///< An internal copy of the stored value
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     *  @brief  BufferModelT class specialization (std::string)
     */
    template <>
    class BufferModelT<std::string> : public BufferModel {
    public:
      inline BufferModelT() {
        m_value.assign(NullBuffer::buffer, NullBuffer::size);
        m_rawBuffer.adopt(m_value.c_str(), m_value.size());
      }

      inline void copy(const std::string &value) {
        m_value = value;
        m_rawBuffer.adopt(m_value.c_str(), m_value.size());
      }

      inline void move(std::string &&value) {
        m_value = std::move(value);
        m_rawBuffer.adopt(m_value.c_str(), m_value.size());
      }

    private:
      std::string m_value = {""}; ///< An internal copy of the stored value as std::string
    };

    typedef std::shared_ptr<BufferModel> BufferModelPtr;

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     *  @brief  Buffer class
     */
    class Buffer {
    public:
      Buffer(const Buffer &) = delete;
      Buffer &operator=(const Buffer &) = delete;
      Buffer &&operator=(Buffer &&) = delete;

      /**
       *  @brief  Constructor
       */
      Buffer();

      /**
       *  @brief  Move constructor
       *  @param  buffer the buffer to move
       */
      Buffer(Buffer &&buffer);

      /**
       *  @brief  Factory method to create a new model
       */
      template <typename T>
      std::shared_ptr<BufferModelT<T>> createModel() const;

      /**
       *  @brief  Factory method to create a new model (std::string specialization)
       */
      std::shared_ptr<BufferModel> createModel() const;

      /**
       *  @brief  Set the new model to handle the buffer
       *
       *  @param  model the new model to handle
       */
      void setModel(std::shared_ptr<BufferModel> model);

      /**
       *  @brief  Get the start address of the buffer internally stored
       */
      const char *begin() const;

      /**
       *  @brief  Get the end address of the buffer internally stored
       */
      const char *end() const;

      /**
       *  @brief  Get the size of the buffer internally stored
       */
      size_t size() const;

      /**
       *  @brief  Adopt a new buffer. A new model is created to handle this new buffer
       *
       *  @param  buffer the start address of the new buffer to adopt
       *  @param  size the size of the new buffer to adopt
       */
      void adopt(const char *buffer, size_t size);

      /**
       *  @brief  Get the model handling the raw buffer
       */
      BufferModelPtr model() const;

    private:
      BufferModelPtr m_model = {nullptr}; ///< The buffer model handling the raw buffer
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline BufferModelT<T>::BufferModelT() {
      m_rawBuffer.adopt((const char *)&m_value, sizeof(m_value));
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void BufferModelT<T>::copy(const T &value) {
      m_value = T(value);
      m_rawBuffer.adopt((const char *)&m_value, sizeof(m_value));
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void BufferModelT<T>::move(T &&value) {
      m_value = std::move(value);
      m_rawBuffer.adopt((const char *)&m_value, sizeof(m_value));
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline std::shared_ptr<BufferModelT<T>> Buffer::createModel() const {
      return std::make_shared<BufferModelT<T>>();
    }
  }
}

#endif //  DQM4HEP_NETBUFFER_H
