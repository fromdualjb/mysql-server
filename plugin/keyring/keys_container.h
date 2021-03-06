/* Copyright (c) 2016, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#ifndef KEYS_CONTAINER_INCLUDED
#define KEYS_CONTAINER_INCLUDED

#include <my_global.h>
#include <hash.h>
#include "keyring_key.h"
#include "buffered_file_io.h"
#include "logger.h"
#include <sys_vars_shared.h> //For PolyLock, AutoWLock, AutoRLock
#include "i_keys_container.h"
#include "keyring_memory.h"

namespace keyring {

extern "C" MYSQL_PLUGIN_IMPORT CHARSET_INFO *system_charset_info;

class Keys_container : public IKeys_container
{
public:
  Keys_container(ILogger* logger);
  my_bool init(IKeyring_io* keyring_io, std::string keyring_storage_url);
  my_bool store_key(IKeyring_io *keyring_io, IKey *key);
  IKey* fetch_key(IKey *key);
  my_bool remove_key(IKeyring_io *keyring_io, IKey *key);
  std::string get_keyring_storage_url();

  ~Keys_container();

  ulong get_number_of_keys()
  {
    return keys_hash.records;
  };
protected:
  Keys_container(const Keys_container &);

  my_bool load_keys_from_keyring_storage(IKeyring_io *keyring_io);
  my_bool upload_keys_into_output_buffer(IKeyring_io *keyring_io);
  void free_keys_hash();
  IKey *get_key_from_hash(IKey *key);
  my_bool store_key_in_hash(IKey *key);
  my_bool remove_key_from_hash(IKey *key);
  virtual my_bool flush_to_backup(IKeyring_io *keyring_io);
  virtual my_bool flush_to_keyring(IKeyring_io *keyring_io);
  my_bool flush_by(IKeyring_io *keyring_io, my_bool (IKeyring_io::*flush)());

  HASH keys_hash;
  size_t memory_needed_to_flush_to_disk;
  ILogger* logger;
  std::string keyring_storage_url;

};

} //namespace keyring

#endif //KEYS_CONTAINER_INCLUDED
