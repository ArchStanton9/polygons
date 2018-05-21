/**
  * @file assertions.hpp
  * @author Dmitri Kuvshinov kuvshinovdr at yandex.ru
  * @brief Substitutes for the standard assert macro: preconditions, postconditions, etc.
  * 
  * Level: FAST (simple checks), SLOW (complex checks), otherwise -- DEFAULT (just checks, medium level).
  * Type: precondition (REQUIRE), postcondition (ENSURE), check (ASSERT).
  * Behaviour: NDEBUG/DEBUG separately: THROW, LOG or DISABLE.
  * 
  * To set levels defined macros (as ASSERTIONS_HPP_THROW, ASSERTIONS_HPP_LOG or ASSERTIONS_HPP_DISABLE):
  * 
  * ASSERTIONS_HPP_DEBUG_FAST (default: ASSERTIONS_HPP_THROW)
  * ASSERTIONS_HPP_DEBUG_DEFAULT (default: ASSERTIONS_HPP_THROW)
  * ASSERTIONS_HPP_DEBUG_SLOW (default: ASSERTIONS_HPP_THROW)
  * ASSERTIONS_HPP_NDEBUG_FAST (default: ASSERTIONS_HPP_LOG)
  * ASSERTIONS_HPP_NDEBUG_DEFAULT (default: ASSERTIONS_HPP_DISABLE)
  * ASSERTIONS_HPP_NDEBUG_SLOW (default: ASSERTIONS_HPP_DISABLE)
  * 
  * If only ASSERTIONS_HPP_DEBUG_DEFAULT / ASSERTIONS_HPP_NDEBUG_DEFAULT are predefined then
  * its values are used for other.
  * 
  * Logging uses std::cerr by default, you may redefine it by predefining macro
  * ASSERTIONS_HPP_CERR_PUT(...) that logs all its parameters (which may be of different types) where needed.
  * By default it writes to std::cerr (and includes iostream).
  * 
  * Checks include (substitute CHECK with REQUIRE, ENSURE or ASSERT):
  *   * CHECK_MSG(c, msg), CHECK_MSG_FAST(c, msg), CHECK_MSG_SLOW(c, msg) for checking condition c to be true with custom message;
  *   * CHECK(c), CHECK_FAST(c), CHECK_SLOW(c) for checking condition c to be true;
  *   * CHECK_OP(a, op, b), CHECK_OP_FAST(a, op, b), ... for checking if ((a) op (b)).
  * 
  * */
#ifndef ASSERTIONS_HPP_INCLUDED_0Y8Q3N
#define ASSERTIONS_HPP_INCLUDED_0Y8Q3N

#define ASSERTIONS_HPP_DISABLE   0
#define ASSERTIONS_HPP_LOG       1
#define ASSERTIONS_HPP_THROW     2

#ifndef ASSERTIONS_HPP_DEBUG_DEFAULT
  #define ASSERTIONS_HPP_DEBUG_DEFAULT ASSERTIONS_HPP_THROW
#endif

#ifndef ASSERTIONS_HPP_DEBUG_FAST
  #define ASSERTIONS_HPP_DEBUG_FAST ASSERTIONS_HPP_DEBUG_DEFAULT
#endif

#ifndef ASSERTIONS_HPP_DEBUG_SLOW
  #define ASSERTIONS_HPP_DEBUG_SLOW ASSERTIONS_HPP_DEBUG_DEFAULT
#endif

#ifndef ASSERTIONS_HPP_NDEBUG_FAST
  #ifdef ASSERTIONS_HPP_NDEBUG_DEFAULT
    #define ASSERTIONS_HPP_NDEBUG_FAST ASSERTIONS_HPP_NDEBUG_DEFAULT
  #else
    #define ASSERTIONS_HPP_NDEBUG_FAST ASSERTIONS_HPP_LOG
  #endif
#endif

#ifndef ASSERTIONS_HPP_NDEBUG_SLOW
  #ifdef ASSERTIONS_HPP_NDEBUG_DEFAULT
    #define ASSERTIONS_HPP_NDEBUG_SLOW ASSERTIONS_HPP_NDEBUG_DEFAULT
  #else
    #define ASSERTIONS_HPP_NDEBUG_SLOW ASSERTIONS_HPP_DISABLE
  #endif
