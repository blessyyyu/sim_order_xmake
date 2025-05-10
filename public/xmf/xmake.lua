add_rules("mode.debug", "mode.release")

add_requires("rapidjson")

target("xmf")
    set_kind("static")
    add_includedirs("include", {public = true})
    add_files("src/*.cpp")
    add_deps("utils")
    add_packages("rapidjson")