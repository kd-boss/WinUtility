#ifndef _COMPTR_H_
#define _COMPTR_H_

#include <assert.h>
#include <sdkddkver.h>
#include <stddef.h>
#include <type_traits>
#include <unknwn.h>

template <typename T> class ComPtrRefBase
{
  public:
    typedef typename T::InterfaceType InterfaceType;

    operator IUnknown **() const throw()
    {
        static_assert(__is_base_of(IUnknown, InterfaceType),
                      "Invalid cast: InterfaceType does not derive from IUnknown");
        return reinterpret_cast<IUnknown **>(m_ptr->ReleaseAndGetAddressOf());
    }

  protected:
    T *m_ptr;
};

template <typename T> class ComPtrRef : public ComPtrRefBase<T>
{
  public:
    ComPtrRef(T *ptr) throw()
    {
        ComPtrRefBase<T>::m_ptr = ptr;
    }

    operator void **() const throw()
    {
        return reinterpret_cast<void **>(ComPtrRefBase<T>::m_ptr->ReleaseAndGetAddressOf());
    }

    operator T *() throw()
    {
        *ComPtrRefBase<T>::m_ptr = nullptr;
        return ComPtrRefBase<T>::m_ptr;
    }

    operator typename ComPtrRefBase<T>::InterfaceType **() throw()
    {
        return ComPtrRefBase<T>::m_ptr->ReleaseAndGetAddressOf();
    }

    typename ComPtrRefBase<T>::InterfaceType *operator*() throw()
    {
        return ComPtrRefBase<T>::m_ptr->Get();
    }

    typename ComPtrRefBase<T>::InterfaceType *const *GetAddressOf() const throw()
    {
        return ComPtrRefBase<T>::m_ptr->GetAddressOf();
    }

    typename ComPtrRefBase<T>::InterfaceType **ReleaseAndGetAddressOf() throw()
    {
        return ComPtrRefBase<T>::m_ptr->RelaseAndGetAddressOf();
    }
};

