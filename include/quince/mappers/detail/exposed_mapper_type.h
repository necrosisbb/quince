#ifndef QUINCE__mappers__detail__exposed_mapper_type_h
#define QUINCE__mappers__detail__exposed_mapper_type_h

/*
    Copyright 2014 Michael Shepanski

    This file is part of the quince library.

    Quince is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Quince is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with quince.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <string>
#include <vector>
#include <type_traits>
#include <quince/serial.h>
#include <quince/detail/column_type.h>
#include <quince/mappers/detail/static_mapper_type.h>


/*
    The purpose of this file is to define the alias template "exposed_mapper_type".
    All the other definitions you see are helpers for exposed_mapper_type; they are not used outside.

    For any mapped type T, exposed_mapper_type<T> is the mapper type for T that will be
    visible to application code.  There are two cases:

    1.  If T is an polymorphically mapped type (i.e. an arithmetic type, serial,
        std::string, vector<uint8_t>, or an artefact of QUINCE_DEFINE_SERVER_ONLY_TYPE)
        then exposed_mapper_type<T> is an alias of abstract_mapper<T>.
    
    2.  It T is a statically mapped type (i.e. a boost::optional, std::tuple, or
        a mapped user-defined class), then exposed_mapper_type<T> is an alias of
        static_mapper_type<T>.
*/

namespace quince {

template<typename> class abstract_mapper;

// TODO: Look for a more future-proof way to make the static decision between static and
// polymorphic mapping.
//
// What I want to say is: if static_mapper_type<T> is defined, then T is statically mapped,
// otherwise it's polymorphically mapped.  But I've wasted too much of my life trying to
// say that.

template<typename T>
class is_polymorphically_mapped : public std::integral_constant<
    bool,
        std::is_arithmetic<T>::value
    ||  std::is_same<T, serial>::value
    ||  std::is_same<T, std::string>::value
    ||  std::is_same<T, std::vector<uint8_t>>::value
    ||  std::is_empty<T>::value  // for types defined by QUINCE_DEFINE_SERVER_ONLY_TYPE
>
{};

template<typename T, typename Enable = void> struct exposed_mapper_type_trait;

template<typename T>
struct exposed_mapper_type_trait<
    T,
    typename std::enable_if<is_polymorphically_mapped<T>::value>::type
> {
    typedef abstract_mapper<T> type;
};

template<typename T>
struct exposed_mapper_type_trait<
    T,
    typename std::enable_if<! is_polymorphically_mapped<T>::value>::type
> {
    typedef static_mapper_type<T> type;
};


template<typename T>
using exposed_mapper_type = typename exposed_mapper_type_trait<T>::type;


}
#endif
