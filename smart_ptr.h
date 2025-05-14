#ifndef SMART_PTR_H_
#define SMART_PTR_H_

#include <utility>

template <typename T>
class SmartPtr {
    T* m_ptr;

public:
    SmartPtr() noexcept : m_ptr(nullptr) {}
    explicit SmartPtr(T* ptr) noexcept : m_ptr(ptr) {}

    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;

    SmartPtr(SmartPtr&& other) noexcept : m_ptr(other.m_ptr) {
        other.m_ptr = nullptr;
    }

    SmartPtr& operator=(SmartPtr&& other) noexcept {
        if (this != &other) {
            delete m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    ~SmartPtr() {
        reset();
    }

    void reset(T* ptr = nullptr) noexcept {
        delete m_ptr;
        m_ptr = ptr;
    }

    T* release() noexcept {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    T* get() const noexcept {
        return m_ptr;
    }

    T& operator*() const {
        return *m_ptr;
    }

    T* operator->() const noexcept {
        return m_ptr;
    }

    explicit operator bool() const noexcept {
        return m_ptr != nullptr;
    }

    void swap(SmartPtr& other) noexcept {
        std::swap(m_ptr, other.m_ptr);
    }
};

template <typename T>
void swap(SmartPtr<T>& lhs, SmartPtr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

#endif
