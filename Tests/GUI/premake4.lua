-----------------------------------------------------------------------------------------------------------------------------
-- Test premake build
-----------------------------------------------------------------------------------------------------------------------------

rootDir = "../../"
sourcesDir = rootDir .. "Sources/"
thirdPartyDir = rootDir .. "3rdparty/"


solution "Test"

	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }

	if os.is("windows") then
		defines { "_WIN32" }
		defines { "WIN32" }
	elseif os.is("macosx") then
		defines { "__APPLE__" }
	elseif os.is("linux") then
		defines { "LINUX" }
	end

	includedirs { 
		thirdPartyDir .. "tinycthread/source/",
		thirdPartyDir .. "glee/",
		thirdPartyDir .. "glfw/include/",
		thirdPartyDir .. "freeimage/Source/",
		thirdPartyDir .. "freetype/include/",
		thirdPartyDir .. "tinyutf8/",
		sourcesDir .. "MSDK/MCore/Includes/",
		sourcesDir .. "MSDK/MEngine/Includes/",
		sourcesDir .. "MSDK/MGui/Includes/",
		sourcesDir .. "Common/",
		sourcesDir
	}
	
	if os.is("windows") then
		includedirs { thirdPartyDir .. "windirent/include/" }
	end

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		
		
	-- freeimage
	include ( rootDir .. "Projects/3rdparty/freeimage" )

	-- freetype
	include ( rootDir .. "Projects/3rdparty/freetype" )
	
	-- tinycthread
	project "tinycthread"
		kind "StaticLib"
		language "C++"
		files { thirdPartyDir .. "tinycthread/**.h", 
				thirdPartyDir .. "tinycthread/**.c" }

	-- Glee
	project "glee"
		kind "StaticLib"
		language "C++"
		files { thirdPartyDir .. "glee/**.h", 
				thirdPartyDir .. "glee/**.c" }
				
	-- glfw
	include ( rootDir .. "Projects/3rdparty/glfw" )
	
	-- MCore
	project "mcore"
		kind "StaticLib"
		language "C++"
		if os.is("windows") then
			includedirs { thirdPartyDir .. "dirent/include/" }
		end
		files { sourcesDir .. "MSDK/MCore/**" }
		defines { "MCORE_STATIC" }
		

	-- MEngine
	project "mengine"
		kind "StaticLib"
		language "C++"

		files { sourcesDir .. "MSDK/MEngine/**" }
		defines { "MENGINE_STATIC" }
		links { "mcore" }
	

	-- MGui
	project "mgui"
		kind "StaticLib"
		language "C++"

		files { sourcesDir .. "MSDK/MGui/**" }
		links { "mcore", "mengine" }
		

	-- MCommon
	project "mcommon"
		kind "StaticLib"
		language "C++"
		files { sourcesDir .. "Common/Contexts/GL/**" }
		files { sourcesDir .. "Common/Loaders/FreeImage/**" }
		files { sourcesDir .. "Common/Loaders/Freetype/**" }
		files { sourcesDir .. "Common/Renderers/**" }
		files { sourcesDir .. "Common/GUI/**" }
		links { "mcore", "mengine", "mgui", "glfw", "glee", "freeimage", "tinycthread" }
		defines { "FREEIMAGE_LIB" }
		

	-- Test
	project "Test"
		kind "ConsoleApp"
		language "C++"
		
		files { "*" }
		links {
			"mcore", "mengine", "mgui", "mcommon",
			"glfw", "glee", "freeimage", "freetype", "tinycthread"
		}
		
		if os.is("macosx") then
			links {
				"Cocoa.framework", "OpenGL.framework", "CoreServices.framework",
				"Foundation.framework", "IOKit.framework"
			}
		
		elseif os.is("windows") then
			links { "Opengl32", "Winmm" }
		
		elseif os.is("linux") then
			links { "GL", "dl", "X11", "Xxf86vm" }
			linkoptions { "-Wl,-rpath=." }
		end
