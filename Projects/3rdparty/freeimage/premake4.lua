-----------------------------------------------------------------------------------------------------------------------------
-- freeimage premake build
-----------------------------------------------------------------------------------------------------------------------------

project "freeimage"
	kind "StaticLib"
	language "C++"

	projectRootDir = "../../../3rdparty/freeimage/"

	files { projectRootDir .. "Source/**.h", projectRootDir .. "Source/**.cpp", projectRootDir .. "Source/**.c" }
	excludes { 
		projectRootDir .. "Source/LibTIFF4/**",
		projectRootDir .. "Source/LibTIFF/tif_win3.c",
		projectRootDir .. "Source/LibTIFF/tif_win32.c",
		projectRootDir .. "Source/LibTIFF/tif_wince.c",
		projectRootDir .. "Source/LibTIFF/tif_msdos.c",
		projectRootDir .. "Source/LibTIFF/tif_atari.c",
		projectRootDir .. "Source/LibTIFF/tif_unix.c",
		projectRootDir .. "Source/LibTIFF/tif_vms.c",
		projectRootDir .. "Source/LibTIFF/tif_apple.c",
		projectRootDir .. "Source/LibTIFF/tif_acorn.c",
		projectRootDir .. "Source/LibTIFF/mkg3states.c",
		projectRootDir .. "Source/LibTIFF/mkspans.c",
		projectRootDir .. "Source/LibJPEG/ansi2knr.c",
		projectRootDir .. "Source/LibJPEG/cjpeg.c",
		projectRootDir .. "Source/LibJPEG/cdjpeg.c",
		projectRootDir .. "Source/LibJPEG/djpeg.c",
		projectRootDir .. "Source/LibJPEG/ckconfig.c",
		projectRootDir .. "Source/LibJPEG/jpegtran.c",
		projectRootDir .. "Source/LibJPEG/rdjpgcom.c",
		projectRootDir .. "Source/LibJPEG/wrjpgcom.c",
		projectRootDir .. "Source/LibJPEG/example.c",
		projectRootDir .. "Source/LibJPEG/jmemmac.c",
		projectRootDir .. "Source/LibJPEG/jmemdos.c",
		projectRootDir .. "Source/LibJPEG/jmemname.c",
		projectRootDir .. "Source/LibJPEG/jmemansi.c",
		projectRootDir .. "Source/LibPNG/pngtest.c",
		projectRootDir .. "Source/LibRawLite/**.c",
		projectRootDir .. "Source/OpenEXR/IlmThread/**Win32*",
		projectRootDir .. "Source/OpenEXR/IlmThread/**Posix*",
		projectRootDir .. "Source/OpenEXR/Half/eLut.cpp",
		projectRootDir .. "Source/OpenEXR/Half/toFloat.cpp",
		projectRootDir .. "Source/OpenEXR/IlmImf/b44ExpLogTable.cpp"
	}
	
	includedirs { 
		projectRootDir .. "Source/",
		projectRootDir .. "Source/FreeImage/",
		projectRootDir .. "Source/MetaData/",
		projectRootDir .. "Source/FreeImageToolkit/",
		projectRootDir .. "Source/LibJPEG/",
		projectRootDir .. "Source/LibMNG/",
		projectRootDir .. "Source/LibOpenJPEG/",
		projectRootDir .. "Source/LibPNG/",
		projectRootDir .. "Source/LibTIFF/",
		projectRootDir .. "Source/OpenEXR/",
		projectRootDir .. "Source/OpenEXR/IlmImf/",
		projectRootDir .. "Source/OpenEXR/Imath/",
		projectRootDir .. "Source/OpenEXR/Iex/",
		projectRootDir .. "Source/OpenEXR/Half/",
		projectRootDir .. "Source/OpenEXR/IlmThread/",
		projectRootDir .. "Source/LibRawLite/",
		projectRootDir .. "Source/ZLib/"
	}

	defines {"FREEIMAGE_LIB", "OPJ_STATIC"}
