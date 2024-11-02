
#pragma once

#define CLUMSY_ENUM_TAGGED_MORPHYSM(base_type,enum_type)										\
public:																							\
	template<enum_type>																			\
	struct imp;																					\
																								\
	template<enum_type>																			\
	static std::shared_ptr<base_type> create();													\


#define CLUMSY_ENUM_TAGGED_MORPHYSM_IMP(base_type,enum_value,...)								\
	template<> class base_type::imp<enum_value> : public  base_type								\
	{																							\
		__VA_ARGS__																				\
	};																							\
																								\
	template<> std::shared_ptr<base_type> base_type::create<enum_value>()						\
	{																							\
		return std::make_shared<base_type::imp<enum_value>>();									\
	}																							\


#define CLUMSY_ENUM_TAGGED_MORPHYSM_IMP_DECLARE(base_type,enum_value)							\
	template<> class base_type::imp<enum_value> : public  base_type								\


#define CLUMSY_ENUM_TAGGED_MORPHYSM_IMP_REGISTER(base_type,enum_value)							\
	template<> std::shared_ptr<base_type> base_type::create<enum_value>()						\
	{																							\
		return std::make_shared<base_type::imp<enum_value>>();									\
	}																							\
