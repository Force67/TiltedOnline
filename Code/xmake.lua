add_requires("glm")

if is_plat("windows") then
    includes("client")
    includes("launcher")
    includes("tp_process")
end

includes("common")
includes("creation")
includes("server")
includes("encoding")
includes("tests")
includes("tools")
