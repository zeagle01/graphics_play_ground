
#pragma once

namespace soft_render
{

	struct Event { virtual ~Event() {} };

	using base_t = Event;


	struct MousePress		:base_t { int button; };
	struct MouseRelease		:base_t { int button; };
	struct KeyPress			:base_t { int key_code; };
	struct KeyRelease		:base_t { int key_code; };
}