#endif

#ifndef ASSERTIONS_HPP_NDEBUG_DEFAULT
  #define ASSERTIONS_HPP_NDEBUG_DEFAULT ASSERTIONS_HPP_DISABLE
#endif

#ifdef NDEBUG
  #if ASSERTIONS_HPP_NDEBUG_FAST == ASSERTIONS_HPP_LOG ||\
      ASSERTIONS_HPP_NDEBUG_DEFAULT == ASSERTIONS_HPP_LOG ||\
      ASSERTIONS_HPP_NDEBUG_SLOW == ASSERTIONS_HPP_LOG
    #define ASSERTIONS_HPP_CERR_PUT_DEFINE
  #endif
#else
  #if ASSERTIONS_HPP_DEBUG_FAST == ASSERTIONS_HPP_LOG ||\
      ASSERTIONS_HPP_DEBUG_DEFAULT == ASSERTIONS_HPP_LOG ||\
      ASSERTIONS_HPP_DEBUG_SLOW == ASSERTIONS_HPP_LOG
    #define ASSERTIONS_HPP_CERR_PUT_DEFINE
  #endif
#endif

#ifdef NDEBUG
  #if ASSERTIONS_HPP_NDEBUG_FAST == ASSERTIONS_HPP_THROW ||\
      ASSERTIONS_HPP_NDEBUG_DEFAULT == ASSERTIONS_HPP_THROW ||\
      ASSERTIONS_HPP_NDEBUG_SLOW == ASSERTIONS_HPP_THROW
    #define ASSERTIONS_HPP_THROW_DEFINE
  #endif
#else
  #if ASSERTIONS_HPP_DEBUG_FAST == ASSERTIONS_HPP_THROW ||\
      ASSERTIONS_HPP_DEBUG_DEFAULT == ASSERTIONS_HPP_THROW ||\
      ASSERTIONS_HPP_DEBUG_SLOW == ASSERTIONS_HPP_THROW
    #define ASSERTIONS_HPP_THROW_DEFINE
  #endif
#endif

#if !defined(ASSERTIONS_HPP_CERR_PUT) && defined(ASSERTIONS_HPP_CERR_PUT_DEFINE) || defined(ASSERTIONS_HPP_THROW_DEFINE)
  #include <ostream>
  namespace assertions
  {
    #if __cplusplus >= 201703L // C++17.
    
    /// Send all arguments to an ostream out.
    template <class... Args>
    std::ostream& print(std::ostream &out, Args&&... args)
    {
      ((out << args) && ...);
      return out;
    }
    
    #else // still needs C++11 at least.
    
    /// Send all arguments to an ostream out.
    template <class... Args>
    std::ostream& print(std::ostream &out, Args&&... args)
    {
      const bool do_it[] { (bool)(out << args)..., true };
      (void)&do_it; // suppresses "unused variable" warning.
      return out;
    }    
    
    #endif
  }
#endif

#if !defined(ASSERTIONS_HPP_CERR_PUT) && defined(ASSERTIONS_HPP_CERR_PUT_DEFINE)
  #include <iostream> // std::cerr, print defined below.
  #define ASSERTIONS_HPP_CERR_PUT(...) assertions::print(std::cerr, __VA_ARGS__)
#endif

#ifdef ASSERTIONS_HPP_THROW_DEFINE
  #include <stdexcept>
  #include <sstream>  
  namespace assertions
  {
    /// Send all arguments to an ostringstream and return the resulting string.
    template <class... Args> inline
    std::string print_to_string(Args&&... args)
    {
      std::ostringstream out;
      print(out, std::forward<Args>(args)...);
      return out.str();
    }
    
    /// Base exception class.
    class Violation_base : public std::logic_error
    {
    public:
      // Basic constructors from a string.
      using std::logic_error::logic_error;
      /// Print a sequence of arguments to ostringstream and use the resulting string as "what".
      template <class... Args>
      explicit Violation_base(Args&&... args)
        : std::logic_error(print_to_string(std::forward<Args>(args)...)) {}
    };
    
    /// An exception thrown on precondition violation.
    class Require_violation : public Violation_base
    {
    public:
        using Violation_base::Violation_base;
    };

    /// An exception thrown on postcondition violation.
    class Ensure_violation : public Violation_base
    {
    public:
        using Violation_base::Violation_base;
    };

    /// An exception thrown on failed assert.
    class Assert_violation : public Violation_base
    {
    public:
        using Violation_base::Violation_base;
    };
  }
