#pragma once
#if 0
#define BASE_GET(var, name) [[nodiscard]] const auto& Get##name() const noexcept { return (var); }
#define BASE_SET(var, name) void Set##name(const auto& val) { (var) = val; }
#define GET(var, name) public: BASE_GET(var, name)
#define SET(var, name) public: BASE_SET(var, name)
#define PRIVATE_GET(var, name) private: BASE_GET(var, name)
#define PRIVATE_SET(var, name) private: BASE_SET(var, name)
#define PROTECTED_GET(var, name) protected: BASE_GET(var, name)
#define PROTECTED_SET(var, name) protected: BASE_SET(var, name)

#define BASE_GETM(var, name) [[nodiscard]] auto& Get##name() noexcept { return (var); }
#define BASE_SETM(var, name) void Set##name(auto& val) { (var) = val; }
#define GETM(var, name) public: BASE_GETM(var, name)
#define SETM(var, name) public: BASE_SETM(var, name)
#define PRIVATE_GETM(var, name) private: BASE_GETM(var, name)
#define PRIVATE_SETM(var, name) private: BASE_SETM(var, name)
#define PROTECTED_GETM(var, name) protected: BASE_GETM(var, name)
#define PROTECTED_SETM(var, name) protected: BASE_SETM(var, name)

#define BASE_GETC(var, name) [[nodiscard]] const auto Get##name() const noexcept { return (var); }
#define BASE_SETC(var, name) void Set##name(const auto val) { (var) = val; }
#define GETC(var, name) public: BASE_GETC(var, name)
#define SETC(var, name) public: BASE_SETC(var, name)
#define PRIVATE_GETC(var, name) private: BASE_GETC(var, name)
#define PRIVATE_SETC(var, name) private: BASE_SETC(var, name)
#define PROTECTED_GETC(var, name) protected: BASE_GETC(var, name)
#define PROTECTED_SETC(var, name) protected: BASE_SETC(var, name)

#define BASE_GETCM(var, name) [[nodiscard]] auto Get##name() noexcept { return (var); }
#define BASE_SETCM(var, name) void Set##name(auto val) { (var) = val; }
#define GETCM(var, name) public: BASE_GETCM(var, name)
#define SETCM(var, name) public: BASE_SETCM(var, name)
#define PRIVATE_GETCM(var, name) private: BASE_GETCM(var, name)
#define PRIVATE_SETCM(var, name) private: BASE_SETCM(var, name)
#define PROTECTED_GETCM(var, name) protected: BASE_GETCM(var, name)
#define PROTECTED_SETCM(var, name) protected: BASE_SETCM(var, name)

#define NONE(var, name)

#define P(decl, get, set, ...) __##decl{__VA_ARGS__}; \
get(__##decl, ##decl) \
set(__##decl, ##decl) private:
#define PU(decl, get, set) __##decl; \
get(__##decl, ##decl) \
set(__##decl, ##decl) private:
#endif
