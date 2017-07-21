// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by softboy on 5/26/17.

#ifndef ZAPI_DS_HASHTABLE_H
#define ZAPI_DS_HASHTABLE_H

#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/ds/String.h"

#include "php/Zend/zend_hash.h"
#include <string>

namespace zapi
{
namespace ds
{

using zapi::lang::Variant;

extern zapi::lang::HashTableDataDeleter zValDataDeleter;

class ZAPI_DECL_EXPORT HashTable
{
public:
   constexpr static uint32_t DEFAULT_HASH_SIZE = 8;
   using IndexType = zend_ulong;
   using HashPosition = ::HashPosition;
   enum class HashKeyType : unsigned char
   {
      String      = HASH_KEY_IS_STRING,
      Long        = HASH_KEY_IS_LONG,
      NotExistent = HASH_KEY_NON_EXISTENT
   };
   
   enum class HashActionType : unsigned char
   {
      Update         = HASH_UPDATE,
      Add            = HASH_ADD,
      UpdateIndirect = HASH_UPDATE_INDIRECT,
      AddNew         = HASH_ADD_NEW,
      AddNext        = HASH_ADD_NEXT
   };
   
   enum class HashFlagType : unsigned char
   {
      Persisent        = HASH_FLAG_PERSISTENT,
      ApplyProtection  = HASH_FLAG_APPLY_PROTECTION,
      Packed           = HASH_FLAG_PACKED,
      Initialized      = HASH_FLAG_INITIALIZED,
      StaticKeys       = HASH_FLAG_STATIC_KEYS,
      HasEmptyIndirect = HASH_FLAG_HAS_EMPTY_IND
   };
public:
   inline HashTable(uint32_t tableSize = DEFAULT_HASH_SIZE,
                    zapi::lang::HashTableDataDeleter defaultDeleter = zValDataDeleter, 
                    bool persistent = false)
   {
      zend_hash_init(&m_hashTable, tableSize, nullptr, defaultDeleter, persistent ? 1 : 0);
   }
   
   HashTable(uint32_t tableSize, bool persistent = false)
      : HashTable(tableSize, nullptr, persistent)
   {}
   
   inline HashTable(const ::HashTable &hashTable)
   {
      m_hashTable = hashTable;
      GC_REFCOUNT(&m_hashTable)++;
   }
   
   ~HashTable()
   {
      zend_hash_destroy(&m_hashTable);
   }
   
   inline uint32_t getSize() const
   {
      return zend_hash_num_elements(&m_hashTable);
   }
   
   inline bool isEmpty() const
   {
      return 0 != getSize();
   }
   
   inline HashTable &insert(const char *key, const Variant &value, bool forceNew = false)
   {
      return insert(String(key), value, forceNew);
   }
   
   inline HashTable &insert(const std::string &key, const Variant &value, bool forceNew = false)
   {
      return insert(String(key), value, forceNew);
   }
   
   inline HashTable &insert(const String &key, const Variant &value, bool forceNew = false)
   {
      if (forceNew) {
         zend_hash_add_new(&m_hashTable, key, value);
      } else {
         zend_hash_add(&m_hashTable, key, value);
      }
      return *this;
   }
   
   inline HashTable &insert(int16_t index, const Variant &value, bool forceNew = false)
   {
      index = index < 0 ? 0 : index;
      return insert(static_cast<zapi_ulong>(index), value, forceNew);
   }
   
   inline HashTable &insert(int32_t index, const Variant &value, bool forceNew = false)
   {
      index = index < 0 ? 0 : index;
      return insert(static_cast<zapi_ulong>(index), value, forceNew);
   }
   
   inline HashTable &insert(uint16_t index, const Variant &value, bool forceNew = false)
   {
      return insert(static_cast<zapi_ulong>(index), value, forceNew);
   }
   
   inline HashTable &insert(uint32_t index, const Variant &value, bool forceNew = false)
   {
      return insert(static_cast<zapi_ulong>(index), value, forceNew);
   }
   
   inline HashTable &insert(zapi_ulong index, const Variant &value, bool forceNew = false)
   {
      if (forceNew) {
         zend_hash_index_add_new(&m_hashTable, index, value);
      } else {
         zend_hash_index_add(&m_hashTable, index, value);
      }
      return *this;
   }
   
   inline HashTable &append(const Variant &value, bool forceNew = true)
   {
      if (forceNew) {
         zend_hash_next_index_insert_new(&m_hashTable, value);
      } else {
         zend_hash_next_index_insert(&m_hashTable, value);
      }
      return *this;
   }
   