#endif//ASSERTIONS_HPP_THROW_DEFINE

#define ASSERTIONS_HPP_STRINGIZE(x) ASSERTIONS_HPP_STRINGIZE2(x)
#define ASSERTIONS_HPP_STRINGIZE2(x) #x

#define ASSERTIONS_HPP_CHECK_LOG(c, ...) \
  ((void)((c)? (void)0: (void)(ASSERTIONS_HPP_CERR_PUT(__VA_ARGS__))))
  
#define ASSERTIONS_HPP_REQUIRE_THROW(c, ...) \
  ((void)((c)? 0: throw assertions::Require_violation(__VA_ARGS__)))

#define ASSERTIONS_HPP_ENSURE_THROW(c, ...) \
  ((void)((c)? 0: throw assertions::Ensure_violation(__VA_ARGS__)))

#define ASSERTIONS_HPP_ASSERT_THROW(c, ...) \
  ((void)((c)? 0: throw assertions::Assert_violation(__VA_ARGS__)))


#ifdef NDEBUG

  #if ASSERTIONS_HPP_NDEBUG_FAST == ASSERTIONS_HPP_DISABLE

    #define REQUIRE_MSG_FAST(x, ...)  ((void)0)
    #define ENSURE_MSG_FAST(x, ...)   ((void)0)
    #define ASSERT_MSG_FAST(x, ...)   ((void)0)

  #elif ASSERTIONS_HPP_NDEBUG_FAST == ASSERTIONS_HPP_LOG

    #define REQUIRE_MSG_FAST(x, ...)  ASSERTIONS_HPP_CHECK_LOG(x, "Require fail: ", __VA_ARGS__, '\n')
    #define ENSURE_MSG_FAST(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Ensure fail: ", __VA_ARGS__, '\n')
    #define ASSERT_MSG_FAST(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Assert fail: ", __VA_ARGS__, '\n')

  #elif ASSERTIONS_HPP_NDEBUG_FAST == ASSERTIONS_HPP_THROW

    #define REQUIRE_MSG_FAST(x, ...)  ASSERTIONS_HPP_REQUIRE_THROW(x, __VA_ARGS__)
    #define ENSURE_MSG_FAST(x, ...)   ASSERTIONS_HPP_ENSURE_THROW(x, __VA_ARGS__)
    #define ASSERT_MSG_FAST(x, ...)   ASSERTIONS_HPP_ASSERT_THROW(x, __VA_ARGS__)

  #else
    #error Invalid ASSERTIONS_HPP_NDEBUG_FAST value.
  #endif
 
 
  #if ASSERTIONS_HPP_NDEBUG_DEFAULT == ASSERTIONS_HPP_DISABLE

    #define REQUIRE_MSG(x, ...)  ((void)0)
    #define ENSURE_MSG(x, ...)   ((void)0)
    #define ASSERT_MSG(x, ...)   ((void)0)

  #elif ASSERTIONS_HPP_NDEBUG_DEFAULT == ASSERTIONS_HPP_LOG

    #define REQUIRE_MSG(x, ...)  ASSERTIONS_HPP_CHECK_LOG(x, "Require fail: ", __VA_ARGS__, '\n')
    #define ENSURE_MSG(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Ensure fail: ", __VA_ARGS__, '\n')
    #define ASSERT_MSG(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Assert fail: ", __VA_ARGS__, '\n')

  #elif ASSERTIONS_HPP_NDEBUG_DEFAULT == ASSERTIONS_HPP_THROW

    #define REQUIRE_MSG(x, ...)  ASSERTIONS_HPP_REQUIRE_THROW(x, __VA_ARGS__)
    #define ENSURE_MSG(x, ...)   ASSERTIONS_HPP_ENSURE_THROW(x, __VA_ARGS__)
    #define ASSERT_MSG(x, ...)   ASSERTIONS_HPP_ASSERT_THROW(x, __VA_ARGS__)

  #else
    #error Invalid ASSERTIONS_HPP_NDEBUG_DEFAULT value.
  #endif 


  #if ASSERTIONS_HPP_NDEBUG_SLOW == ASSERTIONS_HPP_DISABLE

    #define REQUIRE_MSG_SLOW(x, ...)  ((void)0)
    #define ENSURE_MSG_SLOW(x, ...)   ((void)0)
    #define ASSERT_MSG_SLOW(x, ...)   ((void)0)

  #elif ASSERTIONS_HPP_NDEBUG_SLOW == ASSERTIONS_HPP_LOG

    #define REQUIRE_MSG_SLOW(x, ...)  ASSERTIONS_HPP_CHECK_LOG(x, "Require fail: ", __VA_ARGS__, '\n')
    #define ENSURE_MSG_SLOW(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Ensure fail: ", __VA_ARGS__, '\n')
    #define ASSERT_MSG_SLOW(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Assert fail: ", __VA_ARGS__, '\n')

  #elif ASSERTIONS_HPP_NDEBUG_SLOW == ASSERTIONS_HPP_THROW

    #define REQUIRE_MSG_SLOW(x, ...)  ASSERTIONS_HPP_REQUIRE_THROW(x, __VA_ARGS__)
    #define ENSURE_MSG_SLOW(x, ...)   ASSERTIONS_HPP_ENSURE_THROW(x, __VA_ARGS__)
    #define ASSERT_MSG_SLOW(x, ...)   ASSERTIONS_HPP_ASSERT_THROW(x, __VA_ARGS__)

  #else
    #error Invalid ASSERTIONS_HPP_NDEBUG_DEFAULT value.
  #endif

