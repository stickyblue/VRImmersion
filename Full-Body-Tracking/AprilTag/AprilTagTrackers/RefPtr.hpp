// Defines the RefPtr<T> class, a default constructable std::reference_wrapper.

#pragma once

#include "utils/Assert.hpp"

#include <memory>
#include <optional>
#include <type_traits>

// forward declare for RefPtr conversion
template <typename T>
class OptRefPtr;

// This is almost identical to std::reference_wrapper,
// except that it has a default constructor which initializes to an invalid state (nullptr),
// and the define, ATT_ENABLE_ASSERT, will cause assertion failure
// for all other constructors and operators used with nullptr values.

/// Non-owning, never null, "reference pointer".
/// A reference pointer acts like a reference most of the time,
/// however, it allows for stl containers to store references.
template <typename T>
class RefPtr
{
public:
    static_assert(!std::is_reference_v<T>, "Cannot form pointer to reference.");

    // Friend any template instantiation.
    template <typename T2>
    friend class RefPtr;

    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;

private:
    /// Is implicit pointer conversion possible from T pointer to this pointer.
    template <typename From>
    static constexpr bool IsConv = std::is_convertible_v<std::add_pointer_t<From>, Pointer>;

public:
    // Non-owning.
    ~RefPtr() = default;

    /// Constructs into invalid state.
    constexpr RefPtr() noexcept = default;
    constexpr RefPtr(const RefPtr& other) noexcept = default;
    constexpr RefPtr(RefPtr&& other) noexcept = default;

    /// Construct with not-null raw pointer.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr(T2* _rawPtr) noexcept
        : rawPtr(_rawPtr) { ATT_ASSERT(rawPtr != nullptr); }
    /// Copy from RefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr(const RefPtr<T2>& other) noexcept
        : rawPtr(other.rawPtr) { ATT_ASSERT(rawPtr != nullptr); }
    /// Move from not-null RefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr(RefPtr<T2>&& other) noexcept
        : rawPtr(std::move(other.rawPtr)) { ATT_ASSERT(rawPtr != nullptr); }
    /// Copy from not-null OptRefPtr::Get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr(const OptRefPtr<T2>& other) noexcept
        : rawPtr(other.Get()) {}
    /// Copy from not-null std::unique_ptr::get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr(const std::unique_ptr<T2>& other) noexcept
        : rawPtr(other.get()) { ATT_ASSERT(rawPtr != nullptr); }
    /// Copy from not-nullopt or nullptr std::optional::value().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr(const std::optional<T2>& other) noexcept
        : rawPtr(other.value()) { ATT_ASSERT(rawPtr != nullptr); }

    RefPtr& operator=(const RefPtr& rhs) noexcept = default;
    RefPtr& operator=(RefPtr&& rhs) noexcept = default;

    /// Assign with not-null raw pointer.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr& operator=(T2* rhs) noexcept
    {
        rawPtr = rhs;
        ATT_ASSERT(rawPtr != nullptr);
        return *this;
    }
    /// Copy assign from RefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr& operator=(const RefPtr<T2>& rhs) noexcept
    {
        rawPtr = rhs.rawPtr;
        ATT_ASSERT(rawPtr != nullptr);
        return *this;
    }
    /// Move assign from not-null RefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr& operator=(RefPtr<T2>&& rhs) noexcept
    {
        rawPtr = rhs.rawPtr;
        ATT_ASSERT(rawPtr != nullptr);
        return *this;
    }
    /// Copy assign from not-null OptRefPtr::Get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr& operator=(const OptRefPtr<T2>& rhs) noexcept
    {
        rawPtr = rhs.Get();
        return *this;
    }
    /// Copy assign from not-null std::unique_ptr::get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr& operator=(const std::unique_ptr<T2>& rhs) noexcept
    {
        rawPtr = rhs.get();
        ATT_ASSERT(rawPtr != nullptr);
        return *this;
    }
    /// Copy assign from not-nullopt or nullptr std::optional::value().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    RefPtr& operator=(const std::optional<T2>& rhs) noexcept
    {
        rawPtr = rhs.value();
        ATT_ASSERT(rawPtr != nullptr);
        return *this;
    }

    Pointer Get() const noexcept
    {
        ATT_ASSERT(rawPtr != nullptr);
        return rawPtr;
    }

    Pointer operator->() const noexcept { return Get(); }
    Reference operator*() const noexcept { return *Get(); }
    operator Pointer() const noexcept { return Get(); }

    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    operator T2*() const noexcept { return Get(); }
    operator bool() const noexcept = delete;

    /// dynamic_cast pointer to T2 pointer
    template <typename T2>
    OptRefPtr<T2> DynamicCast() const
    {
        return dynamic_cast<T2*>(Get());
    }

private:
    Pointer rawPtr = nullptr;
};

template <typename T1, typename T2>
bool operator==(const RefPtr<T1>& lhs, const RefPtr<T2>& rhs)
{
    return lhs.Get() == rhs.Get();
}
template <typename T1, typename T2>
bool operator!=(const RefPtr<T1>& lhs, const RefPtr<T2>& rhs)
{
    return !operator==(lhs, rhs);
}

template <typename T>
bool operator==(const RefPtr<T>& lhs, const std::nullptr_t& rhs) = delete;
template <typename T>
bool operator==(const std::nullptr_t& lhs, const RefPtr<T>& rhs) = delete;

template <typename T>
bool operator!=(const RefPtr<T>& lhs, const std::nullptr_t& rhs) = delete;
template <typename T>
bool operator!=(const std::nullptr_t& lhs, const RefPtr<T>& rhs) = delete;

