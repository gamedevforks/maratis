-----------------------------------------------------------------------------------------------------------------------------
-- glfw premake build
-----------------------------------------------------------------------------------------------------------------------------

project "glfw"
	kind "StaticLib"
	language "C++"

	projectRootDir = "../../../3rdparty/glfw/"
	
	files {
		projectRootDir .. "src/clipboard.c",
		projectRootDir .. "src/context.c",
		projectRootDir .. "src/gamma.c",
		projectRootDir .. "src/init.c",
		projectRootDir .. "src/input.c",
		projectRootDir .. "src/joystick.c",
		projectRootDir .. "src/monitor.c",
		projectRootDir .. "src/time.c",
		projectRootDir .. "src/window.c"
	}
		
	if os.is("macosx") then
		files {
			projectRootDir .. "src/cocoa_platform.h",
			projectRootDir .. "src/cocoa_clipboard.m",
			projectRootDir .. "src/cocoa_gamma.c",
			projectRootDir .. "src/cocoa_init.m",
			projectRootDir .. "src/cocoa_joystick.m",
			projectRootDir .. "src/cocoa_monitor.m",
			projectRootDir .. "src/cocoa_time.c",
			projectRootDir .. "src/cocoa_window.m",
			projectRootDir .. "src/nsgl_platform.h",
			projectRootDir .. "src/nsgl_context.m"
		}
		defines { "_GLFW_COCOA", "_GLFW_NSGL" }
		
	elseif os.is("windows") then
		files {
			projectRootDir .. "src/win32_platform.h",
			projectRootDir .. "src/win32_clipboard.c",
			projectRootDir .. "src/win32_gamma.c",
			projectRootDir .. "src/win32_init.c",
			projectRootDir .. "src/win32_joystick.c",
			projectRootDir .. "src/win32_monitor.c",
			projectRootDir .. "src/win32_time.c",
			projectRootDir .. "src/win32_window.c",
			projectRootDir .. "src/wgl_platform.h",
			projectRootDir .. "src/wgl_context.c"
		}
		defines { "_GLFW_WIN32", "_GLFW_WGL" }	
		
	else
		files {
			projectRootDir .. "src/x11_platform.h",
			projectRootDir .. "src/x11_clipboard.c",
			projectRootDir .. "src/x11_gamma.c",
			projectRootDir .. "src/x11_init.c",
			projectRootDir .. "src/x11_joystick.c",
			projectRootDir .. "src/x11_monitor.c",
			projectRootDir .. "src/x11_time.c",
			projectRootDir .. "src/x11_window.c",
			projectRootDir .. "src/x11_unicode.c",
			projectRootDir .. "src/glx_platform.h",
			projectRootDir .. "src/glx_context.c"
		}
		defines { "_GLFW_X11", "_GLFW_GLX" }
		-- this should be checked depending of the system
		defines{ "_GLFW_HAS_GLXGETPROCADDRESS" }
	end

	defines { "_GLFW_USE_OPENGL", "_GLFW_USE_MENUBAR", "_GLFW_USE_CHDIR" }
