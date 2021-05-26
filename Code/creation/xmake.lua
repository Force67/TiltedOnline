
local function build_client(name, def)
target("Creation")
    add_configfiles("BuildInfo.h.in")
    set_kind("static")
    set_group("common")
    add_defines(def)
    add_includedirs(
        ".", 
        "Fallout",
        "../", 
        "../../build", 
        {public = true})
    add_headerfiles(
        "**.h",
        "**.inl")
    set_pcxxheader("TiltedOnlinePCH.h")
    add_files("**.cpp")

    add_deps(
        "Common",
        "TiltedReverse",
        "TiltedHooks",
        "FalloutEncoding")

    add_packages(
        "glm",
        "tiltedcore", 
        "hopscotch-map", 
        "cryptopp",
        "spdlog")
end

build_client("Fallout", "TP_FALLOUT=1")