#else//NDEBUG

  #if ASSERTIONS_HPP_DEBUG_FAST == ASSERTIONS_HPP_DISABLE

    #define REQUIRE_MSG_FAST(x, ...)  ((void)0)
    #define ENSURE_MSG_FAST(x, ...)   ((void)0)
    #define ASSERT_MSG_FAST(x, ...)   ((void)0)

  #elif ASSERTIONS_HPP_DEBUG_FAST == ASSERTIONS_HPP_LOG

    #define REQUIRE_MSG_FAST(x, ...)  ASSERTIONS_HPP_CHECK_LOG(x, "Require fail: ", __VA_ARGS__, '\n')
    #define ENSURE_MSG_FAST(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Ensure fail: ", __VA_ARGS__, '\n')
    #define ASSERT_MSG_FAST(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Assert fail: ", __VA_ARGS__, '\n')

  #elif ASSERTIONS_HPP_DEBUG_FAST == ASSERTIONS_HPP_THROW

    #define REQUIRE_MSG_FAST(x, ...)  ASSERTIONS_HPP_REQUIRE_THROW(x, __VA_ARGS__)
    #define ENSURE_MSG_FAST(x, ...)   ASSERTIONS_HPP_ENSURE_THROW(x, __VA_ARGS__)
    #define ASSERT_MSG_FAST(x, ...)   ASSERTIONS_HPP_ASSERT_THROW(x, __VA_ARGS__)

  #else
    #error Invalid ASSERTIONS_HPP_DEBUG_FAST value.
  #endif
 
 
  #if ASSERTIONS_HPP_DEBUG_DEFAULT == ASSERTIONS_HPP_DISABLE

    #define REQUIRE_MSG(x, ...)  ((void)0)
    #define ENSURE_MSG(x, ...)   ((void)0)
    #define ASSERT_MSG(x, ...)   ((void)0)

  #elif ASSERTIONS_HPP_DEBUG_DEFAULT == ASSERTIONS_HPP_LOG

    #define REQUIRE_MSG(x, ...)  ASSERTIONS_HPP_CHECK_LOG(x, "Require fail: ", __VA_ARGS__, '\n')
    #define ENSURE_MSG(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Ensure fail: ", __VA_ARGS__, '\n')
    #define ASSERT_MSG(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Assert fail: ", __VA_ARGS__, '\n')

  #elif ASSERTIONS_HPP_DEBUG_DEFAULT == ASSERTIONS_HPP_THROW

    #define REQUIRE_MSG(x, ...)  ASSERTIONS_HPP_REQUIRE_THROW(x, __VA_ARGS__)
    #define ENSURE_MSG(x, ...)   ASSERTIONS_HPP_ENSURE_THROW(x, __VA_ARGS__)
    #define ASSERT_MSG(x, ...)   ASSERTIONS_HPP_ASSERT_THROW(x, __VA_ARGS__)

  #else
    #error Invalid ASSERTIONS_HPP_DEBUG_DEFAULT value.
  #endif 


  #if ASSERTIONS_HPP_DEBUG_SLOW == ASSERTIONS_HPP_DISABLE

    #define REQUIRE_MSG_SLOW(x, ...)  ((void)0)
    #define ENSURE_MSG_SLOW(x, ...)   ((void)0)
    #define ASSERT_MSG_SLOW(x, ...)   ((void)0)

  #elif ASSERTIONS_HPP_DEBUG_SLOW == ASSERTIONS_HPP_LOG

    #define REQUIRE_MSG_SLOW(x, ...)  ASSERTIONS_HPP_CHECK_LOG(x, "Require fail: ", __VA_ARGS__, '\n')
    #define ENSURE_MSG_SLOW(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Ensure fail: ", __VA_ARGS__, '\n')
    #define ASSERT_MSG_SLOW(x, ...)   ASSERTIONS_HPP_CHECK_LOG(x, "Assert fail: ", __VA_ARGS__, '\n')

  #elif ASSERTIONS_HPP_DEBUG_SLOW == ASSERTIONS_HPP_THROW

    #define REQUIRE_MSG_SLOW(x, ...)  ASSERTIONS_HPP_REQUIRE_THROW(x, __VA_ARGS__)
    #define ENSURE_MSG_SLOW(x, ...)   ASSERTIONS_HPP_ENSURE_THROW(x, __VA_ARGS__)
    #define ASSERT_MSG_SLOW(x, ...)   ASSERTIONS_HPP_ASSERT_THROW(x, __VA_ARGS__)

  #else
    #error Invalid ASSERTIONS_HPP_DEBUG_DEFAULT value.
  #endif

