
target("Creation")
    add_configfiles("BuildInfo.h.in")
    set_kind("static")
    set_group("common")
    add_includedirs(
        ".", 
        "Fallout",
        "../", 
        "../../build", 
        {public = true})
    add_headerfiles(
        "**.h",
        "**.inl")
    add_files("**.cpp")
    add_packages(
        "glm",
        "tiltedcore", 
        "hopscotch-map", 
        "cryptopp")
