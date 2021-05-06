/*! \file picrypt.h
 * \brief Cryptographic class using lib Sodium
*/
/*
    PIP - Platform Independent Primitives
    Cryptographic class using lib Sodium
	Copyright (C) 2016 Andrey Bychkov work.a.b@yandex.ru

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PICRYPT_H
#define PICRYPT_H

#include "pistring.h"

class PICrypt {
public:
	//! Construct and generate random key
	PICrypt();

	//! Set key to "key", key size must be a \a sizeKey()
	bool setKey(const PIByteArray & key);

	//! Generate and set key from keyphrase "secret"
	PIByteArray setKey(const PIString & secret);

	//! Returns current key
	PIByteArray key() {return key_;}

	//! Encrypt given data "data", result size will be increased by \a sizeCrypt()
	PIByteArray crypt(const PIByteArray & data);

	//! Decrypt given data "crypt_data"
	PIByteArray decrypt(const PIByteArray & crypt_data, bool * ok = 0);

	//! Encrypt given data "data" with key "key", result size will be increased by \a sizeCrypt()
	static PIByteArray crypt(const PIByteArray & data, PIByteArray key);

	//! Decrypt given data "crypt_data" with key "key"
	static PIByteArray decrypt(const PIByteArray & crypt_data, PIByteArray key, bool * ok = 0);

	//! Generate hash from keyphrase "secret", may be used as a key for encryption
	static PIByteArray hash(const PIString & secret);

	//! Generate short hash from string "s", may be used for hash table
	static ullong shorthash(const PIString & s, PIByteArray key = PIByteArray());
	
	//! Generate random key
	static PIByteArray generateKey();

	//! Returns key size
	static int sizeKey();

	//! Returns size which be added to data size in encryption process
	static int sizeCrypt();

private:
	PIByteArray nonce_, key_;
	
};

#endif // PICRYPT_H
