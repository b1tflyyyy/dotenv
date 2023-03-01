workspace "Dotenv"
	architecture "x64"
	configurations { "Release" }

project "Dotenv"
	kind          "ConsoleApp"
	language      "C++"
	cppdialect    "C++17"
	staticruntime "off"

	outputdir = "%{cfg.buildcfg}"

	targetdir ("%{wks.location}/x64/%{cfg.buildcfg}")
	objdir    ("%{wks.location}/x64/%{cfg.buildcfg}")

	includedirs{ "dotenv" }
	files      { 
		"dotenv/**.cpp",
		"dotenv/**.h",
		"test/**.cpp",
		"test/**.h"
	}

	filter { "system:windows", "toolset:msc" }
		systemversion "latest"

	filter "configurations:Release"
		runtime  "Release"
		optimize "on"