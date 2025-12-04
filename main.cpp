#include <cstddef>
#include <iostream>
#include <list>
#include <vector>

template<typename T>
class IsClass {
private:
    template<typename U>
    static constexpr bool helper(int U::*) {
        return true;
    }

    template<typename U>
    static constexpr bool helper(...) {
        return false;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename Base, typename Derived>
class IsDerived {
private:
    static constexpr bool helper(Base*) { return true; }
    static constexpr bool helper(...) { return false; }

public:
    static constexpr bool value = helper(static_cast<Derived*>(nullptr));
};

template<typename A, typename B>
class IsSame {
public:
    static constexpr bool value = false;
};

template<typename A>
class IsSame<A, A> {
public:
    static constexpr bool value = true;
};

template<typename T>
class IsFunction {
public:
    static constexpr bool value = false;
};

template<typename Ret, typename... Args>
class IsFunction<Ret(Args...)> {
public:
    static constexpr bool value = true;
};

template<typename T>
class IsArray {
public:
    static constexpr bool value = false;
};

template<typename T, size_t N>
class IsArray<T[N]> {
public:
    static constexpr bool value = true;
};

template<typename T>
class IsArray<T[]> {
public:
    static constexpr bool value = true;
};

template<typename T>
class RemoveExtent {
public:
    using type = T;
};

template<typename T, size_t N>
class RemoveExtent<T[N]> {
public:
    using type = T;
};

template<typename T>
class RemoveExtent<T[]> {
public:
    using type = T;
};

template<typename T>
class RemoveAllExtents {
public:
    using type = T;
};

template<typename T, size_t N>
class RemoveAllExtents<T[N]> {
public:
    using type = typename RemoveAllExtents<T>::type;
};

template<typename T>
class RemoveAllExtents<T[]> {
public:
    using type = typename RemoveAllExtents<T>::type;
};

template<typename T>
class ArrayDimensions {
public:
    static constexpr size_t value = 0;
};

template<typename T, size_t N>
class ArrayDimensions<T[N]> {
public:
    static constexpr size_t value = ArrayDimensions<T>::value + 1;
};

template<typename T>
class ArrayDimensions<T[]> {
public:
    static constexpr size_t value = ArrayDimensions<T>::value + 1;
};

template<typename T>
class IsIterable {
private:
    template<typename U>
    static constexpr auto detect(U& u)
        -> decltype(static_cast<U*>(nullptr)->begin());

    template<typename U, size_t N>
    static constexpr T* detect(U (&)[N]);

    template<typename U>
    static constexpr auto helper(int)
        -> decltype(detect(*(static_cast<U*>(nullptr))), bool()) {
        return true;
    }

    template<typename U>
    static constexpr bool helper(...) {
        return false;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename T>
class HasRandomAccessIterator {
private:
    template<typename U>
    static constexpr auto detect(U&)
        -> decltype(static_cast<U*>(nullptr)->begin());

    template<typename U, size_t N>
    static constexpr U* detect(U (&)[N]);

    template<typename U>
    static constexpr auto helper(int)
        -> decltype(detect(*(static_cast<U*>(nullptr))),
                    detect(*(static_cast<U*>(nullptr))) + 1,
                    detect (*(static_cast<U*>(nullptr)))[0],
                    bool()) {
        return true;
    }

    template<typename U>
    static constexpr bool helper(...) {
        return false;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename T>
class IsCopiable {
private:
    template<typename U>
    static constexpr auto helper(int)
        -> decltype(U(*(static_cast<T*>(nullptr))), bool()) {
        return true;
    }

    template<typename U>
    static constexpr bool helper(...) {
        return false;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename T>
class IsMovable {
private:
    template<typename U>
    static constexpr auto helper(int)
        -> decltype(U(std::move(*(static_cast<T*>(nullptr)))), bool()) {
        return true;
    }

    template<typename U>
    static constexpr auto helper(...) {
        return false;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename T>
class IsPolymorphic {
private:
    template<typename U>
    static constexpr auto helper(int)
        -> decltype(dynamic_cast<void*>(static_cast<U*>(nullptr)), bool()) {
        return true;
    }

    template<typename U>
    static constexpr auto helper(...) {
        return false;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename T>
class IsEmptyClass {
    template<typename U>
    static constexpr bool helper(int U::*) {
        struct Derived : public U {
            char padding;
        };

        return sizeof(U) == sizeof(Derived);
    }

    template<typename U>
    static constexpr bool helper(...) {
        return false;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename T>
class IsEnum {
private:
    // Remove ref
    template<typename U>
    struct RemoveRef {
        using type = U;
    };

    template<typename U>
    struct RemoveRef<U&> {
        using type = U;
    };

    template<typename U>
    struct RemoveRef<U&&> {
        using type = U;
    };

    // Is class?
    template<typename U>
    static constexpr bool detect_class(int U::*) {
        return true;
    }

    template<typename U>
    static constexpr bool detect_class(...) {
        return false;
    }

    // Is array?
    template<typename U>
    struct Detect_Array {
        static constexpr bool value = false;
    };

    template<typename U>
    struct Detect_Array<U*> {
        static constexpr bool value = true;
    };

    template<typename U>
    struct Detect_Array<U[]> {
        static constexpr bool value = true;
    };

    template<typename U, size_t N>
    struct Detect_Array<U[N]> {
        static constexpr bool value = true;
    };

    // Is fundamental?
    static constexpr bool detect_fundamental(void*) { return true; }
    static constexpr bool detect_fundamental(decltype(nullptr)*) {
        return true;
    }
    static constexpr bool detect_fundamental(signed char*) { return true; }
    static constexpr bool detect_fundamental(unsigned char*) { return true; }
    static constexpr bool detect_fundamental(short int*) { return true; }
    static constexpr bool detect_fundamental(unsigned short int*) {
        return true;
    }
    static constexpr bool detect_fundamental(int*) { return true; }
    static constexpr bool detect_fundamental(unsigned int*) { return true; }
    static constexpr bool detect_fundamental(long int*) { return true; }
    static constexpr bool detect_fundamental(unsigned long int*) {
        return true;
    }
    static constexpr bool detect_fundamental(long long int*) { return true; }
    static constexpr bool detect_fundamental(unsigned long long int*) {
        return true;
    }
    static constexpr bool detect_fundamental(char*) { return true; }
    static constexpr bool detect_fundamental(char16_t*) { return true; }
    static constexpr bool detect_fundamental(char32_t*) { return true; }
    static constexpr bool detect_fundamental(float*) { return true; }
    static constexpr bool detect_fundamental(double*) { return true; }
    static constexpr bool detect_fundamental(long double*) { return true; }
    static constexpr bool detect_fundamental(...) { return false; }

public:
    static constexpr bool value =
        !detect_class<T>(0) && !Detect_Array<T>::value &&
        !detect_fundamental(static_cast<typename RemoveRef<T>::type*>(nullptr));
};

template<typename T>
class IsAbstract {
private:
    template<typename U>
    static constexpr auto helper(int)
        -> decltype(*(static_cast<U*>(nullptr)), bool()) {
        return false;
    }

    template<typename U>
    static constexpr bool helper(...) {
        return true;
    }

public:
    static constexpr bool value = helper<T>(0);
};

template<typename T>
class Decay {
private:
    template<typename U>
    struct Helper {
        using type = U;
    };

    template<typename U>
    struct Helper<const U> {
        using type = U;
    };

    template<typename U>
    struct Helper<U[]> {
        using type = U*;
    };

    template<typename U>
    struct Helper<U&> {
        using type = U;
    };

public:
    using type = typename Helper<T>::type;
};

template<typename T>
class IsSigned {
public:
    static constexpr bool value = static_cast<T>(-1) < static_cast<T>(0);
};

// =========================================================
//                   TEST HELPERS
// =========================================================

struct Empty {};
struct NotEmpty {
    int a;
};
class Base {};
class Derived : public Base {};
struct PolyBase {
    virtual void f() {}
};
struct PolyDerived : PolyBase {};
enum MyEnum { A, B };
enum class MyEnumClass { C, D };
struct NonCopiable {
    NonCopiable(const NonCopiable&) = delete;
    NonCopiable() = default;
};
struct AbstractClass {
    virtual void f() = 0;
};
struct Functor {
    void operator()() {}
};

// =========================================================
//                       MAIN
// =========================================================

int main() {
    std::cout << std::boolalpha;    // Print true/false instead of 1/0

    std::cout << "--- 1. IsClass ---\n";
    std::cout << "int: " << IsClass<int>::value << " (Expected: false)\n";
    std::cout << "Empty: " << IsClass<Empty>::value << " (Expected: true)\n";
    std::cout << "Enum: " << IsClass<MyEnum>::value << " (Expected: false)\n";

    std::cout << "\n--- 2. IsDerived ---\n";
    std::cout << "Base->Derived: " << IsDerived<Base, Derived>::value
              << " (Expected: true)\n";
    std::cout << "Derived->Base: " << IsDerived<Derived, Base>::value
              << " (Expected: false)\n";
    std::cout << "int->int: " << IsDerived<int, int>::value
              << " (Expected: false)\n";

    std::cout << "\n--- 3. IsSame ---\n";
    std::cout << "int, int: " << IsSame<int, int>::value
              << " (Expected: true)\n";
    std::cout << "int, float: " << IsSame<int, float>::value
              << " (Expected: false)\n";

    std::cout << "\n--- 4. IsFunction ---\n";
    std::cout << "void(int): " << IsFunction<void(int)>::value
              << " (Expected: true)\n";
    std::cout << "int: " << IsFunction<int>::value << " (Expected: false)\n";
    std::cout << "Functor: " << IsFunction<Functor>::value
              << " (Expected: false)\n";

    std::cout << "\n--- 5. IsArray ---\n";
    std::cout << "int: " << IsArray<int>::value << " (Expected: false)\n";
    std::cout << "int[5]: " << IsArray<int[5]>::value << " (Expected: true)\n";
    std::cout << "int[]: " << IsArray<int[]>::value << " (Expected: true)\n";
    std::cout << "std::vector: " << IsArray<std::vector<int>>::value
              << " (Expected: false)\n";

    std::cout << "\n--- 6/7/8. Array Traits ---\n";
    using T1 = RemoveExtent<int[5][10]>::type;    // Expected: int[10]
    std::cout << "RemoveExtent<int[5][10]> is int[10]: "
              << IsSame<T1, int[10]>::value << "\n";

    using T2 = RemoveAllExtents<int[5][10]>::type;    // Expected: int
    std::cout << "RemoveAllExtents<int[5][10]> is int: "
              << IsSame<T2, int>::value << "\n";

    std::cout << "ArrayDimensions<int[5][10]>: "
              << ArrayDimensions<int[5][10]>::value << " (Expected: 2)\n";

    std::cout << "\n--- 9. IsIterable ---\n";
    std::cout << "int: " << IsIterable<int>::value << " (Expected: false)\n";
    std::cout << "int[5]: " << IsIterable<int[5]>::value
              << " (Expected: true)\n";
    std::cout << "vector<int>: " << IsIterable<std::vector<int>>::value
              << " (Expected: true)\n";

    std::cout << "\n--- 10. HasRandomAccessIterator ---\n";
    std::cout << "vector<int>: "
              << HasRandomAccessIterator<std::vector<int>>::value
              << " (Expected: true)\n";
    std::cout << "list<int>: " << HasRandomAccessIterator<std::list<int>>::value
              << " (Expected: false)\n";
    std::cout << "int[5]: " << HasRandomAccessIterator<int[5]>::value
              << " (Expected: true)\n";

    std::cout << "\n--- 11. IsCopiable ---\n";
    std::cout << "int: " << IsCopiable<int>::value << " (Expected: true)\n";
    std::cout << "NonCopiable: " << IsCopiable<NonCopiable>::value
              << " (Expected: false)\n";

    std::cout << "\n--- 12. IsMovable ---\n";
    // Assuming standard implementation where moving is usually allowed unless
    // deleted
    std::cout << "int: " << IsMovable<int>::value << " (Expected: true)\n";
    std::cout << "NonCopiable (movable): " << IsMovable<NonCopiable>::value
              << " (Expected: true)\n";

    std::cout << "\n--- 13. IsPolymorphic ---\n";
    std::cout << "Empty: " << IsPolymorphic<Empty>::value
              << " (Expected: false)\n";
    std::cout << "PolyBase: " << IsPolymorphic<PolyBase>::value
              << " (Expected: true)\n";

    std::cout << "\n--- 14. IsEmptyClass ---\n";
    std::cout << "Empty: " << IsEmptyClass<Empty>::value
              << " (Expected: true)\n";
    std::cout << "NotEmpty: " << IsEmptyClass<NotEmpty>::value
              << " (Expected: false)\n";
    std::cout << "int: " << IsEmptyClass<int>::value << " (Expected: false)\n";

    std::cout << "\n--- 15. IsEnum ---\n";
    std::cout << "MyEnum: " << IsEnum<MyEnum>::value << " (Expected: true)\n";
    std::cout << "MyEnumClass: " << IsEnum<MyEnumClass>::value
              << " (Expected: true)\n";
    std::cout << "int: " << IsEnum<int>::value << " (Expected: false)\n";
    std::cout << "Empty struct: " << IsEnum<Empty>::value
              << " (Expected: false)\n";

    std::cout << "\n--- 16. IsAbstract ---\n";
    // NOTE: The implementation provided acts more like "IsVoid".
    // For detected types, helper(int) returns false.
    // Standard IsAbstract is hard without intrinsics.
    std::cout << "int: " << IsAbstract<int>::value
              << " (Current Impl: false)\n";
    std::cout << "AbstractClass: " << IsAbstract<AbstractClass>::value
              << " (Current Impl: false)\n";
    std::cout << "void: " << IsAbstract<void>::value
              << " (Current Impl: true)\n";

    std::cout << "\n--- 17. Decay ---\n";
    using DecayedArr = Decay<int[5]>::type;
    std::cout << "Decay<int[5]> is int*: " << IsSame<DecayedArr, int*>::value
              << "\n";
    using DecayedConst = Decay<const int>::type;
    std::cout << "Decay<const int> is int: " << IsSame<DecayedConst, int>::value
              << "\n";

    std::cout << "\n--- 18. IsSigned ---\n";
    std::cout << "int: " << IsSigned<int>::value << " (Expected: true)\n";
    std::cout << "unsigned int: " << IsSigned<unsigned int>::value
              << " (Expected: false)\n";
    std::cout << "float: " << IsSigned<float>::value << " (Expected: true)\n";

    return 0;
}