   inline Variant update(const String &key, const Variant &value)
   {
      return zend_hash_update(&m_hashTable, key, value);
   }
   
   inline Variant update(zapi_ulong index, const Variant &value)
   {
      return zend_hash_index_update(&m_hashTable, index, value);
   }
   
   inline bool remove(const char *key)
   {
      return remove(String(key));
   }
   
   inline bool remove(const std::string &key)
   {
      return remove(String(key));
   }
   
   inline bool remove(const String &key)
   {
      int ret = zend_hash_del(&m_hashTable, key);
      return ret == ZAPI_SUCCESS ? true : false;
   }
   
   inline bool remove(int16_t index)
   {
      index = index < 0 ? 0 : index;
      return remove(static_cast<zapi_ulong>(index));
   }
   
   inline bool remove(int32_t index)
   {
      index = index < 0 ? 0 : index;
      return remove(static_cast<zapi_ulong>(index));
   }
   
   inline bool remove(uint16_t index)
   {
      return remove(static_cast<zapi_ulong>(index));
   }
   
   inline bool remove(uint32_t index)
   {
      return remove(static_cast<zapi_ulong>(index));
   }
   
   inline bool remove(zapi_ulong index) 
   {
      int ret = zend_hash_index_del(&m_hashTable, index);
      return ret == ZAPI_SUCCESS ? true : false;
   }
   
   inline Variant getValue(const char *key)
   {
      return getValue(String(key));
   }
   
   inline Variant getValue(const std::string &key)
   {
      return getValue(String(key));
   }
   
   inline Variant getValue(const String &key)
   {
      return zend_hash_find(&m_hashTable, key);
   }
   
   inline Variant getValue(zapi_ulong index)
   {
      return zend_hash_index_find(&m_hashTable, index);
   }
   
   inline HashTable &clear()
   {
      zend_hash_clean(&m_hashTable);
      return *this;
   }
   
   inline bool contains(const char *key)
   {
      return contains(String(key));
   }
   
   inline bool contains(const std::string &key)
   {
      return contains(String(key));
   }
   
   inline bool contains(const String &key) 
   {
      return zend_hash_exists(&m_hashTable, key) == 1 ? true : false;
   }
   
   inline bool contains(int16_t index)
   {
      index = index < 0 ? 0 : index;
      return contains(static_cast<zapi_ulong>(index));
   }
   
   inline bool contains(int32_t index)
   {
      index = index < 0 ? 0 : index;
      return contains(static_cast<zapi_ulong>(index));
   }
   
   inline bool contains(uint16_t index)
   {
      return contains(static_cast<zapi_ulong>(index));
   }
   
   inline bool contains(uint32_t index)
   {
      return contains(static_cast<zapi_ulong>(index));
   }
   
   inline bool contains(const zapi_ulong index) 
   {
      return zend_hash_index_exists(&m_hashTable, index) == 1 ? true : false;
   }
   
public:
   
   inline Variant operator[](const char *key)
   {
      return operator[](String(key));
   }
   
   inline Variant operator[](const std::string &key)
   {
      return operator[](String(key));
   }
   
   inline Variant operator[](const String &key)
   {
      zval *value = zend_hash_find(&m_hashTable, key);
      if (nullptr == value) {
         value = zend_hash_add_empty_element(&m_hashTable, key);
      }
      return Variant(value, true);
   }
   
   inline Variant operator[](int16_t index)
   {
      index = index < 0 ? 0 : index;
      return operator[](static_cast<zapi_ulong>(index));
   }
   
   inline Variant operator[](int32_t index)
   {
      index = index < 0 ? 0 : index;
      return operator[](static_cast<zapi_ulong>(index));
   }
   
   inline Variant operator[](uint16_t index)
   {
      return operator[](static_cast<zapi_ulong>(index));
   }
   
   inline Variant operator[](uint32_t index)
   {
      return operator[](static_cast<zapi_ulong>(index));
   }
   
   inline Variant operator[](zapi_ulong index)
   {
      zval *value = zend_hash_index_find(&m_hashTable, index);
      if (nullptr == value) {
         value = zend_hash_index_add_empty_element(&m_hashTable, index);
      }
      return Variant(value, true);
   }
   
   
public:
   class iterator 
   {
   public:
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef Variant value_type;
      typedef Variant *pointer;
      typedef Variant &reference;
   public:
      inline iterator() :m_index(HT_INVALID_IDX), m_hashTable(nullptr)
      {}
      
