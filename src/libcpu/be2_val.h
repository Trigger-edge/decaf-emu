#pragma once
#include <common/byte_swap.h>
#include <common/type_traits.h>
#include <fmt/format.h>

template<typename Type>
class be2_val
{
public:
   static_assert(!std::is_array<Type>::value,
                 "be2_val invalid type: array");

   static_assert(!std::is_pointer<Type>::value,
                 "be2_val invalid type: pointer");

   static_assert(sizeof(Type) == 1 || sizeof(Type) == 2 || sizeof(Type) == 4 || sizeof(Type) == 8,
                 "be2_val invalid type size");

   using value_type = Type;

   be2_val() = default;

   template<typename OtherType,
            typename = typename std::enable_if<std::is_constructible<value_type, const OtherType &>::value ||
                                               std::is_convertible<const OtherType &, value_type>::value>::type>
   be2_val(const OtherType &other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { other });
      } else {
         setValue(static_cast<value_type>(other));
      }
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_constructible<value_type, const OtherType &>::value ||
                                               std::is_convertible<const OtherType &, value_type>::value>::type>
   be2_val(OtherType &&other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { std::forward<OtherType>(other) });
      } else {
         setValue(static_cast<value_type>(std::forward<OtherType>(other)));
      }
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_convertible<const OtherType &, value_type>::value ||
                                               std::is_constructible<value_type, const OtherType &>::value>::type>
   be2_val(const be2_val<OtherType> &other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { other.value() });
      } else {
         setValue(static_cast<value_type>(other.value()));
      }
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_convertible<const OtherType &, value_type>::value ||
                                               std::is_constructible<value_type, const OtherType &>::value>::type>
   be2_val(be2_val<OtherType> &&other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { other.value() });
      } else {
         setValue(static_cast<value_type>(other.value()));
      }
   }

   value_type value() const
   {
      return byte_swap(mStorage);
   }

   void setValue(value_type value)
   {
      mStorage = byte_swap(value);
   }

   operator value_type() const
   {
      return value();
   }

   template<typename T = Type,
            typename = typename std::enable_if<std::is_convertible<T, bool>::value ||
                                               std::is_constructible<bool, T>::value
                                              >::type>
   explicit operator bool() const
   {
      return static_cast<bool>(value());
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_convertible<Type, OtherType>::value ||
                                               std::is_constructible<OtherType, Type>::value ||
                                               std::is_convertible<Type, typename safe_underlying_type<OtherType>::type>::value
                                              >::type>
   explicit operator OtherType() const
   {
      return static_cast<OtherType>(value());
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_constructible<value_type, const OtherType &>::value ||
                                               std::is_convertible<const OtherType &, value_type>::value>::type>
   be2_val & operator =(const OtherType &other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { other });
      } else {
         setValue(static_cast<value_type>(other));
      }

      return *this;
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_constructible<value_type, const OtherType &>::value ||
                                               std::is_convertible<const OtherType &, value_type>::value>::type>
   be2_val & operator =(OtherType &&other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { std::forward<OtherType>(other) });
      } else {
         setValue(static_cast<value_type>(std::forward<OtherType>(other)));
      }

      return *this;
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_convertible<const OtherType &, value_type>::value ||
                                               std::is_constructible<value_type, const OtherType &>::value>::type>
   be2_val & operator =(const be2_val<OtherType> &other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { other.value() });
      } else {
         setValue(static_cast<value_type>(other.value()));
      }

      return *this;
   }

   template<typename OtherType,
            typename = typename std::enable_if<std::is_convertible<const OtherType &, value_type>::value ||
                                               std::is_constructible<value_type, const OtherType &>::value>::type>
   be2_val & operator =(be2_val<OtherType> &&other)
   {
      if constexpr (std::is_constructible<value_type, const OtherType &>::value) {
         setValue(value_type { other.value() });
      } else {
         setValue(static_cast<value_type>(other.value()));
      }

      return *this;
   }

   template<typename OtherType, typename K = value_type>
   auto operator ==(const OtherType &other)
      -> decltype(std::declval<const K>().operator ==(std::declval<const OtherType>())) const
   {
      return value() == other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator !=(const OtherType &other)
      -> decltype(std::declval<const K>().operator !=(std::declval<const OtherType>())) const
   {
      return value() != other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator >=(const OtherType &other)
      -> decltype(std::declval<const K>().operator >=(std::declval<const OtherType>())) const
   {
      return value() >= other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator <=(const OtherType &other)
      -> decltype(std::declval<const K>().operator <=(std::declval<const OtherType>())) const
   {
      return value() <= other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator >(const OtherType &other)
      -> decltype(std::declval<const K>().operator >(std::declval<const OtherType>())) const
   {
      return value() > other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator <(const OtherType &other)
      -> decltype(std::declval<const K>().operator <(std::declval<const OtherType>())) const
   {
      return value() < other;
   }

   template<typename K = value_type>
   auto operator +()
      ->  decltype(std::declval<const K>(). operator+()) const
   {
      return +value();
   }

   template<typename K = value_type>
   auto operator -()
      -> decltype(std::declval<const K>(). operator-()) const
   {
      return -value();
   }

   template<typename OtherType, typename K = value_type>
   auto operator +(const OtherType &other)
      -> decltype(std::declval<const K>().operator +(std::declval<const OtherType>())) const
   {
      return value() + other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator -(const OtherType &other)
      -> decltype(std::declval<const K>().operator -(std::declval<const OtherType>())) const
   {
      return value() - other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator *(const OtherType &other)
      -> decltype(std::declval<const K>().operator *(std::declval<const OtherType>())) const
   {
      return value() * other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator /(const OtherType &other)
      -> decltype(std::declval<const K>().operator /(std::declval<const OtherType>())) const
   {
      return value() / other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator %(const OtherType &other)
      -> decltype(std::declval<const K>().operator %(std::declval<const OtherType>())) const
   {
      return value() % other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator |(const OtherType &other)
      -> decltype(std::declval<const K>().operator |(std::declval<const OtherType>())) const
   {
      return value() | other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator &(const OtherType &other)
      -> decltype(std::declval<const K>().operator &(std::declval<const OtherType>())) const
   {
      return value() & other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator ^(const OtherType &other)
      -> decltype(std::declval<const K>().operator ^(std::declval<const OtherType>())) const
   {
      return value() ^ other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator <<(const OtherType &other)
      -> decltype(std::declval<const K>().operator <<(std::declval<const OtherType>())) const
   {
      return value() << other;
   }

   template<typename OtherType, typename K = value_type>
   auto operator >>(const OtherType &other)
      -> decltype(std::declval<const K>().operator >>(std::declval<const OtherType>())) const
   {
      return value() >> other;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() + std::declval<const OtherType>())>
   be2_val &operator +=(const OtherType &other)
   {
      *this = value() + other;
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() - std::declval<const OtherType>())>
   be2_val &operator -=(const OtherType &other)
   {
      *this = value() - other;
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() * std::declval<const OtherType>())>
   be2_val &operator *=(const OtherType &other)
   {
      *this = value() * other;
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() / std::declval<const OtherType>())>
   be2_val &operator /=(const OtherType &other)
   {
      *this = value() / other;
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() % std::declval<const OtherType>())>
   be2_val &operator %=(const OtherType &other)
   {
      *this = value() % other;
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() | std::declval<const OtherType>())>
   be2_val &operator |=(const OtherType &other)
   {
      *this = static_cast<Type>(value() | other);
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() & std::declval<const OtherType>())>
   be2_val &operator &=(const OtherType &other)
   {
      *this = static_cast<Type>(value() & other);
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() ^ std::declval<const OtherType>())>
   be2_val &operator ^=(const OtherType &other)
   {
      *this = static_cast<Type>(value() ^ other);
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() << std::declval<const OtherType>())>
   be2_val &operator <<=(const OtherType &other)
   {
      *this = value() << other;
      return *this;
   }

   template<typename OtherType,
            typename = decltype(std::declval<const value_type>() >> std::declval<const OtherType>())>
   be2_val &operator >>=(const OtherType &other)
   {
      *this = value() >> other;
      return *this;
   }

   template<typename T = Type,
            typename = decltype(std::declval<const T>() + 1)>
   be2_val &operator ++()
   {
      setValue(value() + 1);
      return *this;
   }

   template<typename T = Type,
            typename = decltype(std::declval<const T>() + 1)>
   be2_val operator ++(int)
   {
      auto before = *this;
      setValue(value() + 1);
      return before;
   }

   template<typename T = Type,
            typename = decltype(std::declval<const T>() - 1)>
   be2_val &operator --()
   {
      setValue(value() - 1);
      return *this;
   }

   template<typename T = Type,
            typename = decltype(std::declval<const T>() - 1)>
   be2_val operator --(int)
   {
      auto before = *this;
      setValue(value() - 1);
      return before;
   }

   template<typename IndexType,
            typename K = value_type>
   auto operator [](const IndexType &index)
      -> decltype(std::declval<K>().operator [](std::declval<IndexType>()))
   {
      return value().operator [](index);
   }

   template<typename K = value_type>
   auto operator ->()
      -> decltype(std::declval<K>().operator ->())
   {
      return value().operator ->();
   }

   template<typename K = value_type>
   auto operator ->() const
      -> decltype(std::declval<const K>().operator ->())
   {
      return value().operator ->();
   }

   template<typename K = value_type>
   auto operator *()
      -> decltype(std::declval<K>().operator *())
   {
      return value().operator *();
   }

   template<typename K = value_type>
   auto operator *() const
      -> decltype(std::declval<const K>().operator *())
   {
      return value().operator ->();
   }

   // Helper to access FunctionPointer::getAddress
   template<typename K = value_type>
   auto getAddress()
      -> decltype(std::declval<const K>().getAddress()) const
   {
      return value().getAddress();
   }

   // Helper to access Pointer::getRawPointer
   template<typename K = value_type>
   auto getRawPointer()
      -> decltype(std::declval<const K>().getRawPointer()) const
   {
      return value().getRawPointer();
   }

   // Please use virt_addrof or phys_addrof instead
   auto operator &() = delete;

private:
   value_type mStorage;
};

// Hijack fmtlib to forward be2_val<Type> formatter to Type formatter.
namespace fmt
{

namespace internal
{

// Ensure that fmt::internal::MakeValue sees be2_val as a custom type.
template <typename T>
struct ConvertToInt<be2_val<T>>
{
   enum
   {
      enable_conversion = false,
   };

   enum
   {
      value = false,
   };
};

} // namespace internal

template <typename ArgFormatter, typename Char, typename Type>
inline void
format_arg(BasicFormatter<Char, ArgFormatter> &f,
           const Char *&format_str,
           const be2_val<Type> &val)
{
   // Forward be2_val<Type> format to Type formatter.
   format_str = f.format(format_str,
                         internal::MakeArg<fmt::BasicFormatter<Char, ArgFormatter>>(val.value()));
}

} // namespace fmt
