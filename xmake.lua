
set_xmakever("2.5.1")

-- c code will use c99,
-- cxx code will use cxx17 currently, cxx20 soon
set_languages("c99", "cxx17")

-- 64 bit only, as SkyrimLE support is history
set_arch("x64")
add_vectorexts("ssse3")
set_warnings("all")

-- build configurations
add_rules("mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

if is_mode("debug") then
    add_defines("DEBUG")
end

if is_plat("windows") then
    add_defines("NOMINMAX")
end

-- add projects
includes("./libraries")
includes("./code")