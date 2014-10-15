#ifndef __SYSTEM_UTILITY___
#define __SYSTEM_UTILITY___
#ifndef  __SYSTEM__
#error System.h must be included before System.Utility.h
#endif


//#include <wrl.h>
#include <ctype.h>
#include <cwctype>
#include <algorithm>
#include <memory>
#include <utility>
#include <iterator>
#include <type_traits>
#include <cstddef>
/* 
namespace std {
    template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
        typename _Unique_if<T>::_Single_object
        make_unique(Args&&... args) {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    template<class T>
        typename _Unique_if<T>::_Unknown_bound
        make_unique(size_t n) {
            typedef typename remove_extent<T>::type U;
            return unique_ptr<T>(new U[n]());
        }

    template<class T, class... Args>
        typename _Unique_if<T>::_Known_bound
        make_unique(Args&&...) = delete;
} */
namespace System
{
	namespace Utility
	{

		template <typename Traits>
		class unique_handle
		{
			typedef typename Traits::pointer pointer;
			
			pointer m_value;
			unique_handle(unique_handle const&);
			auto operator=(unique_handle const&) -> unique_handle;
			auto close() throw() -> void
			{
				if (*this)
				{
					Traits::close(m_value);
				}
			}
		public:

			unique_handle(unique_handle && other) throw():
				m_value{ other.release() }
			{

			}

			auto swap(unique_handle<Traits> & other) throw() -> void
			{
				std::swap(m_value, other.m_value);
			}
		
			auto operator=(unique_handle && other) throw() -> unique_handle
			{
				if (this != &other)
				{
					reset(other.release());
				}
				return *this;
			}

			auto get() const throw() -> pointer
			{
				return m_value;
			}

			auto release() throw() -> pointer
			{
				auto value = m_value;
				m_value = Traits::invalid();
				return value;
			}

			auto reset(pointer value = Traits::invalid()) throw() -> bool
			{
				if (m_value != value)
				{
					close();
					m_value = value;
				}
				return static_cast<bool>(*this);
			}

			explicit operator bool() const throw()
			{
				return m_value != Traits::invalid();
			}

			explicit unique_handle(pointer value = Traits::invalid()) throw() :
				m_value{value}
			{
			}

			~unique_handle() throw()
			{
				close();
			}
		};

		template <typename Traits>
		auto operator==(unique_handle<Traits> const & left,
			unique_handle<Traits> const & right) throw() -> bool
		{
			return left.get() == right.get();
		}
		
		template <typename Traits>
		auto operator!=(unique_handle<Traits> const & left,
			unique_handle<Traits> const & right) throw() -> bool
		{
			return left.get() < right.get();
		}

		template <typename Traits>
		auto operator>=(unique_handle<Traits> const & left,
			unique_handle<Traits> const & right) throw() -> bool
		{
			return left.get() >= right.get();
		}

		template <typename Traits>
		auto operator>(unique_handle<Traits> const & left,
			unique_handle<Traits> const & right) throw() -> bool
		{
			return left.get() > right.get();
		}

		template <typename Traits>
		auto operator<=(unique_handle<Traits> const & left,
			unique_handle<Traits> const & right) throw() -> bool
		{
			return left.get() <= right.get();
		}

		template <typename Traits>
		auto operator<(unique_handle<Traits> const & left,
					   unique_handle<Traits> const & right) throw() -> bool
		{
			return left.get() < right.get();
		}

		template <typename Traits>
		auto swap(unique_handle<Traits> & left,
					unique_handle<Traits> & right) throw() ->void
		{
			left.swap(right);
		}

		struct null_handle_traits
		{
			typedef HANDLE pointer;
			static auto invalid() throw() -> pointer
			{
				return nullptr;
			}
			static auto close(pointer value) throw() -> void
			{
				//wrap in verify
				CloseHandle(value);
			}
		};
		typedef unique_handle<null_handle_traits> null_handle;
		struct invalid_handle_traits
		{
			typedef HANDLE pointer;