      explicit inline iterator(::HashTable *hashTable, HashPosition index) :m_index(index), m_hashTable(hashTable)
      {}
      
      inline std::string getStrKey()
      {
         zend_string *key;
         int keyType = zend_hash_get_current_key_ex(m_hashTable, &key, nullptr, &m_index);
         ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "zapi::ds::HashTable::iterator", "Current key is not exist");
         // we just copy here
         return std::string(key->val, key->len);
      }
      
      inline IndexType getNumericKey()
      {
         IndexType index;
         int keyType = zend_hash_get_current_key_ex(m_hashTable, nullptr, &index, &m_index);
         ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "zapi::ds::HashTable::iterator", "Current key is not exist");
         return index;
      }
      
      inline iterator &reset()
      {
         zend_hash_internal_pointer_reset_ex(m_hashTable, &m_index);
         return *this;
      }
      
      inline Variant getKey()
      {
         zend_string *key;
         IndexType index;
         int keyType = zend_hash_get_current_key_ex(m_hashTable, &key, &index, &m_index);
         ZAPI_ASSERT_X(keyType != HASH_KEY_NON_EXISTENT, "zapi::ds::HashTable::iterator", "Current key is not exist");
         // we just copy here
         if (keyType == HASH_KEY_IS_STRING) {
            return Variant(key->val, key->len);
         } else {
            return Variant(index);
         }
      }
      
      inline HashKeyType getKeyType()
      {
         int type = zend_hash_get_current_key_type_ex(m_hashTable, &m_index);
         return static_cast<HashKeyType>(type);
      }
      
      inline virtual Variant getValue()
      {
         return Variant(zend_hash_get_current_data_ex(m_hashTable, &m_index), true);
      }
      
   public:
      inline bool operator==(const iterator &other) const
      {
         return m_index == other.m_index;
      }
      
      inline bool operator!=(const iterator &other) const
      {
         return m_index != other.m_index;
      }
      
      inline iterator &operator++()
      {
         ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
         int result = zend_hash_move_forward_ex(m_hashTable, &m_index);
         ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating beyond end()");
         return *this;
      }
      
      inline iterator operator++(int)
      {
         iterator iter = *this;
         ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
         int result = zend_hash_move_forward_ex(m_hashTable, &m_index);
         ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating beyond end()");
         return iter;
      }
      
      inline iterator &operator--()
      {
         ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
         int result = zend_hash_move_backwards_ex(m_hashTable, &m_index);
         ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating backward beyond begin()");
         return *this;
      }
      
      inline iterator operator--(int)
      {
         iterator iter = *this;
         ZAPI_ASSERT_X(m_hashTable != nullptr, "zapi::ds::HashTable::iterator", "m_hashTable can't be nullptr");
         int result = zend_hash_move_backwards_ex(m_hashTable, &m_index);
         ZAPI_ASSERT_X(result == ZAPI_SUCCESS, "zapi::ds::HashTable::iterator", "Iterating backward beyond begin()");
         return iter;
      }
      
      inline iterator operator+(int32_t step)
      {
         iterator iter = *this;
         if (step > 0) {
            while (step--) {
               ++iter;
            }
         } else {
            while (step++) {
               --iter;
            }
         }
         return iter;
      }
      
      inline iterator operator-(int32_t step)
      {
         return operator+(-step);
      }
      
      inline iterator &operator+=(int step)
      {
         return *this = *this + step;
      }
      
      inline iterator &operator-=(int step)
      {
         return *this = *this - step;
      }
      
      inline Variant operator*()
      {
         return getValue();
      }
      
      virtual ~iterator()
      {}
      
   protected:
      /**
       * @brief current hash table index 
       */
      HashPosition m_index;
      ::HashTable *m_hashTable;
   };
   
   class const_iterator : public iterator
   {
      inline virtual Variant getValue() override
      {
         return Variant(zend_hash_get_current_data_ex(m_hashTable, &m_index));
      }
   };
   
   // STL style
   inline iterator begin()
   {
      return iterator(&m_hashTable, m_hashTable.nInternalPointer);
   }
   
   inline iterator end()
   {
      return iterator(&m_hashTable, HT_INVALID_IDX);
   }
   
private:
   ::HashTable m_hashTable;
};

} // ds
} // zapi

#endif //ZAPI_DS_HASHTABLE_H
