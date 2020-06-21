


#define CE_ENABLE_ASSERT

#ifdef CE_ENABLE_ASSERT
#define CE_CORE_ASSERT(x, ...) { if (!x) { CE_CORE_ERROR("assert failed {0}", __VA_ARGS__); __debugbreak();} }
#else
#define CE_CORE_ASSERT(x, ...)
#endif