#endif//NDEBUG


#define REQUIRE_FAST(x) REQUIRE_MSG_FAST(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)
#define ENSURE_FAST(x)  ENSURE_MSG_FAST(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)
#define ASSERT_FAST(x)  ASSERT_MSG_FAST(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)

#define REQUIRE_OP_FAST(a, op, b) \
  REQUIRE_MSG_FAST(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)
#define ENSURE_OP_FAST(a, op, b) \
  ENSURE_MSG_FAST(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)
#define ASSERT_OP_FAST(a, op, b) \
  ASSERT_MSG_FAST(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)
  
#define REQUIRE(x) REQUIRE_MSG(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)
#define ENSURE(x)  ENSURE_MSG(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)
#define ASSERT(x)  ASSERT_MSG(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)

#define REQUIRE_OP(a, op, b) \
  REQUIRE_MSG(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)
#define ENSURE_OP(a, op, b) \
  ENSURE_MSG(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)
#define ASSERT_OP(a, op, b) \
  ASSERT_MSG(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)

#define REQUIRE_SLOW(x) REQUIRE_MSG_SLOW(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)
#define ENSURE_SLOW(x)  ENSURE_MSG_SLOW(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)
#define ASSERT_SLOW(x)  ASSERT_MSG_SLOW(x, __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": " #x)

#define REQUIRE_OP_SLOW(a, op, b) \
  REQUIRE_MSG_SLOW(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)
#define ENSURE_OP_SLOW(a, op, b) \
  ENSURE_MSG_SLOW(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)
#define ASSERT_OP_SLOW(a, op, b) \
  ASSERT_MSG_SLOW(((a) op (b)), __FILE__ ", " ASSERTIONS_HPP_STRINGIZE(__LINE__) ": ", #a " == ", a, " " #op " ", #b " == ", b)

#endif//ASSERTIONS_HPP_INCLUDED_0Y8Q3N