template <typename T> class ComPtr
{
  public:
    typedef T InterfaceType;

  protected:
    InterfaceType *ptr_;
    template <class U> friend class ComPtr;
    void InternalAddRef() const throw()
    {
        if (ptr_ != nullptr)
        {
            ptr_->AddRef();
        }
    }

    unsigned long InternalRelease() throw()
    {
        unsigned long ref = 0;
        T *tmp = ptr_;
        if (tmp != nullptr)
        {
            ptr_ = nullptr;
            ref = tmp->Release();
        }
        return ref;
    }

  public:
    ComPtr() throw() : ptr_(nullptr)
    {
    }

    ComPtr(decltype(nullptr)) throw() : ptr_(nullptr)
    {
    }

    template <class U> ComPtr(U *other) throw() : ptr_(other)
    {
        InternalAddRef();
    }

    ComPtr(const ComPtr &other) throw() : ptr_(other.ptr_)
    {
        InternalAddRef();
    }

    template <class U>
    ComPtr(const ComPtr<U> &other,
           typename std::enable_if<std::is_convertible<U *, T *>::value, void *>::type * = 0) throw()
        : ptr_(other.ptr_)
    {
        InternalAddRef();
    }
    ComPtr(ComPtr &&other) throw() : ptr_(nullptr)
    {
        if (this != reinterpret_cast<ComPtr *>(&reinterpret_cast<unsigned char &>(other)))
        {
            Swap(other);
        }
    }

    template <class U>
    ComPtr(ComPtr<U> &&other, typename std::enable_if<std::is_convertible<U *, T *>::value, void *>::type * = 0) throw()
        : ptr_(other.ptr_)
    {
        other.ptr_ = nullptr;
    }

    ~ComPtr() throw()
    {
        InternalRelease();
    }

    ComPtr &operator=(decltype(nullptr)) throw()
    {
        InternalRelease();
        return *this;
    }

    ComPtr &operator=(T *other) throw()
    {
        if (ptr_ != other)
        {
            ComPtr(other).Swap(*this);
        }
        return *this;
    }

    template <typename U> ComPtr &operator=(U *other) throw()
    {
        ComPtr(other).Swap(*this);
        return *this;
    }

    ComPtr &operator=(const ComPtr &other) throw()
    {
        if (ptr_ != other.ptr_)
        {
            ComPtr(other).Swap(*this);
        }
        return *this;
    }

    template <class U> ComPtr &operator=(ComPtr<U> &other) throw()
    {
        ComPtr(other).Swap(*this);
        return *this;
    }

    ComPtr &operator=(ComPtr &&other) throw()
    {
        ComPtr(static_cast<ComPtr &&>(other)).Swap(*this);
        return *this;
    }

    template <class U> ComPtr &operator=(ComPtr<U> &&other) throw()
    {
        ComPtr(static_cast<ComPtr<U> &&>(other)).Swap(*this);
        return *this;
    }

    void Swap(ComPtr &&r) throw()
    {
        T *tmp = ptr_;
        ptr_ = r.ptr_;
        r.ptr_ = tmp;
    }
    void Swap(ComPtr &r) throw()
    {
        T *tmp = ptr_;
        ptr_ = r.ptr_;
        r.ptr_ = tmp;
    }

    explicit operator bool() const throw()
    {
        return Get() != nullptr;
    }

    T *Get() const throw()
    {
        return ptr_;
    }

    InterfaceType *operator->() const throw()
    {
        return ptr_;
    }

    ComPtrRef<ComPtr<T>> operator&() throw()
    {
        return ComPtrRef<ComPtr<T>>(this);
    }

    const ComPtrRef<const ComPtr<T>> operator&() const throw()
    {
        return ComPtrRef<const ComPtr<T>>(this);
    }

    T *const *GetAddressOf() const throw()
    {
        return &ptr_;
    }

    T **GetAddressOf() throw()
    {
        return &ptr_;
    }

    T **ReleaseAndGetAddressOf() throw()
    {
        InternalRelease();
        return &ptr_;
    }
    T *Detach() throw()
    {
        T *ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    void Attach(InterfaceType *other) throw()
    {
        if (ptr_ != nullptr)
        {
            auto ref = ptr_->Release();
            (ref);
            ASSERT(ref != 0 || ptr_ != other);
        }
        ptr_ = other;
    }

    unsigned long Reset()
    {
        return InternalRelease();
    }

    HRESULT CopyTo(InterfaceType **ptr) const throw()
    {
        InternalAddRef();
        *ptr = ptr_;
        return S_OK;
    }

    HRESULT CopyTo(REFIID riid, void **ptr) const throw()
    {
        return ptr_->QueryInterface(riid, ptr);
    }

    template <typename U> HRESULT CopyTo(U **ptr) const throw()
    {
        return ptr_->QueryInterface(__uuidof(U), reinterpret_cast<void **>(ptr));
    }

    template <typename U> HRESULT As(ComPtr<U> *p) const throw()
    {
        return ptr_->QueryInterface(__uuidof(U), reinterpret_cast<void **>(p->ReleaseAndGetAddressOf()));
    }

    HRESULT AsIID(REFIID riid, ComPtr<IUnknown> *p) const throw()
    {
        return ptr_->QueryInterface(riid, reinterpret_cast<void **>(p->ReleaseAndGetAddressOf()));
    }
};

// ComPtr comparisons
template <class T, class U> bool operator==(const ComPtr<T> &left, const ComPtr<U> &right) throw()
{
    static_assert(__is_base_of(T, U) || __is_base_of(U, T), "'T' and 'U' pointers must be comparable");
    return left.Get() == right.Get();
}

template <class T> bool operator==(const ComPtr<T> &left, decltype(nullptr)) throw()
{
    return left.Get() == nullptr;
}

template <class T> bool operator==(decltype(nullptr), const ComPtr<T> &right) throw()
{
    return nullptr == right.Get();
}

template <class T, class U> bool operator!=(const ComPtr<T> &left, const ComPtr<U> &right) throw()
{
    static_assert(__is_base_of(T, U) || __is_base_of(U, T), "'T' and 'U' pointers must be comparable");
    return left.Get() != right.Get();
}

template <class T> bool operator!=(const ComPtr<T> &left, decltype(nullptr)) throw()
{
    return left.Get() != nullptr;
}

template <class T> bool operator!=(decltype(nullptr), const ComPtr<T> &right) throw()
{
    return nullptr != right.Get();
}

template <class T, class U> bool operator<(const ComPtr<T> &left, const ComPtr<U> &right) throw()
{
    static_assert(__is_base_of(T, U) || __is_base_of(U, T), "'T' and 'U' pointers must be comparable");
    return left.Get() < right.Get();
}

// ComPtrRef comparisons
template <class T, class U> bool operator==(const ComPtrRef<ComPtr<T>> &left, const ComPtrRef<ComPtr<U>> &right) throw()
{
    static_assert(__is_base_of(T, U) || __is_base_of(U, T), "'T' and 'U' pointers must be comparable");
    return left.GetAddressOf() == right.GetAddressOf();
}

template <class T> bool operator==(const ComPtrRef<ComPtr<T>> &left, decltype(nullptr)) throw()
{
    return left.GetAddressOf() == nullptr;
}

template <class T> bool operator==(decltype(nullptr), const ComPtrRef<ComPtr<T>> &right) throw()
{
    return nullptr == right.GetAddressOf();
}

template <class T> bool operator==(const ComPtrRef<ComPtr<T>> &left, void *b) throw()
{
    return left.GetAddressOf() == b;
}

template <class T> bool operator==(void *left, const ComPtrRef<ComPtr<T>> &right) throw()
{
    return left == right.GetAddressOf();
}

template <class T, class U> bool operator!=(const ComPtrRef<ComPtr<T>> &left, const ComPtrRef<ComPtr<U>> &right) throw()
{
    static_assert(__is_base_of(T, U) || __is_base_of(U, T), "'T' and 'U' pointers must be comparable");
    return left.GetAddressOf() != right.GetAddressOf();
}

template <class T> bool operator!=(const ComPtrRef<ComPtr<T>> &left, decltype(nullptr)) throw()
{
    return left.GetAddressOf() != nullptr;
}

template <class T> bool operator!=(decltype(nullptr), const ComPtrRef<ComPtr<T>> &right) throw()
{
    return nullptr != right.GetAddressOf();
}

template <class T> bool operator!=(const ComPtrRef<ComPtr<T>> &left, void *right) throw()
{
    return left.GetAddressOf() != right;
}

template <class T> bool operator!=(void *left, const ComPtrRef<ComPtr<T>> &right) throw()
{
    return left != right.GetAddressOf();
}

template <class T, class U> bool operator<(const ComPtrRef<ComPtr<T>> &left, const ComPtrRef<ComPtr<U>> &right) throw()
{
    static_assert(__is_base_of(T, U) || __is_base_of(U, T), "'T' and 'U' pointers must be comparable");
    return left.GetAddressOf() < right.GetAddressOf();
}
#endif
