#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <atomic>
#include <type_traits>
#include <utility>

#include <unknwn.h>

#include "ComPtr.h"

template <typename TInterface, typename TCallable, typename TInvoke>
class CallbackImpl;

#if defined(_M_IX86) || defined(__i386__)
template <typename TInterface, typename TCallable, typename TResult,
          typename TClass, typename... TArgs>
class CallbackImpl<TInterface, TCallable, TResult (STDMETHODCALLTYPE TClass::*)(TArgs...)>
    final : public TInterface
{
    std::atomic<ULONG> m_referenceCount{1};
    TCallable m_callable;

public:
    explicit CallbackImpl(TCallable&& callable)
        : m_callable(std::forward<TCallable>(callable))
    {
    }

    CallbackImpl(const CallbackImpl&) = delete;
    CallbackImpl& operator=(const CallbackImpl&) = delete;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override
    {
        if (object == nullptr)
            return E_POINTER;

        *object = nullptr;

        if (riid == __uuidof(IUnknown) || riid == __uuidof(TInterface))
        {
            *object = static_cast<TInterface*>(this);
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return m_referenceCount.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    ULONG STDMETHODCALLTYPE Release() override
    {
        const ULONG referenceCount =
            m_referenceCount.fetch_sub(1, std::memory_order_acq_rel) - 1;

        if (referenceCount == 0)
            delete this;

        return referenceCount;
    }

    HRESULT STDMETHODCALLTYPE Invoke(TArgs... args) override
    {
        try
        {
            static_assert(std::is_same<
                              typename std::invoke_result<TCallable&, TArgs...>::type,
                              HRESULT>::value,
                          "The callback must return HRESULT");
            return m_callable(std::forward<TArgs>(args)...);
        }
        catch (...)
        {
            return E_FAIL;
        }
    }
};
#else
template <typename TInterface, typename TCallable, typename TResult,
          typename TClass, typename... TArgs>
class CallbackImpl<TInterface, TCallable, TResult (TClass::*)(TArgs...)>
    final : public TInterface
{
    std::atomic<ULONG> m_referenceCount{1};
    TCallable m_callable;

public:
    explicit CallbackImpl(TCallable&& callable)
        : m_callable(std::forward<TCallable>(callable))
    {
    }

    CallbackImpl(const CallbackImpl&) = delete;
    CallbackImpl& operator=(const CallbackImpl&) = delete;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override
    {
        if (object == nullptr)
            return E_POINTER;

        *object = nullptr;

        if (riid == __uuidof(IUnknown) || riid == __uuidof(TInterface))
        {
            *object = static_cast<TInterface*>(this);
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return m_referenceCount.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    ULONG STDMETHODCALLTYPE Release() override
    {
        const ULONG referenceCount =
            m_referenceCount.fetch_sub(1, std::memory_order_acq_rel) - 1;

        if (referenceCount == 0)
            delete this;

        return referenceCount;
    }

    HRESULT STDMETHODCALLTYPE Invoke(TArgs... args) override
    {
        try
        {
            static_assert(std::is_same<
                              typename std::invoke_result<TCallable&, TArgs...>::type,
                              HRESULT>::value,
                          "The callback must return HRESULT");
            return m_callable(std::forward<TArgs>(args)...);
        }
        catch (...)
        {
            return E_FAIL;
        }
    }
};
#endif

template <typename TInterface, typename TCallable>
ComPtr<TInterface> MakeCallback(TCallable&& callable)
{
    using Callable = typename std::decay<TCallable>::type;
    using Invoke = decltype(&TInterface::Invoke);
    using Implementation = CallbackImpl<TInterface, Callable, Invoke>;

    Implementation* callback = new Implementation(std::forward<TCallable>(callable));

    ComPtr<TInterface> result;
    result.Attach(static_cast<TInterface*>(callback));
    return result;
}

#endif