			static auto invalid() throw() -> pointer
			{
				return INVALID_HANDLE_VALUE;
			}

			static auto close(pointer value) throw() ->void
			{
				CloseHandle(value);
			}
		};
		typedef unique_handle<invalid_handle_traits> invalid_handle;
		
		struct map_view_deleter
		{
			typedef char const * pointer;

			auto operator()(pointer value) const throw() -> void
			{
				VERIFY(UnmapViewOfFile(value));
			}
		};
		
		auto trim(std::string const & s)-> std::string
		{
			
			auto front = std::find_if_not(std::begin(s), std::end(s), isspace);
            auto back = std::find_if_not(s.rbegin(), s.rend(), isspace);
			return std::string{ front, back.base() };
		}
		
		auto trim(std::wstring const & s)-> std::wstring
		{
			auto front = std::find_if_not(std::begin(s), std::end(s), iswspace);
            auto back = std::find_if_not(s.rbegin(), s.rend(), iswspace);
			return std::wstring{ front, back.base() };
		}

		/* struct MemMappedFile
		{
			
			null_handle m_map;
			LARGE_INTEGER m_size;
			std::unique_ptr<char> m_last;
			std::unique_ptr<char,map_view_deleter> m_first;
			MemMappedFile(MemMappedFile const& other) = delete;
			MemMappedFile& operator=(MemMappedFile const & other) = delete;
			MemMappedFile()
			{}
			~MemMappedFile()
			{
				m_map.reset();
				m_last.release();
				m_first.reset();
			}
			MemMappedFile(MemMappedFile&& other) : m_size(other.m_size)
			{
				m_map = std::move(other.m_map);
				m_last = std::move(other.m_last);
				m_first = std::move(other.m_first);
			}
            MemMappedFile* operator=(MemMappedFile&& other)
			{
				if (this != &other)
				{
					m_map = std::move(other.m_map);
					m_last = std::move(other.m_last);
					m_first = std::move(other.m_first);
					m_size = other.m_size;
				}
                return this;
			}
			auto swap(MemMappedFile& other) throw() -> void
			{
				std::swap(m_map, other.m_map);
				std::swap(m_size, other.m_size);
				std::swap(m_last, other.m_last);
				std::swap(m_first, other.m_first);
			}
			auto open(std::wstring filename) -> bool
			{
				auto file = unique_handle<invalid_handle_traits>
				{
					CreateFile(filename.c_str(),
					GENERIC_READ,
					FILE_SHARE_READ,
					nullptr,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					nullptr)
				};
				if (file.operator bool())
				{

					m_size = {};
					if (!GetFileSizeEx(file.get(), &m_size))
					{
						return false;
					}
					if (m_size.QuadPart == 0)
					{
						return false;
					}
					auto map = unique_handle<null_handle_traits>
					{
						CreateFileMapping(file.get(),
							nullptr,
							PAGE_READONLY,
							0, 0,
							nullptr)
					};
					
					if (!map)
					{
						return false;
					}

					file.reset();

					auto view = std::move(std::unique_ptr<char, map_view_deleter>
					{
						static_cast<char *>(MapViewOfFile(map.get(),
							FILE_MAP_READ,
							0, 0, 0))
					});

					if (view)
					{
						m_first = std::move(view);
						m_last = std::move(std::unique_ptr<char>
						{
							const_cast<char*>(m_first.get() + static_cast<unsigned>(m_size.QuadPart))
						});
					}
					else
					{
						return false;
					}
				}
				return true;
				
			}
			auto begin() throw() ->char*
			{
				auto p = m_first.get();
				return const_cast<char*>(p);
			}
			auto end() throw() -> char*
			{
				auto p = m_last.get();
				return p;
			}
			auto size() throw()-> long long
			{
				return m_size.QuadPart;
			}
		}; */

	};


};

#endif
