-----------------------------------------------------------------------------------------------------------------------------
-- freetype premake build
-----------------------------------------------------------------------------------------------------------------------------

project "freetype"
	kind "StaticLib"
	language "C++"

	projectRootDir = "../../../3rdparty/freetype/"

	files {  
		projectRootDir .. "**.h",
		projectRootDir .. "*.c"
	}
	
	includedirs { 
		projectRootDir,
		projectRootDir .. "include/",
		projectRootDir .. "depend/"
	}
	
	if os.is("macosx") then
		includedirs { projectRootDir .. "mac/" }
	end

	defines { "FT2_BUILD_LIBRARY" }