template <typename T>
RefPtr(std::unique_ptr<T>) -> RefPtr<T>;
template <typename T>
RefPtr(std::unique_ptr<T*>) -> RefPtr<T>;
template <typename T>
RefPtr(std::optional<T>) -> RefPtr<T>;
template <typename T>
RefPtr(std::optional<T*>) -> RefPtr<T>;
template <typename T>
RefPtr(OptRefPtr<T>) -> RefPtr<T>;
template <typename T>
RefPtr(OptRefPtr<T*>) -> RefPtr<T>;

/// Nullable non-owning pointer, like std::optional<RefPtr<T>> without the annoyances and unnecessary bool.
template <typename T>
class OptRefPtr
{
public:
    static_assert(!std::is_reference_v<T>, "Cannot form pointer to reference.");

    // Friend any template instantiation.
    template <typename T2>
    friend class OptRefPtr;

    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;

private:
    /// Is implicit pointer conversion possible from T pointer to this pointer.
    template <typename From>
    static constexpr bool IsConv = std::is_convertible_v<std::add_pointer_t<From>, Pointer>;

public:
    // Non-owning.
    ~OptRefPtr() = default;

    /// Default construct with nullptr.
    constexpr OptRefPtr() noexcept = default;
    constexpr OptRefPtr(const OptRefPtr& other) noexcept = default;
    constexpr OptRefPtr(OptRefPtr&& other) noexcept = default;

    /// Construct with raw pointer.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr(T2* _rawPtr) noexcept
        : rawPtr(_rawPtr) {}
    /// Copy from OptRefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr(const OptRefPtr<T2>& other) noexcept
        : rawPtr(other.rawPtr) {}
    /// Move from OptRefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr(OptRefPtr<T2>&& other) noexcept
        : rawPtr(std::move(other.rawPtr)) {}
    /// Copy from RefPtr::Get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr(const RefPtr<T2>& other) noexcept
        : rawPtr(other.Get()) {}
    /// Copy from std::unique_ptr::get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr(const std::unique_ptr<T2>& other) noexcept
        : rawPtr(other.get()) {}
    /// Copy from std::optional, nullopt becomes nullptr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr(const std::optional<T2>& other) noexcept
        : rawPtr(other.has_value() ? other.value() : nullptr) {}

    OptRefPtr& operator=(const OptRefPtr& rhs) noexcept = default;
    OptRefPtr& operator=(OptRefPtr&& rhs) noexcept = default;

    /// Assign with not-null raw pointer.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr& operator=(T2* rhs) noexcept
    {
        rawPtr = rhs;
        return *this;
    }
    /// Copy assign from OptRefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr& operator=(const OptRefPtr<T2>& rhs) noexcept
    {
        rawPtr = std::move(rhs.rawPtr);
        return *this;
    }
    /// Move assign from OptRefPtr.
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr& operator=(OptRefPtr<T2>&& rhs) noexcept
    {
        rawPtr = rhs.rawPtr;
        return *this;
    }
    /// Copy assign from not-null RefPtr::Get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr& operator=(const RefPtr<T2>& rhs) noexcept
    {
        rawPtr = rhs.Get();
        return *this;
    }
    /// Copy assign from std::unique_ptr::get().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr& operator=(const std::unique_ptr<T2>& rhs) noexcept
    {
        rawPtr = rhs.get();
        return *this;
    }
    /// Copy assign from std::optional::value().
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    OptRefPtr& operator=(const std::optional<T2>& rhs) noexcept
    {
        rawPtr = (rhs.has_value() ? rhs.value() : nullptr);
        return *this;
    }

    void SetNull() noexcept { rawPtr = nullptr; }
    bool IsNull() const noexcept { return rawPtr == nullptr; }
    bool NotNull() const noexcept { return rawPtr != nullptr; }
    Pointer Get() const noexcept
    {
        ATT_ASSERT(rawPtr != nullptr);
        return rawPtr;
    }

    Pointer operator->() const noexcept { return Get(); }
    Reference operator*() const noexcept { return *Get(); }
    operator Pointer() const noexcept { return Get(); }
    template <typename T2, typename = std::enable_if_t<IsConv<T2>>>
    operator T2*() const noexcept { return Get(); }
    operator bool() const noexcept { return NotNull(); }

    /// dynamic_cast pointer to T2 pointer, returns nullopt if failed
    template <typename T2>
    OptRefPtr<T2> DynamicCast() const { return dynamic_cast<T2*>(Get()); }

private:
    Pointer rawPtr = nullptr;
};

template <typename T1, typename T2>
bool operator==(const OptRefPtr<T1>& lhs, const OptRefPtr<T2>& rhs) { return lhs.rawPtr == rhs.rawPtr; }
template <typename T1, typename T2>
bool operator!=(const OptRefPtr<T1>& lhs, const OptRefPtr<T2>& rhs) { return !operator==(lhs, rhs); }

template <typename T>
OptRefPtr(std::unique_ptr<T>) -> OptRefPtr<T>;
template <typename T>
OptRefPtr(std::unique_ptr<T*>) -> OptRefPtr<T>;
template <typename T>
OptRefPtr(std::optional<T>) -> OptRefPtr<T>;
template <typename T>
OptRefPtr(std::optional<T*>) -> OptRefPtr<T>;
template <typename T>
OptRefPtr(RefPtr<T>) -> OptRefPtr<T>;
template <typename T>
OptRefPtr(RefPtr<T*>) -> OptRefPtr<T>;
