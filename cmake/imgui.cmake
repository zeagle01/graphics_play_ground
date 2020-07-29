



function(link_to_imgui target_to_be_linked imgui_dir)

	message("calling link_to_imgui with ${target_to_be_linked} ${imgui_dir}")

	target_sources(
		${target_to_be_linked} PRIVATE
		${imgui_dir}/imconfig.h
		${imgui_dir}/imgui.cpp
		${imgui_dir}/imgui.h
		${imgui_dir}/imgui_demo.cpp
		${imgui_dir}/imgui_draw.cpp
		${imgui_dir}/imgui_internal.h
		${imgui_dir}/imgui_widgets.cpp
		${imgui_dir}/imstb_rectpack.h
		${imgui_dir}/imstb_textedit.h
		${imgui_dir}/imstb_truetype.h

		${imgui_dir}/examples/imgui_impl_glfw.h
		${imgui_dir}/examples/imgui_impl_glfw.cpp

		${imgui_dir}/examples/imgui_impl_opengl3.h
		${imgui_dir}/examples/imgui_impl_opengl3.cpp
	)

target_include_directories(${target_to_be_linked} PRIVATE ${imgui_dir})
target_include_directories(${target_to_be_linked} PRIVATE ${imgui_dir}/examples)


endfunction()
